#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <SD.h>

#define DHTPIN 2
#define DHTTYPE DHT11

//Defino nombre del archivo para la MicroSD
File logFile;
String fileName = "data"+String(random(999)) + ".txt";

DHT dht(DHTPIN, DHTTYPE);

int tiempoActual = 0;
int tiempoEspera = 5;
int tiempoConteo = 0;

void setup() {
    Serial.begin(9600);
    dht.begin();
   
    tiempoActual = millis()/1000;
    tiempoConteo = tiempoActual-1;    

    Serial.print(F("Iniciando SD ..."));
    if (!SD.begin(9))
    {
      Serial.println(F("Error al iniciar"));
      return;
    }
    Serial.println(F("Iniciado correctamente"));

    while (SD.exists(fileName))
    {
      fileName = fileName.substring(0, 4) + String(random(999)) + ".csv";
    }
    Serial.println("Archivo:"+fileName);
}

String leerSensor()
{
  float h = dht.readHumidity();         // Leemos la humedad relativa
  float t = dht.readTemperature();      // Leemos la temperatura en grados centígrados (por defecto)
  float f = dht.readTemperature(true);  // Leemos la temperatura en grados Fahrenheit
  
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
     Serial.println("Error obteniendo los datos del sensor DHT11");
     return "";
  }
  
  float hif = dht.computeHeatIndex(f, h);         // Calcular el índice de calor en Fahrenheit
  float hic = dht.computeHeatIndex(t, h, false);  // Calcular el índice de calor en grados centígrados
  String datoSensor = String(t)+";"+String(h)+";"+String(f)+";"
                      +String(hic)+";"+String(hif);
  return datoSensor;
}

void guardarMicroSD(String datosSensados)
{
  logFile = SD.open(fileName, FILE_WRITE); 
  if (logFile) { 
    logFile.print(String(tiempoActual)+";"+datosSensados+"\n");

    /*
    logFile.print("Segundos:");
    logFile.print(tiempoActual);
    logFile.print(" datos:");
    logFile.println(datosSensados); */
    logFile.close();
  } 
  else {
    Serial.println("Error al abrir el archivo: " + fileName);
  } 
}

void loop() {
  if (tiempoActual > tiempoConteo + tiempoEspera){
      String datosSensados = leerSensor();
      Serial.println(datosSensados);
      guardarMicroSD(datosSensados);
      tiempoConteo = tiempoActual - 1;      
    }
    else {
      tiempoActual = millis()/1000;
    }
 }
