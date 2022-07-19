#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <boost/program_options.hpp>
#include "SatTrackInterface.hpp"

using std::vector;
namespace po = boost::program_options;

class Program {
public:

	Program(int argc, char** argv);
	void Run();
	po::variables_map ReadCmdLine(int argc, char** argv);
	void Predict(const shared_ptr<SatTrackInterface> track, int days);
	void Track(const shared_ptr<SatTrackInterface>& track);
	void AutoTracking(const shared_ptr<SatTrackInterface>& track);

private:
	int argc;
	char** argv;
	vector<string> params;
	po::options_description desc{ "Options" };
	shared_ptr<Satellite> NextSat(const vector<shared_ptr<Satellite>>& satArray);
	shared_ptr<Satellite> MaxElevationSat(const vector<shared_ptr<Satellite>>& satList);
};