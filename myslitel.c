/*********************************************************/
/* myslitel.c - rekurzivni propocet (alfabeta)           */
/* 10.1. 2001 Jan Nemec                                   */
/*********************************************************/
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "volby.h"
#ifdef unix
#include <sys/timeb.h>
#endif

#include "cotyvar.h"
#include "dosio.h"
#include "chyba.h"
#include "generato.h"
#include "globruti.h"
#include "knihovna.h"
#if Typ_Produktu==DOS_UNIX
#include <stdio.h>
#include "dosio.h"
#include "strtah.h"
#endif
#include "hashtab.h"
#include "ohodnoc.h"
#include "tabulky.h"
#include "lokruti.h"
#include "myslitel.h"
#if Typ_Produktu==Win32_Program
#include "kontrola.h"
#include "winmysl.h"
#include <windows.h>
#endif
#if Typ_Produktu==Projekt
#include "scio.h"
#endif
#include "ticho.h"
#include "pole.h"


#ifdef Statistika
int Stav(const TUloha *u) {
  return u->stat.TA;
}
#endif


void DotazNaCas(TUloha *u) {
#ifdef unix
  struct timeb t;
  (void) ftime(&t);
  if (t.time>u->StavPropoctu.sKonceMysleni ||
    t.time==u->StavPropoctu.sKonceMysleni &&
    t.millitm>u->StavPropoctu.msKonceMysleni)
    u->StavPropoctu.MamKoncitMysleni = 1;
#else
  if ((clock() * 1000) / CLK_TCK > u->StavPropoctu.msKonceMysleni)
    u->StavPropoctu.MamKoncitMysleni = 1;
#endif
}


/**********************************************/
/* Po preteceni se vydeli hist. heur. ctyrmi  */
/**********************************************/
void PulHistHeur(TUloha *u) {
  int i;
  for (i = 0; i < (1 << 16); i++)
    u->HistHeur[i] >>= 2;
}


static int PrtOpakRemis(TUloha *u, u32 hF){
  u32 *p=u->RemTab.RemPol;

  if (hF)
    while (hF < *p) p++;
  else
    p += (u->RemTab.poc - 1);
  hassert(p - u->RemTab.RemPol < 101,"Mimo pole RemPol");
  return hF == *p;
}


/*********************************************************/
/* Nastavi podle aktualni pozice polozky uloha->material */
/*********************************************************/
void SectiJenMaterial(TUloha *uloha) {
  s8 *p;
  s8 *konec;
  int sloupec;

  konec=&(uloha->pozice.sch[h8]);
  (void) memset((void *) &(uloha->material), 0, sizeof(TMaterial));
  for (p = &(uloha->pozice.sch[a1]), sloupec = 0; p <= konec;
     sloupec = (sloupec == 9 ? 0 : sloupec + 1), p++)
  switch (*p) {
    case 0:break;
    case 100:break;
    case  1: uloha->material.b[0]++;
             uloha->material.BPSloupec[sloupec]++;
             break;
    case -1: uloha->material.c[0]++;
             uloha->material.CPSloupec[sloupec]++;
             break;
    case  2: uloha->material.b[1]++; break;
    case -2: uloha->material.c[1]++; break;
    case  3: uloha->material.b[2]++; break;
    case -3: uloha->material.c[2]++; break;
    case  4: uloha->material.b[3]++; break;
    case -4: uloha->material.c[3]++; break;
    case  5: uloha->material.b[4]++; break;
    case -5: uloha->material.c[4]++; break;
    case  6: uloha->material.bk = p - uloha->pozice.sch; break;
    case -6: uloha->material.ck = p - uloha->pozice.sch; break;
  }
}



/****************************************************/
/* Tridici heuristika - udrzovani hlavni varianty   */
/* tahu z h.v. pridam bonus                         */
/****************************************************/
void KopirujHVar(TUloha *uloha, u16 t) {
  int i,h;

   h = uloha->zasobnik.pos;
   hassert(h<MaxHloubkaPropoctu,"Hloubka propoctu");
   uloha->hvar[h][h] = t;
   for (i = 1; ; i++) {
     if (!uloha->hvar[h + 1][h + i]) {
       uloha->hvar[h][h + i] = uloha->hvar[h + 1][h + i];
       return;
     }
     uloha->hvar[h][h+i]=uloha->hvar[h+1][h+i];
   }
}

/*******************************************************************/
/* NulovyTah                                                       */
/* Necham ted hrat soupere, i kdyz na to nema pravo. Jsem na tom   */
/* stejne lepe nez beta nebo stejne ? Beta se preda jako prah.     */
/*******************************************************************/
static int NulovyTah(TUloha *uloha, s16 prah, s16 hloubka
#ifdef HTML_VYPISY
 , char *varianta
#endif
  ) {
  int b;
  u8 m;

#ifdef HTML_VYPISY
  FILE *soub = NULL;
  char podvarianta[256];

  if (hloubka > 1) {
    char odkaz[256];
    if (varianta) soub = otevriHtmlLog(uloha, varianta);
    if (soub) {
      fprintf(soub, "\n<P>Metoda nuloveho tahu(%i) do hloubky %i, zanoreni = %i, varianta = %s<BR>\n",
        prah, hloubka, uloha->zasobnik.pos, varianta + 1);
      sprintf(podvarianta, "%sab", varianta);
      sprintf(odkaz, "%s.html", podvarianta);
      fprintf(soub, "<A HREF=\"%s\">null</A> ", odkaz);
    }
  }
#endif

  switch (hloubka) {
    case 1: hloubka = 0; break;
    case 2:
    case 3: hloubka = 1; break;
    default: hloubka -= 2;
  }
  hassert(hloubka >= 0, "Hloubka");


  uloha->StavPropoctu.VNT++;
  uloha->zasobnik.hF[uloha->zasobnik.pos] ^= hnF.bily;
  uloha->zasobnik.hG[uloha->zasobnik.pos] ^= hnG.bily;
  uloha->pozice.bily=!uloha->pozice.bily;
  m = uloha->pozice.mimoch;
  uloha->pozice.mimoch = 0;


  b =  (
    (hloubka ? -AlfaBeta(uloha, (s16)(-prah), (s16)(1-prah), hloubka
#ifdef HTML_VYPISY
 , podvarianta
#endif
     )    : 
  -AlfaBetaBrani(uloha, (s16)(-prah), (s16)(1-prah), 4)
    )
    
    >= prah);
       
  uloha->pozice.mimoch=m;
  uloha->pozice.bily=!uloha->pozice.bily;
#ifdef Statistika
  if (b)
    uloha->stat.NTU[uloha->zasobnik.pos]++;
  else
    uloha->stat.NTN[uloha->zasobnik.pos]++;
#endif
#ifdef HTML_VYPISY
  if (soub) fprintf(soub, "<BR>%s</BR>", (b ? "Orezavam" : "Neorazavam"));
#endif
  uloha->StavPropoctu.VNT--;
  uloha->zasobnik.hF[uloha->zasobnik.pos]^=hnF.bily;
  uloha->zasobnik.hG[uloha->zasobnik.pos]^=hnG.bily;
#ifdef HTML_VYPISY
  if (soub) zavriHtmlLog (soub);
#endif    
  return b;
}

/**********************************************************/
/* Jednoduchy (ne zcela korektni) test, zda je uz remiza  */
/* nedostatkem materialu, aby se dal nepocitalo           */
/**********************************************************/
int MaterialRemis(TUloha *u){
   hassert(u->zasobnik.pos < MaxHloubkaPropoctu, "Hloubka propoctu");
   if (u->material.b[0] || u->material.c[0] ||
       u->zasobnik.bm[u->zasobnik.pos]+
       u->zasobnik.cm[u->zasobnik.pos]>40200)
     return 0;
   return 1;
}


#if 0
/* Nakonec to nevyuzivam */
static cmptah(const void *elem1, const void *elem2 ){
  s16 ct1,ct2;

  ct1 = (((TTah1 *)(elem1)))->cena;
  ct2 = (((TTah1 *)(elem2)))->cena;
  return ct1 < ct2 ? 1 : (ct1 > ct2 ? -1 : 0);
}
#endif

static u8 prohloubeni(TUloha *uloha, int zanor, int hloubka) {
  int pom = uloha->zasobnik.kam[zanor - 1];
  int poc, i;
  s8 *uk;

 /*Souper mi neco sebral a ja jsem dobral
  jeho berouci figuru - to stoji za prohloubeni*/
  if (
    /* Nerozsiroval jsem moc */
    uloha->AlgCfg.AlgKoef.MaxRozsirDober >= 
    uloha->zasobnik.rozsir[zanor-1] &&
    /* Muze jit o dobrani, nejsem v nulovem tahu */
    zanor > 1 && !uloha->StavPropoctu.VNT &&
    /* Bralo se */
    uloha->zasobnik.brani[zanor - 2] &&
    /* Tahne se tam, co minule */
     pom == uloha->zasobnik.kam[zanor - 2] &&
    /* a minule se nerozsirovalo - hlavne kvuli opakovani tehoz prohlubovani
       napr. se vymeni pici a pak toho kryteho pika jeste sezere dama*/
     uloha->zasobnik.rozsir[zanor - 2] != ROZSIR_DOBRANI)
    return ROZSIR_DOBRANI;

  /* Jsem v listu a pesec dosel na predposledni radu  */
  if (
    /* list */
    !hloubka &&
    /* Nebylo toho moc */
    uloha->AlgCfg.AlgKoef.MaxRozsirPesec7 >= uloha->zasobnik.rozsir[zanor-1] &&
    /* Bily pesec na 7 */
    (uloha->pozice.sch[pom] == 1 && pom >= a7
    /* Cerny pesec na 8 */
    || uloha->pozice.sch[pom] == -1 && pom <= h2)
    ) /*od if*/
    return ROZSIR_PESEC_7;

  /* Pokryl sach predstavenim */
  if (zanor > 1 &&
    /* Nebylo toho moc */
    uloha->AlgCfg.AlgKoef.MaxRozsirPredstav >=
      uloha->zasobnik.rozsir[zanor - 1] &&
    /* Byl sach */
    uloha->zasobnik.sach[zanor - 1] &&
    /* Ale reakci nebyl ani tah kralem */
    abs(uloha->pozice.sch[pom]) != 6 &&
    /* ani braní */
    !uloha->zasobnik.brani[zanor - 1]
    )
    return ROZSIR_PREDSTAVENI;

  if (uloha->AlgCfg.AlgKoef.MaxRozsirFischer >=
      uloha->zasobnik.rozsir[zanor-1])
    switch (*(uk = uloha->pozice.sch + pom)) {
    case 3:
      /* Past na Fischerova strelce. */
      if (pom == a7 &&
          uloha->pozice.sch[b7] == -1 &&
          uloha->pozice.sch[c7] == -1 ||
          pom==h7 &&
          uloha->pozice.sch[f7]==-1 &&
          uloha->pozice.sch[g7]==-1)
	  return ROZSIR_FISCHER;
      break;
    case 5:
      /* Ohrozeni krale damou */
      if (uloha->zasobnik.rozsir[zanor-1] <1 &&
          HrozbaCernemuKraliDamou(uloha))
        return ROZSIR_HROZBA_DAMOU;
      break;
    case -3:
      /* Past na Fischerova strelce. */
      if (pom == a2 && uloha->pozice.sch[b2] == 1
          && uloha->pozice.sch[c2] == 1 ||
          pom == h2 && uloha->pozice.sch[f2] == 1 && uloha->pozice.sch[g2] == 1)
        return ROZSIR_FISCHER;
      break;
    case -5:
      /* Ohrozeni krale damou */
      if (uloha->zasobnik.rozsir[zanor - 1] < 1 &&
          HrozbaBilemuKraliDamou(uloha))
        return ROZSIR_HROZBA_DAMOU;
      break;
    case 2:
      if (!hloubka) {
        for (poc = 0, i = 8; i < 16; i++)
	  if (*(uk + (ofsety[i])) < -2)
	    poc++;
	  if(poc > 1) return ROZSIR_VIDLE; /* vidle */
	}
      break;
    case -2:
      if (!hloubka) {
        for (poc = 0, i = 8; i < 16; i++)
          if (*(uk + (ofsety[i])) > 2 && *(uk + ofsety[i]) < 7)
            poc++;
	  if(poc>1) return ROZSIR_VIDLE; /* vidle */
        }
      break;
    case 1: 
      if (*(uk + 11) < -1 && *(uk + 9) < -1)
        return ROZSIR_VIDLE;
      if (
        pom == g3 && uloha->pozice.sch[h2] == -3 &&
        uloha->pozice.sch[f2] == 1 ||
        pom == b3 && uloha->pozice.sch[a2] == -3 &&
        uloha->pozice.sch[c2] == 1)
        return ROZSIR_FISCHER;
      break;
    case -1:
      if (*(uk - 11) < 7 && *(uk - 9) < 7 && *(uk - 11) > 1 && *(uk - 9) > 1)
        return ROZSIR_VIDLE;
      if (pom == g6 && uloha->pozice.sch[h7] == 3 &&
        uloha->pozice.sch[f7] == -1 ||
        pom == b6 && uloha->pozice.sch[a7] == 3 &&
        uloha->pozice.sch[c7] == -1)
      	return ROZSIR_FISCHER;
      break;
  }
  return 0;
}


/*****************************************************************************/
/* AlfaBeta do dane hloubky. Vraci hodnotu z hlediska toho, kdo je na tahu.  */
/* Hleda hodnotu na otevrenem intervalu (alfa,beta), vraci cislo z uzavreneho*/
/* intervalu <alfa,beta>. Pokud nestihne propocet, nastavi MamKoncit         */
/* na 1 a vrati alfa.                                                        */
/*****************************************************************************/
s16 AlfaBeta(TUloha *uloha, s16 alfa, s16 beta, s16 hloubka
#ifdef HTML_VYPISY
 , char *varianta
#endif
             ) {
  
  TTah1 *pt0, *pt1, *pt2;
  int MamPripustnyTah, prohlub;
  u16 *nt1, *nt2;
  int zanor, pom;
  s16 cena, a, b, h;
#ifdef Statistika
  int alfaporadi = -1;
#endif
#ifdef HTML_VYPISY
  int tmp_oh;
  char posledniTah[20];
  char podvarianta[256];
  char odkaz[256];

  FILE *soub = NULL;
  if (varianta) soub = otevriHtmlLog(uloha, varianta);
  if (soub) fprintf(soub, "\n<P>AlfaBeta(%i, %i) do hloubky %i, zanoreni = %i, varianta = %s<BR>\n",
                    alfa, beta, hloubka, uloha->zasobnik.pos, varianta);
#endif
               
  if (beta <= -mat) {
#ifdef HTML_VYPISY
    if (soub) {
      fputs("\nbeta <= -mat, return -mat\n", soub);
      zavriHtmlLog (soub);
    }
#endif    
    return -mat;
  }
#ifdef Statistika
  uloha->stat.AB[hloubka]++;
  uloha->stat.UZ++;
#endif

  zanor = uloha->zasobnik.pos;

  if (zanor > MaxHloubkaPropoctu - 4) {
    cena = HodnotaPozice(uloha, alfa, beta);
#ifdef HTML_VYPISY
    if (soub) {
      fprintf(soub, "Chyba!!! Jsem moc hluboko, vracim HodnotaPozice=%i\n", (int)cena);
      if (soub) zavriHtmlLog (soub);
    }
#endif
    return cena;
  }

/* Nejprve detekuji vsechny mozne remizy */
  if (!uloha->zasobnik.zmen[zanor - 1]) {
    /* V propoctu zatim nedoslo k brani ani tahu pescem =>
       je treba otestovat remizy */
    if (PrtOpakRemis(uloha, uloha->zasobnik.hF[zanor])) {
#ifdef HTML_VYPISY
      if (soub) {
        fprintf(soub, "Opakovani pozice z partie 0\n");
        if (soub) zavriHtmlLog (soub);
      }
#endif
      return 0;
    }

    if (uloha->RemTab.poc + zanor > 100) {
      /*remiza 50 tahu*/
#ifdef HTML_VYPISY
      if (soub) {
        fprintf(soub,"50 tahu 0\n");
        zavriHtmlLog (soub);
      }
#endif
      return 0;
    }
  }
  if (MaterialRemis(uloha)) {
    /*Remiza nedostatkem materialu*/  
#ifdef HTML_VYPISY
    if (soub) {
      fprintf(soub,"Neni matici material 0\n");
      if (soub) zavriHtmlLog (soub);
    }
#endif
    return 0;
  }
  for (cena = (s16)(zanor - 1);cena >= 0; cena--)
    if (uloha->zasobnik.hF[zanor] == uloha->zasobnik.hF[cena] &&
        uloha->zasobnik.hG[zanor]==uloha->zasobnik.hG[cena]) {
    /*Remiza opakovanim pozice */          
#ifdef HTML_VYPISY
      if (soub) {
        fprintf(soub,"Opakovani pozice 0\n");
        zavriHtmlLog (soub);
      }
#endif
      return 0;
    }
/*Konec detekce remiz*/
  DotazNaCas(uloha);
  if (uloha->StavPropoctu.MamKoncitMysleni) {
    /*Kdyz mi dosel cas...*/    
#ifdef HTML_VYPISY
    if (soub) {
      fprintf(soub, "Vyprsel cas, vracim alfa=%i\n", (int) alfa);
      zavriHtmlLog (soub);
    }
#endif
    return alfa;
  }

  a = alfa;
  b = beta;
  h = hloubka;
/* Promenne alfa, beta a hloubka ukladam kvuli pozdejsimu zapisu do hash
   tabulky */

/* Nasleduje cteni z hash tabulky */
  if (GetHash(uloha, a, b, (u8)(h + 1), &cena)) {
#ifdef Statistika
    uloha->stat.HU[hloubka]++;
#endif
#ifdef HTML_VYPISY
    if (soub) {
      fprintf(soub,"vracim Hash=%i", (int) cena);
      zavriHtmlLog (soub);
    }
#endif
    return cena;
  }
  if (prohlub = prohloubeni(uloha, zanor, hloubka)) {
#ifdef HTML_VYPISY
    if (soub) fprintf(soub,"prohlubuji (typ %i), hloubka=%i<BR>\n", (int) prohlub, (int)(hloubka + 1));
#endif
    hloubka++;
    uloha->zasobnik.rozsir[zanor - 1]++;
    uloha->zasobnik.druhRozsir[zanor - 1] = prohlub;
  }
               
  if (hloubka && zanor > 2 && uloha->zasobnik.druhRozsir[zanor - 2] == ROZSIR_PREDSTAVENI
    && uloha->zasobnik.kam[zanor - 2] != uloha->zasobnik.kam[zanor - 1]) {
    hloubka--;
  }          
               
  /* list propoctu */
  if (!hloubka || MaxHloubkaPropoctu < zanor + 4) {
#ifdef HTML_VYPISY
      tmp_oh = Stav(uloha);
#endif
    cena = AlfaBetaBrani(uloha, alfa, beta,
      uloha->AlgCfg.AlgKoef.MaxHloubkaABB);
#ifdef Statistika
    uloha->stat.UZ--;
#endif
#ifdef HTML_VYPISY
    if (soub) fprintf(soub,"vracim ABB=%i (%i)", (int)cena, Stav(uloha) - tmp_oh);
#endif
    goto Konec;
  }

  /* Jsem v sachu? */
  uloha->zasobnik.sach[zanor] = (uloha->pozice.bily ?
    Ohrozeno(uloha->pozice.sch + uloha->material.bk, 0):
    Ohrozeno(uloha->pozice.sch + uloha->material.ck, 1)
  );

/*** Selektivita ***/

/* Metoda nuloveho tahu */
    if (uloha->StavPropoctu.NulTah
      && !uloha->zasobnik.sach[zanor] && beta < mat - 100) {
#ifdef HTML_VYPISY
    	if (hloubka && soub) {
        tmp_oh = Stav(uloha);
        sprintf(podvarianta, "%s" ODDELOVAC_VARIANT "null", varianta);
        sprintf(odkaz, "%s.html", podvarianta);
        fprintf(soub, "zkousim <A HREF=\"%s\">nulltah</A><BR>\n", odkaz);
      }
#endif

      if (NulovyTah(uloha, beta, hloubka
#ifdef HTML_VYPISY
        , podvarianta
#endif        
        )) {
#ifdef HTML_VYPISY
        if (soub) fprintf(soub, "Nulltah, vracim beta (%i)\n", Stav(uloha) - tmp_oh);
#endif
        cena = beta;
        goto Konec;
      } 
#ifdef HTML_VYPISY
      else {
       	if (soub) fprintf(soub, "Neuspech nulltahu (%i)<BR>\n",  Stav(uloha) - tmp_oh);
      }
#endif
    }
  if (!prohlub) {
/* Kdyz v hloubce 3 mam o damu mene nez alfa, snizim na 2 */
    pom = uloha->zasobnik.bm[zanor] - uloha->zasobnik.cm[zanor];
    if (!uloha->pozice.bily) pom = -pom;
    if (hloubka == 3 && pom + 450 < alfa) {
#ifdef HTML_VYPISY
     	if (soub) fputs("O damu min, snizuju hloubku o 1<BR>\n", soub);
#endif
      hloubka--;
    }
  }
               
/*** Konec selektivnich testu ***/

  if (uloha->zasobnik.sach[zanor]) {
    NalezTahyZeSachu(uloha);
    NASTAVOKRAJE(pt1,pt2)
    pt0 = pt1;
    if (pt1 == pt2) {
#ifdef Vypisy_
      if (pisu) fputs(" Mat, vracim -mat]\n", soub);
#endif
#ifdef HTML_VYPISY
      if (soub) zavriHtmlLog (soub);
#endif
      return -mat;
  } else
    if (pt1 + 1 == pt2 && !prohlub) {
/* Je jen jediny tah => prohlubovani */
    hloubka++;
#ifdef HTML_VYPISY
      if (soub)
        fprintf(soub,
          "jen jeden tah prohlubuji, hloubka=%i<BR>\n", (int)(hloubka));
#endif      
    prohlub = 1;
  }
  else {
    if (!prohlub &&
      
      uloha->AlgCfg.AlgKoef.MaxRozsirSach >=
      uloha->zasobnik.rozsir[zanor - 1]) {
    
        hloubka++;
        uloha->zasobnik.rozsir[zanor - 1]++;
#ifdef HTML_VYPISY
        if (soub)
          fprintf(soub, "Je sach, prohlubuji, hloubka=%i<BR>\n", (int)(hloubka));
#endif
      }
    }
  } else {
    NalezTahy(uloha);
    NASTAVOKRAJE(pt1, pt2)
    pt0 = pt1;
    if (pt1 == pt2) {
#ifdef HTML_VYPISY
      if (soub) fputs("Pat, vracim 0<BR>\n", soub);
#endif
     /* Specialni pripad patu - byl by to pat i kdyby kral mohl do sachu
        teoreticky muze nastat, prakticky asi ne */
      cena = 0;
      goto Konec;
    }
  }
  if (
#ifdef NE_HH
			0 &
#endif
			!uloha->StavPropoctu.VNT) {
    nt1 = &(uloha->hh[zanor][0]);
    nt2 = nt1 + 1;
    for (;pt1 < pt2; pt1++) {
      pt1->cena += uloha->HistHeur[pt1->data];
      if ((pt1->data & 127) == uloha->zasobnik.kam[zanor - 1])
        pt1->cena += 50;
      if (pt1->data == uloha->hvar[zanor][zanor])
        pt1->cena += 250;
      else
        if (pt1->data == *nt1)
          pt1->cena += uloha->AlgCfg.AlgKoef.hh1Bonus;
        else
          if (pt1->data == *nt2)
           pt1->cena += uloha->AlgCfg.AlgKoef.hh2Bonus;
    }
  }

#ifdef HTML_VYPISY
 	if (soub) fprintf(soub, "Po prohlubovani i rozsirovani je hloubka %i.<BR>\n", hloubka);
#endif

  SetridTahy(uloha);
#ifdef HTML_VYPISY
  if (soub) {
    fputs("Setridene tahy po zapocitani historicke heuristiky:<BR>\n", soub);
    for (pt1 = pt0; pt1 < pt2; pt1++) {
      char s[20];
      TahToStr(pt1->data, uloha, s);
      fprintf(soub, "%s %i, ", s, (int) pt1->cena);
    }
    fputs("<BR>\n", soub);
  }
#endif
  {

    TTah1 tah;
    u8 hashHloubka;
    u8 priznak;

    if (GetHashNejlepsi(uloha, &tah.data, &hashHloubka, &priznak)) {
#ifdef HTML_VYPISY
        if (soub)
          fprintf(soub, "Nejlepsi z tabulky: tah = 0x%x, hloubka = %i, priznak = %i<BR>\n",
                  (int)tah.data, (int)hashHloubka, (int)priznak);
#endif

      for (pt1 = pt0; pt1 < pt2; pt1++) {
        if (pt1->data == tah.data) {
          if (pt1 != pt0) {
            tah = *pt1;
            memmove((void *)(pt0 + 1), (void *)pt0, sizeof(TTah1) * (pt1 - pt0));
            *pt0 = tah;
#ifdef HTML_VYPISY
            if (soub) {
              char s[20];
              TahToStr(pt0->data, uloha, s);
              fprintf(soub, "%s, nejlepsi z tabulky jde dopredu<BR>\n", s);
            }
#endif
          }
#ifdef HTML_VYPISY
          else if (soub) fputs("Nalezeny tah uz je prvni<BR>\n", soub);
#endif
          break;
        }
      }
    }
  }
  UlozStav(uloha);


  MamPripustnyTah = 0;
#ifdef HTML_VYPISY
  if (soub) fputs("Propocet tahu:<BR>\n", soub);
#endif
  for (pt1 = pt0; pt1 < pt2; pt1++) {

#ifdef HTML_VYPISY
      if (soub) {
        TahToStr(pt1->data, uloha, posledniTah);
        sprintf(podvarianta, "%s%sv%s%s%i", varianta, ODDELOVAC_VARIANT, posledniTah, ODDELOVAC_VARIANT, h - 1);
        sprintf(odkaz, "v%s%s%i.html", posledniTah, ODDELOVAC_VARIANT, h - 1);
        fprintf(soub, "<A HREF=\"%s\">%s</A> ", odkaz, posledniTah);
      }
#endif

    Tahni(pt1->data,uloha);
    if ((uloha->pozice.bily ?
      Ohrozeno(uloha->pozice.sch+uloha->material.ck,1):
      Ohrozeno(uloha->pozice.sch+uloha->material.bk,0))) {
    
      TahniZpet(pt1->data,uloha);
#ifdef HTML_VYPISY
      if (soub) fputs("Nepripustny tah ",soub);
#endif
      continue;
    }

    {
#ifdef HTML_VYPISY
      tmp_oh = Stav(uloha);
#endif      
      pt1->cena = (s16)
        -AlfaBeta(uloha, BlizKMatu(-beta), BlizKMatu(-alfa), (s16)(hloubka - 1)
#ifdef HTML_VYPISY
        , (soub ? podvarianta : NULL)
#endif
        );
    }
    pt1->cena = DalOdMatu(pt1->cena);
    TahniZpet(pt1->data, uloha);
#ifdef HTML_VYPISY
    if (soub) fprintf(soub," (%i) %i ", Stav(uloha) - tmp_oh, (int)pt1->cena);
#endif
    MamPripustnyTah = 1;
    if (pt1->cena>alfa) {
#ifdef Statistika
      alfaporadi = pt1 - pt0;
#endif
#ifndef NE_HH
      if (!uloha->StavPropoctu.VNT) {
        *nt2 = *nt1; *nt1 = pt1->data;
        KopirujHVar(uloha, pt1->data);
        pom = MAX(5 - zanor, 0);
        pom *= pom;
        PISDOHISTHEUR(uloha, pt1->data, pom)
      }
#endif
      alfa=pt1->cena;
    }

    if (pt1->cena >= beta) {
#ifdef HTML_VYPISY
      if (soub)
        fprintf(soub, " Pres beta, vracim beta=%i\n ", (int) beta);
#endif
#ifdef Statistika
      if (pt1 != pt0) uloha->stat.PR[hloubka]++;
      uloha->stat.BETA_CELKEM[hloubka] += (pt2 - pt0);
      uloha->stat.BETA_PORADI[hloubka] += (pt1 -pt0);
      uloha->stat.BETA_POCET[hloubka]++;
#endif
      SetHashNejlepsi(uloha, pt1->data, (u8) hloubka, (u8)0);
      cena = beta; /* prelezeni okenka */
      goto Konec;
    }
  }
  if (MamPripustnyTah) {
#ifdef HTML_VYPISY
    if (soub)
      fprintf(soub," Konec, vracim alfa=%i\n ",(int)alfa);
#endif
    cena = alfa;
  } else
  {
#ifdef HTML_VYPISY
    if (soub)
      fprintf(soub,"Pat vracim 0\n");
#endif
    cena = 0;
  } /* pat */

Konec:
  if(!uloha->StavPropoctu.MamKoncitMysleni) {
    if (cena <= a) SetHash(uloha, (u8) (h + 1), (u8) 2, a); else
      if (cena >= b) SetHash(uloha, (u8) (h + 1), (u8) 1, b); else
        SetHash(uloha,(u8)(h+1),(u8)3,cena);
  }
  if (alfaporadi >= 0 && cena != beta) {
    SetHashNejlepsi(uloha, (pt0 + alfaporadi)->data, (u8) hloubka, (u8) 1);
#ifdef Statistika
    uloha->stat.ALFA_CELKEM[hloubka] += (pt2 - pt0);
    uloha->stat.ALFA_PORADI[hloubka] += alfaporadi;
    uloha->stat.ALFA_POCET[hloubka]++;
#endif
  }
#ifdef HTML_VYPISY
  if (soub) zavriHtmlLog (soub);
#endif
  return cena;
}

