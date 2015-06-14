//============================================================================
// Name        : test_cspice.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "SpiceUsr.h"

using namespace std;

#ifndef PI
	#define PI	3.14159265358979
#endif
#ifndef D2R
	#define D2R	0.01745329251994327	//(PI/180.)
#endif
#ifndef R2D
	#define R2D	57.2957795130823799	//(180./PI)
#endif


int main() {
//	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	printf("\n");


	ConstSpiceChar *versn;

	versn = tkvrsn_c("TOOLKIT");

	cout << "Toolkit version " << versn << endl;

	SpiceDouble et;
	SpiceChar utc[32];
	SpiceDouble   mat[3][3];
	SpiceDouble   radii[3];
	SpiceInt      n;

	SpiceDouble   state[6];
	SpiceDouble   lt;

	furnsh_c( "/home/sshuh/workspace/test_cspice/Debug/naif0011.tls" );	// Load the leapseconds kernel naif0008.tls
	furnsh_c( "/home/sshuh/workspace/test_cspice/Debug/pck00010.tpc" );
	furnsh_c ( "/home/sshuh/workspace/test_cspice/Debug/de405.bsp" );	// Load the planetary ephemeris SPK file de405s.bsp.
	furnsh_c ( "/home/sshuh/workspace/test_cspice/Debug/mgs_ext22.bsp" );

	//	convert UTC to ET
//	str2et_c( "2020-SEP-22 20:13:55", &et );
	utc2et_c( "2020-09-22T20:15:00", &et );

	SpiceDouble earth_orient[3] = {0.0, 0.0, 0.0};
	SpiceDouble u_vec[3] = {1.0, 0.0, 0.0};


//	add 1 day to ET and convert it back to UTC

//	FILE *fp;
//	fp = fopen("b.txt", "w");

//	for( int i=0; i<365; i++ )
//	{
	//	timout_c( et+spd_c(), "YYYY-DOYTHR:MN:SC.### ::RND", 32, utc );
		//timout_c( et, "YYYY-MM-DD T HR:MN:SC.### ::RND", 32, utc );

	//	spkezr_c( "MGS", et, "J2000", "NONE", "MARS", state, &lt );
//		spkezr_c( "SUN", et, "J2000", "NONE", "EARTH", state, &lt );
//		spkezr_c( "MOON", et, "J2000", "NONE", "EARTH", state, &lt );


		SpiceDouble spd86400 = spd_c(); // seconds per day

		SpiceChar* utcstr;
		et2utc_c ( et, "J", 6, 35, utcstr );
		printf("\n%s  J2000=%f\n", utcstr, j2000_c() );

		printf("\nJulian Date=%.5f\n\n", et/spd86400 );


//		SpiceDouble spref = ( jed - j2000_c() ) * spd_c();


		printf("[Earth]\n");

		spkezr_c( "EARTH", et, "J2000", "NONE", "SUN", state, &lt );
//		fprintf(fp, "%.0f %.0f %.0f ", state[0], state[1], state[2]);
		printf("Earth Pos in Solar coordinate= %.3f %.3f %.3f [km]\n"
				"     Vel= %.3f %.3f %.3f [km/s]\n"
				"     theta=%.3f [deg]\n",
				state[0], state[1], state[2], state[3], state[4], state[5],
				atan2(state[1], state[0])*R2D );

//		pxform_c( "J2000", "IAU_EARTH", et, mat );
		pxform_c( "IAU_EARTH", "J2000", et, mat );
		earth_orient[0] = mat[0][0]*u_vec[0] + mat[0][1]*u_vec[1] + mat[0][2]*u_vec[2];
		earth_orient[1] = mat[1][0]*u_vec[0] + mat[1][1]*u_vec[1] + mat[1][2]*u_vec[2];
		earth_orient[2] = mat[2][0]*u_vec[0] + mat[2][1]*u_vec[1] + mat[2][2]*u_vec[2];
//		fprintf(fp, "%f %f %f ", earth_orient[0], earth_orient[1], earth_orient[2]);

		double earth_spherical[2];
		earth_spherical[0] = atan2( earth_orient[1], earth_orient[0] );
		earth_spherical[1] = atan2( earth_orient[2], sqrt(earth_orient[0]*earth_orient[0]+earth_orient[1]*earth_orient[1]) );

		printf( "Earth_orientation vector = (%f %f %f)\n     RA= %f  DEC= %f [deg]\n",
				earth_orient[0], earth_orient[1], earth_orient[2],
				earth_spherical[0]*R2D + 360.0, earth_spherical[1]*R2D);

		// ----------------------------------------------------------------
		printf("\n");
		spkezr_c( "MOON", et, "J2000", "NONE", "EARTH", state, &lt );
//		fprintf(fp, "%.0f %.0f %.0f ", state[0], state[1], state[2]);
		printf( "Moon %.0f %.0f %.0f \n", state[0], state[1], state[2]);

		spkezr_c( "SUN", et, "J2000", "NONE", "EARTH", state, &lt );
//		fprintf(fp, "%.0f %.0f %.0f ", state[0], state[1], state[2]);
		printf( "SUN %.0f %.0f %.0f \n", state[0], state[1], state[2]);


//		spkezr_c( "VENUS", et, "J2000", "NONE", "SUN", state, &lt );
//		fprintf(fp, "%.0f %.0f %.0f ", state[0], state[1], state[2]);

//		spkezr_c( "MARS", et, "J2000", "NONE", "SUN", state, &lt );
//		fprintf(fp, "%.0f %.0f %.0f ", state[0], state[1], state[2]);

//		fprintf(fp, "\n");
		printf("\n");

	//	printf("%.2f %.3f %.3f %.3f %.3f %.3f %.3f\n", et, state[0], state[1], state[2], state[3], state[4], state[5]);


		//bodvrd_c( "EARTH", "RADII", 3, &n, radii );
		//printf("n %d  radii(%f %f %f)\n", n, radii[0], radii[1], radii[2]);


//		et = et + spd_c();
//	}
//
//	fclose(fp);

//	furnsh_c("mykernels.furnsh");
	return 0;
}
