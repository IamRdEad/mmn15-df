import base64
import sys
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

import CRCcalc
import DB
import codes
import responseHandler
import server


def findCode(request):
    # print("received  new request from client...")
    codeIndex = request.find("Code:")
    # print(request)
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
    idIndex = request.find("Name: ")
    ID = request[idIndex + 6:]
    # print("The ID is: " + ID)
    returnValues = DB.addClient(ID)
    # print(returnValues)
    if returnValues == codes.REGISTER_FAIL:
        responseHandler.registerFail()
    else:
        responseHandler.registerSuc(returnValues[1])


def sendKey(request):
    keyIndex = request.find("Public Key: ")
    key = request[keyIndex + 12:]
    UUID = getUUID(request)
    DB.updateKey(key, UUID)
    responseHandler.AESKey(key, UUID)
    # DB.showTable()


def relogin(request):
    pass


def sendFile(request):
    iv = b"\x00" * 16  # just for practical use should no be set to 0
    # print(request)
    UUID = getUUID(request)
    AES_Key = DB.getAESKey(UUID)
    # print("The AES key in the server is:", AES_Key)
    fileName = getFileName(request)
    ContentSize = int(getContentSize(request))
    cipher = AES.new(AES_Key, AES.MODE_CBC, iv)
    decrypted_bytes = b''

    while ContentSize > 0:
        data = server.getBinaryData()
        print("the data the server got:", data)
        ContentSize -= len(data)
        decrypted_bytes += cipher.decrypt(data)

        # print("the data after decryption:", decrypted_bytes)
    decrypted_text = unpad(decrypted_bytes, AES.block_size)
    print(decrypted_text)

    with open(fileName, 'wb') as file:
        file.write(decrypted_text)

    crc_value = CRCcalc.readfile(fileName)
    print("the crc is: ", crc_value)


def validCRC(request):
    pass


def invalidCRC(request):
    pass


def CRCFail(request):
    pass


def getUUID(request):
    """
    the function return the client ID (uuid) in the request
    :param request:
    :return: the uuiid
    """
    # The UUID ends right before the version and starts after Client ID: in the request
    VersionIndex = request.find("Version")
    startIndex = len("Client ID: ")
    UUID = request[startIndex: VersionIndex - 1]
    return UUID


def getContentSize(request):
    ContentIndex = request.find("Content Size: ")
    FileNameIndex = request.find("File Name: ")
    startIndex = ContentIndex + len("Content Size: ")
    contentSize = request[startIndex: FileNameIndex - 1]
    return contentSize


def getFileName(request):
    FileNameIndex = request.find("File Name: ")
    startIndex = FileNameIndex + len("File Name: ")
    fileName = request[startIndex:]
    return fileName


def getContent(request):
    MessageIndex = request.find("Message Content: ")
    startIndex = MessageIndex + len("Message Content: ")
    Content = request[startIndex:]
    return Content
