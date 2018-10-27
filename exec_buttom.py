import subprocess 
import sys
import os
import signal
import sys
import subprocess
import Adafruit_BBIO.GPIO as GPIO
import time

GPIO.setup("P8_12", GPIO.IN)
old_switch_state = 0

class GracefulKiller:
	kill_now = False
	def __init__(self):
		signal.signal(signal.SIGINT, self.exit_gracefully)
		signal.signal(signal.SIGTERM, self.exit_gracefully)

	def exit_gracefully(self,signum, frame):
		self.kill_now = True

if __name__ == '__main__':
	killer = GracefulKiller()
	while True:
		p1 = subprocess.Popen(['/root/iio-non/hx711'])
		if killer.kill_now:
			p1.terminate()

	print "End of the program. I was killed gracefully :)"






#os.system("/root/iio-non/hx711 &")
#os.system("./bmp180")
#os.system("./hmc5883l")
#os.system("./mpu6050")
#os.system("./ads1115")
#os.system("./am335x-adc &")



#os.startfile('/root/iio-non/hx711')
#subprocess.call(["g++", "hx711.cpp"])
