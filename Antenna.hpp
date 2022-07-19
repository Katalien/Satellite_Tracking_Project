#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "windows.h"
#include "ComPort.hpp"
#include "Satellite.hpp"
#include "Handle.hpp"

class ComPort;
class Satellite;

class Antenna {
public:
	Antenna() {};
	Antenna(shared_ptr<ComPort> port) : port(port) {};

	void TrackSatellite(shared_ptr<Satellite> sat) {
		Handle handle;
		handle.OpenNewFile(sat->GetName());
		
		sat->UpdateData();
		int second = sat->GetTime().Second();
		while (1) {
			sat->UpdateData();
			if (!sat->IsVisible()) {
				port->TurnOnAngles(sat->GetAzimuth(), 0);
				cout << sat->GetAzimuth() << " " << sat->GetElevation() << endl;
				return;
			}
		
			port->TurnOnAngles(sat->GetAzimuth(), sat->GetElevation());
			handle.WriteFile(sat->GetLocalTime(), sat->GetAzimuth(), sat->GetElevation());
			UpdateCurrentAngles();
			// for check
			cout << sat->GetTime() << endl;
			cout << "Sat: Az: " << sat->GetAzimuth() << " El: " << sat->GetElevation() << endl;
			cout << "Ant: Az: " << azimuth << " El: " << elevation << endl;
			Sleep(1000);
		}
		handle.CloseFile();
	};

	int GetAzimuth() { return azimuth; }
	int GetElevation() { return elevation; };

	void UpdateCurrentAngles() {
		elevation = port->GetElevation();
		azimuth = port->GetAzimuth();
	}


	void Park() {
		port->TurnOnAngles(0, 0);
	}

	void Move(int az, int el) {
		cout << az << endl;
		port->TurnOnAngles(az, el);
	}

private:
	int azimuth = 0;
	int elevation = 0;
	shared_ptr<ComPort> port;
};