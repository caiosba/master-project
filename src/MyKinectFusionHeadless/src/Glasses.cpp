#include "Glasses.h"

#define _CRT_SECURE_NO_DEPRECATE
#define NOMINMAX

// I know that is ugly that this is a global variable, I tried to use it as a class member
// but I was getting some strange malloc errors
std::ifstream glassesInput("glasses.out");

Glasses::Glasses(int port_number) {
  addRemovePt = false;
	useSimulatedData = false;
	port = port_number;
}

void Glasses::getYawPitchRoll() {
  char code;
  long y, p, r;

  // Read from file
	if (useSimulatedData && !glassesInput.eof()) {
    char *message;
	  string line;
    std::getline(glassesInput, line);
		message = line.c_str();
    sscanf(message, "%c %ld %ld %ld\n", &code, &y, &p, &r);
	}

	// Read from socket
	else {
	  char message[1024];
    int bytes = read(sock, message, 1024);

    if (bytes > 0) {
      message[bytes] = '\0';
	  }

    sscanf(message, "%c %ld %ld %ld\n", &code, &y, &p, &r);
  }

  if (code == 'G') {
    yaw = y;
    pitch = p;
    roll = r;
  }
}

void Glasses::initYawPitchRoll() {
	// Open socket
	if (!useSimulatedData) {
    struct sockaddr_in name;
    struct hostent *hp, *gethostbyname();

    printf("Listen activating.\n");

    /* Create socket from which to read */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)   {
      perror("Opening datagram socket\n");
      exit(EXIT_FAILURE);
    }
    
    /* Bind our local address so that the client can send to us */
    bzero((char *) &name, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    name.sin_port = htons(port);
    
    if (bind(sock, (struct sockaddr *) &name, sizeof(name))) {
      perror("Binding datagram socket\n");
      exit(EXIT_FAILURE);
    }
    
    printf("Socket has port number #%d\n", ntohs(name.sin_port));
	}
}

void Glasses::getXYZ() {
  // Automatic Initialization
  Size subPixWinSize(10,10);
	Size winSize(31,31);
  TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS, 20, 0.03);
  cap >> frame;
  if (frame.empty()) return;
  frame.copyTo(image);
  cvtColor(image, gray, COLOR_BGR2GRAY);

  if (!points[0].empty())
  {
    vector<uchar> status;
    vector<float> err;
    if (prevGray.empty()) gray.copyTo(prevGray);
    calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.001);
    Mat homo;
    try {
      homo = findHomography(points[0], points[1], CV_RANSAC, 3);
      x += homo.at<double>(0,2);
      y += homo.at<double>(1,2);
			if (yaw == 0 && pitch == 0 && roll == 0 && !initialized) {
			  xi = x;
			  yi = y;
			}
			else {
			  initialized = true;
			}
      z = homo.at<double>(2,2);
    }
    catch (Exception &e) {
      // Do nothing
    }

    size_t i, k;
    for (i = k = 0; i < points[1].size(); i++)
    {
      if (addRemovePt)
      {
        if (norm(point - points[1][i]) <= 5)
        {
          addRemovePt = false;
          continue;
        }
      }

      if (!status[i]) continue;

      points[1][k++] = points[1][i];
      circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
    }
    points[1].resize(k);
  }

  if (addRemovePt && points[1].size() < (size_t)MAX_COUNT)
  {
    vector<Point2f> tmp;
    tmp.push_back(point);
    cornerSubPix(gray, tmp, winSize, Size(-1,-1), termcrit);
    points[1].push_back(tmp[0]);
    addRemovePt = false;
  }
 
  std::swap(points[1], points[0]);
  cv::swap(prevGray, gray);
}

void Glasses::finishYawPitchRoll() {
	if (useSimulatedData) {
	  if (glassesInput.is_open()) {
	    glassesInput.close();
	  }
	}
	else {
    close(sock);
	}
}

void Glasses::initXYZ() {
  Size subPixWinSize(10,10);
  TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS, 20, 0.03);
  addRemovePt = false;
	initialized = false;

	this->zeroXYZ();

  if (useSimulatedData) {
	  cap.open("glasses1.avi");
	}
	else {
    cap.open(1);
	}

  if (!cap.isOpened())
  {
    cout << "Could not initialize capturing for optical flow!\n";
  }
	else {
    cap >> frame;
    if (frame.empty()) return;
    frame.copyTo(image);
    cvtColor(image, gray, COLOR_BGR2GRAY);
    goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 0, 0.04);
    cornerSubPix(gray, points[1], subPixWinSize, Size(-1,-1), termcrit);
	}
}

void Glasses::finishXYZ() {
  points[0].clear();
  points[1].clear();
}

void Glasses::init() {
  this->initYawPitchRoll();
	this->initXYZ();
}

void Glasses::finish() {
  this->finishYawPitchRoll();
	this->finishXYZ();
}

void Glasses::get() {
  this->getYawPitchRoll();
	this->getXYZ();
}

void Glasses::zero() {
  this->zeroYawPitchRoll();
	this->zeroXYZ();
}

void Glasses::zeroYawPitchRoll() {
  yaw = 0;
	pitch = 0;
	roll = 0;
}

void Glasses::zeroXYZ() {
  x = 0;
	y = 0;
	z = 0;
	xi = 0;
	yi = 0;
}

Glasses::~Glasses() {
 this->finish();
}
