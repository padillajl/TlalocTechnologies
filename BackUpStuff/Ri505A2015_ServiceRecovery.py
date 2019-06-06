#This script recovers all the services from the device Ri50A2015 and Ri505A2014, with this is possible to use the following features:
# 1) Obtain a specific consecutive service pressing key "1"
# 2) Obtain a bunch of services introducing the init consecutive and the end consecutive, for this feature is needed to press key "2"
# 3) Match init - end service pointers pressing key "3"
# 4) Recover all pending services, this feature reads the initial pointer and the end pointer if both are different will be recovered until reach the end pointer
#    This feature is enabled pressing key "4", this feature does not match init an end pointer
# 5) Set init service pointer to an specific one pressing key "5"
# 6) Clear screen pressing "c"    

#Version log 
#1v0 - 29/05/19 - Initial version

import time
import serial
import WConio
import sys
import struct
import random


SS_Consecutive_Size = 4
SS_InitTime_Size = 12
SS_EndTime_Size = 12
SS_Volume_Size = 8
SS_ClientAccount_Size = 12
SS_GPSLocation_Size = 19
SS_PayForm_Size = 1
SS_TaxID_Size = 1
SS_PriceNumber_Size = 2
SS_UserNumber_Size = 6
SS_Alarms_Size = 4
SS_TaxApplied_Size = 3
SS_PriceApplied_Size = 8
SS_Route_Size = 4
SS_Odometer_Size = 6
SS_ComPriceApplied_Size = 8
SS_ControlNumber_Size = 10
SS_Mass_Size = 8
SS_Density_Size = 5
SS_Temperature_Size = 4
SS_Validator_Size = 2
SS_Consecutive2_Size = 4
SS_UnitsConfiguration_Size = 1
SS_VolumeTotalizer_Size = 8
SS_MassTotalizer_Size = 8
SS_ServiceTotal_Size = 12
SS_ServiceTax_Size = 12
SS_ServiceSubTotal_Size = 12
SS_IEPSApplied_Size = 8
SS_TotalIEPS_Size = 12
SS_UnitPriceWithTax_Size = 8
SS_UnitPriceWithoutTax_Size = 8

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
    LenghtServiceConsecutive = len(ConsecutiveInASCII)    
    StringResultant = memcpy("0000",ConsecutiveInASCII,4-LenghtServiceConsecutive,LenghtServiceConsecutive)
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
def ServiceRecovery():
    global Service_InitPointer
    global Service_EndPointer
    global Service_MaxNumServices
    global Service_ConsecutiveASCII

    Service_Counter = 0
    Service_Pointer = Service_InitPointer

    if (Service_TotalServices > 0):
        while (Service_Counter  < Service_TotalServices) :                    
            Service_ConsecutiveASCII = ConsecutiveToASCII(Service_Pointer)
            Service_Pointer+=1
            Service_Counter+=1
            #Aseguramos que si el equipo sobre pasa el maximo numero de servicios, y quedan servicios pendientes
            if(Service_Pointer > (Service_MaxNumServices - 1)):
                Service_Pointer = 0
            #Obtain service
            Tramarecv = SendCommand("C","5",Service_ConsecutiveASCII,TerminalStr)
            try:
                AnalizeServiceData(Tramarecv)                        
            except:                             
                FileMessage = "Error en el servicio: " + str(Service_ConsecutiveASCII)
                f.write(FileMessage)                
                f.write(Tramarecv)
            delay(1)
    else:
        print "No hay servicios por recuperar"

#************************************************
def GetMaxNumOfServices():
    global Service_MaxNumServices
    global MaxNumServicesObtainedFlag

    #Get max number of Services 
    FrameReceived = SendCommand("C","3"," ",TerminalStr)    
    Service_MaxNumServices = FrameReceived[13:17]      
    try:          
        Service_MaxNumServices = int(Service_MaxNumServices)
        MaxNumServicesObtainedFlag = 1
    except:
        print "Error in obtaining the max number of Services"
        MaxNumServicesObtainedFlag = 0

    delay(1)        

#************************************************
def MainDriver():
    global Token
    global ClearScreen
    global Service_InitPointer
    global Service_EndPointer
    global Service_TotalServices
    global Service_MaxNumServices
    global Service_ConsecutiveASCII
    global MaxNumServicesObtainedFlag

    if(ClearScreen == 1):
        WConio.clrscr()
        print "Press: "
        print "1. Obtain a specific consecutive service"
        print "2. Obtain a bunch of services"
        print "3. Match init-end service pointers"
        print "4. Recover all pending services"
        print "5. Set init service pointer to an specific one"
        print "C. Clear screen"
        ClearScreen = 0

    delay(1)

    if WConio.kbhit():
        key = WConio.getche()
        Token = 0
        if key[1]=='1':
            #Get Max number of Services to validate inputs   
            GetMaxNumOfServices()
            #Do only if the max num of Services has been obtained
            if( MaxNumServicesObtainedFlag ):            
                #Obtain service
                Service_Pointer = int(raw_input("\n\rInsert consecutive service: "))
                #Validate range of Services
                if( (Service_Pointer >= Service_MaxNumServices) or (Service_Pointer < 0) ):
                    print "El consecutivo esta fuera de rango"
                else:
                    Service_ConsecutiveASCII = ConsecutiveToASCII(Service_Pointer)
                    Tramarecv = SendCommand("C","5",Service_ConsecutiveASCII,TerminalStr)
                    try:
                        AnalizeServiceData(Tramarecv)                        
                    except:                                             
                        FileMessage = "Error en el servicio: " + Service_ConsecutiveASCII
                        f.write(FileMessage)
                        FileMessage = "Trama:", Tramarecv
                        f.write(FileMessage)
                    delay(1)

        elif key[1]=='2':
            ConsecutiveInRangeFlag = 0
            #Get Max number of Services to validate inputs   
            GetMaxNumOfServices()
            #Do only if the max num of Services has been obtained
            if( MaxNumServicesObtainedFlag ): 

                Service_InitPointer = int(raw_input("\n\rInsert init consecutive service: "))
                #Range validation
                if( (Service_InitPointer >= Service_MaxNumServices) or (Service_InitPointer < 0) ):            
                    print "El consecutivo esta fuera de rango"                
                else:
                    ConsecutiveInRangeFlag = 1

                if( ConsecutiveInRangeFlag ):
                    ConsecutiveInRangeFlag = 0                    
                    Service_EndPointer = int(raw_input("Insert end consecutive service: "))
                    #Validacion de rango
                    if( (Service_EndPointer >= Service_MaxNumServices) or (Service_EndPointer < 0) ):            
                        print "El consecutivo esta fuera de rango"                    
                    else:
                        ConsecutiveInRangeFlag = 1

                if( ConsecutiveInRangeFlag ):                        
                    #Obtain total services only if both consecutives are in range
                    Service_TotalServices = Service_EndPointer - Service_InitPointer
                    if(Service_TotalServices < 0):  
                        Service_TotalServices = (Service_MaxNumServices - Service_InitPointer) + Service_EndPointer
                    ServiceRecovery()

        elif key[1]=='3':
            Tramarecv = SendCommand("B","G","SE",TerminalStr)
            print "Pointers have been matched"
        elif key[1]=='4':           
            #Get Max number of Services to validate inputs   
            GetMaxNumOfServices()
            #Do only if the max num of Services has been obtained
            if( MaxNumServicesObtainedFlag ): 
                f.write(str(Service_MaxNumServices))
                delay(1)
                #Get initial and end pointer    
                Tramarecv = SendCommand("C","4"," ",TerminalStr)
                Service_InitPointer = Tramarecv[13:17]
                Service_InitPointer = int(Service_InitPointer)
                Service_EndPointer = Tramarecv[17:21]
                Service_EndPointer = int(Service_EndPointer)           
                
                #Number of pending services
                Service_TotalServices = Service_EndPointer - Service_InitPointer
                
                #Verificar que el apuntador no haya dado la vuelta 
                if(Service_TotalServices < 0):
                    Service_TotalServices = (Service_MaxNumServices - Service_InitPointer) + Service_EndPointer   

                #Print service information on screen and in a log file
                FileMessage = "|Pending services: " + str(Service_TotalServices) + "|Initial Pointer: " + str(Service_InitPointer) + "|End Pointer: " + str(Service_EndPointer)    
                print FileMessage
                f.write(FileMessage)
                delay(1)

                #ServiceRecovery Routine
                ServiceRecovery()   
        elif key[1]=='5':
            ConsecutiveInRangeFlag = 0
            #Get Max number of Services to validate inputs   
            GetMaxNumOfServices()
            #Do only if the max num of Services has been obtained
            if( MaxNumServicesObtainedFlag ):
                Service_InitPointer = int(raw_input("\n\rInsert init consecutive service: "))
                #Range validation
                if( (Service_InitPointer >= Service_MaxNumServices) or (Service_InitPointer < 0) ):            
                    print "El consecutivo esta fuera de rango"                
                else:
                    ConsecutiveInRangeFlag = 1
                    Service_ConsecutiveASCII = ConsecutiveToASCII(Service_InitPointer)
                    Tramarecv = SendCommand("B","G","SE"+Service_ConsecutiveASCII,TerminalStr)
                    print "Initial pointer has been set to consecutive: " + Service_ConsecutiveASCII
 
        elif key[1]=='c':
            ClearScreen = 1
#************************************************
def AnalizeServiceData(ServiceStruct):
    global Service_ConsecutiveASCII

    #Unit    
    ServiceStruct_Unit = ServiceStruct[1:5] 
    #Consecutivo
    ServiceStructIndex = 13
    Service_Consecutive = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_Consecutive_Size]    
    ServiceStructIndex = ServiceStructIndex + SS_Consecutive_Size #17
    #Fecha y hora inicial    
    ServiceInit_Hora = str(ServiceStruct[ServiceStructIndex:ServiceStructIndex+2]) + ":" + str(ServiceStruct[ServiceStructIndex+2:ServiceStructIndex+4]) + ":" + str(ServiceStruct[ServiceStructIndex+4:ServiceStructIndex+6]) + " "
    ServiceInit_Fecha = str(ServiceStruct[ServiceStructIndex+6:ServiceStructIndex+8]) + "/" + str(ServiceStruct[ServiceStructIndex+8:ServiceStructIndex+10]) + "/" + str(ServiceStruct[ServiceStructIndex+10:ServiceStructIndex+12]) 
    ServiceStructIndex = ServiceStructIndex + SS_InitTime_Size #29
    #Fecha y hora final    
    ServiceEnd_Hora = str(ServiceStruct[ServiceStructIndex:ServiceStructIndex+2]) + ":" + str(ServiceStruct[ServiceStructIndex+2:ServiceStructIndex+4]) + ":" + str(ServiceStruct[ServiceStructIndex+4:ServiceStructIndex+6]) + " "
    ServiceEnd_Fecha = str(ServiceStruct[ServiceStructIndex+6:ServiceStructIndex+8]) + "/" + str(ServiceStruct[ServiceStructIndex+8:ServiceStructIndex+10]) + "/" + str(ServiceStruct[ServiceStructIndex+10:ServiceStructIndex+12]) 
    ServiceStructIndex = ServiceStructIndex + SS_EndTime_Size #41
    #Volumen de servicio
    Service_Volume = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_Volume_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_Volume_Size #49
    #Cuenta de cliente
    Service_ClientAccount = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_ClientAccount_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_ClientAccount_Size #61
    #GPS Location
    Service_GPSLocation = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_GPSLocation_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_GPSLocation_Size #80
    #PayForm
    Service_Payform = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_PayForm_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_PayForm_Size
    #Tax Number
    Service_TaxNumber = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_TaxID_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_TaxID_Size
    #Price number
    Service_PriceNumber = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_PriceNumber_Size] 
    ServiceStructIndex = ServiceStructIndex+ SS_PriceNumber_Size
    #User Number 
    Service_UserNumber = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_UserNumber_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_UserNumber_Size
    #Alarms
    Service_Alarms = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_Alarms_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_Alarms_Size
    #Tax applied
    Service_TaxApplied = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_TaxApplied_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_TaxApplied_Size
    #Price applied
    Service_PriceApplied = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_PriceApplied_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_PriceApplied_Size
    #Route
    Service_Route = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_Route_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_Route_Size
    #Odometer
    Service_Odometer = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_Odometer_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_Odometer_Size
    #Com price applied
    Service_ComPriceApplied = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_ComPriceApplied_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_ComPriceApplied_Size
    #Control number
    Service_ControlNumber = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_ControlNumber_Size] 
    ServiceStructIndex = ServiceStructIndex+ SS_ControlNumber_Size
    #Mass
    Service_Mass = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_Mass_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_Mass_Size
    #Density
    Service_Density = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_Density_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_Density_Size
    #Temperatura
    Service_Temperature = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_Temperature_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_Temperature_Size
    #Validador
    Service_Validator = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_Validator_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_Validator_Size
    #Consecutivo 2
    Service_ConsecutiveTwo = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_Consecutive2_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_Consecutive2_Size
    #Units configuration
    Service_UnitsConfiguration = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_UnitsConfiguration_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_UnitsConfiguration_Size
    #Volume Totalizer
    Service_VolumeTotalizer = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_VolumeTotalizer_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_VolumeTotalizer_Size
    #Mass Totalizer
    Service_MassTotalizer = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_MassTotalizer_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_MassTotalizer_Size
    #Service Total
    Service_Total = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_ServiceTotal_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_ServiceTotal_Size
    #Tax Total
    Service_TaxTotal = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_ServiceTax_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_ServiceTax_Size 
    #SubTotal
    Service_SubTotal = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_ServiceSubTotal_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_ServiceSubTotal_Size
    #IEPS Value applied
    Service_IEPSApplied = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_IEPSApplied_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_IEPSApplied_Size
    #Service Total IEPS
    Service_TotalIEPS = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_TotalIEPS_Size]
    ServiceStructIndex = ServiceStructIndex+ SS_TotalIEPS_Size
    #Unitary price with taxes
    Service_UnitPriceWithTax = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_UnitPriceWithTax_Size]
    ServiceStructIndex = ServiceStructIndex+SS_UnitPriceWithTax_Size
    #Unitary price without tax
    Service_UnitPriceWithoutTax = ServiceStruct[ServiceStructIndex:ServiceStructIndex+SS_UnitPriceWithoutTax_Size]
    ServiceStructIndex = ServiceStructIndex+SS_UnitPriceWithoutTax_Size



    FileMessage = "******************************************************************************************************************"
    print FileMessage
    f.write(FileMessage)
    FileMessage = "| Apuntador de servicios: " + Service_ConsecutiveASCII
    print FileMessage
    f.write(FileMessage)
    FileMessage =  "| Unit: " + ServiceStruct_Unit + "| Consecutive: " + Service_Consecutive + "| Hora y fecha inicial: " + ServiceInit_Hora + ServiceInit_Fecha + "| Hora y fecha final: " + ServiceEnd_Hora + ServiceEnd_Fecha + "| Volumen: " + Service_Volume + "| Cuenta de cliente: " + Service_ClientAccount  + "| GPS Location: " + Service_GPSLocation +"| Forma de pago: " + Service_Payform
    print FileMessage
    f.write(FileMessage)
    FileMessage =  "| Numero de Impuesto: " + Service_TaxNumber + "| Numero de precio: " + Service_PriceNumber + "| Numero de usuario: " + Service_UserNumber + "| Tax aplicado: " + Service_TaxApplied + "| Precio aplicado: " + Service_PriceApplied +"| Ruta: " + Service_Route + "| Odometro: " + Service_Odometer + "| Precio aplicado: " + Service_ComPriceApplied + "| Numero de control: " + Service_ControlNumber + "| Masa: " + Service_Mass 
    print FileMessage
    f.write(FileMessage)
    FileMessage =  "| Densidad: " + Service_Density + "| Temperatura: " + Service_Temperature + "| Validador: " + Service_Validator + "| Consecutivo 2: " + Service_ConsecutiveTwo + "| Configuracion de unidades: " + Service_UnitsConfiguration + "| Totalizador de volumen: " + Service_VolumeTotalizer + "| Totalizador de masa: " + Service_MassTotalizer      
    print FileMessage
    f.write(FileMessage)
    FileMessage = "| Importe total: " + Service_Total + "| Importe impuestos: " + Service_TaxTotal + "| Importe SubTotal: " + Service_SubTotal + "| IEPS aplicado: " + Service_IEPSApplied + "| IEPS Total: " + Service_TotalIEPS + "| Precio unitario con impuestos: " + Service_UnitPriceWithTax + "| Precio unitario sin impuestos: " + Service_UnitPriceWithoutTax
    print FileMessage    
    f.write(FileMessage)
    FileMessage =  "\n\r******************************************************************************************************************"
    print FileMessage
    f.write(FileMessage)
     

#************************************************
if __name__ == '__main__':
    global Token
    global ClearScreen

    old_setting = WConio.gettextinfo()[4] & 0x00FF
    Puerto= int(raw_input("COM Serial Port number (Ej: \"06\"):"))
    Unid= int(raw_input("Numero de unidad (Ej: \"0090\"): "))
    TerminalStr = "%04d"%Unid
    ProdCounter = 0
    
    try:
        s = serial.Serial('COM' + str(Puerto), 38400)
        s.timeout=0.8
        #s.setRTS(level=1)         # RTS arriba
        #s.setDTR(level=1)         # DTR arriba
    
    except serial.SerialException:
        #-- Error al abrir el puerto serie
        sys.stderr.write("Error opening com por COM%s" % str(Puerto))
        sys.exit(1)
    
    print "Puerto COM (%d) abierto... a 9600bps"%(Puerto)   

    #Open file to record the log
    f=open("Services.txt","a")

    ClearScreen = 1

    #Run app
    while 1:
        MainDriver()

    WConio.textattr(WConio.LIGHTGREEN)

  

    

