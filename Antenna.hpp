#include <iostream>
#include <string>
#include <memory>
#include "ComPort.hpp"
#include "SatelliteTrack.hpp"

class ComPort;

class Antenna {
public:
	Antenna(ComPort* port) : port(port) {};

	void TrackSatellite(ComPort* port, SatelliteTrack* sat)  {
		sat->UpdateData();
		int second = sat->GetTime().Second();
		while (sat->GetAzimuth() >= 0 && sat->GetElevation() >= 0) { /// how long ?
			sat->UpdateData();
			if (sat->GetTime().Second() >= second) {
				port->TurnOnAngles(sat->GetAzimuth(), sat->GetElevation());
				UpdateCurrentAngles();
				// for check
				if (azimuth == sat->GetAzimuth()) { cout << "Azimuth is fine" << endl; }
				if (elevation == sat->GetElevation()) { cout << "Elevation is fine" << endl; }
			}
		}
	};

	void UpdateCurrentAngles() {
		azimuth = port->GetAzimuth();
		elevation = port->GetElevation();
	}

	int GetAzimuth() { return azimuth; };
	int GetElevation() { return elevation; };

private:
	int azimuth;
	int elevation;
	unique_ptr<ComPort> port;
};