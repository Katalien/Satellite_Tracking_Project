#pragma once
#include "Handle.hpp"

/// <summary>
/// Get all Tle data about active satellites 
/// </summary>
/// <returns>String with all Tle data</returns>
string Handle::getTleData() {
    CURL* curl;
    string dataString;
    CURLcode data;
    char url[] = "https://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle";
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dataToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataString);
        data = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    delete curl;
    return dataString;
};

/// <summary>
/// Find necessary satellite and get its tle data
/// </summary>
/// <param name="name"> Satelite name</param>
/// <returns>String with tle data of specific satellite</returns>
string Handle::getSatelliteData(string const& name) {
    string dataString = getTleData();
    size_t pos = dataString.find(name);
    if (pos == string::npos) {
        cout << "the satellite doesn't exist" << endl << endl;
        return "";
    }
    int nameSize = name.size();
    string info_tmp = dataString.substr(pos, 168);
    size_t pos_info = info_tmp.find("\n");
    string info = info_tmp.substr(pos_info + 1, 141);
    return info_tmp;
}

size_t Handle::dataToString(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/// <summary>
/// Open file. The name is the name of the satellite and its AOS time
/// </summary>
/// <param name="name"> name of the satellite </param>
/// <param name="time"> Aos time of the satellite </param>
void Handle::openNewFile(string const& name, DateTime const& time) {
    int day = time.Day();
    int month = time.Month();
    int hour = time.Hour();
    int minute = time.Minute();
    string path = "../sat_documentation/" + name + "_time_" + to_string(hour) + "h_" + to_string(minute) + "min" + ".txt";
    file.open(path);
}

/// <summary>
/// Write data in file
/// </summary>
/// <param name="data"> Data to write in the form of string </param>
void Handle::writeFile(string const& data ) {
    if (file.is_open()) {
        file << data << endl;
    }
}

/// <summary>
/// Write data about the satellite position at the moment: azimuth and elevation
/// </summary>
/// <param name="time"> current time </param>
/// <param name="azimuth"> current azimuth </param>
/// <param name="elevation"> current azimuth </param>
void Handle::writeFile(DateTime const& time, int const& azimuth, int const& elevation) {
    if (file.is_open()) {
        file << time << " " << " Az: " << azimuth << " El: " << elevation << endl;
    }
    else { cout << "file is not open" << endl; }
}

/// <summary>
/// Close file
/// </summary>
void Handle::closeFile() {
    file.close();
}