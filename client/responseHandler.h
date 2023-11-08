#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <fstream>
#include "utils.h"


using std::string;
using boost::asio::ip::tcp;
void getResponse(tcp::socket& s);
int findCode(string response); 
void registerSuc(string response);
void sendKey(tcp::socket& s, string publicKey);
void getPrivateKey(string response);
void registerFail();
string createHeader(string code);