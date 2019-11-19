/*********************************************************/
/* ohodnoc.c - ohodnocovaci funkce                       */
/* 13.1. 2001 Jan Nemec                                  */
/*********************************************************/
#include "volby.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#if Typ_Produktu==DOS_UNIX
#include "dosio.h"
#endif
#include "generato.h"
#include "hashtab.h"
#include "hashpech.h"
#include "cotyvar.h"
#include "myslitel.h"
#include "ohodnoc.h"
#include "pole.h"

u8 XPole[h8+1]=

{  100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100,   0,   1,   2,   3,   4,   5,   6,   7, 100, /* 1*/
   100,   0,   1,   2,   3,   4,   5,   6,   7, 100, /* 2*/
   100,   0,   1,   2,   3,   4,   5,   6,   7, 100, /* 3*/
   100,   0,   1,   2,   3,   4,   5,   6,   7, 100, /* 4*/
   100,   0,   1,   2,   3,   4,   5,   6,   7, 100, /* 5*/
   100,   0,   1,   2,   3,   4,   5,   6,   7, 100, /* 6*/
   100,   0,   1,   2,   3,   4,   5,   6,   7, 100, /* 7*/
   100,   0,   1,   2,   3,   4,   5,   6,   7 }; /* 8*/

u8 YPole[h8+1]=

{  100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100,   0,   0,   0,   0,   0,   0,   0,   0, 100, /* 1*/
   100,   1,   1,   1,   1,   1,   1,   1,   1, 100, /* 2*/
   100,   2,   2,   2,   2,   2,   2,   2,   2, 100, /* 3*/
   100,   3,   3,   3,   3,   3,   3,   3,   3, 100, /* 4*/
   100,   4,   4,   4,   4,   4,   4,   4,   4, 100, /* 5*/
   100,   5,   5,   5,   5,   5,   5,   5,   5, 100, /* 6*/
   100,   6,   6,   6,   6,   6,   6,   6,   6, 100, /* 7*/
   100,   7,   7,   7,   7,   7,   7,   7,   7 }; /* 8*/

s8 OpacnePole[h8+1]=

{  100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100,  91,  92,  93,  94,  95,  96,  97,  98, 100, /* 1 */
   100,  81,  82,  83,  84,  85,  86,  87,  88, 100, /* 2*/
   100,  71,  72,  73,  74,  75,  76,  77,  78, 100, /* 3*/
   100,  61,  62,  63,  64,  65,  66,  67,  68, 100, /* 4*/
   100,  51,  52,  53,  54,  55,  56,  57,  58, 100, /* 5*/
   100,  41,  42,  43,  44,  45,  46,  47,  48, 100, /* 6*/
   100,  31,  32,  33,  34,  35,  36,  37,  38, 100, /* 7*/
   100,  21,  22,  23,  24,  25,  26,  27,  28 }; /* 8*/


s8 BelostPole[h8+1]=

{  100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100,   0,   1,   0,   1,   0,   1,   0,   1, 100, /* 1 */
   100,   1,   0,   1,   0,   1,   0,   1,   0, 100, /* 2*/
   100,   0,   1,   0,   1,   0,   1,   0,   1, 100, /* 3*/
   100,   1,   0,   1,   0,   1,   0,   1,   0, 100, /* 4*/
   100,   0,   1,   0,   1,   0,   1,   0,   1, 100, /* 5*/
   100,   1,   0,   1,   0,   1,   0,   1,   0, 100, /* 6*/
   100,   0,   1,   0,   1,   0,   1,   0,   1, 100, /* 7*/
   100,   1,   0,   1,   0,   1,   0,   1,   0 }; /* 8*/

s8 StdCenaB[8][h8+1]=
/* Pesec */{
{  100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100,   0,   0,   0,   0,   0,   0,   0,   0, 100, /* 1 */
   100,   0,   0,  -1, - 8, - 8,   0,   0,   0, 100, /* 2*/
   100,   1,   1,   1,   4,   4,   0,   1,   1, 100, /* 3*/
   100,   1,   1,   6,   8,   8,   6,   1,   1, 100, /* 4*/
   100,   4,   4,   6,  10,  10,   3,   4,   4, 100, /* 5*/
   100,  10,  10,  10,  15,  15,  10,  10,  10, 100, /* 6*/
   100,  20,  20,  20,  20,  20,  20,  20,  20, 100, /* 7*/
   100,   0,   0,   0,   0,   0,   0,   0,   0 }, /* 8*/

/* Kun */
{  100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100, -10,  -4,  -3,  -2,  -2,  -3,  -4, -10, 100, /* 1 */
   100,  -5,  -5,  -3,   0,   0,  -3,  -5,  -5, 100, /* 2*/
   100,  -3,   0,   3,   5,   5,   3,   0,  -3, 100, /* 3*/
   100,  -2,   5,   5,   7,   7,   5,   5,  -2, 100, /* 4*/
   100,   0,   5,   6,   8,   8,   6,   5,   0, 100, /* 5*/
   100,   5,   6,   7,   8,   8,   7,   6,   5, 100, /* 6*/
   100,   0,   5,   6,   8,   8,   6,   5,   0, 100, /* 7*/
   100, -10,  -5,  -2,   0,   0,  -2,  -5, -10 }, /* 8*/
/* Kral */
{  100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100,  10,  15,  13,  -5,   0,  -5,  17,  12, 100, /* 1 */
   100,  -5,  -5, -10, -15, -15, -10,  -5,  -5, 100, /* 2*/
   100, -15, -15, -18, -20, -20, -18, -15, -15, 100, /* 3*/
   100, -30, -30, -30, -30, -30, -30, -30, -30, 100, /* 4*/
   100, -30, -30, -30, -30, -30, -30, -30, -30, 100, /* 5*/
   100, -30, -30, -30, -30, -30, -30, -30, -30, 100, /* 6*/
   100, -30, -30, -30, -30, -30, -30, -30, -30, 100, /* 7*/
   100, -30, -30, -30, -30, -30, -30, -30, -30}, /* 8*/
   /* Kral v koncovce */
{  100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100, -20, -15, -12, -10, -10, -12, -15, -20, 100, /* 1 */
   100, -15, -12,  -7,   0,   0,  -7, -12, -15, 100, /* 2*/
   100, -12,  -7,   1,   5,   5,   1,  -7, -12, 100, /* 3*/
   100, -10,   0,   5,  10,  10,   5,   0, -10, 100, /* 4*/
   100, -10,   0,   5,  10,  10,   5,   0, -10, 100, /* 5*/
   100, -12,  -7,   1,   5,   5,   1,  -7, -12, 100, /* 6*/
   100, -15, -12,  -7,   0,   0,  -7, -12, -15, 100, /* 7*/
   100, -20, -15, -12, -10, -10, -12, -15, -20}, /* 8*/
   /* Strelec */
{  100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100,  -5,  -4,  -3,  -2,  -2,  -3,  -4,  -5, 100, /* 1 */
   100,  -5,   6,  -2,   5,   5,  -2,   6,  -5, 100, /* 2*/
   100,   0,   0,   1,   5,   5,   1,   0,   0, 100, /* 3*/
   100,   0,   2,   5,   5,   5,   5,   5,   0, 100, /* 4*/
   100,   0,   5,   6,   8,   8,   6,   5,   0, 100, /* 5*/
   100,   0,   0,   0,   0,   0,   0,   0,   0, 100, /* 6*/
   100,   0,   0,   0,   0,   0,   0,   0,   0, 100, /* 7*/
   100,  -5,   0,   0,   0,   0,   0,   0,  -5 }, /* 8*/
/* Volny pesec (aditivum)*/{
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100,   0,   0,   0,   0,   0,   0,   0,   0, 100, /* 1 */
   100,   2,   2,   2,   2,   2,   2,   2,   2, 100, /* 2*/
   100,   3,   3,   3,   3,   3,   3,   3,   3, 100, /* 3*/
   100,   4,   4,   4,   4,   4,   4,   4,   4, 100, /* 4*/
   100,   7,   7,   7,   7,   7,   7,   7,   7, 100, /* 5*/
   100,   8,   8,   8,   8,   8,   8,   8,   8, 100, /* 6*/
   100,   0,   0,   0,   0,   0,   0,   0,   0, 100, /* 7*/
   100,   0,   0,   0,   0,   0,   0,   0,   0 }, /* 8*/
   /* Kral tlaceny do bileho rohu */{
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100, -10,  -8,  -6,  -4,  -2,   0,   1,   2, 100, /* 1 */
   100,  -8,  -6,  -3,  -1,   2,   2,   3,   1, 100, /* 2*/
   100,  -6,  -3,   0,   2,   3,   5,   1,   0, 100, /* 3*/
   100,  -4,  -1,   2,   9,  11,   3,   0,  -2, 100, /* 4*/
   100,  -2,   0,   3,  11,   9,   2,  -1,  -4, 100, /* 5*/
   100,   0,   1,   5,   3,   2,   0,  -3,  -6, 100, /* 6*/
   100,   1,   3,   1,   0,  -1,  -3,  -6,  -8, 100, /* 7*/
   100,   2,   1,   0,  -2,  -4,  -6,  -8, -10 }, /* 8*/

   /* Kral tlaceny do cerneho rohu */{
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
   /*     a    b    c    d    e    f    g    h*/
   100,   2,   1,   0,  -2,  -4,  -6,  -8, -10, 100, /* 1 */
   100,   1,   3,   1,   0,  -1,  -3,  -6,  -8, 100, /* 2*/
   100,   0,   1,   5,   2,   3,   0,  -3,  -6, 100, /* 3*/
   100,  -2,   0,   3,  11,   9,   3,  -1,  -4, 100, /* 4*/
   100,  -4,  -1,   2,   9,  11,   2,   0,  -2, 100, /* 5*/
   100,  -6,  -3,   0,   3,   2,   5,   1,   0, 100, /* 6*/
   100,  -8,  -6,  -3,  -1,   0,   1,   3,   1, 100, /* 7*/
   100, -10,  -8,  -6,  -4,  -2,   0,   1,   2 }, /* 8*/

};
static s16 KonCenyPescu[8]={0,-30,-28,-8,10,30,50,0};

/* Vrati nahodne cele cislo z intervalu (-maxabs,maxabs)
   z rozdeleni, kde hustota linearne roste od -maxabs,
   kde je 0 k 0, kde je nejvyssi a pak linearne klesa
   k maxabs, kde je nula. Jde o jakousi velmi spatnou
   aproximaci binomickeho rozdeleni.
*/

int Sum(int maxabs) {
  int i, j, m;
 
  if (maxabs <= 0) return 0;
  m = maxabs * maxabs;
  i = rand() % m;
  for (j = 0; j <= maxabs; j++) {
    i -= j;
    if (i <= 0) return j - maxabs + 1;
  }
  for (j = maxabs - 1; j > 1; j++) {
    i -= j;
    if (i <= 0) return maxabs - j;
  }
  return maxabs - 1;
}


/*************************************/
/* nastavi TAlgCfg na bezne hodnoty  */
/*************************************/
void NastavAlgCfgStd(TAlgCfg *a) {
  int i;

  a->AlgKoef = STDAlgKoef;
  for (i = a2; i <= h7; i++) {
    a->bp[i] = StdCenaB[0][i];
    a->bpv[i] = StdCenaB[5][i];
  }
  for (i = 1; i <= 6; i++) {
    memcpy(a->cp + a1 + (7 - i) * 10, a->bp + a1 + i * 10, 16);
    memcpy(a->cpv + a1 + (7 - i) * 10, a->bpv + a1 + i * 10, 16);
  }

  for(i = a1; i <= h8; i++) {
    a->bj[i] = StdCenaB[1][i];
    a->bs[i] = StdCenaB[4][i];
    a->bkz[i] = StdCenaB[2][i];
    a->kk[i] = StdCenaB[3][i];
    a->kkb[i] = StdCenaB[6][i];
    a->kkc[i] = StdCenaB[7][i];
  }

  for(i = 0; i <= 7; i++){
    memcpy(a->cj + a1 + (7 - i) * 10, a->bj + a1 + i * 10, 16);
    memcpy(a->cs + a1 + (7 - i) * 10, a->bs + a1 + i * 10, 16);
    memcpy(a->ckz + a1 + (7 - i) * 10, a->bkz + a1 + i * 10, 16);
  }
}


/*******************/
/* zasumi TAlgCfg  */
/*******************/
void ZasumAlgCfg(TAlgCfg *a, int sum){

  int i;

  a->AlgKoef.PesecKryty += Sum(sum);
  a->AlgKoef.PesecVedle += Sum(sum);
  a->AlgKoef.PesecE2Blok += Sum(sum);
  a->AlgKoef.PesecD2Blok += Sum(sum);
  a->AlgKoef.DvojPesec += Sum(sum);
  a->AlgKoef.PesecIzolovany += Sum(sum);
  a->AlgKoef.PesecVolny += Sum(sum);
  a->AlgKoef.PesecSlaby += Sum(sum);
  a->AlgKoef.SlabyPesecNapaden += Sum(sum);
  a->AlgKoef.VezNaSloupci += Sum(sum);
  a->AlgKoef.FischeruvStrelec += Sum(sum);

  for (i = a2; i <= h7; i++) {
    a->bp[i] += Sum(sum / 2);
    a->bpv[i] += Sum(sum / 2);
    a->cp[i] += Sum(sum / 2);
    a->cpv[i] += Sum(sum / 2);
  }
 
  for (i = a1; i <= h8; i++){
    a->bj[i] += Sum(sum);
    a->bs[i] += Sum(sum);
    a->bkz[i] += Sum(sum);
    a->kk[i] += Sum(sum);
    a->cj[i] += Sum(sum);
    a->cs[i] += Sum(sum);
    a->ckz[i] += Sum(sum);
  }
}

/* 
  Vsechny figurky musi byt pochopitelne na sachovnici
  a pesec nekde mezi druhou a sedmou radou (vcetne.)
*/

int ChytiBilehoPesce(int CernyKral, int BilyPesec, int HrajeBily){
  int kcx, kcy, pbx, pby;

  if (HrajeBily) BilyPesec += 10;
  /*ted posuzuji, jako by byl na tahu cerny*/
  kcx = XPole[CernyKral];
  kcy = YPole[CernyKral];
  pbx = XPole[BilyPesec];
  pby = YPole[BilyPesec];
  if (pby > kcy + 1 || abs(kcx - pbx) > 8 - pby) return 0;
  return 1;
}

#define Koncovka 0
#define BilyMacen 1
#define CernyMacen 2
#define StredniHra 3

/*****************************************/
/* Vrati hodnotu pozice mezi alfa a beta */
/*****************************************/
 /*na u->zasobnik.hodpos[u->zasobnik.pos] ulozi
   1) nedojde-li k orezani hodnotu pozice else
    2) nejsem-li v koreni u->zasobnik.hodpos[u->zasobnik.pos-1] else
     3) 0
   */

s16 HodnotaPozice(TUloha *u,s16 alfa,s16 beta) {
  s16 h, s, /* sem postupne pricitam pozicni faktory k materialu */
    stara, /* stara pozice bez materialu (kvuli prorezani) */
    material,bm,cm;
  s8 *ph8,*p,*pom,*pzaklad;
  int bily;
  int hloubka,typ,i,j;
  THashPech *php;

#ifdef Statistika
    u->stat.OH++;
#endif
  
  bily = u->pozice.bily;
  hloubka = u->zasobnik.pos;
 
  if (hloubka) stara = (s16) u->zasobnik.hodpos[hloubka - 1];
  
  bm = u->zasobnik.bm[hloubka];
  cm = u->zasobnik.cm[hloubka];

  bm -= StdCenyFigur[6];
  cm -= StdCenyFigur[6];

  if (bm >= 550 && cm >= 550) typ = StredniHra; else
  if (bm < 350 && bm < cm - 200) typ = BilyMacen; else
  if (cm < 350 && cm < bm - 200) typ = CernyMacen; else
  typ = Koncovka;

  material = h = (s16)(bm - cm);
  if (!bily) {
    h = (s16) -h;
    if (hloubka) stara = (s16) -stara;
  }
 /*  Kdyz jsem blizko matu, nema smysl hodnotit pozici */
  if (alfa > mat -255 || beta < -mat + 255)
      goto Konec; 
  /* Nasleduje orezani
  Kdyz novy material + pozice pred pultahem je moc velke
  nebo moc male, ani to nepocitej */

  if (hloubka) {
    if(h + u->AlgCfg.AlgKoef.MaxPosZmena + stara <= alfa) {
      h = alfa;
      goto Konec;
    }
    if (h -u->AlgCfg.AlgKoef.MaxPosZmena + stara >= beta) {
      h = beta;
      goto Konec;
    }
  }

#ifdef Statistika
    u->stat.OH2++;
#endif

  s = 0;
  pzaklad = u->pozice.sch;
  ph8 = pzaklad + h8;

    /* Pozice kralu v matici koncovce */
  if (typ == BilyMacen || typ == CernyMacen) {
    div_t bk,ck;
    bk = div(u->material.bk - a1, 10);
    ck = div(u->material.ck - a1, 10);

    i = abs(bk.rem - ck.rem) + abs(bk.quot - ck.quot) - 3;
    if (i < 0) i++;
    i = 14 - i;
    if (typ == CernyMacen) {
      s += i * 5;
      /* koncovka SJ */
      if (!cm && bm == StdCenyFigur[2] + StdCenyFigur[3]) {
         for (p = pzaklad + a1; p <= ph8; p++)
           if (*p == 3) break;
         if (BelostPole[p - pzaklad])
           s -= 5 * u->AlgCfg.kkc[u->material.ck];
         else
           s -= 5 * u->AlgCfg.kkb[u->material.ck];

      } else {
        s -= u->AlgCfg.kk[u->material.ck];
      }
    } else { /* Bily macen */
      s -= i * 5;
      /* koncovka SJ */
      if (!bm && cm == StdCenyFigur[2] + StdCenyFigur[3]) {
         for (p = pzaklad + a1; p <= ph8; p++)
           if (*p == -3) break;
         if (BelostPole[p - pzaklad])
           s += 5 * u->AlgCfg.kkb[u->material.bk];
         else
           s += 5 * u->AlgCfg.kkc[u->material.bk];

      } else {
        s += u->AlgCfg.kk[u->material.bk];
      }
    }
  }

  /*
    Zkusim nacist hodnotu pescove struktury z hash tabulek.
  */
  if (!u->AlgCfg.AlgKoef.PovolPechHash) {
    php = u->hpt;
    goto NeniTam; 
  }

  php = u->hpt + (u->zasobnik.hPechF[hloubka] &    ((1 << u->HashCfg.DveNaXHashPech) - 1));
  if (php->kod == u->zasobnik.hPechG[hloubka]) goto JeTam;

  NeniTam:;
#ifdef Statistika
    u->stat.OHP++;
#endif
  NastavPesce(u,php);
  JeTam:;
  s += php->cena;

  /* blokace pesce ed 27 figurou */
  if (*(pzaklad + e2) ==  1 && *(pzaklad + e3)) s += u->AlgCfg.AlgKoef.PesecE2Blok;
  if (*(pzaklad + d2) ==  1 && *(pzaklad + d3)) s += u->AlgCfg.AlgKoef.PesecD2Blok;
  if (*(pzaklad + e7) == -1 && *(pzaklad + e6)) s -= u->AlgCfg.AlgKoef.PesecE2Blok;
  if (*(pzaklad + e7) == -1 && *(pzaklad + d6)) s -= u->AlgCfg.AlgKoef.PesecD2Blok;
 

  {/*Nasleduje test na utekle volne pesce*/
    int NejBileho = 0, NejCerneho = 0;
    for (i = 1; i <= 4; i++) 
      if (u->material.c[i] || u->material.b[i]) {
        i = 0;
        break;
      }
    if (i) { /*Pokud nejsou figury, jen kralove a pesci...*/
      j = u->material.b[0];
      for (i = 0; i < j; i++) {
        if (!((php->bt[i]) & 16) || /*pesec neni volny*/
          ChytiBilehoPesce(u->material.ck,php->b[i], u->pozice.bily)) /* nebo jde zastavit*/
          continue; 
        NejBileho = MAX(NejBileho, YPole[php->b[i]]);
      }
      j = u->material.c[0];
      for (i = 0; i < j; i++) {
        if (!((php->ct[i])&16) || /*pesec neni volny*/
          ChytiBilehoPesce(OpacnePole[u->material.bk],OpacnePole[php->c[i]],!u->pozice.bily)) /* nebo jde zastavit*/
          continue; 
        NejCerneho=MAX(NejCerneho, 7-YPole[php->c[i]]);
      }
      if (NejBileho > NejCerneho + 1) s+= ((NejBileho << 4) + 256); else
      if (NejBileho < NejCerneho - 1) s-= (((7 - NejCerneho) << 4) + 256);
    }
  }

  for (p = pzaklad + a1; p <= ph8; p++)
    switch (*p) {
      case 0: continue;
      case 100: continue;
      case 1: continue;
      case -1: continue;
      case 2: s += u->AlgCfg.bj[p - pzaklad]; break;
      case -2: s -= u->AlgCfg.cj[p - pzaklad]; break;
      case 3:
        /*Strelec v pasti*/
        if ((p == pzaklad + a7 || p == pzaklad + b8) &&
          *(pzaklad + b6) == 1 && *(pzaklad + c7)==1 ||
          (p == pzaklad + h7 || p == pzaklad + g8) &&
          *(pzaklad + g6) == 1 && *(pzaklad + f7) == 1)
            s += u->AlgCfg.AlgKoef.FischeruvStrelec;
    
        /* strelec je dobry, kdyz ma barvu souperovych pescu */
          s += ((BelostPole[p - pzaklad] ? php->cb : php->cc) << 1);
        /* Tabulkova hodnota */
          s += u->AlgCfg.bs[p-pzaklad];
        break;
      case -3:
        /*Strelec v pasti*/
        if ((p == pzaklad + a2 || p == pzaklad + b1) &&
          *(pzaklad + b3) == -1 && *(pzaklad + c2) == -1 ||
          (p == pzaklad + h2 || p == pzaklad + g1) &&
          *(pzaklad + g3) == -1 && *(pzaklad + f2) == -1)
            s -= u->AlgCfg.AlgKoef.FischeruvStrelec;
    
        /*strelec je dobry, kdyz ma barvu souperovych pescu*/
        s -= ((BelostPole[p - pzaklad] ? php->bb : php->bc) << 1);
        s -= u->AlgCfg.cs[p - pzaklad];
        break;
      case 4:
        j = XPole[p - pzaklad];
        /*Vez na volnem sloupci*/
          if (!u->material.CPSloupec[j]) {
          if (!u->material.BPSloupec[j]) s += u->AlgCfg.AlgKoef.VezNaSloupci;
        } else {
              for (i = -10; i <= 10; i += 20) { /* Po sloupci nahoru a dolu */
                for (pom = p + i; !*pom; pom += i);
              if (*pom == -1)
                  for (j = 0; j < u->material.c[0]; j++)
                      if (php->c[j] == (pom - pzaklad)) {
                      if (php->ct[j] & 2) /*Napada opozdeneho pesce*/
                        s += u->AlgCfg.AlgKoef.SlabyPesecNapaden;
                          break;
                  }
            }
        }/* od else*/
        break;
      case -4:
        j = XPole[p - pzaklad];
        /*Vez na volnem sloupci*/
          if (!u->material.BPSloupec[j]) {
          if (!u->material.CPSloupec[j]) s -= u->AlgCfg.AlgKoef.VezNaSloupci;
        } else {
           /* Po sloupci nahoru a dolu */
            for (i =- 10; i <= 10; i += 20) {
              for (pom = p + i; !*pom; pom += i);
            if (*pom == 1)
                for(j = 0; j < u->material.b[0]; j++)
                    if (php->b[j] == (pom - pzaklad)) {
                    if (php->bt[j] & 2) /*Napada opozdeneho pesce*/
                      s -= u->AlgCfg.AlgKoef.SlabyPesecNapaden;
                      break;
                }
          }
        }/* od else*/
        break;
      case 6:
        if (u->zasobnik.bm[hloubka] > StdCenyFigur[6] + 700)
          s += u->AlgCfg.bkz[p - pzaklad];
          else
          s+=u->AlgCfg.kk[p-pzaklad];
          if (typ == Koncovka) {
            for (i = 0; i < 8; i++)
            if(*(p + ofsety[i]) == -1) {/*Kral napada pesce*/
              for (j = 0; j < u->material.c[0]; j++)
                if (php->ct[j] & 2 && php->c[j] == (p - pzaklad) + ofsety[i])
                /*Napada opozdeneho pesce*/
                  s += u->AlgCfg.AlgKoef.SlabyPesecNapaden;
          }
        }
          break;
      case -6:
        if (u->zasobnik.cm[hloubka] > StdCenyFigur[6] + 700)
          s -= u->AlgCfg.ckz[p - pzaklad];
          else
          s -= u->AlgCfg.kk[p - pzaklad];
          if (typ == Koncovka) {
            for (i = 0; i < 8; i++)
              if (*(p + ofsety[i]) == 1) {/*Kral napada pesce*/
              for(j = 0; j < u->material.b[0]; j++)
                if(php->bt[j] & 2 && php->b[j] == (p-pzaklad) + ofsety[i])/*Napada opozdeneho pesce*/
                    s -= u->AlgCfg.AlgKoef.SlabyPesecNapaden;
            }
        }
          break;
  } /* od for cyklu pres pole */
  if (bily) h += s; else h -= s;
  u->zasobnik.hodpos[hloubka] = (bily ?  h : - h) - material;
  goto ven;
Konec:
  u->zasobnik.hodpos[hloubka] = hloubka ? u->zasobnik.hodpos[hloubka - 1] : 0;
ven:
  return (h < alfa ? alfa : (h > beta ? beta : h));
}
