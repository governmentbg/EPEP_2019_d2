if "%1" == "appeal" goto valid1
if "%1" == "admin" goto valid1
if "%1" == "area" goto valid1
if "%1" == "special" goto valid1
if "%1" == "military" goto valid1
if "%1" == "region" goto valid1
echo %0: invalid or missing court type 1>&2
goto final

:valid1
if "%9" == "" goto valid2
echo %0: maximum 8 extra arguments supported 1>&2
goto final

:valid2
call strip
sleep 2
if exist appname.%1.zip del appname.%1.zip
zip -DX appname.%1.zip appname.exe _sql\%1.sql _sql\common.sql _sql\common~.sql _sql\update.sql text\* htm\* %2 %3 %4 %5 %6 %7 %8
copy appname.%1.zip \\dc1.var.is-bg.net\Shares\Groups\SAS

:final
