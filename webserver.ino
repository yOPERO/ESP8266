#include <SoftwareSerial.h>
#define SSID "your wifi"      //your wifi ssid here
#define PASS "your pass"   //your wifi wep key here
SoftwareSerial dbgSerial(10, 11); // RX, TX
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.setTimeout(500);
  dbgSerial.begin(38400);  //can't be faster than 19200 for softserial
   
  
  //test if the module is ready
  sendAndWait( "AT","OK",10);
   
  //Change to mode 1 
  //sendAndWait("AT+CWMODE=1","no change","OK",100);
  Serial.println("AT+CWMODE=3");
  delay(100);
   
  // connect to router
  connectWiFi(SSID,PASS);
 
  //set the multiple connection mode
  sendAndWait("AT+CIPMUX=1","OK",100);
   
  //set the server of port 80 check "no change" or "OK"
  sendAndWait("AT+CIPSERVER=1,80","no change",100);
   
  //print the ip addr
  dbgSerial.println("ip address:");
  Serial.println("AT+CIFSR");
  delay(100);
  while ( Serial.available() ) {
    dbgSerial.write(Serial.read());
  }  
  dbgSerial.println();
   
  dbgSerial.println( "Start Webserver" );
}
 
void loop() {
  while (Serial.available() >0 )
  {
    char c = Serial.read();
    if (c == 71) {
      dbgSerial.println("Send Web Request");
      webserver();
      delay(500);
    }
  }
}
 
void http(String output) 
{
  Serial.print("AT+CIPSEND=0,");
  Serial.println(output.length());
  if (Serial.find(">"))
  {
    //Serial.println(output);
    sendAndWait(output,"SEND OK",10);
  }   
}
 
// Get the data from the WiFi module and send it to the debug serial port
boolean sendAndWait(String AT_Command, char *AT_Response, int wait){
  dbgSerial.print(AT_Command);
  Serial.println(AT_Command);
  delay(wait);
  while ( Serial.available() > 0 ) {
    if ( Serial.find(AT_Response)  ) {
        dbgSerial.print(" --> ");
        dbgSerial.println(AT_Response);
    return 1;
    }     
  }
  dbgSerial.println(" fail!");
  return 0;
}
 
void webserver(void) {
 char temp1[10];
 dtostrf(analogRead(A0),1,2,temp1);
 http("<title>yOPERO-TECH</title><H3>Hi, I am the ESP8266 Wifi Module</H3><p>Your Arduino's A0 pin value is = "+ String(temp1) +"</p>");
 //delay(1000);
 sendAndWait("AT+CIPCLOSE=0","",500);
}
boolean connectWiFi(String NetworkSSID,String NetworkPASS)
{
  String cmd = "AT+CWJAP=\"";
  cmd += NetworkSSID;
  cmd += "\",\"";
  cmd += NetworkPASS;
  cmd += "\"";
   
  //dbgSerial.println(cmd);  
  //sendAndWait(cmd,"OK",10);
  Serial.println(cmd);
  delay(100);
  while ( Serial.available() ) {
    dbgSerial.write(Serial.read());
  }  
}
