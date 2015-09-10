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

EXE = delogo.auf
OBJ = filter.obj optdlg.obj editdlg.obj
RES = delogo.res


all: $(EXE)


$(EXE): $(OBJ) $(RES)
	$(LN) $(LFLAG) $(OBJ)
	$(RL) -fe$(EXE) $(RES)

filter.obj: filter.c filter.h logo.h optdlg.h
	$(CC) $(CFLAG) filter.c

optdlg.obj: optdlg.c optdlg.h filter.h logo.h resource.h
	$(CC) $(CFLAG) optdlg.c

editdlg.obj: editdlg.c editdlg.h resource.h
	$(CC) $(CFLAG) editdlg.c

$(RES): delogo.rc resource.h
	$(RC) $(RFLAG) delogo.rc
