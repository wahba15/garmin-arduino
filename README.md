# garmin-arduino

## Introduction
This repository will hold the code for the in-car display for altitude and other GPS information from Garmin.

* A RS232 to TTL converter is used for the garmin 18x LVC GPS
https://www.amazon.com/gp/product/B07BJJ1T5G/ref=ppx_yo_dt_b_asin_title_o08_s00?ie=UTF8&psc=1
The outputs from the converter are connected as follows:
	* Tx -> Pin 2
	* Rx -> Pin 3

* The I2C LCD display uses:
	* SDA -> Pin A4
	* SCL -> Pin A5

* The DHT22 is connected:
	* Data -> Pin 7