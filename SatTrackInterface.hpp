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

	SatTrackInterface(LPCTSTR portName) {
		port = make_shared<ComPort>(portName);
		port->GetConnection();
		antenna = make_shared<Antenna>(port);
		shared_ptr<Satellite> sat = make_shared<Satellite>();
		satellites.push_back(sat);
	};

	SatTrackInterface(int amount, ...) {
		tle = make_shared<TleData>();
		tle->PrintTle();
		port = make_shared<ComPort>(L"COM3");
		port->GetConnection();
		antenna = make_shared<Antenna>(port);
		va_list list;
		va_start(list, amount);
		for (int i = 0; i < amount; ++i) {
			string name = va_arg(list, string);
			string info = tle->GetSatelliteData(name);
			shared_ptr<Satellite> sat = make_shared<Satellite>(info, name);
			satellites.push_back(sat);
		}
		va_end(list);
	};

	SatTrackInterface(vector<string> names) {
		port = make_shared<ComPort>(L"COM3");
		port->GetConnection();
		antenna = make_shared<Antenna>(port);
		
		for (auto name : names) {
			string info = tle->GetSatelliteData(name);
			shared_ptr<Satellite> sat = make_shared<Satellite>(info, name);
			satellites.push_back(sat);
		}

	};

	vector<shared_ptr <Satellite>> GetSatellites() { return satellites; }
	int GetAmount() { return satellites.size(); };

	shared_ptr <Satellite> GetSatellite() {
		if (satellites.size() == 1) {
			return satellites.at(0);
		}
	}

	shared_ptr <Satellite> GetSatelliteByName(string name) {
		for (auto sat : satellites) {
			if (sat->GetName() == name) {
				return sat;
			}
		}
	}

	~SatTrackInterface() {
		port->ClosePort();
	}

private:
	shared_ptr<TleData> tle;
	shared_ptr<ComPort> port;
	vector<shared_ptr <Satellite>> satellites;
};