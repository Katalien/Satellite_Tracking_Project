#include <iostream>
#include "Handler.hpp"
#include "TLE_CONSTANTS.hpp"

class Satellite {
public:
	Satellite(string name) {
		string info = handler->GetData(name);
		ParseTLEString(info);
	}

	void PrintInfo();

private:
	Handler* handler = new Handler;
	int number;
	double first_der;
	double second_der;
	double slow_koeff;
	double inclane;
	double longitude;
	double eccentricity;
	double pericenter;
	double anomaly;
	double rotation;
	double round_num;

	void ParseTLEString(string info);
};