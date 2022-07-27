#include "TleData.hpp"

/// <summary>
/// Get data about satellites from calestrack and write in string
/// </summary>
void TleData::downloadTleData() {
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
    tleData = dataString;
}

/// <summary>
/// Get TLE data of the specific satellite by its name
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
string TleData::getSatelliteData(string const& name) {
    size_t pos = tleData.find(name);
    if (pos == string::npos) {
        throw exception("The satellite doesn't exist\n");
    }
    string info_tmp = tleData.substr(pos, 168);
    size_t pos_info = info_tmp.find("\n");
    string info = info_tmp.substr(pos_info + 1, 141);
    return info;
}

size_t TleData::dataToString(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}