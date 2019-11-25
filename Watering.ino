#include <SoftwareSerial.h>

#include <SimpleDHT.h>
// Librería  <SimpleDHT.h> del sensor de temperatura y humedad relativa
#include <SPI.h>
//Librería <SPI.h> del motor DC compatible con la minibomba de agua DC
#define humidity_sensor_pin A0
//Se define variable del sensor de humedad en el suelo en el pin A0
#define ldr_pin A5
//Defino variable del sensor de fotoresistencia LDR en el pin A5
 
SoftwareSerial blue(4,5); // bluetooth module connected here 
 
int Vresistor = A0; 
int Vdata = 0; 
// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
// Se declara la variable pinDHT11 y lo asocio al pin 2
SimpleDHT11 dht11;
int ldr_value = 0;
//Se declara la variable ldr_value (fotoresistencia) y 
int water_pump_pin = 3;
//Se declara la variable mini bomba de agua y lo asocio al pin 3
int water_pump_speed = 255;
//Velocidad de la minibomba de agua oscila entre 100 como mínimo y 255 como máximo. Yo he 
//elegido 255 pero ustedes pueden elegir la que estimen conveniente. A más velocidad, mayor 
//bombeo de agua
 
void setup(){ 
    
  pinMode(Vresistor, INPUT); 
  Serial.begin(9600); 
  blue.begin(9600); 
   
} 
void loop(){ 
  
  // start working…
// Mide la temperatura y humedad relativa y muestra resultado
//  .println(“***********”);
  Serial.println("Sample DHT11…");
  // read with raw sample data.
  byte temperature = 0;
  byte humidity_in_air = 0;
  byte data[40] = {0};
//  if (dht11.read(pinDHT11, &temperature, &humidity_in_air, data)) {
//    Serial.print("Read DHT11 failed");
//    return;
//  }
  
  Serial.print("Sample RAW Bits: ");
  for (int i = 0; i < 40; i++) {
    Serial.print((int)data[i]);
    if (i > 0 && ((i + 1) % 4) == 0) {
      Serial.print(" ");
    }
  }
  Serial.println("");
  
  Serial.print("Sample OK: ");
  Serial.print("Temperature: ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print("Relative humidity in air: ");Serial.print((int)humidity_in_air); Serial.println(" %");
  
  // DHT11 sampling rate is 1HZ.

  // Ground humidity value in %
// Mide la humedad en el suelo en % y muestra el resultado
  
  int ground_humidity_value = map(analogRead(humidity_sensor_pin), 0, 1023, 100, 0);
  Serial.print("Ground humidity: ");
  Serial.print(100-ground_humidity_value);
  blue.print(100-ground_humidity_value);
  Serial.println("%");

  

//**********************
// Condiciones de riego 
// Si la humedad en el suelo es igual o inferior al 50%, si la luminosidad es inferior al 30%,
// Si la temperatura es inferior al 30%, entonces el sistema de riego riega. 
// En caso de que no se  cumpla alguno o ninguno de los 3 requisitos anteriores,
// el sistema de riego no riega
//**********************
 if( (100-ground_humidity_value) <= 20) {
 digitalWrite(water_pump_pin, HIGH);
 Serial.println("Irrigate");
 analogWrite(water_pump_pin, water_pump_speed);
//El motor de la bomba de agua arranca con la velocidad elegida anteriormente en el código
 }
 else{
 digitalWrite(water_pump_pin, LOW);
 Serial.println("Do not irrigate");
//El motor de la bomba de agua se para y no riega
 }
 delay (1000); 
// Ejecuta el código cada 100 milisegundos
  
}
