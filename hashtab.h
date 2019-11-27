/**********************************************************/
/* hashtab.h - hash funkce pro tabulku ohodnocenych pozic */
/* 31.12. 2000 Jan Nemec                                  */
/**********************************************************/
#ifndef hashtabH
#define hashtabH
#include "volby.h"
#include "cotyvar.h"

/* THashNahoda - obsahuje nahodna cisla pro vsechny dvojice
(typ kamene, pole), dale pro stav mimochodem, rosad a kdo
je na tahu*/
typedef struct{
    u32 f[13][99];
/* [od cerneho krale pres prazdne pole az po bileho krale][pole]*/
    u32 bily;
    u32 roch[16];
    u32 mimoch[69];
}THashNahoda;

/* Sada nahodnych cisel pro dve funkce */
extern THashNahoda hnF;

/* Sada nahodnych cisel pro dve funkce */
extern THashNahoda hnG; 

/*********************************************************/
/* HashInit naplni koeficienty obou hashovacich funkci   */
/* pseudonahodnymi cisly                                 */
/* Vola se JEDNOU na zacatku celeho PROGRAMU             */
/*********************************************************/
cfunkce void HashInit(void);

/**********************************************************************/
/* HashF a HashG                                                      */
/* Hashovaci funkce, vrati hash kod pozice. Nevola se v rekursivnim   */
/* propoctu, ale jen jednou na zacatku propoctu. V rekursi se pocita  */
/* "inkementalne" postupnym xorovanim tahu.                           */
/* F urcuje pozici v tabulce, G cislo pro kontrolu                    */
/**********************************************************************/
cfunkce u32 HashF(TPozice *pos);
cfunkce u32 HashG(TPozice *pos);


/**************************************************/
/* Je pozice v tabulce ? A jak je ohodnocena ?    */
/**************************************************/
cfunkce int GetHash(TUloha *uloha, s16 alfa, s16 beta, u8 hloubka, s16 *cena);

/*******************************/
/* Ulozi pozici do tabulky     */
/*******************************/
cfunkce void SetHash(TUloha *uloha, u8 hloubka, u8 priznak, s16 cena);

/*******************************************/
/* Je nejlepsi tah z pozice v tabulce ?    */
/*******************************************/
cfunkce int GetHashNejlepsi(TUloha *uloha, u16 *tah, u8 *hloubka, u8 *priznak);

/*************************************/
/* Ulozi nejlepsi tah do tabulky     */
/*************************************/
cfunkce void SetHashNejlepsi(TUloha *uloha, u16 tah, u8 hloubka, u8 priznak);

/*************************************************/
/* HashPechF a HashPechF                         */
/* 2 hash funkce struktury pescu                 */
/* (v rekurzi nevolat, pocita se inkrementalne)  */
/*************************************************/
cfunkce u32 HashPechF(TPozice *pos);
cfunkce u32 HashPechG(TPozice *pos);

/********************************************************/
/* Je struktura pescu v tabulce ? A jak je ohodnocena ? */
/********************************************************/
cfunkce int GetPechHash(TUloha *uloha, THashPech **hp);

/****************************************/
/* Ulozi strukturu pescu do tabulky     */
/****************************************/
cfunkce void SetPechHash(TUloha *uloha, THashPech *hp);
#endif
