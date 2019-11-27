#ifndef dejtahH
#define dejtahH

#include "cotyvar.h"

extern int ChciKnihovnu; 

/*Sem pri inicializaci nastavim, zda jsem uspesne nalezl knihovnu zahajeni*/
extern int MamKnihovnu;

cfunkce void GetBookMoves(TUloha* uloha);

/**************************************************************************/
/*  Dejtah - Myslici algoritmus (jeho volani)                             */
/* cas je v ms                                                            */
/* PrijmiRemis muze byt NULL. Jinak tam pha da, zda by prijal remizu      */
/* VzdejSe muze by NULL. Jinak, zda se pha vzdava                         */
/* vraci 1, pokud existuje tah, jinak 0                                   */ 
/**************************************************************************/
cfunkce int DejTah(TUloha *uloha, TTah1 *t1, u32 cas, int maxHloubka, int *PrijmiRemis, int *VzdejSe);

#endif
