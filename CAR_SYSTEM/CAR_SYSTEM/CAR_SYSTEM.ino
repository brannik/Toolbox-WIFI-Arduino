#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "ANONYMOUS"
#define APPSK  "11235813"
#endif

/* Set these to your desired credentials. */
const char* ssid = APSSID;
const char* password = APPSK;

ESP8266WebServer server(80);

boolean drlState = false;
boolean intState = false;
boolean ampState = false;
boolean dvrState = false;
boolean defDrlState, defIntState, defAmpState, defDvrState;
boolean lastDrl, lastInt, lastAmp, lastDvr;
boolean isBootup = true;

String drlDelay, intDelay, ampDelay, dvrDelay;

//const int drlPin = 4;
//const int intPin = 5;
//const int ampPin = 6;
//const int dvrPin = 7;

boolean convertValue(String val) {
    if (val.equals("on")) return true;
    if (val.equals("off")) return false;
}
void handleDefault() {
    server.send(200, "text/html", "<h1>If you see this message, it's likely that your computer is infected with a virus  :) :) :)</h1>");
}
void handleRoot() {
    server.send(200, "text/html", "<table><tr><td>DRL</td><td>ON</td></tr><tr><td>INT</td><td>ON</td></tr><tr><td>AMP</td><td>OFF</td></tr><tr><td>DVR</td><td>OFF</td></tr></table>");
    
    defDrlState = convertValue(server.arg("drl"));
    defIntState = convertValue(server.arg("int"));
    defAmpState = convertValue(server.arg("amp"));
    defDvrState = convertValue(server.arg("dvr"));

    drlDelay = server.arg("drlDelay");
    intDelay = server.arg("intDelay");
    ampDelay = server.arg("ampDelay");
    dvrDelay = server.arg("dvrDelay");
}

void handleSwitches() {
    if (server.arg("element") == "drl") {
        if (server.arg("val") == "on") {
            Serial.println("DRL switch to OFF");
            drlState = false;
        }
        else if (server.arg("val") == "off") {
            Serial.println("DRL switch to ON");
            drlState = true;
        }

        server.send(200, "text/html", "true");
    }
    if (server.arg("element") == "int") {
        if (server.arg("val") == "on") {
            Serial.println("INTERIOR switch to OFF");
            intState = false;
        }
        else if (server.arg("val") == "off") {
            Serial.println("INTERIOR switch to ON");
            intState = true;
        }
        server.send(200, "text/html", "true");
    }
    if (server.arg("element") == "amp") {
        if (server.arg("val") == "on") {
            Serial.println("AMP switch to OFF");
            ampState = false;
        }
        else if (server.arg("val") == "off") {
            Serial.println("AMP switch to  ON");
            ampState = true;
        }
        server.send(200, "text/html", "true");
    }
    if (server.arg("element") == "dvr") {
        if (server.arg("val") == "on") {
            Serial.println("DVR switch to OFF");
            dvrState = false;
        }
        else if (server.arg("val") == "off") {
            Serial.println("DVR switch to ON");
            dvrState = true;
        }
        server.send(200, "text/html", "true");
    }
}
void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.on("/defaults", handleRoot);
    server.on("/switch", handleSwitches);
    server.onNotFound(handleDefault);
    server.begin();
    Serial.println("HTTP server started");

    //pinMode(drlPin, OUTPUT);
    //pinMode(intPin, OUTPUT);
    //pinMode(ampPin, OUTPUT);
    //pinMode(dvrPin, OUTPUT);

}

void loop() {
    server.handleClient();
    if (isBootup) {
        delayedStart();
        isBootup = false;
    }
    if (drlState != lastDrl) {
        // send to pins
        //sendPinSignals(drlPin, drlState);
        lastDrl = drlState;
    }
    if (intState != lastInt) {
        // send to pins
        //sendPinSignals(intPin, intState);
        lastInt = intState;
    }
    if (ampState != lastAmp) {
        // send to pins
        //sendPinSignals(ampPin, ampState);
        lastAmp = ampState;
    }
    if (dvrState != lastDvr) {
        // send to pins
        //sendPinSignals(dvrPin, dvrState);
        lastDvr = dvrState;
    }
    delay(100);
}

void sendPinSignals(int pNumber, boolean state) {
    if (state == true) {
        digitalWrite(pNumber, HIGH);
    }
    else {
        digitalWrite(pNumber, LOW);
    }
    Serial.println("Switched pin -> ");
    Serial.println(pNumber);
    Serial.println(" to state -> ");
    Serial.println(state);
}
void delayedStart() {
    // otlojen start na pusnatite razdeli (izpulnqva se samo 1-q put sled davaneto na kontakt)
}