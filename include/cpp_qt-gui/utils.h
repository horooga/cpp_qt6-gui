#include <QString>
#include <fstream>
#include <iostream>
#include <string>

struct Station {

	Station() {
		name = "Новая станция";
		port = 8000;
		timeout1 = 5;
		address1 = "127.0.0.1";
		address2 = "";
		address3 = "";
	}

  public:
	std::string name;
	unsigned int port;
	unsigned int timeout1;
	std::string address1;
	std::string address2;
	std::string address3;
};

bool validateStationName(const QString &name);

void writeConfig(const std::vector<Station> &stations);

std::vector<Station> parseConfig();
