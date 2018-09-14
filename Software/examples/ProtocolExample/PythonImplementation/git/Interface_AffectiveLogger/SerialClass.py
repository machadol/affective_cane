import sys
from ArPy_Host import ProtocolDecoder
import time
from PyQt4.QtCore import QThread, SIGNAL
from PyQt4 import QtGui, QtCore
import random
import DB_manager
from QuaternionsTest import *

class SerialWatcher(QThread):

    new_gsr_pck = QtCore.pyqtSignal(int)
    new_pox_pck = QtCore.pyqtSignal(int)
    new_Acc_pck = QtCore.pyqtSignal(list)
    new_DB_created = QtCore.pyqtSignal(str)
    new_Touch_pck = QtCore.pyqtSignal(int)

    def __init__(self, SerialPort,BaudRate,WatchPeriod):

        super(SerialWatcher, self).__init__()
        self.PckId_Samples = 0
        self.PckId_GSR = 1
        self.PckId_Pox = 2
        self.PckId_ACC = b'\x03'
        self.PckId_TOUCH = 4

        self.buffersize_tosavein_db = 10

        self.serialport = SerialPort
        self.baudrate = BaudRate
        self.refreshrate_ms = WatchPeriod

        self.PD = ProtocolDecoder(self.serialport, self.baudrate)

    def create_db(self, db):

        self.dbu_GSR = DB_manager.DatabaseUtility(db, 'GSR')
        self.dbu_POX = DB_manager.DatabaseUtility(db, 'POX')
        self.dbu_ACC = DB_manager.DatabaseUtility(db, 'ACC')
        self.dbu_TOUCH = DB_manager.DatabaseUtility(db, 'TOUCH')

        self.GSR_samplelist = []
        self.POX_samplelist = []
        self.ACC_sampleList = []
        self.TOUCH_sampleList = []

    def start_watcher(self):
        
        self.first_timer = time.time()

        # each time this thread is started, it will create a Qtimer
        # that will read the sensor
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.refresh_results)
        self.timer.start(self.refreshrate_ms)
        
        self.exec_()

    def refresh_results(self):

        self.result_dict = self.PD.read_Results()

        print("REFRESH_RESULTS")
        print(len(self.result_dict))

        # result_dict =[]
        # for each in range(0,100):
        #     dict = {}

        #     dict['type'] = random.randint(1,2)
        #     dict['value'] = random.randint(1, 1024)
        #     result_dict.append(dict)

        if(len(self.result_dict) > 0):
            for each in self.result_dict:
                if(each['type'] == self.PckId_GSR):
                    print("Chegou Pck GSR")
                    self.new_gsr_pck.emit(each['data'])
                    self.GSR_samplelist.append(each['data'])
                    if(len(self.GSR_samplelist) > self.buffersize_tosavein_db):
                        self.dbu_GSR.AddMultipleEntryToTable(self.GSR_samplelist)
                        self.GSR_samplelist[:] = []
                elif(each['type'] == self.PckId_Pox):
                    print("Chegou Pck Pox")
                    self.new_pox_pck.emit(each['data'])
                    self.POX_samplelist.append(each['data'])
                    if(len(self.POX_samplelist) > self.buffersize_tosavein_db):
                        self.dbu_POX.AddMultipleEntryToTable(self.POX_samplelist)
                        self.POX_samplelist[:] = []
                elif(each['type'] == self.PckId_ACC):
                    print("Chegou Pck ACC")
                    self.new_Acc_pck.emit(each['data'])
                    self.ACC_sampleList.append(each['data'])
                    if(len(self.ACC_sampleList) > self.buffersize_tosavein_db):
                        self.dbu_ACC.AddMultipleEntryToTable(self.ACC_sampleList)
                        self.ACC_sampleList[:] = []
                elif(each['type'] == self.PckId_TOUCH):
                    print("Chegou Pck TOUCH")
                    self.new_Touch_pck.emit(each['data'])
                    self.TOUCH_sampleList.append(each['data'])
                    if(len(self.TOUCH_sampleList) > self.buffersize_tosavein_db):
                        self.dbu_TOUCH.AddMultipleEntryToTable(self.TOUCH_sampleList)
                        self.TOUCH_sampleList[:] = []
                else:
                    print("Tipo nao Reconhecido")
                    print(each['type'])

                    pass
    def stop_thread(self):

        print("Watcher is being killed.")
        self.timer.stop()
        
        self.dbu_GSR.closeConnections()
        self.dbu_POX.closeConnections()
        self.dbu_ACC.closeConnections()

        self.wait()









