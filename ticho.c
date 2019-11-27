#include <stdlib.h>
#include "volby.h"
#include "cotyvar.h"
#include "myslitel.h"
#include "ticho.h"
#include "chyba.h"
#include "hashtab.h"
#include "ohodnoc.h"
#include "lokruti.h"
#include "generato.h"
#if Typ_Produktu==DLL
#include "interface.h"
#endif

/******************************************************************************/
/* AlfaBetaBrani do dane hloubky.Vraci hodnotu z hlediska toho,kdo je na tahu.*/
/* Hleda hodnotu na otevrenem intervalu(alfa,beta),vraci cislo z uzavreneho   */
/* intervalu <alfa,beta>                                                      */
/******************************************************************************/
s16 AlfaBetaBrani(TUloha *uloha, s16 alfa, s16 beta, s16 hloubka) {
  TTah1 *pt1, *pt2, *pt0;
  s16 BezPropoctu, pomhod;
  int zanor;

#ifdef Statistika
  uloha->stat.ABB[hloubka]++;
  uloha->stat.UZ++;
#endif

#if Typ_Produktu==DLL
  Callbacks.NextNode();
  if (uloha->StavPropoctu.MamKoncitMysleni) {
      return ZadnaCena;
  }
#endif
  if (GetHash(uloha, alfa, beta, 1, &BezPropoctu))
    return BezPropoctu;
  zanor = uloha->zasobnik.pos;
  int selDepth = zanor + uloha->StavPropoctu.NulTah;
  if (selDepth > uloha->StavPropoctu.SelDepth)
  {
      uloha->StavPropoctu.SelDepth = selDepth;
#if Typ_Produktu==DLL
      Callbacks.SetSelDepth(selDepth);
#endif
  }
  if (zanor>MaxHloubkaPropoctu - 4)
    return HodnotaPozice(uloha, alfa, beta);
  hassert(uloha->zasobnik.pos < MaxHloubkaPropoctu, "Hloubka propoctu");

  /* Neni-li matici material, je remiza. Nema to smysl testovat
     v uvodnim vyvolani, proto hloubka <  MaxHloubkaABB */
  /* TODO promyslet */
  if (hloubka < uloha->AlgCfg.AlgKoef.MaxHloubkaABB && MaterialRemis(uloha))
    return 0;

  /*List*/
  if (!hloubka)
    return HodnotaPozice(uloha,alfa,beta);

  uloha->zasobnik.sach[zanor] = (uloha->pozice.bily ?
    Ohrozeno(uloha->pozice.sch + uloha->material.bk, 0):
    Ohrozeno(uloha->pozice.sch + uloha->material.ck, 1));

  /*Ulozim si, jsem-li v sachu*/
  if (uloha->zasobnik.sach[zanor]) {
    NalezTahyZeSachu(uloha);/*Vsechny legalni tahy*/
    NASTAVOKRAJE(pt1,pt2)
    if (pt1 == pt2) return -mat;
    BezPropoctu = alfa; /*Jsem-li v sachu, nesmim ukoncit propocet*/
    /* Ale presto musim spocitat hodnotu pozice,
       abych ji v synovi mohl pouzit pri linem
       vyhodnocovani.*/
    pomhod = HodnotaPozice(uloha,alfa,beta);
  }
  else {
    /*Nejsem v sachu*/
    pomhod = BezPropoctu = HodnotaPozice(uloha,alfa,beta);
    /* a dohledani do ticha mi muze pozici jen zlepsit.*/
    if (BezPropoctu >= beta || MaxHloubkaPropoctu < zanor + 4)
      return BezPropoctu;
    NalezBrani(uloha);
    /*Vsechny pseudolegalni tahy, ktere nejsou tiche.*/
    NASTAVOKRAJE(pt1,pt2)
    if (pt1 == pt2)return BezPropoctu;
    /*Zadny netichy tah*/
    if (alfa < BezPropoctu) alfa = BezPropoctu;
    /*Dohledani pozici nezhorsi.*/
  }
  pt0 = pt1;
  for(; pt1 < pt2; pt1++) {
    pt1->cena += uloha->HistHeur[pt1->data];
    if (((pt1->data) & 127) == uloha->zasobnik.kam[zanor - 1])
      pt1->cena += 50;
  }
  pt1 = pt0;
  SetridTahy(uloha);
  UlozStav(uloha);
  for(; pt1 < pt2; pt1++) {
    if (!uloha->zasobnik.sach[zanor] &&
      pt1->cena + BezPropoctu < alfa) continue;
    Tahni(pt1->data, uloha);
    if ((uloha->pozice.bily ?
      Ohrozeno(uloha->pozice.sch + uloha->material.ck, 1):
      Ohrozeno(uloha->pozice.sch + uloha->material.bk, 0))){
      TahniZpet(pt1->data, uloha);
      continue;
    }
    pt1->cena = 
      (s16) -AlfaBetaBrani(
               uloha,
               BlizKMatu(-beta),
               BlizKMatu(-alfa),
               (s16)(hloubka - 1));
    pt1->cena = DalOdMatu(pt1->cena);
    if (pt1->cena>alfa)
      alfa = pt1->cena;
    TahniZpet(pt1->data,uloha);
    if (uloha->StavPropoctu.MamKoncitMysleni) {
        return ZadnaCena;
    }
    if(pt1->cena>=beta) return beta; /*prelezeni okenka */
  }
  return alfa;
}
