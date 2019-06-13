Run hidemouse
;
chk=0


^h::
	if (chk==0)	
    	{
 		Process, Close, AutoHideMouseCursor_p.exe 
		WinActivate, ahk_exe Home IO.exe
		WinRestore, ahk_exe Home IO.exe
		chk=1
	}
	else
	{
		Run hidemouse
		WinActivate, ahk_exe Home IO.exe
		WinRestore, ahk_exe Home IO.exe	
		chk=0
	}
return


^m::
	{	
	Run, pythonw C:\Users\chris\Documents\Arduino\Wavelet\mouse.py
	WinActivate, ahk_exe Home IO.exe
	WinRestore, ahk_exe Home IO.exe
	}


return

;B1: 84,244 
;B2: 124,245
;B3: 84,284
;B4: 124,285
