import socket
import requestHandler

conn, client = None, None


def serverStart(IP, PORT):
    """
    this function responsible for setting up a server connection
    :return: None
    """
    global conn, client
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((IP, PORT))
    server_socket.listen(1)
    while True:
        print('waiting for a connection...')
        conn, client = server_socket.accept()
        print("received a connection")
        getRequest()
        getRequest()
        getRequest()


def sendResponse(response):
    conn.send(response.encode())


def getRequest():
    global conn
    data = conn.recv(1024).decode()
    # print("the data is:\n" + data)
    if data.startswith("Client ID"):
        requestHandler.findCode(data)
    else:
        print("invalid request")


def getBinaryData():
    global conn
    return conn.recv(1024)
