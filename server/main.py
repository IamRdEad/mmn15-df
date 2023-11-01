import socket

PATH = "port.info"
PORT = 1357  # default port in case port.info file doesn't exist
IP = "0.0.0.0"


def getPort():
    """
    this function responsible for reading the port number from the port.info file
    in case the port doesn't exist the default port 1357 will be used
    :return: None
    """
    global PORT
    try:
        portFile = open(PATH, "r")
        PORT = int(portFile.readline())
        print("port was found successfully using port: ", PORT)
    except FileNotFoundError:
        print("error find port.info file using port: ", PORT)
    except ValueError:
        print("error reading port number using port: ", PORT)


def serverStart():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((IP, PORT))
    server_socket.listen(1)
    while True:
        print('waiting for a connection...')
        conn, client = server_socket.accept()
        print("received a connection")


def main():
    print("getting port number from port.info file...")
    getPort()
    print("setting up server...")
    serverStart()


if __name__ == '__main__':
    main()
