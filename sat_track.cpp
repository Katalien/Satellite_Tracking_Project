//// used @dnwrnr sgp4 library "https://github.com/dnwrnr/sgp4"
//
//#define CURL_STATICLIB
//#include <curl\curl.h>
//#include <iostream>
//#include <string>
//#include <ctime>
//#include <CoordTopocentric.h>
//#include <CoordGeodetic.h>
//#include <Observer.h>
//#include <SGP4.h>
//#pragma warning(disable:4996)
//
//using namespace std;
//
//
////std::byte TLESIZE = 182;
//
//
//static size_t DataToString(void* contents, size_t size, size_t nmemb, void* userp) {
//    ((string*)userp)->append((char*)contents, size * nmemb);
//    return size * nmemb;
//}
//
//
//string GetTleData() {
//    CURL* curl;
//    string dataString;
//    CURLcode data;
//    char file_name[] = "tle_data.txt";
//    curl = curl_easy_init();
//    char url[] = "https://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle";
//    if (curl) {
//        curl_easy_setopt(curl, CURLOPT_URL, url);
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DataToString);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataString);
//        data = curl_easy_perform(curl);
//        curl_easy_cleanup(curl);
//    }
//    return dataString;
//}
//
//int main() {
//    string dataString = GetTleData();
//    cout << dataString << endl;
//    while (true) {
//        string name;
//        cout << "Enter satellite name:" << endl;
//        getline(cin, name);
//        cout << endl;
//        size_t pos = dataString.find(name);
//        if (pos == string::npos) {
//            cout << "the satellite doesn't exist" << endl << endl;
//            continue;
//        }
//        int nameSize = name.size();
//        string info_tmp = dataString.substr(pos, 168);
//        size_t pos_info = info_tmp.find("\n");
//        string info = info_tmp.substr(pos_info + 1, 141);
//        cout << info << endl;
//
//        string str1 = info_tmp.substr(0, 25);
//        string str2 = info_tmp.substr(26, 69);
//        string str3 = info_tmp.substr(97, 69);
//
//        Tle tle = Tle(str1, str2, str3);
//        const double site_latitude = 60.00750854195759;
//        const double site_longtitude = 30.374288671966475;
//        const double site_altitude = 0; 
//        Observer obs(site_latitude, site_longtitude, site_altitude);
//
//        SGP4 sgp4(tle);
//
//        std::cout << tle << std::endl;
//        time_t* t;
//        time_t curtime = time(0);
//        t = &curtime;
//        tm* curtm = gmtime(t);
//        tm* tm2 = localtime(&curtime);
//
//        DateTime dt = tle.Epoch().Now();
//        Eci eci = sgp4.FindPosition(dt);
//        CoordTopocentric topo = obs.GetLookAngle(eci);
//        CoordGeodetic geo = eci.ToGeodetic();
//
//        cout << dt << endl << " Az:" << topo.azimuth * 180 / 3.141592 << " El:" << topo.elevation * 180 / 3.141592 << endl;
//        cout << endl << " Longtitude " << geo.longitude * 180 / 3.141592 << " Latitude " << geo.latitude * 180 / 3.141592 << endl << endl;
//        cin.clear();
//    }
//    return 0;
//}
//
//
