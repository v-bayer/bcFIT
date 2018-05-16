#!/usr/bin/python3
# -*- coding: utf-8 -*-

""" autor: Vaclav Bayer, xbayer05
    IPK Projekt c.2, Komunikace server/klient HTTP protokol """

import socket
import signal
import time
import sys
import re
import os


class Args:
    def __init__(self):
        self.arg_chunk = 0
        self.arg_port = 0
        self.arg_time = 0
        self.flag_chunk = 0
        self.flag_port = 0
        self.flag_time = 0

    def printHelp(self):
        sys.stdout.write("Komunikace klient/server pomoci protokolu HTTP 1.1 \n"
            "Chunked-transfer encoding \n\n"
            "Pripustne argumenty:\n"
            "   -h, --help.............................vytiskne napovedu\n"
            "   -c=[0-9], --chunk-max-size=[0-9].......urci max. velikost dat v chunku\n"
            "   -p=[0-9], --port=[0-9].................port ke komunikaci s klientem\n"
            "   -t=[0-9], --min-chunk-time=[0-9].......minimalni cas cekani pred odeslanim\n"
            "                                          dalsiho chunku\n")
        exit(0)

    def loadArgs(self):
        """ Nacteni argumentu pres regularni vyrazy """

        if len(sys.argv) < 1 or len(sys.argv) > 4:
            sys.stderr.write("Nepovoleny pocet argumentu. \n")
            exit(1)

        for i in range(1, len(sys.argv)):
            # napoveda
            if sys.argv[i] == "-h":
                self.printHelp()
            # napoveda
            elif sys.argv[i] == "--help":
                self.printHelp()
            # max. velikost chunku
            elif (re.match('^-c=\d+', sys.argv[i])):
                self.arg_chunk += 1
                self.flag_chunk = int(sys.argv[i][3:])
            # max. velikost chunku
            elif (re.match('^--chunk-max-size=\d+', sys.argv[i])):
                self.arg_chunk += 1
                self.flag_chunk = int(sys.argv[i][17:])
            # cislo portu
            elif (re.match('^-p=\d+', sys.argv[i])):
                self.arg_port += 1
                self.flag_port = int(sys.argv[i][3:])
            # cislo portu
            elif (re.match('^--port=\d+', sys.argv[i])):
                self.arg_port += 1
                self.flag_port = int(sys.argv[i][7:])
            # cas
            elif (re.match('^-t=\d+', sys.argv[i])):
                self.arg_time += 1
                self.flag_time = int(sys.argv[i][3:])
            # cas
            elif (re.match('^--min-chunk-time=\d+', sys.argv[i])):
                self.arg_time += 1
                self.flag_time = int(sys.argv[i][17:])
            else:
                sys.stderr.write("Nepovoleny parametr nebo hodnota parametru. Pro napovedu zadejte -h nebo --help. \n")
                exit(1)
        self.checkArgs()

    def checkArgs(self):
        """ Osetreni nedovolenych kombinaci parametru, napr. 2x cas... """

        if self.arg_chunk > 1 or self.arg_port > 1 or self.arg_time > 1 :
            sys.stderr.write("Nepovoleny pocet argumentu. \n")
            exit(1)

        if self.arg_chunk == 0:
            print("Hodnota Chunk size nebyla zadana. Nastaveni defaultni hodnotou: 1024")
            self.flag_chunk = 1024
        if self.arg_port == 0:
            print("Port nebyl zadan. Nastaveni defaultni hodnotou: 80")
            self.flag_port = 80
        if self.arg_time == 0:
            print("Chunk time neni nastaven. Nastaveni defaultni hodnotou: 0ms")
            self.flag_time = 0


def my_sigint(sig, dummy):
    """ Vyckani na signal Ctrl+c, ukonceni spojeni """
    serv.closeConn()
    sys.exit(0)

class Connect:

    def __init__(self):
        self.host = '127.0.0.1'
        self.port = arguments.flag_port
        self.size = arguments.flag_chunk
        self.time = arguments.flag_time
        self.lastChunk = 0
        self.fileName = 'ipkHttpServer'
        # nastaveni casu
        self.globalDate = time.strftime("-%Y-%m-%d:%H:%M:%S", time.localtime())

    def createConn(self):
        """ Pripojeni k serveru """

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.socket.bind((self.host, self.port))
        except Exception as exc:
            sys.stderr.write("Na server se nepodarilo pripojit: " + str(exc) + "\n")
            sys.exit(1)

        print ("Soket byl uspesne vytvoren. Pro ukonceni pouzijte Ctrl+C")
        self.waitToConn()

    def createHead(self,  code):
        """ Vytvoreni hlavicky, 200-uspech nebo 404-neuspech """

        date = time.strftime("%a, %d %b %Y %H:%M:%S GMT", time.localtime())

        head = ""
        if (code == 404):
            head = 'HTTP/1.1 404 Not Found\n'
        elif(code == 200):
            head = 'HTTP/1.1 200 OK\n'

        head += 'Date: ' + date +'\n'
        head += 'Content-Type: text/html\n'
        head += 'Content-Length: ' + str(self.size) + '\n'
        head += 'Transfer-Encoding: chunked\n'
        if self.lastChunk == 1:
            head += 'Connection: close\n'
        return head

    def waitToConn(self):
        """ Hlavni cast. Ceknani na pripojeni a posilani chunku """

        while True:
            print ("Cekani na pripojeni.")
            # maximalni pocet pripojeni
            self.socket.listen(5)

            conn, addr = self.socket.accept()

            print("Navazani pripojeni s adresou:", addr)

            # defaultne na 4000
            data = conn.recv(4000)
            text = bytes.decode(data)

            inFile = '.in.log'
            outFile = '.out.log'

            # Zapis do souboru pro prijem komunikace
            finalFileName = self.fileName+self.globalDate+inFile
            openedFile = open(finalFileName, 'w+')
            openedFile.write(text)
            openedFile.close()

            # Ziskani metody
            method = text.split(' ')
            method = method[0]

            # Podpora jen dvou metod, GET a HEAD
            if (method == 'HEAD') or (method == 'GET'):
                searchFile = text.split(' ')
                searchFile = searchFile[1]
                # odstraneni nechteneho zbytku od souboru
                searchFile = searchFile.split('?')
                searchFile = searchFile[0]

                textToSend = ""
                arrayText = []

                # defaultni soubor pokud neni zadan konkretni
                if (searchFile == '/'):
                    searchFile = 'www/index.html'

                try:
                    # pokus otevrit soubor, v pripde neuspechu err404
                    openedFile = open(searchFile,'rb')
                    textToSend = openedFile.read()
                    fileSize = len(textToSend)+1
                    if (method == 'GET'):
                        # rozdeleni textu do casti podle chunk size parametru
                        start = 0
                        while self.size <= fileSize:
                            arrayText.append(textToSend[start:start+self.size])
                            start += self.size
                            fileSize -= self.size
                            if self.size > fileSize:
                                self.size = fileSize
                                arrayText.append(textToSend[start:start+self.size])
                            if self.size == fileSize:
                                break;
                    openedFile.close()

                    headToSend = self.createHead(200)

                # 404 err
                except Exception as exc:
                    print ("Soubor se nepodarilo otevrit.\n", exc)
                    self.lastChunk = 1
                    headToSend = self.createHead(404)

                    if (method == 'GET'):
                        textToSend = "\n<html><body><p>Error 404: File not found</p></body></html>"

                finalText = headToSend
                if self.lastChunk == 1:
                    finalText += textToSend
                print(finalText)

                finalFileName = self.fileName+self.globalDate+outFile
                openedFile = open(finalFileName, 'a+')
                if (method == 'GET') and self.lastChunk != 1:
                    #print("Printuju arrayText: ", arrayText)
                    for i in range(len(arrayText)):
                        if i == len(arrayText)-1:
                            self.lastChunk = 1
                            headToSend = self.createHead(200)
                        finalText = headToSend
                        finalText += '\n'
                        finalText += bytes.decode(arrayText[i])
                        print(finalText)
                        openedFile.write(finalText + '\n')
                        finalText = finalText.encode()
                        conn.send(finalText)
                        time.sleep(self.time)
                else:
                    openedFile.write(finalText + '\n')
                    finalText = finalText.encode()
                    conn.send(finalText)
                    time.sleep(self.time)

                openedFile.close()
                print ("Ukonceni komunikace s klientem.")
                conn.close()
                self.lastChunk = 0

            else:
                print("Nepodporovana metoda: ", method)

    def closeConn(self):
        try:
            print("Vypinani serveru.")
            serv.socket.shutdown(socket.SHUT_RDWR)

        except Exception as exc:
            sys.stderr.write("Spojeni se nepodarilo ukoncit: " + str(exc) + "\n")
            sys.exit(1)


arguments = Args()
arguments.loadArgs()
signal.signal(signal.SIGINT, my_sigint)
print ("Spusteni serveru.")
serv = Connect()
serv.createConn()