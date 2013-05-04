#ifndef dejtahH
#define dejtahH

/*Sem pri inicializaci nastavim, zda jsem uspesne nalezl
knihovnu zahajeni*/

extern int MamKnihovnu;

/**************************************************************************/
/*  Dejtah - Myslici algoritmus (jeho volani)
/* cas je v ms
/* PrijmiRemis muze byt NULL. Jinak tam pha da, zda by prijal remizu      */
/* VzdejSe muze by NULL. Jinak, zda se pha vzdava			  */
/***************************************************************************/
cfunkce int DejTah(TUloha *uloha, TTah1 *t1, u32 cas, int maxHloubka, int *PrijmiRemis,	int *VzdejSe);

#endif
