 /*=============================================
 *               DESCRIPCION:
 *=============================================
 * 
 * Este codigo es el que utiliza el 
 * microcontrolador PIC16F887
 * para controlar el sistema del reloj
 * inteligente. Este codigo incluye la libreria
 * para utilizar la LCD Nokia5110 como display
 * del reloj.
 * 
 * ============================================*/

#include "Nokia5110.h"
#include <pic16f887.h>

#define BOTON1 PORTEbits.RE2
#define BOTON2 PORTEbits.RE1
#define BOTON3 PORTEbits.RE0


unsigned char str3[15];
unsigned char a, b, d, cont1;
unsigned char contador1, contador2, contador3;
int *dias2, *dias1, *mes2, *mes1, *a2, *a1, *horas2, *horas1, *minutos2, *minutos1, *seg2, *seg1, *control1, *control2;

int menu(dias2, dias1, mes2, mes1, a2, a1, horas2, horas1, minutos2, minutos1, seg2, seg1, control1, control2){
    
EYRLCD_WriteString("Ajuste Hora", 1, 10); //muestra hora en pantalla
EYRLCD_WriteString("Ajuste Fecha", 1, 11); //muestra hora en pantalla
EYRLCD_WriteString("Otras", 3, 12); //muestra hora en pantalla 

     if ((control1) == 2){
         (seg1) ++;
         (control1) = 0;
     }
    
     if ((seg1) == 10){
         (seg1) = 0;
         (seg2)++;
     }
     
     if ((seg2) == 6){
         (seg1) = 0;
         (seg2) = 0;
         (minutos1)++;
         
     }
     
     if ((minutos1) == 10){
         (minutos1) = 0;
         (minutos2)++;
     }

     if (minutos2 == 6){
         minutos1 = 0;
         minutos2 = 0;
         horas1++;
     }
     
     if (horas1 == 10){
         minutos1 = 0;
         minutos2 = 0;
         horas2++;
     } 
     
     if (horas2 == 1 && horas1 == 3){
         minutos1 = 0;
         minutos2 = 0;
         horas1 = 1;
         horas2 = 0;
         
         //c++;
         
     }      

//  if (c == 2){
//      uD++;
//      c = 0;
//  }
//  
//  if (uD == 10){
//      dD++;
//      uD = 0;
//  }
//  
//  c2 = uMes + dMes;
//  
//  if ( (c2 == 1) ||  (c2 == 3) ||  (c2 == 5) ||  (c2 == 7) ||  (c2 == 8)){
//      if (dD == 3 && uD == 2){
//          uD = 1;
//          dD = 0;
//          uMes++;
//      }
//  }
//  else if ((uMes == 2) && (dMes == 0)){
//      if ((dD == 2) && (uD == 9)){
//          uD = 1;
//          dD = 0;
//          uMes++;
//      }
//  }
//  else {
//      if ((dD == 3) && (uD == 1)) {
//          uD = 1; 
//          dD = 0;
//          uMes++;
//      }
//  }
//  
//  if (uMes == 10){
//      uMes = 0;
//      dMes++;
//  }
//  
//  if ((dMes == 1) && (uMes == 3)){
//      uA++;
//      uMes = 1;
//      dMes = 0;
//      uD = 1;
//      dD = 0;
//  }
//  
//  if(uA == 10){
//      uMes = 1;
//      dMes = 0;
//      uD = 1;
//      dD = 0;
//      uA = 0;
//      dA++;
//      
//  }
  
  if (BOTON3 == 1){
    cont1 = 0;
} 
  else cont1 = 1;
  

return cont1;
}