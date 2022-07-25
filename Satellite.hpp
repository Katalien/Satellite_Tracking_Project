#pragma once
#include <iostream>
#include <string>
#include <list>
#include<vector>
#include <cmath>
#include <SGP4.h>
#include <CoordTopocentric.h>
#include <CoordGeodetic.h>
#include <Observer.h>
#include "Handle.hpp"


using namespace std;
class Handle;
using std::vector;

// Polytech
const double site_latitude = 60.00750854195759;
const double site_longtitude = 30.374288671966475;
const double site_altitude = 0;


class TrackInfo {
public:
	DateTime time;
	DateTime localTime;
	int azimuth = 0;
	int elevation = 0;
	double longitude = 0.0;
	double latitude = 0.0;
	double altitude= 0.0;
};

class PassDetails {
public:
	DateTime aos;
	DateTime los;
	double max_elevation = 0.0;
};

enum class Direction { 
	east, 
	west,
	unknown,
};

class Satellite {
public:
	Satellite() = default;
	Satellite( string const& info, string const& name);


	const Tle& GetTle() const { return *tle; };
	const string GetName() const { return name; }
	void UpdateData();
	void UpdateData(DateTime _time);

	void CreateSchedule(int const& numOfDays);
	void WriteScheduleIFile();
	void WriteScheduleIFile(string const& filename);

	int GetAzimuth() const { return info.azimuth; };
	int GetElevation() const { return info.elevation; }
	double GetAltitude() const { return info.altitude; }
	double GetLongtitude() const { return info.longitude; }
	double GetLatitude() const { return info.latitude; }
	DateTime GetTime() const { return info.time; };
	DateTime GetLocalTime() const { return info.localTime; };

	bool IsVisible();
	bool IsVisible(DateTime _time);

	DateTime GetAos() const { return passInfo.aos; };
	DateTime GetLos() const { return passInfo.los; };
	double GetMaxElevation() const { return passInfo.max_elevation; };

	void UpdatePassInfo(DateTime const& t1);

	DateTime GetHalfTime();

	double GetAzimuthByTime(DateTime const& time);
	Direction GetDirection() { return dir; };
	double GetLongitudeByTime(DateTime const& time);
	Observer GetSiteInfo() { return site; };
	double radiansToDegrees(double x);
	DateTime ToLocalTime(DateTime time) { return time.AddHours(3.0); }

private:
	const string name;
	shared_ptr<Tle> tle;
	TrackInfo info;
	CoordGeodetic site{ 0, 0, 0 };
	Observer obs{0, 0, 0};

	list<class PassDetails> pass_list;

	PassDetails passInfo{0, 0, 0};

	Direction dir = Direction::unknown;



	void UpdatePassDetails(const CoordGeodetic& user_geo,
		SGP4 const& sgp4,
		const DateTime& start_time,
		const DateTime& end_time,
		const int time_step);

	void CreatePassList(
		CoordGeodetic const& user_geo,
		SGP4 const& sgp4,
		DateTime const& start_time,
		DateTime const& end_time,
		const int time_step);

	double FindMaxElevation(
		CoordGeodetic const& user_geo,
		SGP4 const& sgp4,
		DateTime const& aos,
		DateTime const& los);


	DateTime FindCrossingPoint(
		const CoordGeodetic& user_geo,
		SGP4 const& sgp4,
		DateTime const& initial_time1,
		DateTime const& initial_time2,
		bool finding_aos);

	void DefineDirection();
	void FillInfo(CoordTopocentric const& topo, CoordGeodetic const& geo);

};