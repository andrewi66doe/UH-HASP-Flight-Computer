#!/usr/bin/python
import time
import datetime
import measure
import csv

from acquisition_example import do_hasp_acquisition, device


def wait_mp():
    log_temp()
    temp = measure.get_mp_temp(device)
    while(temp >= 70):
        print(temp)
        print("Minipix too hot: {}".format(temp))
        temp = measure.get_mp_temp(device)
	time.sleep(1)

def log_temp():
    
    ts = time.time()
    st = datetime.datetime.fromtimestamp(ts).strftime('%H:%M:%S.%f')[:-3]
    print("Pi: " + str(measure.get_rpi_temp()) + "  Mp:  " + str(measure.get_mp_temp(device)) + "  " + str(st))    
    with open("mp_temp.csv","a") as log:
        temp = measure.get_mp_temp(device)
        ts = time.time()
        st = datetime.datetime.fromtimestamp(ts).strftime('%H:%M:%S.%f')[:-3]
        log.write("Pi: " + str(measure.get_rpi_temp()) + "  Mp:  " + str("%.3f" % measure.get_mp_temp(device)) + "  " + str(st) + "\n")

if __name__ == "__main__":
	while True:

	        log_temp()
	
                temp = measure.get_mp_temp(device)
		if (measure.get_mp_temp(device) >= 70):
			print("Minipix too hot, shutting USB off...")
			#measure.power_usb_off()
			time.sleep(5)
			print ("Turning USB on...")
			#measure.power_usb_on()
			time.sleep(5)
                        # This is a hack to get temperature readings do not delete
                        device.doSimpleAcquisition(1, 0.1, pixet.PX_FTYPE_AUTODETECT, "")
			wait_mp()
			measure.get_mp_temp(device)
			#measure.restart_acq()
					
                print("Running acquisition now")
                do_hasp_acquisition()
		# print("Pi: " + str(measure.get_rpi_temp()) + "  Mp:  " + str(measure.get_mp_temp(device)) + "  " + str(st))
		time.sleep(1)

