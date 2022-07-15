//#pragma once
//#include <iostream>
//#include <string>
//#include <vector>
//#include <memory>
//#include "ComPort.hpp"
//#include "TleData.hpp"
//#include "Satellite.hpp"
//#include "Antenna.hpp"
//
//using std::vector;
//
//// парсер командной строки
//// класс связи спутника, антенны, порта
//// std::optional
//
//shared_ptr<Satellite> MaxElevationSat(const shared_ptr<Satellite>& s1, const shared_ptr<Satellite>& s2) {
//	if (s1->GetMaxElevation() >= s2->GetMaxElevation()) {
//		return s1;
//	}
//	else {
//		return s2;
//	}
//}
//
//// return next satellite
//shared_ptr<Satellite> NextSat(const vector<shared_ptr<Satellite>>& satArray) {
//	shared_ptr<Satellite> current = satArray.at(0);
//	for (int i = 1; i < satArray.size(); ++i) {
//		if (current->GetAos() > satArray.at(i)->GetAos()) {
//			current = satArray.at(i);
//		}
//		if (current->GetAos() == satArray.at(i)->GetAos()) {
//			current = MaxElevationSat(current, satArray.at(i));
//		}
//	}
//	return current;
//}
//
//void AutoTracking(const shared_ptr<TleData>& tle) {
//	int amount;
//	DateTime currentTime = DateTime::Now();
//	DateTime endTime = currentTime.AddDays(1);
//	shared_ptr<ComPort> port = make_shared<ComPort>(L"COM3");
//	port->GetConnection();
//	shared_ptr<Antenna> antenna = make_shared<Antenna>(port);
//	shared_ptr<Satellite> currentSat = make_shared<Satellite>();
//	cout << "Enter number of satellites" << endl;
//	cin >> amount;
//	vector<shared_ptr<Satellite>> satArray;
//	for (int i = 0; i < amount; i++) {
//		string name;
//		cout << i << " Enter satellite name:" << endl;
//		cin >> name;
//		string info = tle->GetSatelliteData(name);
//		shared_ptr<Satellite> satellite = make_shared<Satellite>(info, name);
//		satArray.push_back(satellite);
//	}
//
//	while (true) { // while: check in one day
//		currentTime = DateTime::Now();
//		for (int i = 0; i < amount; ++i) {
//			satArray.at(i)->UpdatePassInfo(currentTime);
//		}
//		currentSat = NextSat(satArray);
//		antenna->TrackSatellite(currentSat);
//
//	}
//}
//
//void Track(const shared_ptr<TleData>& tle) {
//	string name;
//	cout << "Enter satellite:" << endl;
//	getline(cin, name);
//	string info = tle->GetSatelliteData(name);
//	shared_ptr<Satellite> satTrack = make_shared<Satellite>(info, name);
//	satTrack->UpdateData();
//	shared_ptr<ComPort> port = make_shared<ComPort>(L"COM3");
//	port->GetConnection();
//	shared_ptr<Antenna> antenna = make_shared<Antenna>(port);
//	antenna->TrackSatellite(satTrack);
//	port->ClosePort();
//}
//
//void Predict(const shared_ptr<TleData>& tle) {
//	string name;
//	int days;
//	cout << "Enter satellite:" << endl;
//	getline(cin, name);
//	string info = tle->GetSatelliteData(name);
//	shared_ptr<Satellite> satPred = make_shared<Satellite>(info, name);
//	cout << "Enter amount of days for schedule:" << endl;
//	cin >> days;
//	satPred->CreateSchedule(days);
//}
//
//int main(int argc, char* argv[]) {
//	//shared_ptr<TleData> tle = make_shared<TleData>();
//	//Track(tle);
//	for (int i = 0; i < argc; i++) {
//		cout <<"Hello world" << endl;
//	}
//	return 0;
//}
//
////
////
/////*cout << "Choose action:" << endl << "1. Get Sattelite Data" << endl << "2. Predict Sattelite Path" << endl << "3. Set up the antenna" << endl << "Enter number:" << endl;
////cin >> option;
////switch (option) {
////	case '1':
////		cout << "Satellite data:" << endl; break;
////	case '2':
////		cout << "Sattelite Path:" << endl; break;
////	case '3':
////		cout << "Set up the antenna:" << endl; break;
////}*/
//////
//////string str1 = "NOAA 18";
//////string str2 = "1 28654U 05018A   22188.17840690  .00000122  00000+0  89763-4 0  9996";
//////string str3 = "2 28654  98.9483 258.5984 0013141 275.7880  84.1794 14.12749863882855";
//////
////
////
//////int sec = satTrack->GetTime().Second();
//////while (1) {
//////	satTrack->UpdateData();
//////	if (satTrack->GetTime().Second() >= sec) {
//////		cout << sec << endl;
//////		cout << "Lat: " << satTrack->GetLatitude() << endl << "Long: " << satTrack->GetLongtitude() << endl;
//////		cout << satTrack->GetTime() << endl << "Az: " << satTrack->GetAzimuth() << endl << " El: " << satTrack->GetElevation() << endl << endl << endl;
//////		sec++;
//////	}
//////}
//
////
////
////while (1) {
////	for (int i = 0; i < amount; ++i) {
////		if (satArray[i]->IfVisible() == true) {
////			visible.push_back(satArray[i]);
////		}
////		satArray[i]->CreateSchedule(10); // ? num of days
////	}
////
////	if (visible.size() == 0) {
////		antenna->Park();
////	}
////
////	if (visible.size() == 1) {
////		antenna->TrackSatellite(visible.at(0));
////		visible.pop_back();
////	}
////
////}