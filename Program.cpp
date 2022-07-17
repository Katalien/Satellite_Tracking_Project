#include "Program.hpp"

Program::Program(int argc, char** argv) : argc(argc), argv(argv) {
	desc.add_options()
		("predict", po::value<std::string>(), "Create schedule for satellite")
		("track", po::value<std::string>(), "Track one satellite")
		("autoTrack", po::value<std::string>(), "Track several satellites in turn")
		("s", po::value<vector<string>>(&params), "Name of satellite")
		("days", po::value<int>(), "Name of satellite")
		("am", po::value<int>()->required(), "Name of satellite")
		("help", "Show options");
}

void Program::Run() {
	po::variables_map vm = ReadCmdLine(argc, argv);
	
	if (vm.count("help")) {
		cout << desc << endl;
	}
	int days = vm["days"].as<int>();
	int amount = vm["am"].as<int>();
	const shared_ptr<SatTrackInterface> track = make_shared< SatTrackInterface>(amount, params);

	if(vm.count("predict")){
		Predict(track, days);
	}
}

po::variables_map Program::ReadCmdLine(int argc, char** argv) {
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

void Program::Track(const shared_ptr<SatTrackInterface>& track) {
	vector<shared_ptr<Satellite>> satList = track->GetSatellites();
	shared_ptr<Satellite> satTrack = track->GetSatellite();
	satTrack->UpdateData();
	while (1) {
		track->antenna->TrackSatellite(satTrack);
	}
}

void Program::Predict(const shared_ptr<SatTrackInterface> track, int days) {
	vector<shared_ptr<Satellite>> satList = track->GetSatellites();
	for (auto sat : satList) {
		sat->CreateSchedule(days);
		sat->WriteScheduleIFile();
	}
}

void Program::AutoTracking(const shared_ptr<SatTrackInterface>& track) {
	DateTime currentTime = DateTime::Now();
	DateTime endTime = currentTime.AddDays(1);
	shared_ptr<Satellite> currentSat = make_shared<Satellite>();

	vector<shared_ptr<Satellite>> satList = track->GetSatellites();
	vector<shared_ptr<Satellite>> visibleSatList;

	while (true) { // while: check in one day
		currentTime = DateTime::Now();
		for (auto s : satList) {
			s->UpdatePassInfo(currentTime);
		}

		for (auto sat : satList) {
			if (sat->IsVisible() == true) {
				visibleSatList.push_back(sat);
			}
		}

		currentSat = (visibleSatList.size() == 0 ? NextSat(satList) : MaxElevationSat(satList));

		if (currentSat->IsVisible() == false) {
			track->antenna->Move(currentSat->GetAzimuth(), 0);
			continue;
		}

		track->antenna->TrackSatellite(currentSat);
		cout << "Next satellite is " << currentSat->GetName() << " Aos " << currentSat->GetAos() << endl;
	}
}

shared_ptr<Satellite> Program::CompareElevation(const shared_ptr<Satellite>& s1, const shared_ptr<Satellite>& s2) {
	if (s1->GetMaxElevation() >= s2->GetMaxElevation()) {
		return s1;
	}
	else {
		return s2;
	}
}

shared_ptr<Satellite> Program::MaxElevationSat(const vector<shared_ptr<Satellite>>& satList) {
	shared_ptr<Satellite> maxElSat = satList.at(0);
	for (int i = 1; i < satList.size(); ++i) {
		maxElSat = CompareElevation(maxElSat, satList.at(i));
	}
	return maxElSat;
}

// return next satellite
shared_ptr<Satellite> Program::NextSat(const vector<shared_ptr<Satellite>>& satArray) {
	shared_ptr<Satellite> current = satArray.at(0);
	for (int i = 1; i < satArray.size(); ++i) {
		if (current->GetAos() > satArray.at(i)->GetAos()) {
			current = satArray.at(i);
		}
		if (current->GetAos() == satArray.at(i)->GetAos()) {
			current = CompareElevation(current, satArray.at(i));
		}
	}
	return current;
}
