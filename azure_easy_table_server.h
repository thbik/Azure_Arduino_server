

#include <WiFi101.h>
#include <SPI.h>
#include <String.h>



int i=0;
char ssid[] =""; // Wifi network name
char pass[] ="";//Wifi network password
char server[] = ""; // Azure server name
char table_name[] = ""; // Azure easy table name
String content;
char buffer[64];
char ver[]="2.0.0";
int val=0; // integer to be sent to the server
WiFiClient client;
String ret;

// You need to connect to internet first.

void setup() {
  //Initialize serial and wait for port to open:
	Serial.begin(9600);
	while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
	}
 
  // check for the presence of the shield:
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println("WiFi shield not present");
		while (true);
	}

  // Print WiFi MAC address:
	printMacAddress();

  // scan for existing networks:
	Serial.println("Scanning available networks...");
	listNetworks();
	WiFi.begin(ssid, pass);
	delay(3000);
	printWiFiStatus();
	Serial.println("trying to connect....");
  
  // Connect to Azure server	
	
	if (client.connect(server, 80)) {
		Serial.println("connected to server");
    }
    else{
		Serial.println("failed to connect");
    }
}


void loop() {

	Serial.println("trying to connect....");
 
	if (client.connect(server, 80)) {
		Serial.println("connected to server");
	}
    else{
		Serial.println("failed to connect");
    }
    val=0; // Define the value you want to add to the easy table
	postrequest(val);
	wait_response();
	read_response();
	end_request();
	delay(100);
	
}


void printMacAddress() {
  // the MAC address of your WiFi shield
  byte mac[6];

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

void listNetworks() {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
    Serial.flush();
  }
}
void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.println("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.println("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.println("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.println("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.println("Auto");
      break;
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

  
  
  void wait_response()
{
  while (!client.available()) {
    if (!client.connected()) {
      return;
    }
  }
}


void read_response()
{
	bool print = true;
	while (client.available()) {
		char c = client.read();
		ret=client.readString();
		char fg[ret.length()];
		ret.toCharArray(fg,ret.length());
		char *wor="";
		char *buf;
		buf = strstr(fg,wor);
		if( buf != NULL){
			Serial.println(buf);
		}   
	}
}


void end_request()
{
  client.stop();
}

void request(){
	client.print("GET ");  
	client.print("/tables/"); // add after/tables/{easy table name}?zumo-api-version=2.0.0");  
	client.println(" HTTP/1.1"); 
    client.print("Host: "); 
    client.println(server);  
    client.println("Connection: close");
    client.println();
    
  }
  
void postrequest(int value){
	content="{\"value\": ";
	String val="\"";
	char buffer[64];
	sprintf(buffer, "%d", value);
	String val2="\"";
	String hel="}";
	content=content+val+buffer+val2+hel;
	Serial.println(content);
	client.print("POST ");  //Do a GET
    client.print("/tables/"); // add after/tables/{easy table name}?zumo-api-version=2.0.0");  
    client.println(" HTTP/1.1"); 
    client.print("Host: "); 
    client.println(server);  //with hostname header
    client.print("Content-Type: ");
    client.println("application/json");
    client.print("Content-Length: ");
    client.println(content.length());
    client.println();
    client.println(content);
    client.println("Connection: close");
    client.println();
  }

