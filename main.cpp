#define CURL_STATICLIB
#include <curl\curl.h>
#include <iostream>
#include <string>
#include "Satellite.hpp"

int main() {
	string name;
	cout << "Enter satellite:" << endl;
	cin >> name;
	Satellite* s = new Satellite(name);
	/*Handler* h = new Handler;
	string info = h->GetData("2022-058A");
	cout << info << endl;*/
	s->PrintInfo();
}