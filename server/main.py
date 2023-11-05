import DB
import server

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


def main():
    DB.clearTable()
    print("getting port number from port.info file...")
    getPort()
    print("setting up server...")
    DB.DBFlow()
    server.serverStart(IP, PORT)


if __name__ == '__main__':
    main()
