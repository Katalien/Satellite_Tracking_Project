#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdarg.h>
#include "ComPort.hpp"
#include "TleData.hpp"
#include "Satellite.hpp"
#include "Antenna.hpp"

using namespace std;
using std::vector;

class SatTrackInterface {
public:
	shared_ptr<Antenna> antenna;

	SatTrackInterface(LPCTSTR portName);
	SatTrackInterface(vector<string> const& names, double siteLat, double siteLong, int timeSpan);

	vector<shared_ptr <Satellite>> getSatellites() const { return satellites; }
	shared_ptr <Satellite> getSatellite() const;
	shared_ptr <Satellite> getSatelliteByName(string const& name) const;
	int getAmount() const { return satellites.size(); };

	void connectAntena();

private:
	shared_ptr<TleData> tle;
	shared_ptr<ComPort> port;
	vector<shared_ptr <Satellite>> satellites;
};