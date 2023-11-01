import sqlite3
import uuid
from datetime import datetime

PATH = "defensive.db"  # name of the database file.


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

    def __str__(self):
        return "ID: " + self._ID + " Name: " + self._name

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


def DBFlow():
    """
     this function responsible for the flow and logic of the database
     :return: None
     """
    global PATH
    db_con = sqlite3.connect(PATH)  # connect to the DB file, if it doesn't exist create one
    cur = db_con.cursor()
    clientData(db_con, cur)
    filesData(db_con, cur)


def clientData(db_con, cur):
    """
    this function responsible to check if there is a client table. if there is get data otherwise create one
    :param db_con: connection to the database
    :param cur: cursor to the database
    :return: None
    """
    table_name = "clients"
    cur.execute(f"SELECT name FROM sqlite_master WHERE type='table' AND name='{table_name}'")
    result = cur.fetchone()
    if result:
        importClients(db_con, cur)
    else:
        createClientsTable(db_con, cur)


def importClients(db_con, cur):
    for row in cur.execute("SELECT * FROM clients"):
        client = Client(row[0], row[1], row[2], row[3], row[4])
        ClientsList.append(client)


def createClientsTable(db_con, cur):
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


def filesData(db_con, cur):
    """
    this function responsible to check if there is a files table. if there is get data otherwise create one
    :param db_con: connection to the database
    :param cur: cursor to the database
    :return: None
    """
    table_name = "clients"
    cur.execute(f"SELECT name FROM sqlite_master WHERE type='table' AND name='{table_name}'")
    result = cur.fetchone()
    if result:
        importFiles(db_con, cur)
    else:
        createFilesTable(db_con, cur)


def importFiles(db_con, cur):
    for row in cur.execute("SELECT * FROM files"):
        file = Files(row[0], row[1], row[2], row[3])
        FilesList.append(file)


def createFilesTable(db_con, cur):
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
