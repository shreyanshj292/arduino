import serial
import pydirectinput
ser = serial.Serial('COM5', 9600)
running = True
while running:
    value = int(ser.readline())
    val1 = value % 10000 #fire
    val = int((value - val1))
    val2 = int((val % 100000000)/10000) #left button
    val = int((val - val2))
    val3 = int((val % 1000000000000)/100000000) #Left horizontal
    val = int((val - val3))
    val4 = int((val % 10000000000000000)/1000000000000) #Left vertical
    val = int((val - val4))
    val5 = int((val % 100000000000000000000)/10000000000000000) #Rigth button
    val = int((val - val5))
    val6 = int((val % 1000000000000000000000000)/100000000000000000000) #Rigth horizontal
    val = int((val - val6))
    val7 = int((val % 10000000000000000000000000000)/1000000000000000000000000) #Right Vertical
    val = int((val - val7))
    val8 = int((val % 100000000000000000000000000000000)/10000000000000000000000000000) # reload val = 1990
    val = int((val - val8))
    val9 = int((val % 1000000000000000000000000000000000000)/100000000000000000000000000000000) # lv
    if val1 >= 1991:
        pydirectinput.leftClick()
    if val8 >= 1991:
        pydirectinput.press("r")
    if val2 == 1000:
        pydirectinput.press("l")
    if val5 == 1000:
        pydirectinput.press("e")
    if val4 >= 1750:
        pydirectinput.press("w")
    if val4 <= 1250:
        pydirectinput.press("s")
    if val3 >= 1750:
        pydirectinput.press("d")
    if val3 <= 1250:
        pydirectinput.press("a")