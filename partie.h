/*********************************************************/
/* partie.h - prace s partii - seznamem tahu             */
/* 1.6. 2000 Jan Nemec                                   */
/*********************************************************/
#ifndef partieH
#define partieH
#include "volby.h"
#include "cotyvar.h"
#define LadeniPartie 100
cfunkce void DonePartie(TPartie **prt);
/*******************************/
/* Smaze spojak a da tam NULL  */
/*******************************/
cfunkce void InitPartie(TPartie **prt, TPozice *pos);
/*******************************/
/* Udela novou partii          */
/*******************************/
cfunkce int PocetPultahu(TPartie *prt);
/*******************************/
/* PocetPultahu (vlevo+vpravo) */
/*******************************/
cfunkce int VlevoPocetPultahu(TPartie *prt);
/*******************************/
/* PocetPultahu (vlevo)        */
/*******************************/
cfunkce void PridejTah(TPartie **prt, TPartData data);
/***********************/
/* Novy tah do partie  */
/***********************/
#endif
