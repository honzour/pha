/*********************************************************/
/* ohodnoc.h - ohodnocovaci funkce                       */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#ifndef ohodnocH
#define ohodnocH
#include "volby.h"
#include "cotyvar.h"

/*****************************************/
/* Vrati hodnotu pozice mezi alfa a beta */
/*****************************************/
cfunkce s16 HodnotaPozice(TUloha *u,s16 alfa,s16 beta);

/*************************************/
/* nastavi TAlgCfg na bezne hodnoty  */
/*************************************/
cfunkce void NastavAlgCfgStd(TAlgCfg *a);

/*************************************/
/* zasumi TAlgCfg (max zmena = sum)  */
/*************************************/
cfunkce void ZasumAlgCfg(TAlgCfg *a, int sum);

/***********************************************************/
/* Vrati nahodne cele cislo z intervalu (-maxabs,maxabs)   */
/* z rozdeleni, kde hustota linearne roste od -maxabs,     */
/* kde je 0 k 0, kde je nejvyssi a pak linearne klesa       */
/* k maxabs, kde je nula. Jde o jakousi velmi spatnou       */
/* aproximaci binomickeho rozdeleni.                       */
/***********************************************************/
cfunkce int Sum(int maxabs);

extern s8 BelostPole[99];
#endif
