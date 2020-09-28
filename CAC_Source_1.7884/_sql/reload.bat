if "%1" == "appeal" goto valid1
if "%1" == "admin" goto valid1
if "%1" == "area" goto valid1
if "%1" == "special" goto valid1
if "%1" == "military" goto valid1
if "%1" == "region" goto valid1
echo %0: invalid or missing court type 1>&2
goto final

:valid1
make
set xinput=-i common~.sql
if not "%3" == "" set xinput=%xinput% -i "%3"
set xinput=%xinput% -i common.sql -i %1.sql
if "%2" == "ib" goto ib
if "%2" == "pg" goto pg
echo %0: invalid or missing database type 1>&2
goto unset

:ib
..\xsql\xsql --%2 %xinput% ..\appname.%1.gdb
goto unset

:pg
..\xsql\xsql --%2 %xinput%

:unset
set xinput=

:final
