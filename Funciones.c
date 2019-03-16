/*******************************************************************************
 *
 *                          Libreria Funciones_Reloh
 * 
 *******************************************************************************
 * FileName:        Funciones.c
 * Processor:       PICxxxxxx
 * Complier:        XC8 v1.30
 * Author:          Jose Pablo Guerra
 * Description:     Libreria creada para controlar las funciones del reloj, los 
 *                  antirrebotes, incremento de variables y control de variables
 *                  para los distintos menus que tiene el reloj.
 *******************************************************************************
 * Rev.         Date            Comment
 *   v0.0.0      05/10/2018      Creación del firmware
 *   v0.1.0      13/10/2018      Todas las funciones agregadas y funcionales.
 ******************************************************************************/




#include "Fuente.h"
#include "Funciones_Reloj.h"
#include "Nokia5110.h"
#include <xc.h>
#include <pic16f887.h>
#include <stdio.h>
#include <stdlib.h>

//Declaracion de variables
extern unsigned char uS, dS, uM, dM, uH, dH, c, c2, cS,  mostrar; //variables para control de la hora
//cS controla los segundos para que cuenten correctamente - c2 controla el cambio de dia al ser reloj de 12 horas 


extern unsigned char  AuM, AdM, AuH, AdH, apagar; //variables para control de la hora

extern unsigned char  ctrl1, ctrl2, ctrl3, ctrl4, cambio, ctrl5; // controla el acceso a los menus, ctrl1 controla el menu general, ctrl2, por ahora, controla el menu ajustes
extern unsigned char uD, dD, uMes, dMes, uA, dA, luz; //variables para control de la fecha

extern unsigned char str[9], str2[10], pos, str3[10]; //variables para mostar en pantalla. Pos controla la posicion del cursor en el menu. 


extern unsigned char b1int;        //bandera de boton 1 (interrupcion)
extern unsigned char b2int;        //bandera de boton 2 (interrupcion)
extern unsigned char b3int;        //bandera de boton 3 (interrupcion)
extern unsigned char b4int;        //bandera de boton 4 (interrupcion)
unsigned char b4 = 0, b3 = 0, b2 = 0, b1 = 0;
unsigned char x4 = 0, x3 = 0, x2 = 0, x1 = 0, x5 = 0, x6 = 0, x7 = 0;

#define _XTAL_FREQ 4000000
#define BOTON1 PORTEbits.RE2
#define BOTON2 PORTEbits.RE1
#define BOTON3 PORTEbits.RE0
#define BOTON4 PORTDbits.RD6


/*******************************************************************************
 * Function:        int menu(void)
 * Description:     Esta funcion contra el menu general del reloj
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retonra ctrl1 que le permite al reloj salir o no del menu
 * Remarks:         None
 * ****************************************************************************/

int menu(){
EYRLCD_WriteString("MENU", 25, 8); //muestra ajustes en pantalla
EYRLCD_WriteString("Ajustes", 18, 10); //muestra ajustes en pantalla
EYRLCD_WriteString("Otras", 18, 12); //muestra Otras en pantalla
EYRLCD_WriteString("->", 4, pos); //muestra seleccionador en pantalla


hora();
fecha();
 
if (chequeo_Salirmenu() == 0){
    ctrl1 = 0;
}

else ctrl1 = 1;
  

chequeo_CambioPos();
    if (pos == 14){
        pos = 10;
    EYRLCD_Clear();
    }


if ((chequeo_Seleccion() == 1) && (pos == 10)){
    ctrl2 = 1;
    EYRLCD_Clear();
    pos = 8;
    while (ctrl2 == 1){
        
        ctrl2 = menu_Ajuste();
    }
    EYRLCD_Clear();
}

return ctrl1;
}


/*******************************************************************************
 * Function:        void hora(void)
 * Description:     Esta funcion controla el aumento de la hora, para que el reloj no se detenga
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 * ****************************************************************************/

void hora(void){
        if (cS == 9){
         uS ++;
         cS = 0;
     }
    
     if (uS == 10){
         uS = 0;
         dS++;
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
}


/*******************************************************************************
 * Function:        void fecha(void)
 * Description:     Esta funcion controla el incremento de la fecha, para que el reloj no se detenga.
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 * ****************************************************************************/
        
void fecha(void){
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
}


/*******************************************************************************
 * Function:        int chequeo_menu(void)
 * Description:     control del antirrebote para entrar al menu
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna x4, la bandera del boton, que se pone en 1.
 * Remarks:         None
 * ****************************************************************************/

int chequeo_menu(void){
            if (b4int == 1){                        //Hubo alguna interrupcion?
            if(b4 == 0 & BOTON4 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b4 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera
                   x4 = 1;
                   EYRLCD_Clear();  
            }
            if(BOTON4 == 0){                    //cuando el boton ya lo apague
                b4 = 0;                         //apagar la mera mera bandera
            }
            b4int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return x4;
}

/*******************************************************************************
 * Function:        int chequeo_Salirmenu(void)
 * Description:     control del antirrebote para salir del menu
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna x4, la bandera del boton, que se pone en 0.
 * Remarks:         None
 * ****************************************************************************/


int chequeo_Salirmenu(void){
            if (b4int == 1){                        //Hubo alguna interrupcion?
            if(b4 == 0 & BOTON4 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b4 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera
                   x4 = 0; 
                   EYRLCD_Clear();
            }
            if(BOTON4 == 0){                    //cuando el boton ya lo apague
                b4 = 0;                         //apagar la mera mera bandera
            }
            b4int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return x4;
}


/*******************************************************************************
 * Function:        int chequeo_Salirmenu2(void)
 * Description:     control del antirrebote para salir del menu
 *                  Es necesario esta funcion, debido a los multiples menus que tiene el reloj.
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna x2, la bandera del boton, que se pone en 0.
 * Remarks:         None
 * ****************************************************************************/

int chequeo_Salirmenu2(void){
            if (b4int == 1){                        //Hubo alguna interrupcion?
            if(b4 == 0 & BOTON4 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b4 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera
                   x2 = 0; 
                   EYRLCD_Clear();
            }
            if(BOTON4 == 0){                    //cuando el boton ya lo apague
                b4 = 0;                         //apagar la mera mera bandera
            }
            b4int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return x2;
}

/*******************************************************************************
 * Function:        int chequeo_CambioPos(void)
 * Description:     Control antirrebote para la posicion del cursor del menu
 * Precondition:    None
 * Parameters:      None
 * Return Values:   Retorna el valor de la posicion al ser presionado.
 * Remarks:         None
 * ****************************************************************************/

int chequeo_CambioPos(void){
            if (b3int == 1){                        //Hubo alguna interrupcion?
            if(b3 == 0 & BOTON3 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b3 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera 
                   pos = pos + 2;
                   EYRLCD_Clear();
            }
            if(BOTON3 == 0){                    //cuando el boton ya lo apague
                b3 = 0;                         //apagar la mera mera bandera
            }
            b3int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return pos;
}


/*******************************************************************************
 * Function:        int chequeo_Seleccion(void)
 * Description:     Control antirrebote para la seleccion del menu
 * Precondition:    None
 * Parameters:      None
 * Return Values:   Retorna el valor de x2 al ser presionado, x2 = 1.
 * Remarks:         None
 * ****************************************************************************/

int chequeo_Seleccion(void){
            if (b2int == 1){                        //Hubo alguna interrupcion?
            if(b2 == 0 & BOTON2 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b2 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera
                   x2 = 1; 
            }
            if(BOTON2 == 0){                    //cuando el boton ya lo apague
                b2 = 0;                         //apagar la mera mera bandera
            }
            b2int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return x2;
}

/*******************************************************************************
 * Function:        int chequeo_mostrar(void)
 * Description:     Muestra el logo cada vez que se desea.
 * Precondition:    None
 * Parameters:      None
 * Return Values:   Retorna el valor de x2 al ser presionado, x2 = 1.
 * Remarks:         None
 * ****************************************************************************/

int chequeo_mostrar(void){
            if (b2int == 1){                        //Hubo alguna interrupcion?
            if(b2 == 0 & BOTON2 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b2 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera
                   mostrar = 1; 
            }
            if(BOTON2 == 0){                    //cuando el boton ya lo apague
                b2 = 0;                         //apagar la mera mera bandera
            }
            b2int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return mostrar;
}

/*******************************************************************************
 * Function:        int chequeo_Seleccion2(void)
 * Description:     control de seleccion del menu Ajustes
 *                  Es necesario esta funcion, debido a los multiples menus que tiene el reloj.
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna x5, la bandera del boton, que se pone en 1.
 * Remarks:         None
 * ****************************************************************************/

int chequeo_Seleccion2(void){
            if (b2int == 1){                        //Hubo alguna interrupcion?
            if(b2 == 0 & BOTON2 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b2 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera
                   x5 = 1; 
            }
            if(BOTON2 == 0){                    //cuando el boton ya lo apague
                b2 = 0;                         //apagar la mera mera bandera
            }
            b2int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return x5;
}

/*******************************************************************************
 * Function:        int chequeo_Salirmenu3(void)
 * Description:     control del antirrebote para salir del menu
 *                  Es necesario esta funcion, debido a los multiples menus que tiene el reloj.
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna x5, la bandera del boton, que se pone en 0.
 * Remarks:         None
 * ****************************************************************************/

int chequeo_Salirmenu3(void){
            if (b4int == 1){                        //Hubo alguna interrupcion?
            if(b4 == 0 & BOTON4 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b4 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera
                   x5 = 0; 
                   EYRLCD_Clear();
            }
            if(BOTON4 == 0){                    //cuando el boton ya lo apague
                b4 = 0;                         //apagar la mera mera bandera
            }
            b4int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return x5;
}


/*******************************************************************************
 * Function:        int aumentar(void)
 * Description:     Esta funcion controla la variable cambio, para justes de fecha y hora
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna camboi, el valor que va entre 1 y 3 dependiendo que se aumente.
 * Remarks:         None
 * ****************************************************************************/

int aumentar(void){
    if (b3int == 1){                        //Hubo alguna interrupcion?
            if(b3 == 0 & BOTON3 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b3 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera 
                   cambio++;
            }
            if(BOTON3 == 0){                    //cuando el boton ya lo apague
                b3 = 0;                         //apagar la mera mera bandera
            }
            b3int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return cambio;
}

/*******************************************************************************
 * Function:        int aumentar_uM(void)
 * Description:     Esta funcion aumenta el valor de unidades de minuto para el ajuste de la hora
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna uM, unidades de minuto.
 * Remarks:         None
 * ****************************************************************************/

int aumentar_uM(void){
    if (b1int == 1){                        //Hubo alguna interrupcion?
            if(b1 == 0 & BOTON1 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b1 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera 
                   uM++;
            }
            if(BOTON1 == 0){                    //cuando el boton ya lo apague
                b1 = 0;                         //apagar la mera mera bandera
            }
            b1int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return uM;
}

/*******************************************************************************
 * Function:        int aumentar_AuM(void)
 * Description:     Esta funcion aumenta el valor de unidades de minuto para el ajuste de la alarma
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna AuM, unidades de minuto para alarma.
 * Remarks:         None
 * ****************************************************************************/


int aumentar_AuM(void){
    if (b1int == 1){                        //Hubo alguna interrupcion?
            if(b1 == 0 & BOTON1 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b1 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera 
                   AuM++;
            }
            if(BOTON1 == 0){                    //cuando el boton ya lo apague
                b1 = 0;                         //apagar la mera mera bandera
            }
            b1int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return AuM;
}


/*******************************************************************************
 * Function:        int aumentar_uH(void)
 * Description:     Esta funcion aumenta el valor de unidades de hora para el ajuste de la hora
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna uH, unidades de hora.
 * Remarks:         None
 * ****************************************************************************/


int aumentar_uH(void){
    if (b1int == 1){                        //Hubo alguna interrupcion?
            if(b1 == 0 & BOTON1 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b1 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera 
                   uH++;
            }
            if(BOTON1 == 0){                    //cuando el boton ya lo apague
                b1 = 0;                         //apagar la mera mera bandera
            }
            b1int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return uH;
}

/*******************************************************************************
 * Function:        int aumentar_uD(void)
 * Description:     Esta funcion aumenta el valor de unidades de dia para el ajuste de la fecha
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna uD, unidades de dia.
 * Remarks:         None
 * ****************************************************************************/


int aumentar_uD(void){
    if (b1int == 1){                        //Hubo alguna interrupcion?
            if(b1 == 0 & BOTON1 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b1 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera 
                   uD++;
            }
            if(BOTON1 == 0){                    //cuando el boton ya lo apague
                b1 = 0;                         //apagar la mera mera bandera
            }
            b1int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return uD;
}

/*******************************************************************************
 * Function:        int aumentar_uMes(void)
 * Description:     Esta funcion aumenta el valor de unidades de mes para el ajuste de la fecha
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna uMes, unidades de mes.
 * Remarks:         None
 * ****************************************************************************/

int aumentar_uMes(void){
    if (b1int == 1){                        //Hubo alguna interrupcion?
            if(b1 == 0 & BOTON1 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b1 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera 
                   uMes++;
            }
            if(BOTON1 == 0){                    //cuando el boton ya lo apague
                b1 = 0;                         //apagar la mera mera bandera
            }
            b1int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return uMes;
}

/*******************************************************************************
 * Function:        int aumentar_uA(void)
 * Description:     Esta funcion aumenta el valor de unidades de a'o para el ajuste de la fecha
 * Precondition:    None
 * Parameters:      None
 * Return Values:   retorna uA, unidades de a'o.
 * Remarks:         None
 * ****************************************************************************/

int aumentar_uA(void){
    if (b1int == 1){                        //Hubo alguna interrupcion?
            if(b1 == 0 & BOTON1 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b1 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera 
                   uA++;
            }
            if(BOTON1 == 0){                    //cuando el boton ya lo apague
                b1 = 0;                         //apagar la mera mera bandera
            }
            b1int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return uA;
}

/*******************************************************************************
 * Function:        int activar_alarma(void)
 * Description:     Esta funcion controla una variable para encender o apagar la alarma
 * Precondition:    None
 * Parameters:      None
 * Return Values:   El valor de apagar.
 * Remarks:         None
 * ****************************************************************************/

int activar_alarma(void){
    if (b1int == 1){                        //Hubo alguna interrupcion?
            if(b1 == 0 & BOTON1 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b1 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera 
                   apagar++;
            }
            if(BOTON1 == 0){                    //cuando el boton ya lo apague
                b1 = 0;                         //apagar la mera mera bandera
            }
            b1int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return apagar;
}


/*******************************************************************************
 * Function:        int control_luz(void)
 * Description:     Esta funcion controla una variable para encender o apagar la luz
 * Precondition:    None
 * Parameters:      None
 * Return Values:   El valor de luz.
 * Remarks:         None
 * ****************************************************************************/

int control_luz(void){
    if (b3int == 1){                        //Hubo alguna interrupcion?
            if(b3 == 0 & BOTON3 == 1){          //el boton esta prendido y no estaba marcado como si ya estaba prendido
                   b3 = 1;                         //marcar que si fijo se prendio encendiendo la mera mera bandera 
                   luz++;
            }
            if(BOTON3 == 0){                    //cuando el boton ya lo apague
                b3 = 0;                         //apagar la mera mera bandera
            }
            b3int = 0;                          //Apagamos la bandera de la interrupcion
        }
            return luz;
}

/*******************************************************************************
 * Function:        int menu_ajustes(void)
 * Description:     Desplega el menu de ajustes y sus funciones
 * Precondition:    None
 * Parameters:      None
 * Return Values:   El valor de ctrl2.
 * Remarks:         None
 * ****************************************************************************/

int menu_Ajuste(){
    
    hora();
    fecha();
EYRLCD_WriteString("Alarma", 18, 8); //muestra hora en pantalla    
EYRLCD_WriteString("Hora", 18, 10); //muestra hora en pantalla
EYRLCD_WriteString("Fecha", 18, 12); //muestra hora en pantalla
EYRLCD_WriteString("->", 4, pos); //muestra hora en pantalla

if (chequeo_Salirmenu2() == 0){
    pos = 10;
    ctrl2 = 0;
}
else ctrl2 = 1;

chequeo_CambioPos();
    if (pos == 14){
        pos = 8;
    EYRLCD_Clear();
    }

if ((chequeo_Seleccion2() == 1) && (pos == 8)){
    ctrl3 = 1;
    EYRLCD_Clear();
    cambio = 1;
    while (ctrl3 == 1){
        ctrl3 = setear_alarma();
    }
    EYRLCD_Clear();
}


if ((chequeo_Seleccion2() == 1) && (pos == 10)){
    ctrl3 = 1;
    EYRLCD_Clear();
    cambio = 1;
    while (ctrl3 == 1){
        ctrl3 = incrementar_hora();
    }
    EYRLCD_Clear();
}

if ((chequeo_Seleccion2() == 1) && (pos == 12)){
    ctrl4 = 1;
    EYRLCD_Clear();
    cambio = 1;
    while (ctrl4 == 1){
        
        ctrl4 = incrementar_fecha();
    }
    EYRLCD_Clear();
}


return ctrl2;
}


/*******************************************************************************
 * Function:        int incrementar_hora(void)
 * Description:     Incrementa la hora al entrar al menu ajustes
 * Precondition:    None
 * Parameters:      None
 * Return Values:   ctrl3 para salir del menu.
 * Remarks:         None
 * ****************************************************************************/

int incrementar_hora() {
    
aumentar();

if (cambio == 3){
    cambio = 1;
}

if (cambio == 1){
    aumentar_uM();
    
    if (uM == 10){
         uS = 0;
         dS = 0;
         uM = 0;
         dM++;
     }
    
     if (dM == 6){
         uS = 0;
         dS = 0;
         uM = 1;
         dM = 0;
     }
}

else if (cambio == 2){
 aumentar_uH();
    
     if (uH == 10){
         uS = 0;
         dS = 0;
         uH = 0;
         dH++;
     } 
     
     if ((dH == 1) && (uH == 3)){
         uS = 0;
         dS = 0;
         uH = 1;
         dH = 0; 
     }   
}

if (cambio == 1){
        EYRLCD_WriteString("Ajuste M",9, 8); //muestra hora en pantalla
}
else if (cambio == 2){
    EYRLCD_WriteString("Ajuste H", 9, 8); //muestra hora en pantalla
}

    
    sprintf(str3, "%d%d:%d%d-%d", dH, uH, dM,uM, cambio); // mueve el valor de las variables al array para imprimir.
    EYRLCD_WriteString(str3, 13, 10); //muestra hora en pantalla
    
if (chequeo_Salirmenu3() == 0){
    ctrl3 = 0;
}

else ctrl3 = 1;
    return ctrl3;
   
}

/*******************************************************************************
 * Function:        int incrementar_fecha(void)
 * Description:     Incrementa la fecha al entrar al menu ajustes
 * Precondition:    None
 * Parameters:      None
 * Return Values:   ctrl4 para salir del menu.
 * Remarks:         None
 * ****************************************************************************/

int incrementar_fecha() {
    hora();
    
aumentar();
    if (cambio == 4){
        cambio = 1;
    }
    
    
if (cambio == 1){ 
    
    aumentar_uD();
    if (uD == 10){
      dD++;
      uD = 0;
  }
    
    if (dD == 4 && uD == 1){
        uD = 1;
        dD = 0;
    }
}
    
if (cambio == 2){
    aumentar_uMes();
        if (dMes == 0 && uMes == 10){
            uMes = 0;
            dMes++;
        }
        
        if (dMes == 1 && uMes == 3){
            dMes = 0;
            uMes = 1;
        }
    }

if (cambio == 3){
    aumentar_uA();
        if (uA == 10){
            uA = 0;
            dA++;
        }
    }

if (cambio == 1){
        EYRLCD_WriteString("Ajuste D",9, 8); //muestra hora en pantalla
}
else if (cambio == 2){
    EYRLCD_WriteString("Ajuste M", 9, 8); //muestra hora en pantalla
}
else if (cambio == 3){
    EYRLCD_WriteString("Ajuste A", 9, 8); //muestra hora en pantalla
}

    
    
    sprintf(str2, "%d%d/%d%d/%d%d-%d", dD, uD, dMes,uMes, dA, uA,cambio); // mueve el valor de las variables al array para imprimir.
    EYRLCD_WriteString(str2, 11, 10); //muestra hora en pantalla
    
if (chequeo_Salirmenu3() == 0){
    ctrl4 = 0;
}

else ctrl4 = 1;
    
    return ctrl4;
   
}


/*******************************************************************************
 * Function:        int setear_alarma(void)
 * Description:     Incrementa las variables de la alarma al entrar al menu ajustes
 * Precondition:    None
 * Parameters:      None
 * Return Values:   ctrl3 para salir del menu.
 * Remarks:         None
 * ****************************************************************************/

int setear_alarma(){
    
aumentar();

if (cambio == 3){
    cambio = 1;
}

if (cambio == 1){
    
    hora();
    fecha();
    
    aumentar_AuM();
    
    if (AuM == 10){
         AuM = 0;
         AdM++;
     }
    
     if (AdM == 6){
         AuM = 1;
         AdM = 0;
     }
}

else if (cambio == 2){
 aumentar_uH();
    
     if (AuH == 10){
         uH = 0;
         dH++;
     } 
     
     if ((AdH == 1) && (AuH == 3)){
         AuH = 1;
         AdH = 0; 
     }   
}

if (cambio == 1){
        EYRLCD_WriteString("Ajuste M",9, 8); //muestra hora en pantalla
}
else if (cambio == 2){
    EYRLCD_WriteString("Ajuste H", 9, 8); //muestra hora en pantalla
}

    
    sprintf(str3, "%d%d:%d%d-%d", AdH, AuH, AdM,AuM, cambio); // mueve el valor de las variables al array para imprimir.
    EYRLCD_WriteString(str3, 13, 10); //muestra hora en pantalla
    
if (chequeo_Salirmenu3() == 0){
    ctrl3 = 0;
}

else ctrl3 = 1;
    return ctrl3;   
    
}


