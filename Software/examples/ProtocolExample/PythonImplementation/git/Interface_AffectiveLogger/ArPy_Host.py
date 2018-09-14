import serial
import time
import struct

class ProtocolDecoder:

    def __init__(self, SerialPort, baudrate):

            'The initializer will instantiate the serial port. '
            self.Port = SerialPort
            self.SerialDesc = serial.Serial(
                                            port = self.Port, 
                                            baudrate=baudrate, 
                                            timeout=0.001,
                                            inter_byte_timeout=0.0001) # Alternative)


            self.SerialDesc.reset_input_buffer()
            self.SerialDesc.reset_output_buffer()

            # Reset buffer operation needs a little time.
            time.sleep(1)

    def read_Results(self):

        pckTerminator = b'\n'
        pckFound = False   
        msgEnd = False

        ListOfMessages = []

        QtyBytes = self.SerialDesc.inWaiting()

        if(QtyBytes > 0):

            #If enters here, then there're messages already!

            ReadMessage = self.SerialDesc.read(QtyBytes)

            while(len(ReadMessage) > 0):

                
                #print(ReadMessage)
                #print('\n')
                if(ReadMessage[0] == ord(b'\x00') and len(ReadMessage) > 2):
                    # Test whether it is a samples Package!

                    # Considering 2 bytes Ints checks if ReadMessage has Package Ending
                    # This is a count for the first two bytes in the package (ID and number of Itens)
                    BytesAtBeggining = 2

                    # First Byte is the number of INTS inside the pkt
                    NumberOfSamples = ReadMessage[1]

                    if(len(ReadMessage) >= BytesAtBeggining + 2 * NumberOfSamples + 1):
                    
                        if(ReadMessage[BytesAtBeggining+ 2*NumberOfSamples] == ord(pckTerminator)):

                            SampleRates = []

                            for each in range(0,NumberOfSamples):

                                intBeggining = BytesAtBeggining+each*2
                                intEnd = intBeggining+2
                                SampleRates.append(int.from_bytes(ReadMessage[intBeggining:intEnd], byteorder='little'))

                            info = {"type": 0, "data": SampleRates}

                            ListOfMessages.append(info)

                            ReadMessage = ReadMessage[BytesAtBeggining+ 2*NumberOfSamples+1: ]
                            ReadMessage = ReadMessage[1:]
                            print('Got a List!')
                        else:
                            print('skipping... Terminator Not Found')
                            ReadMessage = ReadMessage[1:]
                    else:
                        print('skipping... Truncated Package')
                        ReadMessage = ReadMessage[1:]    

                elif(ReadMessage[0] == ord(b'\x01') or ReadMessage[0] == ord(b'\x02') or ReadMessage[0] == ord(b'\x04')):
                    # If enters here, we got an INT pack

                    if(len(ReadMessage) >= 4 ):                    
                        BytesAtBeggining = 2 
                        # Here we are interested in Int Bytes (in arduino ints have 2 bytes)
                        if(ReadMessage[BytesAtBeggining+1] == ord(pckTerminator)):

                            info = {"type": ReadMessage[0], "data": int.from_bytes(ReadMessage[1:3], byteorder='little')}
                            print('Got a Int!')
                            ListOfMessages.append(info)

                            ReadMessage = ReadMessage[BytesAtBeggining+2:]
                        else:
                            print('skipping... Truncated Package')
                            ReadMessage = ReadMessage[1:]

                    else:
                        print('Skipping... Not big enough')
                        ReadMessage = ReadMessage[1:]

                elif(ReadMessage[0] == ord(b'\x03') and len(ReadMessage) > 2):
                    
                    #If enters here, we got a Float array pck

                    # Considering 2 bytes Ints checks if ReadMessage has Package Ending
                    # This is a count for the first two bytes in the package (ID and number of Itens)
                    BytesAtBeggining = 2

                    # First Byte is the number of INTS inside the pkt
                    NumberOfSamples = ReadMessage[1]

                    if(len(ReadMessage) >= BytesAtBeggining + 4*NumberOfSamples + 1):

                        ReadMessage = ReadMessage[BytesAtBeggining:]

                        if(ReadMessage[4*NumberOfSamples] == ord(pckTerminator)):
                            

                            data = []

                            for each in range(0,NumberOfSamples):

                                TupleOutput = struct.unpack('f',ReadMessage[0:4]) 
                                data.append( TupleOutput[0])
                                ReadMessage = ReadMessage[4:]
                                
                            info = {"type": b'\x03', "data": data}
                            #Just skip the last char
                            ReadMessage = ReadMessage[1:]
                            ListOfMessages.append(info)

                        else:
                            print('skipping... Terminator Not Found')
                            ReadMessage = ReadMessage[1:]


                    else:
                        print('skipping...Package is Incomplete')
                        ReadMessage = ReadMessage[1:]


                elif(ReadMessage[0] == ord(b'\x04')):
                    # If enters here, we got an Float Pack
                    
                    # Considering 2 bytes Ints checks if ReadMessage has Package Ending
                    # This is a count for the first two bytes in the package (ID and number of Itens)
                    BytesAtBeggining = 2

                    if(len(ReadMessage) >= 6):
                        BytesAtBeggining = 4

                        if(ReadMessage[BytesAtBeggining+1] == ord(pckTerminator)):

                            info = {"type": ReadMessage[0], "data": struct.unpack('f',ReadMessage[1:5])}
                            print('Got a Float')
                            ListOfMessages.append(info)

                            ReadMessage = ReadMessage[BytesAtBeggining+2:]
                        else:
                            print('skipping... Truncated Package')
                            ReadMessage = ReadMessage[1:]
                    else:
                        print('Skipping... Not Big enough')
                        ReadMessage = ReadMessage[1:]

              
                else:
                    print('skipping...Package is Incomplete')
                    ReadMessage = ReadMessage[1:]


            #Finally, when everything is OK, return the list of messages gotten
            return ListOfMessages