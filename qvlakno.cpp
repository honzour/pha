#include "volby.h"
#if Typ_Produktu==Qt_Program
#include <stdio.h>
#include <qthread.h>
#include <qapplication.h>
#include "qhlokno.h"
#include "qvlakno.h"
#include "qtsachy.h"
#include "qtahevent.h"
#include "cotyvar.h"
#include "myslitel.h"

TVlaknoMysleni::TVlaknoMysleni(TSachovnice *s)
{
 this->s=s;
}

void TVlaknoMysleni::run(){
 TTah1 t;

// puts("Vlakno");
 DejTah(s->u,&t,5000,NULL,NULL);
 Aplikace->sendEvent(s,new TahEvent(t.data));
// printf("%i\n",(int)t.data);
}
#endif

