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

class Satellite;
class SatellitePredict;
class PassDetails;

class Handle {
public:
    string getTleData();

    string getSatelliteData(string const& tleData);

    void openNewFile(string const& name, DateTime const& time);

    void writeFile(string const& data);

    void writeFile(DateTime const& time, int const& azimuth, int const& elevation);

    void writeFile(DateTime const& time, int const& azimuth, int const& elevation, int const& antAz, int const& antEl);

    void closeFile();

private:
    ofstream file;

    static size_t dataToString(void* contents, size_t size, size_t nmemb, void* userp);
};