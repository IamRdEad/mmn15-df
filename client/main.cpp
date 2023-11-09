#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include  "fileHandler.h"
#include "requestHandler.h"
#include "responseHandler.h"
#include "utils.h"
#include "RSAWrapper.h"
#include "Base64Wrapper.h"


using std::string;
using boost::asio::ip::tcp;

void flowControl(tcp::socket& s, string& name);
void genrateKeys(tcp::socket& s);
string readAES();

int main() {
	string ip, port, name, filePath;
	transferFile(ip, port, name, filePath);

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
	//std::remove("me.info");
	//std::remove("priv.key");
	std::ifstream meFile;
	string UUID = "";
	string AES_Key = "";
	meFile.open(ME_FILE);
	//if me file not exsits then need to register, if exsits then its reLogin
	if (!meFile) {
		std::ofstream meFile(ME_FILE); //create me.info file
		meFile << name << '\n'; //write the name in the me.info file
		meFile.close();
		loginRequest(s,name);
		genrateKeys(s);
	}
	else {
		reLoginRequest(s); 
	}

	meFile.close();
	AES_Key = readAES();
	int i = 0;
	for (; i < 4; i++) {
		sendFile(s, AES_Key);
		if (compareCRC(s)) {
			std::cout << "CRC OK\n";
			validCRC(s);
			break;
		}
		else {
			invalidCRC(s);
			sendFile(s, AES_Key);
		}
	}
	if(i==4){ 
		failCRC(s);
	}
}
string readAES() {
	std::ifstream AESFile("AES.txt");
	std::string line;
	std::getline(AESFile, line);
	AESFile.close();
	std::remove("AES.txt"); 
	return line; 
}
void genrateKeys(tcp::socket& s) {
	RSAPrivateWrapper rsapriv;
	string publicKey = rsapriv.getPublicKey(); 
	string privateKey = rsapriv.getPrivateKey();
	string publicKeyEncoded = Base64Wrapper::encode(publicKey); 
	std::ofstream privKeyFile;
	privKeyFile.open("priv.key");
	privKeyFile << Base64Wrapper::encode(privateKey);
	privKeyFile.close();
	sendKey(s, publicKeyEncoded); 
}