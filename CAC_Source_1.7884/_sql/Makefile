VPATH = ../_pre

CON = index.sql common_.sql common.sql common~.sql appeal_.sql admin_.sql area_.sql special_.sql \
	military_.sql region_.sql identify.sql sbe_desc.sql uclp.sql street.sql profile.sql
DEF = nom.sql doc.sql xfer.sql admin.sql appeal.sql area.sql special.sql military.sql region.sql
UPD = update.sql test_upd.sql
PRE = cpp -P -o

all: $(CON) $(DEF) $(UPD)

appeal_.sql: ../_pre/__upsb_p.sql ../_pre/__inar_p.sql
admin_.sql: ../_pre/__upsb_d.sql ../_pre/__inar_d.sql
area_.sql: ../_pre/__inar_a.sql ../_pre/__upsb_a.sql
special_.sql: ../_pre/__upsb_s.sql ../_pre/__inar_s.sql
military_.sql: ../_pre/__upsb_m.sql ../_pre/__inar_m.sql
region_.sql: ../_pre/__upsb_r.sql ../_pre/__inar_r.sql

../_pre/sbe_desc.pre: ../_pre/sbe_desc.sed ../_pre/sbe_desc.txt Makefile
	echo DELETE FROM T_SBE_DESC; > sbe_desc.pre
	echo COMMIT WORK; >> sbe_desc.pre
	sed -f ../_pre/sbe_desc.sed ../_pre/sbe_desc.txt >> sbe_desc.pre
	mv sbe_desc.pre ../_pre/sbe_desc.pre

$(DEF) $(UPD): shared.pre appname.pre

$(CON) $(DEF): %.sql : %.pre Makefile
	cpp -P -o $@ $<

$(UPD): %.sql : %.pre Makefile
	cpp -P -o $@ $<

# | sed -e "s/\^ /;\r\n/g" -e "s/ , /, /g" > $@

clean:
	rm -f *.sql
