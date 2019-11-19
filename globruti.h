/*********************************************************/
/* globruti.h - globalni sachove rutiny                  */
/* napr. zahrani a vraceni tahu v partii                 */ 
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#ifndef globrutiH
#define globrutiH
#include "volby.h"
#include "cotyvar.h"

/***********************************************************************/
/*  GlobTahni - Tah v partii (nikoliv v propoctu)                      */
/*  na zasobnik ani nesahne                                            */
/*  je-li useknout true, je dalsim parametrem u16 - ktery ze se to     */
/*  ma zahrat tah - a tim se taky preplacne pripadny ocas partie       */
/*  je-li useknout false, nejsou zadne dalsi parametry, jde o          */
/*  znovuzahrani vraceneho tahu - ten se nacte z partie, ta se posune  */
/*  o 1 doprava, ocas se nemaze                                        */
/*  Je-li TestKonnce true, otestuje se, neskončila-li partie.          */
/***********************************************************************/
cfunkce void GlobTahni(int TestKonce, TUloha *uloha, int useknout, ...);

/***************************************************************************/
/*   GlobTahniZpet - Tahne zpet v partii (Nikoliv v propoctu)              */
/*   Posune se v partii o 1 doleva, ocas nemaze, na zasobnik ani nesahne   */
/***************************************************************************/
cfunkce void GlobTahniZpet(TUloha *uloha);

/*************************************************************************/
/*  KorektniTah - je to pripustny tah ?                                  */
/*  Zvedne uroven zasobniku, nalezne tahy, vyskrta nepripustne,          */
/*  pokusi se tam ten tah najit a nakonec snizi uroven zasobniku zpet.   */
/*************************************************************************/
cfunkce int KorektniTah(u16 tah, TUloha* uloha);

/*************************************************************************/
/*  KTNehledej -  Analogie KorektiTah, jen už vše musí být na zásobníku. */
/*  Ze zásobníku pouze čte.                                                 */
/*************************************************************************/
cfunkce int KTNehledej(u16 tah, TUloha* uloha);

/*************************************************************************
 *  JenPripustne - Vynda z prvni dle pos volne (ale v praxi obsazene)    *
 *  urovne zasobniku nepripustne tahy.                                   *
 *  tj. ty vedouci k sachu. Pomerne pomale, neuzivat v rekurzi propoctu. *
 *  Nastavi pocatek nove prazdne urovne (snizi nebo necha)               *
 *************************************************************************/
cfunkce void JenPripustne(TUloha* uloha);

/****************************************************/
/* Je mezi nalezenymi tahi nejaky vedouci z odkud ? */
/****************************************************/
cfunkce int JeTam1(u8 odkud, TUloha* uloha);

/***********************************************************/
/* Je mezi nalezenymi tahi nejaky vedouci z odkud do kam ? */
/***********************************************************/
cfunkce int JeTam2(u8 odkud, u8 kam, TUloha* uloha);

#if Typ_Produktu==WindowsProgram
/***************************************************/
/* Jen windows - pripadny dialog promeny pesce     */
/***************************************************/ 
cfunkce u16 DoplnTah(HWND hwnd, u8 odkud, u8 kam, TUloha* uloha);
#endif

#if Typ_Produktu==Qt_Program
cfunkce u16 DoplnTah(u8 odkud,u8 kam,TUloha *uloha);
#endif

/************************************************************************
*  Je remíza 50 tahy nebo 3x stejná pozice ?                            *
*  vrací 50, 3 nebo 0 (význam je jasný)                                 *
************************************************************************/
cfunkce int Remiza50Nebo3(TUloha* uloha);

#define BilyVMatu 1

#define CernyVMatu 2

#define Remis3 3

#define Remis50 4

#define RemisBilyVPatu 5

#define RemisCernyVPatu 6

#define RemisMaloMaterialu 7

/************************************************************************
 * Jak dopadla partie ? Nastaví správně uloha->KonecPartie              *
 ************************************************************************/
cfunkce void ZjistiTypKoncePartie(TUloha* uloha);
#if Typ_Produktu==WindowsProgram
extern char NazvyVysledku[8][38];
#endif
#endif
