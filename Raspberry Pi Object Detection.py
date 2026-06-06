# Watch the video here: https://youtu.be/ieH1bMKlqek

from gpiozero import DistanceSensor, LED, Button
from RPLCD.i2c import CharLCD
from time import sleep


# Ultrasonic sensor
sensor = DistanceSensor(echo=26, trigger=19)


# LEDs
redLED = LED(17)
yellowLED = LED(27)
greenLED = LED(22)


# Push button
button = Button(13)


# LCD setup
lcd = CharLCD('PCF8574', 0x27)


systemOn = True
lastButtonState = False


while True:
   # Button toggle
   if button.is_pressed and not lastButtonState:
       systemOn = not systemOn
       lcd.clear()
       sleep(0.3)


   lastButtonState = button.is_pressed


   if not systemOn:
       redLED.off()
       yellowLED.off()
       greenLED.off()


       lcd.clear()
       lcd.write_string("System OFF")
       print("SYSTEM OFF")
       sleep(0.5)
       continue


   inches = sensor.distance * 100 / 2.54


   if 2 <= inches <= 8:
       redLED.on()
       yellowLED.off()
       greenLED.off()
       status = "RED ON"


   elif 9 <= inches <= 15:
       redLED.off()
       yellowLED.on()
       greenLED.off()
       status = "YELLOW ON"


   elif 16 <= inches <= 22:
       redLED.off()
       yellowLED.off()
       greenLED.on()
       status = "GREEN ON"


   else:
       redLED.off()
       yellowLED.off()
       greenLED.off()
       status = "OUT RANGE"


   lcd.clear()
   lcd.write_string(f"Dist: {inches:.1f} in")
   lcd.cursor_pos = (1, 0)
   lcd.write_string(status)


   print(f"Distance: {inches:.2f} in -- {status}")


   sleep(0.5)
