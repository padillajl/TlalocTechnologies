#This script recovers all the events from the device Ri50A2015 and Ri505A2014, with this is possible to use the following features:
# 1) Obtain a specific consecutive event pressing key "1"
# 2) Obtain a bunch of events introducing the init consecutive and the end consecutive, for this feature is needed to press key "2"
# 3) Match init - end event pointers pressing key "3"
# 4) Recover all pending events, this feature reads the initial pointer and the end pointer if both are different will be recovered until reach the end pointer
#    This feature is enabled pressing key "4", this feature does not match init an end pointer
# 5) Clear screen pressing "c"    

#Version log 
#1v0 - 29/05/19 - Initial version

import time
import serial
import WConio
import sys
import struct
import random

#Events ID
EventID_RegisterInit = "000"

EventID_DecimalPointUsageChanged = "001"
EventID_GPSPlantPositionChanged = "003"
EventID_TypeOfMeterChanged = "004"
EventID_MassModelChanged = "005"
EventID_TypeOfMeasureChanged = "006"
EventID_UnitMeasureChanged = "007"
EventID_CompDensityChanged = "008"
EventID_PasswordChanged = "009"

EventID_JuntionboxTamperOpen = "013"
EventID_JuntionboxTamperClose = "014"
EventID_MeterTamperOpen = "017"
EventID_MeterTamperClose = "018"

EventID_LocalTimeChangeSaved = "01F"
EventID_LocalPriceSaved = "020"
EventID_LocalTaxSaved = "021"
EventID_LocalUnitNumberSaved = "022"

EventID_FactorAChange = "025"
EventID_FactorBChange = "026"
EventID_EndServiceTimeChanged = "02B"
EventID_ResetCountersChange = "02C"
EventID_FlowDirectionChanged = "02F"
EventID_CalibrationChange = "03F"
EventID_ErrorFlow = "04C"
EventID_NoServiceInit = "04D"
EventID_NoServiceEnd = "04E"

EventID_MovilPriceSaved = "052"
EventID_MovilTaxSaved = "053"
EventID_LocalMinFlowChange = "054"
EventID_LocalMaxFlowChange = "055"
EventID_FactorPChange = "05F"
EventID_PathInit = "066"
EventID_PathEnd = "067"
EventID_LocalCancelService = "068"
EventID_OperationModeChanged = "077"


EventIDDescription = {EventID_RegisterInit: "Inicio de Registro",
                      EventID_DecimalPointUsageChanged: "Cambio en uso de punto decimal",
                      EventID_GPSPlantPositionChanged: "Cambio en posicion de planta",
                      EventID_TypeOfMeterChanged: "Cambio en el tipo de medidor",
                      EventID_MassModelChanged: "Cambio en el modelo masico",
                      EventID_TypeOfMeasureChanged: "Cambio en el tipo de medicion",
                      EventID_UnitMeasureChanged: "Cambio en la unidad de medida",
                      EventID_CompDensityChanged: "Cambio en la densidad de compensacion",
                      EventID_PasswordChanged: "Cambio de contrasena",
                      EventID_JuntionboxTamperOpen: "Caja de distribuidor abierto",
                      EventID_JuntionboxTamperClose: "Caja de distribuidor cerrado",
                      EventID_MeterTamperOpen: "Caja de sensor abierto",
                      EventID_MeterTamperClose: "Caja de sensor cerrado", 
                      EventID_LocalTimeChangeSaved: "Cambio de zona horaria", 
                      EventID_LocalPriceSaved: "Cambio local de precio",
                      EventID_LocalTaxSaved: "Cambio local de impuestos",
                      EventID_LocalUnitNumberSaved: "Cambio de numero de unidad",
                      EventID_FactorAChange: "Cambio de factor A",
                      EventID_FactorBChange: "Cambio de factor B",
                      EventID_EndServiceTimeChanged: "Cambio en tiempo fin de servicio",
                      EventID_ResetCountersChange: "Cambio en reseteo de contadores",
                      EventID_FlowDirectionChanged: "Cambio en el sentido de rotacion",
                      EventID_CalibrationChange: "Autocalibracion",
                      EventID_ErrorFlow: "Error de flujo durante servicio",
                      EventID_NoServiceInit: "Error de servicio sin inicio",
                      EventID_NoServiceEnd: "Fin de servicio sin inicio",
                      EventID_MovilPriceSaved: "Cambio externo de precio",
                      EventID_MovilTaxSaved: "Cambio externo de impuesto",
                      EventID_LocalMinFlowChange: "Cambio de flujo minimo",
                      EventID_LocalMaxFlowChange: "Cambio de flujo maximo",
                      EventID_FactorPChange: "Cambio de factor P",
                      EventID_PathInit: "Inicio de ruta",
                      EventID_PathEnd: "Fin de ruta",
                      EventID_LocalCancelService: "Servicio local cancelado",
                      EventID_OperationModeChanged: "Cambio en modo de operacion"
                      }


EventDataTypeOfAnswer = {EventID_RegisterInit:0, EventID_DecimalPointUsageChanged:1, EventID_GPSPlantPositionChanged:1, EventID_TypeOfMeterChanged:1, 
                         EventID_MassModelChanged:1, EventID_TypeOfMeasureChanged:1, EventID_UnitMeasureChanged:1, EventID_CompDensityChanged:1, 
                         EventID_PasswordChanged:1, EventID_JuntionboxTamperOpen:0, EventID_JuntionboxTamperClose:0, EventID_MeterTamperOpen:0, 
                         EventID_MeterTamperClose:0, EventID_LocalTimeChangeSaved:1, EventID_LocalPriceSaved:1, EventID_LocalTaxSaved:1, 
                         EventID_LocalUnitNumberSaved:1, EventID_FactorAChange:1, EventID_FactorBChange:1, EventID_EndServiceTimeChanged:1, 
                         EventID_ResetCountersChange:0, EventID_FlowDirectionChanged:1, EventID_CalibrationChange:0, EventID_ErrorFlow:0, 
                         EventID_NoServiceInit:0, EventID_NoServiceEnd:0, EventID_MovilPriceSaved:0, EventID_MovilTaxSaved:0,
                         EventID_LocalMinFlowChange:0, EventID_LocalMaxFlowChange:0, EventID_FactorPChange:1, EventID_PathInit: 0,
                         EventID_PathEnd:0, EventID_LocalCancelService:0, EventID_OperationModeChanged:0, }

#************************************************
def CheckSum(Data):
    Total=0
    for i in range(len(Data)):
        Total+=ord(Data[i])    
    Total = (Total&0xff)
    
    Checksum  = DecToAscii(Total>>4)
    Checksum += DecToAscii(Total&0x0f)
    return Checksum
#************************************************
def EncryptFrame(Data,Key):
    Resultado=""    
    for i in range(len(Data)):        
        if(Key<ord(Data[i])):
            Key = ord(Data[i])            
        Key ^= ord(Data[i])
        #print Data[i]
        #print Key
        Resultado+=chr(Key)                    
    return Resultado        
#**************************************************
def DencryptFrame(Data,Key):
    Resultado=""        
    for i in range(len(Data)):
        Dato = ord(Data[i])^Key
        Key ^= Dato
        Resultado += chr(Dato)
    return Resultado
#**************************************************
def Defrag(String):
        StartOfFrame = String.find('\02')
        EndOfFrame = String.find('\03')
        Key = ((ord(String[StartOfFrame+7])&0x0f) + (ord(String[EndOfFrame-1])&0xf0))
        Key ^= 0x39
        Size = EndOfFrame - StartOfFrame - 18
        DataToDencrypt = DencryptFrame(String[StartOfFrame+8:StartOfFrame+8+5+Size],Key)
        return String[StartOfFrame:StartOfFrame+8]+DataToDencrypt+String[-6:]
#**************************************************
def DecToAscii(Data):
    if(Data<10):
        return chr(Data+48)
    else:
        return chr(Data+65-10)
#**************************************************
def AsciiToDec(Data):
    if(Data>='0' and Data <='9'):
        return ord(Data)-ord('0')
    elif(Data>='A' and Data <='Z'):
        return ord(Data)-ord('A')+10
#**************************************************
def delay(SecDelay):
    start = time.clock()
    elapsed=0
    while (elapsed<SecDelay):
        elapsed = (time.clock() - start)
    return
#**************************************************
def CRC16(wCRC, Data):
    wCRC^=(ord(Data))<<8
    for i in range(8):
        #print i
        if (wCRC&0x8000):
            wCRC=(wCRC<<1)^0x1947
        else:
            wCRC<<=1
    return wCRC&0xFFFF
#**************************************************
def CRC16CCITT(string, count):
    Temp=0x0000
    for i in range(count):
        Temp=CRC16(Temp,string[i])		
    return Temp
#**************************************************
def memcpy(String1,String2,Init,Size):    
    String2 = String1[0:Init] + String2
    return String2
#**************************************************
def ConsecutiveToASCII(Consecutive):
    ConsecutiveInASCII = str(Consecutive)
    LenghtEventConsecutive = len(ConsecutiveInASCII)    
    StringResultant = memcpy("0000",ConsecutiveInASCII,4-LenghtEventConsecutive,LenghtEventConsecutive)
    return StringResultant   

#**************************************************
#Function that generates the frame wich contains the command, unit and data
def SendCommand(Command,SubCommand,Data,Unit):
        global Token                
        DataSize = len(Data)

        #Token increment
        Token += 1
        if Token > 99:
            Token = 0

        Key = random.randint(0,65535)
        Key |= 0x8080

        Seed = (((Key>>8)&0x0f)+(Key&0xf0))
        Seed ^= 0x39
                
        Frame = Unit
        Frame += "%02d"%Token
        Frame += chr(Key>>8)

        #Encrypt only the command, subcommand, data size and data
        FrameToEncrypt = Command + SubCommand + "%03X"%DataSize

        if(DataSize > 0):
            FrameToEncrypt += Data

        #Encrypt the frame
        EncryptedFrame = EncryptFrame(FrameToEncrypt,Seed)

        Frame += EncryptedFrame
        CRCTemp = CRC16CCITT(Frame,len(Frame))
        
        Frame += DecToAscii((CRCTemp>>12)&0x0f)
        Frame += DecToAscii((CRCTemp>>8)&0x0f)
        Frame += DecToAscii((CRCTemp>>4)&0x0f)
        Frame += DecToAscii((CRCTemp)&0x0f)

        #Form the structure of the Frame
        Frame = chr(0x02) + Frame + chr(Key&0xf0) + chr(0x03)

        s.write(Frame)
        recv = s.read(350)

        #Check for frames received      
        if( recv.find(chr(0x02))>=0 and recv.find(chr(0x03))>0):
            if( recv.find(chr(0x02)) < recv.find(chr(0x03))):
                StartOfFrame = recv.find('\02')
                EndOfFrame = recv.find('\03')
                CRCOfFrame = recv[EndOfFrame-5:EndOfFrame-1]
                #CRCCalculated = "%04X"%(mycrc.CRC16CCITT(recv[StartOfFrame+1:EndOfFrame-5]))
                CRCCalculated = CRCOfFrame
                if(CRCOfFrame == CRCCalculated):
                    if(Token == int(recv[StartOfFrame+5:StartOfFrame+7])):
                        return Defrag(recv)
                else:
                    print "Error de CRC"
                    return Defrag(recv)
            else:
                s.flushInput()
        else:
            s.flushInput()
            return ""
#************************************************
def EventRecovery():
    global Event_InitPointer
    global Event_EndPointer
    global Event_MaxNumEvents
    global Event_ConsecutiveASCII

    Event_Counter = 0
    Event_Pointer = Event_InitPointer

    if (Event_TotalEvents > 0):
        while (Event_Counter  < Event_TotalEvents) :                    
            Event_ConsecutiveASCII = ConsecutiveToASCII(Event_Pointer)
            Event_Pointer+=1
            Event_Counter+=1
            #Aseguramos que si el equipo sobre pasa el maximo numero de Eventos, y quedan Eventos pendientes
            if(Event_Pointer > (Event_MaxNumEvents - 1)):
                Event_Pointer = 0
            #Obtain Event
            FrameReceived = SendCommand("C","1",Event_ConsecutiveASCII,TerminalStr)
            try:
                AnalizeEventData(FrameReceived)                        
            except:                             
                FileMessage = "Error en el evento: " + str(Event_ConsecutiveASCII)
                f.write(FileMessage)                
                f.write(FrameReceived)
            delay(1)
    else:
        print "No hay Eventos por recuperar"

def GetMaxNumOfEvents():
    global Event_MaxNumEvents
    global MaxNumEventsObtainedFlag

    #Get max number of Events 
    FrameReceived = SendCommand("C","2"," ",TerminalStr)    
    Event_MaxNumEvents = FrameReceived[13:17]      
    try:          
        Event_MaxNumEvents = int(Event_MaxNumEvents)
        MaxNumEventsObtainedFlag = 1
    except:
        print "Error in obtaining the max number of Events"
        MaxNumEventsObtainedFlag = 0

    delay(1)

#************************************************
def MainDriver():
    global Token
    global ClearScreen
    global Event_InitPointer
    global Event_EndPointer
    global Event_TotalEvents
    global Event_MaxNumEvents
    global Event_ConsecutiveASCII
    global MaxNumEventsObtainedFlag

    if(ClearScreen == 1):
        WConio.clrscr()
        print "Press: "
        print "1. Obtain a specific consecutive Event"
        print "2. Obtain a bunch of Events"
        print "3. Match init-end Event pointers"
        print "4. Recover all pending Events"
        print "C. Clear screen"
        ClearScreen = 0

    delay(1)

    if WConio.kbhit():
        key = WConio.getche()
        Token = 0

        if key[1]=='1':                
            #Get Max number of events to validate inputs
            GetMaxNumOfEvents()
            if( MaxNumEventsObtainedFlag ):
                #Obtain event
                Event_Pointer = int(raw_input("\n\rInsert consecutive Event: "))
                #Validate range of events
                if( (Event_Pointer >= Event_MaxNumEvents) or (Event_Pointer < 0) ):
                    print "El consecutivo esta fuera de rango"
                else:
                    Event_ConsecutiveASCII = ConsecutiveToASCII(Event_Pointer)
                    FrameReceived = SendCommand("C","1",Event_ConsecutiveASCII,TerminalStr)
                    try:
                        AnalizeEventData(FrameReceived)                        
                    except:                                             
                        FileMessage = "Error en el evento: " + Event_ConsecutiveASCII
                        f.write(FileMessage)
                        FileMessage = "Trama:", FrameReceived
                        f.write(FileMessage)
                    delay(1)

        elif key[1]=='2':
            ConsecutiveInRangeFlag = 0
            #Get Max number of events to validate inputs
            GetMaxNumOfEvents()

            #Do only if the max num of events has been obtained
            if( MaxNumEventsObtainedFlag ):
                Event_InitPointer = int(raw_input("\n\rInsert init consecutive Event: "))
                #Validacion de rango
                if( (Event_InitPointer >= Event_MaxNumEvents) or (Event_InitPointer < 0) ):            
                    print "El consecutivo esta fuera de rango"                
                else:
                    ConsecutiveInRangeFlag = 1

                if( ConsecutiveInRangeFlag ):
                    ConsecutiveInRangeFlag = 0
                    Event_EndPointer = int(raw_input("Insert end consecutive Event: "))
                    #Validacion de rango
                    if( (Event_EndPointer >= Event_MaxNumEvents) or (Event_EndPointer < 0) ):            
                        print "El consecutivo esta fuera de rango"                    
                    else:
                        ConsecutiveInRangeFlag = 1

                #If both consecutives are in range    
                if(ConsecutiveInRangeFlag):
                    #Obtain total Events
                    Event_TotalEvents = Event_EndPointer - Event_InitPointer
                    if(Event_TotalEvents < 0):  
                        Event_TotalEvents = (Event_MaxNumEvents - Event_InitPointer) + Event_EndPointer

                    EventRecovery()

        elif key[1]=='3':
            FrameReceived = SendCommand("B","G","EV",TerminalStr)
            print "Pointers have been matched"
        elif key[1]=='4':             
            #Get Max number of events to validate inputs
            GetMaxNumOfEvents()
            #Do only if the max num of events has been obtained
            if( MaxNumEventsObtainedFlag ):
                f.write(str(Event_MaxNumEvents))
                #Get initial and end pointer    
                FrameReceived = SendCommand("C","0"," ",TerminalStr)
                Event_InitPointer = FrameReceived[13:17]
                Event_InitPointer = int(Event_InitPointer)
                Event_EndPointer = FrameReceived[17:21]
                Event_EndPointer = int(Event_EndPointer)           
                
                #Number of pending Events
                Event_TotalEvents = Event_EndPointer - Event_InitPointer
                
                #Verificar que el apuntador no haya dado la vuelta 
                if(Event_TotalEvents < 0):
                    Event_TotalEvents = (Event_MaxNumEvents - Event_InitPointer) + Event_EndPointer   

                #Print Event information on screen and in a log file
                FileMessage = "|Pending Events: " + str(Event_TotalEvents) + "|Initial Pointer: " + str(Event_InitPointer) + "|End Pointer: " + str(Event_EndPointer)    
                print FileMessage
                f.write(FileMessage)
                delay(1)

                #EventRecovery Routine
                EventRecovery()   
 
        elif key[1]=='c':
            ClearScreen = 1
#************************************************
def AnalizeEventData(EventStruct):
    #Unit
    Event_Unit = EventStruct[1:5] 
    #Consecutivo
    Event_Consecutive = EventStruct[13:17]
    #Fecha y hora	
    Event_Hora = str(EventStruct[17:19]) + ":" + str(EventStruct[19:21]) + ":" + str(EventStruct[21:23])
    Event_Fecha = str(EventStruct[23:25]) + "/" + str(EventStruct[25:27]) + "/" + str(EventStruct[27:29]) 
    #Identificador
    Event_ID = EventStruct[29:32]
    #Event Description
    try:
        Event_Description = EventIDDescription[Event_ID]
    except:
        Event_Description = "No description"    

    #Type of answer of each event ID
    try:
        Event_TypeOfAnswer = EventDataTypeOfAnswer[Event_ID]
    except:
        Event_TypeOfAnswer = 0        

    #When the event has not user in the answer 
    if(Event_TypeOfAnswer == 0):
        Event_Data = EventStruct[51:86]
    #When the event has user in the answer         
    elif(Event_TypeOfAnswer == 1):
        Event_Data = "| User: " + EventStruct[51:57] + "| Data: " + EventStruct[57:86]


    #print "| Unidad: " + Event_Unit + "| Consecutivo: " + Event_Consecutive + "| Fecha y hora: " + Event_Fecha + " " + Event_Hora + "| Evento:" + EventIDDescription[Event_ID] + "| Datos:" + Event_Data
    print "| Unidad: " + Event_Unit + "| Consecutivo: " + Event_Consecutive + "| Fecha y hora: " + Event_Fecha + " " + Event_Hora  + "| Evento:" + Event_ID + "| Description: " + Event_Description + "| Datos:" + Event_Data

#************************************************
if __name__ == '__main__':
    global Token
    global ClearScreen
    old_setting = WConio.gettextinfo()[4] & 0x00FF
    Puerto= int(raw_input("COM Serial Port number (Ej: \"06\"):"))-1
    Unid= int(raw_input("Numero de unidad (Ej: \"0090\"): "))
    TerminalStr = "%04d"%Unid
    ProdCounter = 0
    
    try:
        s = serial.Serial(Puerto, 38400)
        s.timeout=0.8
        s.setRTS(level=1)         # RTS arriba
        s.setDTR(level=1)         # DTR arriba
    
    except serial.SerialException:
        #-- Error al abrir el puerto serie
        sys.stderr.write("Error opening com por COM%s" % str(Puerto+1))
        sys.exit(1)
    
    print "Puerto COM (%d) abierto... a 9600bps"%(Puerto+1)   

    #Open file to record the log
    f = open("EventsReportLog.txt","a")

    #Clear screen
    ClearScreen = 1

    while 1:
        MainDriver()

    WConio.textattr(WConio.LIGHTGREEN)



    

