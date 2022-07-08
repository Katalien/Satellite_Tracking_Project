#pragma once
#include <iostream>
#include <string>
#include <SGP4.h>
#include <CoordTopocentric.h>
#include <CoordGeodetic.h>
#include <Observer.h>
#include "Handle.hpp"

using namespace std;

class Satellite {
public:
	Satellite() {};
	Satellite(string name): name(name) {
		Handle handle;
		string info = handle.GetSatelliteData(name);
		string str1 = info.substr(0, 25);
		string str2 = info.substr(26, 69);
		string str3 = info.substr(97, 69);
		tle = new Tle(str1, str2, str3);
	}

	const Tle& GetTle() const { return *tle; };
	const string GetName() const { return name; }

private:
	string name;
	const Tle* tle;
	// Should I store tle or fill all data?

};