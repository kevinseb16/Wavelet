
import serial
import pyautogui
import time
import msvcrt as m
import re

while 1:
	chk = 0
	print("\nConnecting to device... Please wait!\n")
	while chk != 2:
		while chk == 0:
			try:
				print("Trying COM7 port...") 
				ser = serial.Serial('COM7', 9600, timeout=0)	#Opens Bluetooth Serial Port COM5
				chk=2
			except:
				print("COM 7 connection failed!\n")
				chk=1

		while chk == 1:
			try:
				print("Trying COM5 port...")
				ser = serial.Serial('COM5', 9600, timeout=0)	#Opens Bluetooth Serial Port COM6
				chk=2
			except:
				print("COM 5 connection failed!")
				print("Retrying... \n")
				chk=0
	 

	print("\nConnection established successfully! Starting up...")
	print("Press 'Ctrl+C' at any time to exit...\n")
	time.sleep(1)
	timer = 0
	h_move = 0
	v_move = 0
	pyautogui.FAILSAFE = False
	
	while timer < 50:
		try:
			
			s=ser.readline()
			h_move = 0
			v_move = 0
			try:
					match = re.search(r"X:(.*)Y", s)
					if match:
						h_move = float(match.group(1))
						print("v = "),
						print(v_move),
						timer = 0
					match = re.search(r"Y:(.*)Z", s)
					if match:
						v_move = -float(match.group(1))
						print("h = "),
						print(h_move)
						timer = 0
					pyautogui.moveRel(10*h_move,v_move)
					#if "CMDF" in s:
					#	print("<click>")
					#	pyautogui.click()
			
			except Exception as e:
				print("Error: "),
				print(e)
			
			
			#ser.flushInput()
			#time.sleep(0.01)
			timer += 1
		
		except ser.SerialTimeoutException:
			print("Timed Out... Retrying...")
			time.sleep(0.5)
	print("Connection idle... Retrying!")
	#time.sleep(1)