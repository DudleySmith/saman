
#include "ofxXbeeDummyProtocolDefine.h"
#include "FastSPI_LED2.h"

#define NUM_LEDS 32

const int   greenPin = 12; // led sign for good infos -> Green
const int   redPin = 13; // led sign for good infos -> Red

const CRGB  white = CRGB(255,255,255);

CRGB leds_2[NUM_LEDS];
CRGB leds_3[NUM_LEDS];
CRGB leds_4[NUM_LEDS];
CRGB leds_5[NUM_LEDS];
CRGB leds_6[NUM_LEDS];
CRGB leds_7[NUM_LEDS];
CRGB leds_8[NUM_LEDS];
CRGB leds_9[NUM_LEDS];
CRGB leds_10[NUM_LEDS];
CRGB leds_11[NUM_LEDS];

#define nbLoopsForHeartBeat 5000
double nbLoops;

String sXbeeID;
String reading="";
      
void setup() {
  nbLoops = 0;
  // initialize serial communication:
  Serial.begin(9600);
  //Serial.setTimeout(100);
  
  Serial.println();
  Serial.println("-------------------");
  Serial.println("Init");
  
  sXbeeID = XBEEID_NONE;
  // initialize the LED pin as an output
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  // Initialize LEDS
  setupLEDS();
}

void setupLEDS() {
  Serial.println("Setup Leds");
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);

  // For safety (to prevent too high of a power draw), the test case defaults to
  // setting brightness to 25% brightness
  LEDS.addLeds<WS2811, 2>(leds_2, NUM_LEDS);
  LEDS.addLeds<WS2811, 3>(leds_3, NUM_LEDS);
  LEDS.addLeds<WS2811, 4>(leds_4, NUM_LEDS);
  LEDS.addLeds<WS2811, 5>(leds_5, NUM_LEDS);
  LEDS.addLeds<WS2811, 6>(leds_6, NUM_LEDS);
  LEDS.addLeds<WS2811, 7>(leds_7, NUM_LEDS);
  LEDS.addLeds<WS2811, 8>(leds_8, NUM_LEDS);
  LEDS.addLeds<WS2811, 9>(leds_9, NUM_LEDS);
  LEDS.addLeds<WS2811, 10>(leds_10, NUM_LEDS);
  LEDS.addLeds<WS2811, 11>(leds_11, NUM_LEDS);
  
  allLEDS(getArray(2), 0);
  allLEDS(getArray(3), 0);
  allLEDS(getArray(4), 0);
  allLEDS(getArray(5), 0);
  allLEDS(getArray(6), 0);
  allLEDS(getArray(7), 0);
  allLEDS(getArray(8), 0);
  allLEDS(getArray(9), 0);
  allLEDS(getArray(10), 0);
  allLEDS(getArray(11), 0);
  
  LEDS.setBrightness(255);    
  LEDS.show();
}

void loop() {
  // Sending heartbeat every n loops
  nbLoops++;
  if(nbLoops>nbLoopsForHeartBeat){
    nbLoops = 0;
    if(sXbeeID==XBEEID_NONE){
      sXbeeID = "0001";
      Serial.println(HEAD + sXbeeID + ID_ASKING + TAIL);
    }else{
      Serial.println(HEAD + sXbeeID +  HEARTBEAT + TAIL);
    }
  }
  
  // Receiving message character 1 by 1 --
  while(Serial.available()>0){
    // Blinking --
    isBad();
    // Reading a char --
    char charRead;
    reading += (char)Serial.read();
    if(reading.startsWith(HEAD)){
      // Good Message, how is the tail ?
      if(reading.endsWith(TAIL)){
        // The tail is good
        isGood();
        // We can treat
        doYourStuffWithMessage(reading);
        // and reset
        reading = "";
      }else{
        // No tail, we wait
        isGoodBad();
      }
    }else{
      // Bad message --
      isBad();
      // We reset
      reading = "";
    }
  }
}  

void doYourStuffWithMessage(String _message){
   
  bool idControl = false;
  String sIDReceived;
  String bigMode = "";
  String fineMode = "";
  int    pinNum = 0;
  int    arg1 = 0;
  int    arg2 = 0;
  /*
  Serial.println(_message);
  Serial.println("OK !!!!!!!!!!");
  */
    // On controle l'entete
    // On controle l'ID
    sIDReceived = _message.substring(1,5);
    if(sIDReceived == sXbeeID){
      // On cherche le big Mode
      bigMode   = _message.substring(5,8);
      fineMode  = _message.substring(8,11);
      pinNum    = _message.substring(11,14).toInt();
      arg1      = _message.substring(14,17).toInt();
      arg2      = _message.substring(17,20).toInt();
      
      // GOOD DATAS ----------------
      if(bigMode==BigMode_Out){
        /*
        Serial.println(bigMode);
        Serial.println(fineMode);
        Serial.println(pinNum);
        Serial.println(arg1);
        Serial.println(arg2);
        */
        // We're driving outputs
        if(fineMode==FineMode_All){
          /*
          The whole strip
          [0001OUTALL004255]
          all leds on among value received
          */
          isGood();
          allLEDS(getArray(pinNum), arg1);
          LEDS.show();
        }
        else if(fineMode==FineMode_Drop){
          /*
          Drop mode
          [0001OUTDRP003120050]
          Drop the LEDS
          */ 
          isGood();
          drop(getArray(pinNum), arg1, arg2);
          LEDS.show();
        }else{
          isBad();
        }
      }else{
        isBad();
      }
    }else{
      isBad();
    }
    
}

// LEDS Section -----------------------------------
CRGB* getArray(int _pinNum){
  
  switch(_pinNum){
    case 2:
      return leds_2;
      break;
    case 3:
      return leds_3;
      break;
    case 4:
      return leds_4;
      break;
    case 5:
      return leds_5;
      break;
    case 6:
      return leds_6;
      break;
    case 7:
      return leds_7;
      break;
    case 8:
      return leds_8;
      break;
    case 9:
      return leds_9;
      break;
    case 10:
      return leds_10;
      break;
    case 11:
      return leds_11;
      break;
    default:
      return leds_2;
  }
}

void drop(CRGB _leds[NUM_LEDS], int _position, int _smoothness){
  
  //  [0001OUTDRP003120050]
  
  int realPosition = ((float)_position/255.0f)*NUM_LEDS;
  int minPosition = max(0,((float)(_position-_smoothness)/255.0f)*NUM_LEDS);
  /*
  Serial.println("Real/Min/Ratio/Color");
  Serial.println(realPosition);
  Serial.println(minPosition);
  */
  memset(_leds, 0,  NUM_LEDS * sizeof(struct CRGB));
  
  for(int idxLed = minPosition; idxLed<=realPosition; idxLed++){
   float ratio = (float)(realPosition-idxLed)/(realPosition-minPosition);
   //Serial.println(ratio);
   
   CRGB color;
   color.r = 128*ratio*ratio;
   color.g = 128*ratio*ratio;
   color.b = 128*ratio*ratio;
   
   Serial.println(color.r);
   
   _leds[idxLed] = color;
  
  }
  
  
}

void allLEDS(CRGB _leds[NUM_LEDS], float _value){
  memset(_leds, _value,  NUM_LEDS * sizeof(struct CRGB));
}

void isGood(){
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);
}
void isBad(){
  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, HIGH);
}
void isGoodBad(){
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, HIGH);
}
