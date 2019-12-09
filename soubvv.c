/*********************************************************/
/* soubvv.c - IO partie do formatu HS 1350               */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "volby.h"
#include "cotyvar.h"
#include "strtah.h"
#include "globruti.h"
#include "generato.h"
#include "myslitel.h"
#include "partie.h"
#include "pole.h"
#include "boardFromFEN.h"

char PismenoKamene(s8 k){
	if(k<0)k=(s8)-k;
	switch(k){
	case 1: return 'P';
	case 2: return 'J';
	case 3: return 'S';
	case 4: return 'V';
	case 5: return 'D';
	case 6: return 'K';
	}
 return 'X';
}

/******************************************************/
/* Ulozeni aktualni partie do souboru formatu HS 1350 */
/******************************************************/
int UlozDoSouboru(char *jmeno,TUloha *u) {
	FILE *f;
	TPartie *prt;
	char jmt[15];
	int i;
	s8 j;

	f=fopen(jmeno,"w");
	if(!f)return 0;
	prt=u->prt;
	while (u->prt->l){
		GlobTahniZpet(u);
	}
	fprintf(f,"Po��te�n� postaven�:\nB�l�:\n");
	for(j=1;j<=6;j++)
		for(i=a1;i<=h8;i++)
			if(u->pozice.sch[i]==j){
				putc(PismenoKamene(j),f);
				putc((i-a1)%10+'a',f);
				putc((i-a1)/10+'1',f);
				putc(' ',f);
			}
	fprintf(f,"\nMal� ro��da: %s",(u->pozice.roch&1)?"Ano":"Ne");
	fprintf(f,"\nVelk� ro��da: %s",(u->pozice.roch&2)?"Ano":"Ne");
	fprintf(f,"\n�ern�:\n");
	for(j=-1;j>=-6;j--)
		for(i=a1;i<=h8;i++)
			if(u->pozice.sch[i]==j){
				putc(PismenoKamene(j),f);
				putc((i-a1)%10+'a',f);
				putc((i-a1)/10+'1',f);
				putc(' ',f);
			}
	fprintf(f,"\nMal� ro��da: %s",(u->pozice.roch&4)?"Ano":"Ne");
	fprintf(f,"\nVelk� ro��da: %s",(u->pozice.roch&8)?"Ano":"Ne");
	fputs("\nNa tahu je: ",f);
	fputs(u->pozice.bily ? "B�l�":"�ern�",f);
	putc('\n',f);
	fprintf(f,"Pr�b�h partie:\n");
	i=0;
	while (u->prt->p){
		NalezTahy(u);
		TahToStr(u->prt->p->data.t,u,jmt);
		if(!i&&!u->pozice.bily){
			fprintf(f,"     1... %s\n",jmt);
			GlobTahni(0,u,0);
			i=2;
			continue;
		}
		if(u->pozice.bily)
			fprintf(f,"%i. %s",i/2+1,jmt);
		else
			fprintf(f,", %s\n",jmt);
		GlobTahni(0,u,0);
		i++;
	}
	while (u->prt!=prt)GlobTahniZpet(u);
	fclose(f);
	return 1;
}

int OtevriFENZeSouboru(char *jmeno,TUloha *u) {
  char s[512];
  TPozice pos;
  FILE *f;

  f = fopen(jmeno,"r");
  if (!f) {
	  printf("Nemuzu otevrit %s\n", jmeno);
	  return 0;
  }
  if (!fgets(s,sizeof(s) - 1,f)) {
    printf("Nemuzu cist z %s\n", jmeno);
    fclose(f);
    return 0;
  }
  fclose(f);

  boardFromFEN(&pos, s);

  InitPartie(&(u->prt), &pos);
  u->pozice = pos;
  
  return 1;
}


int OtevriZeSouboru(char *jmeno,TUloha *u){
/*******************************************************/
/* Otevreni aktualni partie ze souboru formatu HS 1350 */
/*******************************************************/
	TPartie *prt; /* Zde si uchov�m p�vodn� partii*/
	TPozice pos; /*Zde si uchov�m p�vodn� pozici*/
/*Kdy� se otev�en� nepoda��, vr�t�m se k p�vodn�m*/
	int i,j,k;
	s8 g;
	FILE *f;
	char s[100];
	u16 t;

	prt=u->prt;
	u->prt=NULL;
/* Te� m�m v prt p�vodn� partii a u->prt pr�zdnou */
	pos=u->pozice;
	for(i=0;i<=7;i++)
		for(j=0;j<=7;j++){
			u->pozice.sch[i*10+j+a1]=0;
		}
	u->pozice.roch=0;
	u->pozice.mimoch=0;
/* Te� analogicky s pozic�*/
	f=fopen(jmeno,"r");
	if(!f)goto chyba;
	if(!fgets(s,50,f) || strcmp(s,"Po��te�n� postaven�:\n")||
		!fgets(s,50,f))goto chybaf;
        if(!strcmp(s,"B�l�:\n"))u->pozice.bily=1;else
		if(!strcmp(s,"�ern�:\n"))u->pozice.bily=0;else
			goto chybaf;
	if(!fgets(s,97,f))goto chybaf;
	j=strlen(s);
	for(i=0;i<j;){
		switch(s[i]){
		case 'K':g=6; break;
		case 'D':g=5; break;
		case 'V':g=4; break;
		case 'S':g=3; break;
		case 'J':g=2; break;
		case 'P':g=1; break;
		default:g=0;
		}
		if(!g)break;
		i++;
		if(s[i]>'h' || s[i]<'a'|| s[i+1]>'8' || s[i+1]<'1') break;
		u->pozice.sch[a1+(s[i]-'a')+(s[i+1]-'1')*10]=g;
		i+=3;
	}
	if(!fgets(s,50,f))goto chybaf;
	if(strncmp(s,"Mal� ro��da: ",13))goto chybaf;
	if(!strcmp(s+13,"Ano\n"))u->pozice.roch|=1;
	else
	if(strcmp(s+13,"Ne\n"))goto chybaf;
	if(!fgets(s,50,f))goto chybaf;
	if(strncmp(s,"Velk� ro��da: ",14))goto chybaf;
	if(!strcmp(s+14,"Ano\n"))u->pozice.roch|=2;
	else
	if(strcmp(s+14,"Ne\n"))goto chybaf;

	if(!fgets(s,50,f) || strcmp(s,"�ern�:\n")) goto chybaf;
	if(!fgets(s,97,f))goto chybaf;
	j=strlen(s);
	for(i=0;i<j;){
		switch(s[i]){
		case 'K':g=6; break;
		case 'D':g=5; break;
		case 'V':g=4; break;
		case 'S':g=3; break;
		case 'J':g=2; break;
		case 'P':g=1; break;
		default:g=0;
		}
		if(!g)break;
		i++;
		g=(s8)-g;
		if(s[i]>'h' || s[i]<'a'|| s[i+1]>'8' || s[i+1]<'1') break;
		u->pozice.sch[a1+(s[i]-'a')+(s[i+1]-'1')*10]=g;
		i+=3;
	}
	if(!fgets(s,50,f))goto chybaf;
	if(strncmp(s,"Mal� ro��da: ",13))goto chybaf;
	if(!strcmp(s+13,"Ano\n"))u->pozice.roch|=4;
	else
	if(strcmp(s+13,"Ne\n"))goto chybaf;
	if(!fgets(s,50,f))goto chybaf;
	if(strncmp(s,"Velk� ro��da: ",14))goto chybaf;
	if(!strcmp(s+14,"Ano\n"))u->pozice.roch|=8;
	else
	if(strcmp(s+14,"Ne\n"))goto chybaf;
	
	if(!fgets(s,50,f))goto chybaf;
	if(strncmp(s,"Na tahu je: ",12))goto chybaf;
	if(!strcmp(s+12,"B�l�\n"))u->pozice.bily=1;
	else
	if(!strcmp(s+12,"�ern�\n"))u->pozice.bily=0;
	else goto chybaf;
	if(!fgets(s,50,f) || strcmp(s,"Pr�b�h partie:\n"))
		goto chybaf;
/* A kone�n� na��t�m partii */
	InitPartie(&(u->prt),&(u->pozice));
	if(!u->pozice.bily){
		/* ve v�choz� pozici za��nal �ern� */
	if(!fgets(s,50,f)||!s[0]) goto KonecPartie;
		if(strstr(s,"     1... ")!=s) goto chybaf;
		NalezTahy(u);
		JenPripustne(u);
		if(!StrToTah(s+10,u,&t))goto chybaf;
		if(!KTNehledej(t,u))goto chybaf;
		GlobTahni(1,u,1,(unsigned)t);
	}
	
	while(1){
		if(!fgets(s,50,f)||!s[0]) break;
		j=strlen(s);
	for(k=0;k<2;k++){
		if(k){
			for(i=0;i<j;i++)if(s[i]==',') break;
			if(s[i]!=',') goto KonecPartie;
		}
		else{
			for(i=0;i<j;i++)if(s[i]=='.') break;
		}
		i+=2;
		if(i>=j)goto chybaf;
		NalezTahy(u);
		JenPripustne(u);
		if(!StrToTah(s+i,u,&t))goto chybaf;
		if(!KTNehledej(t,u))goto chybaf;
		GlobTahni(1,u,1,(unsigned)t);
		}
	}
KonecPartie:;
	fclose(f);
	DonePartie(&prt);
	return 1;
chybaf:
	fclose(f);
chyba:
	DonePartie(&(u->prt));
	u->prt=prt;
	u->pozice=pos;
/* Obnovil jsem p�vodn� pozici a partii */
	return 0;
}

