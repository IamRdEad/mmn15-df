#include "fileHandler.h"
#include "utils.h"

void transferFile(string& ip, string& port, string& name, string& filePath) {
	std::ifstream transferFile;
	transferFile.open(INFO_FILE);
	if (!transferFile) {
		throw std::runtime_error("unable to find transfer.info file");
	}
	string temp = " ";
	//read the first line which should be the ip.  
	std::getline(transferFile, temp);
	//read second line which is the  clientID
	std::getline(transferFile, name);
	//read 3rd line which is the path of the file to send
	std::getline(transferFile, filePath);

	//client id should not be more then 254 (1 for the \0);
	if (name.length() > 254) {
		throw std::runtime_error("clientId is too long please try again with shorted id\n");
	}
	//separate the ip from the port
	size_t index = temp.find(":");
	ip = temp.substr(0, index);
	port = temp.substr(index + 1);
	transferFile.close();
}