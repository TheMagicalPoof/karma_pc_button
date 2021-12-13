#include <AsyncTCP.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <HTTPClient.h>

#define KEY_PIN 15

unsigned long StartTime;

void TurnOn(){
	digitalWrite(KEY_PIN, HIGH);
	delay(500);
	digitalWrite(KEY_PIN, LOW);
}

class Web {
	public:
		Web(int port = 80) : _server(port) {
			_Initialize();
		}

	private:
		AsyncWebServer _server;
		void _Initialize(){
			_server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request){ _HandlerRooT(request); });
			_server.begin();
		}

		void _HandlerRooT(AsyncWebServerRequest* request){
			HTTPClient http;
			http.begin("https://api.ipify.org/");
			int httpCode = http.GET();
			if (httpCode > 0) {
		    	String payload = http.getString();
		    	if(request->hasParam("on")){
		    		request->send(200, "text/html", "PC is on, RDP IP is: " + payload);
		    		TurnOn();
		    	} else {
		    		request->send(200, "text/html", "Karma, use \"on\" method. RDP IP is: " + payload);
		    	}
			} else {
				request->send(200, "text/html", "Bad request to ip service.");
			}
			http.end();
		}
};

void setup(){
	Serial.begin(115200);
	WiFi.begin("SIM - Starlink Network", "abc2375906");
    StartTime = millis();
    new Web(80);
	
	pinMode(KEY_PIN, OUTPUT);
	digitalWrite(KEY_PIN, LOW);
	
}

void loop(){
	if(millis() - StartTime > 5000){
		StartTime = millis();
		if(WiFi.status() != WL_CONNECTED){
			Serial.println("Wifi is off");
		} else {
			Serial.println("Wifi is on");
			Serial.println(WiFi.localIP());
		} 
	}


}