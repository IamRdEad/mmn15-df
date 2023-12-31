import DB
import codes
import server
from Crypto.Cipher import AES
from Crypto.Cipher import PKCS1_OAEP
import base64
from Crypto.PublicKey import RSA

VERSION = 3


def registerFail():
    response = "Version: " + str(VERSION) + "\nCode: " + str(codes.REGISTER_FAIL) + "\npayload size: 0"
    server.sendResponse(response)


def registerSuc(UUID):
    response = "Version: " + str(VERSION) + "\nCode: " + str(codes.REGISTER_SUC) + "\npayload size: " + str(len(UUID)) \
               + "\npayload: " + UUID + "\n"
    server.sendResponse(response)


def AESKey(keyEncoded, uuid):
    aes_key = AES.get_random_bytes(16)
    keyDecodedData = base64.b64decode(keyEncoded)
    rsa_key = RSA.import_key(keyDecodedData)
    cipher_rsa = PKCS1_OAEP.new(rsa_key)
    encrypted_aes_key = cipher_rsa.encrypt(aes_key)
    aesKeyEncoded = base64.b64encode(encrypted_aes_key).decode('utf-8')
    DB.updateAESKey(aes_key, uuid)
    payloadSize = len(uuid) + len(aesKeyEncoded)
    response = "Version: " + str(VERSION) + "\nCode: " + str(codes.PRIV_KEY) + "\npayload size: " + str(
        payloadSize) + "\nClient ID: " + uuid + "\nAES Key: " + aesKeyEncoded
    server.sendResponse(response)


def reLoginAES(aes_key, uuid):
    public_key = DB.getPublicKey(uuid)
    keyDecodedData = base64.b64decode(public_key)
    rsa_key = RSA.import_key(keyDecodedData)
    cipher_rsa = PKCS1_OAEP.new(rsa_key)
    encrypted_aes_key = cipher_rsa.encrypt(aes_key)
    aesKeyEncoded = base64.b64encode(encrypted_aes_key).decode('utf-8')
    response = "Version: " + str(VERSION) + "\nCode: " + str(codes.APPROVE_RELOGIN) + "\nPayload Size: " + \
               str(len(uuid) + len(aesKeyEncoded)) + "\nClient ID: " + uuid + "\nAES Key: " + aesKeyEncoded
    server.sendResponse(response)


def sendCRC(ClientID, ContentSize, fileName, Cksum):
    payloadSize = len(ClientID) + ContentSize + len(fileName) + len(Cksum)
    response = "Version: " + str(VERSION) + "\nCode: " + str(codes.CRC_REC) + "\npayload size: " + str(payloadSize) \
               + "\nClient ID: " + ClientID + "\nContent Size: " + str(ContentSize) + "\nFile Name: " + fileName \
               + "\nCksum: " + Cksum
    server.sendResponse(response)
