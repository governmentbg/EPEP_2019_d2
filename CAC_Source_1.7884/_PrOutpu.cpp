#include "cac.h"

static
bool make_desc_book_format_fn(char* const fmtFName, const size_t fFNSz,
  const begNEndNYear& bneny, const char lawType)
{  // 10.feb.2005
  FUNC_NAME("make_desc_book_format_fn");
  bool res = false;

  if(fmtFName != NULL && fFNSz > 1 && lawType != '\0')
  {
    const char* suffix = "co";
    // We have to support the following formats:
    // - the old book format as text file
    // - the new book format as text file
    if(bneny.jan2005Format)
    {  // the new books format
	// New formats are available only in text   //20050314
	suffix = "_t" ;
    }
    ostrstream(fmtFName, fFNSz)
	<< "text\\invb" << lawType << suffix << bneny.wantXInstLawsToChar()
      << DEFAULT_FORMAT_FILE_EXTENSION << ends;
    fmtFName[fFNSz - 1] = '\0';
    if(strlen(fmtFName) + 1 < fFNSz)
    {
      res = true;
    }
    if(res == false)
	REPORT_F_UNSUCCESSFUL;
  }
  else
    REPORT_F_INVALID_ARGS;
  return res;
}  // make_desc_book_format_fn

const char * makeDeskBSummaryFmtFN (const char * const bkFmtN)
  // returns a pointer to a newly cteated string or NULL
 {
  size_t l ;
  const int bInd = 8 ;

  if (bkFmtN && (l = strlen (bkFmtN)) > bInd && bkFmtN [bInd] == 'b')
   {
    char * fmtN = new char [l + 1] ;

    strcpy (fmtN, bkFmtN) ;
    fmtN [bInd] = 's' ;
    return fmtN ;
   }
  return NULL ;
 }  // makeDeskBSummaryFmtFN

#include "_PrDBkD_.h"
#include "_PrDBkD.cpp"

#include "_PrDBkA_.h"
#include "_PrDBkAD.cpp"

void print_desc_book (TWindow * pParent)
{
  begNEndNYear bneny ;
  char cC ;

# if INSTANCE
#   if APPEAL
  if (TPrintDescBookDialog (pParent, & bneny, & cC).Execute () == IDOK)
#   else  // APPEAL
          // AREA, ADMIN
  if (TPrintDescBookArealDialog (pParent, & bneny,
                                 & cC).Execute () == IDOK)
#   endif  // APPEAL
# else  // INSTANCE
  if (TPrintDescBookDialog (pParent, & bneny, & cC).Execute () == IDOK)
# endif  // INSTANCE
   {
    char fmtFName [128] ;

    // 10.feb.2005 - begin
    //scSprintf (sizeof (fmtFName), fmtFName, "text\\invb%cco%c%s",
    //           cC, bneny.wantXInstLawsToChar (),
    //           DEFAULT_FORMAT_FILE_EXTENSION) ;
    if(make_desc_book_format_fn(fmtFName, sizeof(fmtFName), bneny, cC))
    {  // 10.feb.2005 - end
	const char * summaryFmt = makeDeskBSummaryFmtFN (fmtFName) ;
	subjectHeapArray * pSHA = new subjectHeapArray (summaryFmt, cC) ;
	kindHeapArray * pKHA = new kindHeapArray (summaryFmt, cC) ;
	subjectRangeHeapArray * pSRHA =
	new subjectRangeHeapArray (summaryFmt, cC, pParent) ;
	mergedKindsHeapArray * pMKHA =
	  new mergedKindsHeapArray (summaryFmt, pParent) ;
# if INSTANCE
	orderedDamnTypeHeapArray * pDTHA =
	  new orderedDamnTypeHeapArray (summaryFmt, cC) ;
# endif
	unsigned lawsCnt = 0 ;
	unsigned maxLawNo = 0 ;
	char t[2];  // 27.dec.2004 - begin
	TWaitThreadDialog dialog;

	t[0] = cC;
      t[1] = '\0';
      register_usage(REGIME_INVENTORY_BOOK, 1, t,
	"Описна Книга");  // 27.dec.2004 - end
	printAnyDescBook (pParent, cC, bneny, fmtFName, maxLawNo,
				(* pSHA), (* pKHA), (* pSRHA), (* pMKHA),
# if INSTANCE
                        (* pDTHA),
# endif
                        lawsCnt) ;
      if (summaryFmt)
       {
# if INSTANCE
#   if APPEAL
        print_desk_bk_summary (pParent, cC, bneny, summaryFmt,
                               maxLawNo, lawsCnt) ;
#   else  // of APPEAL
	if (cC != TYPE_PUNISHMENT_LAW &&
            cC != TYPE_CITIZEN_LAW  // 22.oct.2001
           )
        {
          register_usage(REGIME_INVENTORY_SUMMARY, 1, t, "ОК - Рекапит.");
	    printDescBkSummary (pParent, cC, bneny, summaryFmt, maxLawNo,
                              (* pSHA), (* pKHA), (* pSRHA), (* pMKHA),
                              (* pDTHA), lawsCnt) ;
	}
	  else
        {
          register_usage(REGIME_INVENTORY_CODEVAL_SUMMARY, 1, t, "ОК - Рекапит.");
          print_code_value_desc_bk_summary (pParent, cC, bneny, summaryFmt,
                                            maxLawNo, lawsCnt) ;
	}
#   endif  // of APPEAL
# else  // of INSTANCE
        register_usage(REGIME_INVENTORY_SUMMARY, 1, t, "ОК - Рекапит.");
        printDescBkSummary (pParent, cC, bneny, summaryFmt, maxLawNo,
                            (* pSHA), (* pKHA), (* pSRHA), (* pMKHA),
                            lawsCnt) ;
# endif  // of INSTANCE
	delete [] summaryFmt ;
	 }
# if INSTANCE
	delete pDTHA ;
# endif
	delete pMKHA ;
	delete pSRHA ;
	delete pSHA ;
	delete pKHA ;
    }  // 10.feb.2005
   }
 }  // print_desc_book

//#include "_report.cpp"

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static int query_activity_varianth(const begMYEndMYTypeCIU& bMYEMYTCIU, char* varStr, const size_t varStrSz)
{
	int varianth = -1;

	if(varStr != NULL && varStrSz > 0)
	{
		strCopy(varStr, "Отчет ", varStrSz);

	#if INSTANCE
	#  if APPEAL
	#  else  // of APPEAL             // COURT_AREAL, COURT_MILITARY -->
		if(bMYEMYTCIU.firstInstLaws == false)
			strConcat(varStr, "I", varStrSz);
		strConcat(varStr, "I ", varStrSz);
	#  endif  // of APPEAL
	#else  // of INSTANCE
	#endif  // of INSTANCE

		SUPPRESS_UNUSED(varianth);

		if(bMYEMYTCIU.internalUse)
		{
			varianth = 2;
			strConcat(varStr, "ВП", varStrSz);
		}
		else
	#if INSTANCE                      // COURT_APPEAL, COURT_AREAL, COURT_MILITARY  -->
		if(bMYEMYTCIU.forBoss)
		{
			varianth = 3;
			strConcat(varStr, "Предс", varStrSz);
		}
	#if COURT_TYPE == COURT_AREAL
		else if(bMYEMYTCIU.forCorrupt)
		{
			varianth = 4;
			strConcat(varStr, "Коруп", varStrSz);
		}
	#endif
	else
	#else  // of INSTANCE
	#endif  // of INSTANCE              // ALL COURTs
		{
			varianth = 1;
			strConcat(varStr, "МП", varStrSz);
		}
	}
	
	return varianth;
}

void query_activity (TWindow * pParent)
{
	begMYEndMYTypeCIU bMYEMYTCIU ;

	if (TPrintReportBookDialog (pParent, & bMYEMYTCIU).Execute () == IDOK)
	{
		TWaitThreadDialog wtdl;
		TRangeReportGroup reportGroup;

		reportGroup.Import(&bMYEMYTCIU);

	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		if (bMYEMYTCIU.forCorrupt)
		{
			danger_report( pParent, reportGroup );
			return;
		}
	#endif  // AREAL || MILITARY || REGIONAL

	#if COURT_TYPE == COURT_REGIONAL
		// 2017:027; 2017:037 also for variant 2
		{
			char varStr[SIZE_OF_ALIAS];  // 01.dec.2004 - begin
			int varianth = query_activity_varianth(bMYEMYTCIU, varStr, sizeof(varStr));

			reportGroup.withSubjects = (varianth == 2);

			if (varianth == 1 || varianth == 2)
			{
				// 2017:046 anything non-punishment
				if (!strchr(bMYEMYTCIU.types, TYPE_PUNISHMENT_LAW))
				{
					c1rangerep(pParent, reportGroup);
					return;
				}
				// 2017:031
				else
				{
					reportGroup.excludeLow = bMYEMYTCIU.admLowSubjRange * SUBJECT_MULTIPLY;
					reportGroup.excludeHigh = bMYEMYTCIU.admHighSubjRange * SUBJECT_MULTIPLY;
					p1rangerep(pParent, reportGroup);
					return;
				}
			}
		}
	#endif  // REGIONAL

	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		// 2017:040; 2017:117 +MILITARY +related
		if (bMYEMYTCIU.firstInstLaws)
		{
			char varStr[SIZE_OF_ALIAS];  // 01.dec.2004 - begin
			int varianth = query_activity_varianth(bMYEMYTCIU, varStr, sizeof(varStr));

			reportGroup.withSubjects = (varianth == 2);

			if (varianth == 1 || varianth == 2)
			{
				// 2017:046 anything non-punishment
				if (!strchr(bMYEMYTCIU.types, TYPE_PUNISHMENT_LAW))
				{
					c1rangerep(pParent, reportGroup);
					return;
				}
				// 2017:031
				else
				{
					p1rangerep(pParent, reportGroup);
					return;
				}
			}
			else if (varianth == 3)  // 2018-02-01
			{
				bsrangerep(pParent, reportGroup);
				return;
			}
		}
		else	// 2017:045
		{
			char varStr[SIZE_OF_ALIAS];  // 01.dec.2004 - begin
			int varianth = query_activity_varianth(bMYEMYTCIU, varStr, sizeof(varStr));

			if (varianth == 1 || varianth == 2)
			{
				// 2017:046 anything non-punishment
				if (!strchr(bMYEMYTCIU.types, TYPE_PUNISHMENT_LAW))
				{
					reportGroup.instances = INSTANCE_2ND;
					reportGroup.connectType = CONNECT_TYPE_LOWER_INSTANCE;
					c2courtrep(pParent, reportGroup);
					return;
				}
				// 2017:031
				else
				{
					reportGroup.instances = INSTANCE_2ND;
					reportGroup.connectType = CONNECT_TYPE_LOWER_INSTANCE;
					p2courtrep(pParent, reportGroup);
					return;
				}
			}
			else if (varianth == 3)  // 2018-02-02
			{
				reportGroup.instances = INSTANCE_2ND;
				reportGroup.connectType = CONNECT_TYPE_LOWER_INSTANCE;
				bscourtrep(pParent, reportGroup);
				return;
			}
		}
	#endif  // AREAL || MILITARY

	#if COURT_TYPE == COURT_APPEAL
		// 2017:060
		{
			char varStr[SIZE_OF_ALIAS];  // 01.dec.2004 - begin
			int varianth = query_activity_varianth(bMYEMYTCIU, varStr, sizeof(varStr));

			if (varianth == 1 || varianth == 2)
			{
				// 2017:046 anything non-punishment
				if (!strchr(bMYEMYTCIU.types, TYPE_PUNISHMENT_LAW))
				{
					reportGroup.instances = INSTANCE_ALL;
					reportGroup.connectType = CONNECT_TYPE_RECALL_LAWSUIT;
					c2courtrep(pParent, reportGroup);
					return;
				}
				else
				{
					reportGroup.instances = INSTANCE_ALL;
					reportGroup.connectType = CONNECT_TYPE_RECALL_LAWSUIT;
					p2courtrep(pParent, reportGroup);
					return;
				}
			}
		}
	#endif  // APPEAL

		error("Този вариант на отчета не се поддържа.");
	}
}  // query_activity
#endif // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_ADMIN
void query_activity (TWindow * pParent)
{
	begMYEndMYTypeCIU bMYEMYTCIU ;

	if (TPrintReportBookDialog (pParent, & bMYEMYTCIU).Execute () == IDOK)
	{
		TWaitThreadDialog wtdl;
		TRangeReportGroup reportGroup;

		reportGroup.Import(&bMYEMYTCIU);

		if (bMYEMYTCIU.forVSS)
		{
			reportGroup.instances = INSTANCE_ALL;
			letrangerep(pParent, reportGroup);
		}
		else if (bMYEMYTCIU.forBoss)  // 2018-02-02
		{
			if (bMYEMYTCIU.firstInstLaws)
				bsrangerep(pParent, reportGroup);
			else
			{
				reportGroup.instances = INSTANCE_2ND;
				reportGroup.connectType = CONNECT_TYPE_LOWER_INSTANCE;
				bscourtrep(pParent, reportGroup);
			}
		}
		else
		{
			reportGroup.instances = bMYEMYTCIU.firstInstLaws ? INSTANCE_1ST : INSTANCE_2ND;
			reportGroup.attrib = POST_INTERNAL;
			reportGroup.withSubjects = TRUE;
			letrangerep(pParent, reportGroup);
		}
	}
}
#endif  // ADMIN

void print_decision_judgement (TWindow * pParent,
//                               const  // 09.jun.2000
					 TDatedContainer * pContainer,
					 TLawsuit * pLawsuit,
					 const CDate dateReturned)
 {
  printDecisionOrJudgement (pParent, pContainer, pLawsuit,
				    & dateReturned) ;
 }  // print_decision_judgement

void print_decision_judgement (TWindow * pParent,
//                               const  // 09.jun.2000
					 TDatedContainer * pContainer,
                               TLawsuit * pLawsuit)
 {
  printDecisionOrJudgement (pParent, pContainer, pLawsuit) ;
 }  // print_decision_judgement

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL              // AREAL COURT only
void print_arakelyan_distribution(TWindow* parent,   // punishment laws only ?
	begMYEndMYTypeCIU& rBMYEMYTCIU)
{
  bool first = true;
  bool second = true;
  bool confirm;
  bool goodData;

  do
  {
    goodData = true;

    rBMYEMYTCIU.firstInstLaws = false;
    rBMYEMYTCIU.secondInstLaws = false;

    confirm = T2ChkBoxesDialog(parent, "Разпределение на постъпилите дела",
      "Обхват", "Данни за &първоинстанционни дела",
      "Данни за второинстанционни / &касационни дела",
      first, second).Execute() == IDOK;
    if(confirm)
    {
      if(first == false && second == false)
        goodData = error(NO_OUTPUT_CHOSEN);
    }
  }
  while(confirm == true && goodData == false);

  if ((confirm && goodData) && (first||second))
  {
    if(first && second)
    {
	plainJudgeCheckup plain(parent, rBMYEMYTCIU);

      register_usage(REGIME_JUDGE_INLAWDISTR_CHECKUP, 1,
	  rBMYEMYTCIU.types, "СС Пост");  // 01.dec.2004
      plain.prepare( first, second );
      plain.printPlainJudgeCheckup();
    }
    else
    {
      if(first)
        rBMYEMYTCIU.firstInstLaws = true;
      else
      {
        rBMYEMYTCIU.forBoss = true;
//@@        rBMYEMYTCIU.secondInstLaws = true;
      }

	InstancePlainJudgeCheckup plain(parent, rBMYEMYTCIU);



	register_usage(REGIME_JUDGE_INLAWDISTR_CHECKUP, 2,
	  rBMYEMYTCIU.types, "СС Пост Инст");  // 01.dec.2004
	plain.prepare( first, second);
	plain.printPlainJudgeCheckup();
    }
  }
}  // print_arakelyan_distribution
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE

#include "_PrJdCD_.h"
#include "_PrJdCD.cpp"

#if INSTANCE
#if APPEAL
void print_judge_checkup(TWindow * parent)
{
	begMYEndMYTypeCIU bMYEMYTCIU;

	if(TPrintJudgeCheckupDialog(parent, &bMYEMYTCIU).Execute () == IDOK)
	{
		TWaitThreadDialog wtdl;
		TReportGroup reportGroup;

		reportGroup.Import(&bMYEMYTCIU);
		// 2008:189 LPR: redirect VSS
		if (bMYEMYTCIU.variant == JUDGE_REPORT_VSS)
			lawsuit_judge_report(parent, reportGroup);
		else if (bMYEMYTCIU.variant == JUDGE_REPORT_VSS_APPEAL)
			appeal_judge_report(parent, reportGroup);
		else if (check_finished_lawsuits (bMYEMYTCIU.types, bMYEMYTCIU.BegDate (), bMYEMYTCIU.EndDate(), parent, NULL,
			&(bMYEMYTCIU.finLawsCheckTerminated), &(bMYEMYTCIU.finLawsInconsistent)))
		{
			if (check_sessions(bMYEMYTCIU.types, bMYEMYTCIU.BegDate(), bMYEMYTCIU.EndDate(), parent, 0, NULL, NULL,
				false, true, &(bMYEMYTCIU.sessCheckTerminated), &(bMYEMYTCIU.sessInconsistent)))
			{
				if (strchr(bMYEMYTCIU.types, TYPE_CITIZEN_LAW) || strchr(bMYEMYTCIU.types, TYPE_TRADE_LAW))
				{
					TIlievaJudgeCheckupThread iJC(bMYEMYTCIU);

					register_usage(REGIME_APPEAL_JUDGE_CHECKUP, 1, bMYEMYTCIU.types, "СС");
					iJC.prepare();
					iJC.printJudgeCheckup();
				}
				else
				{
					TJudgeCheckupThread jC(bMYEMYTCIU);

					register_usage(REGIME_JUDGE_CHECKUP, 1, bMYEMYTCIU.types, "СС НД");
					jC.prepare();
					jC.printJudgeCheckup();
				}
			 }
		}			
	}
}  // print_judge_checkup
#   else  // of APPEAL                                // AREAL COURT  -->
void print_judge_checkup (TWindow * parent)
{
	begMYEndMYTypeCIU bMYEMYTCIU;

	if (TPrintJudgeCheckupDialog(parent, &bMYEMYTCIU).Execute () == IDOK)
	{
		TWaitThreadDialog wtdl;
		TReportGroup reportGroup;

		reportGroup.Import(&bMYEMYTCIU);
		// 2008:189 LPR: redirect VSS
		if (bMYEMYTCIU.variant == JUDGE_REPORT_VSS)
			lawsuit_judge_report(parent, reportGroup);
		else if (bMYEMYTCIU.variant == JUDGE_REPORT_VSS_APPEAL)
			appeal_judge_report(parent, reportGroup);
		// 1. -------------------------------------
		else if (bMYEMYTCIU.internalUse)
		{
			if (check_finished_lawsuits (bMYEMYTCIU.types, bMYEMYTCIU.BegDate(), bMYEMYTCIU.EndDate(), parent,
				NULL, &(bMYEMYTCIU.finLawsCheckTerminated), &(bMYEMYTCIU.finLawsInconsistent)))
			{
				if (check_sessions(bMYEMYTCIU.types, bMYEMYTCIU.BegDate(), bMYEMYTCIU.EndDate (), parent, 0, NULL,
					NULL, false, true, &(bMYEMYTCIU.sessCheckTerminated), &(bMYEMYTCIU.sessInconsistent)))
				{
				#if COURT_TYPE == COURT_AREAL	// 2007:186 LPR: FIXME
					if (strlen (bMYEMYTCIU.types) == 1 && bMYEMYTCIU.types [0] == TYPE_FIRM_LAW)
					{
						TFirmJudgeCheckupThread fJC(bMYEMYTCIU);

						register_usage(REGIME_JUDGE_CHECKUP, 2, bMYEMYTCIU.types, "СС ФД");
						fJC.prepare ();
						fJC.printJudgeCheckup();
					}
					else
				#endif  // AREAL
					{
						TJudgeCheckupThread jC(bMYEMYTCIU);

						register_usage(REGIME_JUDGE_CHECKUP, 1, bMYEMYTCIU.types, "СС");
						jC.prepare();
						jC.printJudgeCheckup();
					}
				}
			}
		}
		// 2. -------------------------------------
		else if (bMYEMYTCIU.forBoss)
		{
			if (check_finished_lawsuits (bMYEMYTCIU.types, bMYEMYTCIU.BegDate(), bMYEMYTCIU.EndDate(), parent, NULL,
				&(bMYEMYTCIU.finLawsCheckTerminated), &(bMYEMYTCIU.finLawsInconsistent)))
			{
				if (check_sessions (bMYEMYTCIU.types, bMYEMYTCIU.BegDate(), bMYEMYTCIU.EndDate (), parent, 0, NULL,
					NULL, false, true, &(bMYEMYTCIU.sessCheckTerminated), &(bMYEMYTCIU.sessInconsistent)))
				{
					if (strchr(bMYEMYTCIU.types, TYPE_PUNISHMENT_LAW))
					{
						TKracholovJudgeCheckupThread kJC(bMYEMYTCIU);

						register_usage(REGIME_JUDGE_EXTENDED_CHECKUP, 2, bMYEMYTCIU.types, "ССР НД");
						kJC.prepare();
						kJC.printJudgeCheckup();
					}
					else
					{
						TPdCitJudgeCheckupThread PCJC(bMYEMYTCIU);

						register_usage(REGIME_JUDGE_EXTENDED_CHECKUP, 1, bMYEMYTCIU.types, "ССР");
						PCJC.prepare();
						PCJC.printJudgeCheckup();
					}
				}
			}
		}
		// 3. -------------------------------------
		else if (bMYEMYTCIU.decidedLawsJudgeCheckup)
		{
			if (check_finished_lawsuits(bMYEMYTCIU.types, bMYEMYTCIU.BegDate(), bMYEMYTCIU.EndDate(), parent, NULL,
				&(bMYEMYTCIU.finLawsCheckTerminated), &(bMYEMYTCIU.finLawsInconsistent)))
			{
				if (strlen (bMYEMYTCIU.types) == 1 && bMYEMYTCIU.types [0] == TYPE_FIRM_LAW)
				{
					DecidedFirmLawsPlainJudgeCheckup dfpl(parent, bMYEMYTCIU);

					register_usage(REGIME_JUDGE_FINLAWDISTR_CHECKUP, 2, bMYEMYTCIU.types, "СС Св ФД");
					dfpl.prepare();
					dfpl.printPlainJudgeCheckup();
				}
				else
				{
					DecidedLawsPlainJudgeCheckup plain(parent, bMYEMYTCIU);

					register_usage(REGIME_JUDGE_FINLAWDISTR_CHECKUP, 1, bMYEMYTCIU.types, "СС Св");
					plain.prepare();
					plain.printPlainJudgeCheckup();
				}
			}
		}
		// 4. -------------------------------------
		else if (bMYEMYTCIU.sessionsDistribution)
		{
			register_usage(REGIME_JUDGE_SESS_DISTR_CHECKUP, 1, bMYEMYTCIU.types, "СС Зас");
			session_dist_report(parent, reportGroup);
		}
#if COURT_TYPE == COURT_AREAL
		// 5. -------------------------------------
		else if (strlen(bMYEMYTCIU.types) == 1 && bMYEMYTCIU.types[0] == TYPE_FIRM_LAW)
		{
			register_usage(REGIME_JUDGE_INLAWDISTR_CHECKUP, 3, bMYEMYTCIU.types, "СС Пост Ф");
			print_zdravkova_distribution(parent, bMYEMYTCIU);
		}
#endif  // AREAL
		// Income lawsuits distribution ---------------------
		else
		{
			print_arakelyan_distribution(parent,bMYEMYTCIU);
		}
	}
}  // print_judge_checkup
#   endif  // of APPEAL
# else  // of INSTANCE
void print_judge_checkup (TWindow * parent)
{
	begMYEndMYTypeCIU bMYEMYTCIU;

	if (TPrintJudgeCheckupDialog (parent, &bMYEMYTCIU).Execute() == IDOK)
	{
		TWaitThreadDialog wtdl;
		TReportGroup reportGroup;

		reportGroup.Import(&bMYEMYTCIU);

		if (bMYEMYTCIU.variant == 3)
		{
			rpmnorep(parent, reportGroup);
			return;
		}

		// 2008:183 LPR: redirect VSS
		if (bMYEMYTCIU.variant == JUDGE_REPORT_VSS)
			lawsuit_judge_report(parent, reportGroup);
		else if (bMYEMYTCIU.variant == JUDGE_REPORT_VSS_APPEAL)
			appeal_judge_report(parent, reportGroup);
		else if (check_finished_lawsuits (bMYEMYTCIU.types, bMYEMYTCIU.BegDate(), bMYEMYTCIU.EndDate (), parent, NULL,
			&(bMYEMYTCIU.finLawsCheckTerminated), &(bMYEMYTCIU.finLawsInconsistent)))
		{
			if (check_sessions (bMYEMYTCIU.types, bMYEMYTCIU.BegDate(), bMYEMYTCIU.EndDate(), parent, 0, NULL, NULL,
				false, true, &(bMYEMYTCIU.sessCheckTerminated), &(bMYEMYTCIU.sessInconsistent)))
			{
				TJudgeCheckupThread jC(bMYEMYTCIU);

				register_usage(REGIME_JUDGE_CHECKUP, 1, bMYEMYTCIU.types, "СС");
				jC.prepare();
				jC.printJudgeCheckup();
			}
		}
	}
}  // print_judge_checkup
# endif  // of INSTANCE

void print_appealed_input_diary (TWindow * parent)
 {
  begNEndNYearTypesAC bnenyT ;

  if (TPrintInputDiaryApplLawsDialog (parent, bnenyT).Execute () == IDOK)
  {
    register_usage(REGIME_APPEALED_IN_REG_BOOK, 1, bnenyT.types,
	"Входящ регистър");
    AppealedLawsInputDiary (parent, bnenyT).
	PrintAppealedLawsInputDiary () ;
  }
 }  // print_appealed_input_diary


#if COURT_TYPE == COURT_AREAL
void print_634_trade_book (TWindow * parent)
 {
  CharPtr tmp (strlen (TYPE_TRADE_OR_CITIZEN_LAWS) + 1) ;

  strcpy (tmp, TYPE_TRADE_OR_CITIZEN_LAWS) ;
  intersection (tmp, Default -> colleges) ;
  if (tmp [0])
    TPrint634TLBookDialog (parent).Execute () ;
  else
    error (INSUFFICIENT_RIGHTS_TO_START) ;
 }  // print_634_trade_book
#endif  // AREAL


// 20051010 -->
#if COURT_TYPE == COURT_REGIONAL
static bool ask_new_variant()
{
#if TESTVER
	return ask("Новия вариант?");
#else  // TESTVER
	return true;
#endif  // TESTVER
}

class book_ZZDN : public BookZZDNEnum
 {
  protected :
    TCountRecsQuery * pQ ;
    char * criteria ;
    const size_t critSz ;
    bool selectSessionsOK ;
    ZZDN_Dlg_data  cCBDEDBN_ZZDN ;
    TWindow * pParent ;
    TQuery * pAbPrQ ;
    char * abPrCriteria ;
    //const size_t abPrCritSz ;
    bool init_cCBDEDBN_ZZDN () ;
    bool composeCriteria () ;
    void makeQuery () ;
    //bool composeAbPrCriteria () ;
    //void makeAbPrQuery () ;
    void destroyQueries () ;
    char * makeBookFormatFN () ;
	// returns a pointer to a newly created string or NULL

    void fillCodeValueArray (CodeValueArray & codeValueArr) ;

  public :
    book_ZZDN (TWindow * parent) ;
    virtual ~book_ZZDN () ;
    virtual void printBook_ZZDN () ;
 } ;  // class book_ZZDN

#include "_zzdnbk.cpp"

void print_book_ZZDN (TWindow * pParent)
{
	if (ask_new_variant())
		zzdn_book(pParent);
	else
		book_ZZDN (pParent).printBook_ZZDN () ;
}  // print_book_ZZDN
#endif  // REGIONAL
// 20051010 <--
