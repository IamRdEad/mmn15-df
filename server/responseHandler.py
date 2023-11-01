import codes


def findCode(request):
    print("received  new request from client...")
    print(request)
    codeIndex = request.find("Code:")
    code = int(request[codeIndex + 6:codeIndex + 10])
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


def register(request):
    pass


def sendKey(request):
    pass


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
