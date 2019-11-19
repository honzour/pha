/*********************************************************/
/* strtah.c - prevod tahu na retezec a naopak            */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#include <string.h>
#include <stdlib.h>
#include "volby.h"
#include "cotyvar.h"
#include "strtah.h"
#include "pole.h"

static int JakaFigura(u8 c, s8 *v)
 {
  switch(c){
   case 'n':*v=0;return 1;
   case 'b':*v=1;return 1;
   case 'r':*v=2;return 1;
   case 'q':*v=3;return 1;
   default:return 0;
  }
 }

static int JakaFiguraIKral(u8 c, s8 *v)
 {
  switch(c){
   case 'N':*v=2;return 1;
   case 'B':*v=3;return 1;
   case 'R':*v=4;return 1;
   case 'Q':*v=5;return 1;
   case 'K':*v=6;return 1;
   default:return 0;
  }
 }

/* udela tah z figura in 2..6,-6..-2  kam in a1..h8 oradka,osloupec in 1..7, */
static int DobreZadany(s8 figura, s8 osloupec, s8 oradka, s8 kam, TUloha *uloha, u16 *tah)
 {int  i;
  TTah1 *pz,*pk; /*pointery do zasobniku tahu
 pz - zacatek
 pk - konec */

  pz=uloha->zasobnik.tahy+uloha->zasobnik.hranice[uloha->zasobnik.pos];
   /*pocatek posledni urovne v zasobniku*/
  pk=uloha->zasobnik.tahy+uloha->zasobnik.hranice[uloha->zasobnik.pos+1];

  for(i=0;pz<pk;pz++)
   if(
       (!(pz->data>>14))&&        /* normalni tah */
       ((pz->data&127)==kam)&&     /* na stejne policko */
       (figura==(uloha->pozice.sch[pz->data>>7])) /* stejnou figurkou */
      )
   {
    if(
      (osloupec==-1 && oradka==-1) ||
      (oradka==-1 && (((pz->data>>7)%10)-1)==osloupec) ||
      (osloupec==-1 && (((pz->data>>7)-a1)/10)==oradka) ||
      ((((pz->data>>7)-a1)/10)==oradka && (((pz->data>>7)%10)-1)==osloupec)
       )
    {i++;*tah=(pz->data);}
   }

 return i;
 }

/*************************************************************************/
/* StrToTTah1 - prevod retezce na tah                                    */
/*************************************************************************/
int StrToTah(char *s, TUloha *uloha, u16 *tah)
 {unsigned kam,odkud;
  s8 v;
  int osloupec,oradka;

 if(!s[0] || !s[1]) return 0;
 if(s[0]>='a'&&s[0]<='h') /*tah pescem*/
  {  /*moznosti a4; a4 (o 2);axb4; axb6 e.p.; a8D; axb8D*/
    if(s[1]>='2'&&s[1]<='7')
     { /* ani brani, ani promena */
      kam=21+(s[0]-'a')+10*(s[1]-'1');
      if (uloha->pozice.bily)
        if (uloha->pozice.sch[kam-10]==1)
          (*tah)=(u16)(kam|((kam-10)<<7));  /*bilym pescem o 1*/
      else
        {if (uloha->pozice.sch[kam-20]==1)
          (*tah)=(u16)(kam|((kam-20)<<7)); /*bilym pescem o 2*/
        else return 0;}

      else
       if (uloha->pozice.sch[kam+10]==-1)
        (*tah)=(u16)(kam|((kam+10)<<7));  /*cernym pescem o 1*/
       else
      {if (uloha->pozice.sch[kam+20]==-1)
       (*tah)=(u16)(kam|((kam+20)<<7)); /*cernym pescem o 2*/
       else return 0;}
      return 1;
     } /* od ani brani, ani promena */
    else
   { /* Zatim vim, ze s[0] in <'a,'h'> a s[1] not in <'2','7'>
        neboli je to tah pescem, ktery je brani nebo promena */
    if(s[1]=='8')/*promena bileho pesce ne branim*/
     {
      if(!JakaFigura(s[2],&v))return 0;
      kam=(s[0]-'a');
      (*tah)=(u16)(((3<<14)|(v<<10)|((kam)<<7)|((kam)<<4)));
      return 1;
     }
    else
    if(s[1]=='1')/*promena cerneho pesce ne branim*/
     {
      if(!JakaFigura(s[2],&v))return 0;
      kam=(s[0]-'a');
      (*tah)=(u16)(((3<<14)|(1<<12)|(v<<10)|((kam)<<7)|((kam)<<4)));
      return 1;
     }
    else
    if(s[1]=='x')
    {/*brani pescem: moznosti axb4; axb6 e.p.; axb8D*/
     if(s[2]>'h'||s[2]<'a'||s[3]>'8'||s[3]<'1') return 0;
     if(s[3]<'8'&&s[3]>'1') /*moznosti axb4; axb6 e.p.*/
      { /* brani, ktere neni zaroven promena */
       kam=21+(s[2]-'a')+10*(s[3]-'1');
       odkud=kam+s[0]-s[2];
       if(uloha->pozice.bily)odkud-=10;else odkud+=10;
       (*tah)=(u16)(((uloha->pozice.sch[kam])? 0:(1<<15))|(odkud<<7)|kam);
    /* zde jsem pouzil podobnost zaznamu brani mimochodem s
       meznym tahem v ttah1.data*/
    return 1;
      } /* od brani, ktere neni zaroven promena */
     else /*axb8D*/
     { /* promena branim */

    if(s[3]=='8')/*promena bileho pesce branim*/
     {
      if(!JakaFigura(s[4],&v))return 0;
      (*tah)=(u16)(((3<<14)|(v<<10)|((s[0]-'a')<<7)|((s[2]-'a')<<4)));
      return 1;
     }
    else
    if(s[3]=='1')/*promena cerneho pesce branim*/
     {
      if(!JakaFigura(s[4],&v))return 0;
      (*tah)=(u16)(((3<<14)|(1<<12)|(v<<10)|((s[0]-'a')<<7)|((s[2]-'a')<<4)));
      return 1;
     }
    else return 0;
    } /* od promena branim */
    } /* od brani */
    else return 0;
   }  /* od brani nebo promena */
  } /* od tah pescem*/
 else /*tah figurou*/
 if(s[0]=='O') /*rosada*/
  {if(s[1]!='-'||s[2]!='O')return 0;
   if(s[3]=='-'&&s[4]=='O')
    {if(uloha->pozice.bily)(*tah)=VBRoch;else(*tah)=VCRoch;}
   else
    {if(uloha->pozice.bily)(*tah)=MBRoch;else(*tah)=MCRoch;}
  return 1;
  }
 else /*tah figurou, ne rosada*/
  {
  /* tah */
   if (!JakaFiguraIKral(s[0],&v))return 0;
   if (!uloha->pozice.bily)v=(s8)-v;
   if(s[1]=='x')
#define NastavKam(param) {kam=(a1+(s[(param)]-'a')+(s[(param)+1]-'1')*10);}
    {osloupec=-1;oradka=-1;NastavKam(2);} /* Jxe5 */
    else
    {if(s[1]<='8' && s[1]>='1') /* V1e3 nebo V1xe3*/
     {osloupec=-1;oradka=s[1]-'1';
      if(s[2]=='x')NastavKam(3)else NastavKam(2)
     }
    else
    {   /* Va1 Vba1 Vb1a1 Vbxa1 Vb1xa1 */
     if(s[2]=='x'){osloupec=s[1]-'a';oradka=-1;NastavKam(3)}/*Vbxa1*/
     else
     { /* Va1 Vba1 Vb1a1 Vb1xa1 */
      /* to debilni ceckovsky case .... (obcas miluji Pascal)*/
      if(s[3]=='x')
       {osloupec=s[1]-'a';oradka=s[2]-'1';NastavKam(4)}/*Vb1xa1 */
      else
      if(s[3]>='1' && s[3]<='8')
       {osloupec=s[1]-'a';oradka=-1;NastavKam(2) } /*Vba1*/
      else
      if(s[3]>='a' && s[3]<='h')
       {osloupec=s[1]-'a';oradka=s[2]-'1';NastavKam(3)} /*Vb1a1*/
      else
       {osloupec=-1;oradka=-1;NastavKam(1)} /*Vb1*/
     }
    }
    }
   if(((int)DobreZadany((s8)v,(s8)osloupec,(s8)oradka,(s8)kam,uloha, tah))==1)
               return 1;
    else return 0;
  }
}

/* Je tah urcen jednoznacne ? (urcen je J_xxx) a tahy v uloze jsou nalezene */
int JednoZnacny(u16 tah, TUloha *uloha,int urcen)
 {int odkud, kam;
  TTah1 *pz,*pk; /*pointery do zasobniku tahu
 pz - zacatek
 pk - konec */

  if(tah>>14) return 1;/* Nenormalni tah je vzdy jednoznacny*/
  kam=tah&127;
  odkud=tah>>7;
  pz=uloha->zasobnik.tahy+uloha->zasobnik.hranice[uloha->zasobnik.pos];
   /*pocatek posledni urovne v zasobniku*/
  pk=uloha->zasobnik.tahy+uloha->zasobnik.hranice[uloha->zasobnik.pos+1];

  for(;pz<pk;pz++)
   if((!(pz->data>>14))&&        /* normalni tah */
       ((pz->data&127)==kam)&&     /* na stejne policko */
       (odkud!=(pz->data>>7)) &&   /* z jineho policka */
       ((uloha->pozice.sch[odkud])==(uloha->pozice.sch[pz->data>>7])) /* stejnou figurkou */
       )
      switch(urcen){
       case J_Nic:return 0;
       case J_Radka:if(odkud/10==(pz->data>>7)/10)return 0;break;
       case J_Sloupec:if(odkud%10==(pz->data>>7)%10)return 0;break;
      }
 return 1;
 }

/*************************************************************************/
/* LongStrToTTah1 - prevod retezce na tah                                */
/*************************************************************************/
int LongStrToTah(char* s, TUloha* uloha, u16* tah)
{
    unsigned kam, odkud;
    s8 v;

    if (!s[0] || !s[1] || !s[2] || !s[3])
        return 0;

    odkud = 21 + (s[0] - 'a') + 10 * (s[1] - '1');
    kam = 21 + (s[2] - 'a') + 10 * (s[3] - '1');

    v = uloha->pozice.sch[odkud];
    if (v == 6 || v == -6)
    {
        if (odkud - kam == 2)
        {
            if (uloha->pozice.bily)
                (*tah) = VBRoch; 
            else
                (*tah) = VCRoch;
            return 1;
        }
        else if (odkud - kam == -2)
        {
            if (uloha->pozice.bily)
                (*tah) = MBRoch;
            else
                (*tah) = MCRoch;
            return 1;
        }
    }
    else if (v == 1 || v == -1)
    {
        if (((uloha->pozice.sch[kam]) == 0) && (odkud % 10 != kam % 10))
        {
            (*tah) = (u16)((1 << 15) | (odkud << 7) | kam);
            return 1;
        }
    }

    if (s[4])
    {
        if (!JakaFigura(s[4], &v))
            return 0;
        if (uloha->pozice.bily)
        {
            odkud = odkud - a7;
            kam = kam - a8;
        }
        else
        {
            odkud = odkud - a2;
            kam = kam - a1;
        }
        (*tah) = (u16)(((3 << 14) | (v << 10) | (odkud << 7) | (kam << 4)));
        if (!uloha->pozice.bily)
            (*tah) = (*tah) | (1 << 12);
    }
    else
    {
        (*tah) = (u16)(kam | (odkud << 7));
    }
    return 1;
}

/*************************************************************************/
/* TTah1ToStr - prevod tahu na retezec                                    */
/*************************************************************************/
void TahToStr(u16 tah, TUloha *uloha, char *s)
  {int odkud,kam,i;

  if(!(tah>>14)) /* Normalni tah*/
   {kam=tah&127;
    odkud=tah>>7;
    i=0;
    switch (abs(uloha->pozice.sch[odkud])){
    case 1:if (uloha->pozice.sch[kam]) s[i++]=(char)((odkud-a1)%10 + 'a');
        goto PesecJeJednoznacny;
    case 2:s[i++]='N';break;
    case 3:s[i++]='B';break;
    case 4:s[i++]='R';break;
    case 5:s[i++]='Q';break;
    case 6:s[i++]='K';break;
    }
   if (!JednoZnacny(tah,uloha,J_Nic)) /* Zkusim Da1 */
    {    /* Tak Dha1 */
    if (JednoZnacny(tah,uloha,J_Sloupec)) s[i++]=(char)((odkud-a1)%10 + 'a');
     else /* Tak D1a1 */
      if (JednoZnacny(tah,uloha,J_Radka)) s[i++]=(char)((odkud-a1)/10 + '1');
       else /* Tak teda Dh1a1 (nutne pokud jsou 3 damy na h1, h8 a a8)*/
    {s[i++]=(char)((odkud-a1)%10 + 'a');
     s[i++]=(char)((odkud-a1)/10 + '1');
    }
    }
   PesecJeJednoznacny:;
    if (uloha->pozice.sch[kam]) s[i++]='x';
    s[i++]=(char)((kam-a1)%10 + 'a');
    s[i++]=(char)((kam-a1)/10 + '1');
    s[i]=0;
    return;
    }
  /* Nenormalni tah
     Mala rosada*/
  if (tah==MBRoch || tah==MCRoch)
#pragma warning (disable : 4996)
  { (void) strcpy((char *) s,"O-O"); return;}
  /*Velka rosada*/
  if (tah==VBRoch || tah==VCRoch)
#pragma warning (disable : 4996)
  { (void) strcpy((char *) s,"O-O-O"); return;}

  /*Promena bileho pesce*/
 if ((tah>>12)==12 || (tah>>12)==13 ) {
 if ((tah>>12)==12)
  {odkud=a7+((tah>>7)&7);
   kam=a8+((tah>>4)&7);}
  else
  {odkud=a2+((tah>>7)&7);
   kam=a1+((tah>>4)&7);}
   i=0;
   s[i++]=(char)((odkud-a1)%10 + 'a');
   if (uloha->pozice.sch[kam]) {s[i++]='x'; s[i++]=(char)((kam-a1)%10 + 'a');}
   s[i++]=(char)((kam-a1)/10 + '1');
   switch((tah>>10)&3){
   case 0:s[i++]='N';break;
   case 1:s[i++]='B';break;
   case 2:s[i++]='R';break;
   case 3:s[i++]='Q';break;
   }
   s[i]=0;
   return;
  }
 /* Brani mimochodem (nic jineho to uz byt nemuze)*/
 tah&=0x3fff; /* odstraneni prvnich dvou bitu, aby se lepe siftovalo*/
 kam=tah&127;
 odkud=tah>>7;
 i=0;
 s[i++]=(char)((odkud-a1)%10 + 'a');
/* s[i++]=(odkud-a1)/10 + '1';*/
 s[i++]='x';
 s[i++]=(char)((kam-a1)%10 + 'a');
 s[i++]=(char)((kam-a1)/10 + '1');
 s[i++]='e';
 s[i++]='p';
 s[i]=0;
 return;
}

void TahToLongStr(u16 tah, char* s)
{
    int odkud, kam, i;
    i = 0;
    if (!(tah >> 14)) /* Normalni tah*/
    {
        kam = tah & 127;
        odkud = tah >> 7;
        s[i++] = (char)((odkud - a1) % 10 + 'a');
        s[i++] = (char)((odkud - a1) / 10 + '1');
        s[i++] = (char)((kam - a1) % 10 + 'a');
        s[i++] = (char)((kam - a1) / 10 + '1');
        s[i] = 0;
        return;
    }
    /* Nenormalni tah
       Mala rosada*/
    if (tah == MBRoch)
    {
        s[i++] = 'e';
        s[i++] = '1';
        s[i++] = 'g';
        s[i++] = '1';
        s[i] = 0;
        return;
    }
    else if (tah == MCRoch)
    {
        s[i++] = 'e';
        s[i++] = '8';
        s[i++] = 'g';
        s[i++] = '8';
        s[i] = 0;
        return;
    }
    /*Velka rosada*/
    else if (tah == VBRoch) {
        s[i++] = 'e';
        s[i++] = '1';
        s[i++] = 'c';
        s[i++] = '1';
        s[i] = 0;
        return;
    }
    else if (tah == VCRoch)
    {
        s[i++] = 'e';
        s[i++] = '8';
        s[i++] = 'c';
        s[i++] = '8';
        s[i] = 0;
        return;
    }

    /*Promena bileho pesce*/
    if ((tah >> 12) == 12 || (tah >> 12) == 13) {
        if ((tah >> 12) == 12)
        {
            odkud = a7 + ((tah >> 7) & 7);
            kam = a8 + ((tah >> 4) & 7);
        }
        else
        {
            odkud = a2 + ((tah >> 7) & 7);
            kam = a1 + ((tah >> 4) & 7);
        }
        s[i++] = (char)((odkud - a1) % 10 + 'a');
        s[i++] = (char)((odkud - a1) / 10 + '1');
        s[i++] = (char)((kam - a1) % 10 + 'a');
        s[i++] = (char)((kam - a1) / 10 + '1');
        switch ((tah >> 10) & 3) {
        case 0:s[i++] = 'n'; break;
        case 1:s[i++] = 'b'; break;
        case 2:s[i++] = 'r'; break;
        case 3:s[i++] = 'q'; break;
        }
        s[i] = 0;
        return;
    }
    /* Brani mimochodem (nic jineho to uz byt nemuze)*/
    tah &= 0x3fff; /* odstraneni prvnich dvou bitu, aby se lepe siftovalo*/
    kam = tah & 127;
    odkud = tah >> 7;
    s[i++] = (char)((odkud - a1) % 10 + 'a');
    s[i++] = (char)((odkud - a1) / 10 + '1');
    s[i++] = (char)((kam - a1) % 10 + 'a');
    s[i++] = (char)((kam - a1) / 10 + '1');
    s[i] = 0;
    return;
}
