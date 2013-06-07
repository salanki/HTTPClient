#include <WiFi.h>
#include <HTTPClient.h>

#define COM_SSID_NAME "KungarnaAvTylosand"
#define COM_PASSPHRASE "bigpetepassword"
#define SERVER "www.google.com"

//#define LINKDEBUG // Print what is sent/received on the link
// #define COM_WDT // A watchdog timer can be useful due to the WiFi stacks tenendcy to just hang

#define QUERY_PATH "/search?q="
#define MAX_TERM_SIZE 100

HTTPClient client(SERVER, 80);
struct http_result {
  int status;
  FILE *data;
};

void setup() {
  int status = WL_IDLE_STATUS;     // the Wifi radio's status
  
  Serial.begin(2400);
  Serial.println(F("Connecting to WiFi"));
  
  while((status = WiFi.begin(COM_SSID_NAME, COM_PASSPHRASE)) != WL_CONNECTED) {
    Serial.println(F("Wifi Association failed."));
  }
  
  printCurrentNet();
  printWifiData();
}

void loop() {
  if(!do_query("arduino")) Serial.println(F("HTTP GET failed"));
  delay(5000);
}

boolean do_query(char *tag) {
  char buffer[sizeof(QUERY_PATH)+MAX_TERM_SIZE+1];
  sprintf_P(buffer, PSTR(QUERY_PATH"%s"), tag);

  struct http_result result = httpGet(buffer);
  if(result.status == -1) return false;
  else if(result.status == 200) {
    char c;
    
    Serial.print(F("Response: "));
    while((c = fgetc(result.data)) != EOF) Serial.print(c);
    Serial.println();
    
    client.closeStream(result.data);
    return true;
  }
  else {
    Serial.print(F("Unknown server status: "));
    Serial.println(result.status);

    client.closeStream(result.data);
    return false;
  }
}

struct http_result httpGet(char *uri) {
  struct http_result result;
 
#ifdef LINKDEBUG
  client.debug(-1);
#endif
#ifdef COM_WDT
  wdt_enable(WDTO_8S); /* If a request for some reason does not return within 8 seconds we will reset, this is not optimal but the Arduino TCP library doesn't handle timeouts very well */
#endif
  result.data = client.getURI(uri);
  result.status = client.getLastReturnCode();
#ifdef COM_WDT
  wdt_disable();
#endif
  
  if (result.data == NULL) {
    Serial.println(F("HTTP failed to connect"));
    result.status = -1;
  }  
  return result;
}

/* Support methods */
void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];  
  WiFi.macAddress(mac);
  Serial.print(F("MAC address: "));
  Serial.print(mac[5],HEX);
  Serial.print(F(":"));
  Serial.print(mac[4],HEX);
  Serial.print(F(":"));
  Serial.print(mac[3],HEX);
  Serial.print(F(":"));
  Serial.print(mac[2],HEX);
  Serial.print(F(":"));
  Serial.print(mac[1],HEX);
  Serial.print(F(":"));
  Serial.println(mac[0],HEX);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);    
  Serial.print(F("BSSID: "));
  Serial.print(bssid[5],HEX);
  Serial.print(F(":"));
  Serial.print(bssid[4],HEX);
  Serial.print(F(":"));
  Serial.print(bssid[3],HEX);
  Serial.print(F(":"));
  Serial.print(bssid[2],HEX);
  Serial.print(F(":"));
  Serial.print(bssid[1],HEX);
  Serial.print(F(":"));
  Serial.println(bssid[0],HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print(F("signal strength (RSSI):"));
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print(F("Encryption Type:"));
  Serial.println(encryption,HEX);
  Serial.println();
} 



