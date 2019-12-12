
import serial
import pyautogui
import time
import msvcrt as m


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
				print("Trying COM3 port...")
				ser = serial.Serial('COM3', 9600, timeout=0)	#Opens Bluetooth Serial Port COM6
				chk=2
			except:
				print("COM 3 connection failed!")
				print("Retrying... \n")
				chk=2
	 

	print("\nConnection established successfully! Starting up...")
	print("Press 'Ctrl+C' at any time to exit...\n")
	time.sleep(1)
	timer = 0
	h_move = 0
	v_move = 0
	
	while timer < 50:
		delay = 0
		try:
			s=ser.readline()
			if "CMD1" in s:
				print("<tilt_backward>")
				pyautogui.typewrite(['up'])
			elif "CMD9" in s:
				print("<tilt_forward>")
				pyautogui.typewrite(['down'])
			elif "CMD2" in s:
				print("<tilt_right>")
				#pyautogui.hotkey('alt','tab')
				pyautogui.typewrite(['shiftleft'])
				#if v_move == 0: 
				#	v_move = 1
				#else:
				#	v_move = 0
			elif "CMD8" in s:
				print("<tilt_left>")
				#pyautogui.hotkey('win','tab')
				pyautogui.typewrite(['ctrlleft'])
				#if v_move == 0: 
				#	v_move = -1
				#else:
				#	v_move = 0
			elif "CMD3" in s:
				print("<tilt_ccw")
				pyautogui.typewrite(['left'])
				#if h_move == 0: 
				#	h_move = -1
				#else:
				#	h_move = 0
			elif "CMD7" in s:
				print("<tilt_cw>")
				pyautogui.typewrite(['right'])
				#if h_move == 0: 
				#	h_move = 1
				#else:
				#	h_move = 0
			elif "." in s:
				timer = 0
			else:
				delay = 1		
			
			if "CMDF" in s:
				print("<enter>")
				#pyautogui.press('f11')
			
			pyautogui.moveRel(20*h_move,20*v_move)
				
			time.sleep(0.05)
			#timer += delay
		
		except ser.SerialTimeoutException:
			print("Timed Out... Retrying...")
			time.sleep(0.5)
	print("Connection idle... Retrying!")