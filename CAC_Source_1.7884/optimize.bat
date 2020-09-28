if "%1" == "" optimize appname.gdb
if exist appname.gbk goto eexist
if not exist d:\court\appname\%1 goto enoent

gbak -b -user ???????? -password ???????? 127.0.0.1:d:\court\appname\%1 appname.gbk
if errorlevel 1 goto final
del %1
gbak -r -user ???????? -password ???????? appname.gbk 127.0.0.1:d:\court\appname\%1
if errorlevel 1 goto final
del appname.gbk
goto final

:eexist
echo appname.gbk already exists 1>&2
goto final

:enoent
echo d:\court\appname\%1 does not exist 1>&2

:final
