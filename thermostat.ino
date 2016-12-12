#include "SparkFunRHT03/SparkFunRHT03.h"

const int RHT03_data = 6;

RHT03 rht;

int heat = D5;
int cool = D6;
int fan  = D7;
bool needCooling;
bool needHeating;

TCPClient client;

void setup(void) {
    Serial.begin(9600);
    rht.begin(RHT03_data);
    pinMode(heat, OUTPUT);
    pinMode(cool, OUTPUT);
    pinMode(fan, OUTPUT);
}

void loop(void) {
    int ret = rht.update();
    if(ret == 1){
        float tmp = rht.tempF();
        float hum = rht.humidity();
        Serial.println(String(tmp,1) + "F\t" + String(hum,1) + "%");
    }
    else{
        delay(RHT_READ_INTERVAL_MS);
    }
  delay(1000); //just here to slow down the output so it is easier to read
  digitalWrite(fan, HIGH);
  delay(1000);
  digitalWrite(fan,LOW);
  //System.sleep(10); Lowers power consuption from 75mA to 30mA, but also turns off wifi. Not low enough for long-term battery power, either.
  //May want to set something up that leaves wifi off most of the time, since right now this thing is running on a huge battery pack.
}

//void receiveWeather // May implement later.
