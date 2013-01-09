/*
 # (C) 2009-2011 Jürgen Löb
 # jloeb@main-host.de
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 # See the Attribution-NonCommercial-ShareAlike 3.0 Unported for more details.
 #
 */

struct TrackingData {
	double viewmatrix[16];
	double yaw;
	double pitch;
	double roll;
	double x;
	double y;
	double z;
};

struct Vec3d {
	double x;
	double y;
	double z;
};
