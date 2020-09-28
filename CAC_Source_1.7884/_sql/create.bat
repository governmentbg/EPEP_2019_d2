if "%1" == "appeal" goto valid1
if "%1" == "admin" goto valid1
if "%1" == "area" goto valid1
if "%1" == "special" goto valid1
if "%1" == "military" goto valid1
if "%1" == "region" goto valid1
if "%1" == "struct" goto valid1
echo %0: invalid or missing court type 1>&2
goto final

:valid1
make
if errorlevel == 1 goto final
set xinput=-i nom.sql -i doc.sql -i xfer.sql -i gener_%2%.sql -i index.sql
if "%1" == "struct" goto struct
set xinput=%xinput% -i uclp.sql -i street.sql -i common.sql -i common_.sql -i sbe_desc.sql -i ..\eisnom\crime.sql
set xinput=%xinput% -i ..\eisnom\depart.sql -i ..\eisnom\eispp.sql -i ..\eisnom\place.sql -i ..\eisnom\fixes.sql
set xinput=%xinput% -i %1.sql -i %1_.sql
if "%1" == "appeal" set xinput=%xinput% -i appc_%2%.sql

:struct
if "%2" == "ib" goto ib
if "%2" == "pg" goto pg
echo %0: invalid or missing database type 1>&2
goto unset

:ib
if exist ..\AppName.gdb del ..\AppName.gdb
..\xsql\xsql --ib -i creat_ib.sql %xinput%
goto unset

:pg
..\xsql\xsql --pg -d postgres -i creat_pg.sql %xinput% -i user_pg.sql

:unset
set xinput=

:final
