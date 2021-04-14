#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

int id_devices = 1;



Servo ServoTrappe;                                                              // creating servo

int loop_time = 0;

/*network var*/
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0D, 0x14, 0x84
};
  /*default var if dhcp fail*/
IPAddress ip(192, 168, 1, 177);
IPAddress myDns(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
  /*END*/

  /*database var*/
char hostname[] = "redcommand.alwaysdata.net";                                  // server's hostname/URL
char python_server_path[] = "/python/arduino";
char user[] = "230393";                                                         // MySQL user login username
char password[] = "Maxime1612";                                                 // MySQL user login password
int database_port = 3306;
IPAddress database_ip;                                                          // IP of MySQL server
  /*END*/

bool is_connected;                                                              // var for offline mod
/*END*/

EthernetClient client;


/*default var*/
float temp_zone_chaude = 38;
float temp_zone_froide = 28;
int angle_trappe = 91;                                                          // set it to 91 beacause 91 is the auto mode, in fact, the "trappe" fonction only support int in range 0, 90
int humidity = 50;
/*END*/

/*initialize variable of sensor*/
  /*temp var for test*/
float current_temp_zone_chaude = 37;
float current_temp_zone_froide = 27;
int current_humidity = 60;
  /*END*/
/*END*/

// var to make easier to update HTTP request
const char *request_arg[] = {"current_temp_zone_chaude=", "current_temp_zone_froide=", "current_humidity=", "id_devices="};


void database() {

  if(client.connect(hostname, 80)) {                                            // try to connect server
   Serial.println("connected to server");
  }

  /*make a HTTP request:*/
  String request = "GET ";                                                      // because https://www.youtube.com/watch?v=meu7W_xJbh8 ...
  request += python_server_path;
  request += "?";
  request += request_arg[0];
  request += current_temp_zone_chaude;
  request += "&";
  request += request_arg[1];
  request += current_temp_zone_froide;
  request += "&";
  request += request_arg[2];
  request += current_humidity;
  request += "&";
  request += request_arg[3];
  request += id_devices;
  request += " HTTP/1.1";

  client.println(request);                                                      // send request to server
  Serial.println(request);

    /*send a HTTP request to server:*/
  request = "Host: ";
  request += hostname;

  client.println(request);                                                      // send request to server
  Serial.println(request);

  client.println("Connection: close");                                          // end HTTP request
  Serial.println("Connection: close");
    /*END*/

  client.println();                                                             // end HTTP header
  /*END*/

  /*receive data from server*/
  String message = "";
  bool header = true;
  while(client.connected()) {
    if(client.available()){
      /*read an incoming byte from server and print it to serial monitor:*/
      char c = client.read();

      if (c == '(') {                                                           // check for the end of the header
        header = false;
        Serial.println();
        Serial.println("header ended");
      }

      Serial.print(c);
      /*END*/

      if (!header) {                                                            // when header ended, put data into message variable
        message += c;
      }
    }
  }
  Serial.println();
  Serial.println();
  Serial.print("message = ");
  Serial.println(message);
  Serial.println();
  /*END*/

  client.stop();                                                                // end connection to server
  Serial.println("connection ended succesfuly");

  /*removing () to message*/
  int message_lenght = message.length();
  message.remove(0, 1);
  message.remove((message_lenght - 2), 1);
  Serial.println(message);
  /*END*/


  /*analyzing message to put data in var*/
    /*temp_zone_chaude*/
  String temp_message = message;
  int temp_message_lenght = temp_message.length();
  temp_message.remove(int(temp_message.indexOf(", ")), (temp_message_lenght - int(temp_message.indexOf(", "))));
  message.remove(0, (int(message.indexOf(", ")) + 2));
  temp_zone_chaude = temp_message.toFloat();
  Serial.print("temp_zone_chaude = ");
  Serial.println(temp_zone_chaude);
    /*END*/
    /*temp_zone_froide*/
  temp_message = message;
  temp_message_lenght = temp_message.length();
  temp_message.remove(int(temp_message.indexOf(", ")), (temp_message_lenght - int(temp_message.indexOf(", "))));
  message.remove(0, (int(message.indexOf(", ")) + 2));
  temp_zone_froide = temp_message.toFloat();
  Serial.print("temp_zone_froide = ");
  Serial.println(temp_zone_froide);
    /*END*/
    /*angle_trappe*/
  temp_message = message;
  temp_message_lenght = temp_message.length();
  temp_message.remove(int(temp_message.indexOf(", ")), (temp_message_lenght - int(temp_message.indexOf(", "))));
  message.remove(0, (int(message.indexOf(", ")) + 2));
  angle_trappe = temp_message.toInt();
  Serial.print("angle_trappe = ");
  Serial.println(angle_trappe);
    /*END*/
    /*humidity*/
  temp_message = message;
  temp_message_lenght = temp_message.length();
  temp_message.remove(int(temp_message.indexOf(", ")), (temp_message_lenght - int(temp_message.indexOf(", "))));
  message.remove(0, (int(message.indexOf(", ")) + 2));
  humidity = temp_message.toInt();
  Serial.print("humidity = ");
  Serial.println(humidity);
    /*END*/
  /*END*/

}


void ethernet_init() {
  /*start the Ethernet connection:*/
  Serial.println("Trying to get an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
      is_connected = false;
    }
    else {
      is_connected = true;
    }
    // initialize the Ethernet device not using DHCP:
    Ethernet.begin(mac, ip, myDns, gateway, subnet);
  }
  else {
    Serial.println("succesfuly connect using DHCP");
    is_connected = true;
  }

  Serial.print("is connected : ");
  Serial.println(is_connected);
  if (is_connected == true) {
    // print your local IP address:
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
  }
  /*END*/
}

void tapis_chauffant(boolean status) {
  // création d'une fonction basique permettant d'activer ou de désactiver le tapis chauffant.
  // exemple d'utilisation de la fonction : "heating_mat(true)" or "heating_mat(false)"
  digitalWrite(7, status);
}

void trappe(int angle = 15) {
  // création d'une fonction permettant de gérer la trappe (possibilitée de définir l'angle d'ouverture, sinon l'angle par défault sera 15°)
  // exemple d'utilisation de la fonction : "trappe()" pour ouvrir la trappe à l'angle par default. Ou bien : "trappe(35)" pour ouvrir la trappe à 35°.
  // ATTENTION : la variable "angle" doit être comprise entre 0 et 90
  ServoTrappe.write(angle);
}

float get_temp_zone_froide() {                                                     // does not work because it is not a analog sensor
  // création d'une fonction permettant de lire la température de la zone froide
  // exemple d'utilisation de la fonction : "float maTemperature = get_temp_zone_froide()"
  int temp = analogRead(A1);                                                    // lit la valeur analogique sur la broche A1 (le capteur de température de la zone froide)
  temp = map(temp, 0, 1023, 0, 5000);                                           // convertit la valeur analogique en mV
  float quantum = 0.01;                                                         // exemple pour un pas de 0.01°C/mV
  float temperature = temp * quantum;                                           // convertit la variable "temp" en °C (grâce au pas)
  return (temperature);                                                         // renvoie la température
}


void set_temp_zone_froide() {
  // création d'une fonction permettant de réguler la température de la zone froide
  // exemple d'utilisation de la fonction : "set_temp_zone_froide()"
  if (current_temp_zone_froide == temp_zone_chaude) {                           // vérifie si la température est bonne
    tapis_chauffant(false);                                                     // éteint le tapis chauffant
    if (angle_trappe == 91) {                                                   // regarde si la trappe est en mode auto
      trappe(0);                                                                // si la trappe est en mode auto, ferme la trappe
    }
  }

  else if (current_temp_zone_froide > temp_zone_froide) {                       // si la température est chaude
    tapis_chauffant(false);                                                     // éteint le tapis chauffant
    if (angle_trappe == 91) {                                                   // regarde si la trappe est en mode auto
      trappe();                                                                 // si la trappe est en mode auto, ouvre la trappe
    }
    if (current_temp_zone_froide > (temp_zone_froide + 5)) {                    // si la température est trop chaude
      trappe(90);                                                               // force l'ouverture de la trappe en grand
    }
  }

  else if (current_temp_zone_froide < temp_zone_froide) {                       // si la température est froide
    tapis_chauffant(true);                                                      // alume le tapis chauffant
    if (angle_trappe == 91) {                                                   // regarde si la trappe est en mode auto
      trappe(0);                                                                // si la trappe est en mode auto, ferme la trappe
    }
    if (current_temp_zone_froide < (temp_zone_froide - 5)) {                    // si la température est trop chaude
      trappe(0);                                                                // force la fermeture de la trappe
    }
  }
}


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  ServoTrappe.attach(3);                                                        // initialisation de la broche 3 pour le servo moteur (pour la trappe)
  pinMode(7, OUTPUT);                                                           // initialisation de la broche 7 pour le tapis chauffant

  ethernet_init();

}

void loop() {

  if (is_connected == true) {                                                   // check if offline mode is disabled
    database();
  }

  for (int i = 0; i < 60; i++) {

    set_temp_zone_froide();

    delay(10000);                                                               // wait 10 second
  }

  if (is_connected == false) {                                                  // if offline mode is enabled, trying to reconnect
    ethernet_init();


  }
}
