import time
import socket
import struct
import os
import sys
import subprocess

class mainFunction:
    def __init__(self):
        self.hardware = None
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.header = (0x12345678, 0)

        self.findHardware()

    def deactivate(self):
        self.sock = None
        return

    def findHardware(self):
        currentDir = sys._MEIPASS if getattr(sys, '_MEIPASS', None) else 'data'
        mapper = os.path.join(currentDir, 'Mapper.exe')
        driver = os.path.join(currentDir, 'mouseMoveDriver.sys')
        command = f"{mapper} {driver}"
        subprocess.run("sc stop faceit", shell=True)
        # subprocess.run(command, shell=True)

        time.sleep(1)

        try:
            self.sock.connect(('localhost', 6666))
        except:
            input("Driver\nError loading the driver, make sure using Windows 10 and Secure Boot off.")
            os._exit(1)

    def move(self, x, y, click="0"):
        try:
            memory_data = (int(x), int(y), 0)
            self.send_packet(self.header + memory_data)

            if click != "0":
                self.shoot()

        except Exception as e:
            print(f"Error in move method: {e}")

    def shoot(self):
        self.send_packet(self.header + (0, 0, 0x1))
        time.sleep(0.001)
        self.send_packet(self.header + (0, 0, 0x2))

    def send_packet(self, packet_data):
        packet_bytes = struct.pack('IIiii', *packet_data)
        self.sock.send(packet_bytes)
