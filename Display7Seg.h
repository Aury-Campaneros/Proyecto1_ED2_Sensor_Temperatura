#ifndef __DISPLAY7SEG_H__
#define __DISPLAY7SEG_H__
#include <Arduino.h>
//Definir variables globales
extern uint8_t pinA, pinB, pinC, pinD, pinE, pinF, pinG;

//Función para configurar display de 7 segmentos
void configurarDisplay(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E, uint8_t F, uint8_t G);
//función para desplegar el digito de 7 segmentos
void desplegar7seg(uint8_t digito);
#endif // __DISPLAY7SEG_H__