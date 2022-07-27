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

	void trackSatellite(shared_ptr<Satellite> sat);

	int getAzimuth() { return azimuth; }
	int getElevation() { return elevation; };

	void updateCurrentAngles();

	void park() { port->turnOnAngles(0, 0); }

	void move(int const& az, int const& el) { port->turnOnAngles(az, el); }

private:
	int azimuth = 0;
	int elevation = 0;
	shared_ptr<ComPort> port;
	shared_ptr<Satellite> currentSat = nullptr;

	int parkAzimuthToWest(int const& aosAz, int const& losAz);

	int parkAzimuthToEast(int const& aosAz, int const& losAz);

	bool isWaiting();

	int antennaParkAzimuth();

	bool delayToWest(int const& aosAz, int const& losAz);

	bool delayToEast(int const& aosAz, int const& losAz);

	bool azimuthIsInreasing();

	bool crossSiteLongtitude();

	bool crossZero();

	bool needToConvertAngle();

	int convertAngle(int const& angle);
};