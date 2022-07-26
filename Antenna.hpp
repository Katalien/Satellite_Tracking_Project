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
	Antenna() = default;
	Antenna(shared_ptr<ComPort> port) : port(port) {};

	void TrackSatellite(shared_ptr<Satellite> sat);

	int GetAzimuth() { return azimuth; }
	int GetElevation() { return elevation; };

	void UpdateCurrentAngles();

	void Park() { port->TurnOnAngles(0, 0); }

	void Move(int const& az, int const& el) { port->TurnOnAngles(az, el); }

private:
	int azimuth = 0;
	int elevation = 0;
	shared_ptr<ComPort> port;
	shared_ptr<Satellite> currentSat = nullptr;

	int ParkAzimuthToWest(int const& aosAz, int const& losAz);

	int ParkAzimuthToEast(int const& aosAz, int const& losAz);

	bool IsWaiting();

	int AntennaParkAzimuth();

	bool DelayToWest(int const& aosAz, int const& losAz);

	bool DelayToEast(int const& aosAz, int const& losAz);

	bool AzimuthIsInreasing();

	bool CrossSiteLongtitude();

	bool CrossZero();

	bool NeedToConvertAngle();

	int ConvertAngle(int const& angle);
};