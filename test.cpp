//#define CURL_STATICLIB
//#include <curl\curl.h>
//#include <iostream>
//#include <string>
//
//using namespace std;
//
//byte DATASIZE = 159;
//
//
//static size_t DataToString(void* contents, size_t size, size_t nmemb, void* userp) {
//    ((string*)userp)->append((char*)contents, size * nmemb);
//    //return size * nmemb;
//}
//
//int main() {
//    CURL* curl;
//    string dataString;
//    CURLcode data;
//    curl = curl_easy_init();
//    if (curl) {
//        curl_easy_setopt(curl, CURLOPT_URL, "https://celestrak.com/NORAD/elements/gp.php?GROUP=last-30-days&FORMAT=tle");
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DataToString);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataString);
//        data = curl_easy_perform(curl);
//        curl_easy_cleanup(curl);
//    }
//    cout << dataString << endl;
//    string name;
//    cout << "Enter satellite name:" << endl;
//    cin >> name;
//    cout << endl;
//    size_t pos = dataString.find(name);
//    int nameSize = name.size();
//    cout << nameSize << endl;
//    string info = dataString.substr(pos + nameSize,  DATASIZE );
//    cout << info << endl;
//    return 0;
//}
