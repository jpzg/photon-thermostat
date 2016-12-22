// This #include statement was automatically added by the Particle IDE.
#include "SparkFunRHT03/SparkFunRHT03.h"

const int RHT03_data = D2;
const int heat = D6;
const int cool = D1;
const int fan  = D5;
const int relay = A0;

RHT03 rht;

float tmp;
float hum;

float day_limits[2] = {65.0,69.0}; // Lower limit beyond which the heat turns on and upper limit beyond which it turns off.
float night_limits[2] = {60.0,65.0};
float * limits; // Turns out this pointer stuff might be necessary, and I <3 pointers. 

bool active = false;

TCPClient client;
byte server[] = {192,168,1,11};

void update_rht(){
    while(!rht.update()){
        delay(RHT_READ_INTERVAL_MS);
    }
}

void setup(void) {
    rht.begin(RHT03_data);
    pinMode(heat, OUTPUT);
    pinMode(cool, OUTPUT);
    pinMode(fan, OUTPUT);
    pinMode(relay, OUTPUT);
    pinMode(D7, OUTPUT);
    digitalWrite(relay,HIGH); // Make sure the TRIACs are not already conducting
    delay(500);
    digitalWrite(relay,LOW);
    client.connect(server,8889);
    while(!Particle.connected()){ delay(1000); }
    client.println("Setup complete");
    RGB.control(true);
}

void loop(void) {
    if(Time.hour() >= 6 & Time.hour() < 23){ // Use day limits only between 6AM and 11PM
        limits = day_limits;
        RGB.color(255,128,0); // Orange during the day
    } else {
        limits = night_limits; // Arrays are just pointers to the first element, so this is valid.
        RGB.color(255,0,0); // Red at night
    }
    
    update_rht();
    tmp = rht.tempF();
    hum = rht.humidity();
    
    if(tmp < *limits){
        digitalWrite(heat,HIGH);
        digitalWrite(fan,HIGH);
        digitalWrite(D7,HIGH);
        active = true;
        Particle.publish("heat-on");
        client.print("Turning on heat");
    }
    else{
        Particle.publish("temperature",String(tmp));
        Particle.publish("humidity",String(hum));
    }
    // Stays in this loop while the heat is on
    while((tmp < *(limits+1)) & active){ // Adding one to the limits pointer before dereferencing is the same as limits[1] would be in Python; the next element is in the next spot in memory.
        delay(5*1000); // Slow it down. 5s between readings.
        rht.update();
        tmp = rht.tempF();
        hum = rht.humidity();
        Particle.publish("temperature",String(tmp));
        Particle.publish("humidity",String(hum));
        client.print("tmp: " + String(tmp));
    }
    if(active){
        digitalWrite(relay,HIGH); // Cuts power for everything so the TRIACs can turn off (not needed if wiring uses)
        digitalWrite(heat,LOW);
        digitalWrite(fan,LOW);
        digitalWrite(D7,LOW);
        delay(1000);
        digitalWrite(relay,LOW);
        active = false;
        Particle.publish("heat-off");
        client.print("Turning off heat");
    }
    delay(5*1000);
    client.print("tmp: " + String(tmp));
}
