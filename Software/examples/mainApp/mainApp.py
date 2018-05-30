#Data visualizer

from pyqtgraph import QtGui, QtCore
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import serial
import time
import  csv

class caneReader():

    def __init__(self):
        self.Port = '/dev/ttyACM0'
        self.SerialDesc = serial.Serial(self.Port, baudrate=115200, timeout=1)
        self.SerialDesc.reset_input_buffer()
        self.SerialDesc.reset_output_buffer()

        self.startTimer = time.time()


    def caneThreadedReader(self):

        QtyBytes = self.SerialDesc.inWaiting()

        bytesRx = []

        if(QtyBytes > 0):

            ReadMessage = self.SerialDesc.read(QtyBytes)

            while(len(ReadMessage) > 0):

                if(ReadMessage[0:4] == b'\r\n\r\n'):

                    pktType = chr(ReadMessage[4])

                    ReadMessage = ReadMessage[4:]

                    #Faz um shift de 3 casas para frente
                    ReadMessage = ReadMessage[3:]

                    #Describe here the packages you want do decode and save,
                    #And how they are supposed to be decoded.
                    if(pktType == '1' or pktType == '2' or pktType == '3'):

                        self.PktRcvTime = time.time()
                        input(ReadMessage)                        

                        while(True):

                            if(chr(ReadMessage[0]) == '\r'):
                                break
                            else:
                                bytesRx.append(ReadMessage[0])

                            ReadMessage = ReadMessage[1:]                          

                        readData = "".join(map(chr, bytesRx))

                        with open(str(pktType)+'_Data.txt', 'a+' ) as f:
                            dataToWrite = [self.PktRcvTime-self.startTimer,readData]
                            
                            for each in dataToWrite:
                                print(each)
                            f.write(str(dataToWrite))


                else:

                    ReadMessage = ReadMessage[1:]




if __name__ == '__main__':

    cane = caneReader()
    
    while (1):

        cane.caneThreadedReader()

