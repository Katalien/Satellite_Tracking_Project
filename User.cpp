#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "ComPort.hpp"
#include "TleData.hpp"
#include "Satellite.hpp"
#include "Antenna.hpp"

using std::vector;

void Track(TleData* tle) {
	string name;
	cout << "Enter satellite:" << endl;
	getline(cin, name);
	string info = tle->GetSatelliteData(name);
	SatelliteTrack* satTrack = new SatelliteTrack(info, name);
	ComPort* port = new ComPort(L"COM3");
	port->GetConnection();
	Antenna* ant = new Antenna(port);
	ant->TrackSatellite(satTrack);
	port->ClosePort();
	delete(satTrack);
	delete(ant);
	delete(port);
}

void Predict(TleData* tle) {
	string name;
	int days;
	cout << "Enter satellite:" << endl;
	getline(cin, name);
	string info = tle->GetSatelliteData(name);
	SatelliteTrack* satPred = new SatelliteTrack(info, name);
	cout << "Enter amount of days for schedule:" << endl;
	cin >> days;
	satPred->CreateSchedule(days);
	delete(satPred);
}

void AutoTracking(TleData* tle) {
	int amount;
	ComPort* port = new ComPort(L"COM3");
	port->GetConnection();
	Antenna* antenna = new Antenna(port);
	vector<SatelliteTrack*> visible;
	SatelliteTrack* currentSat;
	cout << "Enter number of satellites" << endl;
	cin >> amount;
	SatelliteTrack** satArray = new SatelliteTrack*[amount];
	for (int i = 0; i < amount; i++) {
		string name;
		cout << i << " Enter satellite name:" << endl;
		getline(cin, name);
		string info = tle->GetSatelliteData(name);
		SatelliteTrack* satellite = new SatelliteTrack(info, name);
		satArray[i] = satellite;
	}
	while (1) {
		for (int i = 0; i < amount; ++i) {
			if (satArray[i]->IfVisible() == true) {
				visible.push_back(satArray[i]);
			}
		}

		if (visible.size() == 0) {
			antenna->Park();
		}

		if (visible.size() == 1) {
			antenna->TrackSatellite(visible.at(0));
			visible.pop_back();
		}
		if (visible.size() > 1) {
			
		}
	}
	


	for (int i = 0; i < amount; i++) {
		delete(satArray[i]);
	}
	delete(satArray);
}

int main() {
	TleData* tle(new TleData);
	Predict(tle);
	delete(tle);
	return 0;
}

//
//
///*cout << "Choose action:" << endl << "1. Get Sattelite Data" << endl << "2. Predict Sattelite Path" << endl << "3. Set up the antenna" << endl << "Enter number:" << endl;
//cin >> option;
//switch (option) {
//	case '1':
//		cout << "Satellite data:" << endl; break;
//	case '2':
//		cout << "Sattelite Path:" << endl; break;
//	case '3':
//		cout << "Set up the antenna:" << endl; break;
//}*/
////
////string str1 = "NOAA 18";
////string str2 = "1 28654U 05018A   22188.17840690  .00000122  00000+0  89763-4 0  9996";
////string str3 = "2 28654  98.9483 258.5984 0013141 275.7880  84.1794 14.12749863882855";
////
//
//
////int sec = satTrack->GetTime().Second();
////while (1) {
////	satTrack->UpdateData();
////	if (satTrack->GetTime().Second() >= sec) {
////		cout << sec << endl;
////		cout << "Lat: " << satTrack->GetLatitude() << endl << "Long: " << satTrack->GetLongtitude() << endl;
////		cout << satTrack->GetTime() << endl << "Az: " << satTrack->GetAzimuth() << endl << " El: " << satTrack->GetElevation() << endl << endl << endl;
////		sec++;
////	}
////}