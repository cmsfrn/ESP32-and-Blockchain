
//Libraries
#include <Web3.h>
#include <Contract.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

// Connection specification
#define USE_SERIAL Serial
#define ENV_SSID     "..."
#define ENV_WIFI_KEY "..."
#define MY_ADDRESS "..."

string* CONTRACT_ADDRESS = new string("...");
string* INFURA_HOST = new string("rinkeby.infura.io");
string* INFURA_PATH = new string("...");
string result;
Web3 web3((string*)INFURA_HOST, (string*)INFURA_PATH);
Servo myservo;
StaticJsonDocument<400> doc;
float num = 2 ; //data coming from network
int pos = 0;
void setup() {
    USE_SERIAL.begin(115200);
    myservo.attach(26);
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFi.begin(ENV_SSID, ENV_WIFI_KEY);

    // attempt to connect to Wifi network:
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        // wait 1 second for re-trying
        delay(1000);
    }

    USE_SERIAL.println("Connected");

    
}

void loop() {

    eth_call();
    
    if (num ==1 & pos ==0){
      for (pos; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);  
        Serial.println(pos);
      }
      return;
    }else if (num == 2 & pos==180){
      myservo.deattach();
      return;      
    }
}

void eth_call() {
    Contract contract(&web3, CONTRACT_ADDRESS);
    strcpy(contract.options.from, MY_ADDRESS);
    strcpy(contract.options.gasPrice,"2000000000000");
    contract.options.gas = 5000000;
    string function = "Check()";
    string param = contract.SetupContractData(&function);
    result = contract.Call(&param);
    

    DeserializationError error = deserializeJson(doc, result);

    // Test if parsing succeeds.
    if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
    }
    String longitude = doc["result"];
    num = longitude.toFloat();
    Serial.println(longitude.c_str());
    Serial.println(num);
}
