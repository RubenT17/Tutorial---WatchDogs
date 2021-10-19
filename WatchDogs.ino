 /*
        TUTORIAL PARA EL MANEJO DEL TIMER WATCHDOGS DE ARDUINO
 * ************************************************************************
 *
 *  Created on:   20.07.2021
 *  Author:       Rubén Torres Bermúdez <rubentorresbermudez@gmail.com>
 *
 *  Description:
 *    Este archivo .ino contiene un tutorial para aprender a configurar el
 *    WatchDogs que tiene Arduino en su microcontrolador ATmel AVR.
 *  
 *   2021 Rubén Torres Bermúdez.
 *
 *   This project is licensed under the GNU General Public License v3.0
 *
 * ************************************************************************
*/


#include<avr/wdt.h> /* Cabecera para el WDT */

/*
THRESHOLD     VALUE           CONSTANT NAME SUPPORTED ON
15 ms       WDTO_15MS       ATMega 8, 168, 328, 1280, 2560
30 ms       WDTO_30MS       ATMega 8, 168, 328, 1280, 2560
60 ms       WDTO_60MS       ATMega 8, 168, 328, 1280, 2560
120 ms      WDTO_120MS      ATMega 8, 168, 328, 1280, 2560
250 ms      WDTO_250MS      ATMega 8, 168, 328, 1280, 2560
500 ms      WDTO_500MS      ATMega 8, 168, 328, 1280, 2560
1 s         WDTO_1S         ATMega 8, 168, 328, 1280, 2560
2 s         WDTO_2S         ATMega 8, 168, 328, 1280, 2560
4 s         WDTO_4S         ATMega 168, 328, 1280, 2560
8 s         WDTO_8S         ATMega 168, 328, 1280, 2560
*/

void setup() {
  Serial.begin(9600); 
  Serial.println("Watchdog Demo");
  pinMode(13, OUTPUT);
  watchdogSetup(); //Funcion para ajustar el Watch Dogs
}


void watchdogSetup(void) {
  noInterrupts();       
  wdt_reset(); // reset WD timer

/*    **VER PDF**
 WDTCSR configuration:
 WDIE = 1: Interrupt Enable
 WDE = 1 :Reset Enable
 WDP ==> Dependiendo del valor, activa el watch dogs en un tiempo diferente
           Cuanto menos valor tenga la palabra, menos tiempo tardará va reduciendo en 1/2
           0101 -> 1s
           0110 -> 2s
           0111 -> 4s
           1000 -> 8s
           1001 -> 16s          
*/

// **** Entrar al modo de configuración del WatchDogs ****
  WDTCSR |= (1<<WDCE) | (1<<WDE);    //Dar este valor para entrar en sus parámetros
  
  // Set Watchdog settings:
  WDTCSR = (1<<WDIE) | (1<<WDE) | (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (0<<WDP0);
  interrupts(); //Activa las interrupciones del sistema
}

ISR(WDT_vect) {   //Esta interrupción se activa momentos antes de que se ejecute el WatchDogs, asi que el codigo debe de ser muy corto
  digitalWrite (12, HIGH);
}

void loop() {
  for(int i = 0; i<20; i++) /* Blink LED unos segundos */
  {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    wdt_reset(); /* Reset WD timer */
  }
  digitalWrite (13, HIGH);
  while(1){ /* Loop infinito que causa el rset */
    Serial.println ("Bloqueo");
  }
}
