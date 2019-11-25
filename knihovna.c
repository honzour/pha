/* Šachové centrum - Knihovna zahájení Petr Kučera 2000*/
/* Definice funkcí potřebných pro přístup do knihovny zahájení na nejvyšší úrovni
 */
#include "knihovna.h"
#include "cteni.h"
#include "obk_ch.h"
#include <stdlib.h>
#include <math.h>
#if Typ_Produktu==DLL
#include <string.h>
#include "interface.h"
#endif

int init_knihovna(const char *jmsoub)
{
  return init_cteni(jmsoub);
}

u16 query_knihovna(TPozice *pozice, char *eco)
{
  f_pozice_t f_pozice;
  int ch,j,vybran=0;
  srand((unsigned)time(NULL));
  char vaha = 1 + rand() % 100;
  f_tah_t tah;
  u16 tah_data;
  if(cteni_soubor==NULL) {
#if defined(LADIT_KNIHOVNA)&&LADIT_KNIHOVNA>0
    fprintf(stderr,"volana neinicializovana knihovna\n");
#endif
    return 0;
  }
#if defined(LADIT_KNIHOVNA)&&LADIT_KNIHOVNA>0
  fprintf(stderr,"hledam pozici:\n");
  print_pozice(stderr,pozice);
#endif
  ch=hledej_pozici(pozice, &f_pozice);
  if(ch)
    return 0;
#if defined(LADIT_KNIHOVNA)&&LADIT_KNIHOVNA>0
  dump_f_pozice(stderr,&f_pozice);
#endif
  if(!f_pozice.pocet_tahu) {
    return 0;
  }
  for(j=0; j<f_pozice.pocet_tahu; j++) {
    if(vaha<=f_pozice.tahy[j].vaha) {
      tah=f_pozice.tahy[j];
      vybran=1;
      break;
    }
    else
      vaha-=f_pozice.tahy[j].vaha;
  }
  if(!vybran) {
    tah=f_pozice.tahy[f_pozice.pocet_tahu-1];
  }
  if(eco!=NULL) {
    eco[0]=tah.ECO_pismeno;
    eco[1]='0'+tah.ECO_cislo/10;
    eco[2]='0'+tah.ECO_cislo%10;
    eco[3]=0;
  }
  tah_data=tah.tah;
  free(f_pozice.tahy);
  return tah_data;
}

u16 KnihDejTah(TPozice *pos)
{
  char eco[4];
  u16 result = query_knihovna(pos, eco);
#if Typ_Produktu==DLL
  if (result != 0)
  {
/* TODO PSA
    char message[40];
    strcpy_s(message, "Found in opening book, ECO: ");
    strcat_s(message, eco);
	*/
    Callbacks.TellGUIInfo(eco);
  }
#endif
  return result;
}

int knih_dej_vsechny_tahy(TPozice *pozice, f_tah_t **tahy, int *pocet_tahu)
{
  f_pozice_t f_pozice;
  int ch;
  if(tahy==NULL||pocet_tahu==NULL)
    return CH_NULL;
  *tahy=NULL;
  *pocet_tahu=0;
  if(cteni_soubor==NULL) {
#if defined(LADIT_KNIHOVNA)&&LADIT_KNIHOVNA>0
    fprintf(stderr,"volana neinicializovana knihovna\n");
#endif
    return 0;
  }
  ch=hledej_pozici(pozice, &f_pozice);
  if(ch)
    return ch;
  if(!f_pozice.pocet_tahu) {
    *pocet_tahu=0;
    *tahy=NULL;
    return 0;
  }
  *tahy=f_pozice.tahy;
  *pocet_tahu=f_pozice.pocet_tahu;  
  return 0;
}
