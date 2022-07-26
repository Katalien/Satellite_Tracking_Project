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

    void downloadTleData();

    string GetSatelliteData(string const& name);

    void PrintTle() { cout << tleData << endl; };

private:
    string tleData;
    static size_t DataToString(void* contents, size_t size, size_t nmemb, void* userp);

};