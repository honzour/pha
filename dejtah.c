#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "volby.h"
#include "cotyvar.h"
#include "knihovna.h"
#include "tabulky.h"
#include "hashtab.h"
#include "generato.h"
#include "ohodnoc.h"
#include "pole.h"
#include "myslitel.h"
#include "globruti.h"
#include "lokruti.h"
#if Typ_Produktu==DOS_UNIX
#include "dosio.h"
#include "strtah.h"
#endif
#ifdef unix
#include <sys/timeb.h>
#endif
#if Typ_Produktu==Win32_Program
#include "winmysl.h"
#endif


int MamKnihovnu=0; /*Pri inicializaci se (zrejme)
prepise na 1*/

void NastavCas(int ms, TUloha *u) {
/* Nastavi cas tak, aby program koncil premysleni
za ms milisekund od doby vyvolani funkce */
#ifdef unix /*Je smutne, ze tohle nejde napsat implementacne
nezavisle... */
  struct timeb t;

  (void) ftime(&t);
  u->StavPropoctu.sKonceMysleni = t.time + ms / 1000;
  u->StavPropoctu.msKonceMysleni = t.millitm + ms % 1000;
  if (u->StavPropoctu.msKonceMysleni >= 1000) {
    u->StavPropoctu.msKonceMysleni -= 1000;
    u->StavPropoctu.sKonceMysleni++;
  }
#else
/*Kdyz nejsme na unixu, spocitam aspon neco podobneho
-ale neni to totez clock() pocita jen cas, kdy je proces
na procesoru
*/
u->StavPropoctu.msKonceMysleni=(clock()*1000)/CLK_TCK+ms;
#endif
}

void SectiMaterialBmCm(TUloha *uloha)
/*************************************************/
/* Nastavi podle aktualni pozice polozky		 */
/* uloha->zasobnik.bm[0], uloha->zasobnik.cm[0]  */
/* a uloha->material							 */
/*************************************************/
 {s8 *p;
  s8 *konec;
  int sloupec;

 uloha->zasobnik.bm[0]=0; uloha->zasobnik.cm[0]=0;
 konec=&(uloha->pozice.sch[h8]);
 (void)memset((void *)&(uloha->material),0,sizeof(TMaterial));
 for(p=&(uloha->pozice.sch[a1]),sloupec=0; p<=konec;
     sloupec=(sloupec==9 ? 0 : sloupec+1),p++){
  if(*p !=100 )
   if (*p>0)uloha->zasobnik.bm[0]+=StdCenyFigur[*p];
    else if(*p<0)uloha->zasobnik.cm[0]+=StdCenyFigur[-*p];
  switch(*p){
  case 0:break;
  case 100:break;
  case  1:uloha->material.b[0]++;uloha->material.BPSloupec[sloupec]++;break;
  case -1:uloha->material.c[0]++;uloha->material.CPSloupec[sloupec]++;break;
  case  2:uloha->material.b[1]++;break;
  case -2:uloha->material.c[1]++;break;
  case  3:uloha->material.b[2]++;break;
  case -3:uloha->material.c[2]++;break;
  case  4:uloha->material.b[3]++;break;
  case -4:uloha->material.c[3]++;break;
  case  5:uloha->material.b[4]++;break;
  case -5:uloha->material.c[4]++;break;
  case  6:uloha->material.bk=p-uloha->pozice.sch;break;
  case -6:uloha->material.ck=p-uloha->pozice.sch;break;
  }
  }
 }

static s16 UpravHodPos(TUloha *u, s16 hodpos){
/*********************************************************/
/* UpravHodPos											 */
/* - vstup = hodnota pozice i s materialem z hlediska 	 */
/*   strany ktera je na tahu							 */
/* - vystup = hodnota pozice bez materialu z hlediska	 */
/*  bileho												 */
/*********************************************************/
	if(!u->pozice.bily)hodpos=(s16)-hodpos;
	return (s16)(hodpos+
		u->zasobnik.cm[u->zasobnik.pos]-
		u->zasobnik.bm[u->zasobnik.pos]);
}

static void NastavNullTah(TUloha *uloha)
/*********************************************/
/* Povoli nebo zakaze uziti nuloveho tahu    */
/* podle typu pozice (zakazuje v koncovce)   */
/*********************************************/
{
uloha->StavPropoctu.NulTah=1;
if(!uloha->AlgCfg.AlgKoef.PovolNullTah
   || !uloha->material.b[3]&&!uloha->material.b[4]&&(uloha->material.b[1]+uloha->material.b[2])<2||
 /*bily ma nejvyse jednu nejvyse lehkou figuru*/
	!uloha->material.c[3]&&!uloha->material.c[4]&&(uloha->material.c[1]+uloha->material.c[2])<2
 /*cerny ma nejvyse jednu nejvyse lehkou figuru*/
	)
	 uloha->StavPropoctu.NulTah=0;


  //uloha->StavPropoctu.NulTah=0;
 }

int cmprem (const void *ho1, const void *ho2){
/**********************/
/* funkce pro qsort() */
/**********************/
 return (*((u32 *)ho1)<*((u32 *)ho2) ?
 1 : (*((u32 *)ho1)>*((u32 *)ho2) ? -1 : 0));
}


static void NastavRemTab(TUloha *u) {
/************************************************/
/* Nastavi tabulku hodnot hash funkce pro       */
/* detekci opakovani pozic z partie v propoctu  */
/************************************************/
 int i;
 TPartie *prt;

 prt = u->prt;
 for(i = 0; i < 100; i++) {
   u->RemTab.RemPol[i] = prt->data.hashF;
   if (prt->data.zmena) break;
   prt = prt->l;
   if (!prt) break;
  }
  u->RemTab.poc=i+1;
  u->RemTab.RemPol[u->RemTab.poc]=0;
  qsort((void *)(u->RemTab.RemPol),u->RemTab.poc,sizeof(u32),cmprem);
}

/**********************************************************/
/* Inicializace pred zahajenim rekurzivniho propoctu      */
/* vola se z DejTah										  */
/**********************************************************/
void InitVypocet(TUloha *uloha) {
  int i;

  uloha->zasobnik.pos=0;
  SectiMaterialBmCm(uloha);
  uloha->zasobnik.hodpos[0]=UpravHodPos(uloha,HodnotaPozice(uloha,-mat,mat));
  NastavNullTah(uloha);
  uloha->zasobnik.hF[0]=HashF(&(uloha->pozice));
  uloha->zasobnik.hG[0]=HashG(&(uloha->pozice));
  uloha->zasobnik.hPechF[0]=HashPechF(&(uloha->pozice));
  uloha->zasobnik.hPechG[0]=HashPechG(&(uloha->pozice));
  /*(void)memset((void *)&(uloha->hh),0,sizeof(uloha->hh));*/
  for(i=0;i<(1<<16);i++)uloha->HistHeur[i]>>=3;
  (void)memset((void *)&(uloha->HistHeur),0,sizeof(uloha->HistHeur));
  (void)memset((void *)&(uloha->hvar[0][0]),0,sizeof(uloha->hvar));
  uloha->StavPropoctu.VNT=0;
  NastavRemTab(uloha);
#if 0
  uloha->zasobnik.bkohroz[0]=HrozbaBilemuKraliDamou(uloha);
  uloha->zasobnik.ckohroz[0]=HrozbaCernemuKraliDamou(uloha);
#endif
#ifdef Statistika
  memset((void *)&(uloha->stat),0,sizeof(TStatistika));
#endif
}

/*************************************************/
/*  Dejtah - Myslici algoritmus (jeho volani)    */
/*************************************************/
int DejTah(TUloha *uloha, TTah1 *t1, u32 cas, int maxHloubka, int *PrijmiRemis, int *VzdejSe) {
  TTah1 *pt0,*pt1,*pt2,tmp;
  s16 alfa,beta,h,stara,RemCena=0;
  int rozdil;
  int Vzdavam;
#ifdef HTML_VYPISY
  int tmp_oh;
  int hloubka_oh;
  char posledniTah[20];
  char varianta[64];
  char odkaz[64];
  FILE *soub = otevriHtmlLog(uloha, "koren");
  if (soub) fputs("DejTah, propocet z korene.<BR>\n", soub);
#endif
				
  if (PrijmiRemis) *PrijmiRemis=0;
  if (VzdejSe) {
    *VzdejSe=0;
    Vzdavam=1;
  } else Vzdavam=0;

  if (maxHloubka < 1 || maxHloubka > MaxHloubkaPropoctu - 4)
    maxHloubka = MaxHloubkaPropoctu - 4;

#if Typ_Produktu==Projekt
/* Pro specialni urovne zkratim dobu mysleni. Delam to trosku
nedeterministicky, aby to nehralo porad stejne. */
  switch(PrjData.typ){
  case 1: maxhloubka=4;
   if(!(rand()%4))maxhloubka++; else  if(!(rand()%10))cas=10;
   if(cas>1000) cas=1000; break;
  /* Opice */
  case 2: maxhloubka=5;
    if(!(rand()%4))maxhloubka++; else
    if(!(rand()%4))maxhloubka--;
   if(cas>1000) cas=1000; break;
  /* Clovek */
  case 3: if(cas>1200) cas=800+rand()%400; break;
  /* Prumer */
  case 4: if(cas>3500) cas=2500+rand()%1000; break;
  /* Mistr */
  }

  if(PrjData.typ > 2) /*Sekretarka a normal neznaji zahajeni*/
#endif /* projekt */
  if (MamKnihovnu && (t1->data = KnihDejTah(&(uloha->pozice)))) {
#ifdef HTML_VYPISY
    if (soub)
      fprintf(
        soub,
        "Pozice je v knihovne zahajeni, vracim %x.<BR>\n", t1->data);
    zavriHtmlLog(soub);
#endif
    /*Kdyz jsem nasel pozici v knihovne, hned koncim.*/
	  return 1;
  }
 
  if(t1->data=TabDejTah(uloha)){
#ifdef HTML_VYPISY
    if (soub)
      fprintf(
        soub,
        "Pozice je v knihovne zahajeni, vracim %x.<BR>\n", t1->data);
    zavriHtmlLog(soub);
#endif
   /*Kdyz jsem nasel pozici v databazi koncovek, hned koncim.*/
	 return 1;
  }
 
  uloha->StavPropoctu.MamKoncitMysleni=0;
  NastavCas(cas,uloha);
  InitVypocet(uloha);
#ifdef Vypisy
  printf("\nFunkce DejTah. Na tahu je %s. BM=%i, CM=%i\n",
		uloha->pozice.bily ? "bily" : "cerny",
 (int) uloha->zasobnik.bm[0],(int) uloha->zasobnik.cm[0]);
#endif
 if (PrijmiRemis) {
  if(uloha->pozice.bily && uloha->zasobnik.bm[0]<=StdCenyFigur[6] ||
     !uloha->pozice.bily && uloha->zasobnik.cm[0]<=StdCenyFigur[6])
    *PrijmiRemis=1; else *PrijmiRemis=0;
  }
  NalezTahy(uloha);
  JenPripustne(uloha);
 /* To je sice pomale, ale nutne, nebot se mi nemusi podarit dopocitat
   do hloubky 1 a pak bych mohl vratit nepripustny tah.
   Kdyz to ale zavolam jen jednou za cely vypocet nejlepsiho tahu,
   tak to nezdrzuje. Ale v rekurzivnim propoctu pocitam i s ilegalnimi tahy */
  if (!uloha->zasobnik.hranice[1]) {
    /*Neni zadny pripustny tah*/
#ifdef HTML_VYPISY
    if (soub) fputs("Zadny pripustny tah", soub);
    zavriHtmlLog(soub);
#endif
    return 0;
  }
  
  if (uloha->zasobnik.hranice[1] == 1){
#ifdef Vypisy
    puts("Je jen jeden pripustny tah");
#endif
#ifdef HTML_VYPISY
    if (soub) {
      char s[10];

      TahToStr(uloha->zasobnik.tahy[0].data, uloha, s);
      fprintf(soub, "Jediny pripustny tah %s.", s);
    }
    zavriHtmlLog(soub);
#endif
    Vzdavam=0;
    goto zaver;
  }
/* tedy mám alespoò dva tahy*/
  pt0 = pt1 = uloha->zasobnik.tahy;
  pt2 = uloha->zasobnik.tahy + uloha->zasobnik.hranice[1];
  SetridTahy(uloha);
#if Typ_Produktu==Win32_Program
  VypAktualizujNejlepsi(uloha, pt0->data);
#endif
 
  if (HodnotaPozice(uloha, -mat, mat) >= -250) Vzdavam = 0;
 /* HodnotaPozice musím zavolat kvùli nastavení tabulek.
 Navíc mi to pomù¾e pøi rozhodování,
 zda se mám vzdát. Nevzdávám se jen tehdy, pokud HodPos >= -vì¾
 nebo propoèet do aspoò jedné hloubky >= -vì¾ nebo mám materiálu za
 dámu. */
  UlozStav(uloha);

  for(h = 0; h < maxHloubka; h++) {
#ifdef HTML_VYPISY
    if (soub)
      fprintf(soub, "<H2>Propocet do hloubky %i</H2>\n", h);
    hloubka_oh = Stav(uloha);
#endif
		
#if 0
#ifdef Vypisy
    memset((void *)BETA_POCET,0,sizeof(BETA_POCET));
    memset((void *)BETA_PORADI,0,sizeof(BETA_PORADI));
    memset((void *)BETA_CELKEM,0,sizeof(BETA_CELKEM));
    memset((void *)ALFA_POCET,0,sizeof(ALFA_POCET));
    memset((void *)ALFA_PORADI,0,sizeof(ALFA_PORADI));
    memset((void *)ALFA_CELKEM,0,sizeof(ALFA_CELKEM));
#endif
#endif
		
#if Typ_Produktu==Win32_Program
    VypAktualizujHloubku(uloha,h);
#endif

    if (h) {
      pt0 = pt1 = uloha->zasobnik.tahy;
      pt2 = uloha->zasobnik.tahy + uloha->zasobnik.hranice[1];
      if (pt1->cena > mat - 1000) {
        alfa = pt1->cena - 10;
        beta = MIN(pt1->cena + 10, mat);
      } else
      if (pt1->cena < -mat + 1000) {
        alfa = MAX(pt1->cena - 10, -mat);
        beta = pt1->cena + 10;
      } else
      if (h & 1) {
        alfa = (s16)(pt1->cena - rozdil - 10);
        beta = (s16)(pt1->cena + rozdil);
      } else {
        alfa = (s16)(pt1->cena - rozdil);
        beta = (s16)(pt1->cena + rozdil + 10);
      }
    } else {
       /* pt0 1 a 2 uz mám nastavené*/
      alfa = -mat;
      beta = mat;
      rozdil = 20;
    }
#ifdef Vypisy
    printf("\nPropocet do hloubky %i, alfa=%i, beta=%i: ",(int) h,(int) alfa, (int) beta);
#endif
#ifdef HTML_VYPISY
    if (soub) fprintf(soub, " alfa = %i, beta = %i<BR>", (int) alfa, (int) beta);
#endif

   /* Cyklus pøes tahy*/
    for(; pt1 < pt2; pt1++) {
      uloha->hvar[h][h] = 0;
#ifdef Vypisy
      {
        char s[20];
        TahToStr(pt1->data, uloha, s);
        printf("%s", s);
        fflush(stdout);
      }
#endif
#ifdef HTML_VYPISY
      {
        TahToStr(pt1->data, uloha, posledniTah);
        sprintf(varianta, "%i" ODDELOVAC_VARIANT "%s" ODDELOVAC_VARIANT, h, posledniTah);
        sprintf(odkaz,  "%skoren.html", varianta);
        if (soub) fprintf(soub, "<A HREF=\"%s\">%s</A> ", odkaz, posledniTah);
      }
#endif

#if Typ_Produktu==Win32_Program
      VypAktualizujTah(uloha,pt1->data);
#endif

      Tahni(pt1->data,uloha);
#ifdef HTML_VYPISY
      tmp_oh = Stav(uloha);
#endif
      pt1->cena = (s16)-AlfaBeta(uloha,(s16)(-beta),(s16)(-alfa), h
#ifdef HTML_VYPISY
        , varianta
#endif
        );
#ifdef HTML_VYPISY
      if (soub) fprintf(soub, "(%i) ", Stav(uloha) - tmp_oh);
#endif
      TahniZpet(pt1->data,uloha);
      tmp = *pt1;
      if (tmp.cena >= -250) Vzdavam = 0;
      if (uloha->StavPropoctu.MamKoncitMysleni)	{
        goto ZaverKoncimMysleni;
      }

      if (pt1 == pt0) { /* pocítám první tah*/
        RemCena=pt1->cena;
#if Typ_Produktu==Win32_Program
        VypAktualizujCenu(uloha,pt1->cena);
#endif
        if (pt1->cena <= alfa) {
	    /* První tah podtekl alfa. Poèítaè zaèíná tusit nestìstí.
	    Nejspís by podtekly i vsechny dalsí tahy*/
#ifdef HTML_VYPISY
          if (soub) fputs("alfa podteceni, pocitam znovu \n", soub);

        sprintf(varianta, "%i" ODDELOVAC_VARIANT "%s_alfa" ODDELOVAC_VARIANT, h, posledniTah);
        sprintf(odkaz,  "%skoren.html", varianta);
          
          if (soub) fprintf(soub, "<A HREF=\"%s\">%s</A> ", odkaz, posledniTah);
#endif
          Tahni(pt1->data,uloha);
#ifdef HTML_VYPISY
          tmp_oh = Stav(uloha);
#endif
	        pt1->cena = (s16)-AlfaBeta(uloha, (s16)(-alfa - 1), (s16)(mat), h
#ifdef HTML_VYPISY
        , varianta
#endif
            );
#ifdef HTML_VYPISY
      if (soub) fprintf(soub, "(%i) ", Stav(uloha) - tmp_oh);
#endif
	        TahniZpet(pt1->data,uloha);
          if (uloha->StavPropoctu.MamKoncitMysleni)	{
            ZaverKoncimMysleni:
#ifdef HTML_VYPISY
            if (soub) fputs("Dosel cas<BR>\n", soub);
#endif
            goto zaver;
          }

#ifdef Vypisy
          printf(" Alfa podteceni");
#endif
          
          RemCena=pt1->cena;
#if Typ_Produktu==Win32_Program
          VypAktualizujCenu(uloha,pt1->cena);
#endif
        } else
        if (pt1->cena >= beta) {
        /*První tah pøetekl beta*/
      	  KopirujHVar(uloha, pt1->data);
#ifdef HTML_VYPISY
          if (soub) fputs("beta preteceni, pocitam znovu \n", soub);

          sprintf(varianta, "%i" ODDELOVAC_VARIANT "%s_beta" ODDELOVAC_VARIANT, h, posledniTah);
          sprintf(odkaz,  "%skoren.html", varianta);
			
          if (soub) fprintf(soub, "<A HREF=\"%s\">%s</A> ", odkaz, posledniTah);
#endif
      	  Tahni(pt1->data,uloha);
#ifdef HTML_VYPISY
          tmp_oh = Stav(uloha);
#endif
          pt1->cena = (s16)-AlfaBeta(uloha, (s16)(-mat), (s16)(1 - beta), h
#ifdef HTML_VYPISY
              , varianta
#endif
            );
#ifdef HTML_VYPISY
          if (soub) fprintf(soub, "(%i) ", Stav(uloha) - tmp_oh);
#endif
          TahniZpet(pt1->data,uloha);
          if (uloha->StavPropoctu.MamKoncitMysleni)	goto ZaverKoncimMysleni;
#ifdef Vypisy
          fputs(" Beta preteceni", stdout);
#endif
          RemCena = pt1->cena;
#if Typ_Produktu==Win32_Program
          VypAktualizujCenu(uloha,pt1->cena);
#endif
        }
        alfa = pt1->cena;
        if (alfa >= mat - h) {
#ifdef Vypisy
          printf(" Davam mat %i. pultahem",mat - alfa + 1);
#endif
#ifdef HTML_VYPISY
          if (soub) fprintf(soub, " Davam mat %i. pultahem.<BR>\n", mat - alfa + 1);
#endif
          goto zaver;
        }
        beta = (s16)(alfa + 1);
        if (uloha->StavPropoctu.MamKoncitMysleni) goto ZaverKoncimMysleni;
#ifdef Vypisy
        printf(" %i ",(int) pt1->cena);
        printf("a=%i, b=%i ",(int) alfa, (int) beta);
#endif
#ifdef HTML_VYPISY
        if (soub) fprintf(soub, " %i ", (int)pt1->cena);
        if (soub) fprintf(soub, "a=%i, b=%i ", (int) alfa, (int) beta);
#endif
      } /* od prvního tahu */ else {
      /* jiný nez první tah */
#ifdef Vypisy
        printf(" %i ",(int) pt1->cena);
#endif
#ifdef HTML_VYPISY
        if (soub) fprintf(soub, " %i ", (int)pt1->cena);
#endif
        tmp = *pt1;
        if (tmp.cena > alfa) {
      	  memmove(pt0 + 1, pt0, (pt1 - pt0) * sizeof(TTah1));
          *pt0 = tmp;
      	  alfa = tmp.cena;
#if Typ_Produktu==Win32_Program
          VypAktualizujNejlepsi(uloha, pt0->data);
          VypAktualizujCenu(uloha, pt0->cena);
#endif
          RemCena = pt0->cena;
#ifdef Vypisy
          printf("dobry ");
          printf("a=%i, b=%i ", beta - 1, mat);
#endif
#ifdef HTML_VYPISY
          if (soub) fputs("dobry ", soub);
          sprintf(varianta, "%i" ODDELOVAC_VARIANT "%s_dobry" ODDELOVAC_VARIANT, h, posledniTah);
          sprintf(odkaz,  "%skoren.html", varianta);
          if (soub) fprintf(soub, "<A HREF=\"%s\">%s</A> ", odkaz, posledniTah);
#endif
          KopirujHVar(uloha, tmp.data);
          Tahni(tmp.data, uloha);
#ifdef HTML_VYPISY
          tmp_oh = Stav(uloha);
#endif
          pt0->cena = (s16)-AlfaBeta(uloha, (s16)(-mat), (s16)(1 - beta), h
#ifdef HTML_VYPISY
            , varianta
#endif
          );
#ifdef HTML_VYPISY
      if (soub) fprintf(soub, "(%i) ", Stav(uloha) - tmp_oh);
#endif
          TahniZpet(tmp.data,uloha);
          if (pt0->cena >= -250) Vzdavam = 0;
          if (uloha->StavPropoctu.MamKoncitMysleni) goto ZaverKoncimMysleni;
#if Typ_Produktu==Win32_Program
          VypAktualizujCenu(uloha, pt0->cena);
#endif
          RemCena = pt0->cena;
          if (pt0->cena >= mat - h) {
#ifdef Vypisy
            printf(" Davam mat %i. pultahem",mat - pt0->cena + 1);
#endif
#ifdef HTML_VYPISY
            if (soub) fprintf(soub, " Davam mat %i. pultahem",mat - pt0->cena + 1);
#endif
            goto zaver;
          }
          alfa = (s16)(pt0->cena);
          beta = (s16)(alfa + 1);	
#ifdef Vypisy
          printf(" %i ", pt0->cena);
          printf("a=%i, b=%i ", (int)alfa, (int)beta);
#endif
#ifdef HTML_VYPISY
          if (soub) fprintf(soub, " %i ", pt0->cena);
          if (soub) fprintf(soub, "a=%i, b=%i ", (int)alfa, (int)beta);
#endif
        } /* Od pøeteèení alfa */
      } /* Od jiného než prvního tahu*/
    } /*od cyklu pøes tahy*/
#ifdef HTML_VYPISY
      if (soub) fprintf(soub, " <P>\n%i", Stav(uloha) - hloubka_oh);
#endif

    if (alfa <= -mat + h) {
#ifdef Vypisy
  	  printf(" Dostanu mat %i. pultahem", mat + alfa + 1);
#endif
#ifdef HTML_VYPISY
      if (soub) fprintf(soub, " Dostanu mat %i. pultahem", mat + alfa + 1);
#endif
  	  break;
    }
    if (h) rozdil = abs(stara - pt0->cena) + 10;
    stara = pt0->cena;
    PISDOHISTHEUR(uloha,pt0->data,h*h)
  } /*Od cyklu pøes hloubku*/
zaver:
  *t1 = uloha->zasobnik.tahy[0];
#ifdef Vypisy
  putchar('\n');
#endif
  if (PrijmiRemis) {
    if (uloha->pozice.bily && uloha->zasobnik.bm[0] <= StdCenyFigur[6] ||
      !uloha->pozice.bily && uloha->zasobnik.cm[0] <= StdCenyFigur[6])
      *PrijmiRemis = 1; else *PrijmiRemis = 0;
  }
  if (RemCena < -50 && PrijmiRemis) *PrijmiRemis = 1;
  if (Vzdavam) *VzdejSe = 1;
#ifdef HTML_VYPISY
  if (soub) fprintf(soub, "\n<BR>\n(%i)", Stav(uloha));
  zavriHtmlLog(soub);
#endif
  return 1;
} /* od funkce */
