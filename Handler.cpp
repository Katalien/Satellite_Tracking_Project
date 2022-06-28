#include "Handler.hpp"

size_t Handler::DataToString(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string Handler::Input() {
    string name;
    cout << "Enter satellite name:" << endl;
    cin >> name;
    cout << endl;
    return name;
}


string Handler::GetData(string name) {
    CURL* curl;
    string dataString;
    CURLcode data;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://celestrak.com/NORAD/elements/gp.php?GROUP=last-30-days&FORMAT=tle");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DataToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataString);
        data = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    size_t pos = dataString.find(name);
    int nameSize = name.size();
    string tmp_info_string = dataString.substr(pos + nameSize, DATASIZE);
    size_t pos_info = tmp_info_string.find("1");
    string info = tmp_info_string.substr(pos_info, DATASIZE - pos_info);
    return info;
}
