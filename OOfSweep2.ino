#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> 
#endif //THis is the code needed to control the Adafruit NeoPixel

#define LED_PIN    6 //Pin for the NeoPixel
#define LED_COUNT 60 //The max amount of NeoPixels attached to the Arduino
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); //The declaration of the NeoPixel

#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int pos = 0; //Sets starting potion for servo

int fsrPin = 0;     //Pin for Force Sensor
int fsrReading;     // The Analog Reading of the Force Sensor

void setup(void) {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif //Sets the control of time for the NeoPixel
  myservo.attach(9); //The Digital Pin for the servo
  Serial.begin(9600); //Initializes Serial Communication at 9600 bits per second
  strip.begin();           //Initializes the NeoPixel strip
  strip.show();            //Sets Pixel as off when the program starts
  strip.setBrightness(50); //Controls the Brightness
}

void loop(void) {
  fsrReading = analogRead(fsrPin); //Reads the values obtained from the Sensor
  Serial.print("Analog reading = "); //These two lines write out what the values are
  Serial.print(fsrReading);     
  if (fsrReading < 50) { //This statement states if the force is below 50, it will not trigger anything
    Serial.println(" - No pressure"); //to help me confirm it is in the right state
  } else { //If it is above 50, it will start this sequence
    Serial.println(" - Touched"); //Confirms that the state has been executed
    colorWipe(strip.Color(0,   255,   0), 0); // The Green Light. For some reason, this causes an extra rotation/jittering from the servo. I believe that because it is using a delay, it causes that, but even setting it to 0 doesn't remove the problem.
    for (pos = 0; pos <= 150; pos += 1) { // goes from 0 degrees to 180 degrees at 1 degree at a time
      myservo.write(pos);              // tells the servo to go to position in variable 'pos'
      delay(17);                       // waits 17ms for the servo to reach the position.
    }
    colorWipe(strip.Color(255,   0,   0), 0);//The Red Light
    for (pos = 150; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees. There is no juttering here, which is what confused me the most!
      myservo.write(pos);              
      delay(17);                       //Similar time as before, but can be set to 3ms for Lion Breath exercise
    }
    delay(100); //The Delay Needed before another reading can be done
  }
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment. This part was confused, and I was unsure what to do. 
  }
}
