#include <SPI.h>

//Arduino code to read analog data and convert the values to 8 bit chunks to be transferred via SPI to be read by
//the VPForce mainboard. The settings are based on the VPForce Shift Register in series with the Arduino. The arduino
//MISO pin is connected to the shift register input and the shift register output in connected to the mainboard input.
//THIS CODE WILL NOT WORK WITHOUT THE SHIFT REGISTER AND ARDUINO AS A PAIR.

//Based on the WinWing profile in the VPForce configurator which uses the following structure per Walmis:
// union {
//   struct {
//     uint8_t buttons [7];
//     uint8_t reserved; 
//     uint16_t axis[4]; // range 0..4096, center at 2048
//   } __attribute((packed));
//   uint8_t data[16];
// } ;
//
// Grip Debug gives the following: 
// FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00
// Each pair is an 8 bit number (hex), the structure reserves the first 7 pairs as buttons, the 8th pair is blank, and the
// remaining 8 pairs are the axis data, with two consecutive pairs making up the axis value (9&10, 11&12, 13&14, 15&16)


int analogPin1 = A1; // potentiometer wiper (middle terminal) connected to analog pin 1
int analogPin2 = A2; // potentiometer wiper (middle terminal) connected to analog pin 2
int analogPin3 = A3; // potentiometer wiper (middle terminal) connected to analog pin 3
int analogPin4 = A4; // potentiometer wiper (middle terminal) connected to analog pin 4
int val1 = 0;  // variable to store the value read
int val2 = 0;  // variable to store the value read
int val3 = 0;  // variable to store the value read
int val4 = 0;  // variable to store the value read
volatile byte lowerbit1 = 0;
volatile byte upperbit1 = 0; 
volatile byte lowerbit2 = 0;
volatile byte upperbit2 = 0; 
volatile byte lowerbit3 = 0;
volatile byte upperbit3 = 0; 
volatile byte lowerbit4 = 0;
volatile byte upperbit4 = 0; 
volatile byte pullcount; // variable to count the number of SPI pulls



void setup() {
  //Serial.begin(115200);           //  setup serial

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT); 
  
  //intitialize pull counter
  pullcount = 0; 

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

  //interrupt routine on pin 2, calls resetCount function on falling edge of SS signal
  //without this call, the analog data doesn't initialize in the proper order
  attachInterrupt(0, resetCount, FALLING);
    

}

// reset the pull counter when called (interrupt)
void resetCount()
{
  pullcount = 0; 
}

// SPI interrupt routine
//This interrupt is called everytime SPI transfer is called from the master
ISR (SPI_STC_vect)
{
// With the output of the shift register going to the VPForce board, and the Arduino as an input to the shift register,
// pullcount 1 and 2 are set as 0 to position the analog values to the correct position in the SPI queue.


 if (pullcount == 3) //pair 9
    {
      SPDR = lowerbit1;
      pullcount++; 
    }
    else if (pullcount == 4) //pair 10
    {
      SPDR = upperbit1;
      pullcount++;  
    }
    else if (pullcount == 5) //pair 11
    {
      SPDR = lowerbit2;
      pullcount++;  
    }
    else if (pullcount == 6) //pair 12
    {
      SPDR = upperbit2;
      pullcount++;  
    }
    else if (pullcount == 7) //pair 13
    {
      SPDR = lowerbit3;
      pullcount++;  
    }
    else if (pullcount == 8) //pair 14
    {
      SPDR = upperbit3;
      pullcount++;  
    }
    else if (pullcount == 9) //pair 15
    {
      SPDR = lowerbit4;
      pullcount++;  
    }
    else if (pullcount == 10) //pair 16
    {
      SPDR = upperbit4;
      pullcount++;  
    }
    else 
    {
      SPDR = 0;
      pullcount++; 
    }
  

}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine

void loop()
{  
    val1 = analogRead(analogPin1);  // read the input pin 1
    val2 = analogRead(analogPin2);  // read the input pin 2
    val3 = analogRead(analogPin3);  // read the input pin 3
    val4 = analogRead(analogPin4);  // read the input pin 4
    // the data shifts 8 bits at a time. The ADC of the Arduino is a 10 bit ADC, so 
    // the upperbit is the top 2 bits and the lowerbit is the lower 8 bits of the 10 bit value
    // future release could consider shifting the 10 bit data 2 bits left for a scaled 12 bit value since 
    // the WinWing structure is given for a 12 bit value (0..4095)
    upperbit1 = val1 >> 8;
    lowerbit1 = val1;
    upperbit2 = val2 >> 8;
    lowerbit2 = val2;
    upperbit3 = val3 >> 8;
    lowerbit3 = val3;
    upperbit4 = val4 >> 8;
    lowerbit4 = val4;
}
