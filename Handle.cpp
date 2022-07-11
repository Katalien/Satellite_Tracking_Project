#pragma once
#include "Handle.hpp"



/// <summary>
/// Get all Tle data about active satellites 
/// </summary>
/// <returns>String with all Tle data</returns>
string Handle::GetTleData() {
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

/// <summary>
/// Find necessary satellite and get its tle data
/// </summary>
/// <param name="name"> Satelite name</param>
/// <returns>String with tle data of specific satellite</returns>
string Handle::GetSatelliteData(string name) {
    string dataString = GetTleData();
   // cout << dataString << endl;
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

size_t Handle::DataToString(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void Handle::OpenNewFile(string name) {
    string path = "C:/Users/EZinyakova/source/repos/Satellite_New/sat_track_documentation/" + name + ".txt";
    file.open(path);
}

void Handle::WriteFile( string data ) {
    if (file.is_open()) {
        file << data << endl;
    }
}

void Handle::WriteFile(DateTime time, int azimuth, int elevation) {
    if (file.is_open()) {
        file << time << " " << " Az: " << azimuth << " El: " << elevation << endl;
    }
    else { cout << "file is not open" << endl; }
}
//
//void WriteFile(list<struct PassDetails>* pass_list) {
//    std::stringstream ss;
//
//    ss << std::right << std::setprecision(1) << std::fixed;
//
//    std::list<struct PassDetails>::const_iterator itr = pass_list.begin();
//    do
//    {
//        ss << "AOS: " << itr->aos.AddHours(3.0)
//            << ", LOS: " << itr->los.AddHours(3.0)
//            << ", Max El: " << std::setw(4) << Util::RadiansToDegrees(itr->max_elevation)
//            << ", Duration: " << (itr->los - itr->aos)
//            << std::endl << std::endl;
//    } while (++itr != pass_list.end());
//
//    std::cout << ss.str();
//}
//
void Handle::CloseFile() {
    file.close();
}