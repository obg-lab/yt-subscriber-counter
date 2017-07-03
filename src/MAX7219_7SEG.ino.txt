#include "LedControl.h"
// EasyESP or NodeMCU Pin D8 to DIN, D7 to Clk, D6 to LOAD, no.of devices is 1
LedControl lc=LedControl(D8,D7,D6,1);

void setup()
{
 // Initialize the MAX7219 device
  lc.shutdown(0,false);   // Enable display
  lc.setIntensity(0,15);  // Set brightness level (0 is min, 15 is max)
  lc.clearDisplay(0);     // Clear display register
}
void loop()
{
 // Blink all segments 4 times
 for(int k=0; k<4; k++){
   for(int i=0; i<8; i++){
     lc.setDigit(0,i,8,true);
   }  // End i
   delay(300);
   lc.clearDisplay(0);
   delay(300);
 } // End k

 // Display 0 through E
 for(int k=0; k<15; k++){
   for(int i=0; i<8; i++){
    lc.setDigit(0,i,k,false);
  }// End i
  delay(1000);
 } // End k

}
