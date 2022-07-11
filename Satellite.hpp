#pragma once
#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <SGP4.h>
#include <CoordTopocentric.h>
#include <CoordGeodetic.h>
#include <Observer.h>
#include "Handle.hpp"


using namespace std;
class Handle;


class TrackInfo {
public:
	DateTime time;
	DateTime localTime;
	int azimuth;
	int elevation;
	double longtitude;
	double latitude;
	double altitude;
};


class PassDetails {
public:
	DateTime aos;
	DateTime los;
	double max_elevation;
};


class Satellite {
public:
	Satellite() {};
	Satellite(string tleData, string name);

	const Tle& GetTle() const { return *tle; };
	const string GetName() const { return name; }

protected:
	string name;
	const Tle* tle;
};

class SatelliteTrack : public Satellite {
public:
	SatelliteTrack() {};
	SatelliteTrack(string info, string name) : Satellite(info, name) { UpdateData(); };

	void UpdateData();

	void CreateSchedule(int numOfDays);

	int GetAzimuth() const { return info.azimuth; };
	int GetElevation() const { return info.elevation; }
	double GetAltitude() const { return info.altitude; }
	double GetLongtitude() const { return info.longtitude; }
	double GetLatitude() const { return info.latitude; }
	DateTime GetTime() const { return info.time; };
	DateTime GetLocalTime() const { return info.localTime; };
	int GetMaxElevation() { return GetMaxElevation(); }
	bool IfVisible();
	

private:
	TrackInfo info;

	list<class PassDetails> pass_list;

	PassDetails passInfo;

	// Polytech
	const double site_latitude = 60.00750854195759;
	const double site_longtitude = 30.374288671966475;
	const double site_altitude = 0;

	void UpdatePassDetails(const CoordGeodetic& user_geo,
		SGP4& sgp4,
		const DateTime& start_time,
		const DateTime& end_time,
		const int time_step);

	void CreatePassList(
		const CoordGeodetic& user_geo,
		SGP4& sgp4,
		const DateTime& start_time,
		const DateTime& end_time,
		const int time_step);

	double FindMaxElevation(
		const CoordGeodetic& user_geo,
		SGP4& sgp4,
		const DateTime& aos,
		const DateTime& los);


	DateTime FindCrossingPoint(
		const CoordGeodetic& user_geo,
		SGP4& sgp4,
		const DateTime& initial_time1,
		const DateTime& initial_time2,
		bool finding_aos);



};








class SatellitePredict : public Satellite {
public:

	

private:


	// Polytech
	const double site_latitude = 60.00750854195759;
	const double site_longtitude = 30.374288671966475;
	const double site_altitude = 0;

    
};