.SUFFIXES = .cpp .obj

.cpp.obj:
	bcc32 -c -w -v $<

all: generate.exe recreate.exe

generate.exe: generate.obj myrand.obj
	bcc32 -v -e$@ generate.obj myrand.obj

recreate.obj: hbase66k.cpp

recreate.exe: recreate.obj myrand.obj
	bcc32 -v -e$@ recreate.obj myrand.obj

CFLAGS = -O2 -Wall -W -s

gcc_seq:
	gcc $(CFLAGS) -o generate.exe generate.cpp myrand.cpp
	generate
	zip BASES.ZIP BASES.DAT
	gcc $(CFLAGS) -o recreate.exe recreate.cpp myrand.cpp
	recreate
	cmp FULL.DAT RECR.DAT
