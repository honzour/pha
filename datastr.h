/* Sachové centrum - Knihovna zahájení Petr Kučera 2000*/
/** Deklarace typů datových struktur použitých k reprezentaci
    knihovny zahájení v paměti.
  */

#ifndef __DATASTR_H__
#define __DATASTR_H__ 1

#include "cotyvar.h"

/** HLOUBKA_ADRESARE - určuje hloubku jedné úrovně adresáře. */
#define HLOUBKA_ADRESARE 4
/** POCET_ADRES_V_BLOKU - určuje počet adres v jednom bloku adresáře. */
#define POCET_ADRES_V_BLOKU 16 /*2^4*/
/*Konstanta určující masku pro odmaskování prvních HLOUBKA_ADRESARE bitů
 */
#define ZAKLADNI_MASKA 15

/*určuje, jestli mají být pozice v souboru pakovány, nebo má být ukládán
 přímo datový typ TPozice*/
#define PAKUJ_POZICE 1

/** tah_t - popis tahu s ECO a váhou výběru, pokud je váha menší než 0,
    jde o dosud nespecifikovanou váhu.
*/
typedef struct _tah {
  /*váha výběru tahu*/
  char vaha;
  /*písmeno ze zápisu ECO pozice vzniklé tímto tahem*/
  char ECO_pismeno;
  /*číslo ze zápisu ECO pozice vzniklé tímto tahem*/
  char ECO_cislo;
  /*dvoubajtový popis tahu*/
  u16 tah;
  /*pořadí partie ve zdrojovém souboru knihovny, v níž byl tah naposledy
    přidáván.*/
  int cis_partie;
  /*Součet vah ze všech partií, v nichž byl tah přidáván*/
  int soucet_vah;
  /*1 pokud byl tah v poslední partii přidáván a jeho váha nebyla explicitně
    určena.*/
  int neurcena;
} tah_t;

/*Počáteční velikost pole tahy ve struktuře pozice_t*/
#define POZICE_TAHY_POCATECNI_VELIKOST 10
/*Přírůstek velikosti pole tahy ve struktuře pozice_t, při jejím překročení*/
#define POZICE_TAHY_DELTA 10

/*Konstanta určující, že ECO pozice není implicitní A00*/
#define NENI_IMPL_A00 0
/*Konstanta určující, že ECO pozice je A00 implicitní v rámci knihovny*/
#define KNIH_IMPL_A00 1
/*Konstanta určující, že ECO pozice je A00 implicitní pouze v rámci dané partie*/
#define PART_IMPL_A00 2

/** pozice_t - struktura popisující pozici s tahy, které z ní vychází*/
typedef struct _pozice {
  /*Odkaz na další pozici v seznamu pozic*/
  struct _pozice *dalsi_pozice;
  /*Hašovací klíč pozice popsané touto strukturou*/
  u32 hasovaci_klic;
  /*písmeno ze zápisu ECO popisované pozice*/
  char ECO_pismeno;
  /*číslo ze zápisu ECO popisované pozice*/
  char ECO_cislo;
  /*Určuje, jestli ECO pozice je implicitní A00,
    je-li NENI_IMPL_A00, pak nejde o implicitní A00,
    je-li ECO_A00&KNIH_IMPL_A00, pak jde o implicitní A00 v rámci celé knihovny,
    je-li ECO_A00&PART_IMPL_A00, pak se jedná o A00 implicitní pouze v rámci dané partie
  */
  char ECO_A00;
  /*popisovaná pozice*/
  TPozice pozice;
  /*počet tahů vedoucích z této pozice*/
  int pocet_tahu;
  /*součet vah určených tahů v aktuální partii*/
  int soucet_vah;
  /*počet tahů s neurčenou vahou v současné partii*/
  int pocet_neurcenych;
  /*počet tahů s nenulovou vahou v současné partii*/
  int pocet_nenulovych;
  /*počet partií, v nichž se pozice vyskytla*/
  int pocet_partii;
  /*velikost pole tahy*/
  int vel_tahu;
  /*pole obsahující tahy vedoucí z této pozice*/
  tah_t *tahy;
  /*vypočtená pozice v souboru*/
  long poloha;
  /*číslo partie, v níž byl přidán poslední tah s nenulovou nebo neurčenou vahou,
    nebo -1, pokud tam takový tah není*/
  int part_nenul_tah;
} pozice_t;

/** blok_pozice_t - struktura popisující obsah bloku s pozicemi, které
    mají stejný hašovací klíč.
 */
typedef struct _blok_pozic {
  /*Počet pozic jež jsou uložené v této struktuře*/
  int pocet_pozic;
  /*Hašovací klíč společný všem pozicím uloženým v této struktuře*/
  u32 hasovaci_klic;
  /*Poloha bloku v souboru s knihovnou (vypočteno těsně před uložením)*/
  long poloha;
  /*Počet bytů, které blok v souboru bude zabírat*/
  long delka_bloku;
  /*Ukazatel na hlavu spojového seznamu s pozicemi, které jsou v tomto bloku uloženy.*/
  pozice_t *pozice;
} blok_pozic_t;

/** blok_adresare_t - struktura popisující obsah bloku s adresářem v paměti.*/
typedef struct _blok_adresare {
  /*Poloha bloku v souboru s knihovnou zahájení (vypočteno těsně před uložením).*/
  long poloha;
  /*
    Úroveň bloku ve struktuře popisující obsah knihovny, určuje, která skupina
    bitů v hašovacím klíči se použije při přechodu na další úroveň.
  */
  int uroven;
  /*
    Ukazatelé na bloky pozic, které jsou určeny hodnotou hašovacího klíče.
    Hašovací klíč je uložen implicitně pomocí indexu v poli, jde o 16 prvkové pole,
    na každé úrovni se tedy štěpí podle čtveřice bitů, pokud poskládáme příslušné
    čtveřice za sebou, dostaneme prefix klíče dlouhý 4*(uroven+1) bitů.
    Pokud je prvek pole NULL, znamená to buď, že pozice s klíčem začínajícím
    daným prefixem se dosud nevyskytla, nebo že je více použitých klíčů s daným
    prefixem, v tom případě musí být příslušný prvek pole adresy_adresaru ne NULL.
    Pokud je prvek ne NULL, je jediný klíč s daným prefixem, v tom případě musí
    být prvek v poli adresy_adresaru se stejným indexem NULL.
   */
  blok_pozic_t *adresy_pozic[POCET_ADRES_V_BLOKU];
  /*
    Ukazatele na bloky adresářů dalších úrovní v hierarchické struktuře popisující
    knihovnu zahájení.
   */
  struct _blok_adresare *adresy_adresaru[POCET_ADRES_V_BLOKU];
} blok_adresare_t;


/*Následující typy slouží k popisu stavu souboru.*/

/*typ popisující tah uložený do souboru*/
typedef struct _f_tah {
  /*Pravděpodobnost, s jakou má být tah vybírán*/
  char vaha;
  /*Písmeno z kódu ECO tahu*/
  char ECO_pismeno;
  /*Číslo z kódu ECO tahu*/
  char ECO_cislo;
  /*Dvoubajtový popis tahu*/
  u16 tah;
} f_tah_t;

/*Struktura popisující pozici uloženou v souboru s knohovnou zahájení.*/
typedef struct {
  /*
    Další pozice ve spojovém seznamu pozic z jednoho bloku.
    Skutečná adresa další pozice je ZAC_ADR+dalsi_pozice.
   */
  long dalsi_pozice;
  /*
    Stav hry popisovaný touto pozicí.
   */
  TPozice pozice;
  /*char ECO_pismeno;*/
  /*char ECO_cislo;*/
  /*Počet tahů, které vedou z této pozice.*/
  int pocet_tahu;
  /*Pole s tahy vedoucími z této pozice.*/
  f_tah_t *tahy;
} f_pozice_t;

/*Struktura popisující blok pozic uložený v souboru.*/
typedef struct _f_blok_pozic {
  /*Délka bloku v bytech (=sizeof(f_blok_pozic_t)+pocet_pozic*sizeof(f_pozice_t).*/
  long delka_bloku;
  /*Počet pozic uložených v tomto bloku.*/
  int pocet_pozic;
  /*Hašovací klíč společný všem pozicím v bloku.*/
  u32 hasovaci_klic;
  /*
    Pozice první pozice bloku v souboru - posun od začátku pozic.
    Skutečná adresa v souboru je ZAC_POZIC+adr_pozic.
   */
  long adr_pozic;
} f_blok_pozic_t;

/*Struktura popisující blok adresáře uložený v souboru.*/
typedef struct _f_blok_adresare {
  /*Poloha pole adresy v souboru. Skutečná adresa začátku pole je ZAC_POZIC+zac_adres.*/
  long zac_adres;
  /*Úroveň bloku v hierarchické struktuře adresáře.*/
  int uroven;
  /*
    Pole s adresami následujících bloků. Pokud je příslušný prvek 0, nenásleduje
    v jeho směru žádný další blok. Pokud má bitový zápis hodnotu 0<adresa>,
    jde o adresu bloku adres, pokud má tvar 1<adresa>, pak jde o adresu bloku
    pozic. Všechny adresy jsou relativní vzhledem k ZAC_ADR. Adresa 0 je tedy adresou
    základního bloku, na nějž se však nemůže vyskytnout odkaz v žádném bloku adres.
   */
  long adresy[POCET_ADRES_V_BLOKU];
} f_blok_adresare_t;

#if PAKUJ_POZICE

/*Struktura popisující typ TPozice (viz cotyvar.h) v souboru, je-li tento typ pakován*/
typedef struct {
  /*Stav rošády - jako v TPozice*/
  u8 roch;
  /*Stav pole mimochodem - jako v TPozice*/
  u8 mimoch;
  /*Je-li bílý na tahu - jako v TPozice*/
  int bily;
  /*
    Popis šachovnice - šachovnice 8*8 po řádcích - byte popisuje dvě následující pole.
    Obsah pole je zapsán jako 6+hodnota z TPozice.sch. 
  */
  char sch[32];
} f_tpozice_t;

/*
  Převede pozici ze struktury typu TPozice do struktury typu f_tpozice_t.

  parametr pozice vstupní pozice
  parametr fpozice výstupní pozice

  vrací 0 pokud nedojde k chybě, jinak kód chyby daný konstantami v souboru
          obkc.h
 */
int pakuj(TPozice *pozice, f_tpozice_t *fpozice);

/*
  Převede pozici ze struktury typu f_tpozice_t do struktury typu TPozice.

  parametr fpozice vstupní pozice
  parametr pozice výstupní pozice

  vrací 0 pokud nedojde k chybě, jinak kód chyby daný konstantami v souboru
          obkc.h
 */
int rozpakuj(f_tpozice_t *fpozice, TPozice *pozice);

#endif

/*Začátek bloků adresáře v souboru - poloha základního bloku.*/
extern long ZAC_ADR;
/*Začátek bloků s pozicemi v souboru.*/
extern long ZAC_POZIC;

/*
  Maska, která odfiltruje z prvku pole adresy strutkury typu f_blok_adres_t adresu.
  (Zamaskuje případnou 1 na začátku.)
 */
#define BL_MASKA ((long)(((long)1)<<(sizeof(long)*8-1)))

#include <stdio.h>

/*
  Porovná pozice a zjistí, jestli jsou shodné.

  parametr a první porovnávaná pozice
  parametr b druhá porovnávaná pozice

  vrací 1, jsou-li pozice stejné, jinak 0.
 */
int stejne_pozice(TPozice *a, TPozice *b);

/*
  Vrací znakový popis figury k číselnému.

  parametr fig číselný popis figury.
  vrací znak, který figuru popisuje, malé písmeno v případě černé figury,
        velké písmeno v případě bílé figury.
 */
char zn_fig(int fig);
/*
  Vypíše obsah pozice přečtené ze souboru.

  parametr file soubor, do nějž bude pozice vypsána
  parametr pozice vypisovaná pozice
  vrací 0, nedojde-li k chybě, jinak kód chyby daný konstantou v obk_ch.h
 */
int dump_f_pozice(FILE *file, f_pozice_t *pozice);

/*
  Vypíše pozici do souboru.

  parametr file soubor, do nějž bude pozice vypsána.
  parametr pozice vypisovaná pozice
  vrací 0, nedojde-li k chybě, jinak kód chyby daný konstantou v obk_ch.h
 */
int print_pozice(FILE *file, TPozice *pozice);

#endif
