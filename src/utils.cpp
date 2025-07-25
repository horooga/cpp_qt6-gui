#include "cpp_qt-gui/utils.h"
#include <QString>
#include <cstdlib>
#include <fstream>
#include <string>

bool validateStationName(const QString &name) {
	return !name.trimmed().isEmpty();
}

std::pair<std::string, std::string> parseValue(const std::string &line) {
	int delimPos = line.find('=');
	return {line.substr(0, delimPos), line.substr(delimPos + 1)};
}

std::vector<Station> parseConfig() {
	const char *home = std::getenv("HOME");
	std::ifstream file(std::string(home) +
					   "/.kaskad-scada/networkinteraction.ini");
	if (!file.is_open()) {
		return {};
	}

	Station station;
	std::vector<Station> stations;

	std::string line;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}
		if (!line.find('=')) {
			station.name = line;
			stations.push_back(station);
			continue;
		}
		auto [key, value] = parseValue(line);
		if (key == "port") {
			station.port = std::stoi(value);
		} else if (key == "timeout1") {
			station.timeout1 = std::stoi(value);
		} else if (key == "address1") {
			station.address1 = value;
		}
	}
	file.close();
	return std::vector<Station>(stations.begin(), stations.end());
}

void writeConfig(const std::vector<Station> &stations) {
	const char *home = std::getenv("HOME");
	std::ofstream file(std::string(home) +
					   "/.kaskad-scada/networkinteraction.ini");

	for (const auto &station : stations) {
		file << station.name << "\n";
		file << "port=" << station.port << "\n";
		file << "timeout1=" << station.timeout1 << "\n";
		file << "address1=" << station.address1 << "\n";
	}
	file.close();
}
