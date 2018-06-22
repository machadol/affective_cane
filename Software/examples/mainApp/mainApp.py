#Data visualizer

from pyqtgraph import QtGui, QtCore
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import serial
import time
from curses import wrapper

#A vector to count how many
pckCounter = np.zeros(9,  dtype=int )

class caneReader():

    def __init__(self):
        self.Port = '/dev/rfcomm0'
        self.SerialDesc = serial.Serial(self.Port, baudrate=115200, timeout=0.1)
        self.SerialDesc.reset_input_buffer()
        self.SerialDesc.reset_output_buffer()

        self.startTimer = time.time()
        self.isReading = False

    def caneThreadedReader(self):

        QtyBytes = self.SerialDesc.inWaiting()

        if(QtyBytes > 0):

            SerialPortReadMessage = self.SerialDesc.read(QtyBytes)

            ReadMessage = SerialPortReadMessage

            while(len(ReadMessage) > 0):

                if(ReadMessage == b'Waiting Click\r\n'):
                    print('Connected and waiting click...')
                    ReadMessage = b''

                elif(len(ReadMessage) > 4 ):
                    self.isReading = True
                    # Since we discovered a pck starter,
                    # the fourth char is the type
                    if(ReadMessage[0:4] == b'\r\n\r\n'):

                        if(ReadMessage[5:7] == b'\r\n'):

                            bytesRx = []

                            pktType = chr(ReadMessage[4])
                            
                            #for the screen counter
                            pckCounter[int(pktType)-1] += 1

                            #Advance to start of subpck
                            ReadMessage = ReadMessage[7:]

                            #Describe here the packages you want do decode and save,
                            #And how they are supposed to be decoded.
                            if(pktType == '1' or pktType == '2' or pktType == '3'):

                                self.PktRcvTime = time.time()                        

                                while(True):

                                    if(ReadMessage == b''):
                                        SerialPortReadMessage = self.SerialDesc.read(QtyBytes)
                                        ReadMessage = SerialPortReadMessage

                                    elif(chr(ReadMessage[0]) == '\r' or chr(ReadMessage[0]) == '\n' ):
                                        if(chr(ReadMessage[0]) == '\r'):
                                            ReadMessage = ReadMessage[2:]
                                        else:
                                            ReadMessage = ReadMessage[1:]
                                        #detected end of pck
                                        break
                                    else:
                                        bytesRx.append(ReadMessage[0])
                                        ReadMessage = ReadMessage[1:]

                                readData = "".join(map(chr, bytesRx))

                                with open(str(pktType)+'_Data.txt', 'a+' ) as f:

                                    dataToWrite = ['%.4f' % float(self.PktRcvTime-self.startTimer),readData]

                                    f.write(",".join(dataToWrite))

                                    f.write('\n')

                            # Accelerometer package identifier
                            elif(pktType == '9'):
                                self.PktRcvTime = time.time()

                                subpktCount = 0

                                while(True):

                                    if(ReadMessage == b''):
                                        SerialPortReadMessage = self.SerialDesc.read(QtyBytes)
                                        ReadMessage = SerialPortReadMessage

                                    elif(chr(ReadMessage[0]) == '\r' or chr(ReadMessage[0]) == '\n' ):                                    
                                        if(subpktCount < 4):
                                            bytesRx.append(ord(','))
                                            subpktCount +=1

                                            if(chr(ReadMessage[0]) == '\r'):
                                                ReadMessage = ReadMessage[2:]
                                            else:
                                                ReadMessage = ReadMessage[1:]
                                        else:
                                            break
                                    else:
                                        bytesRx.append(ReadMessage[0])
                                        ReadMessage = ReadMessage[1:]                                    

                                readData = ''.join(map(chr,bytesRx))

                                with open(str(pktType)+'_Data.txt', 'a+' ) as f:
                                    dataToWrite = ['%.2f' % float(self.PktRcvTime-self.startTimer),readData]

                                    f.write(",".join(dataToWrite))

                                    f.write('\n')

                        else:
                            ReadMessage = ReadMessage[1:]       


                    else:
                        ReadMessage = ReadMessage[1:]
                else:
                        ReadMessage = ReadMessage[1:]


def mainScreenCurses(stdscr):
    # Clear screen
    stdscr.clear()

    cane = caneReader()
    startTime = time.time()

    while(1):
        
        
        cane.caneThreadedReader()

        endTime = time.time()

        if(endTime - startTime > 1.0):
            stdscr.erase()
            if(cane.isReading is not True):
                stdscr.addstr("Cane not Transmitting\n")
            else:
                stdscr.addstr("Cane is Transmitting\n")

            stdscr.addstr("Packets Read:\n")
            stdscr.addstr("Ox: %d pcks/sec \n"    %pckCounter[0])
            stdscr.addstr("GSR: %d pcks/sec \n"   %pckCounter[2])
            stdscr.addstr("Accel: %d pcks/sec \n" %pckCounter[8])

            pckCounter.fill(0)
            startTime = endTime
            

        stdscr.refresh()
        #stdscr.getkey()


if __name__ == '__main__':

    
    
    wrapper(mainScreenCurses)

        

