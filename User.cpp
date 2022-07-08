#pragma once
#include <iostream>
#include <string>
#include "ComPort.hpp"
#include "PortCommand.hpp"
#include "Satellite.hpp"
#include "SatelliteTrack.hpp"

int main(){
	string name;
	char option;
	cout << "Enter satellite:" << endl;
	getline(cin, name);
	Satellite* satellite = new Satellite(name);
	SatelliteTrack* satTrack = new SatelliteTrack(satellite);
	unique_ptr<ComPort> port(new ComPort(L"COM3"));
	cout << satTrack->GetLatitude() * 180 / 3.14 << endl << satTrack->GetLongtitude() * 180 / 3.14 << endl;
	cout << satTrack->GetTime() << endl << "Az: " << satTrack->GetAzimuth() * 180 /3.14 << endl << " El: " << satTrack->GetAzimuth() * 180 / 3.14 << endl;
	
	/*port->GetConnection();
	
	port->ClosePort();*/
	delete(satellite);
	delete(satTrack);
	return 0;
}


/*cout << "Choose action:" << endl << "1. Get Sattelite Data" << endl << "2. Predict Sattelite Path" << endl << "3. Set up the antenna" << endl << "Enter number:" << endl;
cin >> option;
switch (option) {
	case '1':
		cout << "Satellite data:" << endl; break;
	case '2':
		cout << "Sattelite Path:" << endl; break;
	case '3':
		cout << "Set up the antenna:" << endl; break;
}*/