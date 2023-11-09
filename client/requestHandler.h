#pragma once
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "utils.h"
#include <fstream>
#include "responseHandler.h"
#include "RSAWrapper.h"
#include "Base64Wrapper.h"
#include "AESWrapper.h"

#include <iomanip>
#include <sstream>

using std::string;
using std::to_string;
using boost::asio::ip::tcp;


void loginRequest(tcp::socket& s, string& name);
void reLoginRequest(tcp::socket& s);
void sendKey(tcp::socket& s, string publicKey);
void sendFile(boost::asio::ip::tcp::socket& s, const string& AES_key);
void validCRC(tcp::socket& s);
void invalidCRC(tcp::socket& s);
void failCRC(tcp::socket& s);

string toPythonLikeString(const std::string& binaryData);

