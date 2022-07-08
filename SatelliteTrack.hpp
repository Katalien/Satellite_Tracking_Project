#include <iostream>
#include <string>
#include <SGP4.h>
#include <CoordTopocentric.h>
#include <CoordGeodetic.h>
#include <Observer.h>
#include "Satellite.hpp"


using namespace std;

// Polytech
const double site_latitude = 60.00750854195759;
const double site_longtitude = 30.374288671966475;
const double site_altitude = 0;

class SatelliteTrack {
public:
	SatelliteTrack();
	SatelliteTrack(const Satellite* sat) : sat(sat), name(sat->GetName()) { UpdateData(); };

	void UpdateData() {
		Observer obs(site_latitude, site_longtitude, site_altitude); // 
		Observer obs2(0, 0, 0);
		CoordGeodetic site(site_latitude, site_longtitude, site_altitude);
		obs2.SetLocation(site);
		SGP4 sgp4(sat->GetTle());
		Eci eci = sgp4.FindPosition(time);;
		time = sat->GetTle().Epoch().Now();
		localTime = time.AddHours(3.0);
		CoordTopocentric topo = obs.GetLookAngle(eci);
		CoordGeodetic geo = eci.ToGeodetic();
		azimuth = topo.azimuth;
		elevation = topo.elevation;
		longtitude = geo.longitude;
		latitude = geo.latitude;
		altitude = geo.altitude;
	};

	double GetAzimuth() const { return azimuth; };
	double GetElevation() const { return elevation; }
	string GetName() const { return name; }
	double GetAltitude() const  { return altitude; }
	double GetLongtitude() const { return longtitude; }
	double GetLatitude() const { return latitude; }
	DateTime GetTime() const { return time; };
	DateTime GetLocalTime() const { return localTime; };


private:
	const Satellite*& sat;

	string name;
	DateTime time;
	DateTime localTime;
	double azimuth;
	double elevation;
	double longtitude;
	double latitude;
	double altitude;
};