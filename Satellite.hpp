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
	double azimuth = 0;
	double elevation = 0;
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
	Satellite( string const& info, string const& name, double siteLat, double siteLong, int timeSpan);


	const Tle& getTle() const { return *tle; };
	const string getName() const { return name; }
	void updateData();

	void createSchedule(int const& numOfDays);
	void writeScheduleIFile();
	void writeScheduleIFile(string const& filename);

	double getAzimuth() const { return info.azimuth; };
	double getElevation() const { return info.elevation; }
	double getAltitude() const { return info.altitude; }
	double getLongtitude() const { return info.longitude; }
	double getLatitude() const { return info.latitude; }
	DateTime getTime() const { return info.time; };
	DateTime getLocalTime() const { return info.localTime; };

	bool isVisible();

	DateTime getAos() const { return passInfo.aos; };
	DateTime getLos() const { return passInfo.los; };
	double getMaxElevation() const { return passInfo.max_elevation; };

	void updatePassInfo(DateTime const& t1);

	DateTime GetHalfTime();

	double getAzimuthByTime(DateTime const& time);
	Direction getDirection() { return dir; };
	double getLongitudeByTime(DateTime const& time);
	Observer getSiteInfo() { return site; };
	double radiansToDegrees(double x);
	DateTime toLocalTime(DateTime time) { return time.AddHours((double)timeSpan); }

private:
	const string name;
	shared_ptr<Tle> tle;
	TrackInfo info;
	CoordGeodetic site{ 0, 0, 0 };
	Observer obs{0, 0, 0};
	int timeSpan;

	list<class PassDetails> passList;

	PassDetails passInfo{0, 0, 0};

	Direction dir = Direction::unknown;



	void updatePassDetails(const CoordGeodetic& user_geo,
		SGP4 const& sgp4,
		const DateTime& start_time,
		const DateTime& end_time,
		const int time_step);

	void createPassList(
		CoordGeodetic const& user_geo,
		SGP4 const& sgp4,
		DateTime const& start_time,
		DateTime const& end_time,
		const int time_step);

	double findMaxElevation(
		CoordGeodetic const& user_geo,
		SGP4 const& sgp4,
		DateTime const& aos,
		DateTime const& los);


	DateTime findCrossingPoint(
		const CoordGeodetic& user_geo,
		SGP4 const& sgp4,
		DateTime const& initial_time1,
		DateTime const& initial_time2,
		bool finding_aos);

	void defineDirection();
};