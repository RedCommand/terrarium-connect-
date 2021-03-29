from flask import *
import mysql.connector



def connect_database(id_devices, devices_exist=True):
    mydb = mysql.connector.connect(
        host="mysql-redcommand.alwaysdata.net",
        user="230393",
        password="Maxime1612",
        database="redcommand_terrarium"
    )

    mycursor = mydb.cursor()

    if not devices_exist :
        mycursor.execute("INSERT INTO devices (id, status, temp_zone_chaude, current_temp_zone_chaude, temp_zone_froide, current_temp_zone_froide, angle_trappe, humidity, current_humidity) SELECT {}, 1, 34, 34, 24, 24, 42, 42, 42 WHERE NOT EXISTS (SELECT * FROM devices WHERE id={});".format(id_devices, id_devices))
        mydb.commit()
        print("created new devices")


    return mydb, mycursor




app = Flask(__name__)

@app.route('/')
def hello_world():
    return render_template('hello.html')

@app.route('/arduino')
def arduino():
    current_temp_zone_chaude = request.args.get("current_temp_zone_chaude")
    current_temp_zone_froide = request.args.get("current_temp_zone_froide")
    current_humidity = request.args.get("current_humidity")
    id_devices = request.args.get("id_devices")

    mydb, mycursor = connect_database(id_devices, False)
    print("test")

    mycursor.execute("SELECT `temp_zone_chaude`, `temp_zone_froide`, `angle_trappe`, `humidity` FROM `devices` WHERE id={}".format(id_devices))

    var = ""

    for x in mycursor:
        var = var + "{}".format(x)

    #var = current_temp_zone_chaude + current_temp_zone_froide + current_humidity + id_devices

    return var


if __name__ == "__main__":
    app.run(debug=True)
