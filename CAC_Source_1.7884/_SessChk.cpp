#include "cac.h"  // 18.may.2001
//# include "all.h"

bool bad_session (const BriefSessionsToCheckGroup & sess,
		   const SessionErrors whichError,
               outText * pOfs, const bool errMsg)
 {
  char tmp [196] ;
  const size_t errSz = 1024 ;
  char * const err = new char [errSz] ;
  const char * const UnexpMembersh1 = "Броят съдии не е " ;
  const char * const UnexpMembersh2 = ", както се очаква" ;

  err [0] = '\0' ;
  DatedContainer_ (sess).Print (tmp, sizeof (tmp)) ;
  switch (whichError)
   {
    case SE_WrongMembership1 :
      scSprintf (errSz, err,
                 "%s:\n""%s1%s.", tmp, UnexpMembersh1, UnexpMembersh2) ;
      break ;
    case SE_WrongMembership2 :
      scSprintf (errSz, err,
                 "%s:\n""%s2%s.", tmp, UnexpMembersh1, UnexpMembersh2) ;
      break ;
    case SE_WrongMembership3 :
      scSprintf (errSz, err,
                 "%s:\n""%s3%s.", tmp, UnexpMembersh1, UnexpMembersh2) ;
      break ;
    case SE_BadFixConcerningNextSess :
      scSprintf (errSz, err,
                 "%s:\n"
                 "Заседанието е докладвано, но предходното заседание "
                 "по делото все още не е. ВНИМАНИЕ, това противоречие се "
                 "открива само ако и двете заседания са в разглеждания "
                 "период.",
                 tmp) ;
      break ;
    case SE_BadFixConcerningLawDate :
      scSprintf (errSz, err,
                 "%s:\n""Датата на заседанието е по-малка от датата на "
                 "образуване на делото.", tmp) ;
      break ;
    case SE_DifferentCompositions :
      scSprintf (errSz, err,
                 "%s:\n""Съставът не съвпада със състава на делото.",
		     tmp) ;
	break ;
    default :
	scSprintf (errSz, err,
		     "%s:\n""Unspecified error.", tmp) ;
	break ;
   }
  if (errMsg)  // 04.jan.2000
    error (err) ;
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
 }  // bad_session

class TCheckSessionsThread : public TCheckupThread
{
	protected:
		const char* const types;
		const CDate begDate;
		const CDate endDate;
		const long int composition;
		const char* const lawKinds;
		const char* const sessKinds;
		const bool excludeResFixed;
		const bool closedMembershipIsAlwaysOK;
		bool& okYet;
		outText* const pOf;
		virtual bool ArgsOk();
		virtual void Execute();

	public:
		TCheckSessionsThread(const char* const types_, const CDate begD,
			const CDate endD, const long int composit,
			const char* const lawKinds_, const char* const sessKinds_,
			const bool excludeResFixed_,
			const bool closedMembershipIsAlwaysOK_, bool& okYet_,
			outText& of);

		bool WantBreak() { return wtdl->WantBreak(); }
};

TCheckSessionsThread::
TCheckSessionsThread(const char* const types_, const CDate begD,
	const CDate endD, const long int composit,
	const char* const lawKinds_, const char* const sessKinds_,
	const bool excludeResFixed_,
	const bool closedMembershipIsAlwaysOK_, bool& okYet_,
  outText& of):
		types(types_), begDate(begD), endDate(endD), composition(composit),
		lawKinds(lawKinds_), sessKinds(sessKinds_),
		excludeResFixed(excludeResFixed_),
		closedMembershipIsAlwaysOK(closedMembershipIsAlwaysOK_),
		okYet(okYet_), pOf(of ? &of : NULL)
{
}

bool TCheckSessionsThread::ArgsOk()
{
	return (TCheckupThread::ArgsOk() && types && types[0] &&
		begDate.Empty() == false && endDate.Empty() == false);
}

void TCheckSessionsThread::Execute()
{
	try
	{  // -- 0 --
    wtdl->SetHeader("Проверка на заседанията....");
    wtdl->PleaseWait();

    SessionsToCheck sessions (types, begDate, endDate, composition,
                                lawKinds, sessKinds, excludeResFixed) ;
    TLawsuit law ;
    BriefSessionsToCheckGroup prevSess ;
    BriefSessionsToCheckGroup sess ;

    totalPartSteps = sessions.SessionsFound();
    wtdl->SetProgressRange(0, totalPartSteps);
    endingVal = totalPartSteps;
    clearGroupData (law) ;
    while (sessions.NextSession (sess))
     {  // -- 2 --
	Tick(false);
	if(wtdl->WantBreak())
		break;
      if (! (TRCDKey_ (law.key) == TRCDKey_ (sess.key)))
       {
        law.key = sess.key ;
        if (! law.Get ())
	   {
          clearGroupData (law) ;
          law.key = sess.key ;
         }
       }
      if (sess.result != RESULT_FIXED && sess.result != RESULT_DECREE)
        if (law.finished.Empty () || sess.date <= law.finished)
          if (judges_membership_ok_spk (law.kind, sess,
                                        closedMembershipIsAlwaysOK, pOf)
              == false)
          okYet = false ;
      if (sess.date < law.date)
        okYet = bad_session (sess, SE_BadFixConcerningLawDate, pOf) ;
	if (excludeResFixed == false)
        if (TRCDKey_ (prevSess.key) == TRCDKey_ (sess.key))
          if (prevSess.result == RESULT_FIXED &&
		  sess.result != RESULT_FIXED)
           okYet = bad_session (sess, SE_BadFixConcerningNextSess, pOf) ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
	if (composition)
	  if (sess.composition != law.composition)
	    okYet = bad_session (sess, SE_DifferentCompositions, pOf) ;
#   endif  // of APPEAL
# else  // of INSTANCE
	if (composition)
	  if (sess.composition != law.composition)
	    okYet = bad_session (sess, SE_DifferentCompositions, pOf) ;
# endif  // of INSTANCE
	if (excludeResFixed == false)
	  prevSess.MoveDataFromBSCG (sess) ;
     }  // -- 2 --
    Tick(true);
    wtdl->AlmostDone();
	}  // -- 0 --
	CATCH_ANYTHING
}

bool check_sessions(const char *const types, const CDate &begDate, const CDate &endDate, TWindow *parent,
	const long composition, const char *const lawKinds, const char *const sessKinds, const bool excludeResFixed,
	const bool closedMembershipIsAlwaysOK, bool *pCheckTerminated, bool *pInconsistencyFound)
{
	if (!(types && types[0] && (begDate.Empty() == false && endDate.Empty() == false)))
		return false;

	bool okYet = true;
	long outPos;
	outText of(parent, EMPTY_FMT);

	{
		TCheckSessionsThread checker(types, begDate, endDate, composition, lawKinds, sessKinds, excludeResFixed,
			closedMembershipIsAlwaysOK, okYet, of);

		checker.Generate();

		if (pCheckTerminated)
			*pCheckTerminated = checker.WantBreak();
	}

	outPos = of.pcount();
	of.newPage();
	of.newLine();

	if (okYet == false)
	{
		if (pInconsistencyFound)
			*pInconsistencyFound = true;

		okYet = ask("Бяха открити противоречия в данните за заседанията.\n%s", Q_CONTINUE);
	}

	if (!(okYet && outPos == 0))
		show_text_stream_use_settings(NULL, of, EMPTY_FMT, FLOAT_ALIGNMENT);

	return okYet;
}
