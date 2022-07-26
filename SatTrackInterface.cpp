#include "SatTrackInterface.hpp"

SatTrackInterface::SatTrackInterface(LPCTSTR portName) {
	shared_ptr<Satellite> sat = make_shared<Satellite>();
	satellites.push_back(sat);
}

SatTrackInterface::SatTrackInterface(int amount, ...) {
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
}

SatTrackInterface::SatTrackInterface(vector<string> const& names) {
	tle = make_shared<TleData>();
	for (auto& name : names) {
		string info;
		try {
			info = tle->GetSatelliteData(name);
			satellites.emplace_back(make_shared<Satellite>(info, name));
		}
		catch (const std::exception& ex) {
			cout << ex.what() << " Check the entered data " << endl;
			exit(-1);
		}
	}
}

void SatTrackInterface::ConnectAntena() {
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

shared_ptr<Satellite> SatTrackInterface::GetSatellite() const {
	if (satellites.size() == 1) {
		return satellites.at(0);
	}
	return nullptr;
}

shared_ptr <Satellite> SatTrackInterface::GetSatelliteByName(string const& name) const {
	for (auto& sat : satellites) {
		if (sat->GetName() == name) {
			return sat;
		}
	}
	return nullptr;
}
