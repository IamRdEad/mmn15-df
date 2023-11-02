#include <iostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>



using std::string;
using boost::asio::ip::tcp;
using std::string;

void transferFile(string& ip, string& port, string& name, string& filePath);

int main() {
	string ip, port, name, filePath;
	transferFile(ip, port, name, filePath);
	//std::cout << "IP: " << ip << "\nPort: " << port << "\nName: " << name << "\nFile Path: " << filePath << std::endl;

	//establish connection to the server
	boost::asio::io_context io_context;
	tcp::socket s(io_context);
	tcp::resolver resolver(io_context);
	boost::asio::connect(s, resolver.resolve(ip, port));
	std::cout << "connected\n";
	return 0;
}

/*
* the function get the ip, port, name and the name of the client
* from the transfer.info file
*/
void transferFile(string& ip, string& port, string& name, string&  filePath) {
	std::ifstream transferFile;
	transferFile.open("transfer.info");
	if (!transferFile) {
		throw std::runtime_error("unable to find transfer.info file");
	}
	string temp = "";
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