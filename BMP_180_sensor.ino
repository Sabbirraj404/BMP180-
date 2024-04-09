#include <LiquidCrystal_I2C.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#define sensor A1
int gas, co2lvl;
LiquidCrystal_I2C lcd(0x27, 20, 4);
double T, P, S, A;
char status;
// You will need to create an SFE_BMP180 object, here called "pressure":
SFE_BMP180 pressure;
#define ALTITUDE 1655.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters
void setup()
{
  pinMode(sensor, INPUT);
  Serial.begin(9600);

  lcd.begin(20, 4);  
   lcd.init();  /*LCD display initialized*/
  lcd.clear();     /*Clear LCD Display*/
  lcd.backlight();                            
  
  lcd.setCursor (0,1);
  lcd.print("  Air Quality Meter  ");
 
  delay(3000);
  lcd.clear();

Serial.begin(9600);
Serial.println("BMP180 Measurements");


 lcd.init();  /*LCD display initialized*/
  lcd.clear();     /*Clear LCD Display*/
  lcd.backlight();
// Initialize the sensor (it is important to get calibration values stored on the device).
if (pressure.begin())
Serial.println("BMP180 init success");
else
{
// Oops, something went wrong, this is usually a connection problem,
// see the comments at the top of this sketch for the proper connections.
Serial.println("BMP180 init fail\n\n");
while(1); // Pause forever.
}
}
void loop()
{
 gas = analogRead(sensor);
  co2lvl = gas-120;
  co2lvl = map(co2lvl,0,1024,400,5000);
  Serial.println(gas);
  lcd.setCursor (0,0);
  lcd.print("CO2 :");
  lcd.setCursor(6,0);
  lcd.print(co2lvl);
  lcd.print("ppm");
  delay(1000);
  
  if((co2lvl >= 350)&&(co2lvl <= 1000))
  {
    lcd.print("Good");
  

  }

  else if((co2lvl >= 1000)&&(co2lvl <= 2000))
  {
 
    lcd.print("Bad");
     
    
  }
  else
  {
    lcd.print("Danger");
   
  }
  delay(1000);


  lcd.clear();


  
char status;
double T,P,p0,a;
// Loop here getting pressure readings every 10 seconds.
// If you want sea-level-compensated pressure, as used in weather reports,
// you will need to know the altitude at which your measurements are taken.
// We're using a constant called ALTITUDE in this sketch:
Serial.println();
Serial.print("provided altitude: ");
/*lcd.setCursor(0, 1);
lcd.print("Alt: ");
Serial.print(ALTITUDE,0);
Serial.print(" meters, ");
Serial.print(ALTITUDE*3.28084,0);
lcd.print(ALTITUDE*3.28084,0);
Serial.println(" feet");
lcd.print(" ft");*/
// If you want to measure altitude, and not pressure, you will instead need
// to provide a known baseline pressure. This is shown at the end of the sketch.
// You must first get a temperature measurement to perform a pressure reading.
// Start a temperature measurement:
// If request is successful, the number of ms to wait is returned.
// If request is unsuccessful, 0 is returned.
status = pressure.startTemperature();
if (status != 0)
{
// Wait for the measurement to complete:
delay(status);
// Retrieve the completed temperature measurement:
// Note that the measurement is stored in the variable T.
// Function returns 1 if successful, 0 if failure.
status = pressure.getTemperature(T);
if (status != 0)
{
// Print out the measurement:
Serial.print("temperature: ");
Serial.print(T,2);
Serial.print(" deg C, ");
Serial.print((9.0/5.0)*T+32.0,2);
Serial.println(" deg F");
lcd.setCursor(0, 1);
lcd.print("Tem: ");
lcd.print(T,1);
lcd.print(" C ");
// Start a pressure measurement:
// The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
// If request is successful, the number of ms to wait is returned.
// If request is unsuccessful, 0 is returned.
status = pressure.startPressure(3);
if (status != 0)
{
// Wait for the measurement to complete:
delay(status);
// Retrieve the completed pressure measurement:
// Note that the measurement is stored in the variable P.
// Note also that the function requires the previous temperature measurement (T).
// (If temperature is stable, you can do one temperature measurement for a number of pressure.)
// Function returns 1 if successful, 0 if failure.
status = pressure.getPressure(P,T);
if (status != 0)
{
// Print out the measurement:
Serial.print("absolute pressure: ");
Serial.print(P,2);
Serial.print(" mb, ");
Serial.print(P*0.0295333727,2);
Serial.println(" inHg");
lcd.setCursor(0, 2);
lcd.print("APr.:");
lcd.print(P*0.0295333727,2);
lcd.print("inHg");
// The pressure sensor returns abolute pressure, which varies with altitude.
// To remove the effects of altitude, use the sealevel function and your current altitude.
// This number is commonly used in weather reports.
// Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
// Result: p0 = sea-level compensated pressure in mb
p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
Serial.print("relative (sea-level) pressure: ");
Serial.print(p0,2);
Serial.print(" mb, ");
Serial.print(p0*0.0295333727,2);
Serial.println(" inHg");

// On the other hand, if you want to determine your altitude from the pressure reading,
// use the altitude function along with a baseline pressure (sea-level or other).
// Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
// Result: a = altitude in m.
a = pressure.altitude(P,p0);
Serial.print("computed altitude: ");
Serial.print(a,0);
Serial.print(" meters, ");
Serial.print(a*3.28084,0);
Serial.println(" feet");
}
else Serial.println("error retrieving pressure measurement\n");
}
else Serial.println("error starting pressure measurement\n");
}
else Serial.println("error retrieving temperature measurement\n");
}
else Serial.println("error starting temperature measurement\n");
delay(1000); // Pause for 5 seconds.
}
