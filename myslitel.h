/*********************************************************/
/* myslitel.h - rekurzivni propocet (alfabeta)           */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#ifndef myslitelH
#define myslitelH
#include "volby.h"
#ifdef Vypisy
#include <stdio.h>
#endif
#include "cotyvar.h"
cfunkce void UlozStav(TUloha *uloha);
/***************************************************************************
*   UlozStav - ulozi stav mimochodem a rosad na zasobnik do pos            *
 *   s pos nic nedela                                                      *
 ***************************************************************************/
cfunkce void SectiMaterialBmCm(TUloha *uloha);
/*************************************************/
/* Nastavi podle aktualni pozice polozky		 */
/* uloha->zasobnik.bm[0], uloha->zasobnik.cm[0]  */
/* a uloha->material							 */
/*************************************************/
cfunkce void SectiJenMaterial(TUloha *uloha);
/*********************************************************/
/* Nastavi podle aktualni pozice polozky uloha->material */
/*********************************************************/
cfunkce void InitVypocet(TUloha *uloha);
/**********************************************************/
/* Inicializace pred zahajenim rekurzivniho propoctu      */
/* vola se z DejTah										  */
/**********************************************************/


void KopirujHVar(TUloha *uloha, u16 t);

s16 AlfaBeta(TUloha *uloha, s16 alfa, s16 beta, s16 hloubka
#ifdef HTML_VYPISY
 , char *varianta
#endif
);

#define PISDOHISTHEUR(u,t,x)\
{if((u)->HistHeur[(u16)(t)]+x<=255)\
(u)->HistHeur[(u16)(t)]+=(MIN((x),10));\
else {(u)->HistHeur[(u16)(t)]=255;PulHistHeur(u);}}
/*Zapis do tabulek historicke heuristiky v uloze u
nejlepsi tah t na pozici
prida x tak, aby nepreteklo*/

void DotazNaCas(TUloha *u);


/**
  Vrati vahu provedenych operaci od zacatku vypoctu.
*/
cfunkce int Stav(const TUloha *u);

void PulHistHeur(TUloha *u);

/**********************************************************/
/* Jednoduchy (ne zcela korektni) test, zda je uz remiza  */
/* nedostatkem materialu, aby se dal nepocitalo           */
/**********************************************************/
int MaterialRemis(TUloha *u);
 
#define NASTAVOKRAJE(p1, p2) \
{\
 (p1) = uloha->zasobnik.tahy + uloha->zasobnik.hranice[zanor];\
 (p2) = uloha->zasobnik.tahy + uloha->zasobnik.hranice[zanor+1];\
}


/*Prevod hodnot pozice. Normalni hodnoty necha byt,
ale ty matici (treba mat 5. pultahem) posune. Pri volani
alfabety se totiz take posouvam po stromu.*/
#define BlizKMatu(X) \
((s16)(abs((int)(X))<SebranyKral ? (X) : (((X)<0) ? ((X)-1) : ((X)+1) )))
#define DalOdMatu(X) \
((s16)(abs((int)(X))<SebranyKral ? (X) : (((X)<0) ? ((X)+1) : ((X)-1) )))

/* Konstanta urcujici hranici pro hodnotu pozice:
     abs(hodnota)>SebranyKral    =>  nekdo nema krale
     else oba maji krale
   Zavisi na ohodnoceni figur  */
#define SebranyKral 10000

#endif

