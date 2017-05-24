#!/usr/bin/python3

import serial
import re
import datetime
import logging

from time import sleep
from subprocess import Popen, PIPE

BAUD_RATE = 115200
LOG_FORMAT = '%(asctime)-15s %(message)s'

logging.basicConfig(format=LOG_FORMAT)
logger = logging.getLogger('serial_interface')

def discover_arduinos():
    """
    Discover all Arduinos connected to the system and return their device paths
    """
    serial_ids = Popen(['ls', '/dev/serial/by-id'], stdout=PIPE, stderr=PIPE)

    paths = []

    if serial_ids.returncode:
        return paths

    for id in serial_ids.stdout:
        id = id.decode('utf8').strip()

        if re.search(r"Arduino", id):
            sym_link = Popen(['file', '/dev/serial/by-id/' + id], stdout=PIPE)

            for link in sym_link.stdout:
                out = re.search(r"tty.*", link.decode('utf8'))
                if out:
                    paths.append(out.group(0))

    return paths


def read_data(device_path):
    """
    Opens a serial connection to a given device path and reads data into it's appropriate
    file. If a disconnect occurs attempt to reconnect indefinitely.
    """
    logger.warning("Connection successful! Reading data now...")
    ser = serial.Serial(device_path, BAUD_RATE, timeout=1)
    while True:
        try:
            line = ser.readline().decode("utf8", "replace")
        except serial.serialutil.SerialException:
            connected = False
            attempts = 1

            while not connected:
                try:
                    paths = discover_arduinos()
                    if paths:
                        path = "/dev/" + paths.pop()
                    else:
                        path = ""

                    ser = serial.Serial(path, BAUD_RATE, timeout=1)
                    connected = True
                except serial.serialutil.SerialException:
                    logger.warning("Arduino is disconnected! retrying({})".format(attempts))
                    sleep(5)
                    attempts += 1
                else:

                    logger.warning("Successfully reconnected!")
        except KeyboardInterrupt:
            ser.close()
            exit(0)

        #log_line = '[' + str(datetime.datetime.utcnow()) + '] ' + line
        log_line = line

        debug_messages = open("messages.data", "a")
        sensor_readings = open("readings.data", "a")

        if re.search("\[MSG\]|\[ERROR\]|\[WARNING\]", line):
            debug_messages.write(log_line)
        elif re.search("\[DATA\]", line):
            sensor_readings.write(log_line)
        debug_messages.close()
        sensor_readings.close()


if __name__ == "__main__":
    arduino_paths = discover_arduinos()

    attempts = 1

    while not arduino_paths:
        logger.warning("Failed to discover any connected Arduinos retrying({})".format(attempts))
        sleep(5)
        arduino_paths = discover_arduinos()
        attempts += 1

    logger.warning("Discovered {} Arduinos...".format(len(arduino_paths)))
    logger.warning("Arduino device paths: {}".format(arduino_paths))

    read_data('/dev/' + arduino_paths[0])

