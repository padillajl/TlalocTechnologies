#Simulador Endress & Hauser 2018 
#Creado por Jose Luis Hernandez Padilla

import serial
import struct
import WConio
import time

print"Description:\n\r"
print"This script simulates completely the function of the mass meter Endress And Hauser"

Port = int(raw_input("\nPort Number COM"))-1

#Inicializacion
ModbusFrame_ID = 0
ModbusFrame_FunctionCode = 0
ModbusFrame_StartAddressReg = 0
ModbusFrame_NumberOfRegisters = 0
ModbusFrame_DataSize = 0
ModbusFrame_Data = 0


PressedKey = 0

Simulator_Console_Messages = '0'
Register_Values_Messages = '0'

TimerVar = 0
StartTimerVar = 0
TimerInSecondsVar = 1

TRUE = 1
FALSE = 0

MassMeterConfigured = FALSE
StartService = FALSE
PrefixedServiceVolumeValue = 0
PrefixedServiceQuantityToStock = 0 #Prefijar cantidad a surtir
PrefixedServiceMassValue = 0
PricePerLiter = 10
PrefixedService = 0


Endianness_LittleEndian = 1
Endianness_BigEndian = 2

Modbus_Baudrate = 9600
Modbus_ID = 0x01
Endianness = Endianness_BigEndian
Modbus_Parity_Even = 0
Modbus_Parity_Odd = 1
Modbus_Parity_None = 2
Modbus_Parity = Modbus_Parity_Even
#Modbus registers
#Variable registers
REG_VOL_TOTAL = 2809
REG_MASS_TOTAL = 2609
REG_DENSITY  = 2012
REG_TEMPERATURE  = 2016
REG_MASS_FLOW = 2006 
REG_VOL_FLOW = 2008
#Modbus configuration registers
REG_CFG_ASSIGN_TOTALIZER_ONE = 2600
REG_CFG_ASSIGN_TOTALIZER_TWO = 2800
REG_CLR_MASS_TOTAL = 2607
REG_CLR_VOL_TOTAL = 2807
REG_CFG_FLOAT = 4923
REG_CFG_INT = 4922
#Modbus parameter registers
REG_LOW_FLOW_CO_VAR = 5100
REG_LOW_FLOW_CO_VALUE = 5137
REG_CALIB_FACTOR_K = 7512
REG_MASS_FLOW_FACTOR = 5518
REG_VOL_FLOW_FACTOR = 5522
REG_DENS_FLOW_FACTOR = 5526
REG_SERIAL_NUMBER = 7002
REG_TYPE_SENSOR = 7011

String = '0'
Char = '0'

ModbusVariableValues = { REG_VOL_TOTAL: 0.0, REG_MASS_TOTAL: 0.0, REG_DENSITY: 0.80, REG_TEMPERATURE: 21.6, REG_MASS_FLOW: 12, REG_VOL_FLOW: 15}
ModbusParameterValues = { REG_LOW_FLOW_CO_VAR: 2, REG_LOW_FLOW_CO_VALUE: 2.4, REG_CALIB_FACTOR_K: 2.5, REG_MASS_FLOW_FACTOR: 1, REG_SERIAL_NUMBER: "D4081516000" , REG_TYPE_SENSOR: "PROMASS E" }
ModbusConfigValues = { REG_CFG_ASSIGN_TOTALIZER_ONE: 1, REG_CFG_ASSIGN_TOTALIZER_TWO: 2, REG_CLR_MASS_TOTAL: 1, REG_CLR_VOL_TOTAL: 1, REG_CFG_FLOAT: 1, REG_CFG_INT: 1}
TIMER_1 = 0
TIMER_2 = 1
TIMER_3 = 2
TimerVar = {TIMER_1:1, TIMER_2:5, TIMER_3:10}
StartTimerVar = {TIMER_1:0, TIMER_2:0, TIMER_3:0}

#CRC
global lo_bytes
lo_bytes = [
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40]

global hi_bytes
hi_bytes = [
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40]

def StartTimer(NumberOfTimer):	
	global StartTimerVar

	StartTimerVar[NumberOfTimer] = time.clock()	

def SetTimer(TimerInSeconds,NumberOfTimer):
	global TimerVar
	TimerVar[NumberOfTimer] = TimerInSeconds

def RefreshTimer(NumberOfTimer):	
	global StartTimerVar

	StartTimerVar[NumberOfTimer] = time.clock()	

def GetTimerStatus(NumberOfTimer):
	global TimerVar
	global StartTimerVar	

	elapsed = (time.clock() - StartTimerVar[NumberOfTimer])
	
	if(elapsed < TimerVar[NumberOfTimer]):
		return FALSE
	else:		
		elapsed=0		
		return TRUE
	
	
def ByteToHex( byteStr ):
	hex = []
	for aChar in byteStr:
		hex.append( "%02X " % ord( aChar ) )
	return ''.join( hex ).strip()

def CRC16(frame):
	crc_hi = 0xff
	crc_lo = 0xff
	for x in frame:
		crc_index = crc_hi ^ ord(x)
		crc_hi = crc_lo ^ hi_bytes[crc_index]
		crc_lo = lo_bytes[crc_index]
	return [crc_hi, crc_lo]

def FloatToIEEE(floatval):
	global Endianness
	global Endianness_BigEndian

	if Endianness == Endianness_LittleEndian:
		ieebytes = struct.pack("<f",floatval)
	elif Endianness == Endianness_BigEndian :
		ieebytes = struct.pack(">f",floatval)
	return ieebytes

def ShowMenuMessage():
	
	WConio.gotoxy(40,12)
	print "Main Menu Options, press: "
	WConio.gotoxy(40,13)
	print "---------------------------------------------------"
	WConio.gotoxy(40,14)
	print "P 	 	 - To Prefix Service in Liters"
	WConio.gotoxy(40,15)
	print "R 	 	 - To Prefix Service in Quantity To Stock ($)"
	WConio.gotoxy(40,16)
	print "T 		 - To Change the temperature"
	WConio.gotoxy(40,17)
	print "E 		 - To Change the Endianness"
	WConio.gotoxy(40,18)
	print "F 		 - To change the volume flow value"
	WConio.gotoxy(40,19)
	print "S 		 - To show the actual format of communication"	
	WConio.gotoxy(40,20)
	print "Space Bar - To begin/Finish Delivery"
	WConio.gotoxy(40,21)
	print "ESC 		 - To abort"
	WConio.gotoxy(40,22)
	print "---------------------------------------------------"
	WConio.gotoxy(40,23)


def ShowCommInfo():
	global Endianness
	global Endianness_BigEndian
	global Endianness_LittleEndian

	print "\n\rBaudrate: " + str(Modbus_Baudrate)
	print "Modbus ID: " 	+ str(Modbus_ID)
	#Endianness
	if Endianness == Endianness_BigEndian:
		print "Endianness: BigEndian"
	elif Endianness == Endianness_LittleEndian:
		print "Endianness: LittleEndian"

def AnalizeFrame(frame):

	global ModbusFrame_ID
	global ModbusFrame_FunctionCode
	global ModbusFrame_StartAddressReg
	global ModbusFrame_NumberOfRegisters
	global ModbusFrame_Data

	if (len(frame)< 5):
		return FALSE

	#Get Slave address
	ModbusFrame_ID = ord(frame[0])

	
	if (ModbusFrame_ID == Modbus_ID):
		#Validamos que sea integra la trama
#		print "\n\rAnalizing Frame: " + frame.encode('hex')
		crc= CRC16(frame[:-2])
		if (crc[0]==ord(frame[-2])) and (crc[1]==ord(frame[-1])):
			response=TRUE
		else:
			return FALSE
	else:
		return FALSE

	#Get Function code
	ModbusFrame_FunctionCode = ord(frame[1])
	#Modbus Register
	ModbusFrame_StartAddressReg = ord(frame[3])+ord(frame[2])*256

	if(ModbusFrame_FunctionCode == 0x03):
		ModbusFrame_NumberOfRegisters = ord(frame[5])+ord(frame[4])*256
	#Check for a write single register
	elif(ModbusFrame_FunctionCode == 0x06):
		ModbusFrame_Data = ord(frame[5])+ord(frame[4])*256
	#Check for a write multiple register
	elif(ModbusFrame_FunctionCode == 0x10):
		#Pendiente
		ModbusFrame_NumberOfRegisters = ord(frame[5])+ord(frame[4])*256
		ModbusFrame_DataSize = ord(frame[6])
		ModbusFrame_Data = ord(frame[8])+ord(frame[7])*256

	return TRUE

def CreateFrame():

	global ModbusFrame_ID
	global ModbusFrame_FunctionCode
	global ModbusFrame_StartAddressReg
	global ModbusFrame_NumberOfRegisters
	global ModbusFrame_Data
	global Simulator_Console_Messages
	global MassMeterConfigured
	
	Response = chr(ModbusFrame_ID)
	Response+=chr(ModbusFrame_FunctionCode)
	
	#Read register
	if (ModbusFrame_FunctionCode==0x03):
		print "Respondiendo 1.0"

		Response += chr((2*ModbusFrame_NumberOfRegisters)&0xFF)
		#Answer the information asked
							#Parameters
		#-----------------------------------------------------------------
		#Give the var to wich the low flow belongs
		if( ModbusFrame_StartAddressReg == REG_LOW_FLOW_CO_VAR ):
			Simulator_Console_Messages +=  "\n\r Variable of Low Flow Cut Off Read"
			Response += "\x00\x02"
		#Give the value of CutOff
		elif( ModbusFrame_StartAddressReg == REG_LOW_FLOW_CO_VALUE):
			Simulator_Console_Messages += "\n\r Value of Low Flow Cut Off Read"
			Response += FloatToIEEE(2.5)

		elif( ModbusFrame_StartAddressReg == REG_CALIB_FACTOR_K):
			Simulator_Console_Messages += "\n\r Factor K of Calibration Read"
			Response += FloatToIEEE(2.417)

		elif( ModbusFrame_StartAddressReg == REG_MASS_FLOW_FACTOR):
			Simulator_Console_Messages += "\n\r Mass, Volume and Density Factor Read"
			Response += "\x3f\x80\x00\x00\x00\x00\x00\x00\x3f\x80\x00\x00\x00\x00\x00\x00\x3f\x80\x00\x00"

		elif( ModbusFrame_StartAddressReg == REG_SERIAL_NUMBER):
			Simulator_Console_Messages += "\n\r Serial Number Read: D4081516000"
			#Example of serial number : "D4081516000" 
			Response += "\x44\x34\x30\x38\x31\x35\x31\x36\x30\x30\x30\x00\x00\x00\x00\x00\x00\x00\xFF\xFF"

		elif( ModbusFrame_StartAddressReg == REG_TYPE_SENSOR):
			#Example of type of sensor : "PROMASS E"
			Simulator_Console_Messages += "\n\r TypeSensor Read: PROMASS E"
			Response += "\x50\x52\x4F\x4D\x41\x53\x53\x20\x45\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFF"

			MassMeterConfigured = 1			
		#-----------------------------------------------------------------

							#Variables
		#-----------------------------------------------------------------
		#Get density and temp
		if( ModbusFrame_StartAddressReg == REG_DENSITY ):
			#Density   
			Response += FloatToIEEE(ModbusVariableValues[REG_DENSITY])
			#Blank Space
			Response += "\xFF\xFF\xFF\xFF"
			#Temperatura   
			Response += FloatToIEEE(ModbusVariableValues[REG_TEMPERATURE])			

		elif( ModbusFrame_StartAddressReg == REG_MASS_TOTAL):
			#Masa      
			Response += FloatToIEEE(ModbusVariableValues[REG_MASS_TOTAL])

		elif( ModbusFrame_StartAddressReg == REG_VOL_TOTAL):
			#Volumen
			Response += FloatToIEEE(ModbusVariableValues[REG_VOL_TOTAL])

		elif( ModbusFrame_StartAddressReg == REG_MASS_FLOW):
			Response += FloatToIEEE(ModbusVariableValues[REG_MASS_FLOW])
			Response += FloatToIEEE(ModbusVariableValues[REG_VOL_FLOW])			

	#Write single register
	elif (ModbusFrame_FunctionCode==0x06):
		
		Response += chr((ModbusFrame_StartAddressReg >> 8) & 0x00ff)
		Response += chr(ModbusFrame_StartAddressReg & 0x00ff)

		Response += chr((ModbusFrame_Data >> 8) & 0x00ff)
		Response += chr(ModbusFrame_Data & 0x00ff)

	#Write multiple registers	
	elif (ModbusFrame_FunctionCode==0x10):		

		#Start Address
		Response += chr((ModbusFrame_StartAddressReg >> 8) & 0x00ff)
		Response += chr(ModbusFrame_StartAddressReg & 0x00ff)
		#Number of registers
		Response += chr((ModbusFrame_NumberOfRegisters >> 8) & 0x00ff)
		Response += chr(ModbusFrame_NumberOfRegisters & 0x00ff)		

	#Operations
	if( ModbusFrame_StartAddressReg == REG_CFG_FLOAT):
		Simulator_Console_Messages += "\n\r Float Config Done!"

	elif( ModbusFrame_StartAddressReg == REG_CFG_INT):
		Simulator_Console_Messages += "\n\r Integer Config Done!"

	elif( ModbusFrame_StartAddressReg == REG_CFG_ASSIGN_TOTALIZER_ONE ):
		Simulator_Console_Messages += "\n\r Totalizer One Assigned As Mass!"

	elif( ModbusFrame_StartAddressReg == REG_CFG_ASSIGN_TOTALIZER_TWO ):		
		Simulator_Console_Messages += "\n\r Totalizer Two Assigned As Volume!"		
		#Clear totalizer 1
	elif( ModbusFrame_StartAddressReg == REG_CLR_MASS_TOTAL ):
		Simulator_Console_Messages += "\n\r Totalizer One of Mass Cleared!"
		ModbusVariableValues[REG_MASS_TOTAL] = 0.0
		#Clear Totalizer 2
	elif( ModbusFrame_StartAddressReg == REG_CLR_VOL_TOTAL ):
		Simulator_Console_Messages += "\n\r Totalizer Two of Volume Cleared!"
		ModbusVariableValues[REG_VOL_TOTAL] = 0.0


		
	crc = CRC16(Response)
	Response+=chr(crc[0])
	Response+=chr(crc[1])
	
	return  Response

def PrintScreen():
	global Simulator_Console_Messages
	global Register_Values_Messages
	global MassMeterConfigured
	global PressedKey

	WConio.textattr(WConio.LIGHTCYAN)
	out_string = "Pegasus Control Endress And Hauser Simulator running on port : COM" + str(Port+1) 
	WConio.gotoxy(1,1)
	WConio.cputs(out_string)

	#Console text includes Density, Temperature, Mas Totalizer, Volume Totalizer, Mass Flow, Volume Flow
	out_string1="%1.4f  " % ModbusVariableValues[REG_DENSITY]
	out_string2="%3.4f  " % ModbusVariableValues[REG_TEMPERATURE]
	out_string3="%3.4f  " % ModbusVariableValues[REG_MASS_TOTAL]
	out_string4="%3.4f  " % ModbusVariableValues[REG_VOL_TOTAL]
	out_string5="%3.4f  " % ModbusVariableValues[REG_MASS_FLOW]
	out_string6="%3.4f  " % ModbusVariableValues[REG_VOL_FLOW]

	outstring8 =  '|         Console Messages:       |'
	WConio.gotoxy(1,3)
	WConio.cputs(outstring8)
	outstring8 =  '-----------------------------------'
	WConio.gotoxy(1,4)
	WConio.cputs(outstring8)
	WConio.cputs(Simulator_Console_Messages)
	
	ShowMenuMessage()

	if MassMeterConfigured == TRUE:
		Simulator_Console_Messages = "\n\r Configuration Done!"
		MassMeterConfigured = FALSE

	WConio.textattr(WConio.LIGHTGRAY)
	Register_Values_Messages = " |       Registers Values:       |"	
	WConio.gotoxy(40,3)
	WConio.cputs(Register_Values_Messages)
	Register_Values_Messages = " ---------------------------------"	
	WConio.gotoxy(40,4)
	WConio.cputs(Register_Values_Messages)
	Register_Values_Messages = " | Density : " + out_string1 				+ "		|"
	WConio.gotoxy(40,5)
	WConio.cputs(Register_Values_Messages)
	Register_Values_Messages = " | Temperature : " + out_string2 			+ " 	|"
	WConio.gotoxy(40,6)
	WConio.cputs(Register_Values_Messages)
	Register_Values_Messages = " | Mass Totalizer : " + out_string3 		+ " 	|"	
	WConio.gotoxy(40,7)
	WConio.cputs(Register_Values_Messages)
	Register_Values_Messages = " | Volume Totalizer : " + out_string4 		+ " 	|"	
	WConio.gotoxy(40,8)
	WConio.cputs(Register_Values_Messages)
	Register_Values_Messages = " | Mass Flow : " + out_string5 + " Kg/min" 	+ " 	|" 	
	WConio.gotoxy(40,9)
	WConio.cputs(Register_Values_Messages)
	Register_Values_Messages = " | Volume Flow : " + out_string6 + " L/min "+ " 	|"
	WConio.gotoxy(40,10)
	WConio.cputs(Register_Values_Messages)
	
	outstring7 = "Press space bar to begin/finish delivery. Press ESC to abort"
	WConio.gotoxy(40,25)
	WConio.cputs(outstring7)
	
	WConio.gotoxy(1,26)

if __name__ == '__main__':
	#Get communication port to use
	lwVolTotal = 0
	lwMassTotal = 0

	try:
		s = serial.Serial(Port,Modbus_Baudrate,parity=serial.PARITY_EVEN)
		s.timeout=0.010
		s.setRTS(level=1)         # RTS arriba
		s.setDTR(level=1)         # DTR arriba
	
	except serial.SerialException:
		#-- Error al abrir el puerto serie
		sys.stderr.write("Error opening com por COM%s" % str(Puerto+1))
		sys.exit(1)

	StartTimer(TIMER_2)
	SetTimer(5,TIMER_2)

	while 1:
		Char = s.read(20)

		if GetTimerStatus(TIMER_2) == TRUE:
			WConio.clrscr()
			RefreshTimer(TIMER_2)


		if (WConio.kbhit()):
			key= WConio.getch()			

			#Prefix service in liters
			if key[1]=="p":
				if StartService == FALSE:
					#Calculate values 
					PrefixedServiceVolumeValue = float(raw_input("Prefix Service in liters (lts): "))
					PrefixedServiceMassValue = PrefixedServiceVolumeValue *  ModbusVariableValues[REG_DENSITY]
					PrefixedService = 1					
			#Attention to space bar			
			elif key[0] == 32 :
				if StartService==TRUE:
					StartService=FALSE		
				elif StartService==FALSE:
					StartService=TRUE
					StartTimer(TIMER_1)
					SetTimer(TimerInSecondsVar,TIMER_1)

			#Change the temperature
			elif key[1] == "t":
				ModbusVariableValues[REG_TEMPERATURE] = float(raw_input("Type the new temperature value (C): "))
				print "The new temperature value is: " + str(ModbusVariableValues[REG_TEMPERATURE])				
			#Change the flow
			elif key[1] == "f":
				ModbusVariableValues[REG_VOL_FLOW] = float(raw_input("Type the new volume flow value (L/min): "))
				ModbusVariableValues[REG_MASS_FLOW] =  ModbusVariableValues[REG_VOL_FLOW] * ModbusVariableValues[REG_DENSITY]				
			#Change the Endianness
			elif key[1] == "e":
				EndiannessOption = int(raw_input("Press 1 To Set Little Endian or 2 for Big Endian"))
				if EndiannessOption == 1:
					Endianness = Endianness_LittleEndian
				elif EndiannessOption == 2:
					Endianness = Endianness_BigEndian				
			#Show the actual communication format
			elif key[1] == "s":
				ShowCommInfo()				
			#Prefix service in price
			elif key[1] == "r":
				PrefixedService = 1
				PricePerLiter = float(raw_input("\n\rType the price Per Liter ($): "))
				PrefixedServiceQuantityToStock = float(raw_input("Prefix Service Quantity to Stock ($): "))
				PrefixedServiceVolumeValue = PrefixedServiceQuantityToStock/PricePerLiter
				PrefixedServiceMassValue = PrefixedServiceVolumeValue *  ModbusVariableValues[REG_DENSITY]


		if StartService == TRUE:
			#Calculate the totalizers 
			if GetTimerStatus(TIMER_1) == TRUE:
				lwMassTotal = float(ModbusVariableValues[REG_MASS_FLOW]/float(60))
				lwVolTotal = float(ModbusVariableValues[REG_VOL_FLOW]/float(60))

				ModbusVariableValues[REG_MASS_TOTAL] += lwMassTotal
				ModbusVariableValues[REG_VOL_TOTAL] +=  lwVolTotal

				RefreshTimer(TIMER_1)
			#Check if the service is prefixed
			if PrefixedService == 1:
				if PrefixedServiceVolumeValue <= ModbusVariableValues[REG_VOL_TOTAL]:															
					#Finish service
					StartService = FALSE

					ModbusVariableValues[REG_MASS_TOTAL] = PrefixedServiceMassValue
					ModbusVariableValues[REG_VOL_TOTAL]  = PrefixedServiceVolumeValue
					
					#Restart the prefixed Service Volume
					PrefixedServiceVolumeValue = 0
					PrefixedService = 0
					Simulator_Console_Messages = ''
			
		#Decode the frame and create the answer
		if (len(Char) > 7):
			String = Char
			FrameAnalized = AnalizeFrame(String)
			#Reset the string
			String = '0'
			if FrameAnalized == 1:
				tramarespuesta = CreateFrame()
				print "\n\rEnviado: " , tramarespuesta.encode('hex')

				if(len(tramarespuesta) > 5):
					s.write(tramarespuesta)	

		PrintScreen()			
				
	s.close()