#include <SPI.h>

//Arduino code to read analog data and convert the values to 8 bit chunks to be transferred via SPI to be read by
//the VPForce mainboard. The settings are based on using only an Arduino as the ADC and button interface. The arduino
//MISO pin is connected to the mainboard input. This code can be used up to a maximum of 16 inputs with 4 axes maximum.
//That means that using 4 axis there will be 12 buttons and with 2 axes there will be 14 buttons, etc.
//THIS CODE WILL NOT WORK WITH THE SHIFT REGISTER AND ARDUINO AS A PAIR.

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


int analogPin1 = A0; // Axis input 1
int analogPin2 = A1; // Axis input 2
//int analogPin3 = A2; // Axis input 3
//int analogPin4 = A3; // Axis input 4
int digPin0 = A2; // Button input
int digPin1 = A3; // Button input
int digPin2 = A4; // Button input
int digPin3 = A5; // Button input
int val1 = 0;  // variable to store the value read by an axis
int val2 = 0;  // variable to store the value read by an axis
//int val3 = 0;  
//int val4 = 0; 
volatile byte lowerbit1 = 0;
volatile byte upperbit1 = 0; 
volatile byte lowerbit2 = 0;
volatile byte upperbit2 = 0; 
// volatile byte lowerbit3 = 0;
// volatile byte upperbit3 = 0; 
// volatile byte lowerbit4 = 0;
// volatile byte upperbit4 = 0; 
volatile byte buttonbit1 = 255; 
volatile byte buttonbit2 = 255; 
volatile byte pullcount; // variable to count the number of SPI pulls
int dval1;
int dval2; 
int dval3; 
int dval4;
int dval5; 
int dval6;
int dval7;
int dval8;
int dval9;
int dval10;
int dval11;
int dval12;
int dval13;
int dval14;
//int dval15;
//int dval16; 



void setup() {
  //Serial.begin(115200);           //  setup serial (debug only)

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT); //Pin 12
  pinMode(SS, INPUT);  //Pin 10
  pinMode(digPin0, INPUT_PULLUP); //Button, default state high. 
  pinMode(digPin1, INPUT_PULLUP); //Button, default state high. 
  pinMode(digPin2, INPUT_PULLUP); //Button, default state high. 
  pinMode(digPin3, INPUT_PULLUP); //Button, default state high. 
  pinMode(11, INPUT_PULLUP); //Button, default state high. 
  pinMode(9, INPUT_PULLUP); //Button, default state high. 
  pinMode(8, INPUT_PULLUP); //Button, default state high. 
  pinMode(7, INPUT_PULLUP); //Button, default state high. 
  pinMode(6, INPUT_PULLUP); //Button, default state high. 
  pinMode(5, INPUT_PULLUP); //Button, default state high. 
  pinMode(4, INPUT_PULLUP); //Button, default state high. 
  pinMode(3, INPUT_PULLUP); //Button, default state high. 
  pinMode(1, INPUT_PULLUP); //Button, default state high.  
  pinMode(0, INPUT_PULLUP); //Button, default state high.  
  
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


 if (pullcount == 0) // byte 2
    {
      SPDR = 255; 
      pullcount++; 
    }
    else if (pullcount == 1) //byte 3
    {
      SPDR = buttonbit1;
      pullcount++; 
    }
    else if (pullcount == 2) //byte 4
    {
      SPDR = buttonbit2;
      pullcount++;
    }
    else if (pullcount == 3) //byte 5
    {
      SPDR = 0;
      pullcount++; 
    }
    else if (pullcount == 4) //byte 6
    {
      SPDR = 0;
      pullcount++;  
    }
    else if (pullcount == 5) //byte 7
    {
      SPDR = 0;
      pullcount++;  
    }
    else if (pullcount == 6) //byte 8
    {
      SPDR = 0;
      pullcount++;  
    }
    else if (pullcount == 7) //byte 9
    {
      SPDR = lowerbit1;
      pullcount++;  
    }
    else if (pullcount == 8) //byte 10
    {
      SPDR = upperbit1;
      pullcount++;  
    }
    else if (pullcount == 9) //byte 11
    {
      SPDR = lowerbit2;
      pullcount++;  
    }
    else if (pullcount == 10) //byte 12
    {
      SPDR = upperbit2;
      pullcount++;  
    }
    else if (pullcount == 11) //byte 13
    {
      SPDR = 0;
      pullcount++;  
    }
    else if (pullcount == 12) //byte 14
    {
      SPDR = 0;
      pullcount++;  
    }
    else if (pullcount == 13) //byte 15
    {
      SPDR = 0;
      pullcount++;  
    }
    else if (pullcount == 14) //byte 16
    {
      SPDR = 0;
      pullcount++;  
    }
    else 
    {
      SPDR = 255;
      pullcount++; 
    }
  

}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine

void loop()
{  

    val1 = analogRead(analogPin1);  // read the input pin 1
    val2 = analogRead(analogPin2);  // read the input pin 2
    //val3 = analogRead(analogPin3);  // read the input pin 3
    //val4 = analogRead(analogPin4);  // read the input pin 4

    // the data shifts 8 bits at a time. The ADC of the Arduino is a 10 bit ADC, so 
    // the upperbit is the top 2 bits and the lowerbit is the lower 8 bits of the 10 bit value
    // future release could consider shifting the 10 bit data 2 bits left for a scaled 12 bit value since 
    // the WinWing structure is given for a 12 bit value (0..4095)

    upperbit1 = val1 >> 8;
    lowerbit1 = val1;
    upperbit2 = val2 >> 8;
    lowerbit2 = val2;
    // upperbit3 = val3 >> 8;
    // lowerbit3 = val3;
    // upperbit4 = val4 >> 8;
    // lowerbit4 = val4;

    //pack the button inputs into an 8 bit where 1 = open, 0 = pressed
    if (digitalRead(11) == LOW)
      {
        dval1 = 254; //11111110
        buttonbit1 &= dval1;
      }
    else
      {
        dval1 = 1; //00000001
        buttonbit1 |= dval1; 
      }
      
    if (digitalRead(digPin1) == LOW)
      {
        dval2 = 253; //11111101
        buttonbit1 &= dval2;
      }
    else
      {
        dval2 = 2; //00000010
        buttonbit1 |= dval2;  
      }
      
    if (digitalRead(digPin2) == LOW)
      {
        dval3 = 251; //11111011
        buttonbit1 &= dval3;
      }
    else
      {
        dval3 = 4; //00000100
        buttonbit1 |= dval3;
      }

    if (digitalRead(digPin3) == LOW)
      {
        dval4 = 247; //11110111
        buttonbit1 &= dval4;
      }
    else
      {
        dval4 = 8; //00001000
        buttonbit1 |= dval4;
      }

    if (digitalRead(9) == LOW)
      {
        dval5 = 239; //11101111
        buttonbit1 &= dval5;
      }
    else
      {
        dval5 = 16; //00010000
        buttonbit1 |= dval5;
      }

    if (digitalRead(8) == LOW)
      {
        dval6 = 223; //11011111
        buttonbit1 &= dval6;
      }
    else
      {
        dval6 = 32; //00100000
        buttonbit1 |= dval6;
      }

    if (digitalRead(7) == LOW) 
      {
        dval7 = 191; //10111111
        buttonbit1 &= dval7;
      }
    else
      {
        dval7 = 64; //01000000
        buttonbit1 |= dval7; 
      }

    if (digitalRead(6) == LOW)
      {
        dval8 = 127; //01111111
        buttonbit1 &= dval8;
      }
    else
      {
        dval8 = 128; //10000000
        buttonbit1 |= dval8;
      }


    // Serial.print("buttonbit1: "); //debug
    // Serial.println(buttonbit1, BIN); //debug

    if (digitalRead(5) == LOW)
      {
        dval9 = 254; //11111110
        buttonbit2 &= dval9;
      }
    else
      {
        dval9 = 1;
        buttonbit2 |= dval9; 
      }

    if (digitalRead(4) == LOW)
      {
        dval10 = 253; //11111101
        buttonbit2 &= dval10;
      }
    else
      {
        dval10 = 2; //00000010
        buttonbit2 |= dval10;
      }

    if (digitalRead(3) == LOW) 
      {
        dval11 = 251; //11111011
        buttonbit2 &= dval11;
      }
    else
      {
        dval11 = 4; //00000100
        buttonbit2 |= dval11;
      }

    if (digitalRead(1) == LOW) //problems with pin 1 (TX/RX?) removing for testing
      {
        dval12 = 247; //11110111
        buttonbit2 &= dval12; 
      }
    else
      {
        dval12 = 8; //00001000
        buttonbit2 |= dval12; 
      }

      if (digitalRead(0) == LOW)
      {
        dval13 = 239; //11101111
        buttonbit2 &= dval13;
      }
    else
      {
        dval13 = 16; //00010000
        buttonbit2 |= dval13;
      }

    if (digitalRead(digPin0) == LOW)
      {
        dval14 = 223; //11011111
        buttonbit2 &= dval14;
      }
    else
      {
        dval14 = 32; //00100000
        buttonbit2 |= dval14;
      }

  
    // Serial.print("buttonbit2: "); //debug
    // Serial.println(buttonbit2, BIN); //debug
    //  delay(500); //debug

    
}
