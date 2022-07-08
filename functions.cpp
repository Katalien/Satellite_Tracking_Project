// used @dnwrnr sgp4 library "https://github.com/dnwrnr/sgp4"

#define CURL_STATICLIB
#include <curl\curl.h>
#include <iostream>
#include <string>
#include <ctime>
#include <CoordTopocentric.h>
#include <CoordGeodetic.h>
#include <Observer.h>
#include <SGP4.h>
#pragma warning(disable:4996)

using namespace std;


byte TLESIZE = 182;


static size_t DataToString(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


string GetTleData() {
    CURL* curl;
    string dataString;
    CURLcode data;
    char file_name[] = "tle_data.txt";
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://celestrak.org/NORAD/elements/gp.php?GROUP=last-30-days&FORMAT=tle");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DataToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataString);
        data = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return dataString;
}

string Input() {
    string name;
    cout << "Enter satellite name:" << endl;
    getline(cin, name);
    cout << endl;
    return name;
}

Tle CreateSatTle(string dataString, string name) {

    size_t pos = dataString.find(name);
    if (pos == string::npos) {
        cout << "the satellite doesn't exist" << endl << endl;
    }
    int nameSize = name.size();
    string info_tmp = dataString.substr(pos, 168);
    size_t pos_info = info_tmp.find("\n");
    string info = info_tmp.substr(pos_info + 1, 141);
    cout << info << endl;

    string str1 = info_tmp.substr(0, 25);
    string str2 = info_tmp.substr(26, 69);
    string str3 = info_tmp.substr(97, 69);

    Tle tle = Tle(str1, str2, str3);
    return tle;
}

double RadianToDegree(const double val) {
    return val * 180 / 3.141592;
}

void PrintSatLocation(CoordGeodetic geo) {
    cout << endl << " Longtitude " << RadianToDegree(geo.longitude) << " Latitude " << RadianToDegree(geo.latitude) << endl << endl;
}

void PrintLookAngles(DateTime dt, CoordTopocentric topo) {
    cout << dt << endl << " Az:" << RadianToDegree(topo.azimuth) << " El:" << RadianToDegree(topo.elevation) << endl;
}

void GiveCommand(CoordTopocentric topo) {
    cout << "W" << RadianToDegree(topo.azimuth) << " " << RadianToDegree(topo.elevation) << endl;
}

int main() {
    string dataString = GetTleData();
    cout << dataString << endl;
    string name = Input();
    Tle tle = CreateSatTle(dataString, name);

        
    const double site_latitude = 60.00750854195759;
    const double site_longtitude = 30.374288671966475;
    const double site_altitude = 0; //?
    Observer obs(site_latitude, site_longtitude, site_altitude);
    SGP4 sgp4(tle);

    std::cout << tle << std::endl;
    DateTime dt_tmp = tle.Epoch().Now();
    int curSecond = dt_tmp.Second();
    cout << curSecond << endl;
    while (true) {
        DateTime dt = tle.Epoch().Now();
        if (dt.Second() >= curSecond) {
            Eci eci = sgp4.FindPosition(dt);
            CoordTopocentric topo = obs.GetLookAngle(eci);
            CoordGeodetic geo = eci.ToGeodetic();
            //PrintLookAngles(dt, topo);    
            GiveCommand(topo);
            ++curSecond;
        }
    }    
    return 0;
}

