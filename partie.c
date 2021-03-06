﻿/*********************************************************/
/* partie.c - prace s partii - seznamem tahu             */
/* 6.1. 2000 Jan Nemec                                   */
/*********************************************************/
#include <stdlib.h>
#include "cotyvar.h"
#include "chyba.h"
#include "hashtab.h"
#include "partie.h"

/*******************************/
/* PocetPultahu (vlevo+vpravo) */
/*******************************/
int PocetPultahu(TPartie* prt) {
    int i;

    while(prt->l!=NULL)prt=prt->l;
    i=0;
    while(prt->p!=NULL){prt=prt->p;i++;}
    return i;
}

/*******************************/
/* PocetPultahu (vlevo)        */
/*******************************/
int VlevoPocetPultahu(TPartie* prt) {
    int i;
    TPartie *p;

    p=prt->l;i=0;
    while(p!=NULL){p=p->l;i++;}
    return i;
}

/*****************************************/
/* MazejVlevo                            */
/* Maze pomoci free spojak doleva vcetne */
/*****************************************/
static void MazejVlevo(TPartie** prt)
{TPartie *p,*p2;

 if (prt && *prt) 
   {p=(*prt)->l;
   while (p)
    {p2=p->l;
    free(p);
    p=p2;}
   free(*prt);
   *prt=NULL;  }}

/*****************************************/
/* MazejVpravo                           */
/* Maze pomoci free spojak doprava vcetne*/
/*****************************************/
static void MazejVpravo(TPartie** prt)
{TPartie *p,*p2;

 if (prt && *prt)
  {p=(*prt)->p;
   while (p){
   p2=p->p;
    free(p);
    p=p2;} 
  free(*prt); 
  *prt=NULL;  }}

/*******************************/
/* Smaze spojak a da tam NULL  */
/*******************************/
void DonePartie(TPartie** prt) {
#if (LadeniPartie>10)
if (!prt)
 Chyba("partie.c - DonePartie");
#endif
if (*prt) {
MazejVlevo(&((*prt)->l));  MazejVpravo(prt);}}

/* InitPartie smaze partii (neni-li NULL) a vytvori novou prazdnou*/
void InitPartie(TPartie **prt, TPozice *pos) {
  DonePartie(prt); 
 *prt = (TPartie *) malloc(sizeof(TPartie));
  (*prt)->l = NULL;
  (*prt)->p = NULL;
  (*prt)->data.hashF = HashF(pos);
    (*prt)->data.zmena = 0;
}

/*PridejTah umaze ocas vpravo, prida tah a presune se na nej*/
void PridejTah(TPartie **prt, TPartData data){
#if (LadeniPartie>10)
 if (!prt || !*prt) Chyba("partie.c - PridejTah");
#endif 
 MazejVpravo(&((*prt)->p));
 (*prt)->p = (TPartie *) malloc(sizeof(TPartie)); 
(*prt)->p->p=NULL;
 (*prt)->p->l=*prt; 
 *prt=(*prt)->p;
 (*prt)->data=data;
}
