/* Šachové centrum - Knihovna zahájení Petr Kučera 2000*/
/* hašovací funkce převzata z hashtab.h - Jan Němec*/

#ifndef __HASH_H__
#define __HASH_H__

#include "cotyvar.h"
#include "pole.h"

/*Struktura popisující konstanty použité při výpočtu hašovacího klíče
  - převzato od Jana Němce ze souboru hashtab.h
 */
typedef struct {
  u32 f[13][h8];
  u32 bily;
  u32 roch[16];
  u32 mimoch[h5+1];
} hash_nahoda_t;

/*Proměnná, která obsahuje konstanty použité při výpočtu hašovacího klíče
 */
extern hash_nahoda_t hash_nahoda;

/*Funkce pučítá hašovací klíč pro pozici
  - převzato od Jana Němce ze souboru hashtab.h
  
  parametr poz pozice, pro niž je počítán hašvací klíč
  vrací hašovací klíč pozice poz
 */
u32 obk_hash(TPozice *poz);

/*Provede inicializaci mechanismu výpočtu hašovacího klíče
  vyplněním proměnné hash_nahoda náhodnými veličinami

  vrací 0 při úspěšné inicializaci, jinak kód chyby daný souborem obk_ch.h
 */
int obk_init_hash();

#endif
