#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include  "fileHandler.h"
#include "requestHandler.h"
#include "responseHandler.h"
#include "CONSTANTS.h"



using std::string;
using boost::asio::ip::tcp;

void flowControl(tcp::socket& s, string& name);

int main() {
	string ip, port, name, filePath;
	transferFile(ip, port, name, filePath);
	//std::cout << "IP: " << ip << "\nPort: " << port << "\nName: " << name << "\nFile Path: " << filePath << std::endl;

	//establish connection to the server
	boost::asio::io_context io_context;
	tcp::socket s(io_context);
	tcp::resolver resolver(io_context);
	boost::asio::connect(s, resolver.resolve(ip, port));
	flowControl(s,name);


	return 0;
}

/*
* this function is responisable for the flow control of the program
*/
void flowControl(tcp::socket& s,  string& name) {
	std::ifstream meFile;
	meFile.open(ME_FILE);
	//if me file not exsits then need to register, if exsits then its reLogin
	if (!meFile) {
		std::ofstream meFile(ME_FILE); //create me.info file
		meFile << name << '\n'; //write the name in the me.info file
		meFile.close();
		loginRequest(s,name);
	}
	else {
		reLoginRequest();
	}
	
	meFile.close();
	getResponse(s);

}