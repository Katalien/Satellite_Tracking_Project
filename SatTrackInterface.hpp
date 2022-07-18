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
		shared_ptr<Satellite> sat = make_shared<Satellite>();
		satellites.push_back(sat);
	};

	SatTrackInterface(int amount, ...) {
		tle = make_shared<TleData>();
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

	SatTrackInterface(vector<string> const& names) {
		tle = make_shared<TleData>();
		for (auto& name : names) {
			string info;
			try {
				info = tle->GetSatelliteData(name);
			}
			catch (const std::exception& ex) {
				cout << "Check the entered data" << endl;
				exit(-1);
			}
			satellites.emplace_back(make_shared<Satellite>(info, name));
			
		}
	};

	void ConnectAntena() {
		port = make_shared<ComPort>(L"COM3");
		try {
			port->GetConnection();
		}
		catch (const std::exception& ex) {
			cout << ex.what() << endl;
			exit(-1);
		}		
		antenna = make_shared<Antenna>(port);
	}

	void CheckAntennaConnection() {
		try {
			antenna->UpdateCurrentAngles();
		}
		catch (const std::exception& ex) {
			cout << ex.what() << endl;
			cout << "No connection to antenna" << endl;
			exit(-1);
		}
	}

	vector<shared_ptr <Satellite>> GetSatellites() { return satellites; }
	int GetAmount() { return satellites.size(); };

	shared_ptr <Satellite> GetSatellite() {
		if (satellites.size() == 1) {
			return satellites.at(0);
		}
		return nullptr;
	}

	shared_ptr <Satellite> GetSatelliteByName(string name) {
		for (auto &sat : satellites) {
			if (sat->GetName() == name) {
				return sat;
			}
		}
	}

	

private:
	shared_ptr<TleData> tle;
	shared_ptr<ComPort> port;
	vector<shared_ptr <Satellite>> satellites;
};