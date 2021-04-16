import mysql.connector
from flask import *


mydb = mysql.connector.connect(user='230393', password='Maxime1612',
                              host='mysql-redcommand.alwaysdata.net',database='redcommand_terrarium')

if mydb:
    print ("Connected Successfully")
else:
    print ("Connection Not Established")





class create_dict(dict):

    # __init__ function
    def __init__(self):
        self = dict()

    # Function to add key:value
    def add(self, key, value):
        self[key] = value

mydict = create_dict()
mycursor = mydb.cursor()
mycursor.execute("SELECT * FROM devices")
result = mycursor.fetchall()

print(result)

for row in result:
    mydict.add(row[0],({"status":row[1],"temp_zone_chaude":row[2],"current_temp_zone_chaude":row[3], "temp_zone_froide":row[4], "current_temp_zone_froide":row[5], "angle_trappe":row[6], "humidity":row[7], "current_humidity":row[8]}))

myjson = json.dumps(mydict, indent=2)

print(myjson)
