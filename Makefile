make:
	@echo "syntaxe: make clean | qt | konzole | pha | windows"

outdir=out
ifeq (${target}, windows)
cc=wine /home/honza/honza/mingw/bin/gcc.exe
windres=/home/honza/honza/mingw/bin/windres.exe
else
cc=gcc
endif

.PHONY: clean
clean:
	if [ -e ${outdir} ]; then rm -r -f ${outdir}; fi

opt=-O3 -Wno-unused-result

baseobj=${outdir}/cotyvar.o ${outdir}/cteni.o ${outdir}/datastr.o \
	${outdir}/dosio.o ${outdir}/hash.o ${outdir}/hashpech.o \
	${outdir}/hashtab.o ${outdir}/generato.o ${outdir}/globruti.o \
	${outdir}/chyba.o ${outdir}/knihovna.o ${outdir}/kontrola.o \
	${outdir}/lokruti.o ${outdir}/myslitel.o ${outdir}/obk_ch.o \
	${outdir}/ohodnoc.o ${outdir}/partie.o ${outdir}/pgn.o \
	${outdir}/sachy.o ${outdir}/strtah.o ${outdir}/tabulky.o \
	${outdir}/soubvv.o ${outdir}/dejtah.o ${outdir}/ticho.o ${outdir}/testy.o

ifeq (${target}, windows)
volby=${def} ${opt} -c -mwindows
winspecobj=${outdir}/winio.o ${outdir}/winmain.o ${outdir}/winmysl.o  ${outdir}/winpos.o  ${outdir}/winsachy.o ${outdir}/partiar.o ${outdir}/dlg.o ${outdir}/promdlg.o
winallobj=${baseobj} ${winspecobj}
endif
ifeq (${target}, qt)
qtspecobj=${outdir}/qtsachy.o ${outdir}/qsachovnice.o \
	${outdir}/moc_qhlokno.o ${outdir}/qhlokno.o ${outdir}/qvlakno.o \
	${outdir}/qpromena.o
qtallobj=${baseobj} ${qtspecobj}
qtinclude=-I/usr/lib/qt2/include
qtvlakno=-DQT_THREAD_mSUPPORT
qtdef=-DTyp_Produktu=5
qtlib=-L/usr/lib/qt2/lib -lqt-mt
volby=${opt} ${qtvlakno} ${qtdef} ${qtinclude} -c
endif # ifeq (${target}, qt)

ifeq (${target},konzole)
def=-DTyp_Produktu=2
konallobj=${baseobj}
volby=${def} ${opt} -c
endif
ifeq (${target},pha)
def=-DTyp_Produktu=4
phaallobj=${baseobj} ${outdir}/scio.o
volby=${def} ${opt} -c
endif

.PHONY: sachy
ifeq (${target},qt)
sachy: ${outdir} ${outdir}/qtsachy
endif
ifeq (${target},konzole)
sachy: ${outdir} ${outdir}/konsachy
endif
ifeq (${target},pha)
sachy: ${outdir} ${outdir}/pha
endif
ifeq (${target},windows)
sachy: ${outdir} ${outdir}/winsachy.exe
endif


.PHONY: qt
qt:
	make target=qt sachy
	
.PHONY: konzole
konzole:
	make target=konzole sachy
	
.PHONY: windows
windows:
	make target=windows sachy

.PHONY: pha
pha:
	make target=pha sachy

${outdir}:
	mkdir ${outdir}

${outdir}/winsachy.exe: ${winallobj} Makefile
	${cc} ${opt} -mwindows ${winallobj} -o ${outdir}/winsachy.exe
	
${outdir}/qtsachy: ${qtallobj} Makefile 
	${cc} ${opt} ${qtallobj} ${qtlib} -o ${outdir}/qtsachy

${outdir}/konsachy: ${konallobj} Makefile 
	${cc} ${opt} ${konallobj} -o ${outdir}/konsachy

${outdir}/pha: ${phaallobj} Makefile 
	${cc} ${opt} ${phaallobj} -o ${outdir}/pha

${outdir}/qtsachy.o: qtsachy.cpp qhlokno.h Makefile 
	g++ qtsachy.cpp -o ${outdir}/qtsachy.o ${volby}

${outdir}/qpromena.o: qpromena.cpp Makefile 
	g++ qpromena.cpp -o ${outdir}/qpromena.o ${volby}

${outdir}/qhlokno.o: qhlokno.cpp qhlokno.h qsachovnice.h qvlakno.h Makefile 
	g++ qhlokno.cpp -o ${outdir}/qhlokno.o ${volby}

${outdir}/moc_qhlokno.o: qhlokno.h qsachovnice.h Makefile 
	moc qhlokno.h -o ${outdir}/moc_qhlokno.cpp && g++ ${outdir}/moc_qhlokno.cpp -o ${outdir}/moc_qhlokno.o ${volby}

${outdir}/qsachovnice.o: qsachovnice.cpp qsachovnice.h volby.h cotyvar.h chyba.h pole.h qpromena.h Makefile 
	g++ qsachovnice.cpp -o ${outdir}/qsachovnice.o ${volby}

${outdir}/qvlakno.o: qvlakno.cpp qvlakno.h qhlokno.h qtsachy.h myslitel.h volby.h cotyvar.h Makefile 
	g++ qvlakno.cpp -o ${outdir}/qvlakno.o ${volby}
	
# sekce definujici zavislosti

z_soubvv=soubvv.c soubvv.h cotyvar.h volby.h strtah.h globruti.h generato.h myslitel.h partie.h pole.h Makefile
z_cotyvar=cotyvar.c cotyvar.h volby.h Makefile
z_globruti=globruti.c globruti.h volby.h cotyvar.h chyba.h myslitel.h generato.h partie.h  hashtab.h lokruti.h strtah.h pole.h Makefile 
z_generato=generato.c generato.h volby.h cotyvar.h ohodnoc.h chyba.h globruti.h lokruti.h pole.h Makefile
z_lokruti=lokruti.c lokruti.h volby.h cotyvar.h generato.h hashtab.h dosio.h pole.h Makefile 
z_chyba=chyba.c chyba.h volby.h Makefile 
z_hashpech=hashpech.c hashpech.h volby.h ohodnoc.h Makefile
z_hashtab=hashtab.c hashtab.h volby.h chyba.h Makefile 
z_dosio=dosio.c dosio.h hashpech.h volby.h cotyvar.h myslitel.h globruti.c lokruti.c tabulky.h hashtab.h sachy.h ohodnoc.h pole.h Makefile 
z_kontrola=kontrola.c hashtab.h volby.h generato.h lokruti.h globruti.h Makefile
z_myslitel=myslitel.c myslitel.h volby.h generato.h globruti.h knihovna.h tabulky.h ticho.h Makefile
z_dejtah=dejtah.c dejtah.h myslitel.h volby.h generato.h globruti.h knihovna.h tabulky.h Makefile 
z_ohodnoc=ohodnoc.c ohodnoc.h myslitel.h volby.h cotyvar.h generato.h hashpech.h hashtab.h Makefile 
z_partie=partie.c partie.h chyba.h volby.h hashtab.h Makefile 
z_pgn=pgn.c pgn.h partie.h strtah.h cotyvar.h volby.h globruti.h  generato.h Makefile 
z_sachy=sachy.c pgn.h sachy.h partie.h kontrola.h chyba.h myslitel.h knihovna.h hashtab.h ohodnoc.h cotyvar.h volby.h Makefile 
z_strtah=strtah.c strtah.h volby.h cotyvar.h Makefile 
z_tabulky=tabulky.c tabulky.h generato.h volby.h cotyvar.h sachy.h dosio.h myslitel.h lokruti.h globruti.h Makefile 
z_knihovna=knihovna.c knihovna.h cteni.h obk_ch.h cotyvar.h volby.h Makefile 
z_hash=hash.c hash.h cotyvar.h volby.h Makefile 
z_datastr=datastr.c datastr.h hash.h cotyvar.h volby.h Makefile 
z_cteni=cteni.c cteni.h obk_ch.h datastr.h hash.h cotyvar.h volby.h Makefile 
z_obk_ch=obk_ch.c obk_ch.h Makefile 
z_scio=scio.c scio.h globruti.h dosio.h volby.h cotyvar.h chyba.h myslitel.h generato.h Makefile
z_testy=testy.c cotyvar.h volby.h partie.h Makefile

# totez pro specificke windowsi soubory

z_winio=winio.c winio.h Makefile
z_winmain=winmain.c winmain.h Makefile
z_winpos=winpos.c winpos.h Makefile
z_winmysl=winmysl.c winmysl.h Makefile
z_winsachy=winsachy.res Makefile
z_partiar=partiar.c partiar.h Makefile
z_dlg=dlg.c dlg.h Makefile
z_dlg=promdlg.c promdlg.h Makefile

${outdir}/winio.o: ${z_winio}
	${cc} winio.c -o ${outdir}/winio.o ${volby}

${outdir}/winmain.o: ${z_winmain}
	${cc} winmain.c -o ${outdir}/winmain.o ${volby}

${outdir}/winpos.o: ${z_winpos}
	${cc} winpos.c -o ${outdir}/winpos.o ${volby}
	
${outdir}/winmysl.o: ${z_winmysl}
	${cc} winmysl.c -o ${outdir}/winmysl.o ${volby}

${outdir}/partiar.o: ${z_partiar}
	${cc} partiar.c -o ${outdir}/partiar.o ${volby}

${outdir}/dlg.o: ${z_dlg}
	${cc} dlg.c -o ${outdir}/dlg.o ${volby}

${outdir}/promdlg.o: ${z_promdlg}
	${cc} promdlg.c -o ${outdir}/promdlg.o ${volby}
	
${outdir}/winsachy.o: ${z_winsachy}
	${windres} winsachy.res -o ${outdir}/winsachy.o


${outdir}/soubvv.o: ${z_sobvv}
	${cc} soubvv.c -o ${outdir}/soubvv.o ${volby}
	
${outdir}/cotyvar.o: ${z_cotyvar} 
	${cc} cotyvar.c -o ${outdir}/cotyvar.o ${volby}
	
${outdir}/globruti.o: ${z_globruti}
	${cc} globruti.c -o ${outdir}/globruti.o ${volby}

${outdir}/lokruti.o: ${z_lokruti}
	${cc} lokruti.c -o ${outdir}/lokruti.o ${volby}

${outdir}/generato.o: ${z_generato}
	${cc} generato.c -o ${outdir}/generato.o ${volby}

${outdir}/chyba.o: ${z_chyba}
	${cc} chyba.c -o ${outdir}/chyba.o ${volby}

${outdir}/dosio.o: ${z_dosio}
	${cc} dosio.c -o ${outdir}/dosio.o ${volby}

${outdir}/hashpech.o: ${z_hashpech}
	${cc} hashpech.c -o ${outdir}/hashpech.o ${volby}

${outdir}/hashtab.o: ${z_hashtab}
	${cc} hashtab.c -o ${outdir}/hashtab.o ${volby}

${outdir}/kontrola.o: ${z_kontrola}
	${cc} kontrola.c -o ${outdir}/kontrola.o ${volby}

${outdir}/myslitel.o: ${z_myslitel}
	${cc} myslitel.c -o ${outdir}/myslitel.o ${volby}

${outdir}/ticho.o: ${z_ticho}
	${cc} ticho.c -o ${outdir}/ticho.o ${volby}

${outdir}/dejtah.o: ${z_dejtah}
	${cc} dejtah.c -o ${outdir}/dejtah.o ${volby}

${outdir}/ohodnoc.o: ${z_ohodnoc}
	${cc} ohodnoc.c -o ${outdir}/ohodnoc.o ${volby}

${outdir}/partie.o: ${z_partie}
	${cc} partie.c -o ${outdir}/partie.o ${volby}

${outdir}/pgn.o: ${z_pgn}
	${cc} pgn.c -o ${outdir}/pgn.o ${volby}

${outdir}/sachy.o: ${z_sachy}
	${cc} sachy.c -o ${outdir}/sachy.o ${volby}

${outdir}/strtah.o: ${z_strtah}
	${cc} strtah.c -o ${outdir}/strtah.o ${volby}

${outdir}/tabulky.o: ${z_tabulky}
	${cc} tabulky.c -o ${outdir}/tabulky.o ${volby}

${outdir}/knihovna.o: ${z_knihovna}
	${cc} knihovna.c -o ${outdir}/knihovna.o ${volby}

${outdir}/hash.o: ${z_hash}
	${cc} hash.c -o ${outdir}/hash.o ${volby}

${outdir}/datastr.o: ${z_datastr}
	${cc} datastr.c -o ${outdir}/datastr.o ${volby}

${outdir}/cteni.o: ${z_cteni}
	${cc} cteni.c -o ${outdir}/cteni.o ${volby}

${outdir}/obk_ch.o: ${z_obk_ch}
	${cc} obk_ch.c -o ${outdir}/obk_ch.o ${volby}

${outdir}/scio.o: ${z_scio}
	${cc} scio.c -o ${outdir}/scio.o ${volby}

${outdir}/testy.o: ${z_scio}
	${cc} testy.c -o ${outdir}/testy.o ${volby}
