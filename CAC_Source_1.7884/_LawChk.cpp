#include "cac.h"

static const char * const NON_FIN_FIRM_LAW_PATCH_FNCT_NAME =
  "\"non_finished_firm_law_patch\": " ;

static void preliminary_buff_check (char * const buffer, const size_t bufferSize)
 {
  if (! buffer)
    fatal ("%sbuffer is NULL", NON_FIN_FIRM_LAW_PATCH_FNCT_NAME) ;
  if (! bufferSize)
    fatal ("%sbufferSize is 0", NON_FIN_FIRM_LAW_PATCH_FNCT_NAME) ;
 }  // preliminary_buff_check

static void default_non_finished_condition (char * const buffer,
						 const size_t bufferSize,
						 const CDate * const pEndD)
 {
  preliminary_buff_check (buffer, bufferSize) ;
  buffer [0] = '\0' ;

  const bool endPPresent = pEndD && pEndD -> Empty () == false ;
  ostrstream oss (buffer, bufferSize) ;

  if (endPPresent)
    oss  <<
     " AND (F_FINISHED IS NULL OR F_FINISHED >= "
	 << (* pEndD).Value() << ")" ;  // 15.apr.2003 - end
  else
    oss  << " AND (F_FINISHED IS NULL)" ;
  oss << ends ;
  buffer [bufferSize - 1] = '\0' ;
 }  // default_non_finished_condition

static void ending_buff_check (char * const buffer, const size_t bufferSize)
 {
  preliminary_buff_check (buffer, bufferSize) ;
  if (strlen (buffer) == bufferSize - 1)
   {
    error ("%sbufferSize too small", NON_FIN_FIRM_LAW_PATCH_FNCT_NAME) ;
    buffer [0] = '\0' ;
   }
 }  // ending_buff_check

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
const char * non_finished_firm_law_patch (char * const buffer,
							const size_t bufferSize,
							const char * const
							  // lawKinds  // 09.oct.2000
							,
							const CDate * const pEndD)
// returns buffer, that contains additional conditons.
//
// this patch may be inserted before "ORDER BY".
//
// pEndP may be null.
// (* pEndP) may be empty.
// In these cases the period is unlimited.
//
// The (* pEndP) day is NOT included in the period.
//
 {
  preliminary_buff_check (buffer, bufferSize) ;
  default_non_finished_condition (buffer, bufferSize, pEndD) ;
  ending_buff_check (buffer, bufferSize) ;
  return buffer ;
 }  // non_finished_firm_law_patch
#endif  // APPEAL || ADMIN || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL
const char * non_finished_firm_law_patch (char * const buffer,
							const size_t bufferSize,
							const char * const lawKinds,
							const CDate * const pEndD)
// returns buffer, that contains additional conditons.
//
// this patch may be inserted before "ORDER BY".
//
// pEndP may be null.
// (* pEndP) may be empty.
// In these cases the period is unlimited.
//
// The (* pEndP) day is NOT included in the period.
//
 {
  preliminary_buff_check (buffer, bufferSize) ;
  default_non_finished_condition (buffer, bufferSize, pEndD) ;

  bool firmRequired = false ;

  if (lawKinds [0])
   {
    char * tmp = new char [strlen (lawKinds) + 1] ;

    strcpy (tmp, lawKinds) ;
    intersection (tmp, KIND_FIRM_LAWS) ;
    if (tmp [0])
	firmRequired = true ;
    delete [] tmp ;
   }
  if (firmRequired)
   {  // -- 0 --
    const bool endPPresent = pEndD && pEndD -> Empty () == false ;
    char dateSpec [128] ;

    dateSpec [0] = '\0' ;
    if (endPPresent)
     {
      ostrstream (dateSpec, sizeof (dateSpec))
	  << " AND T_DECISION.F_ENTER_DATE < "
        << (* pEndD).Value() << " " << ends ;  // 15.apr.2003 - end
     }
    dateSpec [sizeof (dateSpec) - 1] = '\0' ;
    if (strlen (dateSpec) == sizeof (dateSpec) - 1)
      error ("%sdateSpec buffer too short",
             NON_FIN_FIRM_LAW_PATCH_FNCT_NAME) ;
    else
     {  // -- 1 --
//      char resStr [16] ;  // 12.oct.2000 commented

//      scSprintf (sizeof (resStr), resStr, "%c%c%c",
//                 RESULT_FIRM_REGISTRATION,
//                 RESULT_BRANCH_REGISTRATION,
//                 RESULT_REGISTRATION_DENIAL
//                ) ;

	ostrstream oss (buffer + strlen (buffer),
                      bufferSize - strlen (buffer)) ;
      oss
        <<
         " AND ((T_LAWSUIT.F_TYPE != \'" << TYPE_FIRM_LAW << "\'"
               ") OR "
               "(T_LAWSUIT.F_TYPE = \'" << TYPE_FIRM_LAW << "\' AND " ;
      //if (DEFAULT_UCLP == VARNA_UCLP)  // 05.may.2004
      if (COURT_UCLP == VARNA_UCLP)  // 05.may.2004
	  oss
          <<
		   "T_LAWSUIT.F_DATE >= " << VARNA_FIRM_LAWS_NOT_FIN_DATE.Value()
		     << " AND " ;  // 15.apr.2003 - end
	oss
	  <<
                "(SELECT COUNT (F_NO) FROM T_DECISION "
                   "WHERE T_LAWSUIT.F_NO = T_DECISION.F_NO AND "
                         "T_LAWSUIT.F_YEAR = T_DECISION.F_YEAR AND "
                         "T_LAWSUIT.F_TYPE = T_DECISION.F_TYPE AND "
                         "T_DECISION.F_RESULT IN ("
//           << stringToSQLCharSet (resStr).getTheSet ()
		// 2014:219 RESULT_FIRM_REGISTRATIONS -> RESULT_FIRM_FINISHEDS
	    << stringToSQLCharSet (RESULT_FIRM_FINISHEDS).
                                            getTheSet ()  // 12.oct.2000
                                             << ") "
                         << dateSpec <<
                ")"
                  " = 0"
               ")"
		  ") "
        << ends ;
      buffer [bufferSize - 1] = '\0' ;
     }  // -- 1 --
   }  // -- 0 --
  ending_buff_check (buffer, bufferSize) ;
  return buffer ;
 }  // non_finished_firm_law_patch
#endif  // AREAL

bool confirm_bad_finished_laws ()
 {
  return ask ("Бяха открити противоречия в данните "
		  "за свършилите дела.\n""%s", WANT_CONTINUE_QUESTION) ;
 }  // confirm_bad_finished_laws

bool bad_finished_law (const TLawsuit & law,
			     const FinishedLawErrors whichError,
			     outText * pOfs,
			     const bool //errMsg // 04.jan.2001; 2008:255 // unused
			     )
 {

  // 2008:255 LPR: altered theDate/noMatch const/cases for final date
  char tmp [128] ;
  const size_t errSz = 1024 ;
  char * const err = new char [errSz] ;
  const char * const theDate = "Датата от финализиращото " ;
  const char * const noMatch =
    " заседание "
    "не съвпада с датата на свършване на делото." ;

  err [0] = '\0' ;
  TRCDKey_ (law.key).Print (tmp, sizeof (tmp)) ;
  switch (whichError)
   {
    case FLE_NoFinalizedSession :
	scSprintf (errSz, err,
		     "%s:\n""Делото е свършило, но липсва заседанието с "
		     "финализиращ резултат.", tmp) ;
      break ;
    case FLE_TooManyFinalizedSessions :
       {
	const char * rejectCompl = law.decMade.Empty () ? "" :
          "след последната отмяна на хода по същество и " ;

        scSprintf (errSz, err,
                   "%s:\n"
                   "Повече от едно заседание с финализиращ "
                   "резултат %sпреди свършване на делото.",
		   tmp, rejectCompl) ;
       }
      break ;
    case FLE_FinishedBeforeTheCreating :
      scSprintf (errSz, err, "%s:\n"
		 "Делото е свършило преди образуването си.", tmp) ;
      break ;
    case FLE_NoFinalizingJudge :
      scSprintf (errSz, err,
                 "%s:\n""Заседанието, с което делото приключва е без "
                 "съдия-докладчик.", tmp) ;
      break ;
    case FLE_BadFinalizedOpenSessionDate :
      scSprintf (errSz, err,
		 "%s:\n""%s%s%s", tmp, theDate, "открито", noMatch) ;
      break ;
    case FLE_BadFinalizedClSessionDate :
	scSprintf (errSz, err,
		 "%s:\n""%s%s%s", tmp, theDate, "закрито", noMatch) ;
      break ;
    case FLE_Firm_DoubleRegistered :  // 04.jan.2001
      scSprintf (errSz, err,
                 "%s:\n""Повече от една регистрация.", tmp) ;
      break ;
    case FLE_Firm_TooOldRegisteredSoon :  // 04.jan.2001  // for Varna
      scSprintf (errSz, err,
                 "%s:\n""Прекалено старо дело с регистрация през "
                 "разглеждания период.", tmp) ;
      break ;
    case FLE_Firm_FakedDateFinished :  // 04.jan.2001
      scSprintf (errSz, err,
                 "%s:\n""Неправилно обявено за приключило (неподходящ "
                 "финализиращ резултат от заседание).", tmp) ;
      break ;
    case FLE_Firm_LawDoubleFinished :  // 09.apr.2001
	scSprintf (errSz, err,
                 "%s:\n""Свършило е по два различни начина - чрез акт "
                 "относно регистрация и чрез заседание с финализиращ "
                 "резултат.", tmp) ;
      break ;
    case FLE_CanceledAndGranted :  // 30.jan.2002 - begin
      scSprintf (errSz, err,
		 "%s:\n""Делото е прекратено, а има въведена "
		 "\"Степен на уважаване на иска\".", tmp) ;
      break ;
    case FLE_FinishedAndAgeContradiction :
	scSprintf (errSz, err,
		 "%s:\n""Липсва \"Продължителност\".", tmp) ;
      break ;  // 30.jan.2002 - end
    case FLE_BadFinalizedClSessionResult :  // 13.jul.2004
      scSprintf (errSz, err,
		 "%s:\n""Неправилен финализиращ резултат от заседание - "
		 "делото не може да се разпознае нито като "
		 "решено, нито като прекратено.", tmp) ;
      break ;
    case FLE_NotCanceledAndNotGranted :  // 30.jan.2002 - begin
      scSprintf (errSz, err,
		 "%s:\n""Делото е решено по същество, но няма въведена "
		 "\"Степен на уважаване на иска\".", tmp) ;
      break ;
    default :
      scSprintf (errSz, err,
		 "%s:\n""Unspecified error.", tmp) ;
      break ;
   }

  // 20080714 if (errMsg)  // 04.jan.2000
  // 20080714    error (err) ;
       
  if (pOfs)
   {
    const column clmn (70, err) ;

    for (size_t i = 0 ; i < clmn.getRowsCount () ; i ++)
     {
	if (pOfs -> pageLinesLeft () < 2)
        pOfs -> newPage () ;
      (* pOfs) << clmn [i] ;
      pOfs -> newLine () ;
     }
   }
  delete [] err ;
  return false ;
 }  // bad_finished_law

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL

static void registration_res_part_criteria (ostrstream & oss)
 {
  oss <<  "SELECT COUNT (F_NO) FROM T_DECISION "
	    "WHERE T_DECISION.F_NO = T_LAWSUIT.F_NO AND "
	    "T_DECISION.F_YEAR = T_LAWSUIT.F_YEAR AND "
	    "T_DECISION.F_TYPE = T_LAWSUIT.F_TYPE AND "
	    "T_DECISION.F_RESULT IN ("	// 2014:209 T_DECISION.
	   << stringToSQLCharSet (RESULT_FIRM_REGISTRATIONS).getTheSet ()
			 << ")" ;
 }  // registration_res_part_criteria

static void enumerate_double_registered_damns (const CDate begD,
						    const CDate endD,
						    outText * const pOutF = NULL,
						    int * const pCount = NULL,
						    const bool errMsg = true)
 {
  if (pCount)
    (* pCount) = 0 ;

  char criteria [1024] ;
   {  // -- 0 --
    ostrstream oss (criteria, sizeof (criteria)) ;

    oss
	<< "SELECT F_NO, F_YEAR, F_TYPE, F_DATE "
	   "FROM T_LAWSUIT WHERE "
         "F_TYPE = \'" << TYPE_FIRM_LAW << "\' AND " ;
    if (endD.Empty () == false)
      oss
      << "F_DATE <= " << endD.Value() << " AND " ;  // 15.apr.2003
    // --- doubled registration at general
    oss
      << "(" ;
    //if (DEFAULT_UCLP == VARNA_UCLP)  // 05.may.2004
    if (COURT_UCLP == VARNA_UCLP)  // 05.may.2004
      oss
      <<  "F_DATE < " << VARNA_FIRM_LAWS_NOT_FIN_DATE.Value() << " OR " ;//15.apr.2003
    oss << "(" ;
    registration_res_part_criteria (oss) ;
    oss << ") > 1 "
         ")" ;
    // ---
    if (begD.Empty () == false || endD.Empty () == false)
     {  // === double made during the given period
      oss
      << " AND "
         "(" ;
      registration_res_part_criteria (oss) ;
      if (begD.Empty () == false)
        oss
      << " AND T_DECISION.F_ENTER_DATE >= " << begD.Value() ;  // 15.apr.2003
	if (endD.Empty () == false)
        oss
      << " AND T_DECISION.F_ENTER_DATE <= " << endD.Value() ;  // 15.apr.2003
	oss
      << ") > 0 " ;
     }  // ===
    oss
      << "ORDER BY F_YEAR, F_NO ;" << ends ;
   }  // -- 0 --
  criteria [sizeof (criteria) - 1] = '\0' ;
  if (criteria_finished_ok (criteria,
                            "enumerate_double_registered_damns"))
   {  // -- 1 --
    TQuery q (criteria) ;
    int count = 0 ;

    while (q.Read ())
     {  // -- 4 --
      TLawsuit lawsuit ;
      FinishedLawErrors whichErr = FLE_Firm_DoubleRegistered ;

      lawsuit << q ;
      if (
          //DEFAULT_UCLP == VARNA_UCLP &&  // 05.may.2004
          COURT_UCLP == VARNA_UCLP &&  // 05.may.2004
          lawsuit.date < VARNA_FIRM_LAWS_NOT_FIN_DATE)
        whichErr = FLE_Firm_TooOldRegisteredSoon ;
      bad_finished_law (lawsuit, whichErr, pOutF, errMsg) ;
      count ++ ;
     }  // -- 4 --
    if (pCount)
      (* pCount) = count ;
   }  // -- 1 --
 }  // enumerate_double_registered_damns

static bool firm_law_double_finished (const TLawsuit & law)
 {
  if (TRCDKey_ (law.key).FilledOK () == false)
    return false ;
  if (law.key.type != TYPE_FIRM_LAW)
    return false ;

  char crit [512] ;
   {
    ostrstream oss (crit, sizeof (crit)) ;

    oss
	<< "SELECT F_NO, F_YEAR, F_TYPE, F_DATE "
         "FROM T_LAWSUIT WHERE "
         "F_TYPE = \'" << law.key.type << "\' AND "
         "F_YEAR = " << law.key.year << " AND " 
         "F_NO = " << law.key.no << " AND "
          "((" ;
    registration_res_part_criteria (oss) ;
    oss << ") != 0 "
          ") " ;
    oss << ";" << ends ;
   }
  crit [sizeof (crit) - 1] = '\0' ;
  if (criteria_finished_ok (crit, "firm_law_double_finished"))
   {
    TQuery q (crit) ;
    return q.Read () ;
   }
  else
    return false ;
 }  // firm_law_double_finished

#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // INSTANCE

class TCheckFinishedLawsThread : public TCheckupThread
{
	protected:
		const char* const types;
		const CDate begDate;
		const CDate endDate;
		const char* const kinds;
		bool& okYet;
		bool& errFound;
    outText* const pOf;
		virtual bool ArgsOk();
		virtual void MakeGeneralRow();
		virtual void Execute();

	public:
		TCheckFinishedLawsThread(const char* const types_,
			const CDate begD, const CDate endD,
			const char * const kinds,
			bool& okYet_, bool& errorFound, outText& outF);

	bool WantBreak() { return wtdl->WantBreak(); }
};

TCheckFinishedLawsThread::
TCheckFinishedLawsThread(const char* const types_, const CDate begD,
	const CDate endD, const char* const kinds_,
  bool& okYet_, bool& errorFound, outText& outF):
		TCheckupThread(
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
			(strchr (types_, TYPE_FIRM_LAW) ? 30 : 0)
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE
			),
		types(types_), begDate(begD), endDate(endD), kinds(kinds_),
    okYet(okYet_), errFound(errorFound), pOf(outF ? &outF : NULL)
{
}

bool TCheckFinishedLawsThread::ArgsOk()
{
	return (TCheckupThread::ArgsOk() && types && types[0] &&
	begDate.Empty() == false && endDate.Empty() == false);
}

void TCheckFinishedLawsThread::MakeGeneralRow()
{
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
	if (strchr (types, TYPE_FIRM_LAW))
	{
    wtdl->SetHeader("Проверка за фирмени дела с повече от една "
    	"регистрация...");
		int dblRegCnt = 0 ;

		enumerate_double_registered_damns (begDate, endDate, pOf,
                                           & dblRegCnt) ;
		if (okYet)
			if (dblRegCnt)
				okYet = false ;
	}
#   endif  // of APPEAL
# endif  // of INSTANCE  // 04.jan.2001 - begin
}  // CheckFinishedLawsT::MakeGeneralRow

void TCheckFinishedLawsThread::Execute()
{
  try
  {  // -- 0 --
    wtdl->SetHeader("Проверка на свършилите дела....");
    wtdl->PleaseWait();

    FinalizedSessions finSess (types, begDate, endDate, kinds) ;
    TLawsuit law ;

    totalPartSteps = finSess.FinishedLawsNumber();
    endingVal = barHiVal - generalRowSteps;
    if(endingVal < barLowVal)
	endingVal = barLowVal;
    wtdl->SetProgressRange(barLowVal, barHiVal);
    {  // -- 1 --
	SessPtr pSess;

	do
	{  // -- 2 --
	  const bool nextGone =
	    finSess.NextSession (law, (* pSess), pOf) ;

		Tick(false);
	  if(wtdl->WantBreak())
		break;
	  if (TRCDKey_ (law.key).FilledOK ())
	    if (law.finished < law.date)
	    {
		bad_finished_law (law, FLE_FinishedBeforeTheCreating,
					pOf) ;
		errFound = true ;
	    }
	  if (nextGone)
	  {  // -- 3 --
	    if (! ucnPresent (pSess.Ref ().judge))
	     {
		bad_finished_law (law, FLE_NoFinalizingJudge, pOf) ;
		errFound = true ;
	     }
	    if (law.age == '\0' ||  // 30.jan.2002 - begin
		  law.age == AGE_NONE_YET)
	     {
		bad_finished_law (law, FLE_FinishedAndAgeContradiction,
					pOf) ;
		errFound = true ;
	     }  // 30.jan.2002 - end
# if INSTANCE  // 04.jan.2001 - begin
#   if APPEAL
#   else  // of APPEAL
	    if (law.key.type == TYPE_FIRM_LAW)
		if (! strchr (RESULT_FIRM_LEGAL_FINALIZEDS,
				  pSess.Ref ().result))
		 {
		  bad_finished_law (law, FLE_Firm_FakedDateFinished, pOf) ;
		  errFound = true ;
		 }
		else  // 09.apr.2001 - begin
		if (firm_law_double_finished (law))
		 {
		bad_finished_law (law, FLE_Firm_LawDoubleFinished,
				  pOf) ;
		errFound = true ;
		 }  // 09.apr.2001 - end
	  if (strchr (KIND_1ST_LAWSUITS, law.kind))  // 30.jan.2002-begin
	    if (law.grant && law.grant != GRANT_NONE_YET)
	    {
		if (sessionCancelsTheLaw (* pSess, & law,
					strchr (KIND_CLOSED_SESSIONS,
						pSess.Ref ().kind) ==
					NULL)
		 )
		 {
		bad_finished_law (law, FLE_CanceledAndGranted, pOf) ;
		errFound = true ;
		 }  // 30.jan.2002 - end
	    }
	    else if (law.key.type != TYPE_PUNISHMENT_LAW &&
			!sessionCancelsTheLaw (* pSess, & law,
			strchr (KIND_CLOSED_SESSIONS, pSess.Ref ().kind) ==
			NULL)
		 )
	    {
		bad_finished_law (law, FLE_NotCanceledAndNotGranted, pOf) ;
		errFound = true ;
	    }
#   endif  // of APPEAL
# else  // of INSTANCE
	  if (law.grant &&  // 30.jan.2002 - begin
		law.grant != GRANT_NONE_YET)
	  {
	    if (sessionCancelsTheLaw (* pSess, & law,
					strchr (KIND_CLOSED_SESSIONS,
						pSess.Ref ().kind) ==
					NULL)
		 )
	     {
		bad_finished_law (law, FLE_CanceledAndGranted, pOf) ;
		errFound = true ;
	     }  // 30.jan.2002 - end
	  }
	  else if (law.key.type != TYPE_PUNISHMENT_LAW &&
		!sessionCancelsTheLaw (* pSess, & law,
		strchr (KIND_CLOSED_SESSIONS,
		pSess.Ref ().kind) == NULL)
	  )
	  {
	    bad_finished_law (law, FLE_NotCanceledAndNotGranted, pOf) ;
	    errFound = true ;
	  }
# endif  // of INSTANCE  // 04.jan.2001 - end
	}  // -- 3 --
	}  // -- 2 --
	while (TRCDKey_ (law.key).FilledOK ()) ;
	    Tick(true);                                      // Gossip service
	if (finSess.BadIntegrity ())
	  okYet = false ;
	if (okYet)
	  if (errFound)
	    okYet = false ;
    }  // -- 1 --
    if(endingVal < barHiVal)
	endingVal = barHiVal;
		currentPartStep = 0;
    if(wtdl->WantBreak() == false)
	    MakeGeneralRow();
    Tick(true);
    wtdl->AlmostDone();
	}  // -- 0 --
	CATCH_ANYTHING
}

bool check_finished_lawsuits(const char * const types, const CDate &begDate, const CDate &endDate, TWindow *parent,
	const char *const kinds, bool *pCheckTerminated, bool *pInconsistencyFound)
{
	if (!(types && types[0] && (begDate.Empty() == false && endDate.Empty() == false)))
		return false;

	bool okYet = true;
	long outPos;
	bool errFound = false;
	outText of(parent, EMPTY_FMT);

	try
	{
		TCheckFinishedLawsThread checker(types, begDate, endDate, kinds, okYet, errFound, of);
 		checker.Generate();

		if (pCheckTerminated)
			*pCheckTerminated = checker.WantBreak();
	}
	CATCH_ANYTHING

	outPos = of.pcount();
	of.newPage();
	of.newLine();

	if (okYet == false)
	{
		if (pInconsistencyFound)
			*pInconsistencyFound = true;

		okYet = confirm_bad_finished_laws();
	}

	if (!(okYet && outPos == 0))
		show_text_stream_use_settings(NULL, of, EMPTY_FMT, FLOAT_ALIGNMENT);

	return okYet;
}
