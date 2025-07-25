#include <QString>
#include <fstream>
#include <iostream>
#include <string>

struct StationSettings {

	StationSettings() {
		name = "";
		latitude = "";
		longitude = "";
		altitude = "";
		timezone = "";
	}

  public:
	std::string name;
	std::string latitude;
	std::string longitude;
	std::string altitude;
	std::string timezone;
};

StationSettings parseStationSettingsFile();

bool validateStationName(const QString &name);
