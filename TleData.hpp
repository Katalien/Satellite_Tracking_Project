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

    string getSatelliteData(string const& name);

    void printTle() { cout << tleData << endl; };

private:
    string tleData;
    static size_t dataToString(void* contents, size_t size, size_t nmemb, void* userp);

};