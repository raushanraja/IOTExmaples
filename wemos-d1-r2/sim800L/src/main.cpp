#include <Arduino.h>
#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(D3, D2); //SIM800L Tx & Rx is connected to Arduino #3 & #2

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {

    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  Serial.println("Initializing...");
  delay(10000);


  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600,SWSERIAL_8N1,D3,D2,false,256,0);

  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
}

void loop()
{
  updateSerial();
}
// AT+CSCA="+919897051914"
// AT+CSCS="GSM" 
// AT+CMGS="+919997439193"
// #include <Arduino.h>
// #include <SoftwareSerial.h>
// SoftwareSerial SIM900A(D3, D2); //SIM800L Tx & Rx is connected to Arduino #3 & #2

//  void SendMessage()
// {
//   Serial.println ("Sending Message");
//   SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
//   delay(1000);
//   Serial.println ("Set SMS Number");
//   SIM900A.println("AT+CMGS=\"+919973148043\"\r"); //Mobile phone number to send message
//   delay(1000);
//   Serial.println ("Set SMS Content");
//   SIM900A.println("G");// Messsage content
//   delay(100);
//   Serial.println ("Finish");
//   SIM900A.write(26);// ASCII code of CTRL+Z
//   delay(1000);
//   Serial.println ("Message has been sent");
// }
//  void RecieveMessage()
// {
//   Serial.println ("SIM900A Membaca SMS");
//   delay (1000);
//   SIM900A.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
//   delay(1000);
//   Serial.write ("Unread Message done");
//  }

// void setup()
// {
//   SIM900A.begin(9600);   // Setting the baud rate of GSM Module  
//   Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
//   Serial.println ("SIM900A Ready");
//   delay(100);
//   Serial.println ("Type s to send message or r to receive message");
// }
// void loop()
// {
//   if (Serial.available()>0)
//    switch(Serial.read())
//   {
//     case 's':
//       SendMessage();
//       break;
//     case 'r':
//       RecieveMessage();
//       break;
//   }
//  if (SIM900A.available()>0)
//    Serial.write(SIM900A.read());
// }
