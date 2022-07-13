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
		if (sat->GetAzimuth() < 0 || sat->GetElevation() < 0) {
			cout << "Satellite is out of view " << endl;
			return;
		}
		sat->UpdateData();
		int second = sat->GetTime().Second();
		while (1) {
			sat->UpdateData();
			if (sat->GetTime().Second() == second) {
				if (sat->GetTime().Second() == 59) {
					while (sat->GetTime().Second() != 0) {
						sat->UpdateData();
					}
					second = -1;
				}
				//port->TurnOnAngles(sat->GetAzimuth(), sat->GetElevation());
				//handle.WriteFile(sat->GetLocalTime(), sat->GetAzimuth(), sat->GetElevation());
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
		azimuth = port->GetAzimuth();
		elevation = port->GetElevation();
	}

	int GetAzimuth() { return azimuth; };
	int GetElevation() { return elevation; };

	void Park() {
		port->TurnOnAngles(0, 0);
	}

private:
	int azimuth = 0;
	int elevation = 0;
	shared_ptr<ComPort> port;
};