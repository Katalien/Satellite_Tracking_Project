#include "Program.hpp"

/// <summary>
/// User's options
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
Program::Program(int const& argc, char** argv) : argc(argc), argv(argv) {
	desc.add_options()
		("satName", po::value<vector<string>>(&params), "Name of satellite")
		("days", po::value<int>()->default_value(3), "Amount of days for prediction")
		("predict", "Create schedule for satellite")
		("track", "track one satellite")
		("autoTrack", "track several satellites in turn")
		("longtitude", po::value<double>()->default_value(30.37428867), "longtitude of the view point")
		("latitude", po::value<double>()->default_value(60.0075085), "latitude of the view point")
		("timeZone", po::value<int>()->default_value(3), "time zone")
		("help", "Show options");
}

/// <summary>
/// run the program with certain user's command (predict, track, autoTrack or help) 
/// </summary>
void Program::run() {

	po::variables_map vm = readCmdLine(argc, argv);

	if (vm.count("help")) {
		cout << desc << endl;
		return;
	}

	shared_ptr<SatTrackInterface> trackSat = make_shared< SatTrackInterface>(params, vm["latitude"].as<double>(), vm["longtitude"].as<double>(), vm["timeZone"].as<int>());

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

/// <summary>
/// Process the parametrs of the command line
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
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


/// <summary>
/// Tracking of several satellites with automatic switching betwen them
/// </summary>
/// <param name="track"> Pointer to the object of the SatTrackInterface class with all info about the satellites, the antenna and the port </param>
void Program::autoTracking(const shared_ptr<SatTrackInterface>& track) const {
	DateTime currentTime = DateTime::Now();
	shared_ptr<Satellite> currentSat = make_shared<Satellite>();

	vector<shared_ptr<Satellite>> satList = track->getSatellites();
	vector<shared_ptr<Satellite>> visibleSatList;
	currentSat = nullptr;
	while (true) {
		currentTime = DateTime::Now();
		if (!currentSat) {
			for (auto& s : satList) {
				s->updateData();
				s->updatePassInfo(currentTime);
			}

			for (auto& sat : satList) {
				if (sat->isVisible()) {
					visibleSatList.push_back(sat);
				}
			}
			currentSat = (visibleSatList.size() == 0 ? nextSat(satList) : maxElevationSat(visibleSatList));
			
		}
		
		// set antenna in necessary position for waiting next satellite
		track->antenna->trackSatellite(currentSat);
		track->antenna->updateCurrentAngles();
	
		currentTime = DateTime::Now();

		if (currentTime >= currentSat->getLos()) {
			currentSat = nullptr;		
			visibleSatList.clear();
		}
	}
}

/// <summary>
/// Track one satellite. If it is out of view - wait fo it
/// </summary>
/// <param name="track"> Pointer to the object of the SatTrackInterface class with all info about the satellites, the antenna and the port </param>
void Program::track(const shared_ptr<SatTrackInterface>& track) const {
	vector<shared_ptr<Satellite>> satList = track->getSatellites();
	shared_ptr<Satellite> satTrack = track->getSatellite();
	while (true) {
		track->antenna->trackSatellite(satTrack);
	}
}

/// <summary>
/// Make schedule gor the satellite and write it in the file
/// </summary>
/// <param name="track"> Pointer to the object of the SatTrackInterface class with all info about the satellites, the antenna and the port </param>
/// <param name="days"> Amount of days for schedule </param>
void Program::predict(const shared_ptr<SatTrackInterface> track, int const& days) const {
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

/// <summary>
/// Find the satellite with the maximum elevation in the ealiest appearance
/// </summary>
/// <param name="satList"> vector of the pointers to the satellites </param>
/// <returns> The satellite with the max elevation </returns>
shared_ptr<Satellite> Program::maxElevationSat(const vector<shared_ptr<Satellite>>& satList) const {
	auto maxElevationSat = std::max_element(satList.begin(), satList.end(), 
		[] (shared_ptr<Satellite> const &sat_a, 
			shared_ptr<Satellite> const &sat_b) -> bool
		{
			return sat_a->getElevation() < sat_b->getElevation();
		});
	return *maxElevationSat;
}

/// <summary>
/// Choose the satellite to track further. Choose the earliest one or one with the max elevation
/// </summary>
/// <param name="satList"> vector of the pointers to the satellites </param>
/// <returns> Smart pointer to the next tracking satellite </returns>
shared_ptr<Satellite> Program::nextSat(const vector<shared_ptr<Satellite>>& satList) const {
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

