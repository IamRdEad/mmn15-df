import sys

import DB
import codes
import responseHandler


def findCode(request):
    print("received  new request from client...")
    # print(request)
    codeIndex = request.find("Code:")
    code = int(request[codeIndex + 6:codeIndex + 10])
    # print(code)
    if code == codes.REGISTER:
        register(request)
    elif code == codes.KEY_SEND:
        sendKey(request)
    elif code == codes.RELOGIN:
        relogin(request)
    elif code == codes.SEND_FILE:
        sendFile(request)
    elif code == codes.CRC_VALID:
        validCRC(request)
    elif code == codes.CRC_INVALID:
        invalidCRC(request)
    elif code == codes.CRC_FAIL:
        CRCFail(request)
    else:
        print("UNKNOWN CODE system will shut down")
        sys.exit(1)


def register(request):
    idIndex = request.find("Name:")
    ID = request[idIndex + 6:]
    # print("The ID is: " + ID)
    returnValues = DB.addClient(ID)
    # print(returnValues)
    if returnValues == codes.REGISTER_FAIL:
        responseHandler.registerFail()
    else:
        responseHandler.registerSuc(returnValues[1])
    DB.showTable()


def sendKey(request):
    keyIndex = request.find("Public Key: ")
    key = request[keyIndex + 12:]
    VersionIndex = request.find("Version")
    UUID = request[11: VersionIndex - 1]
    DB.updateKey(key, UUID)


def relogin(request):
    pass


def sendFile(request):
    pass


def validCRC(request):
    pass


def invalidCRC(request):
    pass


def CRCFail(request):
    pass
