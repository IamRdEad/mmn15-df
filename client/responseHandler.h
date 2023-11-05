#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <fstream>

using std::string;
using boost::asio::ip::tcp;

void getResponse(tcp::socket& s);
int findCode(string response); 
void registerSuc(string response);