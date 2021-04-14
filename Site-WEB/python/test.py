import mysql.connector
from flask import *


conn = mysql.connector.connect(user='230393', password='Maxime1612',
                              host='mysql-redcommand.alwaysdata.net',database='redcommand_terrarium')

if conn:
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
select_employee = """SELECT * FROM users"""
cursor = conn.cursor()
cursor.execute(select_employee)
result = cursor.fetchall()
print(result)

for row in result:
    mydict.add(row[0],({"user":row[1],"devices":row[2],"devices":row[3]}))

stud_json = json.dumps(mydict, indent=2)

print(stud_json)
