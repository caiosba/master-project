VRTrack headtracking driver for Linux V1.0

1. Basics

VRTrack is the linux headtracking driver for the vusix VR920 HMD. The driver package consists of a daemon (vrtrack) which can be run in the background and for convenience a basic control application (vrcontrol) that enables one to easily tweak the various driver settings and to callibrate the device. Of course the driver can also be controlled via command line.

The driver provides the trackingdata in different formats to the application using it. It always writes the data to /dev/headtracking. A line read from /dev/vrtrack consists of six floats that correspond a sensor reading in this format:

	yaw pitch rool x y z

Yaw, pitch and roll are angles from 0 to 360 degrees. X, y and z are always zero for the vr920, since it only supports three degrees of freedom. These values are reserved for future devices which may support six degrees of freedom, in the hope to propose a standard for tracking devices. 

The driver can scale the readings and invert the axes independantly to get the needed value range for the used application and a pleasant experience.

For maximum compatibility with existing applications there are four other modes of operation available that can be enabled separately:

1.1 joystick
The driver emulates a joystick device /dev/input/jsX. The readings for yaw, pitch and roll are the X,Y and Z axis of the emulated joystick. This may be used to enable basic headtracking support in games that do not natively support headtracking.

1.2 mouse
The driver emulates a joystick device /dev/input/mouseX. The readings for yaw and pitch are being translated to X and Y of the mouse device, so when you look right the mouse pointer moves to the right and when you look up the pointer moves upwards and vice versa.  This may also be used to enable basic headtracking support in games that do not natively support headtracking. It can also be used to just control the mouse pointer of the window system. Controlling the viewport of the window system can also be a resonable purpose. With the new MPX extension in xorg this may be possible.

1.3 UDP

In UDP mode the driver sends the tracking data via network as UDP unicast. The approach to send the data out via network makes the language used for writing the application independant from the language used for developing the driver. The packet sent to the clients contains the three angles, yaw, pitch and roll and x,y and z as 32 bit fixed point in Q16.16 format. This mode may i.e. used to control flightgear. 

1.4 multicast

In multicast mode the driver sends the tracking data via network as UDP multicast, thus many clients may read the data, which makes parallelization more possible, i.e. one could use one machine for rendering and another machine for calculations. In addition to this, the approach to send the data out via network makes the language used for writing the application independant from the language used for developing the driver. The tracking data sent to the clients contains the three angles, yaw, pitch and roll and for easy usage a viewmatrix, one can directly use with scenegraph libraries. If you intend to develop an application using the headtracking of the VR920 see the file democlient.cpp included in the download for details on how to get the data into your application. This mode is used by the stereoscopic image viewer SIV (see www.mygnu.de).


2. Commandline usage

The driver needs rw access to the /dev/cuse device, i.e. root permissions.

Usage:

usage: vrtrack [options]

options:
    --help|-h|-?                print this help message
    --device=DEVICENAME         customn device name (defaults to headtracking)
    --controldevice=DEVICENAME	 customn control device name
                                (defaults to vrcontrol)
    --multicast                 send tracking data via multicast
    --udp                       send tracking data via udp
    --mouse                     send tracking data via mouse emulation
    --joystick                  send tracking data via joystick emulation
    --scale=SCALE               scale movement (default: 1.0)
    --invert-yaw|--invert-x     invert yaw/x-axis
    --invert-pitch|--invert-y   invert pitch/y-axis
    --invert-roll|--invert-z    invert roll/z-axis
    --old-average               use old averaging algorithm
                                (for compatibility only)
    --mip=IP                    use customn ip address for udp multicasts
                                (defaults to 224.0.0.42)
    --mport=PORT                use customd port for udp multicasts
                                (defaults to 4242)
    --uip=IP                    use customn ip address for udp unicasts
                                (defaults to 127.0.0.1)
    --uport=PORT                use customd port for udp unicasts
                                (defaults to 4242)
    --callibration=CALLIB       use customn callibration file (default: ~/.vrtrack.cal)
    --configuration=CONF        use customn configuation file (default: ~/.vrtrack.conf)
    --delay=DELAY               use customn delay in main loop (default:1000)
    -f                          stay in the foreground


The options --device and --controldevice allow you to change the device nodes used by the driver. 

With the commandline switches --multicast, --udp, --mouse and --joystick you can enable the optional modes described above. 

The flags --invert-yaw, --invert-x, --invert-pitch, --invert-y, --invert-rolland  --invert-z invert the corresponding axes for different applications. I.e. inverting the y axis makes the mouse pointer in mouse mode move up when you look down.

The scale option enables you to customize the sensitivity of the tracking data to match yours and the applications needs.The --old-average  flag has the only purpose to demonstrate the benefits of the new filter algorithm in the driver over the old one and should not be used for serious use. 

With --mip, --mport, --uip, --uport you can set the network adresses and ports for UDP and multicast mode.

I.e. you may specify a multicast address and a port to send the data to. If you choose not to define these values the driver will send the tracking data to a sane default (224.0.0.42:4242). In any case, the client using the tracking data has to join the same address with the same port. You may decide not to use real network, if you only want to use the data on the same machine. In this case you may use loopback networking:

To add the route for the multicast address to the loopback interface use:  route add -host 224.0.0.42 lo

To remove the route use: route del -host 224.0.0.42 lo

With --callibration and --configuration one may specify customn callibration and configuration files. If you do so this file will also be in the users home. To use i.e. a global calibration file you have to specify the full path starting with a slash. The option --delay allows a customn delay between sensor readings. With the -f otion the driver can be forced to stay in the foreground for debugging purposes.

The commandline setting always overrides the configuration stored in ~/vrtrack.conf or the file you probably specified on the command line.


3. The control application

The control application vrcontrol can be used to configure and calibrate the driver. The most important options of the driver can be configured with it. For communication with the driver the device node /dev/vrcontrol is being used. Make sure the driver is running before executing the control application.  After executing the  control application it instantly starts to display the trackingdata. Initially it starts in normal operation mode. 

3.1 Callibration

By pressing the C key you can activate/deactivate the calibration mode. In calibration mode the control application displays the most amount of information, the angles for yaw, pitch and roll above the normalized sensor data. Below the minima/maxima of the sensors are being displayed.The calibration works similar to the calibration of vusixs windows driver, one has to turn around the device until no minima/maxima changes anymore. Afterwards turn the device forward (the direction where you want to have the center of your view) and press the Z key to set the zero direction. Once you have completed calibration press S to store the calibration for future use. Finally press the C key again to exit calibration mode.

As default the calibration is being saved to the file .vrtrack.cal in the users home, i.e. /root/.vrtrack.cal. You may specify a different filename using the -c option. If you do so this file will also be in the users home. To use i.e. a global calibration file you have to specify the full path starting with a slash.

Important note: During calibration make sure that the display of the VR920 device is displaying something. Since the displays not only showing a blue screen influences the sensor data (at least with my device) you'll end with wrong calibration else. You may use i.e. nvidia-settings to ensure this.

3.2 Configuration

In normal operations mode one may perform different configuration tasks by pressing keys.

W 	- write configuration
R 	- read configuration (resets unsaved changes)
Y	- toggle yaw inversion
P 	- toggle pitch inversion
r 	- toggle roll inversion
+/- 	- change the scale factor
J	- enable/disable joystick emulation
M	- enable/disable mouse emulation
U	- enable/disable unicast udp mode
J	- enable/disable multicast udp mode


4. Dependencies

You need to have libusb, libfuse, libconfig++ and libcurses installed on your system. You also need a recent kernel (2.6.31 or higher) for the cuse interface.

5. Building from source

For building from source unpack the zip and cd to vrtrack and run make. Make sure to have the development version of the needed libraries installed.

6. Technical notes

6.1 Needed libs

Libconfig++ is being used to store the calibration and configuration data and libcurses for the control application. Obviously libusb is neccesary to access the device.

6.2 Kernel

It is important that the linux kernel used is cuse capable, which means it has to be post 2.6.31 and the cuse module has to be loaded. CONFIG_CUSE can be found under Files Systems -> Character device in Userpace support in kernel configuration when FUSE is enabled.

6.3 Filter

The initial release used a simple averaging algorithm to smoothen the tracking data. This was far from perfect. However, the old averaging is still available dor comparison by using the --old-average parameter. As default the driver uses a better algorithm combining median and average values of the sensor readings. This gives a far more stable and smoother experience than the old method.

7 Building from source:

For building from source unpack the zip and cd to vr920 and run make / make install. If you use a binary based distribution, make sure you have the devel version of the needed libraries installed on your system.

8. Footnote

If you like the driver, feel free to link to www.mygnu.de. If you developed an application using the tracking data provided by the driver please leave a comment, because then I can review the application and eventually write about it. To request commercial licenses contact us at info(at)mygnu.de.
