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
struct PassDetails;

class Handle {
public:
    string GetTleData();

    string GetSatelliteData(string tleData);

    void OpenNewFile(string name);

    void WriteFile( string data);

    void WriteFile(DateTime time, int azimuth, int elevation);

    void CloseFile();

private:
    ofstream file;

    static size_t DataToString(void* contents, size_t size, size_t nmemb, void* userp);
};