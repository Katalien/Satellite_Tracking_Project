#include "Satellite.hpp"
#include <limits>

using std::string;

double Satellite::radiansToDegrees(double x) {
	return (x * 180 / 3.14159265359);
}

Satellite::Satellite(string const& info, string const& name) : name(name) {
    site = { site_latitude, site_longtitude, site_altitude };
    obs.SetLocation(site);
    std::string addTab(25 - name.size(), ' ');
	string str1 = name + addTab;
	string str2 = info.substr(0, 69);
	string str3 = info.substr(71, 69);
    tle = make_shared<Tle>(str1, str2, str3);
    UpdateData();
    DefineDirection();
}

void Satellite::UpdateData() {
	info.time = GetTle().Epoch().Now();
	SGP4 sgp4(GetTle());
	Eci eci = sgp4.FindPosition(info.time);
	info.localTime = info.time.AddHours(3.0);
	CoordTopocentric topo = obs.GetLookAngle(eci);
	CoordGeodetic geo = eci.ToGeodetic();
    info.azimuth = radiansToDegrees(topo.azimuth);
    info.elevation = radiansToDegrees(topo.elevation);
    info.longitude = radiansToDegrees(geo.longitude);
    info.latitude = radiansToDegrees(geo.latitude);
    info.altitude = radiansToDegrees(geo.altitude);
};

void Satellite::UpdatePassInfo(DateTime const& t1) {
    if (pass_list.empty()) {
        CreateSchedule(1);
    }
    list<PassDetails>::const_iterator it = pass_list.begin();
    while (t1 >= it->los) {
        pass_list.pop_front();
        it = pass_list.begin();
    }
    passInfo.aos = it->aos;
    passInfo.los = it->los;
    passInfo.max_elevation = it->max_elevation;
}

// дублироавние кода - вынести часть в отдельную функцию
double Satellite::GetAzimuthByTime(DateTime const& time) {
    // здесь нужна проверка, что данные заполнены
    SGP4 sgp4(GetTle());
    Eci eci = sgp4.FindPosition(time);
    CoordTopocentric topo = obs.GetLookAngle(eci);
    return radiansToDegrees(topo.azimuth);
}

double Satellite::GetLongitudeByTime(DateTime const& time) {
    SGP4 sgp4(GetTle());
    Eci eci = sgp4.FindPosition(time);
    CoordTopocentric topo = obs.GetLookAngle(eci);
    CoordGeodetic geo = eci.ToGeodetic();
    return radiansToDegrees(geo.longitude);
}


double Satellite::FindMaxElevation(
    CoordGeodetic const& user_geo,
    SGP4 const& sgp4,
    DateTime const& aos,
    DateTime const& los)
{
    Observer obs(user_geo);

    bool running;

    double time_step = (los - aos).TotalSeconds() / 9.0;
    DateTime current_time(aos); //! current time
    DateTime time1(aos); //! start time of search period
    DateTime time2(los); //! end time of search period
    double max_elevation; //! max elevation

    running = true;

    do {
        running = true;
        //max_elevation = -99999999999999.0;
        max_elevation = (-1) * std::numeric_limits<double>::infinity();
        while (running && current_time < time2) {
            /*
             * find position
             */
            Eci eci = sgp4.FindPosition(current_time);
            CoordTopocentric topo = obs.GetLookAngle(eci);

            if (topo.elevation > max_elevation) {
                /*
                 * still going up
                 */
                max_elevation = topo.elevation;
                /*
                 * move time along
                 */
                current_time = current_time.AddSeconds(time_step);
                if (current_time > time2) {
                    /*
                     * dont go past end time
                     */
                    current_time = time2;
                }
            }
            else {
                /*
                 * stop
                 */
                running = false;
            }
        }

        /*
         * make start time to 2 time steps back
         */
        time1 = current_time.AddSeconds(-2.0 * time_step);
        /*
         * make end time to current time
         */
        time2 = current_time;
        /*
         * current time to start time
         */
        current_time = time1;
        /*
         * recalculate time step
         */
        time_step = (time2 - time1).TotalSeconds() / 9.0;
    } while (time_step > 1.0);

    return max_elevation;
}

DateTime Satellite::FindCrossingPoint(
    const CoordGeodetic& user_geo,
    SGP4 const& sgp4,
    const DateTime& initial_time1,
    const DateTime& initial_time2,
    bool finding_aos)
{
    Observer obs(user_geo);

    bool running;
    int cnt;

    DateTime time1(initial_time1);
    DateTime time2(initial_time2);
    DateTime middle_time;

    running = true;
    cnt = 0;
    while (running && cnt++ < 16) {
        middle_time = time1.AddSeconds((time2 - time1).TotalSeconds() / 2.0);
        /*
         * calculate satellite position
         */
        Eci eci = sgp4.FindPosition(middle_time);
        CoordTopocentric topo = obs.GetLookAngle(eci);

        if (topo.elevation > 0.0) {
            /*
             * satellite above horizon
             */
            if (finding_aos) {
                time2 = middle_time;
            }
            else {
                time1 = middle_time;
            }
        }
        else {
            if (finding_aos) {
                time1 = middle_time;
            }
            else {
                time2 = middle_time;
            }
        }

        if ((time2 - time1).TotalSeconds() < 1.0) {
            /*
             * two times are within a second, stop
             */
            running = false;
            /*
             * remove microseconds
             */
            int us = middle_time.Microsecond();
            middle_time = middle_time.AddMicroseconds(-us);
            /*
             * step back into the pass by 1 second
             */
            middle_time = middle_time.AddSeconds(finding_aos ? 1 : -1);
        }
    }

    /*
     * go back/forward 1second until below the horizon
     */
    running = true;
    cnt = 0;
    while (running && cnt++ < 6) {
        Eci eci = sgp4.FindPosition(middle_time);
        CoordTopocentric topo = obs.GetLookAngle(eci);
        if (topo.elevation > 0) {
            middle_time = middle_time.AddSeconds(finding_aos ? -1 : 1);
        }
        else {
            running = false;
        }
    }
    return middle_time;
}


void Satellite::UpdatePassDetails( CoordGeodetic const& user_geo,
    SGP4 const& sgp4,
    const DateTime& start_time,
    const DateTime& end_time,
    const int time_step)
{
    Observer obs(user_geo);

    DateTime aos_time;
    DateTime los_time;


    DateTime previous_time(start_time);
    DateTime current_time(start_time);

    bool found_aos = false;

    bool end_of_pass = false;

    Eci eci = sgp4.FindPosition(current_time);
    CoordTopocentric topo = obs.GetLookAngle(eci);

    if (!found_aos && topo.elevation > 0.0) {
        if (start_time == current_time) {
 
            aos_time = start_time;
        }
        else {
            aos_time = FindCrossingPoint(
                user_geo,
                sgp4,
                previous_time,
                current_time,
                true);
        }
        found_aos = true;
    }
    else if (found_aos && topo.elevation < 0.0) {
        found_aos = false;
        end_of_pass = true;
        los_time = FindCrossingPoint(
            user_geo,
            sgp4,
            previous_time,
            current_time,
            false);

        PassDetails pd;
        passInfo.aos = aos_time;
        passInfo.los = los_time;
        passInfo.max_elevation = FindMaxElevation(
            user_geo,
            sgp4,
            aos_time,
            los_time);
    }
}


void Satellite::CreatePassList(
    const CoordGeodetic& user_geo,
    SGP4 const& sgp4,
    const DateTime& start_time,
    const DateTime& end_time,
    const int time_step)
{
    Observer obs(user_geo);

    DateTime aos_time;
    DateTime los_time;

    bool found_aos = false;

    DateTime previous_time(start_time);
    DateTime current_time(start_time);

    while (current_time < end_time) {
        bool end_of_pass = false;

        /*
         * calculate satellite position
         */
        Eci eci = sgp4.FindPosition(current_time);
        CoordTopocentric topo = obs.GetLookAngle(eci);

        if (!found_aos && topo.elevation > 0.0) {
            /*
             * aos hasnt occured yet, but the satellite is now above horizon
             * this must have occured within the last time_step
             */
            if (start_time == current_time) {
                /*
                 * satellite was already above the horizon at the start,
                 * so use the start time
                 */
                aos_time = start_time;
            }
            else {
                /*
                 * find the point at which the satellite crossed the horizon
                 */
                aos_time = FindCrossingPoint(
                    user_geo,
                    sgp4,
                    previous_time,
                    current_time,
                    true);
            }
            found_aos = true;
        }
        else if (found_aos && topo.elevation < 0.0) {
            found_aos = false;
            /*
             * end of pass, so move along more than time_step
             */
            end_of_pass = true;
            /*
             * already have the aos, but now the satellite is below the horizon,
             * so find the los
             */
            los_time = FindCrossingPoint(
                user_geo,
                sgp4,
                previous_time,
                current_time,
                false);

            PassDetails pd;
            pd.aos = aos_time;
            pd.los = los_time;
            pd.max_elevation = FindMaxElevation(
                user_geo,
                sgp4,
                aos_time,
                los_time);

            pass_list.push_back(pd);
        }

        /*
         * save current time
         */
        previous_time = current_time;

        if (end_of_pass) {
            /*
             * at the end of the pass move the time along by 30mins
             */
            current_time = current_time + TimeSpan(0, 30, 0);
        }
        else {
            /*
             * move the time along by the time step value
             */
            current_time = current_time + TimeSpan(0, 0, time_step);
        }

        if (current_time > end_time) {
            /*
             * dont go past end time
             */
            current_time = end_time;
        }
    };

    if (found_aos) {
        /*
         * satellite still above horizon at end of search period, so use end
         * time as los
         */
        PassDetails pd;
        pd.aos = aos_time;
        pd.los = end_time;
        pd.max_elevation = FindMaxElevation(user_geo, sgp4, aos_time, end_time);

        pass_list.push_back(pd);
    }
}

void Satellite:: WriteScheduleIFile() {
    string path = "../sat_documentation/schedule_" + name + ".txt";
    ofstream file;
    file.open(path);

    auto itr = pass_list.cbegin();
    do {
        file << "AOS: " << itr->aos.AddHours(3.0)
            << " AosAz: " << GetAzimuthByTime(itr->aos)
            << ", LOS: " << itr->los.AddHours(3.0)
            << " LosAz: " << GetAzimuthByTime(itr->los)
            << ", Max El: " << std::setw(4) << Util::RadiansToDegrees(itr->max_elevation)
            << ", Duration: " << (itr->los - itr->aos)
            << std::endl << std::endl;
    } while (++itr != pass_list.end());
    file.close();
}


void Satellite::WriteScheduleIFile(string const& filename) {
    ofstream file;
    file.open(filename);

    auto itr = pass_list.cbegin();
    do {
        file << "AOS: " << itr->aos.AddHours(3.0)
            << ", LOS: " << itr->los.AddHours(3.0)
            << ", Max El: " << std::setw(4) << Util::RadiansToDegrees(itr->max_elevation)
            << ", Duration: " << (itr->los - itr->aos)
            << std::endl << std::endl;
    } while (++itr != pass_list.end());
    file.close();
}


void Satellite::CreateSchedule(int const& numOfDays) {
    DateTime start_date = DateTime::Now(true);
    DateTime end_date(start_date.AddDays((double)numOfDays));
    CoordGeodetic geo(site_latitude, site_longtitude, site_altitude);
    SGP4 sgp4(GetTle());
    CreatePassList(geo, sgp4, start_date, end_date, 180);
    if (pass_list.begin() == pass_list.end()) {
        throw exception("No passes found");
    }
}

bool Satellite::IsVisible(){
    UpdateData();
    return info.azimuth >= 0 && info.elevation >= 0;
}

void Satellite::DefineDirection() {
    double first = GetLongtitude();
    Sleep(1000);
    UpdateData();
    double second = GetLongtitude();
    dir = first > second ? Direction::west : Direction::east;
}

DateTime Satellite::GetHalfTime() {
    int end = GetLos().Minute();
    int start = GetAos().Minute();
    int half = (end - start) / 2 % 60;
    return GetAos().AddMinutes(half);
}