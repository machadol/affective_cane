from PyQt4 import QtCore, QtGui
import pyqtgraph as pg
import sys
import numpy as np
from SerialClass import *
from datetime import datetime
from OpenGLClass import GLWidget as GLWidget
from QuaternionsTest import *
import time

class UI_App(QtGui.QWidget):

    def __init__(self):
        QtGui.QWidget.__init__(self)

        # size of the windows to be plotted at MHEALTH
        self.MHEALTH_bufferSize = 1000

        self.buildWindow()
        self.plotStarter()

        self.x_l = 10;

    def buildWindow(self):

        self.setWindowTitle("Affective Logger By Pedro Inazawa")

        #First Create two Layouts
        InfoGrid = QtGui.QHBoxLayout()

        #InfoGrid will be at bottom, and will hold info and buttons
        self.RunButton = QtGui.QPushButton('START\nLOGGING')
        self.RunButton.setCheckable(True)
        self.RunButton.clicked.connect(self.buttonClick)

        self.ExperimentInfo = QtGui.QTextEdit()
        self.ExperimentInfo.setText("AffectiveLog_" + datetime.now().strftime('%d_%m_%Y_%H_%M'))


        InfoGrid.addWidget(self.RunButton)
        InfoGrid.addWidget(self.ExperimentInfo)

        Infogroupbox = QtGui.QGroupBox('INFO')
        Infogroupbox.setStyleSheet("QGroupBox { border: 1px solid black;}")
        Infogroupbox.setLayout(InfoGrid)

        #Now lets do the DataGrid
        Datagroupbox = QtGui.QGroupBox('DATA')
        Datagroupbox.setStyleSheet("QGroupBox { border: 1px solid black;}")

        LocationalGroupBox = QtGui.QGroupBox('LOCATION')
        LocationalGroupBox.setStyleSheet("QGroupBox { border: 1px solid black;}")
        
        Locational_Grid = QtGui.QVBoxLayout()
        self.Locational_RotationalImg =  GLWidget() 
        self.Locational_Data = QtGui.QTextEdit() 
        Locational_Grid.addWidget(self.Locational_RotationalImg)
        Locational_Grid.addWidget(self.Locational_Data)
        Locational_Grid.setMargin(20)

        LocationalGroupBox.setLayout(Locational_Grid)

        AmbientalGroupBox = QtGui.QGroupBox('AMBIENTAL')
        Ambiental_Grid = QtGui.QVBoxLayout()
        Ambiental_Graph1 = QtGui.QTextEdit() 
        Ambiental_Graph2 = QtGui.QTextEdit() 
        Ambiental_Graph3 = QtGui.QTextEdit() 

        Ambiental_Grid.addWidget(Ambiental_Graph1)
        Ambiental_Grid.addWidget(Ambiental_Graph2)
        Ambiental_Grid.addWidget(Ambiental_Graph3)
        Ambiental_Grid.setMargin(20)

        AmbientalGroupBox.setLayout(Ambiental_Grid)


        MHealthGroupBox = QtGui.QGroupBox('M-Health')
        MHealth_Grid = QtGui.QVBoxLayout()
        self.MHealth_Graph1 = pg.PlotWidget() 
        self.MHealth_Graph2 = pg.PlotWidget() 

        MHealth_Grid.addWidget(self.MHealth_Graph1)
        MHealth_Grid.addWidget(self.MHealth_Graph2)
        MHealth_Grid.setMargin(20)

        MHealthGroupBox.setLayout(MHealth_Grid)

        subgrid = QtGui.QGridLayout()

        subgrid.addWidget(LocationalGroupBox,1,0)
        subgrid.addWidget(AmbientalGroupBox,1,1)
        subgrid.addWidget(MHealthGroupBox, 1,2)
        # subgrid.setSpacing(500)
        subgrid.setMargin(20)

        Datagroupbox.setLayout(subgrid)

        MainGrid = QtGui.QGridLayout()
        MainGrid.addWidget(Infogroupbox,1,0)
        MainGrid.addWidget(Datagroupbox,0,0)

        self.setLayout(MainGrid)

    def plotStarter(self):

        x = [1,2,3,4,5,6,7]
        y = [1,2,3,4,5,6,7]

        self.MHealth_Graph1.plot(x,y,pen='r')
        self.MHealth_Graph1.setLabel('left', 'GSR Raw Value', units='V')
        self.MHealth_Graph1.setLabel('bottom', 'Time', units='s')

        self.MHealth_Graph2.plot(x,y,pen='y')
        self.MHealth_Graph2.setLabel('left', 'Ox Raw Value', units='V')
        self.MHealth_Graph2.setLabel('bottom', 'Time', units='s')

        self.MHEALTH_XAxis = np.arange(0,self.MHEALTH_bufferSize) 

        # Instantiate the array that will hold the Last BufferSize samples we got
        self.LastPoxData = np.zeros(self.MHEALTH_bufferSize, dtype=int)
        self.LastGSRData = np.zeros(self.MHEALTH_bufferSize, dtype=int)
        self.PoxSampleIndex = 0 # Will hold the qty of samples we got yet
        self.GSRSampleIndex = 0 

    def connectToQThread(self):
        self.readerThread = SerialWatcher('/dev/ttyUSB1', 115200, 100)
        
        self.readerThread.new_gsr_pck.connect(self.GSR_Update_Plot)
        self.readerThread.new_pox_pck.connect(self.POX_Update_Plot)
        self.readerThread.new_Acc_pck.connect(self.ACC_Update_Plot)
        self.readerThread.new_Touch_pck.connect(self.TOUCH_Update_Plot)

        self.readerThread.create_db(self.ExperimentInfo.toPlainText())
        self.readerThread.start_watcher()

    def disconnectFromQThread(self):
        self.readerThread.stop_thread()

    def POX_Update_Plot(self, newValue):

        # Puts data until the second-to-last sample
        # then starts rolling the vector and inputing the new value
        if(self.PoxSampleIndex < self.MHEALTH_bufferSize-2):
            self.LastPoxData[self.PoxSampleIndex] = newValue
            self.PoxSampleIndex +=1
        
        else:
            self.LastPoxData = np.roll(self.LastPoxData,-1)
            self.LastPoxData[self.PoxSampleIndex] = newValue

        path = pg.arrayToQPath(self.MHEALTH_XAxis.flatten(), self.LastPoxData.flatten())
        item = QtGui.QGraphicsPathItem(path)
        item.setPen(pg.mkPen('r'))

        self.MHealth_Graph2.clear()    
        self.MHealth_Graph2.addItem(item)

    def GSR_Update_Plot(self, newValue):

        # Puts data until the second-to-last sample
        # then starts rolling the vector and inputing the new value
        if(self.GSRSampleIndex < self.MHEALTH_bufferSize-2):
            self.LastGSRData[self.GSRSampleIndex] = newValue
            self.GSRSampleIndex +=1

        else:
            self.LastGSRData = np.roll(self.LastGSRData,-1)
            self.LastGSRData[self.GSRSampleIndex] = newValue

        path = pg.arrayToQPath(self.MHEALTH_XAxis.flatten(), self.LastGSRData.flatten())
        item = QtGui.QGraphicsPathItem(path)
        item.setPen(pg.mkPen('r'))

        self.MHealth_Graph1.clear()
        self.MHealth_Graph1.addItem(item)


    def ACC_Update_Plot(self, Quat_List):

        print("ACC Update")

        print(Quat_List)
        # First transform to Quaternion
        q = Quaternion(Quat_List[0],Quat_List[1],Quat_List[2], Quat_List[3])

        e = quaternion_to_euler(q)
        
        x = convert_to_degrees(e.x)
        y = convert_to_degrees(e.y)
        z = convert_to_degrees(e.z)
        print(x)
        print(y)
        print(z)
        
        time1 = time.time()

        # if(self.Locational_RotationalImg.xRot >= z*0.9 or self.Locational_RotationalImg.xRot <= z*1.1):
        #     self.Locational_RotationalImg.setYRotation(z)
        
        # if(self.Locational_RotationalImg.yRot >= y*0.9 or self.Locational_RotationalImg.yRot <= y*1.1):
        #     self.Locational_RotationalImg.setXRotation(y)

        # if(self.Locational_RotationalImg.zRot >= x*0.9 or self.Locational_RotationalImg.zRot <= x*1.1):
        #     self.Locational_RotationalImg.setZRotation(x)

        self.Locational_RotationalImg.setYRotation(z)
        self.Locational_RotationalImg.setXRotation(y)
        self.Locational_RotationalImg.setZRotation(x)
        #self.Locational_RotationalImg.setYRotation(x)
        #self.Locational_RotationalImg.setYRotation(y)
        
        time2 = time.time() 
        print(time2 - time1)

    def TOUCH_Update_Plot(self, touchStatus):
        if(touchStatus == 1):
            self.Locational_Data.setText("TOQUE NO SOLO")
        else:
            self.Locational_Data.setText("-")


    def buttonClick(self):

        print("CLICK!")
        print(self.RunButton.isChecked())
        if self.RunButton.isChecked():
            self.connectToQThread()
            self.RunButton.setText("STOP\nLOGGING")
        else:
            self.disconnectFromQThread()
            self.RunButton.setText("START\nLOGGING")


        
if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)

    window = UI_App()
    window.show()

    app.exec_()