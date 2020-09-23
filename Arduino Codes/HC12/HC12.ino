#include <SoftwareSerial.h>
#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <Wire.h>
const byte HC12RxdPin = 4;                  // Recieve Pin on HC12
const byte HC12TxdPin = 5;                  // Transmit Pin on HC12
BME280I2C bme;
SoftwareSerial HC12(HC12TxdPin,HC12RxdPin); // Create Software Serial Port

void setup() {
  
  Serial.begin(9600);                       // Open serial port to computer
  while(!Serial) {} // Wait
  Wire.begin();
  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
  HC12.begin(9600);                         // Open serial port to HC12
  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }
}

void loop() {
  printBME280Data(&Serial);
  delay(500);
  if(HC12.available()){                     // If Arduino's HC12 rx buffer has data
    Serial.write(HC12.read());              // Send the data to the computer
    }
  if(Serial.available()){                   // If Arduino's computer rx buffer has data
    HC12.write(Serial.read());              // Send that data to serial
  }
}

void printBME280Data
(
   Stream* client
)
{
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);
  HC12.print (pres);
   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? "C" :"F"));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->print(" Pa");
   client->print("\n");
   EnvironmentCalculations::AltitudeUnit envAltUnit  =  EnvironmentCalculations::AltitudeUnit_Meters;
   EnvironmentCalculations::TempUnit     envTempUnit =  EnvironmentCalculations::TempUnit_Celsius;

   float altitude = EnvironmentCalculations::Altitude(pres, envAltUnit);
   float dewPoint = EnvironmentCalculations::DewPoint(temp, hum, envTempUnit);
   float seaLevel = EnvironmentCalculations::EquivalentSeaLevelPressure(altitude, temp, pres);
   // seaLevel = EnvironmentCalculations::SealevelAlitude(altitude, temp, pres); // Deprecated. See EquivalentSeaLevelPressure().
  HC12.write (altitude);
   /*client->print("\t\tAltitude: ");
   client->print(altitude);
   client->print((envAltUnit == EnvironmentCalculations::AltitudeUnit_Meters ? "m" : "ft"));
   client->print("\t\tDew point: ");
   client->print(dewPoint);
   client->print("°"+ String(envTempUnit == EnvironmentCalculations::TempUnit_Celsius ? "C" :"F"));
   client->print("\t\tEquivalent Sea Level Pressure: ");
   client->print(seaLevel);
   client->println(" Pa");

*/   delay(1000);
}
