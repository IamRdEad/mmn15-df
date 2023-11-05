#include "responseHandler.h"
#include "CONSTANTS.h"


/*
* this functon is used to read a response from the client
* and act according to the code
*/
void getResponse(tcp::socket& s) {
	char response[1024] = { 0 };
	boost::system::error_code error;
	size_t bytesRead = s.read_some(boost::asio::buffer(response, 1024), error);
	string receivedData(response, bytesRead);
	std::cout << "the response is:\n" << receivedData << std::endl;

	int code = findCode(response);
	switch (code)
	{
	case 2100:
		registerSuc(response); 
		break; 
	default:
		break;
	}
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
		std::cerr << "caught an exception";
		//TODO: need to response with general error; 
	}
	return code;
}

