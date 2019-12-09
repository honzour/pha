/*********************************************************/
/* soubvv.h - IO partie do formatu HS 1350               */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#include "volby.h"
#include "cotyvar.h"
cfunkce int UlozDoSouboru(char *jmeno,TUloha *u);
/******************************************************/
/* Ulozeni aktualni partie do souboru formatu HS 1350 */
/******************************************************/
cfunkce int OtevriZeSouboru(char *jmeno,TUloha *u);
/*******************************************************/
/* Otevreni aktualni partie ze souboru formatu HS 1350 */
/*******************************************************/
cfunkce int OtevriFENZeSouboru(char *jmeno,TUloha *u);
/*******************************************************/
/* Otevreni pozice ze souboru formatu FEN              */
/*******************************************************/
