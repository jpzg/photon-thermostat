// This #include statement was automatically added by the Particle IDE.
#include "SparkFunRHT03/SparkFunRHT03.h"

const int RHT03_data = D2;

RHT03 rht;

const int heat = D6;
const int cool = D1;
const int fan  = D5;
bool active = false;
bool needCooling; // Not used yet.
bool needHeating;

bool publish_data(String name, String data){ // Make fricking sure the events get published
    while(!Particle.publish(name,data)){
        delay(1000);
    }
    return true;
}
bool publish_status(String name){
    while(!Particle.publish(name)){
        delay(1000);
    }
}

void setup(void) {
    rht.begin(RHT03_data);
    pinMode(heat, OUTPUT);
    pinMode(cool, OUTPUT);
    pinMode(fan, OUTPUT);
    pinMode(D7, OUTPUT);
    while(!Particle.connected()){ delay(1000); }
}

void loop(void) {
    for(int ret = rht.update();ret != 1; ret = rht.update()){ delay(RHT_READ_INTERVAL_MS); } // Wait till it has the readings.
    
    float tmp = rht.tempF();
    float hum = rht.humidity();
    if(tmp < 68.0){
        digitalWrite(heat,HIGH);
        digitalWrite(fan,HIGH);
        digitalWrite(D7,HIGH);
        active = true;
        publish_status("heat-on");
    }
    else{
        publish_data("temperature",String(tmp));
        publish_data("humidity",String(hum));
    }
    while(tmp < 68.0){ // All this shit needs to be cleaned.
        int ret = rht.update();
        if(ret == 1){
            float tmp = rht.tempF();
            float hum = rht.humidity();
            publish_data("temperature",String(tmp));
            publish_data("humidity",String(hum));
        }
        else{
            delay(RHT_READ_INTERVAL_MS);
        }
        delay(10*1000); // Slow it down. 10s between readings.
    }
    if(active){
        digitalWrite(heat,LOW);
        digitalWrite(fan,LOW);
        digitalWrite(D7,LOW);
        active = false;
        publish_status("heat-off");
    }
    System.sleep(SLEEP_MODE_DEEP,10*60); // Deep sleep for 10min, then reset. Consumes a few uA in this mode. The publishes aren't working, so I'm disabling this for the moment.
    //delay(10*1000);
}

//void receiveWeather // May implement later.
