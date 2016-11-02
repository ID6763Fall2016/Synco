
#include "Pixels.h"
#include "CapTouch.h"

/*
 *  Simple HTTP get webclient test
 */
 
#include <tinyxml2.h>
using namespace tinyxml2;
using namespace std;

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssids[2]     = {"DiosBot", "HYRULE"};
const char* passwords[2] = {"1osorio!", "jsdo7302!"};

const char* host = "gtbuses.herokuapp.com";

#define NDX_RED 0
#define NDX_BLUE 1
#define NDX_GREEN 2
#define NDX_TROLLEY 3

// Preprocessed and optimized lines and stops data. Avoid loading big nextbus config service.
String LINES_NAMES[4] = {"red","blue","green","trolley"};
uint8_t STOPS_COLLECTION[25][4] = {{0, 0, 0, 1},{0, 0, 0, 1},{0, 0, 0, 2},{1, 1, 0, 2},{1, 1, 0, 0},{1, 1, 0, 0},{1, 1, 0, 0},{1, 1, 0, 0},{1, 1, 0, 0},{1, 1, 1, 1},{1, 1, 2, 2},{1, 1, 0, 2},{1, 1, 0, 2},{0, 0, 1, 0},{0, 0, 2, 0},{1, 1, 2, 2},{1, 1, 2, 2},{1, 1, 0, 0},{1, 1, 0, 0},{1, 1, 0, 0},{0, 0, 1, 0},{0, 0, 1, 0},{0, 0, 1, 0},{0, 0, 1, 0},{0, 0, 1, 0}};
String STOPS_NAMES[4][17] = {{"ferstcher","hubfers","centrstud","creccent","fitthall","mcmil8th","8thhemp","fershemp","fersatla","klaubldg","tech5th","tech4th","techbob","technorth","naveapts_a"},{"technorth","3rdtech","4thtech","5thtech","fersklau","fersatla","fitthall_a","mcmil8th","8thhemp","fershemp_ob","reccent","studcentr","fershub","cherfers","naveapts_a"},{"14thstat","gcat","glc","bakebldg","fershemp_ob","reccent_ob","studcent","tranhub","studcent_ib","creccent","fershemp","ndec","10thhemp","hempcurr","14thbusy_a"},{"centrstud","ferstdr","fershemrt","fersatl_ib","ferschmrt","tech5mrt","techsqua_ib","wpe7mrt","marta_a","techsqua","tech5rec","ferschrec","fersatla","fersherec","recctr","studcentr","tranhub_a"}};
uint8_t STOPS_NDX[4][25][2] = {{{},{},{},{10},{11},{12},{13},{14},{0},{1},{2},{9},{8},{},{},{7},{3},{4},{5},{6},{},{},{},{},{}},{{},{},{},{3},{2},{1},{0},{14},{13},{12},{11},{4},{5},{},{},{9},{10},{6},{7},{8},{},{},{},{},{}},{{},{},{},{},{},{},{},{},{},{7},{6, 8},{},{},{2},{3, 11},{4, 10},{5, 9},{},{},{},{12},{13},{14},{0},{1}},{{8},{9},{6, 9},{5, 10},{},{},{},{},{},{16},{0, 15},{4, 11},{3, 12},{},{},{2, 13},{1, 14},{},{},{},{},{},{},{},{}}};
uint8_t SENSORS_STOP [16] = {0, 2, 3, 5, 7, 9, 10, 11, 12, 14, 15, 16, 18, 20, 22, 24};

//timers
unsigned int searchTimer = 0;
unsigned int routeTimer = 10000;
bool modeSearch = false;
uint8_t nextRoute = NDX_RED;
uint8_t STOP_NDX = 16;

XMLDocument serviceXML;

/* Helper functions */
// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

CapTouch* touchSensors = new CapTouch();
Pixels* busStrip = new Pixels();
Pixels* timeStrip = new Pixels();
Pixels* stopsStrip = new Pixels();

void pixelsSetup() {
  Serial.println("Bus Planner start!");

  touchSensors->setup();

  // put your setup code here, to run once:
  timeStrip->setup(14, 16, 14, true);
  //delay(1000);
  busStrip->setup(14, 15, 2, true);
  //delay(1000);
  stopsStrip->setup(25, 13, 12, false);
  //stopsStrip->setup(25, 12, 13, true);
  //delay(1000);

  timeStrip->fill(0x000000, true);
  busStrip->fill(0x000000, true);
  stopsStrip->fill(0x000000, true);

  Serial.println("Route loop!");
  busStrip->initialize(2, 1);
  timeStrip->initialize(2, 1);
  stopsStrip->initialize(2, 1);
}

void networkSetup() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();

  for(uint8_t i = 0; i < 2 && WiFi.status() != WL_CONNECTED; i++) {
  Serial.print("Connecting to ");
  Serial.println(ssids[i]);
    WiFi.begin(ssids[i], passwords[i]);
  }
    
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Serial.println("MAC address: "); Serial.println(WiFi.macAddress());
  Serial.println("");
}

void setup() {
  Serial.begin(115200);
  delay(100);

  pixelsSetup();
  
  networkSetup();
}

// Network

void getHttpResponse(HTTPClient *http) {
    // get lenght of document (is -1 when Server sends no Content-Length header)
    String response = "";
    char* buff;
    int len = http->getSize();

    // create buffer for read
    int total = 0;
    int maxlen = 128;

    // get tcp stream
    WiFiClient * stream = http->getStreamPtr();

    buff = new char[len];

    // read all data from server
    while(http->connected() && (len > 0 || len == -1)) {
        // get available data size
        size_t size = stream->available();

        if(size) {
            // read up to 128 byte
            int c = stream->readBytes(buff + total, size);

            if(len > 0) {
                len -= c;
                total += c;
            }
        }
    }

  serviceXML.Parse(buff, total);
  
  free (buff);
  buff = NULL;
}

void loadService(String url) {
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(host, 443, url, "F5 69 8C CA 29 68 5E 47 26 38 C5 1A 18 F1 8A 6A EA 60 56 4D"); //HTTP

    Serial.printf("[HTTP] GET... %s%s\n", host, url.c_str());
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        Serial.printf("[HTTP] GET... size: %d\n", http.getSize());
        if(httpCode == HTTP_CODE_OK) {
            Serial.println("---------------------------");

            getHttpResponse(&http);
        }
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}
// End Network

// Lights

uint32_t getBusColor(uint8_t route) {
  if (route == NDX_TROLLEY) {
    return Color(0xFF, 0xFF, 0x00);
  } else if (route == NDX_RED) {
    return Color(0xFF, 0x00, 0x00);
  } else if (route == NDX_BLUE) {
    return Color(0x00, 0x00, 0xFF);
  } else { // Green
    return Color(0x00, 0xFF, 0x00);
  }
}

void selectStop(uint8_t ndx, uint8_t route) {
  stopsStrip->fill(0x000000, true);

  stopsStrip->set(ndx, getBusColor(route), true);
}

void selectBus(uint8_t route) {
  uint8_t start = 0;
  uint8_t length = 0;
  uint32_t color = getBusColor(route);

  if (route == NDX_TROLLEY) {
    start = 0;
    length = 3;
  } else if (route == NDX_RED) {
    start = 4;
    length = 2;
  } else if (route == NDX_BLUE) {
    start = 8;
    length = 2;
  } else { // Green
    start = 11;
    length = 3;
  }

  busStrip->fill(0x000000, true);

  busStrip->fill(color, true, start, start + length);
}

void displayWaitingTime(uint8_t route, int waiting) {
  uint8_t total = (uint8_t) round(waiting / 2.5) + 1;

  timeStrip->fill(0x000000, true);

  if (waiting >= 0) {
    timeStrip->fill(getBusColor(route), true, 0, total);
    //timeStrip->fillAnimation(getBusColor(route), 0, total);
    //timeStrip->animate(800, 1000);
  }
}

void loopSearch() {
    // Update search
}

String getRoutesStopsUrl(uint8_t route) {
  String url = "";
  for (uint8_t i = 0; i < STOPS_COLLECTION[STOP_NDX][route]; i++) {
    if (i > 0) url += '&';
    url += "stops=" + LINES_NAMES[route] + "|" + STOPS_NAMES[route][STOPS_NDX[route][STOP_NDX][i]];
  }

  return url;
}

int getWaitingTime(XMLElement * element) {
  if (element) {
    element = element->FirstChildElement( "predictions" );
    if (element) {
      element = element->FirstChildElement( "direction" );
      if (element) {
        element = element->FirstChildElement( "prediction" );
        if (element) {
          return atoi(element->Attribute( "minutes" ));
        }
      }
    }
  }

  return -1;
}

void loopRoute() {
    // Update route cycle

    if (millis() >= routeTimer) {
      String url = getRoutesStopsUrl(nextRoute);

      if (url != "" ) {
        url = "/multiPredictions?" + url;
  
        Serial.printf("URL %s", url.c_str());
        Serial.println("");
        loadService(url);
        int waitingTime = getWaitingTime(serviceXML.RootElement());

        Serial.println(url);
        Serial.printf("Waiting time: %d\n", waitingTime);
        serviceXML.Parse("");
        serviceXML.Clear();
  
        selectStop(STOP_NDX, nextRoute);
        selectBus(nextRoute);
        displayWaitingTime(nextRoute, waitingTime);

        if (waitingTime >= 0) {
          routeTimer = millis() + 10000;
        }
      }

      nextRoute++;
      nextRoute = nextRoute % 4;
    }
    
}

void routeIndicator() {
  
}

// End Lights


void checkTouch() {
  
}

void detectTouch(uint16_t touched) {
  if (touched == 0) {
    // No touch detected
    return;
  }
  
  for (uint16_t i=0; i<16; i++) {
    if (touched & (1 << i)) {
      STOP_NDX = SENSORS_STOP[i];
      Serial.printf("Stop %d\n", i);
      break;
    }
  }
}

void loop() {
  timeStrip->loop();
  busStrip->loop();  
  stopsStrip->loop();

  if (!stopsStrip->animating()) {
    bool* touchArray = touchSensors->readArray();
  
    detectTouch(touchSensors->read());
  
    if (modeSearch) {
      loopSearch();
    } else {
      loopRoute();
    }
  }
}
