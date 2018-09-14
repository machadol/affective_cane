from PyQt4 import QtOpenGL
from PyQt4.QtCore import QThread, SIGNAL
from PyQt4 import QtGui, QtCore
import time
import sys
import math


try:
    from OpenGL import GL
except ImportError:
    app = QtGui.QApplication(sys.argv)
    QtGui.QMessageBox.critical(None, "OpenGL hellogl",
            "PyOpenGL must be installed to run this example.")
    sys.exit(1)

    
class GLWidget(QtOpenGL.QGLWidget):
    xRotationChanged = QtCore.pyqtSignal(int)
    yRotationChanged = QtCore.pyqtSignal(int)
    zRotationChanged = QtCore.pyqtSignal(int)

    def __init__(self, parent=None):
        super(GLWidget, self).__init__(parent)

        self.object = 0
        self.xRot = 0
        self.yRot = 0
        self.zRot = 0

        self.lastPos = QtCore.QPoint()

        self.trolltechGreen = QtGui.QColor.fromCmykF(0.40, 0.0, 0.25, 0.0)
        self.trolltechPurple = QtGui.QColor.fromCmykF(0.39, 0.39, 0.0, 0.0)

        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.updateGL)

        self.timer.start(40)

        self.lasttime = time.time()

    def minimumSizeHint(self):
        return QtCore.QSize(50,50)

    def sizeHint(self):
        return QtCore.QSize(400, 400)

    def setXRotation(self, angle):

        self.xRot = angle
        #self.updateGL()

    def setYRotation(self, angle):

        self.yRot = angle
        #self.updateGL()
    def setZRotation(self, angle):
        self.zRot = angle
        #self.updateGL()

    def initializeGL(self):
        self.qglClearColor(self.trolltechPurple.dark())
        self.object = self.makeObject()
        GL.glShadeModel(GL.GL_FLAT)
        GL.glEnable(GL.GL_DEPTH_TEST)
        GL.glEnable(GL.GL_CULL_FACE)

    def paintGL(self):

        timer = time.time()

        print(timer - self.lasttime)
        self.lasttime = timer
        GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT)
        GL.glLoadIdentity()
        GL.glTranslated(0.0, 0.0, -10.0)
        GL.glRotatef(self.xRot, 1, 0.0, 0.0)
        GL.glRotatef(self.yRot, 0.0, 1, 0.0)
        GL.glRotatef(self.zRot, 0.0, 0.0, 1)
        GL.glCallList(self.object)

    def resizeGL(self, width, height):
        side = min(width, height)
        if side < 0:
            return
        GL.glViewport(0, 0,width, height)
        GL.glMatrixMode(GL.GL_PROJECTION)
        GL.glLoadIdentity()
        GL.glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0)
        GL.glMatrixMode(GL.GL_MODELVIEW)

    def mousePressEvent(self, event):
        self.lastPos = event.pos()

    def mouseMoveEvent(self, event):
        dx = event.x() - self.lastPos.x()
        dy = event.y() - self.lastPos.y()

        if event.buttons() & QtCore.Qt.LeftButton:
            self.setXRotation(self.xRot + 8 * dy)
            self.setYRotation(self.yRot + 8 * dx)
        elif event.buttons() & QtCore.Qt.RightButton:
            self.setXRotation(self.xRot + 8 * dy)
            self.setZRotation(self.zRot + 8 * dx)

        self.lastPos = event.pos()

    def rotateObject(self, angleX, angleY, angleZ):

        self.setXRotation(angleX)
        self.setYRotation(angleY)
        self.setZRotation(angleZ)

    def makeObject(self):

        genList = GL.glGenLists(1)
        GL.glNewList(genList, GL.GL_COMPILE)

        # GL.glBegin(GL.GL_QUADS)

        # GL.glColor3f(0.0,0.25,0.0)
        # GL.glVertex3f( 0.25, 0.25,-0.25)
        # GL.glVertex3f(-0.25, 0.25,-0.25)
        # GL.glVertex3f(-0.25, 0.25, 0.25)
        # GL.glVertex3f( 0.25, 0.25, 0.25) 
 
        # GL.glColor3f(0.25,0.0,0.0)
        # GL.glVertex3f( 0.25,-0.25, 0.25)
        # GL.glVertex3f(-0.25,-0.25, 0.25)
        # GL.glVertex3f(-0.25,-0.25,-0.25)
        # GL.glVertex3f( 0.25,-0.25,-0.25) 
 
        # GL.glColor3f(0.0,0.25,0.0)
        # GL.glVertex3f( 0.25, 0.25, 0.25)
        # GL.glVertex3f(-0.25, 0.25, 0.25)
        # GL.glVertex3f(-0.25,-0.25, 0.25)
        # GL.glVertex3f( 0.25,-0.25, 0.25)
 
        # GL.glColor3f(0.25,0.25,0.0)
        # GL.glVertex3f( 0.25,-0.25,-0.25)
        # GL.glVertex3f(-0.25,-0.25,-0.25)
        # GL.glVertex3f(-0.25, 0.25,-0.25)
        # GL.glVertex3f( 0.25, 0.25,-0.25)
 
        # GL.glColor3f(0.0,0.0,0.25)
        # GL.glVertex3f(-0.25, 0.25, 0.25) 
        # GL.glVertex3f(-0.25, 0.25,-0.25)
        # GL.glVertex3f(-0.25,-0.25,-0.25) 
        # GL.glVertex3f(-0.25,-0.25, 0.25) 
 
        # GL.glColor3f(0.25,0.0,0.25)
        # GL.glVertex3f( 0.25, 0.25,-0.25) 
        # GL.glVertex3f( 0.25, 0.25, 0.25)
        # GL.glVertex3f( 0.25,-0.25, 0.25)
        # GL.glVertex3f( 0.25,-0.25,-0.25)

        # GL.glEnd()
        GL.glBegin(GL.GL_TRIANGLES)           # Begin drawing the pyramid with 4 triangles
        # Front
        GL.glColor3f(1, 0.0, 0.0)     # Red
        GL.glVertex3f( 0.0, 0.25, 0.0)
        GL.glColor3f(0.0, 1, 0.0)     # Green
        GL.glVertex3f(-0.25, -0.25, 0.25)
        GL.glColor3f(0.0,1,1)     # Blue
        GL.glVertex3f(0.25, -0.25, 0.25)

        #Right
        GL.glColor3f(1, 0.0, 0.0)     # Red
        GL.glVertex3f(0.0, 0.25, 0.0)
        GL.glColor3f(0.0,1,1)     # Blue
        GL.glVertex3f(0.25, -0.25, 0.25)
        GL.glColor3f(0.0, 1, 0.0)     # Green
        GL.glVertex3f(0.25, -0.25, -0.25)

        #Back
        GL.glColor3f(1, 0.0, 0.0)     # Red
        GL.glVertex3f(0.0, 0.25, 0.0)
        GL.glColor3f(0.0, 1, 0.0)     # Green
        GL.glVertex3f(0.25, -0.25, -0.25)
        GL.glColor3f(0.0,1,1)     # Blue
        GL.glVertex3f(-0.25, -0.25, -0.25)

        #Left
        GL.glColor3f(1,0.0,0.0)       # Red
        GL.glVertex3f( 0.0, 0.25, 0.0)
        GL.glColor3f(0.0,0.0,1)       # Blue
        GL.glVertex3f(-0.25,-0.25,-0.25)
        GL.glColor3f(0.0,1,0.0)       # Green
        GL.glVertex3f(-0.25,-0.25, 0.25)

        GL.glColor3f(1,0.0,0.0)       # Red
        GL.glVertex3f(-0.25,-0.25, 0.25)
        GL.glVertex3f(-0.25, -0.25, -0.25)
        GL.glVertex3f(0.25, -0.25,-0.25)

        GL.glColor3f(1,1.0,0.5)       # Red
        GL.glVertex3f(0.25,-0.25, 0.25)
        GL.glVertex3f(-0.25, -0.25, 0.25)
        GL.glVertex3f(0.25, -0.25,-0.25)

        GL.glEnd()   # Done drawing the pyramid

        GL.glEndList()
        return genList

    def quad(self, x1, y1, x2, y2, x3, y3, x4, y4):
        self.qglColor(self.trolltechGreen)

        GL.glVertex3d(x1, y1, -0.05)
        GL.glVertex3d(x2, y2, -0.05)
        GL.glVertex3d(x3, y3, -0.05)
        GL.glVertex3d(x4, y4, -0.05)

        GL.glVertex3d(x4, y4, +0.05)
        GL.glVertex3d(x3, y3, +0.05)
        GL.glVertex3d(x2, y2, +0.05)
        GL.glVertex3d(x1, y1, +0.05)

    def extrude(self, x1, y1, x2, y2):
        self.qglColor(self.trolltechGreen.dark(250 + int(0.25 * x1)))

        GL.glVertex3d(x1, y1, +0.05)
        GL.glVertex3d(x2, y2, +0.05)
        GL.glVertex3d(x2, y2, -0.05)
        GL.glVertex3d(x1, y1, -0.05)

    def normalizeAngle(self, angle):
        while angle < 0:
            angle += 360 * 16
        while angle > 360 * 16:
            angle -= 360 * 16
        return angle

   