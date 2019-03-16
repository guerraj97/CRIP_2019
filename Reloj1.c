/*
 * File:   Reloj.c
 * Autor: Jose Pablo Guerra
 * 
 *==============================================
 *               DESCRIPCION:
 *==============================================
 * 
 * Este codigo es el que utiliza el 
 * microcontrolador PIC16F887
 * para controlar el sistema del reloj
 * inteligente. Este codigo incluye la libreria
 * para utilizar la LCD Nokia5110 como display
 * del reloj.
 * 
 * ==============================================
 *
 * Creado el 5 de septiembre, 2018, 3:07 PM
 * *************************************************************************************************
 * Rev.         Día            Comentarios
 *   v0.0.0      5/09/2018      Version inicial del programa aun no realiza nada.
 *   v0.1.0      8/09/2018      El reloj ya funciona. Utiliza el TMR1 para contar los segundos. 
 *                              Ademas, ya muestra en pantalla la hora y el nombre del reloj.
 *   v0.2.0      15/09/2018     El reloj ya muestra la fecha y la hora. Para la fecha, ya muestra 
 *                              los meses con 31 dias, 30 dias, y cuenta febrero con 28 dias.
 *   v0.2.1      22/09/2018     El reloj ya esta probado fisicamente. Correcciones a la libreria
 *                              de inicializacion por errores en el TIMER1.
 *                              Arreglado conteo de segundos para que no vaya tan rapido. 
 *   v0.3.0      28/09/2018     El menu ya esta implementado, solamente muestra opciones 
 *                              y el conteo del reloj no se detiene, el reloj continua 
 *                              funcionando aunque se este en el menu. 
 *   v0.4.0      03/10/2018     El menu contien las funciones ajustar hora y fecha. El reloj continua
 *                              funcionando aun estando en el editor de fecha.
 *   v0.4.1      05/10/2018     Arreglado el antirrebote para el menu principal, el menu de ajuste y ajuste 
 *                              de hora y fecha.
 *                              Ademas, creacion de la libreria "Funciones_Reloj" y "Funciones.c" para 
 *                              mejor control de las funciones del antirrebotes. El antirrebotes se aplica
 *                              utilizando el TMR0.
 *   v0.4.2      06/10/2018     El antirrebotes no funcionaba del todo, siempre entraba y salia de los menus.
 *                              Problema arreglado en esta version, ahora es posible ajustar la fecha y la hora
 *                              una unidad a la vez sin que se vaya de largo al dejar presionado el boton.
 *   v0.5.0      11/10/2018     Funcion de alarma implementada, se puede ajustar y la comparacion es exitosa.
 *   v0.5.1      11/10/2018     La alarma es posible activarla y desactivarla al gusto del usuario. Arreglos finales a 
 *                              los titulos y otros ajustes. 
 *                              
 ***************************************************************************************************/


//Librerias para utilizar el PIC16F887
#include <xc.h>
#include <pic16f887.h>
#include <stdio.h>
#include <stdlib.h>

//Incluye el archivo de la libreria para controlar la LCD y otras librerias
#include "Nokia5110.h"
#include "InitPIC.h" //Libreria para inicializar el PIC
#include "Funciones_Reloj.h" //Esta libreria contiene todas las funciones que se usan en el reloj, no incluidas aqui.


//************************************
//         Configuracion PIC
//************************************
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//Declaracion de variables
unsigned char uS = 0, dS = 0, uM = 7, dM = 0, uH= 3, dH = 0, c = 0, c2 = 0, cS = 0, luz = 1, mostrar = 0, mostrar2 = 0; //variables para control de la hora
//cS controla los segundos para que cuenten correctamente - c2 controla el cambio de dia al ser reloj de 12 horas 

unsigned char alarma = 0, apagar = 0, encender = 0, al[3] ="No";

extern const unsigned char Logo[];

unsigned char  AuM = 0, AdM = 0, AuH= 1, AdH = 0; //variables para control de la alarma
unsigned char  ctrl1 = 0, ctrl2 = 0, ctrl3 = 0, ctrl4 = 0, cambio = 0; // controla el acceso a los menus, ctrl1 controla el menu general, ctrl2, por ahora, controla el menu ajustes
unsigned char uD = 1, dD =1, uMes = 0, dMes = 1, uA = 8, dA = 1; //variables para control de la fecha

unsigned char str[10], str2[10], pos=10, str3[10], str4[11], mostrar6[3]; //variables para mostar en pantalla. Pos controla la posicion del cursor en el menu. 

unsigned char valTMR1; //el valor para reiniciar el timer1

unsigned char b1int = 0;        //bandera de boton 1 (interrupcion)
unsigned char b2int = 0;        //bandera de boton 2 (interrupcion)
unsigned char b3int = 0;        //bandera de boton 3 (interrupcion)
unsigned char b4int = 0;        //bandera de boton 4 (interrupcion)


//***************************
//Vector de interrupcion
//***************************
void interrupt ISR(){//rutina de atención a las interrupciones
   if(TMR1IF==1)
    {
        cS++;   // complement the value for blinking the LEDs
        if (cS == 10){
            cS = 0;
        }
        if (c == 3){
            c = 0;
        }
        TMR1H = 0b01010110;
        TMR1L = 0b00011110;
        TMR1IF=0;       // Clear timer interrupt flag
        
    }
   
   if (T0IF == 1){
    b1int = 1;                  //bandera de interrupcion para el antirebotes
    b2int = 1;
    b4int = 1;
    b3int = 1;
    TMR0 = 101;                 //ponemos el valor del timer 0 de nuevo
    T0IF = 0;                   //apagamos la bandera de la interrupcion
   }
   
    }


//***************************
//Definicion de constantes
//***************************

#define _XTAL_FREQ 4000000
#define BOTON1 PORTEbits.RE2
#define BOTON2 PORTEbits.RE1
#define BOTON3 PORTEbits.RE0
#define BOTON4 PORTDbits.RD6
#define BUZZER PORTBbits.RB5
#define LUZ PORTCbits.RC2

//***************************
//Funciones
//***************************


/*******************************************************************************
 * Function:        int comparacion(void)
 * Description:     Compara los valores del reloj y alarma para activarla
 * Precondition:    None
 * Parameters:      None
 * Return Values:   coloca un 1 en alarma y retorna indicando que la comparacion
 *                  fue exitosa, o un 0 sino lo fue.
 * Remarks:         None
 * ****************************************************************************/

int comparacion(void){
    
    hora();
    fecha();
    
    if((encender == 1) && (uM == AuM) && (dM == AdM) && (uH == AuH) && (dH == AdH)){
        alarma = 1;
    }
    else alarma = 0;
    return alarma;
}


//***************************
//Programa principal
//***************************
void main(void) {        
    OSC4(); //inicializacion del reloj interno
    initPorts(); // configuracion de los puertos
     __delay_ms(10);
    EYRLCD_Init();
    EYRLCD_Clear();

    EYRLCD_Bitmap(Logo, 0, 0);
    __delay_ms(2500);
    EYRLCD_Clear();
    EYRLCD_WriteString("Bienvenido", 9, 8);
    EYRLCD_WriteString("Ingeniero", 10, 9);
    EYRLCD_WriteString("Jose Guerra", 5, 11); 
    __delay_ms(2500);
    initTMR1();
    initTMR0();
    EYRLCD_Clear();
    __delay_ms(10);
    
 while(1){  
    
     activar_alarma();
     control_luz();
     
     if (luz == 1){
         LUZ = 1;
     }
     else if (luz == 2){
         LUZ = 0;
     }
     else if (luz == 3){
         luz = 1;
     }
     if (apagar == 2){
         encender = 1;
         BUZZER = 1;
         sprintf(al, "%s", "Si"); // mueve el valor de las variables al array para imprimir. 
     }
     else if (apagar == 4){
         apagar = 0;
         encender = 0;
         BUZZER = 0;
        sprintf(al, "%s", "No"); // mueve el valor de las variables al array para imprimir.
     }
     
     
//control del reloj     
     
     if (cS == 9){
         uS ++;
         cS = 0;
     }
    
     if (uS == 10){
         uS = 0;
         dS++;
         if (comparacion() == 1){
         PORTBbits.RB4 = 1;
     }
         else if (comparacion() == 0){
             PORTBbits.RB4 = 0;
         }
     }
     
     if (dS == 6){
         uS = 0;
         dS = 0;
         uM++;
         
         
     }
     
     if (uM == 10){
         uS = 0;
         dS = 0;
         uM = 0;
         dM++;
            
     }

     if (dM == 6){
         uS = 0;
         dS = 0;
         uM = 0;
         dM = 0;
         uH++;
     }
     
     if (uH == 10){
         uS = 0;
         dS = 0;
         uM = 0;
         dM = 0;
         dH++;
     } 
     
     if (dH == 1 && uH == 3){
         uS = 0;
         dS = 0;
         uM = 0;
         dM = 0;
         uH = 1;
         dH = 0;
         
         c++;
         
     }      
    
    
//control de la fecha    

  if (c == 2){
      uD++;
      c = 0;
  }
  
  if (uD == 10){
      dD++;
      uD = 0;
  }
  
  c2 = uMes + dMes;
  
  if ( (c2 == 1) ||  (c2 == 3) ||  (c2 == 5) ||  (c2 == 7) ||  (c2 == 8)){
      if (dD == 3 && uD == 2){
          uD = 1;
          dD = 0;
          uMes++;
      }
  }
  else if ((uMes == 2) && (dMes == 0)){
      if ((dD == 2) && (uD == 9)){
          uD = 1;
          dD = 0;
          uMes++;
      }
  }
  else {
      if ((dD == 3) && (uD == 1)) {
          uD = 1; 
          dD = 0;
          uMes++;
      }
  }
  
  if (uMes == 10){
      uMes = 0;
      dMes++;
  }
  
  if ((dMes == 1) && (uMes == 3)){
      uA++;
      uMes = 1;
      dMes = 0;
      uD = 1;
      dD = 0;
  }
  
  if(uA == 10){
      uMes = 1;
      dMes = 0;
      uD = 1;
      dD = 0;
      uA = 0;
      dA++;
      
  }
  

    sprintf(str4, "Alarma: %s", al); // mueve el valor de las variables al array para imprimir.     
    sprintf(str, "%d%d:%d%d:%d%d", dH, uH, dM,uM, dS, uS); // mueve el valor de las variables al array para imprimir.
    //sprintf(mostrar6, "%d", valTMR1); // mueve el valor de las variables al array para imprimir.
    EYRLCD_WriteString("CRIP", 25, 8); // el nombre del reloj al que posteriormente se le cambiara.
    EYRLCD_WriteString(str4, 9, 9); // el nombre del reloj al que posteriormente se le cambiara.
    EYRLCD_WriteString(str, 12, 11); //muestra hora en pantalla
    sprintf(str2, "%d%d/%d%d/%d%d", dD, uD, dMes,uMes, dA, uA); // mueve el valor de las variables al array para imprimir.
    EYRLCD_WriteString(str2, 12, 13); //muestra hora en pantalla
    //EYRLCD_WriteString(mostrar6, 12, 10); //muestra hora en pantalla
 
    
    if (chequeo_mostrar() == 1){
    EYRLCD_Clear();
    mostrar = 0;
    while (mostrar2 < 5){
    EYRLCD_Bitmap(Logo, 0, 0);
    mostrar2++;
    hora();
    fecha();
    }
    mostrar2 = 0;
    EYRLCD_Clear();
    }
    
    //control de ingreso al menu general del reloj.
    ctrl1 = chequeo_menu();
                   while (ctrl1 == 1){
                       ctrl1 = menu();
               }
    }
               
    }
