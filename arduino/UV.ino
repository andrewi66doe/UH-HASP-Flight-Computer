/* 
source: https://outsidescience.wordpress.com/2012/11/03/diy-science-measuring-light-with-a-photodiode-ii/
Here is a bare-bones Arduino sketch to read the
   outputs of the photodiode detectors described
   in Figure 1 and Figure 2. The output pin of the
   circuit's opamp is connected to analog pin 0
   of the Arduino. The data are read every 0.1 second
   in this example. However, you can change the sampling
   rate by simply changing the argument of the
   delay() statement. 
*/
 
#define inPin0 0 /* analog pin 0 */
#define inPin1 1 /* analog pin 1 */
#define inPin2 2 /* analog pin 2 */
 
void setup(void) {
  Serial.begin(38400);     
}
 
void loop(void) {

  Serial.print("Diode 0 is (V): ");             /* This prints out what's in parantheses. */
  double pinRead0 = analogRead(inPin0);        /* Integer value named pinRead0 is assigned the value of what the analog pin is reading, in this case it's pin 0. */
  double pVolt0 = pinRead0 / (1024.0 * 5.0);   /* Floating point value named pVolt0 is assigned the value of pinRead0 divided by (1024.0*5.0). */
  Serial.println(pVolt0);                   /* Prints out value of pVolt0. */
  
//  Serial.print("Diode 1 is (V): ");
//  int pinRead1 = analogRead(inPin1);
//  double pVolt1 = pinRead1 / (1024.0 * 5.0);
//  Serial.println(pVolt1);
//
//  Serial.print("Diode 2 is (V): ");
//  int pinRead2 = analogRead(inPin2);
//  double pVolt2 = pinRead2 / (1024.0 * 5.0);
//  Serial.print(pVolt2);
//  Serial.println();

  Serial.println();
  delay(1000);
   
}

