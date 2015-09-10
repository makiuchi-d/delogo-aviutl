######################################################################
#	MakeFile for Borland C/C++ Compiler
#

CC  = bcc32
LN  = bcc32
RL  = brc32
RC  = brcc32

CFLAG = -c -O1 -O2 -Oc -Oi -Ov
LFLAG = -tWD -e$(EXE) -O1 -O2
RFLAG = 

EXE = logoscan.auf
OBJ = filter.obj scanpix.obj resultdlg.obj abort.obj
RES = logoscan.res


all: $(EXE)


$(EXE): $(OBJ) $(RES)
	$(LN) $(LFLAG) $(OBJ)
	$(RL) -fe$(EXE) $(RES)

filter.obj: filter.cpp ..\filter.h ..\logo.h scanpix.h resultdlg.h abort.h
	$(CC) $(CFLAG) filter.cpp

scanpix.obj: scanpix.cpp ..\filter.h ..\logo.h scanpix.h approxim.h
	$(CC) $(CFLAG) scanpix.cpp

resultdlg.obj: resultdlg.c resultdlg.h resource.h ..\filter.h ..\logo.h
	$(CC) $(CFLAG) resultdlg.c

abort.obj: abort.cpp abort.h resource.h
	$(CC) $(CFLAG) abort.cpp

$(RES): logoscan.rc
	$(RC) $(RFLAG) logoscan.rc

