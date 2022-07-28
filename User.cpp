#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <boost/program_options.hpp>
#include "SatTrackInterface.hpp"
#include "ComPort.hpp"
#include "TleData.hpp"
#include "Satellite.hpp"
#include "Antenna.hpp"
#include "Program.hpp"

using std::vector;

using namespace std;

namespace po = boost::program_options;


// порт дает текущее положение антенны даже если он выключен? что делать?
// site
// обновдение данных - стирать пред значения

int main(int argc, char* argv[]) {
	auto program = make_shared<Program>(argc, argv);
	program->run();
	return 0;
}