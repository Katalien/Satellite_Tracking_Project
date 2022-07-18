#include "Program.hpp"

Program::Program(int argc, char** argv) : argc(argc), argv(argv) {
	desc.add_options()
		("s", po::value<vector<string>>(&params), "Name of satellite")
		("days", po::value<int>()->default_value(3), "Amount of days for prediction")
		("predict", "Create schedule for satellite")
		("track", "Track one satellite")
		("autoTrack", "Track several satellites in turn")
		("help", "Show options");
}

void Program::Run() {

	po::variables_map vm = ReadCmdLine(argc, argv);

	if (vm.count("help")) {
		cout << desc << endl;
		return;
	}
	
	shared_ptr<SatTrackInterface> track = make_shared< SatTrackInterface>(params);

	if(vm.count("predict")){
		int days = vm["days"].as<int>();
		Predict(track, days);
	}
	if (vm.count("track")) {
		track->ConnectAntena();
		Track(track);
	}
	if (vm.count("autoTrack")) {
		track->ConnectAntena();
		AutoTracking(track);
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
	while (1) {
		satTrack->UpdateData();
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
			s->UpdateData();
			s->UpdatePassInfo(currentTime);
		}

		for (auto sat : satList) {
			if (sat->IsVisible() == true) {
				visibleSatList.push_back(sat);
			}
		}

		currentSat = (visibleSatList.size() == 0 ? NextSat(satList) : MaxElevationSat(satList));

		if (currentSat->IsVisible() == false) {
			currentSat->UpdateData();
			track->antenna->Move(currentSat->GetAzimuth(), 0);
			cout << "No Satellites right now" << endl;
			cout << "the next one is " << currentSat->GetName() << endl;

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
	/*shared_ptr<Satellite> maxElSat = satList.at(0);
	for (int i = 1; i < satList.size(); ++i) {
		maxElSat = CompareElevation(maxElSat, satList.at(i));
	}*/
	//
	auto maxElevationSat = std::max_element(satList.begin(), satList.end(), 
		[] (shared_ptr<Satellite> const &sat_a, 
			shared_ptr<Satellite> const &sat_b) -> bool
		{
			return sat_a->GetElevation() < sat_b->GetElevation();
		});
	return *maxElevationSat;
}

// return next satellite
shared_ptr<Satellite> Program::NextSat(const vector<shared_ptr<Satellite>>& satList) {
	//shared_ptr<Satellite> current = satArray.at(0);
	//for (int i = 1; i < satArray.size(); ++i) {
	//	if (current->GetAos() > satArray.at(i)->GetAos()) {
	//		current = satArray.at(i);
	//	}
	//	if (current->GetAos() == satArray.at(i)->GetAos()) {
	//		current = CompareElevation(current, satArray.at(i));
	//	}
	//}
	//return current;
	auto nextSat = min_element(satList.begin(), satList.end(),
		[](shared_ptr<Satellite> const& sat_first, shared_ptr<Satellite> const& sat_second) 
		{
			if (sat_first->GetAos() == sat_second->GetAos()) {
				return sat_first->GetElevation() < sat_second->GetElevation();
			}
			return sat_first->GetAos() < sat_second->GetAos();
		});
	return *nextSat;
}
