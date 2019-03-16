/*******************************************************************************
 *
 *                  Libreria Funciones para el Reloj
 *
 *******************************************************************************
 * FileName:        Funciones_Reloj.h
 * Processor:       PICxxxxxx
 * Complier:        XC8 v1.30
 * Author:          Jose Pablo Guerra
 * Description:     Esta libreria contiene las distintas funciones para controlar
 *                  al reloj, especialmente los antirrebotes que evitan que entre y 
 *                  salga del menu, o que incremente las variables sin detenerse
 *******************************************************************************
 * Rev.         Date            Comment
 *   v0.0.0      05/10/2018      Creación del firmware
 *   v0.1.0      13/10/2018      Funciones completas del reloj agregadas totalmente
 ******************************************************************************/

void hora(void);
int menu(void);
void cronometro2(void);
void fecha(void);
int chequeo_menu(void);
int chequeo_mostrar(void);
int chequeo_Salirmenu(void);
int chequeo_Salirmenu2(void);
int chequeo_Salirmenu3(void);
int chequeo_Salirmenu4(void);
int chequeo_CambioPos(void);
int chequeo_Seleccion(void);
int chequeo_Seleccion2(void);
int aumentar(void);
int aumentar_uM(void);
int aumentar_AuM(void);
int aumentar_uH(void);
int aumentar_uD(void);
int aumentar_uMes(void);
int aumentar_uA(void);
int activar_alarma(void);
int control_luz(void);
int chequeo_otros(void);
int menu_ajustes(void);
int incrementar_hora(void);
int incrementar_fecha(void);
int setear_alarma(void);


