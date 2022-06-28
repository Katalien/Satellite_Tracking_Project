#define CURL_STATICLIB
#include <curl\curl.h>
#include <iostream>
#include <string>

using namespace std;



class Handler {
public:
	string GetData(string name);
private:
	string info;
	byte DATASIZE = 159;
	
	static size_t DataToString(void* contents, size_t size, size_t nmemb, void* userp);
	string Input();
};