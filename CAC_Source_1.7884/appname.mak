#
# Borland C++ IDE generated makefile
# Generated 28.11.2018 at 10:45:33 
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCC32   = Bcc32 +BccW32.cfg 
BCC32I  = Bcc32i +BccW32.cfg 
TLINK32 = TLink32
TLIB    = TLib
BRC32   = Brc32
TASM32  = Tasm32
#
# IDE macros
#


#
# Options
#
IDE_LinkFLAGS32 =  -LC:\BC5\LIB
IDE_ResFLAGS32 = 
CompLocalOptsAtW16_appname = 
LinkerLocalOptsAtW16_appname =  -LC:\BC5\LIB
ResLocalOptsAtW16_appname = 
BLocalOptsAtW16_appname = 
Comp = $(CompLocalOptsAtW16_appname)
Linker = $(LinkerLocalOptsAtW16_appname)
Res = $(ResLocalOptsAtW16_appname)
B = $(BLocalOptsAtW16_appname)
LinkerLocalOptsAtW32_dbappnamedexe =  -LC:\BC5\LIB;C:\BDE32\LIB -went -wdup -wdef -w-imt -wbdl -wsrf -wmsk -Tpe -aa -V4.0 -c
ResLocalOptsAtW32_dbappnamedexe = 
BLocalOptsAtW32_dbappnamedexe = 
CompInheritOptsAt_dbappnamedexe = -IC:\BC5\INCLUDE;C:\BDE32\INCLUDE;C:\BC5\INCLUDE\MFC;.\COMMON -DINC_OLE2;STRICT;_OWLPCH;INC_OLE2;_RTLDLL;_BIDSDLL;_OWLDLL;CINTERFACE;COBJMACROS;
LinkerInheritOptsAt_dbappnamedexe = -B:400000 -x
LinkerOptsAt_dbappnamedexe = $(LinkerLocalOptsAtW32_dbappnamedexe)
ResOptsAt_dbappnamedexe = $(ResLocalOptsAtW32_dbappnamedexe)
BOptsAt_dbappnamedexe = $(BLocalOptsAtW32_dbappnamedexe)

#
# Dependency List
#
Dep_appname = \
   .\appname.exe

appname : BccW32.cfg $(Dep_appname)
  echo MakeNode

Dep_dbappnamedexe = \
   TMP\cacconnect.obj\
   TMP\abcbook.obj\
   TMP\aboutdialog.obj\
   TMP\accounts.obj\
   TMP\action.obj\
   TMP\addresses.obj\
   TMP\alias.obj\
   TMP\aliases.obj\
   TMP\aliaslistbox.obj\
   TMP\alterstreetdialog.obj\
   TMP\alteruxndialog.obj\
   TMP\announces.obj\
   TMP\appealedbook.obj\
   TMP\appealinst.obj\
   TMP\appjudrep.obj\
   TMP\appnameapp.obj\
   TMP\archivebook.obj\
   TMP\archives.obj\
   TMP\array.obj\
   TMP\articles.obj\
   TMP\attach.obj\
   TMP\bankrupts.obj\
   TMP\bloat.obj\
   TMP\book.obj\
   TMP\book634.obj\
   TMP\book39.obj\
   TMP\bookdialog.obj\
   TMP\bookinstancedialog.obj\
   TMP\bscourtrep.obj\
   TMP\bsrangerep.obj\
   TMP\c1rangerep.obj\
   TMP\c2courtrep.obj\
   TMP\cacdatabase.obj\
   TMP\cadrs.obj\
   TMP\charautolistface.obj\
   TMP\checkface.obj\
   TMP\checkout.obj\
   TMP\clonelistbox.obj\
   TMP\closedsessionbook.obj\
   TMP\columnlistwindow.obj\
   TMP\command.obj\
   TMP\composit.obj\
   TMP\config.obj\
   TMP\configure.obj\
   TMP\connectlistwindow.obj\
   TMP\const.obj\
   TMP\const1.obj\
   TMP\container.obj\
   TMP\copysidewindsdialog.obj\
   TMP\corrupts.obj\
   TMP\counter.obj\
   TMP\countries.obj\
   TMP\courtrep.obj\
   TMP\cp1251.obj\
   TMP\currences.obj\
   TMP\dangerep.obj\
   TMP\dangers.obj\
   TMP\data.obj\
   TMP\datetime.obj\
   TMP\decisionregister.obj\
   TMP\delete.obj\
   TMP\descbook.obj\
   TMP\divorcebook.obj\
   TMP\doc.obj\
   TMP\doublelistheader.obj\
   TMP\doublelistwindow.obj\
   TMP\editaddressdialog.obj\
   TMP\editconnectdialog.obj\
   TMP\editdialog.obj\
   TMP\editincomingdialog.obj\
   TMP\editsidewinddialog.obj\
   TMP\eisfaces.obj\
   TMP\eispp.obj\
   TMP\eispplaws.obj\
   TMP\eisxml.obj\
   TMP\electra.obj\
   TMP\electric.obj\
   TMP\eportal.obj\
   TMP\euher.obj\
   TMP\execlist.obj\
   TMP\executebook.obj\
   TMP\expensebook.obj\
   TMP\expenses.obj\
   TMP\export.obj\
   TMP\externlaws.obj\
   TMP\externsess.obj\
   TMP\faces.obj\
   TMP\filename.obj\
   TMP\filter.obj\
   TMP\firstset.obj\
   TMP\floatannounce.obj\
   TMP\floatarchive.obj\
   TMP\floatcombo.obj\
   TMP\floatconnect.obj\
   TMP\floatdialog.obj\
   TMP\floatelectric.obj\
   TMP\floatexpense.obj\
   TMP\floatimportant.obj\
   TMP\floatgroup.obj\
   TMP\floatindex.obj\
   TMP\floatjuror.obj\
   TMP\floatlog.obj\
   TMP\floatmovement.obj\
   TMP\floatoutreg.obj\
   TMP\floatpublication.obj\
   TMP\floatquery.obj\
   TMP\floatrequest.obj\
   TMP\floatsched.obj\
   TMP\floatsenders.obj\
   TMP\floatsession.obj\
   TMP\floatside.obj\
   TMP\floatsubjectdialog.obj\
   TMP\floatsubpoena.obj\
   TMP\floatsurround.obj\
   TMP\floatucnsperiod.obj\
   TMP\getgroup.obj\
   TMP\getitem.obj\
   TMP\getrecomposedialog.obj\
   TMP\group.obj\
   TMP\groupcombobox.obj\
   TMP\groupdialog.obj\
   TMP\grouplist.obj\
   TMP\grouplistbox.obj\
   TMP\grouplistcolumn.obj\
   TMP\grouplistdialog.obj\
   TMP\grouplistwindow.obj\
   TMP\halls.obj\
   TMP\heritages.obj\
   TMP\import.obj\
   TMP\inconside.obj\
   TMP\indocbook.obj\
   TMP\informeds.obj\
   TMP\inregbook.obj\
   TMP\inregs.obj\
   TMP\interact.obj\
   TMP\invalid.obj\
   TMP\invbookanno.obj\
   TMP\invbookdeliver.obj\
   TMP\invbooksubpoena.obj\
   TMP\joinstreetdialog.obj\
   TMP\joinstreets.obj\
   TMP\joinuxndialog.obj\
   TMP\judgerep.obj\
   TMP\jurors.obj\
   TMP\knownpersonlistbox.obj\
   TMP\knowns.obj\
   TMP\lawsuits.obj\
   TMP\lawsuitsidelistdialog.obj\
   TMP\legalaid.obj\
   TMP\letrangerep.obj\
   TMP\loadhall.obj\
   TMP\links.obj\
   TMP\log.obj\
   TMP\login.obj\
   TMP\longtextdialog.obj\
   TMP\mcac.obj\
   TMP\memblock.obj\
   TMP\message.obj\
   TMP\misc.obj\
   TMP\modifiers.obj\
   TMP\monthperioddialog.obj\
   TMP\msql.obj\
   TMP\mstr.obj\
   TMP\myrand.obj\
   TMP\niacollect.obj\
   TMP\nom.obj\
   TMP\oldsubjectlistbox.obj\
   TMP\office.obj\
   TMP\offields.obj\
   TMP\opensave.obj\
   TMP\originsidelistdialog.obj\
   TMP\originsidelistwindow.obj\
   TMP\output.obj\
   TMP\outregbook.obj\
   TMP\p1rangerep.obj\
   TMP\p2courtrep.obj\
   TMP\p24a1.obj\
   TMP\password.obj\
   TMP\payments.obj\
   TMP\perioddialog.obj\
   TMP\personal2.obj\
   TMP\pointer.obj\
   TMP\posterm.obj\
   TMP\printexe.obj\
   TMP\printarray.obj\
   TMP\printlist.obj\
   TMP\printout.obj\
   TMP\printsub.obj\
   TMP\process.obj\
   TMP\provebook.obj\
   TMP\proves.obj\
   TMP\pushbutton.obj\
   TMP\cacquery.obj\
   TMP\querybankhtml.obj\
   TMP\querydoc.obj\
   TMP\querypolice.obj\
   TMP\querywebhtml.obj\
   TMP\quickdialog.obj\
   TMP\quickface.obj\
   TMP\quicklist.obj\
   TMP\quicksearch.obj\
   TMP\quicksearchedit.obj\
   TMP\radioface.obj\
   TMP\randomact.obj\
   TMP\realities.obj\
   TMP\rangerep.obj\
   TMP\recusalbook.obj\
   TMP\rekindlaw.obj\
   TMP\renumber.obj\
   TMP\renumberdocdialog.obj\
   TMP\renumberlawdialog.obj\
   TMP\renumberside.obj\
   TMP\renumberstreetdialog.obj\
   TMP\renumberuxndialog.obj\
   TMP\report.obj\
   TMP\reportjudge.obj\
   TMP\requestbook.obj\
   TMP\requests.obj\
   TMP\resizedialog.obj\
   TMP\resizelistwindow.obj\
   TMP\resourcedialog.obj\
   TMP\rpmnorep.obj\
   TMP\scaledialog.obj\
   TMP\scheds.obj\
   TMP\selectdialog.obj\
   TMP\selectors.obj\
   TMP\selectsubjectslistbox.obj\
   TMP\senderlistbox.obj\
   TMP\senders.obj\
   TMP\sendlist.obj\
   TMP\sendout.obj\
   TMP\sesdrep.obj\
   TMP\sessions.obj\
   TMP\sesslist.obj\
   TMP\sesstimes.obj\
   TMP\showdialog.obj\
   TMP\showhtml.obj\
   TMP\sidewindlistdialog.obj\
   TMP\sidewindlistwindow.obj\
   TMP\slcreasons.obj\
   TMP\slowbook.obj\
   TMP\snprintf.obj\
   TMP\spawncmd.obj\
   TMP\statquery.obj\
   TMP\streetmap.obj\
   TMP\stringface.obj\
   TMP\subjectlistbox.obj\
   TMP\subjects.obj\
   TMP\subplist.obj\
   TMP\subpoenas.obj\
   TMP\surrounds.obj\
   TMP\table.obj\
   TMP\testfunc.obj\
   TMP\timebook.obj\
   TMP\timedlawsuit.obj\
   TMP\timedsession.obj\
   TMP\tmp.obj\
   TMP\tntdialog.obj\
   TMP\tntlistwindow.obj\
   TMP\transfer.obj\
   TMP\trouble.obj\
   TMP\troubledialog.obj\
   TMP\tryaddresssidelistwindow.obj\
   TMP\twinbutton.obj\
   TMP\ucns.obj\
   TMP\unappealed.obj\
   TMP\unwritten.obj\
   TMP\unwrittendialog.obj\
   TMP\update.obj\
   TMP\userfile.obj\
   TMP\userlistbox.obj\
   TMP\users.obj\
   TMP\utility.obj\
   TMP\utility1.obj\
   TMP\valid.obj\
   TMP\vmessage.obj\
   TMP\scanner.obj\
   TMP\sscanf.obj\
   TMP\vprinter.obj\
   TMP\waitthread.obj\
   TMP\waitwindow.obj\
   TMP\whatevertextbasedialog.obj\
   TMP\whatevertextviewdialog.obj\
   TMP\xfer.obj\
   TMP\winutils.obj\
   TMP\zzdn.obj\
   TMP\_abcbook.obj\
   TMP\_action.obj\
   TMP\_aplclmn.obj\
   TMP\_bankrup.obj\
   TMP\_checks.obj\
   TMP\_clmnarr.obj\
   TMP\_clmutil.obj\
   TMP\_clsesbk.obj\
   TMP\_column.obj\
   TMP\_compjcc.obj\
   TMP\_compose.obj\
   TMP\_comprp1.obj\
   TMP\_comprp2.obj\
   TMP\_const.obj\
   TMP\_convert.obj\
   TMP\_data.obj\
   TMP\_defjdgm.obj\
   TMP\_desjudg.obj\
   TMP\_deskbk.obj\
   TMP\_doc.obj\
   TMP\_extract.obj\
   TMP\_faces.obj\
   TMP\_flags.obj\
   TMP\_grammar.obj\
   TMP\_html.obj\
   TMP\_ilieva.obj\
   TMP\_inregbk.obj\
   TMP\_judgef.obj\
   TMP\_judgesc.obj\
   TMP\_krachol.obj\
   TMP\_lawchk.obj\
   TMP\_linkwrt.obj\
   TMP\_lnkwind.obj\
   TMP\_maillst.obj\
   TMP\_message.obj\
   TMP\_misc.obj\
   TMP\_mixed.obj\
   TMP\_nom.obj\
   TMP\_notify.obj\
   TMP\_outfile.obj\
   TMP\_outputs.obj\
   TMP\_outrgbk.obj\
   TMP\_pdcitjc.obj\
   TMP\_penalty.obj\
   TMP\_pljchk.obj\
   TMP\_pr634tl.obj\
   TMP\_prcsbd.obj\
   TMP\_proutpu.obj\
   TMP\_prrpbd.obj\
   TMP\_prtbkd.obj\
   TMP\_reqdia.obj\
   TMP\_rolls.obj\
   TMP\_rpdata.obj\
   TMP\_rsetbk.obj\
   TMP\_section.obj\
   TMP\_sedistr.obj\
   TMP\_sesschk.obj\
   TMP\_sets.obj\
   TMP\_sqlcrit.obj\
   TMP\_subpoen.obj\
   TMP\_sumtd.obj\
   TMP\_tbclmns.obj\
   TMP\_tbsthrd.obj\
   TMP\_timebk.obj\
   TMP\_tl634bk.obj\
   TMP\_toname.obj\
   TMP\_toostrm.obj\
   TMP\_tostrng.obj\
   TMP\_tscntou.obj\
   TMP\_utility.obj\
   TMP\_zdravk.obj\
   TMP\_zzdncol.obj\
   TMP\appnameapp.res\
   TMP\appnameapp1.res\
   TMP\_appnameapp.res\
   appnameapp.def\
   C:\BDE32\LIB\idapi32.lib\
   odbcv3.lib\
   odbccpv3.lib\
   appname.exe

.\appname.exe :
  $(TLINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LinkerOptsAt_dbappnamedexe) $(LinkerInheritOptsAt_dbappnamedexe) +
C:\BC5\LIB\c0w32.obj+
TMP\cacconnect.obj+
TMP\abcbook.obj+
TMP\aboutdialog.obj+
TMP\accounts.obj+
TMP\action.obj+
TMP\addresses.obj+
TMP\alias.obj+
TMP\aliases.obj+
TMP\aliaslistbox.obj+
TMP\alterstreetdialog.obj+
TMP\alteruxndialog.obj+
TMP\announces.obj+
TMP\appealedbook.obj+
TMP\appealinst.obj+
TMP\appjudrep.obj+
TMP\appnameapp.obj+
TMP\archivebook.obj+
TMP\archives.obj+
TMP\array.obj+
TMP\articles.obj+
TMP\attach.obj+
TMP\bankrupts.obj+
TMP\bloat.obj+
TMP\book.obj+
TMP\book634.obj+
TMP\book39.obj+
TMP\bookdialog.obj+
TMP\bookinstancedialog.obj+
TMP\bscourtrep.obj+
TMP\bsrangerep.obj+
TMP\c1rangerep.obj+
TMP\c2courtrep.obj+
TMP\cacdatabase.obj+
TMP\cadrs.obj+
TMP\charautolistface.obj+
TMP\checkface.obj+
TMP\checkout.obj+
TMP\clonelistbox.obj+
TMP\closedsessionbook.obj+
TMP\columnlistwindow.obj+
TMP\command.obj+
TMP\composit.obj+
TMP\config.obj+
TMP\configure.obj+
TMP\connectlistwindow.obj+
TMP\const.obj+
TMP\const1.obj+
TMP\container.obj+
TMP\copysidewindsdialog.obj+
TMP\corrupts.obj+
TMP\counter.obj+
TMP\countries.obj+
TMP\courtrep.obj+
TMP\cp1251.obj+
TMP\currences.obj+
TMP\dangerep.obj+
TMP\dangers.obj+
TMP\data.obj+
TMP\datetime.obj+
TMP\decisionregister.obj+
TMP\delete.obj+
TMP\descbook.obj+
TMP\divorcebook.obj+
TMP\doc.obj+
TMP\doublelistheader.obj+
TMP\doublelistwindow.obj+
TMP\editaddressdialog.obj+
TMP\editconnectdialog.obj+
TMP\editdialog.obj+
TMP\editincomingdialog.obj+
TMP\editsidewinddialog.obj+
TMP\eisfaces.obj+
TMP\eispp.obj+
TMP\eispplaws.obj+
TMP\eisxml.obj+
TMP\electra.obj+
TMP\electric.obj+
TMP\eportal.obj+
TMP\euher.obj+
TMP\execlist.obj+
TMP\executebook.obj+
TMP\expensebook.obj+
TMP\expenses.obj+
TMP\export.obj+
TMP\externlaws.obj+
TMP\externsess.obj+
TMP\faces.obj+
TMP\filename.obj+
TMP\filter.obj+
TMP\firstset.obj+
TMP\floatannounce.obj+
TMP\floatarchive.obj+
TMP\floatcombo.obj+
TMP\floatconnect.obj+
TMP\floatdialog.obj+
TMP\floatelectric.obj+
TMP\floatexpense.obj+
TMP\floatimportant.obj+
TMP\floatgroup.obj+
TMP\floatindex.obj+
TMP\floatjuror.obj+
TMP\floatlog.obj+
TMP\floatmovement.obj+
TMP\floatoutreg.obj+
TMP\floatpublication.obj+
TMP\floatquery.obj+
TMP\floatrequest.obj+
TMP\floatsched.obj+
TMP\floatsenders.obj+
TMP\floatsession.obj+
TMP\floatside.obj+
TMP\floatsubjectdialog.obj+
TMP\floatsubpoena.obj+
TMP\floatsurround.obj+
TMP\floatucnsperiod.obj+
TMP\getgroup.obj+
TMP\getitem.obj+
TMP\getrecomposedialog.obj+
TMP\group.obj+
TMP\groupcombobox.obj+
TMP\groupdialog.obj+
TMP\grouplist.obj+
TMP\grouplistbox.obj+
TMP\grouplistcolumn.obj+
TMP\grouplistdialog.obj+
TMP\grouplistwindow.obj+
TMP\halls.obj+
TMP\heritages.obj+
TMP\import.obj+
TMP\inconside.obj+
TMP\indocbook.obj+
TMP\informeds.obj+
TMP\inregbook.obj+
TMP\inregs.obj+
TMP\interact.obj+
TMP\invalid.obj+
TMP\invbookanno.obj+
TMP\invbookdeliver.obj+
TMP\invbooksubpoena.obj+
TMP\joinstreetdialog.obj+
TMP\joinstreets.obj+
TMP\joinuxndialog.obj+
TMP\judgerep.obj+
TMP\jurors.obj+
TMP\knownpersonlistbox.obj+
TMP\knowns.obj+
TMP\lawsuits.obj+
TMP\lawsuitsidelistdialog.obj+
TMP\legalaid.obj+
TMP\letrangerep.obj+
TMP\loadhall.obj+
TMP\links.obj+
TMP\log.obj+
TMP\login.obj+
TMP\longtextdialog.obj+
TMP\mcac.obj+
TMP\memblock.obj+
TMP\message.obj+
TMP\misc.obj+
TMP\modifiers.obj+
TMP\monthperioddialog.obj+
TMP\msql.obj+
TMP\mstr.obj+
TMP\myrand.obj+
TMP\niacollect.obj+
TMP\nom.obj+
TMP\oldsubjectlistbox.obj+
TMP\office.obj+
TMP\offields.obj+
TMP\opensave.obj+
TMP\originsidelistdialog.obj+
TMP\originsidelistwindow.obj+
TMP\output.obj+
TMP\outregbook.obj+
TMP\p1rangerep.obj+
TMP\p2courtrep.obj+
TMP\p24a1.obj+
TMP\password.obj+
TMP\payments.obj+
TMP\perioddialog.obj+
TMP\personal2.obj+
TMP\pointer.obj+
TMP\posterm.obj+
TMP\printexe.obj+
TMP\printarray.obj+
TMP\printlist.obj+
TMP\printout.obj+
TMP\printsub.obj+
TMP\process.obj+
TMP\provebook.obj+
TMP\proves.obj+
TMP\pushbutton.obj+
TMP\cacquery.obj+
TMP\querybankhtml.obj+
TMP\querydoc.obj+
TMP\querypolice.obj+
TMP\querywebhtml.obj+
TMP\quickdialog.obj+
TMP\quickface.obj+
TMP\quicklist.obj+
TMP\quicksearch.obj+
TMP\quicksearchedit.obj+
TMP\radioface.obj+
TMP\randomact.obj+
TMP\realities.obj+
TMP\rangerep.obj+
TMP\recusalbook.obj+
TMP\rekindlaw.obj+
TMP\renumber.obj+
TMP\renumberdocdialog.obj+
TMP\renumberlawdialog.obj+
TMP\renumberside.obj+
TMP\renumberstreetdialog.obj+
TMP\renumberuxndialog.obj+
TMP\report.obj+
TMP\reportjudge.obj+
TMP\requestbook.obj+
TMP\requests.obj+
TMP\resizedialog.obj+
TMP\resizelistwindow.obj+
TMP\resourcedialog.obj+
TMP\rpmnorep.obj+
TMP\scaledialog.obj+
TMP\scheds.obj+
TMP\selectdialog.obj+
TMP\selectors.obj+
TMP\selectsubjectslistbox.obj+
TMP\senderlistbox.obj+
TMP\senders.obj+
TMP\sendlist.obj+
TMP\sendout.obj+
TMP\sesdrep.obj+
TMP\sessions.obj+
TMP\sesslist.obj+
TMP\sesstimes.obj+
TMP\showdialog.obj+
TMP\showhtml.obj+
TMP\sidewindlistdialog.obj+
TMP\sidewindlistwindow.obj+
TMP\slcreasons.obj+
TMP\slowbook.obj+
TMP\snprintf.obj+
TMP\spawncmd.obj+
TMP\statquery.obj+
TMP\streetmap.obj+
TMP\stringface.obj+
TMP\subjectlistbox.obj+
TMP\subjects.obj+
TMP\subplist.obj+
TMP\subpoenas.obj+
TMP\surrounds.obj+
TMP\table.obj+
TMP\testfunc.obj+
TMP\timebook.obj+
TMP\timedlawsuit.obj+
TMP\timedsession.obj+
TMP\tmp.obj+
TMP\tntdialog.obj+
TMP\tntlistwindow.obj+
TMP\transfer.obj+
TMP\trouble.obj+
TMP\troubledialog.obj+
TMP\tryaddresssidelistwindow.obj+
TMP\twinbutton.obj+
TMP\ucns.obj+
TMP\unappealed.obj+
TMP\unwritten.obj+
TMP\unwrittendialog.obj+
TMP\update.obj+
TMP\userfile.obj+
TMP\userlistbox.obj+
TMP\users.obj+
TMP\utility.obj+
TMP\utility1.obj+
TMP\valid.obj+
TMP\vmessage.obj+
TMP\scanner.obj+
TMP\sscanf.obj+
TMP\vprinter.obj+
TMP\waitthread.obj+
TMP\waitwindow.obj+
TMP\whatevertextbasedialog.obj+
TMP\whatevertextviewdialog.obj+
TMP\xfer.obj+
TMP\winutils.obj+
TMP\zzdn.obj+
TMP\_abcbook.obj+
TMP\_action.obj+
TMP\_aplclmn.obj+
TMP\_bankrup.obj+
TMP\_checks.obj+
TMP\_clmnarr.obj+
TMP\_clmutil.obj+
TMP\_clsesbk.obj+
TMP\_column.obj+
TMP\_compjcc.obj+
TMP\_compose.obj+
TMP\_comprp1.obj+
TMP\_comprp2.obj+
TMP\_const.obj+
TMP\_convert.obj+
TMP\_data.obj+
TMP\_defjdgm.obj+
TMP\_desjudg.obj+
TMP\_deskbk.obj+
TMP\_doc.obj+
TMP\_extract.obj+
TMP\_faces.obj+
TMP\_flags.obj+
TMP\_grammar.obj+
TMP\_html.obj+
TMP\_ilieva.obj+
TMP\_inregbk.obj+
TMP\_judgef.obj+
TMP\_judgesc.obj+
TMP\_krachol.obj+
TMP\_lawchk.obj+
TMP\_linkwrt.obj+
TMP\_lnkwind.obj+
TMP\_maillst.obj+
TMP\_message.obj+
TMP\_misc.obj+
TMP\_mixed.obj+
TMP\_nom.obj+
TMP\_notify.obj+
TMP\_outfile.obj+
TMP\_outputs.obj+
TMP\_outrgbk.obj+
TMP\_pdcitjc.obj+
TMP\_penalty.obj+
TMP\_pljchk.obj+
TMP\_pr634tl.obj+
TMP\_prcsbd.obj+
TMP\_proutpu.obj+
TMP\_prrpbd.obj+
TMP\_prtbkd.obj+
TMP\_reqdia.obj+
TMP\_rolls.obj+
TMP\_rpdata.obj+
TMP\_rsetbk.obj+
TMP\_section.obj+
TMP\_sedistr.obj+
TMP\_sesschk.obj+
TMP\_sets.obj+
TMP\_sqlcrit.obj+
TMP\_subpoen.obj+
TMP\_sumtd.obj+
TMP\_tbclmns.obj+
TMP\_tbsthrd.obj+
TMP\_timebk.obj+
TMP\_tl634bk.obj+
TMP\_toname.obj+
TMP\_toostrm.obj+
TMP\_tostrng.obj+
TMP\_tscntou.obj+
TMP\_utility.obj+
TMP\_zdravk.obj+
TMP\_zzdncol.obj
$<,$*
C:\BDE32\LIB\idapi32.lib+
odbcv3.lib+
odbccpv3.lib+
C:\BC5\LIB\owlwf.lib+
C:\BC5\LIB\bidsf.lib+
C:\BC5\LIB\ole2w32.lib+
C:\BC5\LIB\import32.lib+
C:\BC5\LIB\cw32.lib
appnameapp.def
TMP\appnameapp.res+
TMP\appnameapp1.res+
TMP\_appnameapp.res

|
TMP\cacconnect.obj :  cacconnect.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ cacconnect.cpp
|

TMP\abcbook.obj :  abcbook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ abcbook.cpp
|

TMP\aboutdialog.obj :  aboutdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ aboutdialog.cpp
|

TMP\accounts.obj :  accounts.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ accounts.cpp
|

TMP\action.obj :  action.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ action.cpp
|

TMP\addresses.obj :  addresses.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ addresses.cpp
|

TMP\alias.obj :  alias.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ alias.cpp
|

TMP\aliases.obj :  aliases.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ aliases.cpp
|

TMP\aliaslistbox.obj :  aliaslistbox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ aliaslistbox.cpp
|

TMP\alterstreetdialog.obj :  alterstreetdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ alterstreetdialog.cpp
|

TMP\alteruxndialog.obj :  alteruxndialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ alteruxndialog.cpp
|

TMP\announces.obj :  announces.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ announces.cpp
|

TMP\appealedbook.obj :  appealedbook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ appealedbook.cpp
|

TMP\appealinst.obj :  appealinst.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ appealinst.cpp
|

TMP\appjudrep.obj :  appjudrep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ appjudrep.cpp
|

TMP\appnameapp.obj :  appnameapp.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ appnameapp.cpp
|

TMP\archivebook.obj :  archivebook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ archivebook.cpp
|

TMP\archives.obj :  archives.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ archives.cpp
|

TMP\array.obj :  array.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ array.cpp
|

TMP\articles.obj :  articles.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ articles.cpp
|

TMP\attach.obj :  attach.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ attach.cpp
|

TMP\bankrupts.obj :  bankrupts.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ bankrupts.cpp
|

TMP\bloat.obj :  bloat.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ bloat.cpp
|

TMP\book.obj :  book.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ book.cpp
|

TMP\book634.obj :  book634.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ book634.cpp
|

TMP\book39.obj :  book39.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ book39.cpp
|

TMP\bookdialog.obj :  bookdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ bookdialog.cpp
|

TMP\bookinstancedialog.obj :  bookinstancedialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ bookinstancedialog.cpp
|

TMP\bscourtrep.obj :  bscourtrep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ bscourtrep.cpp
|

TMP\bsrangerep.obj :  bsrangerep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ bsrangerep.cpp
|

TMP\c1rangerep.obj :  c1rangerep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ c1rangerep.cpp
|

TMP\c2courtrep.obj :  c2courtrep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ c2courtrep.cpp
|

TMP\cacdatabase.obj :  cacdatabase.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ cacdatabase.cpp
|

TMP\cadrs.obj :  cadrs.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ cadrs.cpp
|

TMP\charautolistface.obj :  charautolistface.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ charautolistface.cpp
|

TMP\checkface.obj :  checkface.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ checkface.cpp
|

TMP\checkout.obj :  checkout.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ checkout.cpp
|

TMP\clonelistbox.obj :  clonelistbox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ clonelistbox.cpp
|

TMP\closedsessionbook.obj :  closedsessionbook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ closedsessionbook.cpp
|

TMP\columnlistwindow.obj :  columnlistwindow.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ columnlistwindow.cpp
|

TMP\command.obj :  command.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ command.cpp
|

TMP\composit.obj :  composit.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ composit.cpp
|

TMP\config.obj :  config.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ config.cpp
|

TMP\configure.obj :  configure.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ configure.cpp
|

TMP\connectlistwindow.obj :  connectlistwindow.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ connectlistwindow.cpp
|

TMP\const.obj :  const.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ const.cpp
|

TMP\const1.obj :  const1.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ const1.cpp
|

TMP\container.obj :  container.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ container.cpp
|

TMP\copysidewindsdialog.obj :  copysidewindsdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ copysidewindsdialog.cpp
|

TMP\corrupts.obj :  corrupts.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ corrupts.cpp
|

TMP\counter.obj :  counter.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ counter.cpp
|

TMP\countries.obj :  countries.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ countries.cpp
|

TMP\courtrep.obj :  courtrep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ courtrep.cpp
|

TMP\cp1251.obj :  cp1251.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ cp1251.c
|

TMP\currences.obj :  currences.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ currences.cpp
|

TMP\dangerep.obj :  dangerep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ dangerep.cpp
|

TMP\dangers.obj :  dangers.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ dangers.cpp
|

TMP\data.obj :  data.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ data.cpp
|

TMP\datetime.obj :  datetime.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ datetime.cpp
|

TMP\decisionregister.obj :  decisionregister.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ decisionregister.cpp
|

TMP\delete.obj :  delete.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ delete.cpp
|

TMP\descbook.obj :  descbook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ descbook.cpp
|

TMP\divorcebook.obj :  divorcebook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ divorcebook.cpp
|

TMP\doc.obj :  doc.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ doc.cpp
|

TMP\doublelistheader.obj :  doublelistheader.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ doublelistheader.cpp
|

TMP\doublelistwindow.obj :  doublelistwindow.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ doublelistwindow.cpp
|

TMP\editaddressdialog.obj :  editaddressdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ editaddressdialog.cpp
|

TMP\editconnectdialog.obj :  editconnectdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ editconnectdialog.cpp
|

TMP\editdialog.obj :  editdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ editdialog.cpp
|

TMP\editincomingdialog.obj :  editincomingdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ editincomingdialog.cpp
|

TMP\editsidewinddialog.obj :  editsidewinddialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ editsidewinddialog.cpp
|

TMP\eisfaces.obj :  eisfaces.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ eisfaces.cpp
|

TMP\eispp.obj :  eispp.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ eispp.cpp
|

TMP\eispplaws.obj :  eispplaws.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ eispplaws.cpp
|

TMP\eisxml.obj :  eisxml.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ eisxml.cpp
|

TMP\electra.obj :  electra.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ electra.cpp
|

TMP\electric.obj :  electric.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ electric.cpp
|

TMP\eportal.obj :  eportal.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ eportal.cpp
|

TMP\euher.obj :  euher.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ euher.cpp
|

TMP\execlist.obj :  execlist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ execlist.cpp
|

TMP\executebook.obj :  executebook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ executebook.cpp
|

TMP\expensebook.obj :  expensebook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ expensebook.cpp
|

TMP\expenses.obj :  expenses.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ expenses.cpp
|

TMP\export.obj :  export.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ export.cpp
|

TMP\externlaws.obj :  externlaws.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ externlaws.cpp
|

TMP\externsess.obj :  externsess.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ externsess.cpp
|

TMP\faces.obj :  faces.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ faces.cpp
|

TMP\filename.obj :  filename.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ filename.cpp
|

TMP\filter.obj :  filter.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ filter.cpp
|

TMP\firstset.obj :  firstset.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ firstset.cpp
|

TMP\floatannounce.obj :  floatannounce.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatannounce.cpp
|

TMP\floatarchive.obj :  floatarchive.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatarchive.cpp
|

TMP\floatcombo.obj :  floatcombo.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatcombo.cpp
|

TMP\floatconnect.obj :  floatconnect.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatconnect.cpp
|

TMP\floatdialog.obj :  floatdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatdialog.cpp
|

TMP\floatelectric.obj :  floatelectric.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatelectric.cpp
|

TMP\floatexpense.obj :  floatexpense.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatexpense.cpp
|

TMP\floatimportant.obj :  floatimportant.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatimportant.cpp
|

TMP\floatgroup.obj :  floatgroup.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatgroup.cpp
|

TMP\floatindex.obj :  floatindex.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatindex.cpp
|

TMP\floatjuror.obj :  floatjuror.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatjuror.cpp
|

TMP\floatlog.obj :  floatlog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatlog.cpp
|

TMP\floatmovement.obj :  floatmovement.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatmovement.cpp
|

TMP\floatoutreg.obj :  floatoutreg.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatoutreg.cpp
|

TMP\floatpublication.obj :  floatpublication.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatpublication.cpp
|

TMP\floatquery.obj :  floatquery.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatquery.cpp
|

TMP\floatrequest.obj :  floatrequest.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatrequest.cpp
|

TMP\floatsched.obj :  floatsched.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatsched.cpp
|

TMP\floatsenders.obj :  floatsenders.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatsenders.cpp
|

TMP\floatsession.obj :  floatsession.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatsession.cpp
|

TMP\floatside.obj :  floatside.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatside.cpp
|

TMP\floatsubjectdialog.obj :  floatsubjectdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatsubjectdialog.cpp
|

TMP\floatsubpoena.obj :  floatsubpoena.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatsubpoena.cpp
|

TMP\floatsurround.obj :  floatsurround.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatsurround.cpp
|

TMP\floatucnsperiod.obj :  floatucnsperiod.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ floatucnsperiod.cpp
|

TMP\getgroup.obj :  getgroup.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ getgroup.cpp
|

TMP\getitem.obj :  getitem.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ getitem.cpp
|

TMP\getrecomposedialog.obj :  getrecomposedialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ getrecomposedialog.cpp
|

TMP\group.obj :  group.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ group.cpp
|

TMP\groupcombobox.obj :  groupcombobox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ groupcombobox.cpp
|

TMP\groupdialog.obj :  groupdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ groupdialog.cpp
|

TMP\grouplist.obj :  grouplist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ grouplist.cpp
|

TMP\grouplistbox.obj :  grouplistbox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ grouplistbox.cpp
|

TMP\grouplistcolumn.obj :  grouplistcolumn.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ grouplistcolumn.cpp
|

TMP\grouplistdialog.obj :  grouplistdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ grouplistdialog.cpp
|

TMP\grouplistwindow.obj :  grouplistwindow.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ grouplistwindow.cpp
|

TMP\halls.obj :  halls.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ halls.cpp
|

TMP\heritages.obj :  heritages.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ heritages.cpp
|

TMP\import.obj :  import.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ import.cpp
|

TMP\inconside.obj :  inconside.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ inconside.cpp
|

TMP\indocbook.obj :  indocbook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ indocbook.cpp
|

TMP\informeds.obj :  informeds.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ informeds.cpp
|

TMP\inregbook.obj :  inregbook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ inregbook.cpp
|

TMP\inregs.obj :  inregs.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ inregs.cpp
|

TMP\interact.obj :  interact.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ interact.cpp
|

TMP\invalid.obj :  invalid.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ invalid.cpp
|

TMP\invbookanno.obj :  invbookanno.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ invbookanno.cpp
|

TMP\invbookdeliver.obj :  invbookdeliver.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ invbookdeliver.cpp
|

TMP\invbooksubpoena.obj :  invbooksubpoena.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ invbooksubpoena.cpp
|

TMP\joinstreetdialog.obj :  joinstreetdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ joinstreetdialog.cpp
|

TMP\joinstreets.obj :  joinstreets.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ joinstreets.cpp
|

TMP\joinuxndialog.obj :  joinuxndialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ joinuxndialog.cpp
|

TMP\judgerep.obj :  judgerep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ judgerep.cpp
|

TMP\jurors.obj :  jurors.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ jurors.cpp
|

TMP\knownpersonlistbox.obj :  knownpersonlistbox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ knownpersonlistbox.cpp
|

TMP\knowns.obj :  knowns.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ knowns.cpp
|

TMP\lawsuits.obj :  lawsuits.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ lawsuits.cpp
|

TMP\lawsuitsidelistdialog.obj :  lawsuitsidelistdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ lawsuitsidelistdialog.cpp
|

TMP\legalaid.obj :  legalaid.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ legalaid.cpp
|

TMP\letrangerep.obj :  letrangerep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ letrangerep.cpp
|

TMP\loadhall.obj :  loadhall.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ loadhall.cpp
|

TMP\links.obj :  links.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ links.cpp
|

TMP\log.obj :  log.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ log.cpp
|

TMP\login.obj :  login.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ login.cpp
|

TMP\longtextdialog.obj :  longtextdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ longtextdialog.cpp
|

TMP\mcac.obj :  mcac.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ mcac.cpp
|

TMP\memblock.obj :  memblock.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ memblock.cpp
|

TMP\message.obj :  message.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ message.cpp
|

TMP\misc.obj :  misc.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ misc.cpp
|

TMP\modifiers.obj :  modifiers.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ modifiers.cpp
|

TMP\monthperioddialog.obj :  monthperioddialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ monthperioddialog.cpp
|

TMP\msql.obj :  msql.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ msql.cpp
|

TMP\mstr.obj :  mstr.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ mstr.cpp
|

TMP\myrand.obj :  myrand.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ myrand.cpp
|

TMP\niacollect.obj :  niacollect.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ niacollect.cpp
|

TMP\nom.obj :  nom.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ nom.cpp
|

TMP\oldsubjectlistbox.obj :  oldsubjectlistbox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ oldsubjectlistbox.cpp
|

TMP\office.obj :  office.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ office.cpp
|

TMP\offields.obj :  offields.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ offields.cpp
|

TMP\opensave.obj :  opensave.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ opensave.cpp
|

TMP\originsidelistdialog.obj :  originsidelistdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ originsidelistdialog.cpp
|

TMP\originsidelistwindow.obj :  originsidelistwindow.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ originsidelistwindow.cpp
|

TMP\output.obj :  output.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ output.cpp
|

TMP\outregbook.obj :  outregbook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ outregbook.cpp
|

TMP\p1rangerep.obj :  p1rangerep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ p1rangerep.cpp
|

TMP\p2courtrep.obj :  p2courtrep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ p2courtrep.cpp
|

TMP\p24a1.obj :  p24a1.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ p24a1.cpp
|

TMP\password.obj :  password.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ password.cpp
|

TMP\payments.obj :  payments.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ payments.cpp
|

TMP\perioddialog.obj :  perioddialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ perioddialog.cpp
|

TMP\personal2.obj :  personal2.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ personal2.cpp
|

TMP\pointer.obj :  pointer.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ pointer.cpp
|

TMP\posterm.obj :  posterm.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ posterm.cpp
|

TMP\printexe.obj :  printexe.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ printexe.cpp
|

TMP\printarray.obj :  printarray.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ printarray.cpp
|

TMP\printlist.obj :  printlist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ printlist.cpp
|

TMP\printout.obj :  printout.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ printout.cpp
|

TMP\printsub.obj :  printsub.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ printsub.cpp
|

TMP\process.obj :  process.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ process.cpp
|

TMP\provebook.obj :  provebook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ provebook.cpp
|

TMP\proves.obj :  proves.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ proves.cpp
|

TMP\pushbutton.obj :  pushbutton.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ pushbutton.cpp
|

TMP\cacquery.obj :  cacquery.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ cacquery.cpp
|

TMP\querybankhtml.obj :  querybankhtml.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ querybankhtml.cpp
|

TMP\querydoc.obj :  querydoc.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ querydoc.cpp
|

TMP\querypolice.obj :  querypolice.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ querypolice.cpp
|

TMP\querywebhtml.obj :  querywebhtml.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ querywebhtml.cpp
|

TMP\quickdialog.obj :  quickdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ quickdialog.cpp
|

TMP\quickface.obj :  quickface.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ quickface.cpp
|

TMP\quicklist.obj :  quicklist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ quicklist.cpp
|

TMP\quicksearch.obj :  quicksearch.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ quicksearch.cpp
|

TMP\quicksearchedit.obj :  quicksearchedit.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ quicksearchedit.cpp
|

TMP\radioface.obj :  radioface.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ radioface.cpp
|

TMP\randomact.obj :  randomact.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ randomact.cpp
|

TMP\realities.obj :  realities.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ realities.cpp
|

TMP\rangerep.obj :  rangerep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ rangerep.cpp
|

TMP\recusalbook.obj :  recusalbook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ recusalbook.cpp
|

TMP\rekindlaw.obj :  rekindlaw.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ rekindlaw.cpp
|

TMP\renumber.obj :  renumber.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ renumber.cpp
|

TMP\renumberdocdialog.obj :  renumberdocdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ renumberdocdialog.cpp
|

TMP\renumberlawdialog.obj :  renumberlawdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ renumberlawdialog.cpp
|

TMP\renumberside.obj :  renumberside.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ renumberside.cpp
|

TMP\renumberstreetdialog.obj :  renumberstreetdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ renumberstreetdialog.cpp
|

TMP\renumberuxndialog.obj :  renumberuxndialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ renumberuxndialog.cpp
|

TMP\report.obj :  report.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ report.cpp
|

TMP\reportjudge.obj :  reportjudge.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ reportjudge.cpp
|

TMP\requestbook.obj :  requestbook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ requestbook.cpp
|

TMP\requests.obj :  requests.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ requests.cpp
|

TMP\resizedialog.obj :  resizedialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ resizedialog.cpp
|

TMP\resizelistwindow.obj :  resizelistwindow.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ resizelistwindow.cpp
|

TMP\resourcedialog.obj :  resourcedialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ resourcedialog.cpp
|

TMP\rpmnorep.obj :  rpmnorep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ rpmnorep.cpp
|

TMP\scaledialog.obj :  scaledialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ scaledialog.cpp
|

TMP\scheds.obj :  scheds.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ scheds.cpp
|

TMP\selectdialog.obj :  selectdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ selectdialog.cpp
|

TMP\selectors.obj :  selectors.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ selectors.cpp
|

TMP\selectsubjectslistbox.obj :  selectsubjectslistbox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ selectsubjectslistbox.cpp
|

TMP\senderlistbox.obj :  senderlistbox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ senderlistbox.cpp
|

TMP\senders.obj :  senders.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ senders.cpp
|

TMP\sendlist.obj :  sendlist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ sendlist.cpp
|

TMP\sendout.obj :  sendout.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ sendout.cpp
|

TMP\sesdrep.obj :  sesdrep.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ sesdrep.cpp
|

TMP\sessions.obj :  sessions.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ sessions.cpp
|

TMP\sesslist.obj :  sesslist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ sesslist.cpp
|

TMP\sesstimes.obj :  sesstimes.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ sesstimes.cpp
|

TMP\showdialog.obj :  showdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ showdialog.cpp
|

TMP\showhtml.obj :  showhtml.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ showhtml.cpp
|

TMP\sidewindlistdialog.obj :  sidewindlistdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ sidewindlistdialog.cpp
|

TMP\sidewindlistwindow.obj :  sidewindlistwindow.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ sidewindlistwindow.cpp
|

TMP\slcreasons.obj :  slcreasons.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ slcreasons.cpp
|

TMP\slowbook.obj :  slowbook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ slowbook.cpp
|

TMP\snprintf.obj :  snprintf.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ snprintf.c
|

TMP\spawncmd.obj :  spawncmd.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ spawncmd.cpp
|

TMP\statquery.obj :  statquery.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ statquery.cpp
|

TMP\streetmap.obj :  streetmap.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ streetmap.cpp
|

TMP\stringface.obj :  stringface.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ stringface.cpp
|

TMP\subjectlistbox.obj :  subjectlistbox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ subjectlistbox.cpp
|

TMP\subjects.obj :  subjects.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ subjects.cpp
|

TMP\subplist.obj :  subplist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ subplist.cpp
|

TMP\subpoenas.obj :  subpoenas.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ subpoenas.cpp
|

TMP\surrounds.obj :  surrounds.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ surrounds.cpp
|

TMP\table.obj :  table.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ table.cpp
|

TMP\testfunc.obj :  testfunc.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ testfunc.cpp
|

TMP\timebook.obj :  timebook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ timebook.cpp
|

TMP\timedlawsuit.obj :  timedlawsuit.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ timedlawsuit.cpp
|

TMP\timedsession.obj :  timedsession.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ timedsession.cpp
|

TMP\tmp.obj :  tmp.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ tmp.cpp
|

TMP\tntdialog.obj :  tntdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ tntdialog.cpp
|

TMP\tntlistwindow.obj :  tntlistwindow.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ tntlistwindow.cpp
|

TMP\transfer.obj :  transfer.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ transfer.cpp
|

TMP\trouble.obj :  trouble.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ trouble.cpp
|

TMP\troubledialog.obj :  troubledialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ troubledialog.cpp
|

TMP\tryaddresssidelistwindow.obj :  tryaddresssidelistwindow.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ tryaddresssidelistwindow.cpp
|

TMP\twinbutton.obj :  twinbutton.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ twinbutton.cpp
|

TMP\ucns.obj :  ucns.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ ucns.cpp
|

TMP\unappealed.obj :  unappealed.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ unappealed.cpp
|

TMP\unwritten.obj :  unwritten.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ unwritten.cpp
|

TMP\unwrittendialog.obj :  unwrittendialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ unwrittendialog.cpp
|

TMP\update.obj :  update.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ update.cpp
|

TMP\userfile.obj :  userfile.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ userfile.cpp
|

TMP\userlistbox.obj :  userlistbox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ userlistbox.cpp
|

TMP\users.obj :  users.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ users.cpp
|

TMP\utility.obj :  utility.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ utility.cpp
|

TMP\utility1.obj :  utility1.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ utility1.cpp
|

TMP\valid.obj :  valid.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ valid.cpp
|

TMP\vmessage.obj :  vmessage.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ vmessage.cpp
|

TMP\scanner.obj :  scanner.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ scanner.c
|

TMP\sscanf.obj :  sscanf.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ sscanf.c
|

TMP\vprinter.obj :  vprinter.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ vprinter.c
|

TMP\waitthread.obj :  waitthread.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ waitthread.cpp
|

TMP\waitwindow.obj :  waitwindow.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ waitwindow.cpp
|

TMP\whatevertextbasedialog.obj :  whatevertextbasedialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ whatevertextbasedialog.cpp
|

TMP\whatevertextviewdialog.obj :  whatevertextviewdialog.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ whatevertextviewdialog.cpp
|

TMP\xfer.obj :  xfer.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ xfer.cpp
|

TMP\winutils.obj :  winutils.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ winutils.cpp
|

TMP\zzdn.obj :  zzdn.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ zzdn.cpp
|

TMP\_abcbook.obj :  _abcbook.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _abcbook.cpp
|

TMP\_action.obj :  _action.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _action.cpp
|

TMP\_aplclmn.obj :  _aplclmn.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _aplclmn.cpp
|

TMP\_bankrup.obj :  _bankrup.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _bankrup.cpp
|

TMP\_checks.obj :  _checks.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _checks.cpp
|

TMP\_clmnarr.obj :  _clmnarr.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _clmnarr.cpp
|

TMP\_clmutil.obj :  _clmutil.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _clmutil.cpp
|

TMP\_clsesbk.obj :  _clsesbk.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _clsesbk.cpp
|

TMP\_column.obj :  _column.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _column.cpp
|

TMP\_compjcc.obj :  _compjcc.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _compjcc.cpp
|

TMP\_compose.obj :  _compose.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _compose.cpp
|

TMP\_comprp1.obj :  _comprp1.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _comprp1.cpp
|

TMP\_comprp2.obj :  _comprp2.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _comprp2.cpp
|

TMP\_const.obj :  _const.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _const.cpp
|

TMP\_convert.obj :  _convert.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _convert.cpp
|

TMP\_data.obj :  _data.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _data.cpp
|

TMP\_defjdgm.obj :  _defjdgm.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _defjdgm.cpp
|

TMP\_desjudg.obj :  _desjudg.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _desjudg.cpp
|

TMP\_deskbk.obj :  _deskbk.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _deskbk.cpp
|

TMP\_doc.obj :  _doc.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _doc.cpp
|

TMP\_extract.obj :  _extract.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _extract.cpp
|

TMP\_faces.obj :  _faces.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _faces.cpp
|

TMP\_flags.obj :  _flags.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _flags.cpp
|

TMP\_grammar.obj :  _grammar.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _grammar.cpp
|

TMP\_html.obj :  _html.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _html.cpp
|

TMP\_ilieva.obj :  _ilieva.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _ilieva.cpp
|

TMP\_inregbk.obj :  _inregbk.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _inregbk.cpp
|

TMP\_judgef.obj :  _judgef.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _judgef.cpp
|

TMP\_judgesc.obj :  _judgesc.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _judgesc.cpp
|

TMP\_krachol.obj :  _krachol.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _krachol.cpp
|

TMP\_lawchk.obj :  _lawchk.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _lawchk.cpp
|

TMP\_linkwrt.obj :  _linkwrt.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _linkwrt.cpp
|

TMP\_lnkwind.obj :  _lnkwind.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _lnkwind.cpp
|

TMP\_maillst.obj :  _maillst.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _maillst.cpp
|

TMP\_message.obj :  _message.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _message.cpp
|

TMP\_misc.obj :  _misc.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _misc.cpp
|

TMP\_mixed.obj :  _mixed.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _mixed.cpp
|

TMP\_nom.obj :  _nom.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _nom.cpp
|

TMP\_notify.obj :  _notify.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _notify.cpp
|

TMP\_outfile.obj :  _outfile.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _outfile.cpp
|

TMP\_outputs.obj :  _outputs.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _outputs.cpp
|

TMP\_outrgbk.obj :  _outrgbk.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _outrgbk.cpp
|

TMP\_pdcitjc.obj :  _pdcitjc.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _pdcitjc.cpp
|

TMP\_penalty.obj :  _penalty.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _penalty.cpp
|

TMP\_pljchk.obj :  _pljchk.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _pljchk.cpp
|

TMP\_pr634tl.obj :  _pr634tl.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _pr634tl.cpp
|

TMP\_prcsbd.obj :  _prcsbd.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _prcsbd.cpp
|

TMP\_proutpu.obj :  _proutpu.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _proutpu.cpp
|

TMP\_prrpbd.obj :  _prrpbd.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _prrpbd.cpp
|

TMP\_prtbkd.obj :  _prtbkd.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _prtbkd.cpp
|

TMP\_reqdia.obj :  _reqdia.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _reqdia.cpp
|

TMP\_rolls.obj :  _rolls.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _rolls.cpp
|

TMP\_rpdata.obj :  _rpdata.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _rpdata.cpp
|

TMP\_rsetbk.obj :  _rsetbk.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _rsetbk.cpp
|

TMP\_section.obj :  _section.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _section.cpp
|

TMP\_sedistr.obj :  _sedistr.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _sedistr.cpp
|

TMP\_sesschk.obj :  _sesschk.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _sesschk.cpp
|

TMP\_sets.obj :  _sets.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _sets.cpp
|

TMP\_sqlcrit.obj :  _sqlcrit.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _sqlcrit.cpp
|

TMP\_subpoen.obj :  _subpoen.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _subpoen.cpp
|

TMP\_sumtd.obj :  _sumtd.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _sumtd.cpp
|

TMP\_tbclmns.obj :  _tbclmns.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _tbclmns.cpp
|

TMP\_tbsthrd.obj :  _tbsthrd.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _tbsthrd.cpp
|

TMP\_timebk.obj :  _timebk.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _timebk.cpp
|

TMP\_tl634bk.obj :  _tl634bk.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _tl634bk.cpp
|

TMP\_toname.obj :  _toname.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _toname.cpp
|

TMP\_toostrm.obj :  _toostrm.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _toostrm.cpp
|

TMP\_tostrng.obj :  _tostrng.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _tostrng.cpp
|

TMP\_tscntou.obj :  _tscntou.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _tscntou.cpp
|

TMP\_utility.obj :  _utility.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _utility.cpp
|

TMP\_zdravk.obj :  _zdravk.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _zdravk.cpp
|

TMP\_zzdncol.obj :  _zzdncol.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe) -o$@ _zzdncol.cpp
|

TMP\appnameapp.res :  appnameapp.rc
  $(BRC32) -R @&&|
 $(IDE_ResFLAGS32) $(ROptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe)  -FO$@ appnameapp.rc
|
TMP\appnameapp1.res :  appnameapp1.rc
  $(BRC32) -R @&&|
 $(IDE_ResFLAGS32) $(ROptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe)  -FO$@ appnameapp1.rc
|
TMP\_appnameapp.res :  _appnameapp.rc
  $(BRC32) -R @&&|
 $(IDE_ResFLAGS32) $(ROptsAt_dbappnamedexe) $(CompInheritOptsAt_dbappnamedexe)  -FO$@ _appnameapp.rc
|
# Compiler configuration file
BccW32.cfg : 
   Copy &&|
-w
-R
-v
-vi
-H
-H=appname.csm
-K
-d
-R-
-6
-O2
-y
-W
-H"owl\pch.h"
| $@


