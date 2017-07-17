#!/usr/bin/env python

import subprocess
import os
import autodetect

def power_usb_off():
    os.system("echo '1-1.1' | sudo tee /sys/bus/usb/drivers/usb/unbind")

def power_usb_on():
    os.system("echo '1-1.1' | sudo tee /sys/bus/usb/drivers/usb/bind")

def get_rpi_temp():
    cmd = "/opt/vc/bin/vcgencmd measure_temp"
    process = subprocess.Popen(cmd, shell=True,
                           stdout=subprocess.PIPE, 
                           stderr=subprocess.PIPE,
                           )
    out, err = process.communicate()
    rpi_temp = float(out.split("=")[1].split("'")[0])

    return rpi_temp

def get_mp_temp(device):
    pars = device.parameters()
    par = pars.get("Temperature")
    temp = par.getDouble()
    
    return temp    

def restart_acq():
    os.system("python autodetect.py")

if __name__ == "__main__":
    #print(get_rpi_temp())
    #print(get_mp_temp())
    print(restart_acq())
