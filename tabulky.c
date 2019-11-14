/*********************************************************/
/* tabulky.c - prace s databazi koncovek                 */
/* 8.4. 2002 Jan Nemec                                 */
/*********************************************************/
#include "volby.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "volby.h"
#include "cotyvar.h"
#include "dosio.h"
#include "generato.h"
#include "globruti.h"
#include "sachy.h"
#include "myslitel.h"
#include "lokruti.h"
#include "tabulky.h"
#include "pole.h"
#include "sachy.h"
/*
Format ukladani:
Nejvice ovlivnuji pozici v souboru figury dle poradi
KkPpJjSsVvDd.
napr. KDk  kdohraje*10*64*64 k*64*64 + k*64 + D
Je-li vice figur teze barvy (treba KVVk), je ta
vic vlevo pod tou vic vpravo (uspora) !
Pozor, pesci jen od a2 ko h7, tj. jen 48 moznosti !
Neni-li pesec, je K vlevo dole neostre pod diagonalou
(napr. b1), je-li pesec, je K vlevo.
Jsou-li 2 pesci ruzne barvy, pridava se bit pro mimoch
a to nejvice vlevo, jeste pred kdo hraje.
Rosady jsou zakazane.
Pozor na rosady a brani mimochodem !!!

Hodnoty v tabulce:
0 - chyba nebo remiza
1 - cerny je v matu nebo bily dava 1. pultahem mat
2 - cerny tahne a bily da 1. pultehem mat nebo bily dava 2. pultahem mat
...
-k - totez jako +k, jen opacne barvy
Behem vytvareni -128 -zatim neznamo
*/

#define TABDIR "Endgames/"

static int KdeVPack[h8+1]={
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 1, 2, 3, 4, 5, 6, 7, 0,
 0, 8, 9,10,11,12,13,14,15, 0,
 0,16,17,18,19,20,21,22,23, 0,
 0,24,25,26,27,28,29,30,31, 0,
 0,32,33,34,35,36,37,38,39, 0,
 0,40,41,42,43,44,45,46,47, 0,
 0,48,49,50,51,52,53,54,55, 0,
 0,56,57,58,59,60,61,62,63};
static int KdeVPackKP[h8+1]={
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 1, 2, 3, 0, 0, 0, 0, 0,
 0, 4, 5, 6, 7, 0, 0, 0, 0, 0,
 0, 8, 9,10,11, 0, 0, 0, 0, 0,
 0,12,13,14,15, 0, 0, 0, 0, 0,
 0,16,17,18,19, 0, 0, 0, 0, 0,
 0,20,21,22,23, 0, 0, 0, 0, 0,
 0,24,25,26,27, 0, 0, 0, 0, 0,
 0,28,29,30,31, 0, 0, 0, 0};
static int KdeVPackPech[h7+1]={
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 1, 2, 3, 4, 5, 6, 7, 0,
 0, 8, 9,10,11,12,13,14,15, 0,
 0,16,17,18,19,20,21,22,23, 0,
 0,24,25,26,27,28,29,30,31, 0,
 0,32,33,34,35,36,37,38,39, 0,
 0,40,41,42,43,44,45,46,47};
static int KdeVSch[64]=
{a1,b1,c1,d1,e1,f1,g1,h1,
 a2,b2,c2,d2,e2,f2,g2,h2,
 a3,b3,c3,d3,e3,f3,g3,h3,
 a4,b4,c4,d4,e4,f4,g4,h4,
 a5,b5,c5,d5,e5,f5,g5,h5,
 a6,b6,c6,d6,e6,f6,g6,h6,
 a7,b7,c7,d7,e7,f7,g7,h7,
 a8,b8,c8,d8,e8,f8,g8,h8
};
static int KdeVSchKP[32]=
{a1,b1,c1,d1,
 a2,b2,c2,d2,
 a3,b3,c3,d3,
 a4,b4,c4,d4,
 a5,b5,c5,d5,
 a6,b6,c6,d6,
 a7,b7,c7,d7,
 a8,b8,c8,d8
};
static int KdeVSchPech[48]=
{a2,b2,c2,d2,e2,f2,g2,h2,
 a3,b3,c3,d3,e3,f3,g3,h3,
 a4,b4,c4,d4,e4,f4,g4,h4,
 a5,b5,c5,d5,e5,f5,g5,h5,
 a6,b6,c6,d6,e6,f6,g6,h6,
 a7,b7,c7,d7,e7,f7,g7,h7
 };
static int KdeVSchKral[10]={
 a1, b1, c1, d1,
     b2, c2, d2,
         c3, d3,
             d4
};

static int PocetKamenu(s8 *sch){
 s8 *k;
 int i;

 k=sch+h8;
 for(i=0,sch+=a1;sch<=k;sch++)if(*sch && *sch!=100)i++;
 return i;
}
void TSchTypStr(s8 *sch, char *s){
 char f[2][6];
 s8 *k;
 int i,j,l,b;
 const char *fgr="pjsvdk";

 k=sch+h8;
 memset((void *)f,0,12);
 for(sch+=a1;sch<=k;sch++){
  if(!*sch || *sch==100)continue;
  if(*sch>0 && *sch<7){f[0][*sch-1]++;continue;}
  if(*sch<0 && *sch>-7){f[1][-*sch-1]++;continue;}
 }
 b=1;
 for(i=5; i>=0; i--){
  if(f[0][i]>f[1][i])break;
  if(f[0][i]<f[1][i]){b=0; break;}
 }
 /* V b mam, zda je bily "silnejsi" */
 k = (s8 *) s;
 for((b ? (l=0) : (l=1)); b ? (l<=1) : (l>=0); b ? (l++) : (l--)) /*Neni to cecko krasne ?*/
  for(i=5; i>=0; i--)
   for(j=0; j<f[l][i]; j++)
    *k++=fgr[i];
 *k=0;

}

static int fileexists(char *s){
 FILE *f;
#pragma warning (disable : 4996)
 f=fopen(s,"rb");
 if(!f)return 0;
 fclose(f); return 1;
}

static int cena(char s){
 switch (s){
  case 0: return 0;
  case 'k': return 0;
  case 'p': return 1;
  case 'n': return 2;
  case 'b': return 3;
  case 'r': return 4;
  case 'q': return 5;
  default: return -1; /* TODO : Chyba() */
 }
}
static char jm[4]={'n','b','r','q'};
static int Normalizuj(char *s){
/* Z "kkd udela kdk a podobne" */
 int i;
 char *p, *a, *b, pom[10];

 for(p=s, i=0; *p; p++){
  if (*p=='k') i++;
  if(i==2)break;
 }
 if(!*p)return -1; /* chyba, nejsou 2 kralove*/
 a=s+1; b=p+1;
 while(*a==*b){a++;b++;} /* Dokud je to stejny */
 if(cena(*a)>=cena(*b)) return 1; /*Je to spravne (napr. "kdkp")*/
 /* else je to spatne (napr. "kpkd") */
 strcpy(pom,s);
 strcpy(s,pom+(p-s));
 strncpy(s+(strlen(pom)-(p-s)),pom,(p-s));
 return 0;
}
static int TabDelka(char *typ){
 /*int ntic[12];*/
 int f[2][6], k;
 int r,i,j;

/* memset(ntic,0,sizeof(int)*12);*/
 memset(f,0,sizeof(int)*12);
 k=0;
 while(*typ){
  if(*typ=='k')k++;
  if(k>1)f[1][cena(*typ)]++; else f[0][cena(*typ)]++;
  typ++;
 }
 if(f[0][1] || f[1][1]) r=64*32; else r=64*10; /* za oba krale */
 for(i=1;i<6;i++)
  for(j=0;j<=1;j++)
   switch(f[j][i]){
    case 0: break;
    case 1: if(i==1)r*=48; else r*=64;break;
    case 2: if(i==1)r*=1128;r*=2016;break; /* Nikoliv 64*64, lze totiz usporit na (64*63)/2 */
    case 3: r*=43680;break;  /* (64^3-64)/6 */
    default: return -1; /* chyba */
   }
 if(f[1][0]>0 && f[1][1]>0)r*=2; /* 2 pesci => brani mimochodem */
 r*=2; /* kdo hraje */
 return r;
}

static int FToFgr[2][6]={{6,1,2,3,4,5},{-6,-1,-2,-3,-4,-5}};
static int IntToPozice(int i, char *typ, TPozice *pos){
 int k,j,d,u,r,t;
 int f[2][6];

 d=TabDelka(typ);
 memset(pos->sch,100,sizeof(TSch));
 memset(f,0,sizeof(int)*12);
 for(k=a1,j=0;k<=h8;j=(j==9 ? 0 : j+1),k++)
  if(j<8)pos->sch[k]=0;
 k=0;
 while(*typ){
  if(*typ=='k')k++;
  if(k>1)f[1][cena(*typ)]++; else f[0][cena(*typ)]++;
  typ++;
 }

 pos->mimoch=0;
 pos->roch=0;

 for(k=5;k>=0;k--)
  for(j=1;j>=0;j--)
 switch(f[j][k]){
  case 0: continue;
  case 1:
   if(k==0 && j==0){ /*bily kral*/
   if(f[1][1]||f[0][1])/*Jsou pesci*/{
     u=i&31;
     i>>=5;
     u=KdeVSchKP[u];
     goto pryc3;
   }
 else {
    u=i%10;
    i/=10;
    u=KdeVSchKral[u];
    goto pryc3;
 }
  }
  else {
 if(k==1){
   u=i%48;
   i/=48;
   u=KdeVSchPech[u];
 }else {
   u=i&63;
   i>>=6;
/* pryc2:;*/
   u=KdeVSch[u];}
 pryc3:
   if(*(pos->sch+u)) return 0;
   *(pos->sch+u)=FToFgr[j][k];
  }
 break;
 case 2: /* 2 figury stejneho typu*/
 if(k==1){
   u=i%(24*47); /* (n*(n-1))/2 */
   i/=(24*47);
   for(r=1;(r*(r-1))<=2*u;r++); r--;
   t=KdeVSchPech[r];
   if(*(pos->sch+t)) return 0;
   *(pos->sch+t)=FToFgr[j][k];
   r=u-(r*(r-1))/2;           /* At zije matematika...*/
   t=KdeVSchPech[r];
   if(*(pos->sch+t)) return 0;
   *(pos->sch+t)=FToFgr[j][k];
 }else {
   u=i%(32*63); /* (n*(n-1))/2 */
   i/=(32*63);
   for(r=1;(r*(r-1))<=2*u;r++); r--;
   t=KdeVSch[r];
   if(*(pos->sch+t)) return 0;
   *(pos->sch+t)=FToFgr[j][k];
   r=u-(r*(r-1))/2;           /* At zije matematika...*/
   t=KdeVSch[r];
   if(*(pos->sch+t)) return 0;
   *(pos->sch+t)=FToFgr[j][k];
 }
 break;
 default: return 0; /*Trojice uz radeji ne*/
 }

 pos->bily=i&1;
 i>>=1;

 if(f[0][1] && f[1][1] & i)/* mimochodem*/
  {           /* Zde vyuzivam, ze resim jen 4 kamenne
 koncovky, KPPkp uz neni timto popsano !! */
   s8 *ptr;
   if(pos->bily){
     for(ptr=pos->sch; *ptr!=-1; ptr++);
      pos->mimoch=ptr-pos->sch;
    }else
  {
     for(ptr=pos->sch; *ptr!=1; ptr++);
      pos->mimoch=ptr-pos->sch;
    }
  }
 if(pos->bily && Sach(0,pos->sch) || !pos->bily && Sach(1,pos->sch))
  return 0;
 return 1;
}

static char DejTypPole[7]={'?', 'p', 'n', 'b', 'r', 'q', 'k'};

static int DejTyp(s8 *sch,char *typ){
/* Vraci 0, z hlediska cerneho; 1 z hlediska bileho*/
 int f[2][7], i, j;
 s8 *k; char *p;

 sch+=a1;
 memset(f,0,sizeof(int)*2*7);
 for(k=sch+h8; sch<=k; sch++)
  switch(*sch){
   case 0: continue;
   case 100: continue;
   default: if(*sch<0)f[0][-*sch]++;else f[1][*sch]++;
 }
 p=typ;
 for(i=1;i>=0;i--)
  for(j=6;j>=1;j--)
   while(f[i][j]--){*p=DejTypPole[j]; p++;}
 *p=0;
 return Normalizuj(typ);
}
int PrevratSch(int p){
 div_t t;

 t=div(p-21,10);
 return 21+t.rem+(7-t.quot)*10;
}

static int PoziceToInt(TPozice *pos, char *typ, int *zkoho)
{/*Vezme pozici a spocita typ a vrati int, -1 pri chybe.
  0 z hlediska cerneho, 1 z hlediska bileho */

 int r, i, j, k;
 int f[2][7][4];
 s8 *sch,*pk;
 if(pos->roch) return -1;
 *zkoho=DejTyp(pos->sch,typ);
 if(pos->mimoch)r=1; else r=0;
 r<<=1;
 r|=(!!pos->bily)^!*zkoho;
 sch=pos->sch+a1;
 memset(f,0,sizeof(int)*2*7*4);
 for(pk=sch+h8; sch<=pk; sch++)
  switch(*sch){
   case 0: continue;
   case 100: continue;
   default: if(*sch<0)
     for(i=0; i<3; i++)
      {if(!f[0][-*sch][i]){f[0][-*sch][i]=sch-pos->sch;break;}}
    else
     for(i=0; i<3; i++)
      {if(!f[1][*sch][i]){f[1][*sch][i]=sch-pos->sch;break;}}
 }
 if(!*zkoho){ /*Prohodit bile a cerne*/
  for(i=1; i<7; i++)
   for(j=0; j<4; j++){
    k=f[0][i][j];
    f[0][i][j]=((f[1][i][j])?PrevratSch(f[1][i][j]):0);
    f[1][i][j]=(k?PrevratSch(k):0);
  }
}
 /*Normalizovat bileho krale !!
   Kazdopadne musi byt vlevo
*/
 if((f[1][6][0]-a1)%10>3)
  for(i=0; i<=1; i++)
   for(j=1; j<=6; j++)
    for(k=0; f[i][j][k]; k++)
/*Prevraceni podle svisle osy mezi d. a e. sloupcem*/
     f[i][j][k]=a1+((f[i][j][k]-a1)/10)*10+(7-(f[i][j][k]-a1)%10);

 if(f[0][1][0] || f[1][1][0]) /*Je pesec*/{
   r*=32;
   r|=KdeVPackKP[f[1][6][0]];
 }
 else {
 /* Bily kral musi byt dole, vlevo (uz je) a pod diagonalou*/
  if(f[1][6][0]>h4)
  for(i=0; i<=1; i++)
   for(j=1; j<=6; j++)
    for(k=0; f[i][j][k]; k++)
/*Prevraceni podle vodorovne osy mezi 4. a 5. radou*/
     f[i][j][k]=a1+(7-(f[i][j][k]-a1)/10)*10+(f[i][j][k]-a1)%10;

 if((f[1][6][0]-a1)%10<(f[1][6][0]-a1)/10)
  for(i=0; i<=1; i++)
   for(j=1; j<=6; j++)
    for(k=0; f[i][j][k]; k++)
/*Prevraceni podle diagonaly a1-h8*/
     f[i][j][k]=a1+((f[i][j][k]-a1)/10)+10*((f[i][j][k]-a1)%10);
 r*=10;
 /*Bily kral*/
 if(f[0][1][0]||f[0][1][1]||f[1][6][0]<e1)r+=KdeVPack[f[1][6][0]];
 else
 if(f[1][6][0]<e2)r+=(f[1][6][0]-b2+4); else
 if(f[1][6][0]<e3)r+=(f[1][6][0]-c3+7); else
 r+=9;

}/*od else*/
 r<<=6;
 r|=KdeVPack[f[0][6][0]]; /*Cerny kral*/
 for(i=1;i<6;i++)
  for(j=1;j>=0;j--)
   {
     if(!(f[j][i][0]))continue;
     if(!(f[j][i][1]))
      {if(i==1)
       {r*=48;r+=KdeVPackPech[f[j][i][0]];}
       else
       {r<<=6;r|=KdeVPack[f[j][i][0]];}
       continue;
      }
     if(!(f[j][i][2]))
       {
        if(i==1)
        {r*=(24*47); r+=(((KdeVPack[f[j][i][1]]*(KdeVPack[f[j][i][1]]-1))>>1)+KdeVPack[f[j][i][0]]);}
        else
        {r*=(32*63); r+=(((KdeVPack[f[j][i][1]]*(KdeVPack[f[j][i][1]]-1))>>1)+KdeVPack[f[j][i][0]]);}
        continue;
       }
     return -1;
   }
 return r;

}
static void VelkyTest(char *typ){
 int delka,i,j, zkoho;
 TPozice pos;
 char str[10];

 delka=TabDelka(typ);
 for(i=0;i<delka;i++){
  if(i==4036){
  puts("I dosahlo urovne");
}
   if(IntToPozice(i,typ,&pos)){
     j=PoziceToInt(&pos,str,&zkoho);
     if(j!=i || strcmp(typ, str)){
        puts("Breakpoint");
        printf("%i=%i, %s=%s\n",i,j,typ,str);
       }
    }
  }
}
static char CenaPozice(TPozice *pos, char *typ, char *p){
/* typ a p muze byt NULL. V tom pripade se vzdy
 cte ze souboru. Jinak se z pameti p cte, pokud
 ma pozice typ typ.
*/
 char t[10];
 char cena;
 int index,zkoho;

 TSchTypStr(pos->sch, t);
 index=PoziceToInt(pos,t,&zkoho);
 if(typ && p && !strcmp(t,typ)) cena=p[index]; else
 {FILE *f;
  char s[50];
  strcpy(s,TABDIR);
  strcat(s,t);
  strcat(s,".tab");
#pragma warning (disable : 4996)
  f=fopen(s,"rb");
  if(!f)return(-128);
  fseek(f,index,SEEK_SET);
  fread((void *)&cena,1,1,f);
  fclose(f);
 }
 if(cena==-128 || zkoho)return cena; else return -cena;
}

static int GenerujBezRekurze(char *typ){
 int d,i,zmena,h,zachrana,konec,oh1,oh2,oh3,CernyJenKrale;
 char *p,t[50],cena;
 TUloha *u;
 FILE *f;

    printf("########## %s ##########\n", typ);
 d=TabDelka(typ);
 if(typ[strlen(typ)-1]=='k')CernyJenKrale=1; else CernyJenKrale=0;
 printf("Generuji %s delky %i\n",typ,d);
 p=(char*)malloc(d);
 if(!p)return -1;
 u=InitUlohu(0,0,0);
 oh1 = oh2 = oh3 = 0;
/* Vyresim maty a paty*/
 puts("Resim maty a paty");
 for(i=0;i<d;i++){
  if (!IntToPozice(i,typ,&(u->pozice))) {
    p[i] = 0;
    oh3++;
  }
 /*Chyba ma stejny kod jako remiza, ale to nevadi*/
  else{
    NalezTahy(u);
    JenPripustne(u);
    if(!u->zasobnik.hranice[1]){ /*Zadny tah*/
     if(Sach(u->pozice.bily,u->pozice.sch)){
      p[i]=(u->pozice.bily ? -1 : 1);oh1++;
     }
     else {p[i]=0;oh2++;} /* pat */
    }
    else p[i]=-128;
   }
 }
 printf("Matu %i a patu %i a nesmyslu %i\n", oh1, oh2, oh3);
 h=0;
 do{oh1=0;
    printf("hloubka %i\n",h++);
    zmena=0;
    puts("Utok");
    for(i=0;i<d;i++){
     if(p[i]==-128){TTah1 *p1,*p2;
       /*printf("i==%i\n",i);*/
       IntToPozice(i,typ,&(u->pozice));
       if(!u->pozice.bily && CernyJenKrale)continue;
       NalezTahy(u);
    /*   JenPripustne(u); - pomale */
       UlozStav(u);
       p1=u->zasobnik.tahy+u->zasobnik.hranice[0];
       p2=u->zasobnik.tahy+u->zasobnik.hranice[1];
       zachrana=0;
       konec=0;
       for(;p1<p2;p1++){
       konec=0;
       JednoTahni(p1->data,u);
       if(Sach(!u->pozice.bily,u->pozice.sch))konec=1;
        else
       cena=CenaPozice(&(u->pozice),typ,p);
/*     if(cena==-128||u->pozice.bily&&cena>=0||!u->pozice.bily&&cena<=0) zachrana=1;*/
       JednoTahniZpet(p1->data,u);
       if(konec)continue;
       if(u->pozice.bily && cena==h){
          p[i]=h;goto vyreseno;}
       if(!u->pozice.bily && cena==-h){
          p[i]=-h;goto vyreseno;}
       }
       continue;
       vyreseno:
        zmena=1;
        oh1++;
     }/*od if*/
    }/* od for*/
    printf("%i pozic,\nObrana\n",oh1);
    oh1=0;
    for(i=0;i<d;i++){
     if(p[i]==-128){TTah1 *p1,*p2;
       IntToPozice(i,typ,&(u->pozice));
       if(u->pozice.bily && CernyJenKrale)continue;
       NalezTahy(u);
     /*  JenPripustne(u);*/
       UlozStav(u);
       p1=u->zasobnik.tahy+u->zasobnik.hranice[0];
       p2=u->zasobnik.tahy+u->zasobnik.hranice[1];
       konec=zachrana=0;
       for(;p1<p2;p1++){
       konec=0;
       JednoTahni(p1->data,u);
       if(Sach(!u->pozice.bily,u->pozice.sch))konec=1;
       else
       cena=CenaPozice(&(u->pozice),typ,p);
       JednoTahniZpet(p1->data,u);
       if(konec)continue;
       if(cena==-128||u->pozice.bily&&cena>=0||!u->pozice.bily&&cena<=0) goto vyreseno2;
       }
       zmena=1; /*Neubranil se*/
       oh1++;
       p[i]=(u->pozice.bily ? (-h-1) : (h+1));
       continue;
       vyreseno2:
       oh1 = oh1;
     }/*od if*/
    }/* od for*/
    printf("%i pozic\n",oh1);
   }/*od do-while*/
 while(zmena);
 puts("Nahrazuji -128 nulama");
 for(i=0;i<d;i++)if(p[i]==-128)p[i]=0;
 DoneUlohu(&u);
 strcpy(t,TABDIR);
 strcat(t,typ);
 strcat(t,".tab");
#pragma warning (disable : 4996)
 f=fopen(t,"wb");
 if (!f) {
    printf("Nemůžu otevřít %s\n", t);
    return 1;
 }
 fwrite((void *)p,d,1,f);
 fclose(f);
 free(p);
 return 0;
}

void GenerujTabulky() {
    /* Warning: Take many hours to generate */

    /* 2 pieces */
    /* Draw */
    GenerujTabulku("kk");

    /* 3 pieces */
    /* Win */
    GenerujTabulku("kqk");
    GenerujTabulku("krk");
    /* ? */
    GenerujTabulku("kpk");
    /* Draw */
    GenerujTabulku("knk");
    GenerujTabulku("kbk");

    /* 4 pieces */
    /* Win */
    GenerujTabulku("kqkn");
    GenerujTabulku("kqkb");
    GenerujTabulku("kqkr");
    GenerujTabulku("kbnk");
    GenerujTabulku("kbbk");
    GenerujTabulku("kppk");

    /* Draw */
    GenerujTabulku("knnk");
    GenerujTabulku("kqkq");

    /* ? */
    GenerujTabulku("kqkp"); 
    GenerujTabulku("kpkp");
}

int GenerujTabulku(const char *typ){
  char s[50], p[10], t[10];
  unsigned int i ,j;

  size_t d = strlen(typ);
  if (d > 8)
    return -1;
  strcpy(p, typ);
  
  Normalizuj(p);
  strcpy(s, TABDIR);
  strcat(s, p);
  strcat(s,".tab");
  if (fileexists(s)) {
    printf("%s uz existuje\n", s);
    return 0;
  }
/* Ted vyresim rekurzivne zavislosti
Napr pro tabulku kdkv potrebuji kdk. Tj. musim vyresit
brani a promeny pescu. A navic hrozi zmena typu
Tj z kdkv muze vzniknout kkv, coz je ale kvk !!
Proto je treba volat Normalizuj()

*/
  strcpy(t, p);
  for(i = 0; i < d; i++) {
    if (p[i] == 'k')continue; /* kral se neda sebrat */
  strcpy(s, p);
  if(p[i]=='p'){ /* pesec se muze promenit */
   for(j=0; j<=3; j++){
    p[i]=jm[j];
    if(GenerujTabulku(p)) return -1;
   }
   p[i]='p';
   } /*Kazdy kamen, krome krale lze sebrat*/
   strcpy(t,p);
   for(j=i; p[j]; j++)p[j]=p[j+1];
   if(GenerujTabulku(p)) return -1;
   strcpy(p,t);
  }
/* Nyni uz mam vyresene rekurzivni zavislosti*/
 if(GenerujBezRekurze(t)) return -1;
 return 0;
}

u16 TabDejTah(TUloha *u){
 TTah1 *p1,*p2;
 char s[10];
 u16 data = 0;
 s8 cena,nejcena;

 if(PocetKamenu(u->pozice.sch)>4)return 0;
  DejTyp(u->pozice.sch,s);
/*  if(strlen(s)>4)return 0;*/
  NalezTahy(u);
  JenPripustne(u);
  if(!u->zasobnik.hranice[1])return 0; /*zadny tah*/
  UlozStav(u);
  p1=u->zasobnik.tahy+u->zasobnik.hranice[0];
  p2=u->zasobnik.tahy+u->zasobnik.hranice[1];
  nejcena=-1;
  for(;p1<p2;p1++){
   JednoTahni(p1->data,u);
   cena=CenaPozice(&(u->pozice),NULL,NULL);
   JednoTahniZpet(p1->data,u);
   if(cena==-128)return 0;
   if(!u->pozice.bily)cena=-cena;
   if((nejcena<0 && (cena<nejcena || cena>=0))||
      (nejcena==0 && cena>0) ||
      (nejcena>0 && cena>0 && cena<nejcena))
   {nejcena=cena; data=p1->data;}
}
return data;
}
