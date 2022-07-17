#pragma once
#include <iostream>
#include <string>
#include <memory>

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
		
		sat->UpdateData();
		int second = sat->GetTime().Second();
		//cout << "I am tracking " << sat->GetName() << endl;
		while (1) {
			sat->UpdateData();
			if (sat->IsVisible() == false) {
				return;
			}
			if (sat->GetTime().Second() == second) {
				if (sat->GetTime().Second() == 59) {
					while (sat->GetTime().Second() != 0) {
						sat->UpdateData();
					}
					second = -1;
				}
				port->TurnOnAngles(sat->GetAzimuth(), sat->GetElevation());
				handle.WriteFile(sat->GetLocalTime(), sat->GetAzimuth(), sat->GetElevation());
				UpdateCurrentAngles();
				// for check
				cout << sat->GetTime() << endl;
				cout << "Sat: Az: " << sat->GetAzimuth() << " El: " << sat->GetElevation() << endl;
				cout << "Ant: Az: " << azimuth << " El: " << elevation << endl;
				second++;
			}
		}
		handle.CloseFile();
	};

	void UpdateCurrentAngles() {
		elevation = port->GetElevation();
		azimuth = port->GetAzimuth();
	}

	int GetAzimuth() { return azimuth; };
	int GetElevation() { return elevation; };

	void Park() {
		port->TurnOnAngles(0, 0);
	}

	void Move(int az, int el) {
		port->TurnOnAngles(az, el);
	}

private:
	int azimuth = 0;
	int elevation = 0;
	shared_ptr<ComPort> port;
};