/*********************************************************/
/* lokruti.h - Test sachu ohrozeni a lokalni             */
/* (tj. nikoliv v partii) tahy                           */
/* 6.1. 2001 Jan Nemec                                   */
/*********************************************************/
#ifndef lokrutiH
#define lokrutiH
#include "cotyvar.h"
cfunkce int Ohrozeno(const s8 *p, int bilym);
/***************************************************/
/**  Ohrozeno   Je dane policko ohrozene ?        **/
/***************************************************/
cfunkce int Sach(int komu, s8 *sch);
/*************************************************************************/
/*   Sach je komu v sachu ?                                              */
/*   sch - pointr na 1. policko sachovnice 12*10 (tedy nikoliv a1)       */
/*************************************************************************/
cfunkce int HrozbaBilemuKraliDamou(TUloha *u);
cfunkce int HrozbaCernemuKraliDamou(TUloha *u);
cfunkce void JednoTahni(u16 tah, TUloha *uloha);
/***************************************************************************
 *   JednoTahni - Tahne v testu pripustnosti tahu                          *
 *   Na zasobnik ulozi jen udaje pro TahniZpet a hodpos,
 *   nic nepocita, ani hash funkce												       *
 ***************************************************************************/
cfunkce void JednoTahniZpet(u16 tah, TUloha *uloha);
/***************************************************************************/
/* Opacna funkce k JednoTahni                                              */
/***************************************************************************/
cfunkce void Tahni(u16 tah, TUloha *uloha);
/***************************************************************************
 *   Tahni - Tahne v propoctu (Nikoliv v partii)                           *
 *   Ulozi na zasobnik sebranou figuru a potom zvedne pos o 1              *
 ***************************************************************************/
cfunkce void TahniZpet(u16 tah, TUloha *uloha);
/*************************************************************************
 *   TahniZpet - Tahne zpet v propoctu (Nikoliv v partii)                *
 *    V zasobniku nejprve snizi pos o 1 a pak cte z urovne pos           *
**************************************************************************/
#endif

