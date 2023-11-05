#pragma once
#include  <iostream>
#include <string>
#include <boost/asio.hpp>

using std::string;
using std::to_string;
using boost::asio::ip::tcp;

void loginRequest(tcp::socket& s, string& name);
void reLoginRequest();

