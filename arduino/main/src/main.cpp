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
char hostname[] = "mysql-redcommand.alwaysdata.net";                            // change to your server's hostname/URL
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


void database() {
  if (client.connect(hostname, 80)) {
     Serial.println("connected");
     // Make a HTTP request:
     Serial.print("GET /python/arduino?current_temp_zone_chaude=");
     client.print("GET /python/arduino?current_temp_zone_chaude=");     //YOUR URL
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
     client.print(" ");      //SPACE BEFORE HTTP/1.1
     client.print("HTTP/1.1");
     client.println();
     client.println("Host: <Your Local IP>");
     client.println("Connection: close");
     client.println();
   } else {
     // if you didn't get a connection to the server:
     Serial.println("connection failed");
   }

}





void resolv_dns() {
  dns_client.begin(Ethernet.dnsServerIP());
  dns_client.getHostByName(hostname, database_ip);
  Serial.print("Ip of distant database : ");
  Serial.println(database_ip);
}

void connect_database(){

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
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  if (is_connected == true) {
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
