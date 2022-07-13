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


// Polytech
const double site_latitude = 60.00750854195759;
const double site_longtitude = 30.374288671966475;
const double site_altitude = 0;


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
	Satellite(const string info, string name);


	const Tle& GetTle() const { return *tle; };
	const string GetName() const { return name; }
	void UpdateData();

	void CreateSchedule(int numOfDays);

	int GetAzimuth() const { return info.azimuth; };
	int GetElevation() const { return info.elevation; }
	double GetAltitude() const { return info.altitude; }
	double GetLongtitude() const { return info.longtitude; }
	double GetLatitude() const { return info.latitude; }
	DateTime GetTime() const { return info.time; };
	DateTime GetLocalTime() const { return info.localTime; };

	bool IfVisible();

	DateTime GetAos() const { return passInfo.aos; };
	DateTime GetLos() const { return passInfo.los; };
	double GetMaxElevation() const { return passInfo.max_elevation; };

	void UpdatePassInfo(DateTime t1) {
		if (pass_list.empty() == true) {
			CreateSchedule(1);
		}
		list<PassDetails>::const_iterator it = pass_list.begin();
		while (t1 >= it->los) {
			DeletePassesTime();
			it = pass_list.begin();
		}
		passInfo.aos = it->aos;
		passInfo.los = it->los;
		passInfo.max_elevation = it->max_elevation;
	}


private:
	const string name;
	shared_ptr<Tle> tle;
	TrackInfo info;

	list<class PassDetails> pass_list;

	PassDetails passInfo;

	void DeletePassesTime() {
		pass_list.pop_front();
	}

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