import mysql.connector


id_devices = 4

mydb = mysql.connector.connect(
    host="mysql-redcommand.alwaysdata.net",
    user="230393",
    password="Maxime1612",
    database="redcommand_terrarium"
)


mycursor = mydb.cursor()
mycursor.execute("SELECT `temp_zone_chaude`, `temp_zone_froide`, `angle_trappe`, `humidity` FROM `devices` WHERE id={}".format(id_devices))

var = ""

for x in mycursor:
    var = var + "{}".format(x)

print(var)
