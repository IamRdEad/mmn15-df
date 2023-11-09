#include "requestHandler.h"



void loginRequest(tcp::socket& s, string& name){
	string request = "Client ID: irrelevant\nVersion: " + std::to_string(VERSION) + "\nCode: " +
		std::to_string(REGISTER) + "\nPayload size: " + to_string(name.length()) + "\nName: " + name +"\n";
	//std::cout << request;
	string utf8_request(request);
	boost::asio::write(s, boost::asio::buffer(utf8_request.c_str(), utf8_request.length()));
	getResponse(s);
}
void reLoginRequest(tcp::socket& s) {
	string request = createHeader(to_string(RELOGIN));
	string name = getClientName();
	request += ("\nPayload size: " + to_string(name.length()) + "\nName: " + name); 
	string utf8_request(request);
	boost::asio::write(s, boost::asio::buffer(utf8_request.c_str(), utf8_request.length()));
	getResponse(s);

}

void sendKey(tcp::socket& s, string publicKey) {
	string UUID = getClientID();
	string request ="Client ID: " + UUID;
	request += ("\nVersion: " + std::to_string(VERSION) + "\nCode: " + std::to_string(KEY_SEND));
	string clientName = getClientName();
	int size = clientName.length() + publicKey.length();
	request += ("\nPayload sizez: " + std::to_string(size) + "\nName: " + clientName + "\nPublic Key: " + publicKey);
	std::string utf8_request(request);
	boost::asio::write(s, boost::asio::buffer(utf8_request.c_str(), utf8_request.length()));
	getResponse(s);
}

void sendFile(tcp::socket& s, const string& AES_Key) {
	//Step 1: Get the File
	string filePath = getFilePath();
	std::ifstream transferFile(filePath, std::ios::binary);

	//Step 2: create AES warpper
	string privRSAKey = getPrivKey();
	RSAPrivateWrapper rsapriv_other(Base64Wrapper::decode(privRSAKey)); 
	string decodedAESKey = Base64Wrapper::decode(AES_Key);
	string AES_keyDecrypted = rsapriv_other.decrypt(decodedAESKey);

	AESWrapper aesWrapper(reinterpret_cast<const unsigned char*>(AES_keyDecrypted.c_str()), AES_keyDecrypted.size());

	//Step 3: Encrypt the data and save it to temporary file
	std::ofstream tempFile("EncryptedFile", std::ios::binary);  
	char buffer[MAX_LEN];

	while (!transferFile.eof()) {
		transferFile.read(buffer, sizeof(buffer));
		std::streamsize bytesRead = transferFile.gcount();

		if (bytesRead > 0) {
			string encryptedChunk = aesWrapper.encrypt(buffer, bytesRead);
			tempFile.write(encryptedChunk.c_str(), encryptedChunk.size());
		}
	}
	transferFile.close();
	tempFile.close();

	//Step 4: get the file size
	std::ifstream tempFileRead("EncryptedFile", std::ios::binary | std::ios::ate);
	std::streamsize fileSize = tempFileRead.tellg();
	tempFileRead.seekg(0, std::ios::beg);
	string header = createHeader(std::to_string(SEND_FILE));
	header += ("\nPayload size: " + std::to_string(fileSize) + "\nContent Size: ") + std::to_string(fileSize) + "\nFile Name: " + filePath;
	std::string utf8_request(header);
	boost::asio::write(s, boost::asio::buffer(utf8_request.c_str(), utf8_request.length()));
	while (!tempFileRead.eof()) {
		tempFileRead.read(buffer, sizeof(buffer));
		std::streamsize bytesRead = tempFileRead.gcount();
		if (bytesRead > 0) {
			boost::asio::write(s, boost::asio::buffer(buffer, bytesRead));
		}
	}
	tempFileRead.close();
	std::remove("EncryptedFile");
}

void validCRC(tcp::socket& s) {
	string fileName = getFilePath();
	string request = createHeader(to_string(CRC_VALID));
	request += "\nPayload Size: " + to_string(fileName.length()) + "\nFile Name: " + fileName; 
	std::string utf8_request(request);
	boost::asio::write(s, boost::asio::buffer(utf8_request.c_str(), utf8_request.length()));
}

void invalidCRC(tcp::socket& s) {
	string fileName = getFilePath();
	string request = createHeader(to_string(CRC_INVALID));
	request += "\nPayload Size: " + to_string(fileName.length()) + "\nFile Name: " + fileName;
	std::string utf8_request(request);
	boost::asio::write(s, boost::asio::buffer(utf8_request.c_str(), utf8_request.length()));
}

void failCRC(tcp::socket& s) {
	string fileName = getFilePath();
	string request = createHeader(to_string(CRC_FAIL));
	request += "\nPayload Size: " + to_string(fileName.length()) + "\nFile Name: " + fileName;
	std::string utf8_request(request);
	boost::asio::write(s, boost::asio::buffer(utf8_request.c_str(), utf8_request.length()));
}

string createHeader(string code) {
	string header = "Client ID: " + getClientID() +"\nVersion: " + std::to_string(VERSION) + "\nCode: " + code;
	return header; 
}

std::string toPythonLikeString(const std::string& binaryData) {
	std::ostringstream oss;
	oss << "b'";

	for (unsigned char c : binaryData) {
		if (std::isprint(c)) {
			oss << c;
		}
		else {
			oss << "\\" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(c);
		}
	}

	oss << "'";
	return oss.str();
}