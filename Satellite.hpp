#include <iostream>
#include "Handler.hpp"
#include "TLE_CONSTANTS.hpp"

class Satellite {
public:
	Satellite(string name) {
		string info = handler->GetData(name);
		cout << info << endl;
		//cout << info.substr(2, 5);
		number = stoi(info.substr(number_start-1, number_end - number_start+1));
		first_der = stod(info.substr(first_der_start-1, first_der_end - first_der_start+1));
		second_der = stod(info.substr(second_der_start-1, second_der_end - second_der_start+1));
	}

	void PrintInfo() {
		cout << number << endl;
		cout << first_der << endl;
		cout << second_der << endl;
	}

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
};