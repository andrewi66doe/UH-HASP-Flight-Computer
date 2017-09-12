'''
    Complimentary filter on raw Accelerometer and Gyroscope Data
    Alpha = .02, Rate = 100 Hz, Delta Time = .01 seconds
    @author: Kevin Portillo
'''

import csv 
import pandas as pd
import math
import sys 

# with help from https://stackoverflow.com/questions/15124767/complementary-filter-code-not-working
# and https://github.com/jshoga/IMU-Processing/blob/master/complementaryFilter.m

filename = sys.argv[-1]
rate = 100.0
dt = 1.0/rate
alpha = .02

ENTRIES = 56000

# Open and retrieve data from CSV as df then for ease of use, lists
df = pd.read_csv(filename, low_memory=False)

timestamp = df.Time[:ENTRIES].tolist()

ax = df.ax[:ENTRIES].tolist()
ay = df.ay[:ENTRIES].tolist()
az = df.az[:ENTRIES].tolist()

gx = df.gz[:ENTRIES].tolist()
gy = df.gy[:ENTRIES].tolist()
gz = df.gz[:ENTRIES].tolist()

# Check 
# print(ax, ay, az)
# print(gx, gy, gz)
# print(timestamp)

# Empty lists angles in the x,y,z directions
theta_ax = []
theta_ay = []
theta_az = []
thetaX = []
thetaY = []
thetaZ = []

# Calculate low pass filter angles from Accelerometer data
for index in range(ENTRIES):
    theta_ax.append(alpha*math.atan2(float(az[index]),float(az[index])))
    theta_ay.append(alpha*math.atan2(float(ay[index]),float(az[index])))
    theta_az.append(alpha*math.atan2(float(ay[index]),float(ax[index])))

# Base case for 1st data entry using 0 as initial state
thetaX.append((1-alpha)*float(theta_ax[0]))
thetaY.append((1-alpha)*float(theta_ay[0]))
thetaZ.append((1-alpha)*float(theta_az[0]))

# apply high pass filter to the already processed low pass filtered data 
for index in range(1,ENTRIES-1):
    thetaX.append((1-alpha)*(float(thetaX[index-1]) + float(gx[index])*dt) + float(theta_ax[index]))
    thetaY.append((1-alpha)*(float(thetaY[index-1]) + float(gy[index]*dt)) + float(theta_ay[index]))
    thetaZ.append((1-alpha)*(float(thetaZ[index-1]) + float(gz[index]*dt)) + float(theta_az[index]))

# write to csv by columns so that we can plot in R
columns = [ax,ay,az,gx,gy,gz,thetaX,thetaY,thetaZ]

with open('BNO_RAW_AND_FILTERED.csv', 'w') as out:
    dw = csv.DictWriter(out, fieldnames = ["raw_ax", "raw_ay", "raw_az", "raw_gx", "raw_gy", "raw_gz", "thetaX", "thetaY", "thetaZ"])
    dw.writeheader()
    wr = csv.writer(out)
    for elem in zip(*columns):
        out.write("{0},{1},{2},{3},{4},{5},{6},{7},{8}\n".format(*elem))
