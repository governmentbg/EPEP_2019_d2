// ----- COMMON HEADERS ------------------------------------------------------
#include "database.h"

// ----- SYSTEM HEADERS ------------------------------------------------------
#include <fcntl.h>
#include <io.h>
#include <math.h>
#include <process.h>
#include <time.h>
#include <sys/stat.h>
#include <utime.h>

// ----- WINDOWS HEADERS -----------------------------------------------------
#include <shlobj.h>
#include <lmcons.h>
#include <lmerr.h>

// ----- OWL HEADERS ---------------------------------------------------------
#include <owl/defs.h>
#define TUser TNTUser
#include <owl/applicat.h>
#undef TUser
#include <owl/button.h>
#include <owl/checkbox.h>
#include <owl/combobox.h>
#include <owl/decframe.h>
#include <owl/dialog.h>
#include <owl/edit.h>
#include <owl/gauge.h>
#include <owl/groupbox.h>
#include <owl/listbox.h>
#include <owl/listwind.h>
#include <owl/radiobut.h>
#include <owl/static.h>
#include <owl/validate.h>
#include <owl/tabctrl.h>

// ----- RTL -----------------------------------------------------------------
extern "C" int errno;	// verify / declare

// ----- DEFINITIONS ---------------------------------------------------------
#include "defines.h"

#if SPEEDTESTING && (TESTVER || TYPECHECK || RANGECHECK)
#error SPEEDTESTING does not allow TESTVER, TYPECHECK and RANGECHECK
#endif

#if TESTVER
#define cac_assert(expr) ((expr) ? (void) 0 : fatal("Assert: %s, %s:%d", #expr, __FILE__, __LINE__))
#else
#define cac_assert(expr) ((void) 0)
#endif  // TESTVER

#if TYPECHECK
#define TYPECHECK_PTR_CAST(type, object) (dynamic_cast<type *> (object))
#else  // TYPECHECK
#define TYPECHECK_PTR_CAST(type, object) (static_cast<type *> (object))
#endif  // TYPECHECK

#define GETLASTERROR ((unsigned long) GetLastError())
// Common Controls 4.70+
#define LVS_EX_FULLROWSELECT 32
#define LVS_EX_ONECLICKACTIVATE 64
#define LVM_GETHEADER (LVM_FIRST+31)
#define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST+54)
#define LVM_GETEXTENDEDLISTVIEWSTYLE (LVM_FIRST+55)
#define LVCFMT_NO_SEARCH 2048	// LVCFMT_IMAGE

#define NM_CUSTOMDRAW (NM_FIRST-12)
typedef struct
{
	NMHDR hdr;
	DWORD dwDrawStage;
	HDC hdc;
	RECT rc;
	DWORD dwItemSpec;
	UINT uItemState;
	LPARAM lItemlParam;
} NMCUSTOMDRAW, *LPNMCUSTOMDRAW;
#define CDDS_PREPAINT	1
#define CDDS_POSTPAINT	2
#define CDDS_PREERASE	3
#define CDDS_POSTERASE	4

#define CDRF_NOTIFYPOSTPAINT	0x10
#define CDRF_NOTIFYPOSTERASE	0x40
// Theme Engine XP+
#define STAP_ALLOW_NONCLIENT	0x00000001
#define STAP_ALLOW_CONTROLS	0x00000002
#define STAP_ALLOW_WEBCONTENT	0x00000004
#define ETDT_DISABLE		0x00000001
#define ETDT_ENABLE		0x00000002
#define ETDT_USETABTEXTURE	0x00000004

#define CBS_TYPE (CBS_SIMPLE | CBS_DROPDOWN | CBS_DROPDOWNLIST)
#define LBS_SELTYPE (LBS_EXTENDEDSEL | LBS_MULTIPLESEL | LBS_NOSEL)

#ifdef L_tmpnam
#undef L_tmpnam
#endif  // L_tmpnam
#define L_tmpnam SIZE_OF_PATH		// for _		

#define DEFINE_GROUP_TYPE(type, name) type *name() const { return TYPECHECK_PTR_CAST(type, TGroupDialog::Group()); }
#define DEFINE_SIDES_TYPE(type, name) type *name() const { return TYPECHECK_PTR_CAST(type, TSideWindListDialog::container); }
#define DEFINE_GLIST_TYPE(type, name) type *name() const { return TYPECHECK_PTR_CAST(type, TGroupListDialog::glist); }

// ----- APPLICATION HEADERS -------------------------------------------------
#include "spawncmd.h"
#include "appnameapp.rh"
#include "appnamesys.rh"
#include "appnameapp1.rh"
#include "mcac.h"
#include "myrand.h"			// 2012:221 separate module
#include "datetime.h"			// 2011:097 separate module
#include "misc.h"
#include "TNTListWindow.h"
#define TListWindow TNTListWindow
#include "ResizeListWindow.h"
#include "TNTDialog.h"			// 2009:128 LPR: recovery after fatal()
#define CloseDialog CloseWindow
#include "table.h"
#include "transfer.h"			// Level2level transfer mechanisms
#include "msql.h"				// 2011:081
#include "testfunc.h"
#include "const.h"			// Classified constants
#include "userfile.h"			// fopen/fclose w/ error messages
#include "posterm.h"			// 2013:135
#include "appnameapp.h"			// Variables set inside main()
#include "config.h"			// Configuration variables
#include "PushButton.h"			// 2008:016 [secondId] [iconId/auto]
#include "TwinButton.h"			// 2009:016 moved <secondId> here
#include "group.h"			// Base groups
#include "FloatGroup.h"			// 2011:081
#include "alias.h"			// 2010:173
#include "nom.h"				// Nom groups
#include "container.h"			// Containers
class TLinkWind;
class TSubpoena;
#include "doc.h"				// Doc groups
#include "data.h"				// initialize/shutdown database
#include "counter.h"			// 2013:138 separate module
#include "winutils.h"			// 2017:199 separate module
#include "process.h"			// 2008:184 distrib/output base
#include "opensave.h"			// 2014:024
#include "output.h"			// 2009:070 report/book base
//#include "distrib.h"			// 2016:026 LPR/LRQ: block distrib
#include "checkout.h"			// 2007:152 check for similars
#include "personal2.h"			// 2008:149 new filter personal
#include "filter.h"			// 2008:093 OLE personal filter
#include "import.h"			// 2008:040 reregister; 2008:114 known
#include "export.h"			// 2011:284 separate module, bankrupt
#include "utility.h"			// selectors, convertors, action...
#include "log.h"				// 2014:273 separate module
#include "addresses.h"			// 2008:015 select/edit addresses
#include "ucns.h"				// 2008:017 select/edit ucn groups
#include "valid.h"			// Valid functions
#include "unwritten.h"			// unfinished law, unwritten act
void float_log(TWindow *parent);
void float_movement(TWindow *parent);
void float_electric(TWindow *parent);
#include "floatsession.h"
void timed_session(TWindow *parent);
void timed_lawsuit(TWindow *parent);
#include "floatsched.h"
struct begMYEndMYTypeCIU;
#include "floatoutreg.h"
void float_unappealed(TWindow *parent);	// 2018-05-09
void float_expense(TWindow *parent, char sumType);
void float_surround(TWindow *parent);
#include "floatimportant.h"
void float_request(TWindow *parent);	// 2006:009
#include "floatarchive.h"			// 2006:010
#include "floatconnect.h"			// 2007:018
void float_subpoena(TWindow *parent);	// 2008:007
#include "floatannounce.h"
void period_side(TWindow *parent);		// 2013:086
void float_juror(TWindow *parent);		// 2014:258
// 2008:028 LPR: querydoc -> all, floatmov
void query_movement(TWindow *parent, TRCDKeyGroup *group);
typedef const char *(*match_func)(TRCDKeyGroup *group, char status);
#include "action.h"			// Function redirector/helper
#include "requests.h"			// 2015:055 separate module
#include "lawsuits.h"			// 2016:238 separate module
#include "delete.h"			// 2009:202 from action
#include "tmp.h"				// Work with temporary text files
#include "showhtml.h"			// 2007:103 Work with html files
#include "querydoc.h"			// Request and lawsuit queries
#include "querypolice.h"
#include "renumber.h"			// Re# UCN Groups
void renumber_sideWind(TWindow *parent);
void rekind_lawsuit(TWindow *parent);
#include "printarray.h"			// various text outputs
#include "printout.h"
#include "update.h"			// 2007:020 updates put here
#include "halls.h"			// 2009:205 edit halls, load sessions
void map_user(TWindow *parent, TUser *user);
void edit_users(TWindow *parent);
void edit_subjects(TWindow *parent);
void edit_compositions(TWindow *parent);
void edit_senders(TWindow *parent);
#include "accounts.h"
#include "bankrupts.h"
#include "office.h"			// 2008:063 replace office fields
#include "offields.h"			// 2008:063 replacement functions
#include "loadhall.h"			// Court halls workload
#include "sessions.h"			// 2009:202 open/closed (re)set/result
void extern_sessions(TWindow *parent);
#include "printexe.h"			// 2008:074 print executive order
#include "report.h"			// 2008:200 lead/trail only
#include "reportjudge.h"		// 2008:200; 2015:162 helper only
void lawsuit_judge_report(TWindow *parent, const TReportGroup &group);	// 2008:183
void appeal_judge_report(TWindow *parent, const TReportGroup &group);	// 2008:200
void session_dist_report(TWindow *parent, const TReportGroup &group);	// 2018-07-03
#include "rangerep.h"			// 2016:189 +related
#include "letrangerep.h"		// 2016:194+
#include "c1rangerep.h"			// 2017:027+
#include "p1rangerep.h"			// 2017:031; 2017:041 rprangerep -> p1rangerep
#include "courtrep.h"			// 2017:045
#include "c2courtrep.h"			// 2017:045
#include "p2courtrep.h"			// 2017:046
#include "rpmnorep.h"			// 2017:060
#include "bsrangerep.h"			// 2018-02-01
#include "bscourtrep.h"			// 2018-02-02
#include "book.h"				// 2008:246 header/criteria only
#include "outregbook.h"
#include "dangerep.h"			// 2008:267
void print_subpoena_list(TWindow *parent, TRCDKeyContainer *container, TGArray<TSideWind> *sideWinds);
#include "FloatQuery.h"
#include "configure.h"			// 2010:110
#include "_lnkwind.h"
#include "subpoenas.h"			// 2010:155
void edit_informeds(TWindow *parent);	// 2011:161
void random_act(TWindow *parent);	// 2013:175
#include "legalaid.h"
#include "jurors.h"			// distrib/query jurors
#include "surrounds.h"			// 2014:303 separate module
#include "externlaws.h"			// 2017:152 separate module

#include "CheckFace.h"			// 2007:214 cut down to name message
#include "QuickFace.h"			// 2007:214 based on check face
#include "RadioFace.h"

#include "StringFace.h"
inline bool any(TStringFace *text) { return any(text->S()); }
#include "LongFace.h"
#include "DateFace.h"
#include "DateFaceUpDown.h"
#include "TimeFace.h"
#include "MoneyFace.h"
#include "BulStatFace.h"
#include "BankFace.h"
#include "faces.h"

#include "IndexFace.h"
#include "YearFace.h"
#include "BarCodeFace.h"

#include "QuickSearchEdit.h"
#include "QuickSearch.h"
#include "QuickList.h"			// 2012:074 separate interface element
#include "GroupList.h"			// ListBox/ListWindow/ComboBox data model
#include "GroupListColumn.h"
#include "ColumnListWindow.h"
#include "GroupListWindow.h"		// multicol GroupList + Add/EditDelete
#include "DoubleListHeader.h"		// 2013:147 separate module
#include "DoubleListWindow.h"
#include "ConnectListWindow.h"	// 2010:088
#include "SideWindListWindow.h"	// 2010:074
#include "OriginSideListWindow.h"	// 2017:202
#include "TryAddressSideListWindow.h"	// 2014:247 separate module; 2015:077 LegalSide -> TryAddresSide

#include "GroupListBox.h"		// 2010:077
#include "CloneListBox.h"		// 2010:077
#include "StringListBox.h"		// 2011:083
void expend_sidewind(TWindow *parent, TRCDKeyContainer *container, TSession *session, TLawsuit *lawsuit, TSideWind *sideWind);
#include "SenderListBox.h"		// required by queries
#include "SubjectListBox.h"
#include "OldSubjectListBox.h"	// 2016:060 for floats
#include "AliasListBox.h"
#include "UserListBox.h"
#include "StreetListBox.h"
#include "KnownPersonListBox.h"
#include "FindAliasListBox.h"
#include "EisCrimeListBox.h"
#include "EisDepartListBox.h"
#include "ArchiveIndexListBox.h"
#include "SLCReasonListBox.h"

#include "GroupComboBox.h"
#include "AliasFace.h"
#include "StringAliasFace.h"
inline bool atob(TStringAliasFace *face) { return atob(face->S()); }
inline bool ntob(TStringAliasFace *face) { return face && atob(face); }
#include "PersonAliasFace.h"		// SKIP_LOWER, sort, include ucn, etz
#include "PersonDistribAliasFace.h"	// lawKind, persons array
#include "KnownAliasFace.h"		// typing only
#include "JurorAliasFace.h"		// ucn0, ucn1, ucn2
#include "UserAliasFace.h"		// USER_INTER -> include INTER
#include "LongAliasFace.h"
#include "CharAliasFace.h"
#include "CharArrayListFace.h"	// base for char lists
#include "CharListFace.h"
#include "CharListNameFace.h"		// 2006:206 CharList sorted by name
#include "CharAutoListFace.h"
#include "UCNFace.h"
#include "LNCFace.h"			// 2012:215
#include "ConnectNoFace.h"
#include "NIATDepInNoFace.h"		// 2015:166
#include "TypeListFace.h"		// 2009:337 LPR: none + !write -> all
#include "ArticleAliasFace.h"		// 2007:284
#include "DangerAliasFace.h"		// 2008:262
#include "FindLongAliasFace.h"	// 2010:264
#include "SubjectAliasFace.h"		// always find
#include "ReceiverAliasFace.h"	// 2011:139
#include "ConnectKindAliasFace.h"	// 2011:356
#include "XferKindAliasFace.h"	// 2012:289
#include "CompositionAliasFace.h"	// 2012:009
#include "SenderAliasFace.h"		// 2014:247
#include "CurrencyAliasFace.h"	// 2015:027
#include "NIATDepAliasFace.h"		// 2015:040
#include "SLCReasonAliasFace.h"	// 2015:063
#include "VSSIndexAliasFace.h"	// 2015:159
// 2010:271
#include "AreaAliasFace.h"
#include "RegionAliasFace.h"
#include "UCLPAliasFace.h"
#include "FindAddrAliasFace.h"
#include "FindUCLPAliasFace.h"
#include "FindStreetAliasFace.h"
#include "GetContainerAliasFace.h"	// 2013:213 public

// ----- EXTRAS --------------------------------------------------------------
#include "electric.h"
#include "electra.h"			// 2016:008 separated
#include "trouble.h"			// 2015:294 separated
#include "command.h"
#include "invalid.h"
#include "bloat.h"
#include "appealinst.h"			// 2016:083 separate module
#include "sesstimes.h"			// 2012:249 times/price moved here

// ----- DIALOGS -------------------------------------------------------------
#include "ResourceDialog.h"		// 2015:268
#include "ResizeDialog.h"		// 2009:119 generic purpose resize
#include "ShowDialog.h"			// 2015:266
#include "ScaleDialog.h"		// 2007:141 auto zoom/scale content
#include "QuickDialog.h"		// 2011:257 scale + quick search
#include "WaitWindow.h"			// 2007:155 TWaitWindow, rewritten
#include "GroupDialog.h"
#include "EditDialog.h"			// 2011:166
#include "TroubleDialog.h"		// 2009:197
#include "GroupListDialog.h"		// convinience dialog for Add/EditDelete
#include "PeriodDialog.h"
#include "MonthPeriodDialog.h"	// 2017:219
inline bool get_period(TWindow *parent, TPeriodGroup *period) { return TPeriodDialog(parent, period).Execute() == IDOK; }
#include "WhateverTextBaseDialog.h"	// 2015:292 split into base without
#include "WhateverTextViewDialog.h"	// 2015:292 and derived with electric
#include "EditAddressDialog.h"	// 2010:272 Re-written
#include "SelectDialog.h"		// 2010:160 Select one element from list
#include "LongTextDialog.h"		// 2011:137
#include "FloatDialog.h"		// 2011:133
#include "FloatSubjectDialog.h"	// 2016:061
struct IWebBrowser2;
#include "HtmlBaseDialog.h"		// 2014:147 for html hall load
#include "EditIncomingDialog.h"	// 2015:065

// ----- MENU RELATED DIALOGS ------------------------------------------------
#include "GetRecomposeDialog.h"
#include "AlterUXNDialog.h"		// 2012:303 LPR: base for Join/Renumber
#include "JoinUXNDialog.h"
#include "RenumberUXNDialog.h"
#include "AlterStreetDialog.h"	// 2010:273 LPR: base for Join/Renumber
#include "JoinStreetDialog.h"
#include "RenumberStreetDialog.h"
#include "EditConnectDialog.h"	// 2007:027 LPR: was SelectConnect
#include "EditSideWindDialog.h"	// 2008:169 LPR: was SelectInvolvement
#include "SideWindListDialog.h"	// 2010:074 Add/Edit/Delele/Links/[Messages]
#include "OriginSideListDialog.h"
#include "LawsuitSideListDialog.h"
#include "RenumberDocDialog.h"
#include "RenumberLawDialog.h"	// 2008:009 renumber doc based
#include "UnwrittenDialog.h"		// 2009:078

#include "login.h"
void edit_countries(TWindow *parent);
void edit_currences(TWindow *parent);
#include "p24a1.h"
#include "cadrs.h"
void edit_articles(TWindow *parent);
void edit_dangers(TWindow *parent);
void edit_penalities(TWindow *parent);
void edit_reasons(TWindow *parent);
void edit_slc_reasons(TWindow *parent);
#include "getitem.h"
#include "getgroup.h"
#include "modifiers.h"
#include "selectors.h"
#include "interact.h"			// 2009:225
#include "attach.h"			// 2015:267
void edit_knowns(TWindow *parent);
#include "announces.h"			// 2009:352
#include "scheds.h"
#include "proves.h"
#include "sendlist.h"			// inRegEvent list functions
#include "sendout.h"			// 2009:173 +receive, module
#include "xfer.h"				// 2009:175
#include "eportal.h"			// 2016:222
#include "inregs.h"			// 2013:010 grouped
#include "payments.h"
#include "archives.h"			// 2014:055 indexes + regroup

#include "eisxml.h"			// 2012:283 separate file
#include "eispp.h"			// 2012:263
#include "eisfaces.h"			// 2013:008 separate file
#include "eispplaws.h"
#include "execlist.h"			// 2014:247
#include "niacollect.h"			// 2015:040
#include "SelectSubjectsListBox.h"	// 2016:039 public for migration
#include "euher.h"			// 2017:213
#include "corrupts.h"			// 2017:215

#include "statquery.h"
#include "CopySideWindsDialog.h"
#include "BookDialog.h"

#include "AboutDialog.h"

// ----- BOOKS AND REPORTS ---------------------------------------------------
#include "WaitThread.h"			// 2007:258 for _
#include "_all.h"
void query_web(TWindow *parent);	// 2008:004 admin web/police queries
#include "printsub.h"			// 2008:064 print subpoenas/messages
#include "inconside.h"			// 2010:333 inconsistence by sideWind
#include "printlist.h"
void recusal_book(TWindow *parent);
void process_links(TWindow *parent, TLinkWindArray *linkWinds);	// 2014:268
#include "book39.h"
void query_bank_html(TWindow *parent, bool allowWeb);	// 2017:282
#include "all1.h"
#include "floatcombo.h"			// 2017:212 LPR: separated from all1
#include "floatucnsperiod.h"
#include "heritages.h"
void zzdn_book(TWindow *parent);
