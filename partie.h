/*********************************************************/
/* partie.h - prace s partii - seznamem tahu             */
/* 1.6. 2000 Jan Nemec                                   */
/*********************************************************/
#ifndef partieH
#define partieH
#include "volby.h"
#include "cotyvar.h"
#define LadeniPartie 100

/*******************************/
/* Smaze spojak a da tam NULL  */
/*******************************/
cfunkce void DonePartie(TPartie **prt);

/*******************************/
/* Udela novou partii          */
/*******************************/
cfunkce void InitPartie(TPartie **prt, TPozice *pos);

/*******************************/
/* PocetPultahu (vlevo+vpravo) */
/*******************************/
cfunkce int PocetPultahu(TPartie *prt);

/*******************************/
/* PocetPultahu (vlevo)        */
/*******************************/
cfunkce int VlevoPocetPultahu(TPartie *prt);

/***********************/
/* Novy tah do partie  */
/***********************/
cfunkce void PridejTah(TPartie **prt, TPartData data);
#endif
