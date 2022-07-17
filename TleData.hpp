#pragma once
#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <SGP4.h>
#include <list>
#include <CoordTopocentric.h>
#include <CoordGeodetic.h>
#include <Observer.h>
#include <fstream>
#include "curl/curl.h"

using namespace std;

class TleData {
public:

    TleData() { downloadTleData(); }

    void downloadTleData() {
        CURL* curl;
        string dataString;
        CURLcode data;
        char url[] = "https://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle";
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DataToString);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataString);
            data = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        tleData = dataString;
    }

    string GetSatelliteData(string name) {
        size_t pos = tleData.find(name);
        if (pos == string::npos) {
            cout << "the satellite doesn't exist" << endl << endl;
            return nullptr;
        }
        int nameSize = name.size();
        string info_tmp = tleData.substr(pos, 168);
        size_t pos_info = info_tmp.find("\n");
        string info = info_tmp.substr(pos_info + 1, 141);
        return info_tmp;
    }

    void PrintTle() { cout << tleData << endl; };

private:
    string tleData;
    static size_t DataToString(void* contents, size_t size, size_t nmemb, void* userp) {
        ((string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }


};