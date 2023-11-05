#pragma once
#include <iostream>
#include <string>
#include <fstream>

using std::string;

/*
* the function get the ip, port, name and the name of the client
* from the transfer.info file
*/
void transferFile(string& ip, string& port, string& name, string& filePath);