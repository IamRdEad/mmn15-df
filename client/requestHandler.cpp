#include "requestHandler.h"


void loginRequest(tcp::socket& s, string& name){
	string request = "Client ID: irrelevant\nVersion: 3\nCode: 1025\nPayload size: " + to_string(name.length()) + "\nName: " + name +"\n";
	//std::cout << request;
	std::string utf8_request(request);
	boost::asio::write(s, boost::asio::buffer(utf8_request.c_str(), utf8_request.length()));
}
void reLoginRequest() {

}

