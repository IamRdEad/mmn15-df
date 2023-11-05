import codes
import server


def generateResponse(responseCode):
    if responseCode == codes.REGISTER_SUC:
        registerSuc()
    elif responseCode == codes.REGISTER_FAIL:
        registerFail()
    elif responseCode == codes.PRIV_KEY:
        pass
    elif responseCode == codes.CRC_REC:
        pass
    elif responseCode == codes.THANK:
        pass
    elif responseCode == codes.APPROVE_RELOGIN:
        pass
    elif responseCode == codes.DISAPPROVE_LOGING:
        pass
    elif responseCode == codes.GENERAL_ERROR:
        pass


def registerFail():
    response = "Version: 3\nCode: " + str(codes.REGISTER_FAIL) + "\npayload size: 0"
    server.sendResponse(response)


def registerSuc(UUID):
    response = "Version: 3\nCode: " + str(codes.REGISTER_SUC) + "\npayload size: 16 \npayload: " + UUID +"\n"
    server.sendResponse(response)

