//#pragma once
//#include <iostream>
//#include <string>
//#include "ComPort.hpp"
//#include "PortCommand.hpp"
//#include "SatelliteTrack.hpp"
//
//int main() {
//	ComPort* port = new ComPort(L"COM3");
//	port->GetConnection();
//	port->SetConnectionParams();
//	double az = 015.0;
//	double el = 020.0;
//	port->TurnOnAngles(az, el);
//	while (true) {
//		port->PrintCurrentAngles();
//	}
//	port->ClosePort();
//	delete(port);
//
//
//	string name;
//	cin >> name;
//	SatelliteTrack* sat = new SatelliteTrack(name);
//	return 0;
//}