#include "responseHandler.h"
#include "utils.h"


/*
* this functon is used to read a response from the client
* and act according to the code
*/
void getResponse(tcp::socket& s) {
	char response[MAX_LEN] = { 0 };
	boost::system::error_code error;
	size_t bytesRead = s.read_some(boost::asio::buffer(response, MAX_LEN), error);
	string receivedData(response, bytesRead);
	//std::cout << "the response is:\n" << receivedData << std::endl;

	int code = findCode(response);
	switch (code)
	{
	case REGISTER_SUC:
		registerSuc(response); 
		break; 
	case REGISTER_FAIL:
		registerFail();
		break;
	case PRIV_KEY:
		getPrivateKey(response);
		break; 
	default:
		break;
	}

}

void registerFail() {
	throw std::runtime_error("Register Failed");
}

void getPrivateKey(string response) {
	//std::cout << "The response from the server with the AES key is:\n" + response;
	string subString = "AES Key: ";
	int AESIndex = response.find(subString);
	int startIndex = AESIndex + subString.length();
	string AES_key = response.substr(startIndex);
	//creating AES file with the AES key to esily pass it back to the flow control function 
	std::ofstream AESFile("AES.txt"); 
	AESFile << AES_key; 
	AESFile.close();
}

/*
* this function is responsible for finding the UUID 
* inside a response of a successful registration 
*/
void registerSuc(string response) {
	std::ofstream meFile(ME_FILE, std::ios::app);
	string subString = "payload: ";
	int payloadIndex = response.find(subString);
	int startIndex = payloadIndex + subString.length();
	string UUID = response.substr(startIndex);
	meFile << UUID << std::endl;
	meFile.close();
}

/*
* this functon is used to find the code in a response 
* start index is the index where the code itself starts (comes right after "Code: " in the response)
* the end index +4 from the start since the code is 4 digit (+5 because substr is exclusive to the end)
*/
int findCode(string response) {
	int code; 
	string subString = "Code: ";
	int CodeIndex = response.find(subString);
	int startIndex = CodeIndex + subString.length();
	int endIndex = startIndex + 5;
	string codeStr = response.substr(startIndex, endIndex - startIndex); 
	//std::cout << "The Code is: " << codeStr << std::endl;
	try {
		code = std::stoi(codeStr); 
	}
	catch(const std::exception& e){
		std::cerr << "caught an exception" << e.what();
		//TODO: need to response with general error; 
	}
	return code;
}

