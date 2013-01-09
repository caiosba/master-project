import java.lang.Thread;
import java.io.IOException;

import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

class TrackingData {
	public double[] viewmatrix = new double[16];
	public double yaw;
	public double pitch;
	public double roll;
}

public class JavaDemoClient extends Thread {

	boolean running = true;
	private TrackingData trackingdata;

	public static void main(String[] args) {
		JavaDemoClient democlient = new JavaDemoClient();
		democlient.start();
		System.out.println("Democlient started.");
	}

	public static double arr2double(byte[] arr, int start) {
		int i = 0;
		int len = 8;
		int cnt = 0;
		byte[] tmp = new byte[len];
		for (i = start; i < (start + len); i++) {
			tmp[cnt] = arr[i];
			// System.out.println(java.lang.Byte.toString(arr[i]) + " " + i);
			cnt++;
		}
		long accum = 0;
		i = 0;
		for (int shiftBy = 0; shiftBy < 64; shiftBy += 8) {
			accum |= ((long) (tmp[i] & 0xff)) << shiftBy;
			i++;
		}
		return Double.longBitsToDouble(accum);
	}

	public void run() {
		MulticastSocket socket;
		try {
			socket = new MulticastSocket(4242);

			InetAddress address = InetAddress.getByName("224.0.0.42");
			socket.joinGroup(address);

			DatagramPacket packet;
			byte message[] = new byte[1024];
			while (running) {
				packet = new DatagramPacket(message, message.length);
				socket.receive(packet);
				TrackingData trackingdata = new TrackingData();
				int i = 0;
				for (i = 0; i < 16; i++) {
					trackingdata.viewmatrix[i] = arr2double(message, i * 8);
				}
				trackingdata.yaw = arr2double(message, i * 8);
				i++;
				trackingdata.pitch = arr2double(message, i * 8);
				i++;
				trackingdata.roll = arr2double(message, i * 8);

				this.trackingdata = trackingdata;

				Thread.yield();
				System.out.println("yaw: " + trackingdata.yaw + " pitch: "
						+ trackingdata.pitch + " roll: " + trackingdata.roll
						+ "\n");
			}
			// close socket on exit, gets never executed in this democlient
			// since running never gets false
			socket.leaveGroup(address);
			socket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
