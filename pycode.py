
import serial
import pyautogui
import time
import msvcrt as m

chk = 0

print("Connecting to device... Please wait!\n")

while chk != 2:
	while chk == 0:
		try:
			print("Trying COM5 port...") 
			ser = serial.Serial('COM5', 9600, timeout=0)	#Opens Bluetooth Serial Port COM5
			chk=2
		except:
			print("COM 5 connection failed!\n")
			chk=1			

	while chk == 1:
		try:
			print("Trying COM6 port...")
			ser = serial.Serial('COM6', 9600, timeout=0)	#Opens Bluetooth Serial Port COM6
			chk=2
		except:
			print("COM 6 connection failed! Could not set up Bluetooth connection to device!")
			print("Press any key to retry... or try re-configuring program\n")
			chk=0
			m.getch()
 

print("\nConnection established successfully! Starting up...")
print("Press 'Ctrl+C' at any time to exit...\n")
time.sleep(1)

volstat=0

while 1:
	try:
		s=ser.readline()
	   	if "Z" in s:
			print("Right")
			pyautogui.typewrite(['right'])
	   	elif "Y" in s:
	   		print("Left")
			pyautogui.typewrite(['left'])
		elif "X" in s:
	   		print("VolChange")
			if volstat<=1:
				for x in range(15):
					pyautogui.typewrite(['volumeup'])
				volstat=volstat+1
			elif volstat<=3:
				for x in range(15):
					pyautogui.typewrite(['volumedown'])
				volstat=volstat+1
			else:	
				volstat=0
	   	else:
	    		print(s),
	   	time.sleep(0.05)

	except ser.SerialTimeoutException:
	  	print("Timed Out... Retrying...")
	  	time.sleep(0.5)