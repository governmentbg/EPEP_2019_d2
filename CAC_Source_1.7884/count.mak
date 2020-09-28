all:
	wc -l *.cpp *.c *.def *.h *.rc *.rh lmupdate/*.cpp lmupdate/*.def lmupdate/*.rc connect/*.cpp connect/*.def connect/*.h connect/*.rc connect/*.rh _pre/*.pre text/*.fm? htm/*.htm htm/*.xml

pre:
	wc -l _pre/*.pre

cxx:
	wc -l *.cpp *.c *.def *.h *.rc *.rh lmupdate/*.cpp lmupdate/*.def lmupdate/*.rc connect/*.cpp connect/*.def connect/*.h connect/*.rc connect/*.rh

format:
	wc -l text/*.fm?

office:
	wc -l htm/*.htm htm/*.xml

obsolete:
	wc -l obsolete.cpp obsolete/*.*
