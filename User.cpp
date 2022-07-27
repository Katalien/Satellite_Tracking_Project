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

void f(int x = 4) {
	cout << x << endl;
}

int main(int argc, char* argv[]) {
	shared_ptr<Program> program = make_shared<Program>(argc, argv);
	program->run();
	return 0;
}