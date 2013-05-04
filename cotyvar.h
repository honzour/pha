/*********************************************************/
/* cotyvar.h - definice zakladnich typu a konstant       */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#ifndef cotyvarH
#define cotyvarH
#include <time.h>
#include "volby.h"
/******************************************/
/*       Definice ciselnych typu          */
/******************************************/
#define s8 signed char
#define u8 unsigned char
#define s16 signed short
#define u16 unsigned short
#define s32 signed int
#define u32 unsigned int
/*#define bool int
#define true 1
#define false 0*/
#define KontrolaTypu  if (sizeof(s8) != 1 || sizeof(u8) != 1||\
     sizeof(s16) != 2 || sizeof(u16) != 2||\
     sizeof(s32) != 4 || sizeof(u32) != 4)\
  Chyba("Chyba: spatne nastavene ciselne typy");

typedef s8 TSch[12*10];
 /* Klasicka sachovnice 12*10 */
typedef struct {
u8 roch;  /* binarne 00...00vmVM */
	   /* V,v - moznost velke rosady bileho a cerneho
	      M,m - totez s malou  */
u8 mimoch;  /* Pole, na nemz stoji pesec tahnuvsi v predchozim tahu o 2,
	     nebo 0 pokud se minule netahlo pescem o 2 */
 int bily;    /* Je bily na tahu ?*/
 TSch sch;} TPozice;
#define MaxHloubkaPropoctu 30
/* Horni odhad delky vetve stromu propoctu - kvuli velikosti
   datovych struktur */
#define TahuZPozice 64
/* Tohle neni zadny horni odhad. Jen kvuli velikosti zasobniku
 tahu.*/
#define HloubkaZasobnikuTahu (TahuZPozice*MaxHloubkaPropoctu)
/* Kolik tahu na zasobniku */
typedef struct {u16 data;  s16 cena;} TTah1;
/* Format TTah1::data:   def.: Nenormalni tah je
1) rosada
2) promena pesce
3) brani mimochodem
  def.: Normalni tah je kazdy tah, ktery neni nenormalni.
   Zbesily format TTah1.data je dusledkem me snahy o malou velikost TTah1
   a efektivni rutiny zejmena pro normalni tahy. (Nebot se s timto typem
   pocita v rekurzi a normalni tahy jsou skoro vsechny)
   binarne  1234567812345678
	        nmoooooookkkkkkk (normalni tah a brani mimochodem)
	        nmrcv00000000000 (rosada)
	        nmrcppoookkk0000 (promena pesce)
n - Je to nenormalni tah ?
  Pokud ne (tj. tah je normalni), je m=0 (MUSI byt 0) a ooooooo a kkkkkkk
   ma vyznam poli odkud a kam se tahne.
  Pokud ano (tj. tah je nenormalni):
   m - Je to rosada nebo promena pesce ?
    Pokud ne (tj. je to brani mimochodem) ma ooooooo a kkkkkkk tak‚ vyse
     uvedeny vyznam.
   Pokud ano:
    r - Je to rosada ?
     Pokud ano, potom
      c: je to rosada cerneho ? v: je to velka rosada ?
     Pokud ne (tj. je to promena pesce)
      c: je to promena cerneho pesce ?
      pp: v co se pesec meni 0=kun, 1=strelec, 2=vez, 3=dama
      ooo: cislo, ktere se musi pricist k A7 (je-li to promena bileho pesce)
	   nebo A2 (je-li to promena cerneho pesce), abychom dostali pole,
	   odkud pesec tahne
      kkk: totiz s polem promeny, jen misto A7 (resp. A2) se bere  A8
       (resp A1)

Presne v duchu predchozi definice si napisu konstanty pro rosady */
#define MBRoch (7<<13)
#define VBRoch ((7<<13)|(1<<11))
#define MCRoch (15<<12)
#define VCRoch (31<<11)
/**
  Jeden prvek partie.
*/
typedef struct {
    /*data tahu*/
  u16 t;
  /*stav rosad pred zahranim - viz TPozice*/
  u8 roch;
  /*stav mimochodem pred zahranim - viz TPozice*/
  u8 mimoch;
  /*ktera figurka je tahem sebrana*/
  s8 brani;
  /*bylo tazeno pescem nebo bylo brani*/
  u8 zmena;
  /*kvuli detekci opakovani*/
  u32 hashF;
#ifdef STRINGTAH
  /*Kvùli partiáøi*/
  char nazev[15];
#endif
} TPartData;

typedef struct tpartie{
 struct tpartie *l;
 struct tpartie *p;
 TPartData data;} TPartie;
 /*Partie je obousmerny spojak tahu. Prvni uzel je nevyuzity
 - to abych mohl byt ve stavu pred zahranim tahu.*/
typedef struct{
 int VNT; /*Jsem v metodì nulového tahu ? Kolikrat vnorene ?*/
 int NulTah; /* Smim pouzit metodu nuloveho tahu. (Napr.
 v koncovce (nebo pri ladeni :-)) ne. */
 time_t sKonceMysleni; /*Kdy mam koncit*/
 int msKonceMysleni;
 volatile int MamKoncitMysleni;
 /*Je-li true, prestane se pocitat a vylezu z rekurze*/
} TStavPropoctu;

typedef struct {
  u32 DveNaXHash; /*Kdyz na to umocnim 2, mam velikost tabulky*/
  u32 DveNaXHashPech;/*Totez pro pescovou strukturu*/
  u32 DveNaXHashNejlepsi; /*Totez pro hash nejlepsiho*/
  int PrepisujHash;/*Mam prepisovat starsi ale hlubsi ? - normalne ano*/
} THashCfg; /*Nastaveni hash tabulek */


/** 
  Zasobnik tahu.
  Je-li zasobnik vynulovany byte po bytu, je "spravne prazdny"
*/
typedef struct {
   /** Tahy */
  TTah1 tahy[HloubkaZasobnikuTahu];
  /** Pocatky skupin tahu hranice[0] by mela byt vzdy 0 */
  s16 hranice[MaxHloubkaPropoctu]; 
  /** Prvni volna skupina (je-li 0, je zasobnik prazdny),
      ukazuje take hloubku zanoreni aktualni pozice ve stromu propoctu*/
  s16 pos;
  /** Pole, na nemz stoji pesec tahnuvsi v predchozim tahu o 2 pokud zaroven
      na vedlejsim poli je souperuv pesec,
      nebo 0 pokud se minule netahlo pescem o 2 a nebo se o 2 tahlo, ale souperuv
      pesec neciha. */
  s8 mimoch[MaxHloubkaPropoctu];
  /** Stavy-moznosti rochovat */
  u8 roch[MaxHloubkaPropoctu];
  /** Sebrane figury tj. jaka figura byla sebrana tahem z teto pozice
    (nebylo-li tazeno, nezajimave)*/
  s8 brani[MaxHloubkaPropoctu];
  /** hodnota materialu bileho, hodnota materialu cerneho */  
  s16 bm[MaxHloubkaPropoctu],cm[MaxHloubkaPropoctu];
  /** hodnoty pozice (nezahrnuje material) z hlediska bileho */  
  s16 hodpos[MaxHloubkaPropoctu];
  /** hodnoty hash funkce HashF */
  u32 hF[MaxHloubkaPropoctu];
  /** hodnoty hash funkce HashF */
  u32 hG[MaxHloubkaPropoctu];
  /** hodnoty hash funkce HashF */
  u32 hPechF[MaxHloubkaPropoctu];
  /** hodnoty hash funkce HashF */
  u32 hPechG[MaxHloubkaPropoctu];
  /** Cilova policka tahu - kvuli prohlubovani */
  u8 kam[MaxHloubkaPropoctu];
  /** Je hrac na tahu v sachu ? */
  u8 sach[MaxHloubkaPropoctu];
  /** ke kolika nevratnym zmenam (tah pescem nebo brani) doslo
  na ceste z korene. Jeden tah - beznem brani pescem se
  pocita jako dve zmeny, to ale nevani.*/
  u8 zmen[MaxHloubkaPropoctu];
  /** Ke kolika rozsirenim doslo na ceste z korene. 
  Neuvazuje se rozsireni pri jedinem pripustnem
  tahu.*/
  u8 rozsir[MaxHloubkaPropoctu];
  /** K jakym druhum rozsireni doslo .*/
  u8 druhRozsir[MaxHloubkaPropoctu];
} TZasobnikTahu;


#define ROZSIR_NE 0
#define ROZSIR_FISCHER 1
#define ROZSIR_DOBRANI 2
#define ROZSIR_PESEC_7 4
#define ROZSIR_PREDSTAVENI 8
#define ROZSIR_HROZBA_DAMOU 16
#define ROZSIR_VIDLE 32


typedef struct{
	u32 kod; /* hash funkce pozice */
	s16 cena; /* cena pozice z hrace na tahu... */
	u8 hloubka; /* ....pri propoctu do hloubky hloubka */
	u8 priznak; /* xxxxxxHD
	 D - pozice je stejná nebo lepší než cena
	 H - pozice je stejná nebo horší než cena 
	(=> H&D - pozice má pøesnì danou hodnotu) */
} THashPrvek;

typedef struct {
	u32 kod;
	s16 cena;
	u8 b[8]; /* kde stoji bili pesci, je-li jich mene nez 8, je za poslednim 0*/
	u8 c[8]; /* kde stoji cerni pesci, je-li jich mene nez 8, je za poslednim 0*/
	u8 bt[8]; /*typ bilych pescu 0|0|izolovany|volny|kryty|vedle|opozdeny|blokovany*/
/*Opozdeny pesec je takovy, ze neni kryty pescem a policko pred nim je vicekrat
  napadeno pesci nez pokryto pesci a neni na nem pesec
 */
	u8 ct[8]; /*typ cernych pescu 0|0|izolovany|volny|kryty|vedle|opozdeny|blokovany*/
	u8  bb; /*bili na bilych polich (pocet) blokovani 2x*/
	u8  bc; /*bili na cernych polich (pocet) blokovani 2x*/
	u8  cb; /*cerni na bilych polich (pocet) blokovani 2x*/
	u8  cc; /*cerni na cernych polich (pocet) blokovani 2x*/
	u8  centr; /* pesci v centru (sloupce c az f)
	blokovani se pocitaji dvakrat*/
	u8  otsloupec; /*otevrene sloupce*/
} THashPech; /* Tohle ukladam do hash tabulky pescove struktury*/

/**
  Nastaveni algoritmu.
*/
typedef struct{
	s16 MaxPosZmena; 
/* Maximalni zmena pozicni slozka hodnoty pozice
   za jeden pultah. Uziva se k orezani ohodnocovaci
   funkce. (Aby se neprovadela oh. f. v pozicich se
   ztratou materialu.)
	*/
	s16 PesecKryty;/*Bonus za pesce kryteho jinym pescem*/
	s16 PesecVedle;/*Bonus za pesce, ktery ma vedle sebe
	jineho (Napr. e2 a d2)*/
	s16 PesecE2Blok; /*Pesec na e2/e7 a na e3/e6 je kamen*/
	s16 PesecD2Blok; /*Pesec na d2/d7 a na e3/e6 je kamen*/
	s16 DvojPesec; /*Postih za pesce, pred kterym je alespon
	jeden kolega*/
	s16 PesecIzolovany;/*Kdyz na sousednich sloupcich neni
	 pesec teze barvy*/
	s16 PesecVolny;/*Neni blokovan zadnym pescem a postoupi-li
	o 1, nebude ho moci sebrat zadny souperuv*/
	s16 PesecSlaby;/*Neni blokovan pescem ani kryty ani volny*/
	s16 SlabyPesecNapaden; /*Bonus za napadeni slabeho pesce
	(zatim jen vezi a kralem v koncovce)*/
	s16 VezNaSloupci;
	s16 hh1Bonus;
  s16 hh2Bonus;
	s16 FischeruvStrelec; /*Strelec v pasti na a2/h2 a7/h7*/
	/*Rozsirovaci meze - kdyz jsem provedl n+1 rozsireni,
	nebudu uz delat dalsi (n zavisi na typu rozsireni)*/
	u8 MaxRozsirSach; /* Rozsireni jsem-li v sachu*/
	u8 MaxRozsirPredstav; /* Rozsireni pri kryti sachu predstavenim*/
	u8 MaxRozsirDober;/* Rozsireni pri brani berouci figury*/
	u8 MaxRozsirPesec7;/* Rozsireni v listu pri postupu pesce na 7. / 2. radu*/
	u8 MaxRozsirFischer;/* Rozsireni pri pasti na strelce (1. partie Spasskij - Fischer)*/
	u8 MaxHloubkaABB; /*Dopocet do tiche pozice*/
	u8 PovolHash; /*Ma se vubec uvazovat hash tabulka ?*/
	u8 PovolPechHash; /*Ma se vubec uvazovat hash tabulka pescu ?*/
  u8 PovolNullTah; /*Povolit nulovy tah*/
} TAlgKoef;

typedef struct{
  TAlgKoef AlgKoef;
  /*Jednoduche (ne pole) koeficienty*/
  s16 bp[89],cp[89];
  /*bily, cerny pesec*/
  s16 bpv[89],cpv[89];
  /*bily, cerny volny pesec - aditivum k bp a cp*/
  s16 bkz[99], ckz[99], kk[99], kkb[99], kkc[99];
  /*bily, cerny kral v zahajeni, libovolny kral v koncovce,
  kral tlaceny do bileho a cerneho kouta*/
  s16 bj[99],cj[99];
  /*bily, cerny kun*/
  s16 bs[99],cs[99];
  /*bily, cerny strelec*/
} TAlgCfg;

typedef struct {
/* Na kterem sloupci jsou bili/cerni pesci*/
 u8 BPSloupec[8];
 u8 CPSloupec[8];
/* u8 BVSloupec[8];
 u8 CVSloupec[8];*/
 u8 b[5]; /*Pocet figur bileho - pesec..dama*/
 u8 c[5]; /*Totez pro cerneho*/
 u8 bk, ck; /*Kde jsou kralove*/
} TMaterial;

typedef u8 TVazby[8];
/*napr. NalezBileVazby spravne vyplni, obshuje policka.
Neni-li jich vsech 8, je za posledni 0*/

typedef u32 TRemPol[101];

/*Tabulka pozic z partie, ktere mohou nastat v propoctu
(kvuli detekci remiz v propoctu)*/
typedef struct {
 TRemPol RemPol;
 int poc;
} TRemTab;


typedef struct {
  u32 MS,OH,OH2,OHP,UZ, TA;
  u32 NTU[MaxHloubkaPropoctu],
  NTN[MaxHloubkaPropoctu],
  AB[MaxHloubkaPropoctu],
  PR[MaxHloubkaPropoctu],
  ABB[MaxHloubkaPropoctu],
  HU[MaxHloubkaPropoctu],
  BETA_PORADI[MaxHloubkaPropoctu],
  BETA_POCET[MaxHloubkaPropoctu],
  BETA_CELKEM[MaxHloubkaPropoctu],
  ALFA_PORADI[MaxHloubkaPropoctu],
  ALFA_POCET[MaxHloubkaPropoctu],
  ALFA_CELKEM[MaxHloubkaPropoctu];
} TStatistika;


/* 
   TUloha.
   Je treba si pripravit reentrantnost. Mozna nekdy v budoucnu
   bude vysledek dll (pro wokna nebo unix),
   takze vsechny promenne, ktere by jinak byly globalni, si zabalim do
   jedne strukturky (pak budu mit seznam tehto struktur) */
typedef struct {
  TPozice pozice; /*V ni probiha propocet*/
  TZasobnikTahu zasobnik;/*Zasobnik pro rekurzivni propocet*/
  TRemTab RemTab;/*Tabulka pozic z partie, ktere mohou nastat v propoctu
                  (kvuli detekci remiz v propoctu)*/
  TMaterial material;/*Inkrementalne pocitane veci, hlavne material*/
 
  /*
  Historicka heuristika
  Tah, ktery byl v jedne vetvi propoctu dobry, bude nejspis dobry
  i v jine vetvi na stejne urovni. [propocet][1. nejlepsi, 2. nejlepsi]
  Prida se jako bonus. (Uziti - v trideni po vygenerovani)
  */
  u16 hh[MaxHloubkaPropoctu][2];
 
  /* Jina hist. heur. Pole je indexovane primo kody tahu,
  kdyz je nejaky nejlepsi dostane bonus. (Uziti - v trideni po vygenerovani)*/
  u8 HistHeur[1<<16];
 
  /*Co nalezi k hlavni variante (dalsi bonus)*/
  u16 hvar[MaxHloubkaPropoctu][MaxHloubkaPropoctu];
 
  /*stav nuloveho tahu a ukoncovani*/
  TStavPropoctu StavPropoctu;
 
  /*nastaveni hash tabulek*/
  THashCfg HashCfg;
 
  /*partie*/
  TPartie *prt;
 
  /*je partie ukoncena*/
  int KonecPartie;
 
  /*hlavni hash tabulka*/
  THashPrvek *ht;
 
  /*tabulka nejlepsich tahu*/
  THashPrvek *ntt;
 
  /*hash tabulka struktury pescu*/
  THashPech *hpt;
 
  /*nastaveni algoritmu*/
  TAlgCfg AlgCfg;
 
  /*kde jsou jake vazby (kvuli spec. pripadum v generatoru)*/
  TVazby Vazby;

#ifdef Statistika
  TStatistika stat; 
#endif
} TUloha;

#define mat 30000
/*Konstanta pro hodnotu pozice*/
extern const TPozice ZakladniPostaveni;
/*inicializacni konstantni promenna*/
extern const TAlgKoef STDAlgKoef;
/*inicializacni konstantni promenna*/
#define MAX(X,Y)((X)>(Y)?(X):(Y))
#define MIN(X,Y)((X)<(Y)?(X):(Y))
/*obecne uzitecna makra*/
#endif
