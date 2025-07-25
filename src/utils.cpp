#include "cpp_qt-gui/utils.h"
#include <QString>
#include <fstream>
#include <iostream>
#include <string>

bool validateStationName(const QString &name) {
	return !name.trimmed().isEmpty();
}

StationSettings parseStationSettingsFile() {
	std::ifstream file("$HOME/.kaskad-scada/networkinteraction.ini");
	if (!file.is_open()) {
		std::cerr << "Could not open the file!" << std::endl;
		return 1;
	}

	StationSettings settings;
	std::vector<StationSettings> stations;

	std::string line;
	while (std::getline(file, line)) {
		settings.stationName = line;
	}

	file.close();
	return 0;
}
