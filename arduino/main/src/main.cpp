#include <SPI.h>
#include <Ethernet.h>
#include <Dns.h>

int id_devices = 1;



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

DNSClient dns_client; //initialize dns service
EthernetClient client;


/*temp var for test*/

float temp_zone_chaude = 38;
float current_temp_zone_chaude = 37;
float temp_zone_froide = 28;
float current_temp_zone_froide = 27;
int humidity = 50;
int current_humidity = 60;
/*END*/

const char *request_arg[] = {"current_temp_zone_chaude=", "current_temp_zone_froide=", "current_humidity=", "id_devices="};


void database() {
   // Make a HTTP request:

   if(client.connect(hostname, 80)) {
     Serial.println("connected to server");
   }

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


   request = "Host: ";
   request += hostname;


   client.println(request);
   Serial.println(request);

   client.println("Connection: close");
   Serial.println("Connection: close");

   client.println(); // end HTTP header

   while(client.connected()) {
      if(client.available()){
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }


   client.stop();

   /*
   client.println("GET / HTTP/1.1");

   Serial.print("/python/arduino?current_temp_zone_chaude=");
   client.print("mysql-redcommand.alwaysdata.net/python/arduino?current_temp_zone_chaude=");     //YOUR URL

   Serial.println(current_temp_zone_chaude);
   client.print(current_temp_zone_chaude);

   client.print("&current_temp_zone_froide=");
   Serial.println("&current_temp_zone_froide=");

   client.print(current_temp_zone_froide);
   Serial.println(current_temp_zone_froide);

   client.print("&current_humidity=");
   Serial.println("&current_humidity=");

   client.print(current_humidity);
   Serial.println(current_humidity);

   client.print("&id_devices=");
   Serial.println("&id_devices=");

   client.print(id_devices);
   Serial.println(id_devices);

   client.println("Connection: close");
   client.println();
   while(client.connected()) {
      if(client.available()){
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }
    */
}





void resolv_dns() {
  Serial.print("test");
  dns_client.begin(Ethernet.dnsServerIP());
  dns_client.getHostByName(hostname, database_ip);
  Serial.print("Ip of distant database : ");
  Serial.println(database_ip);
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
    resolv_dns();
    //client.begin();
  }
}






void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  ethernet_init();

}

void loop() {
  if (is_connected == true) {
    database();
  }
  delay(3600000);
}
