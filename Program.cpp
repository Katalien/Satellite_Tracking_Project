#include "Program.hpp"

// it depends on the direction of satellite movement + 
// the same algorithm, but different functions? std::function? - different logic & comparison
// store the direction in class Satellite? count it? +
// ChangeAngle function to convert 0, 1... to 360, 361... with check ? 

//function Antenna parkingAzimuth - to Antenna class?
// make function DoesCrossZero


Program::Program(int const& argc, char** argv) : argc(argc), argv(argv) {
	desc.add_options()
		("satName", po::value<vector<string>>(&params), "Name of satellite")
		("days", po::value<int>()->default_value(3), "Amount of days for prediction")
		("predict", "Create schedule for satellite")
		("track", "track one satellite")
		("autoTrack", "track several satellites in turn")
		("help", "Show options");
}

void Program::run() {

	po::variables_map vm = readCmdLine(argc, argv);

	if (vm.count("help")) {
		cout << desc << endl;
		return;
	}

	shared_ptr<SatTrackInterface> trackSat = make_shared< SatTrackInterface>(params);

	if (vm.count("predict")) {
		int days = vm["days"].as<int>();
		predict(trackSat, days);
	}
	if (vm.count("track")) {
		trackSat->connectAntena();
		track(trackSat);
	}
	if (vm.count("autoTrack")) {
		trackSat->connectAntena();
		autoTracking(trackSat);
	}
}

po::variables_map Program::readCmdLine(int const& argc, char** argv) {
	po::variables_map vm;

	// parse arguments
	po::store(po::parse_command_line(argc, argv, desc), vm);
	// check arguments

	try {
		po::notify(vm);
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		std::cout << desc << std::endl;
		return nullptr;
	}

	return vm;
}



void Program::autoTracking(const shared_ptr<SatTrackInterface>& track) {
	DateTime currentTime = DateTime::Now();
	shared_ptr<Satellite> currentSat = make_shared<Satellite>();

	vector<shared_ptr<Satellite>> satList = track->getSatellites();
	vector<shared_ptr<Satellite>> visibleSatList;
	currentSat = nullptr;
	string path = "order.txt";
	ofstream file;
	file.open(path);
	while (true) {
		currentTime = DateTime::Now();
		if (!currentSat) {
			for (auto& s : satList) {
				s->updateData();
				s->updatePassInfo(currentTime);
				cout << s->getName() << " Aos : " << s->toLocalTime(s->getAos()) << " Los : " << s->toLocalTime(s->getAos()) << endl;
			}

			for (auto& sat : satList) {
				if (sat->isVisible()) {
					visibleSatList.push_back(sat);
				}
			}
			currentSat = (visibleSatList.size() == 0 ? nextSat(satList) : maxElevationSat(visibleSatList));
			cout << "Next satellite is " << currentSat->getName() << " Aos " << currentSat->toLocalTime(currentSat->getAos()) << endl;
			
		}
		file << " Next Satellite is : " << currentSat->getName() << " Aos " << currentSat->toLocalTime(currentSat->getAos()) << " " << currentSat->getAzimuthByTime(currentSat->getAos()) << endl;
		
		// set antenna in necessary position for waiting next satellite
		track->antenna->trackSatellite(currentSat);
		track->antenna->updateCurrentAngles();
		cout << "ant az : " << track->antenna->getAzimuth() << " ant el : " << track->antenna->getElevation() << endl << endl;
		//cout << "Next satellite is " << currentSat->getName() << " Aos " << currentSat->toLocalTime(currentSat->getAos()) << endl;
	
		currentTime = DateTime::Now();

		if (currentTime >= currentSat->getLos()) {
			cout << "curSat is Over" << endl;
			cout << endl << endl << currentTime << endl;
			currentSat = nullptr;		
			visibleSatList.clear();
		}
	}
}

void Program::track(const shared_ptr<SatTrackInterface>& track) {
	vector<shared_ptr<Satellite>> satList = track->getSatellites();
	shared_ptr<Satellite> satTrack = track->getSatellite();
	while (1) {
		track->antenna->trackSatellite(satTrack);
	}
}

void Program::predict(const shared_ptr<SatTrackInterface> track, int const& days) {
	vector<shared_ptr<Satellite>> satList = track->getSatellites();
	for (auto sat : satList) {
		try {
			sat->createSchedule(days);
		}
		catch (const std::exception& ex) {
			cout <<  ex.what() << endl;
			exit(-1);
		}
		
		sat->writeScheduleIFile();
	}
}
//
//void Program::autoTracking(const shared_ptr<SatTrackInterface>& track) {
//	DateTime currentTime = DateTime::Now();
//	DateTime endTime = currentTime.AddDays(1);
//	shared_ptr<Satellite> currentSat = make_shared<Satellite>();
//
//	vector<shared_ptr<Satellite>> satList = track->getSatellites();
//	vector<shared_ptr<Satellite>> visibleSatList;
//
//	while (true) { // while: check in one day
//		currentTime = DateTime::Now();
//		for (auto &s : satList) {
//			s->updateData();
//			s->updatePassInfo(currentTime);
//		}
//
//		for (auto &sat : satList) {
//			if (sat->isVisible()) {
//				visibleSatList.push_back(sat);
//			}
//		}
//
//		currentSat = (visibleSatList.size() == 0 ? nextSat(satList) : maxElevationSat(satList));
//
//		if (!currentSat->isVisible()) {
//			currentSat->updateData();
//			track->antenna->move(currentSat->getAzimuth(), 0);
//			cout << "No Satellites right now" << endl;
//			cout << "the next one is " << currentSat->getName() << endl;
//
//			continue;
//		}
//
//		track->antenna->trackSatellite(currentSat);
//		cout << "Next satellite is " << currentSat->getName() << " Aos " << currentSat->getAos() << endl;
//	}
//}



shared_ptr<Satellite> Program::maxElevationSat(const vector<shared_ptr<Satellite>>& satList) {
	/*shared_ptr<Satellite> maxElSat = satList.at(0);
	for (int i = 1; i < satList.size(); ++i) {
		maxElSat = CompareElevation(maxElSat, satList.at(i));
	}*/
	//
	//cout << "max elevation" << endl;
	auto maxElevationSat = std::max_element(satList.begin(), satList.end(), 
		[] (shared_ptr<Satellite> const &sat_a, 
			shared_ptr<Satellite> const &sat_b) -> bool
		{
			return sat_a->getElevation() < sat_b->getElevation();
		});
	return *maxElevationSat;
}

// return next satellite
shared_ptr<Satellite> Program::nextSat(const vector<shared_ptr<Satellite>>& satList) {
	//shared_ptr<Satellite> current = satArray.at(0);
	//for (int i = 1; i < satArray.size(); ++i) {
	//	if (current->getAos() > satArray.at(i)->getAos()) {
	//		current = satArray.at(i);
	//	}
	//	if (current->getAos() == satArray.at(i)->getAos()) {
	//		current = CompareElevation(current, satArray.at(i));
	//	}
	//}
	//return current;
	//cout << "next sat" << endl;
	auto nextSat = min_element(satList.begin(), satList.end(),
		[](shared_ptr<Satellite> const& sat_first, shared_ptr<Satellite> const& sat_second) 
		{
			if (sat_first->getAos() == sat_second->getAos()) {
				return sat_first->getElevation() < sat_second->getElevation();
			}
			return sat_first->getAos() < sat_second->getAos();
		});
	return *nextSat;
}

