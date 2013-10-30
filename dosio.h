/*********************************************************/
/* dosio.h - consolovy vstup a vystup                    */
/* 11.1. 2001 Jan Nemec                                  */
/*********************************************************/
#ifndef dosioH
#define dosioH
#include "volby.h"
#include <stdio.h>
#include "cotyvar.h"

cfunkce void TestujGeneratorCas(TUloha *uloha);
/*********************************************************/
/* Test rychlosti generatoru tahu                        */
/*********************************************************/
cfunkce void TiskniSachy(TUloha *uloha);
/*********************************************************/
/* Test detekce sachu                                    */
/*********************************************************/

/**
 Vytiskne aktualni pozici do HTML.
*/
cfunkce void HTMLTiskniSachovnici(TPozice *pos, FILE *f);

/**
 Otevre textovy vystupni soubor a vytiskne tam HTML hlavicku.
*/
cfunkce FILE *otevriHtmlLog(TUloha *u, char *varianta);

/**
 Vytiskne HTML konec a zavre soubor.
*/
cfunkce void zavriHtmlLog(FILE *f);

cfunkce void TahPocitace(TUloha *uloha);
/*********************************************************/
/* Volani mysliciho algoritmu a IO                       */
/*********************************************************/
cfunkce void HlavniDosCyklus(TUloha *uloha);
/*********************************************************/
/* Rezim zadavani a plneni prikazu                       */
/*********************************************************/
#endif
