from ArPy_Host import ProtocolDecoder
import time

PD = ProtocolDecoder('/dev/ttyUSB0', 115200)


while(1):
    a = PD.read_Results()
    print(a)

    time.sleep(10)
