/*********************************************************/
/* dosio.h - consolovy vstup a vystup                    */
/* 11.1. 2001 Jan Nemec                                  */
/*********************************************************/
#ifndef dosioH
#define dosioH
#include "volby.h"
#include <stdio.h>
#include "cotyvar.h"

/*********************************************************/
/* Test rychlosti generatoru tahu                        */
/*********************************************************/
cfunkce void TestujGeneratorCas(TUloha* uloha);

/*********************************************************/
/* Test detekce sachu                                    */
/*********************************************************/
cfunkce void TiskniSachy(TUloha* uloha);

cfunkce void TiskniSachovnici(TPozice* pos, FILE* f);

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

/*********************************************************/
/* Volani mysliciho algoritmu a IO                       */
/*********************************************************/
cfunkce void TahPocitace(TUloha * uloha);

/*********************************************************/
/* Rezim zadavani a plneni prikazu                       */
/*********************************************************/
cfunkce void HlavniDosCyklus(TUloha * uloha);
#endif
