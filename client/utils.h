#include <iostream> 
#include <string>
#include <fstream>

using std::string; 

#define	REGISTER 1025
#define	KEY_SEND 1026
#define	RELOGIN 1027
#define	SEND_FILE 1028
#define	CRC_VALID 1029
#define	CRC_INVALID 1030
#define	CRC_FAIL 1031
#define	REGISTER_SUC  2100
#define	REGISTER_FAIL 2101
#define	PRIV_KEY 2102
#define	CRC_REC 2103
#define	THANK 2104
#define	APPROVE_RELOGIN 2105
#define	DISAPPROVE_LOGING 2106
#define	GENERAL_ERROR 2107


#define MAX_LEN 1024

#define ME_FILE "me.info"
#define INFO_FILE "transfer.info"
#define PRIV_FILE "priv.key"

#define VERSION 3

string getClientName();
string getClientID();
string getFilePath();
string getPrivKey();