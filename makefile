######################################################################
#	MakeFile for Borland C/C++ Compiler
#

CC  = bcc32
LN  = bcc32
RL  = brc32
RC  = brcc32

CFLAG = -c
LFLAG = -tWD -e$(EXE)
RFLAG = 

EXE = delogo.auf
OBJ = filter.obj optdlg.obj
RES = delogo.res


all: $(EXE)


$(EXE): $(OBJ) $(RES)
	$(LN) $(LFLAG) $(OBJ)
	$(RL) -fe$(EXE) $(RES)

filter.obj: filter.c filter.h logo.h optdlg.h
	$(CC) $(CFLAG) filter.c

optdlg.obj: optdlg.c optdlg.h filter.h logo.h
	$(CC) $(CFLAG) optdlg.c

$(RES): delogo.rc
	$(RC) $(RFLAG) delogo.rc
