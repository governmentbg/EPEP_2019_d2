.SUFFIXES: .cpp.o

all: lmupdate.exe

.cpp.o:
	gcc -O2 -Wall -Wextra -DCINTERFACE -c -o $@ $<

lmupdate.res: lmupdate.rc Makefile
	windres -O coff lmupdate.rc lmupdate.res

DEPENDS = all.h lmupdate.h vprinter.h memblock.h message.h lmutil.h Makefile

message.o: message.cpp $(DEPENDS)
lmutil.o: lmutil.cpp $(DEPENDS)
lmupdate.o: lmutil.cpp $(DEPENDS)

lmupdate.exe: lmupdate.o lmutil.o message.o lmupdate.res Makefile
	gcc -O2 -Wall -W -DCINTERFACE -s -o lmupdate.exe lmupdate.o lmutil.o message.o lmupdate.res -lole32 -lversion

clean:
	rm -f *.o *.res *.exe
