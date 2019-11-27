/* Šachové centrum - Knihovna zahájení Petr Kučera 2000*/
/* Deklarace funkcí potřebných pro přístup do knihovny zahájení na nejvyšší úrovni
 */

#ifndef __KNIHOVNA_H__
#define __KNIHOVNA_H__ 1

#include "volby.h"
#include "cotyvar.h"

/*Před použitím knihovny nutno volat init_knihovna()

  parametr jmsoub určuje jméno souboru, v němž je obsažena přeložená knihovna
  vrací 0 při úspěšné inicializaci, jinak kód chyby určený konstanatami
        v souboru obk_ch.h
 */
cfunkce int init_knihovna(const char *jmsoub);

/*Hledá tah ze zadané pozice

  parametr pozice obsahuje pozici, z níž je tah hledán
  parametr pole eco musí být dlouhé alespoň 4, obsahuje při nalezení tahu
           ECO pozice vzniklé zahráním tahu.
  vrací 0 pokud nebyla pozice nalezena, nebo dvoubajtový popis tahu, pokud byl tah nalezen.
        tah je vybírán náhodně s pravděpodobností danou jeho vahou. V případě nalezení tahu
        obsahuje pole eco ECO pozice vzniklé zahráním vráceného tahu
*/
cfunkce u16 query_knihovna(TPozice *pozice, char *eco);

/*
  Hledá tah v knihovně zahájení

  parametr pozice obsahuje pozici, z níž je tah hledán
  vrací 0 pokud nebyla pozice nalezena, nebo dvoubajtový popis tahu, pokud byl tah nalezen.
        tah je vybírán náhodně s pravděpodobností danou jeho vahou.

  Poznámka: 0 není kód žádného tahu.
*/
cfunkce u16 KnihDejTah(TPozice *pozice);

#include "datastr.h"

/*
  Vrátí všechny tahy, které jsou v knihovně zahájení k dispozici pro danou pozici.

  parametr pozice obsahuje pozici, z níž jsou tahy hledány
  parametr tahy po úspěšném hledání obsahuje ukazatel na pole s tahy,
           které jsou k dispozici pro danou pozici. Pole je vytvořeno
           voláním malloc ve funkci v případě, že je pozice v knihovně
           nalezena a že z ní vede nějaký tah s nenulovou pravděpodobností,
           v opačném případě je vráceno místo pole NULL.
           Pokud je parametr tahy NULL, je vrácena hodnota CH_NULL
  parametr pocet_tahu obsahuje při úspěšném nalezení pozice počet vrácených
           tahů
  vrací 0 pokud nedojde k chybě (nenalezení pozice není bráno jako chyba,
          stejně jako neinicializovaná knihovna v době volání), jinak kód
          chyby daný konstantami v souboru obk_ch.h
 */
cfunkce int knih_dej_vsechny_tahy(TPozice *pozice, f_tah_t **tahy, int *pocet_tahu);

#endif
