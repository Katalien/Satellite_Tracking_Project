#pragma once
#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <SGP4.h>
#include <CoordTopocentric.h>
#include <CoordGeodetic.h>
#include <Observer.h>
#include "curl/curl.h"

class Satellite;

class Handle {
public :
    string GetTleData() {
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
        return dataString;
    };

    string GetSatelliteData(string name) {
        string dataString = GetTleData();
        size_t pos = dataString.find(name);
        if (pos == string::npos) {
            cout << "the satellite doesn't exist" << endl << endl;
            return nullptr;
        }
        int nameSize = name.size();
        string info_tmp = dataString.substr(pos, 168);
        size_t pos_info = info_tmp.find("\n");
        string info = info_tmp.substr(pos_info + 1, 141);
        return info_tmp;
    }

private:
    static size_t DataToString(void* contents, size_t size, size_t nmemb, void* userp) {
        ((string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
};