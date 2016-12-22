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
bool needCooling; // Not used yet.
bool needHeating;

TCPClient client;
byte server[] = {192,168,1,11};
/*
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
*/ // A buildup of these calls might be causing the "doesn't turn on properly" behavior that I'm seeing right now.

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
    
<<<<<<< HEAD
    update_rht();
    tmp = rht.tempF();
    hum = rht.humidity();
    
    if(tmp < *limits){
=======
    float tmp = rht.tempF();
    float hum = rht.humidity();
    if(tmp < 66.0){
>>>>>>> 0a5fe20802719c24ebb2bb8c1fb3b86a55f631f6
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
<<<<<<< HEAD
    // Stays in this loop while the heat is on
    while((tmp < *(limits+1)) & active){ // Adding one to the limits pointer before dereferencing is the same as limits[1] would be in Python; the next element is in the next spot in memory.
        delay(5*1000); // Slow it down. 5s between readings.
        rht.update();
        tmp = rht.tempF();
        hum = rht.humidity();
        Particle.publish("temperature",String(tmp));
        Particle.publish("humidity",String(hum));
        client.print("tmp: " + String(tmp));
=======
    // Could probably move everything above this to setup(), and remove the while loop
    while(tmp < 66.0){ // All this shit needs to be cleaned. Also, it doesn't seem to stop at 68, so I'm lowering it to 62. May need to add more temperature sensors.
        int ret = rht.update();
        if(ret == 1){
            tmp = rht.tempF();
            hum = rht.humidity();
            publish_data("temperature",String(tmp));
            publish_data("humidity",String(hum));
        }
        else{
            delay(RHT_READ_INTERVAL_MS);
        }
        delay(10*1000); // Slow it down. 10s between readings.
>>>>>>> 0a5fe20802719c24ebb2bb8c1fb3b86a55f631f6
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
<<<<<<< HEAD
    delay(5*1000);
    /* Doesn't need to SLEEP_MODE_DEEP anymore; it's not on battery
    if(Time.hour() > 21){ // Basic scheduling
        System.sleep(SLEEP_MODE_DEEP,8*3600); // Go to bed for the night (10PM-6AM)
    }
    if(Time.hour() > 9){
        System.sleep(SLEEP_MODE_DEEP,5*3600); // Stay off during the day (9AM-2PM)
    }*/
    client.print("tmp: " + String(tmp));
=======
    delay(10*1000); // Sleep for ten seconds. Maybe this will help it publish like it's supposed to?
    System.sleep(SLEEP_MODE_DEEP,5*60); // Deep sleep for 5min, then reset. Consumes a few uA in this mode. The publishes aren't working, so I'm disabling this for the moment.
    //delay(10*1000);
>>>>>>> 0a5fe20802719c24ebb2bb8c1fb3b86a55f631f6
}
