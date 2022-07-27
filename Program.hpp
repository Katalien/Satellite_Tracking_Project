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
	Program() = default;
	Program(int const& argc, char** argv);
	void run();
	po::variables_map readCmdLine(int const& argc, char** argv);
	void predict(const shared_ptr<SatTrackInterface> track, int const& days);
	void track(const shared_ptr<SatTrackInterface>& track);
	void autoTracking(const shared_ptr<SatTrackInterface>& track);

private:
	int argc;
	char** argv;
	vector<string> params;
	po::options_description desc{ "Options" };
	shared_ptr<Satellite> nextSat(const vector<shared_ptr<Satellite>>& satArray);
	shared_ptr<Satellite> maxElevationSat(const vector<shared_ptr<Satellite>>& satList);
};