import sqlite3
import uuid
from datetime import datetime

import codes


class Client:
    """
    Client Class to represent the users of the service
    ID - the ID of the client
    Name - the name of the Client
    PublicKey - a public key the user will use to identify
    LastSeen - the Time when the user last seen on the server
    AESKey - an  encrypted key of the user
    """

    def __init__(self, ID, Name, PublicKey, LastSeen, AESKey):
        self._ID = ID
        self._name = Name
        self._PublicKey = PublicKey
        self._LastSeen = LastSeen
        self.AESKey = AESKey

    def getName(self):
        return self._name


class Files:
    """
    Files Class to store the data about the files on the server
    ID - the ID of the file
    FileName - the name of the File
    PathName - the path where the file is located
    Verified - if the checksum was verified
    """

    def __init__(self, ID, FileName, PathName, Verified):
        self._ID = ID
        self._FileName = FileName
        self._PathName = PathName
        self._Verified = Verified


ClientsList = []
FilesList = []

PATH = "defensive.db"  # name of the database file.
db_con = sqlite3.connect(PATH)
cur = db_con.cursor()


def DBFlow():
    """
     this function responsible for the flow and logic of the database
     :return: None
     """
    clientData()
    filesData()


def clientData():
    """
    this function responsible to check if there is a client table. if there is get data otherwise create one
    :return: None
    """
    table_name = "clients"
    cur.execute(f"SELECT name FROM sqlite_master WHERE type='table' AND name='{table_name}'")
    result = cur.fetchone()
    if result:
        importClients()
    else:
        createClientsTable()


def importClients():
    for row in cur.execute("SELECT * FROM clients"):
        client = Client(row[0], row[1], row[2], row[3], row[4])
        ClientsList.append(client)


def createClientsTable():
    create_table_sql = '''
        CREATE TABLE clients (
            ID TEXT PRIMARY KEY CHECK(length(Name) <= 32),
            Name TEXT CHECK(length(Name) <= 254),
            publicKey TEXT CHECK(length(PublicKey) <= 255),
            LastSeen TIMESTAMP,
            AESKey TEXT CHECK(length(AESKey) <= 128)
        );
        '''
    cur.execute(create_table_sql)
    db_con.commit()


def filesData():
    """
    this function responsible to check if there is a files table. if there is get data otherwise create one
    :return: None
    """
    table_name = "files"
    cur.execute(f"SELECT name FROM sqlite_master WHERE type='table' AND name='{table_name}'")
    result = cur.fetchone()
    if result:
        importFiles()
    else:
        createFilesTable()


def importFiles():
    for row in cur.execute("SELECT * FROM files"):
        file = Files(row[0], row[1], row[2], row[3])
        FilesList.append(file)


def createFilesTable():
    create_table_sql = '''
        CREATE TABLE files (
            ID TEXT PRIMARY KEY CHECK(length(Name) <= 32),
            Name TEXT CHECK(length(Name) <= 254),
            publicKey TEXT CHECK(length(PublicKey) <= 255),
            LastSeen TIMESTAMP,
            AESKey TEXT CHECK(length(AESKey) <= 128)
        );
        '''
    cur.execute(create_table_sql)
    db_con.commit()


def addClient(ID):
    """
    this function register a new user, adds it to the DB and the  client list
    :param ID: the ID of the new USER
    :return: if register successful  - 2100, if failed 2101
    """
    for client in ClientsList:
        if client.getName() == ID:
            print("username is already taken. please try again with different username")
            return codes.REGISTER_FAIL
    currentTime = datetime.now()
    uniqueID = uuid_str = str(uuid.uuid4()).replace("-", "")
    cur.execute("INSERT INTO clients (ID, Name, publicKey, LastSeen, AESKey) VALUES (?,?, ?, ?, ?)",
                (uniqueID, ID, '0000', currentTime, '000000'))
    db_con.commit()
    for row in cur.execute("SELECT * from clients WHERE Name=Name"):
        client = Client(row[0], row[1], row[2], row[3], row[4])
        ClientsList.append(client)
    return codes.REGISTER_SUC, uniqueID


def updateKey(key, UUID):
    cur.execute("UPDATE clients set publicKey = ? WHERE ID = ?", (key, uuid))
    db_con.commit()


def clearTable():
    cur.execute("DELETE FROM clients")
    db_con.commit()


def showTable():
    cur.execute("SELECT * FROM clients")
    rows = cur.fetchall()
    for description in cur.description:
        print(description[0], end='\t')
    print()
    for row in rows:
        for value in row:
            print(value, end='\t')
        print()
