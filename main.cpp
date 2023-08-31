// Librerías
//****************************************************************
#include <Arduino.h>
#include "driver/ledc.h"
#include "Display7Seg.h"
#include "config.h"
//****************************************************************
// Definición de etiquetas
//****************************************************************
// selección de parámetros de la btn PWM
#define pwmChannel_servo 0 // 16 canales 0-15
#define freqPWM1 50        // Frecuencia en Hz
#define resolution 8       // 8 bits de resolución

// Pines de los LED y el Servo
#define ledR 12
#define ledG 14
#define ledY 13
#define servo 27
// Pines del display 7 seg
#define A 23
#define B 22
#define C 21
#define D 19
#define E 18
#define F 5
#define G 4
// Pines de los transistores
#define display1 33
#define display2 32
#define display3 25

// Variables
int btn_Anterior = 0; // Guarda el estado del Pin 36
float temp = 0;       // guarda la lectura de la temperatura
int temp1 = 0;
float temp2 = 0;
float mvolts = 0;
int decenas = 0;  // guarda el valor de la decena de la temperatura
int unidades = 0; // guarda el valor de la unidad de la temperatura
int decimal = 0;  // guarda el valor del decimal de la temperatura

// Configurar el feed 'Temperatura'
AdafruitIO_Feed *tempCanal = io.feed("Temperatura");

void setup()
{
  Serial.begin(115200);
  // Esperamos que abra el monitor serial
  while (!Serial)
    ;
  Serial.print("Conexión a Adafruit IO");
  // conectarse a io.adafruit.com
  io.connect();
  // espera una conexión
  while (io.status() < AIO_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  // Nosotros estamos conectados
  Serial.println();
  Serial.println(io.statusText());

  // llamar la función de configuracionDisplay
  configurarDisplay(A, B, C, D, E, F, G);

  // Declaración de Salidas
  pinMode(ledR, OUTPUT);     // LedRojo
  pinMode(ledG, OUTPUT);     // LedVerde
  pinMode(ledY, OUTPUT);     // LedAmarillo
  pinMode(servo, OUTPUT);    // Control del servo
  pinMode(display1, OUTPUT); // transistor 1
  pinMode(display2, OUTPUT); // Transistor 2
  pinMode(display3, OUTPUT); // Transistor 3

  // Declaración de Entradas ¡
  pinMode(36, INPUT); // Botón para adquirir la btn

  // Configurar el módulo PWM
  ledcSetup(pwmChannel_servo, freqPWM1, resolution);
  // seleccionar en que GPIO tendremos nuestra señal PWM
  ledcAttachPin(servo, pwmChannel_servo);

  io.run(); // Permite conectarse con el servidor de Adafruit IO
}

void loop()
{
  // Inicializo todos los display en cero.
  digitalWrite(display1, LOW);
  digitalWrite(display2, LOW);
  digitalWrite(display3, LOW);

  /************************ Parte 1 - Sensor de Temperatura *******************************/
  volatile int btn = digitalRead(36);   // lectura del pin 36 - Botón
  volatile int sensor = analogRead(34); // lectura del sensor

  if (btn == 1 && btn_Anterior == 0) // Permite adquirir la Lectura del sensor
  {
    temp = (sensor * 3.3 / 4096) * (1 / 0.01);
    temp2 = temp*10;//permite desplegar la temperatura en el Display
    /***************************** Parte 6 - ESP32 WIFI  ************************************/
    // guardar el recuento en el feed 'Temperatura' de Adafruit IO
    Serial.print("Enviando -> ");
    Serial.println(temp);
    tempCanal->save(temp);
  }
  btn_Anterior = btn;
  delay(2); // estabiliza la variable btn_Anterior
  Serial.printf(" Temperatura %f \n", temp);

  /****************** Parte 2 y 3 Semáforo y reloj de temperatura *************************/
  if (temp < 12)
  {
    digitalWrite(ledG, HIGH);
    digitalWrite(ledR, LOW);
    digitalWrite(ledY, LOW);
    ledcWrite(pwmChannel_servo, 11);
  }
  if (temp > 14)
  {
    digitalWrite(ledG, LOW);
    digitalWrite(ledR, HIGH);
    digitalWrite(ledY, LOW);
    ledcWrite(pwmChannel_servo, 27);
  }
  if (temp >= 12 && temp <= 14)
  {
    digitalWrite(ledG, LOW);
    digitalWrite(ledR, LOW);
    digitalWrite(ledY, HIGH);
    ledcWrite(pwmChannel_servo, 19);
  }

  /************************ Parte 4 - Despliegue de temperatura*****************************/
  // Asigna el digito que debe aparecer en cada display
  temp1 = temp2;
  decenas = temp1 / 100;
  temp1 = temp1 - (decenas * 100);
  unidades = temp1 / 10;
  temp1 = temp1 - (unidades * 10);
  decimal = temp1;

  // Activa los pines de los transistores
  digitalWrite(display1, HIGH); // asigna el valor del decimal al display 1
  digitalWrite(display2, LOW);
  digitalWrite(display3, LOW);
  desplegar7seg(decimal);
  delay(5);

  digitalWrite(display1, LOW);
  digitalWrite(display2, HIGH); // asigna el valor del la unidad al display 2
  digitalWrite(display3, LOW);
  desplegar7seg(unidades);
  delay(5);

  digitalWrite(display1, LOW);
  digitalWrite(display2, LOW);
  digitalWrite(display3, HIGH); // asigna el valor del la decena al display 3
  desplegar7seg(decenas);
  delay(5);

  /************************ Parte 5 - Dashboard Adafruit IO  *******************************/
  // Diseño de interfaz
  // en los servidores de Adafruit IO, en donde pueda mostrar el
  // resultado de los valores obtenidos del sensor de temperatura
}
