import sys
import math
import RPi.GPIO as GPIO
from time import sleep
from micropyGPS import MicropyGPS

GPIO.setmode(GPIO.BCM) 
port = 21
r_earth=6356752
lenx = r_earth*math.cos(35.7*math.pi/180)*2*math.pi/360
leny = r_earth*2*math.pi/360
print (lenx,leny)
GPIO.setup(port, GPIO.OUT)
my_gps = MicropyGPS(9, 'dd')
s = sys.stdin
ix = 0
iy = 0
try:
  while (1==1):
    for x in s.readline():
      my_gps.update(x)
    x = my_gps.longitude[0]+0.0
    y = my_gps.latitude[0]+0.0
    if ix == 0:
      ix = x
    if iy == 0:
      iy = y
    dx = (x - ix) * lenx
    dy = (y - iy) * leny
    sd = my_gps.fix_stat
    if (sd == 4):
      blen = 0.1
    elif (sd == 5):
      blen = 0.15 
    else:
      blen = 0.25
    if(dx*dx + dy*dy >= 1):
      GPIO.output(port, GPIO.HIGH)
      sleep(blen)
      GPIO.output(port, GPIO.LOW)
      ##print("\a")
      l = 1/math.sqrt(dx*dx+dy*dy)
      if(l > 2):
        ix = x
        iy = y
      else:
        ix = ix + (x - ix) * l
        iy = iy + (y - iy) * l
      print(sd,'{:.2f}'.format(dx),'{:.2f}'.format(dy)) 
    if(sd != 4): 
      print(sd,'{:.2f}'.format(dx),'{:.2f}'.format(dy)) 

except KeyboardInterrupt:
  GPIO.cleanup()
  sys.exit()
