# take in data file
# go through string and check for token
# append to appropriate csv file
# time stamp that is in the first column should be from the title
import datetime
import csv

# get rid of leading []

list0 = []
list1 = []
list2 = []
listH = [] 
listP = [] 


def sendto(splitlist):
    if len(splitlist) == 0:
        return
    if splitlist[3] == 'TEMP_0:':
        return list0.append(splitlist)
    if splitlist[3] == 'TEMP_1:':
        return list1.append(splitlist)
    if splitlist[3] == 'TEMP_2:':
        return list2.append(splitlist)
    if splitlist[3] == 'HUMIDITY:':
        return listH.append(splitlist)
    if splitlist[3] == 'PRESSURE_S:':
        return listP.append(splitlist)


with open('readings_new.data', 'r') as input_stream:
    for i in input_stream:
        for x in i:
            i = i.replace('[', "")
            i = i.replace(']', "")
        splitlist = i.split()
        # print(splitlist)
        sendto(splitlist)

with open('TEMP_0.csv', 'a') as output_stream0:
    writer0 = csv.writer(output_stream0, dialect=csv.excel, quoting=csv.QUOTE_NONNUMERIC)  # delimiter=',', quoting=csv.QUOTE_NONNUMERIC, dialect= csv.excel
    writer0.writerow(["Date", "Time", "---", "Sensor Name", "Data"])
    writer0.writerows(list0)

with open('TEMP_1.csv', 'a') as output_stream1:
    writer1 = csv.writer(output_stream1)
    writer1.writerow(["Date", "Time", "---", "Sensor Name", "Data"])
    writer1.writerows(list1)

with open('TEMP_2.csv', 'a') as output_stream2:
    writer2 = csv.writer(output_stream2)
    writer2.writerow(["Date", "Time", "---", "Sensor Name", "Data"])
    writer2.writerows(list2)
with open('HUMIDITY.csv', 'a') as output_stream_H:
    writer2 = csv.writer(output_stream_H)
    writer2.writerow(["Date", "Time", "---", "Sensor Name", "Data"])
    writer2.writerows(listH)
with open('PRESSURE_S.csv', 'a') as output_stream_P:
    writer2 = csv.writer(output_stream_P)
    writer2.writerow(["Date", "Time", "---", "Sensor Name", "Data"])
    writer2.writerows(listP)
    
output_stream0.close()
output_stream1.close()
output_stream2.close()
output_stream_H.close()
output_stream_P.close()
