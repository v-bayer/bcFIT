#!/usr/bin/python3
# -*- coding: utf-8 -*-

""" autor: Vaclav Bayer, xbayer05
    IPK Projekt c.2, Komunikace server/klient HTTP protokol """

from urllib.parse import urlparse
import sys
import os
import socket
import time


DEBUG = 1

def main():
    arg = Args()
    arg = arg.loadArgs()
    conn = MyConnect(arg)
    conn.parsingUri()
    #connect = Connect(parse)

class Args:

    def printHelp(self):
        sys.stdout.write("Komunikace klient/server pomoci protokolu HTTP 1.1 \n"
            "Chunked-transfer encoding \n\n"
            "Pripustne argumenty:\n"
            "   -h..........vytiskne napovedu\n"
            "   URI.........format rfc3986\n")
        exit(0)

    def loadArgs(self):
        if len(sys.argv) != 2:
            sys.stderr.write("Nepovoleny pocet argumentu. \n")
            exit(1)

        for i in range(1, len(sys.argv)):
            # napoveda
            if sys.argv[i] == "-h":
                self.printHelp()
            # nebo URI
            else:
                uri = sys.argv[i]
        return uri

class MyConnect:

    def __init__(self, arg):
        self.uri = arg
        self.fileName = 'ipkHttpClient'
        self.globalDate = time.strftime("-%Y-%m-%d:%H:%M:%S", time.localtime())

    def parsingUri(self):
        """ Vytahnuti potrebnych informaci z URI """

        uriItems = urlparse(self.uri)
        if not uriItems:
            sys.stderr.write("Zadane URI neni validni. \n")
            exit(1)
        if not uriItems.netloc:
            sys.stderr.write("Neplatny host. \n")
            exit(1)
        else:
            host = uriItems.netloc
        if DEBUG: print(uriItems)

        # ziskani portu
        i = host.find(':')
        if i >= 0:
            port = int(host[i+1:])
            host = host[:i]
        else:
            port = 80

        if not uriItems.query:
            self.query = 'GET'
        else:
            self.query = uriItems.query

        self.host = host
        self.port = port
        self.path = uriItems.path

        self.Connection()

    def Connection(self):
        """ Navazani spojeni se serverem """
        
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (self.host, self.port)
        sock.connect(server_address)

        close = 'Connection: close'
        inFile = '.in.log'
        outFile = '.out.log'
        headFile = '.header'
        payFile = '.payload'

        
        if self.query == "GET":
            finalFileName = self.fileName+self.globalDate+outFile
            openedFileOUT = open(finalFileName, 'a+')

            finalFileName = self.fileName+self.globalDate+inFile
            openedFileIN = open(finalFileName, 'a+')

            message = "GET " + str(self.path)
            openedFileOUT.write(message)
            openedFileOUT.close()

            sock.sendall(bytes(message, 'utf-8'))

            try:
                while True:
                    finalFileName = self.fileName+self.globalDate+payFile
                    openedFilePAY = open(finalFileName, 'a+')

                    data = sock.recv(1024)
                    data = bytes.decode(data)

                    finalData = data.split('\n\n')
                    finalData = finalData[1]

                    openedFileIN.write(data)
                    openedFilePAY.write(finalData)
                    print(data)

                    if close in data:
                        openedFileIN.close()
                        openedFilePAY.close()
                        break

            except Exception as exc:
                sys.stderr.write("Data se nepodarilo prijmout: " + str(exc) + "\n")
                sys.exit(1)

            finally:
                sock.close()

        elif self.query == "HEAD":
            finalFileName = self.fileName+self.globalDate+outFile
            openedFileOUT = open(finalFileName, 'a+')

            finalFileName = self.fileName+self.globalDate+inFile
            openedFileIN = open(finalFileName, 'a+')

            finalFileName = self.fileName+self.globalDate+headFile
            openedFileHEAD = open(finalFileName, 'w+')

            message = "HEAD " + str(self.path)
            openedFileOUT.write(message)
            openedFileOUT.close()

            sock.sendall(bytes(message, 'utf-8'))

            try:
                while True:

                    data = sock.recv(1024)
                    data = bytes.decode(data)

                    openedFileHEAD.write(data)

                    openedFileIN.write(data)
                    print(data)

                    if close in data:
                        openedFileIN.close()
                        openedFileHEAD.close()
                        break

            except Exception as exc:
                sys.stderr.write("Data se nepodarilo prijmout: " + str(exc) + "\n")
                sys.exit(1)

            finally:
                sock.close()

        else:
            print("Nepodporovana metoda: ", method)


if __name__ == '__main__':
    main()
