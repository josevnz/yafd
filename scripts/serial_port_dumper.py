#!/usr/bin/env python
"""
Simple script to dump the contents of a serial port (ideally your Arduino USB port)
Author: Jose Vicente Nunez (kodegeek.com@protonmail.com)
"""
import serial

BAUD = 9600
TIMEOUT = 2
PORT = "/dev/ttyUSB0"

if __name__ == "__main__":
    serial_port = serial.Serial(port=PORT, baudrate=BAUD, bytesize=8, timeout=TIMEOUT, stopbits=serial.STOPBITS_ONE)
    try:
        while True:
            # Wait until there is data waiting in the serial buffer
            if serial_port.in_waiting > 0:
                serialString = serial_port.readline()
                # Print the contents of the serial data
                print(serialString.decode('utf-8').strip())
    except KeyboardInterrupt:
        pass
