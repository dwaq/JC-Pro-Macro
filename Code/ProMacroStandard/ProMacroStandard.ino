//Example for JC Pro Macro board
//set up to work in Mac OS
//Includes support for 1306 display
//Reference: https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h
//Reference: https://arduinogetstarted.com/tutorials/arduino-button-long-press-short-press 
//
//To do:
//Figure out what to do with bottom support - remake offset so locks in?
//caps lock code? - does not work properly on Mac it seems

//========================================================

#include <SPI.h>
#include <Wire.h>

// Declare variables etc=========================================

bool SW1 = 1; //encoder button
bool SW2 = 1; //lower-left keyswitch
bool SW3 = 1;
bool SW4 = 1; //lower-right keyswitch
bool SW5 = 1;
bool SW6 = 1; //upper-right keyswitch
bool increment = 0;
bool decrement = 0;
long oldPosition;
long newPosition;
int inputMode = 0;
int LEDLight = 1;
int LEDCircle[4] = {0, 1, 3, 2};
int counter = 0;
bool nascar = 0;
long newNascarTurnTime = 0;
long oldNascarTurnTime = 0;
long newBeatTime = 0;
long oldBeatTime = 0;
bool beatOn = 0;

int fanSpeed = 0;
bool fanPulse = 0;
long newPulseTime = 0;
long oldPulseTime = 0;
int fanRPM = 0;
char toneNote;

//int modeArray[] = {0}; //adjust this array to modify sequence of modes - as written, change to {0, 1, 2, 3, 4, 5} to access all modes
//int inputModeIndex = 0;
//int modeArrayLength = (sizeof(modeArray) / sizeof(modeArray[0]));

// Encoder setup =============================================
// from: https://github.com/PaulStoffregen/Encoder
#include <Encoder.h>
// from: https://github.com/NicoHood/HID
#include <HID-Project.h>
Encoder myEnc(0,1); //if rotation is backwards, swap 0 and 1

// Screen setup =============================================

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// NeoPixel setup =============================================

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        5 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 4 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

//============================================================

void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP); //SW1 pushbutton (encoder button)
  pinMode(15, INPUT_PULLUP); //SW2 pushbutton
  pinMode(A0, INPUT_PULLUP); //SW3 pushbutton
  pinMode(A1, INPUT_PULLUP); //SW4 pushbutton
  pinMode(A2, INPUT_PULLUP); //SW5 pushbutton
  pinMode(A3, INPUT_PULLUP); //SW6 pushbutton

  randomSeed(analogRead(A9));
     
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setRotation(2); //sets rotation 1 through 4 (2 = 180º rotation vs 4)
  display.clearDisplay();
  // Draw text
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(30,0);             // Start at top-left corner
  display.print("Hello, world!");
  display.setCursor(30,30);  
  display.print("test");
  display.display();
  delay(10);
  display.setTextSize(3); 

//setup keyboard and mouse input
//perhaps cound add in a delay so that you can program before this starts up
Mouse.begin();
Keyboard.begin();
//BootKeyboard.begin(); - BootKeyboard use appears to give problems w/ Macintosh


//NeoPixel setup=========================================

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear();


  for(int i=0; i<NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(10, 0, 0));
  }
  pixels.show(); // Show results
}


void loop() {

  SW1 = digitalRead(4);
  SW2 = digitalRead(15);
  SW3 = digitalRead(A0);
  SW4 = digitalRead(A1);
  SW5 = digitalRead(A2);
  SW6 = digitalRead(A3);

  newPosition = myEnc.read();
  
  if (newPosition > (oldPosition + 2)) { 
    increment = 1;
    delay(5);
    oldPosition = myEnc.read();
  }
  if (newPosition < (oldPosition - 2)) {
    decrement = 1;
    delay(5);
    oldPosition = myEnc.read();
  }

//=========change mode=================

//   if ((SW6 == 0) && (SW5 == 0)){ 
//      if (inputModeIndex < modeArrayLength){
//        inputModeIndex++;
//        inputMode = modeArray[inputModeIndex];
//      }
//      if (inputModeIndex == modeArrayLength){
//        inputModeIndex = 0;
//        inputMode = modeArray[inputModeIndex];
//      }
//      SW6 = 1;
//      SW5 = 1;
//      delay(250);
//    }

//================================

//screen(); //need to change to only call within functions

//======select input mode:=======
// only use volume mode
volume();
//if (inputMode == 0) volume();
//if (inputMode == 1) jiggler();
//if (inputMode == 2) slitherIO();
//if (inputMode == 3) FCPX();
//if (inputMode == 4) fan();
//if (inputMode == 5) music();

//Serial.println(inputMode);

}

void volume(){ //works with new code

  if (increment == 1) {
        Consumer.write(MEDIA_VOLUME_UP);
        if (LEDLight == 3) LEDLight = 0;
        else if (LEDLight < 3) LEDLight += 1;
        pixels.clear();
        pixels.setPixelColor(LEDCircle[LEDLight], pixels.Color(10, 0, 0));
        pixels.show(); // Show results
        increment = 0;
        decrement = 0;
        //delay(10);
      }
      
  if (decrement == 1) {
        Consumer.write(MEDIA_VOLUME_DOWN);
        if (LEDLight == 0) LEDLight = 3;
        else if (LEDLight > 0) LEDLight -= 1;
        pixels.clear();
        pixels.setPixelColor(LEDCircle[LEDLight], pixels.Color(10, 0, 0));
        pixels.show(); // Show results
        increment = 0;
        decrement = 0;
        //delay(10);
      }

  // FYI, key codes are here: https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h

  if (SW6 == 0){ // minimize window
        Keyboard.press(KEY_LEFT_WINDOWS);
        Keyboard.press(KEY_DOWN_ARROW);          
        Keyboard.releaseAll();
        // wait for release
        while(digitalRead(A3) == 0) {
          delay(50);
        }
      }
  if (SW5 == 0){ // show desktop
        Keyboard.press(KEY_LEFT_WINDOWS);
        Keyboard.press(KEY_D);
        Keyboard.releaseAll();
        // wait for release
        while(digitalRead(A2) == 0) {
          delay(50);
        }
      }
  if (SW4 == 0) { // print screen
        Keyboard.press(KEY_PRINTSCREEN);
        Keyboard.releaseAll();
        // wait for release
        while(digitalRead(A1) == 0) {
          delay(50);
        }
      }
  if (SW3 == 0) { // microphone mute (PowerToys)
        Keyboard.press(KEY_LEFT_WINDOWS);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press(KEY_A);
        Keyboard.releaseAll();
        // wait for release
        while(digitalRead(A0) == 0) {
          delay(50);
        }
      }
  if (SW2 == 0) { // speaker mute
        Consumer.write(MEDIA_VOLUME_MUTE);
        // wait for release
        while(digitalRead(15) == 0) {
          delay(50);
        }
      }
  if (SW1 == 0) { // power toys run
        Keyboard.press(KEY_LEFT_ALT);        
        Keyboard.press(KEY_SPACE);    
        Keyboard.releaseAll();
        // wait for release
        while(digitalRead(4) == 0) {
          delay(50);
        }
      }
}
