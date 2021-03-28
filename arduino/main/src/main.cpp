#include <SPI.h>
#include <Ethernet.h>
#include <Dns.h>

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

EthernetClient client;
DNSClient dns_client; //initialize dns service




void database() {
  if (client.connect(server, 80)) {
     Serial.println("connected");
     // Make a HTTP request:
     Serial.print("GET /testcode/dht.php?humidity=");
     client.print("GET /testcode/dht.php?humidity=");     //YOUR URL
     Serial.println(humidityData);
     client.print(humidityData);
     client.print("&temperature=");
     Serial.println("&temperature=");
     client.print(temperatureData);
     Serial.println(temperatureData);
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
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

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
    Client.begin();
  }
}






void setup() {
  ethernet_init();

}

void loop() {

}
