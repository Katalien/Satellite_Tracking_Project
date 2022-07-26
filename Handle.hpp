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
    string GetTleData();

    string GetSatelliteData(string const& tleData);

    void OpenNewFile(string const& name, DateTime const& time);

    void WriteFile(string const& data);

    void WriteFile(DateTime const& time, int const& azimuth, int const& elevation);

    void WriteFile(DateTime const& time, int const& azimuth, int const& elevation, int const& antAz, int const& antEl);

    void CloseFile();

private:
    ofstream file;

    static size_t DataToString(void* contents, size_t size, size_t nmemb, void* userp);
};