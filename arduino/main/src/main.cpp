#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

int id_devices = 1;



Servo ServoTrappe;                                                              // création d'un servo



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
char hostname[] = "redcommand.alwaysdata.net";                                  // change to your server's hostname/URL
char python_server_path[] = "/python/arduino";
char user[] = "230393";                                                         // MySQL user login username
char password[] = "Maxime1612";                                                 // MySQL user login password
int database_port = 3306;
IPAddress database_ip;                                                          // IP of MySQL server
  /*END*/

bool is_connected;
/*END*/

EthernetClient client;


/*temp var for test*/

float temp_zone_chaude = 38;
float current_temp_zone_chaude = 37;
float temp_zone_froide = 28;
float current_temp_zone_froide = 27;
int angle_trappe = 45;
int humidity = 50;
int current_humidity = 60;
/*END*/

const char *request_arg[] = {"current_temp_zone_chaude=", "current_temp_zone_froide=", "current_humidity=", "id_devices="};


void database() {

   if(client.connect(hostname, 80)) {
     Serial.println("connected to server");
   }

   // Make a HTTP request:
   String request = "GET ";                                                     //because https://www.youtube.com/watch?v=meu7W_xJbh8 ...
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

   client.println(request);
   Serial.println(request);

   // Make a HTTP request:
   request = "Host: ";
   request += hostname;


   client.println(request);
   Serial.println(request);

   client.println("Connection: close");
   Serial.println("Connection: close");

   client.println(); // end HTTP header
   String message = "";
   bool header = true;

   while(client.connected()) {
      if(client.available()){
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();

        if (c == '(') {
          header = false;
          Serial.println();
          Serial.println("header ended");
        }

        Serial.print(c);

        if (!header) {
          message += c;
        }



      }
    }
    Serial.println();
    Serial.println();
    Serial.print("message = ");
    Serial.println(message);
    Serial.println();

    client.stop();
    Serial.println("connection ended succesfuly");


    int message_lenght = message.length();
    message.remove(0, 1);
    message.remove((message_lenght - 2), 1);
    Serial.println(message);


    String temp_message = message;
    int temp_message_lenght = temp_message.length();
    temp_message.remove(int(temp_message.indexOf(", ")), (temp_message_lenght - int(temp_message.indexOf(", "))));
    message.remove(0, (int(message.indexOf(", ")) + 2));
    temp_zone_chaude = temp_message.toFloat();
    Serial.print("temp_zone_chaude = ");
    Serial.println(temp_zone_chaude);

    temp_message = message;
    temp_message_lenght = temp_message.length();
    temp_message.remove(int(temp_message.indexOf(", ")), (temp_message_lenght - int(temp_message.indexOf(", "))));
    message.remove(0, (int(message.indexOf(", ")) + 2));
    temp_zone_froide = temp_message.toFloat();
    Serial.print("temp_zone_froide = ");
    Serial.println(temp_zone_froide);

    temp_message = message;
    temp_message_lenght = temp_message.length();
    temp_message.remove(int(temp_message.indexOf(", ")), (temp_message_lenght - int(temp_message.indexOf(", "))));
    message.remove(0, (int(message.indexOf(", ")) + 2));
    angle_trappe = temp_message.toInt();
    Serial.print("angle_trappe = ");
    Serial.println(angle_trappe);


    temp_message = message;
    temp_message_lenght = temp_message.length();
    temp_message.remove(int(temp_message.indexOf(", ")), (temp_message_lenght - int(temp_message.indexOf(", "))));
    message.remove(0, (int(message.indexOf(", ")) + 2));
    humidity = temp_message.toInt();
    Serial.print("humidity = ");
    Serial.println(humidity);

}


void ethernet_init() {
  // start the Ethernet connection:
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
}


void trappe(int angle = 90) {
  // création d'une fonction permettant de gérer la trappe (possibilitée de définir l'angle d'ouverture, sinon l'angle par défault sera 90°)
  // exemple d'utilisation de la fonction : "trappe()" pour ouvrir la trappe à l'angle par default. Ou bien : "trappe(35)" pour ouvrir la trappe à 35°.
  // ATTENTION : la variable "angle" doit être comprise entre 0 et 180

  ServoTrappe.write(angle);
}



void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  ServoTrappe.attach(3);                                                        // initialisation de la broche 3 pour le servo moteur (pour la trappe)


  ethernet_init();

}

void loop() {
  if (is_connected == true) {
    database();
  }
  delay(600000);
}
