#!/usr/bin/env python

from dropbox import client, rest, session
import RPi.GPIO as GPIO, feedparser, time
import datetime
import sys, os
import subprocess
import spi


now1 = datetime.datetime.now()
fo = open(now1.strftime("%Y%m%d_%H%M%S"), "wb")
a = spi.SPI(0,0)
voltage = [0]*12
print "PY: Starting Program"

runCount = raw_input("Enter the number of iterations: ")
runCount = int(runCount)
speed1 = raw_input("Enter the delay between readings (in seconds): ")
speed = int(speed1)
details = raw_input("Enter test description: ")

fo.write("User Details:\n")
fo.write(details)
fo.write("\n")
fo.write("Delay between readings: ")
fo.write(speed1)
fo.write(" seconds")
fo.write("\n")
fo.write("\n")

#TLC2543 Details
# MSB D7 D6 D5 D4 D3 D2 D1 D0 LSB
#     x  x  x  x  1  1  0  0
#
# D4 through D7 are channel number
# Channel #s are binary
# Internal test are:
#       1011 --> (Vref+ - Vref- )/2
#       1100 --> Vref-
#       1101 --> Vref+
#       1110 --> Shutdown Mode
#
# 0xBC00 should return 2048 (half of Vref+)


data = ["0C00", "1C00", "2C00", "3C00", "4C00", "5C00", "6C00", "7C00", "8C00", "9C00", "AC00", "BC00"]
data2 = ["0000"]

#length_data = len(data[0])/2
length_data = 2

for i in range(1,runCount+1): 

	for i in range(1,13):
		
		a.transfer(data[i-1], length_data)
		#time.sleep(0.125)
		test = a.transfer(data2[0], length_data)
		var3 = int(test[2])
		voltage[i-1] = (var3*0.001221)
	

	print "%6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f" % (voltage[0], voltage[1], voltage[2], voltage[3], voltage[4], voltage[5], voltage[6], voltage[7], voltage[8], voltage[9], voltage[10], voltage[11])
	fo.write("%6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f" % (voltage[0], voltage[1], voltage[2], voltage[3], voltage[4], voltage[5], voltage[6], voltage[7], voltage[8], voltage[9], voltage[10], voltage[11]))
	fo.write("\n")
	time.sleep(speed/2)
    


a.close()
fo.close()

#*********************Below code uploads to dropbox*************#


APP_KEY = 'INSERT_APP_KEY_HERE'
APP_SECRET = 'INSERT_SECRET_HERE'
ACCESS_TYPE = 'app_folder'
TOKENS = 'dropbox_token.txt'

token_file = open(TOKENS)
token_key,token_secret = token_file.read().split('|')
token_file.close()

sess = session.DropboxSession(APP_KEY,APP_SECRET, ACCESS_TYPE )
sess.set_token(token_key,token_secret)
client = client.DropboxClient(sess)

f = open(now1.strftime("%Y%m%d_%H%M%S"))
response = client.put_file(now1.strftime("%Y%m%d_%H%M%S.txt"), f)
f.close()
