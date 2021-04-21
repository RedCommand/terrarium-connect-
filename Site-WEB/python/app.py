from flask import *
import mysql.connector
from flask_restx import Resource, Api
import base64

app = Flask(__name__)
api = Api(app)

def connect_database(id_devices=None, devices_type=None, devices_exist=True):
    mydb = mysql.connector.connect(
        host="localhost",
        user="mini-pc",
        password="MVidal  1612",
        database="redcommand_terrarium"
    )

    mycursor = mydb.cursor()
    if id_devices != None :
        if not devices_exist :
            mycursor.execute("INSERT INTO devices (id, type, temp_zone_chaude, current_temp_zone_chaude, temp_zone_froide, current_temp_zone_froide, angle_trappe, humidity, current_humidity) SELECT {}, '{}', 34, 34, 24, 24, 42, 42, 42 WHERE NOT EXISTS (SELECT * FROM devices WHERE id={});".format(id_devices, devices_type, id_devices))
            mydb.commit()
            print("created new devices")
    return mydb, mycursor




@api.route('/client/<client_token>/<fonction>')
class client(Resource):
    def get(self, client_token, fonction):
        result = None


        def user_exist():
            mycursor.execute("SELECT user FROM `users` WHERE user='{}' AND password='{}'".format(login, password))
            if "{}".format(mycursor.fetchall()) != "[]":
                return True
            else:
                return False

        def list_user_devices():
            if user_exist():
                user_devices_list = []
                mycursor.execute("SELECT `devices_list` FROM users WHERE user='{}'".format(login))
                output= mycursor.fetchall()
                output = output[0]
                output = output[0]
                output = json.loads(output)
                for i in output.get("id"):
                    user_devices_list.append(i)
                print("user_devices_list = ", user_devices_list)
                return user_devices_list
            else:
                return "user or password incorrect"


        def register():
            if not user_exist():
                mycursor.execute("INSERT INTO `users` (`id`, `user`, `password`, `devices_list`) VALUES (NULL, '{}', '{}', NULL);".format(login, password))
                mydb.commit()
            else:
                return "user already exist"

        def list_devices():
            if user_exist():
                mycursor.execute("SELECT `devices_list` FROM users WHERE user='{}'".format(login))
                output= mycursor.fetchall()
                output = output[0]
                output = output[0]
                output = json.loads(output)
                result = []
                for i in output.get("id"):
                    device_id = i
                    mycursor.execute("SELECT `type` FROM `devices` WHERE `id`={}".format(device_id))
                    device_type = mycursor.fetchone()
                    device_type = device_type[0]
                    print(device_type)
                    result.append((device_id, device_type))
                return result
            else:
                return "user or password incorrect"

        def get_data():
            if user_exist():
                id_device = request.args.get("id_device")
                id_device = int(id_device)
                user_devices = list_user_devices()
                print("id_device = ", id_device)
                if id_device in user_devices:
                    mycursor.execute("SELECT * FROM `devices` WHERE id={}".format(id_device))
                    output = mycursor.fetchone()
                    mydict = {"id":output[0],"type":output[1],"temp_zone_chaude":output[2],"current_temp_zone_chaude":output[3], "temp_zone_froide":output[4], "current_temp_zone_froide":output[5], "angle_trappe":output[6], "humidity":output[7], "current_humidity":output[8]}
                    return mydict
                else:
                    return "wrong device"
            else:
                return "user or password incorrect"



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
        elif fonction == "get_data":
            result = get_data()
        else:
            result =  "bad request"


        mycursor.close()
        mydb.close()

        print(result)
        return result



    def post(self, client_token, fonction):

        def user_exist():
            mycursor.execute("SELECT user FROM `users` WHERE user='{}' AND password='{}'".format(login, password))
            if "{}".format(mycursor.fetchall()) != "[]":
                return True
            else:
                return False

        def list_user_devices():
            if user_exist():
                user_devices_list = []
                mycursor.execute("SELECT `devices_list` FROM users WHERE user='{}'".format(login))
                output= mycursor.fetchall()
                output = output[0]
                output = output[0]
                output = json.loads(output)
                for i in output.get("id"):
                    user_devices_list.append(i)
                print("user_devices_list = ", user_devices_list)
                return user_devices_list
            else:
                return "user or password incorrect"


        def update_data():
            if user_exist():
                nb_request = 0
                sql_request = "UPDATE devices SET"
                try:
                    temp_zone_chaude = request.form['temp_zone_chaude']
                    if nb_request > 0:
                        sql_request += ","
                    sql_request += " temp_zone_chaude="
                    sql_request += temp_zone_chaude
                    nb_request += 1
                except:
                    pass
                try:
                    temp_zone_froide = request.form['temp_zone_froide']
                    if nb_request > 0:
                        sql_request += ","
                    sql_request += " temp_zone_froide="
                    sql_request += temp_zone_froide
                    nb_request += 1
                except:
                    pass
                try:
                    angle_trappe = request.form['angle_trappe']
                    if nb_request > 0:
                        sql_request += ","
                    sql_request += " angle_trappe="
                    sql_request += angle_trappe
                    nb_request += 1
                except:
                    pass
                try:
                    humidity = request.form['humidity']
                    if nb_request > 0:
                        sql_request += ","
                    sql_request += " humidity="
                    sql_request += humidity
                    nb_request += 1
                except:
                    pass
                try:
                    id_device = request.form['id_device']
                    id_device = int(id_device)
                    sql_request += " WHERE id={}".format(id_device)
                except:
                    nb_request = 0

                if nb_request > 0:
                    user_devices = list_user_devices()
                    print("id_device = ", id_device)
                    if id_device in user_devices:
                        print("nb arg = ", nb_request)
                        print("sql request = ", sql_request)
                        mycursor.execute(sql_request)
                        mydb.commit()
                    else:
                        return "wrong device"
                else:
                    return "missing arg"
            else:
                return "user or password incorrect"



        mydb, mycursor = connect_database()

        try:
            login, password = base64.b64decode(client_token).decode("UTF-8").split('::')
        except:
            result = "invalid token"
        print("login = ", login, "\npassword = ", password)


        if fonction == "update_data":
            result = update_data()
        else:
            result =  "bad request"


        mycursor.close()
        mydb.close()

        print("result")
        return result




@app.route('/arduino')
def arduino():
    current_temp_zone_chaude = request.args.get("current_temp_zone_chaude")
    current_temp_zone_froide = request.args.get("current_temp_zone_froide")
    current_humidity = request.args.get("current_humidity")
    token_devices = request.args.get("token_devices")


    id_devices, devices_type = base64.b64decode(token_devices).decode("UTF-8").split('::')


    mydb, mycursor = connect_database(id_devices, devices_type, False)


    mycursor.execute("UPDATE devices SET current_temp_zone_chaude='{}', current_temp_zone_froide='{}', current_humidity='{}' WHERE id={};".format(current_temp_zone_chaude, current_temp_zone_froide, current_humidity, id_devices))
    mydb.commit()


    mycursor.execute("SELECT `temp_zone_chaude`, `temp_zone_froide`, `angle_trappe`, `humidity` FROM `devices` WHERE id={}".format(id_devices))

    var = ""
    for x in mycursor:
        var = var + "{}".format(x)

    mycursor.close()
    mydb.close()

    return var

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=4000, debug=True)
