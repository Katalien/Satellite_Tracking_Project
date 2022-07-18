//#pragma once
//#define CURL_STATICLIB
//#include <iostream>
//#include <string>
//#include <SGP4.h>
//#include <list>
//#include <CoordTopocentric.h>
//#include <CoordGeodetic.h>
//#include <Observer.h>
//#include <fstream>
//#include "curl/curl.h"
//
//using namespace std;
//
//static size_t DataToString(void* contents, size_t size, size_t nmemb, void* userp) {
//    ((string*)userp)->append((char*)contents, size * nmemb);
//    return size * nmemb;
//}
//
//
//int main() {
//    CURL* curl;
//    string dataString;
//    CURLcode data;
//    char url[] = "https://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle";
//    curl = curl_easy_init();
//    if (curl) {
//        curl_easy_setopt(curl, CURLOPT_URL, url);
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DataToString);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataString);
//        data = curl_easy_perform(curl);
//        curl_easy_cleanup(curl);
//    }
//    cout << dataString;
//}