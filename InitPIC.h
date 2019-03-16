/*
 * File:   initPIC.h
 * Autor: Jose Pablo Guerra
 * 
 *=============================================
 *               DESCRIPCION:
 *=============================================
 * 
 * LIBRERIA PARA CONFIGURACION DEL PIC. 
 * INCLUYE CONFIGURACION DEL OSCILADOR INTERNO DE 4MHz Y 8MHz
 * ESTA CONFIGURACION ESTA PARA UTILIZARSE EN EL PIC16F887
 * 
 * ADEMAS INCLUYE:
 * 
 * CONFIGURACION DE LOS PUERTOS
 * TIMER 1 Y 0
 * ADC
 * 
 * ============================================
 *
 * Creado el 8 de septiembre, 2018, 11:51 AM
 * *******************************************************************************
 * Rev.         Día            Comebtarios
 *   v1.0.0      8/09/2018      Falta configurar TMR0 y agregar otras librerias
 *   v2.0.0      8/09/2018      Timer0 configurado, aun faltan otras librerias
 *   v3.0.0      19/09/2018     Timer1 configurado y PWM
 ******************************************************************************/

#include <pic16f887.h>
#include <xc.h>

//***************************
//Declaracion de constantes
//***************************


#define LOW 0b00011110//TMR1L
#define HIGH 0b01010110 // TMR1H
#define _XTAL_FREQ 4000000
#define TMR2PRESCALE 4
long freq;
//en conjunto estas anteriores determinan el valor inicial del Timer1 para utilizarse como temporizador
//puede usarse la constante valTMR1 para declarla como entero en un solo registro. 


//***************************
//Inicializacion de puertos, cambiar de ser necesario.
//***************************

void initPorts(void){
    ANSEL = 0;
    ANSELH = 0;
    PORTB = 0;
    PORTC = 0;
    PORTA = 0;
    PORTD = 0;
    PORTE = 0;
    TRISD = 0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISEbits.TRISE0 = 1;
    TRISEbits.TRISE1 = 1;
    TRISEbits.TRISE2 = 1;
    TRISDbits.TRISD6 = 1;
    TRISCbits.TRISC2 = 0;
}

//***************************
//           TIMERS
//***************************
void initTMR1(){

 
    //valTMR1 = 65536-(tiempo * _XTAL_FREQ)/(8*4);
    TMR1H = HIGH;
    TMR1L = LOW;
    T1CON = 0b00011001; //Configuracion inicial del TMR1
    
    //TMR1 = valTMR1; //descomentar para utilizar un valor entero y no los dos registros de arriba.
    
    TMR1IE=1;       //Habilita interrupciones
    GIE=1;          //Enable Global Interrupt
    PEIE=1;         //Enable the Peripheral Interrupt
    TMR1ON = 1;     //Start Timer1    
}

void initTMR0(void){
    //config del timer 0
    
    OPTION_REGbits.T0CS = 0;        //timer como timer
    OPTION_REGbits.PSA = 0;         //prescaler asignado al timer0
    OPTION_REGbits.PS0 = 1;
    OPTION_REGbits.PS1 = 1;         //prescaler 1:256
    OPTION_REGbits.PS2 = 1;
    //luego vamos a cargarle 101 al timer 0 para que cuente cada 20ms
    
    //enable de las interrupciones
    INTCONbits.GIE = 1;             //interrupciones generales habilitadas
    INTCONbits.T0IE = 1;            //interrupciones del timer 0 habilitadas
    TMR0 = 101;
}
//***************************
//    OSCILADORES INTERNOS
//***************************
void OSC8() {
    IRCF2 = 1;
    IRCF1 = 1;
    IRCF0 = 0; //Oscilador de 8 MHZ.

    OSTS = 0; //utilizo oscilador interno

    HTS = 1; // OSCILADOR ESTABLE
    SCS = 1; // OSCILADOR INTERNO PARA EL CLOCK DEL SISTEMA
}

//----------
void OSC4(void){
    
    IRCF0 = 0;  //FRECUENCIA DE OSCILACION
    IRCF1 = 1;
    IRCF2 = 1;
    OSTS = 0;   //OSCILADOR INTERNO
    HTS = 1;    //OSCILADOR ESTABLE
    SCS = 1;    //OSCILADOR INTERNO
}

//***************************
//        MODULO ADC
//***************************

void ADC(int chanel1, int chanel2, int chanel3, int chanel4){
    
    ADFM = 0;       //JUSTIFICADO A LA IZQUIERDA
    VCFG1 = 0;      //VCC COMO REFERENCIA DE VOLTAJE -
    VCFG0 = 0;      //VDD COMO REFERENCIA DE VOLTAJE +
    
    ADCS0 = 0;      //ADC CONVERSION CLOCK SELECT BITS
    ADCS1 = 1;
    
    CHS0 = chanel1;       //CONFIGURO ANx COMO MI PIN ANALOGICO
    CHS1 = chanel2;
    CHS2 = chanel3;
    CHS3 = chanel4;
    
    ADON = 1;       //HABILITAMOS EL MODULO ADC
    
    return;
}

//*************************
//       MODULO PWM
//*************************

