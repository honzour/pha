/* Sachové centrum - Knihovna zahájení Petr Kučera 2000*/
/* Deklarace funkcí potřebných pro přístup do souboru s knihovnou zahájení
 */

#ifndef __CTENI_H__
#define __CTENI_H__ 1

#include <stdio.h>
#include "datastr.h"
#include "cotyvar.h"

/*Proměnná obsahující odkaz na čtený soubor s knihovnou zahájení*/
extern FILE *cteni_soubor;

/*Funkce inicializující modul čtení knihovny, nutno volat před
  prvním použitím některé ze zbylých funkcí čtecího modulu.

  parametr jmsoub jméno souboru obsahujícího knihovnu zahájení
  vrací 0 při úspěšné inicializaci, jinak kód chyby daný konstantami
        v souboru obk_ch.h
 */
int init_cteni(const  char *jmsoub);

/*Funkce přečte blok adresáře ze souboru s knihovnou zahájení

  parametr blok_adr ukazuje na místo, kam se má uložit přečtený blok adresáře
  parametr adresa obsahuje adresu čteného bloku v souboru vztaženou k aktuálnímu
           obsahu konstanty ZAC_ADR deklarované v souboru datastr.h. Adresa 0L vždy
           ukazuje na základní blok, z nějž vedou odkazy dál.
  vrací 0 při úspěšném čtení, jinak kód chyby daný konstantami
        v souboru obk_ch.h
 */
int cti_blok_adresare(f_blok_adresare_t *blok_adr, long adresa);

/*Funkce přečte blok s pozicemi ze souboru s knihovnou zahájení

  parametr blok ukazuje na místo, kam má být přečtený blok uložen
  parametr adresa obsahuje adresu čteného bloku v souboru vztaženou
           k aktuálnímu obsahu konstanty ZAC_POZIC deklarované
           v souboru datastr.h.
  vrací 0 při úspěšném čtení, jinak kód chyby daný konstantami
        v souboru obk_ch.h
 */
int cti_blok_pozic(f_blok_pozic_t *blok, long adresa);

/*Funkce přečte pozici ze souboru s knihovnou zahájení

  parametr blok ukazuje na místo, kam má být přečtená pozice uložena
  parametr adresa obsahuje adresu čtené pozice v souboru vztaženou
           k aktuálnímu obsahu konstanty ZAC_POZIC deklarované
           v souboru datastr.h. 
  vrací 0 při úspěšném čtení, jinak kód chyby daný konstantami
        v souboru obk_ch.h
 */
int cti_pozice(f_pozice_t *pozice, long adresa);

/*Nalezne pozici v souboru s knihovnou zahájení.

  parametr pozice obsahuje pozici, která má být nalezena v souboru
  parametr f_pozice ukazuje na místo, kam má být uložena přečtená
           pozice
  vrací 0, nedojde-li k žádné chybě a pozice je úspěšně nalezena
        CH_CT_FAIL, nedojde-li k žádné chybě, ale pozice nebyla nalezena
        kód chyby daný jinou konstantou deklarovanou v souboru obk_ch.h,
        dojde-li k jiné chybě.
 */
int hledej_pozici(TPozice *pozice, f_pozice_t *f_pozice);

#endif
