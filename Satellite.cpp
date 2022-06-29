#include "Satellite.hpp"


void Satellite::ParseTLEString(string info) {
	number = stoi(info.substr(number_start - 1, number_end - number_start + 1));
	first_der = stod(info.substr(first_der_start - 1, first_der_end - first_der_start + 1));
	second_der = stod(info.substr(second_der_start - 1, second_der_end - second_der_start + 1));
	slow_koeff = stod(info.substr(slow_koeff_start - 1, slow_koeff_end + 1));
	inclane = stod(info.substr(inclane_start - 1, inclane_end + 1));
	longitude = stod(info.substr(longitude_start - 1, longitude_end + 1));
	eccentricity = stod(info.substr(eccentricity_start - 1, eccentricity_end + 1));
	pericenter = stod(info.substr(pericenter_start - 1, pericenter_end + 1));
	anomaly = stod(info.substr(anomaly_start - 1, anomaly_end + 1));
	rotation = stod(info.substr(rotation_start - 1, rotation_end + 1));
	round_num = stod(info.substr(round_num_start - 1, round_num_end + 1));
}

void Satellite::PrintInfo() {
	cout << "number: " << number << endl;
	cout << "first_der: " << first_der << endl;
	cout << "second_der: " << second_der << endl;
	cout << "slow_koeff: " << slow_koeff << endl;
	cout << "inclane: " << inclane << endl;
	cout << "longitude: " << longitude << endl;
	cout << "eccentricity: " << eccentricity << endl;
	cout << "pericenter: " << pericenter << endl;
	cout << "anomaly: " << anomaly << endl;
	cout << "rotation: " << rotation << endl;
	cout << "round_num: " << round_num << endl;
}