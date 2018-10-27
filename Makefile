CFLAGS = -std=gnu++11
INCLUDE = -I/root/libiio-0.14/install/include
LFLAGS = -L/root/libiio-0.14/install/lib

all: hmc5883l bmp180 mpu6050 ads1115 hx711 am335x-adc hcsr04

iio-non: iio-non.cpp
	g++ ${CFLAGS} ${INCLUDE} ${LFLAGS} $< -o $@ -liio

hmc5883l: hmc5883l.cpp
	g++ ${CFLAGS} ${INCLUDE} ${LFLAGS} $< -o $@ -liio

iio-non-bmp180: iio-non-bmp180.cpp
	g++ ${CFLAGS} ${INCLUDE} ${LFLAGS} $< -o $@ -liio

bmp180: bmp180.cpp
	g++ ${CFLAGS} ${INCLUDE} ${LFLAGS} $< -o $@ -liio

iio-non-mpu6050: iio-non-mpu6050.cpp
	g++ ${CFLAGS} ${INCLUDE} ${LFLAGS} $< -o $@ -liio

mpu6050: mpu6050.cpp
	g++ ${CFLAGS} ${INCLUDE} ${LFLAGS} $< -o $@ -liio

ads1115: ads1115.cpp
	g++ ${CFLAGS} ${INCLUDE} ${LFLAGS} $< -o $@ -liio

hx711: hx711.cpp
	g++ ${CFLAGS} ${INCLUDE} ${LFLAGS} $< -o $@ -liio

am335x-adc: am335x-adc.cpp
	g++ ${CFLAGS} ${INCLUDE} ${LFLAGS} $< -o $@ -liio

hcsr04: hcsr04.cpp
	g++ ${CFLAGS} ${INCLUDE} ${LFLAGS} $< -o $@ -liio


clean:
	rm iio-non iio-non-hmc5883l iio-non-bmp180 iio-non-mpu6050 mpu6050 ads1115 hx711 am335x-adc

