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
	SatTrackInterface(int amount, ...);

	SatTrackInterface(vector<string> const& names);

	void ConnectAntena();

	vector<shared_ptr <Satellite>> GetSatellites() { return satellites; }
	int GetAmount() { return satellites.size(); };

	shared_ptr <Satellite> GetSatellite() const;

	shared_ptr <Satellite> GetSatelliteByName(string const& name) const;

private:
	shared_ptr<TleData> tle;
	shared_ptr<ComPort> port;
	vector<shared_ptr <Satellite>> satellites;
};