from flask import *
import mysql.connector
from flask_restx import Resource, Api


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



@api.route('/client/<string:client_token>')
class client(Resource):
    def get(self, client_token):

        mydb, mycursor = connect_database()

        mycursor.execute("SELECT * FROM `devices` WHERE 1")
        result = mycursor.fetchall()

        mycursor.close()
        mydb.close()

        print(client_token)
        # Default to 200 OK
        return {'task': result}

    def post(self):
        pass

todos = {}


@api.route('/hello/<string:todo_id>')
class TodoSimple(Resource):
    def get(self, todo_id):
        return {todo_id: todos[todo_id]}

    def put(self, todo_id):
        todos[todo_id] = request.form['data']
        return {todo_id: todos[todo_id]}




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
    mycursor.execute("SELECT * FROM devices")
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
