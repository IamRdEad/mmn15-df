#include "utils.h"

string getClientName() {
	string Name;
	std::ifstream meFile(ME_FILE);
	if (meFile.is_open()) {
		std::getline(meFile, Name); // the first line is the name
		meFile.close();
	}
	else {
		throw std::runtime_error("program encounterd an error");
	}
	return Name;
}

string getClientID() {
	string UUID;
	std::ifstream meFile(ME_FILE);
	if (meFile.is_open()) {
		std::getline(meFile, UUID); // the first line is the name
		std::getline(meFile, UUID); //the second line is the UUID
		meFile.close();
	}
	else {
		throw std::runtime_error("program encounterd an error");
	}
	return UUID;
}

string getFilePath() {
	string filePath;
	std::ifstream transferFile(INFO_FILE);
	if (transferFile.is_open()) {
		std::getline(transferFile, filePath); // the first line is the IP
		std::getline(transferFile, filePath); //the second line is the name
		std::getline(transferFile, filePath); //the second line is the FilePath
		transferFile.close();
	}
	else {
		throw std::runtime_error("program encounterd an error");
	}
	return filePath;
}
string getPrivKey() {
	string privateKey="";
	std::string line="";
	std::ifstream priveFile(PRIV_FILE);
	if (priveFile.is_open()) {
		while (std::getline(priveFile, line)) {
			privateKey += line;
		}
		priveFile.close();
	}
	else {
		throw std::runtime_error("program encounterd an error");
	}
	return privateKey;
}