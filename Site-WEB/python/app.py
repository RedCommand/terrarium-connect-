from flask import *
import mysql.connector
from flask_restx import Resource, Api
import base64


def connect_database(id_devices=None, devices_exist=True):
    mydb = mysql.connector.connect(
        host="localhost",
        user="mini-pc",
        password="MVidal  1612",
        database="redcommand_terrarium"
    )

    mycursor = mydb.cursor()
    if id_devices != None :
        if not devices_exist :
            mycursor.execute("INSERT INTO devices (id, status, temp_zone_chaude, current_temp_zone_chaude, temp_zone_froide, current_temp_zone_froide, angle_trappe, humidity, current_humidity) SELECT {}, 1, 34, 34, 24, 24, 42, 42, 42 WHERE NOT EXISTS (SELECT * FROM devices WHERE id={});".format(id_devices, id_devices))
            mydb.commit()
            print("created new devices")


    return mydb, mycursor


class create_dict(dict):

    # __init__ function
    def __init__(self):
        self = dict()

    # Function to add key:value
    def add(self, key, value):
        self[key] = value








app = Flask(__name__)

api = Api(app)



@api.route('/client/<client_token>/<fonction>')
class client(Resource):



    def get(self, client_token, fonction):

        result = None

        def user_exist():
            mycursor.execute("SELECT user FROM `users` WHERE user='{}'".format(login))
            if "{}".format(mycursor.fetchall()) != "[]":
                return True
            else:
                return False


        def register():
            if not user_exist():
                mycursor.execute("INSERT INTO `users` (`id`, `user`, `password`, `devices_list`) VALUES (NULL, '{}', '{}', NULL);".format(login, password))
                mydb.commit()
            else:
                return "user already exist"

        def list_devices():
            if user_exist():
                mycursor.execute("SELECT devices_list FROM `users` WHERE user='{}' AND password='{}'".format(login, password))
                return mycursor.fetchall()
            else:
                return "user does not exist"


        mydb, mycursor = connect_database()

        try:
            login, password = base64.b64decode(client_token).decode("UTF-8").split('::')
        except:
            result = "invalid token"

        print("login = ", login, "\npassword = ", password)

        if fonction == "register":
            result = register()

        elif fonction == "list_devices":
            result = list_devices()

        else:
            result =  "bad request"







        mycursor.close()
        mydb.close()

        print(result)
        return result



    def post(self):
        pass




@app.route('/arduino')
def arduino():
    current_temp_zone_chaude = request.args.get("current_temp_zone_chaude")
    current_temp_zone_froide = request.args.get("current_temp_zone_froide")
    current_humidity = request.args.get("current_humidity")
    id_devices = request.args.get("id_devices")

    mydb, mycursor = connect_database(id_devices, False)

    mycursor.execute("UPDATE devices SET current_temp_zone_chaude='{}', current_temp_zone_froide='{}', current_humidity='{}' WHERE id={};".format(current_temp_zone_chaude, current_temp_zone_froide, current_humidity, id_devices))
    mydb.commit()


    mycursor.execute("SELECT `temp_zone_chaude`, `temp_zone_froide`, `angle_trappe`, `humidity` FROM `devices` WHERE id={}".format(id_devices))

    var = ""

    for x in mycursor:
        var = var + "{}".format(x)

    mycursor.close()
    mydb.close()
    return var


@app.route('/test')
def test():

    mydb, mycursor = connect_database()

    mydict = create_dict()
    mycursor.execute("SELECT uesr FROM devices")
    result = mycursor.fetchall()

    print(result)

    for row in result:
        mydict.add(row[0],({"status":row[1],"temp_zone_chaude":row[2],"current_temp_zone_chaude":row[3], "temp_zone_froide":row[4], "current_temp_zone_froide":row[5], "angle_trappe":row[6], "humidity":row[7], "current_humidity":row[8]}))

    myjson = json.dumps(mydict, indent=2, sort_keys=True)
    mycursor.close()
    mydb.close()
    return myjson


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=4000, debug=True)
