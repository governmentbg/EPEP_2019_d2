# include "_prZZDNB.h"		// 20051010
# include "_prZZDNB.cpp"	// 2011:189 LPR: made local

class TZZDNBookThread : public TWaitThread
{
	public:
		TZZDNBookThread(ZZDN_Dlg_data & dlg_ZZDN_, const char* formatFN,
			  char* const crit,
			  bookZZDNOutFile& outf);

	protected:
    ZZDN_Dlg_data & dlg_ZZDN;   	//begNEndNYear& bneny;
		const char* const formatFName;
    int serialNo;
		char* const criteria;
		bookZZDNOutFile& of;

		bool ArgsOK();
		virtual void Execute();
		void ChangeCurrentItemText(const TRCDKey key, const int current, const int total);
    void WriteHeader();
    void WriteOneLawsuit(TRequest& bRequest, const int serialNo);
};  // class ZZDNBookThread

TZZDNBookThread::TZZDNBookThread(ZZDN_Dlg_data & dlg_ZZDN_, const char* formatFN,
					 char* const crit, bookZZDNOutFile& outf) :
					dlg_ZZDN(dlg_ZZDN_),
					formatFName(formatFN),
					criteria(crit),
					of(outf)
{
	if(ArgsOK() == false)
		error("%s\n%s::%s", WRONG_ARGS_PASSED_TO,
					typeid(*this).name(), typeid(*this).name());
  serialNo = dlg_ZZDN_.begN ;
}

bool TZZDNBookThread::ArgsOK()
{
	return (formatFName && formatFName[0] && criteria && criteria[0] && of);
}

void TZZDNBookThread::ChangeCurrentItemText(const TRCDKey key,
	const int current, const int total)
{
	char buff[128];
	ostrstream oss(buff, sizeof(buff));
	TRCDKey_ k(key);

	if(k.FilledOK())
		oss << k << " ";
	oss << "(" << current << " îò " << total << ")";
	oss << ends;
	buff[sizeof(buff) - 1] = '\0';
	wtdl->SetText(buff);
}

void TZZDNBookThread::WriteOneLawsuit(TRequest& bRequest, const int serialNo)
{
  TStringArray* pItems = NULL;
  size_t minRows = 0;

  BookZZDNColumns dBC(formatFName, wtdl->Parent, bRequest, minRows, serialNo, pItems) ;
  const int rows = dBC.getMaxRows();

    of.nextLSToPrint(rows + 1);
    of.horzLine();
    for(int i = 0; i < rows; i ++)
	of.writeItem(dBC, true);
}

void TZZDNBookThread::Execute()
{
	if (ArgsOK())
	{
		try
		{
			wtdl->SetHeader("Ðåãèñòúð ïî ÇÇÄÍ");
			wtdl->PleaseWait();

	TRequest bRequest ;

			TCountRecsQuery q (criteria) ;

		const int total = q.getRecsCount();

			wtdl->SetProgressRange(0, total);

	// Adjust page lines

	int lawsCnt = 0;

	if(total > 0)
	{
	  WriteHeader();
	}

	while (q.Read ())
			{
				if(wtdl->WantBreak())
					break;
				lawsCnt ++ ;
				bRequest << q ;
				ChangeCurrentItemText(bRequest.key, lawsCnt, total);
	  WriteOneLawsuit(bRequest, serialNo);
	  serialNo ++ ;
				wtdl->SetProgress(lawsCnt);
			}

	wtdl->AlmostDone();
			  of.footherAndTail () ;
		}
		CATCH_ANYTHING
	}
}

void TZZDNBookThread::WriteHeader()
{
    of.title() ;
    of.formattedHead(&dlg_ZZDN);
    of.head() ;
}

bool book_ZZDN :: init_cCBDEDBN_ZZDN ()
 {
  return (TPrintBookZZDNDialog (   pParent,
					   & cCBDEDBN_ZZDN,
					     IDD_PRINT_BOOK_ZZDN
					  ).Execute () == IDOK) ;
 }  // book_ZZDN :: init_cCBDEDBN_ZZDN

bool book_ZZDN :: composeCriteria ()
 {
  return composeZZDNCriteria (criteria, critSz,
			     " R.F_NO, R.F_YEAR, R.F_TYPE, R.F_KIND, R.F_COLLEGE, R.F_DATE ",
			     // " , L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND ",
			     NULL, & cCBDEDBN_ZZDN) ;
 }  // book_ZZDN :: composeCriteria

void book_ZZDN :: makeQuery ()
 {
    TWaitWindow ww (pParent, PLEASE_WAIT) ;

    pQ = new TCountRecsQuery (criteria) ;
 }  // book_ZZDN :: makeQuery

book_ZZDN ::  book_ZZDN (TWindow * parent) :
		  BookZZDNEnum (),
		  pParent (parent), pQ (NULL), selectSessionsOK (false),
		  criteria (NULL), critSz (512 + 256 + 128 + 64)
{
  if (init_cCBDEDBN_ZZDN () &&
	((criteria = new char [critSz]) != NULL)
     )
  {
    if (composeCriteria ())
     {
	makeQuery () ;
	if (pQ)
	  selectSessionsOK = true ;
     }
  }
}  // book_ZZDN :: book_ZZDN

void book_ZZDN :: destroyQueries ()
 {
  if (pQ)
   {
    delete pQ ;
    pQ = NULL ;
   }
  selectSessionsOK = false ;
 }  // book_ZZDN :: destroyQueries

book_ZZDN :: ~book_ZZDN ()
 {
  destroyQueries () ;
  if (criteria)
    delete [] criteria ;
 }  // book_ZZDN :: ~book_ZZDN

char * book_ZZDN :: makeBookFormatFN ()
  // returns a pointer to a newly created string or NULL
 {
  char tmp[MAXPATH];
  char* formatFN = NULL;

  strCopy(tmp, "text\\ZZDN_", sizeof(tmp));

  strConcat(tmp, DEFAULT_FORMAT_FILE_EXTENSION, sizeof(tmp));
  {
    int sz = strlen(tmp) + 1;

    if(sz < sizeof(tmp))
    {
	formatFN = new char[sz];
	strCopy(formatFN, tmp, sz);
    }
  }

  return formatFN ;
 }  // book_ZZDN :: makeBookFormatFN

void book_ZZDN::printBook_ZZDN()
{
	char *formatFN = makeBookFormatFN();

	register_usage(REGIME_BOOK_ZZDN, 1, 1, "Êíèãà ÇÇÄÍ");

	if (!formatFN)
		return;

	if (selectSessionsOK)
	{  // -- 0 --
		pQ->Restart();
		bookZZDNOutFile outF(pParent, formatFN);

		try
		{
			TWaitThreadDialog wtdl;
			TZZDNBookThread(cCBDEDBN_ZZDN, formatFN, criteria, outF).ExecWait();
		}
		CATCH_ANYTHING

		show_text_stream_use_settings(NULL, outF, formatFN, FLOAT_ALIGNMENT);
	}

	destroyQueries();
	delete [] formatFN;
}  // book_ZZDN :: printBookZZDN
