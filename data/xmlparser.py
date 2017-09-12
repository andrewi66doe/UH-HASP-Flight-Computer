'''
@author Kevin Portillo

command line: python3 xmlparser.py <filename> <type>
Returns list of data points for a given fieldname
'''
from xml.dom import minidom
import sys

filename = sys.argv[-2]
tag = sys.argv[-1]
xmldoc = minidom.parse(filename)

tkptList = xmldoc.getElementsByTagName(tag)

def getField(field):
    fieldList = []
    for item in tkptList:
       fieldList.append(item.attributes[field].value)
    return fieldList

print(getField('tim'))
print(getField('lat'))
print(getField('lon'))
print(getField('alt'))
print(getField('hdg'))
print(getField('sog'))
