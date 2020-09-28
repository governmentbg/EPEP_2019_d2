#include "cac.h"  // 18.may.2001
//# include "all.h"

# if INSTANCE
#   if APPEAL

// ========================= IlievaSummary ==============================
// ============================= Begin ==================================

const char* const INFORMATION_OF_COURT_WORK =
	"Сведение за работата на съда";

void TIlievaSummary :: MakeFormatFN ()
 {
	scSprintf (sizeof (formatFN), formatFN,
		 "text\\i_sm%c%s",  // "text\\i_sm%s",  // 23.jun.2004 - begin
		 strlen(rBMYEMYTCIU.types) == 1 ?
		   rBMYEMYTCIU.types[0] :
		   TYPESIGN_GENERAL_REPORT,  // 23.jun.2004 - end
						 DEFAULT_FORMAT_FILE_EXTENSION) ;
 }

void TIlievaSummary::RegimeStr(CharPtr& dest, const char* const regime)
{
	if(dest.Size() > 0)
	{
		{
			ostrstream oss(dest, dest.Size ());

			oss << INFORMATION_OF_COURT_WORK;
			if(regime && regime[0])
				oss << " - " << regime;
			oss << ends;
		}
		dest[dest.Size() - 1] = '\0';
	}
}

void TIlievaSummary ::
RequestsDistribute (TRequest & req, long int & vReqWithoutLaw,
			  long int & vResolutions) const
 {
  bool toLaw = false ;
  RequestAllResolutions resolutions (req.key) ;
  RequestResolutionPtr pResol ;

  while (resolutions.NextWholeRecord (pResol.Ref ()))
   {
    const TRequestResolution & r = pResol.Ref () ;

    if (r.resolution == RESOLUTION_LAWED)
      if (r.date <= endD)
        if (toLaw == false)
          toLaw = true ;
    if (r.resolution == '\0' || r.resolution == RESOLUTION_FAKE ||
        r.resolution == RESOLUTION_LAWED ||
        r.resolution == RESOLUTION_NEW)
      continue ;
    if (r.date < begD)
      continue ;
    if (r.date > endD)
      continue ;
    vResolutions ++ ;
   }
  if (toLaw == false)
    if (begD <= req.date && req.date <= endD)
      vReqWithoutLaw ++ ;
 }

void TIlievaSummary :: ResolutionPartialCriteria (ostream & os,
								 const CDate begDate,
								 const CDate endDate)
 {
  os << " ((select count (rr.f_no) from t_request_resolution rr "
             "where rr.f_type = r.f_type and "
                   "rr.f_year = r.f_year and "
                   "rr.f_no = r.f_no and "
//   "(rr.f_date between \"" << begDate << "\" and "  // 15.apr.2003 - begin
//                                      "\"" << endDate << "\""
			 "(rr.f_date between " << begDate.Value() << " and "
							<< endDate.Value() << ""  // 15.apr.2003-end
                   ")"
          ") > 0"
         ") " ;
 }

bool TIlievaSummary :: RequestsCriteria (char * const dest,
						    const size_t destSz,
						    const char * const types,
						    const CDate begDate,
						    const CDate endDate)
 {
  if (! (dest && destSz && types && types [0] &&
	   (begDate.Empty () == false && endDate.Empty () == false)
        ))
    return false ;

  char kindsStr [SIZE_OF_KINDS] ;
   {
    ostrstream oss (kindsStr, sizeof (kindsStr)) ;

    if (strchr (types, TYPE_CITIZEN_LAW))
	oss << KIND_CITIZEN_INDOCS ;
    if (strchr (types, TYPE_TRADE_LAW))  // 23.jun.2004
	oss << KIND_TRADE_INDOCS ;         // 23.jun.2004
    if (strchr (types, TYPE_PUNISHMENT_LAW))
	oss << KIND_PUNISHMENT_INDOCS ;
    oss << ends ;
    kindsStr [sizeof (kindsStr) - 1] = '\0' ;
    if (! (kindsStr [0]))
      return false ;
   }
  dest [0] = dest [destSz - 1] = '\0' ;
   {
    ostrstream oss (dest, destSz - 1) ;

    oss
	<< "SELECT R.F_NO, R.F_YEAR, R.F_TYPE"
         ", R.F_DATE "  // 18.mar.2002
       ;
    oss << " "
         "FROM T_REQUEST R WHERE "
           "R.F_TYPE IN (\'" << TYPE_REQUEST << "\') AND "
           "R.F_KIND IN ("
             << stringToSQLCharSet (kindsStr).getTheSet () <<
                     ") AND " ;
    ResolutionPartialCriteria (oss, begDate, endDate) ;
    oss << " ;" << ends ;
   }
  return criteria_finished_ok (dest, "I?????Summary::RequestsCriteria") ;
 }

void TIlievaSummary :: CountRequests (long int & vReqWithoutLaw,
						 long int & vResolutions)
 {
  const size_t crtSz = 1024 * 2 ;
  char * const crt = new char [crtSz] ;

  if (RequestsCriteria (crt, crtSz, rBMYEMYTCIU.types, begD, endD))
   {  // -- 0 --
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, "Преписки");
		wtdl->SetHeader(regimeDescr);
		TCountRecsQuery  // TQuery  // 14.jun.2002
			q (crt) ;
		TRequest req ;

		totalPartSteps = q.getRecsCount();
		while (q.Read ())
		 {
			Tick(false);
			if(wtdl->WantBreak())
				break;
			req << q ;
			RequestsDistribute (req, vReqWithoutLaw, vResolutions) ;
		 }
		codeValueArr.Add(new CodeValue(10472, vReqWithoutLaw)) ;
    codeValueArr.Add(new CodeValue(10493, vResolutions)) ;
   }  // -- 0 --
  delete [] crt ;
 }

void TIlievaSummary ::
NotFinishedBegLawsDistribute (TLawsuit & law,
					long int & vNotFinTotal,
					long int & v2ndNotFin,
					long int & vPrvNotFin,
				  long int & v2ndNotFinBankruptcy,  // 19.feb.2001
				  long int & vPrvNotFinBankruptcy   // 19.feb.2001
				     ) const
 {
  vNotFinTotal ++ ;
  if (bankruptcy_law (law))  // 19.feb.2001 - begin
   {
    if (law.kind == KIND_CIT_2ND_LAW
	  || law.kind == KIND_TRA_2ND_LAW  // 23.jun.2004
	 )
      v2ndNotFinBankruptcy ++ ;
    else
      if (law.kind == KIND_CIT_PRV_LAW
          || law.kind == KIND_TRA_PRV_LAW  // 23.jun.2004
         )
        vPrvNotFinBankruptcy ++ ;
   }
  else  // 19.feb.2001 - end
   {
    if (law.kind == KIND_CIT_2ND_LAW
        || law.kind == KIND_TRA_2ND_LAW  // 23.jun.2004
       )
      v2ndNotFin ++ ;
    else
      if (law.kind == KIND_CIT_PRV_LAW
          || law.kind == KIND_TRA_PRV_LAW  // 23.jun.2004
         )
        vPrvNotFin ++ ;
   }
 }

void TIlievaSummary :: CountNotFinishedBegLaws ()
 {
  const size_t crtSz = 1024 ;
  char * const crt = new char [crtSz] ;

	crt [crtSz - 1] = '\0' ;
  ostrstream (crt, crtSz - 1)
    << "SELECT "
	   "F_TYPE, F_YEAR, F_NO, F_SUBJECT, "  // 19.feb.2001
	   "F_KIND FROM T_LAWSUIT WHERE "
	   "F_TYPE IN ("
	 << stringToSQLCharSet (rBMYEMYTCIU.types).getTheSet () <<
			 ") AND "
	   "(F_FINISHED IS NULL OR "
	    "F_FINISHED >= " << begD.Value() <<  // 15.apr.2003
				 ") AND "
	   "F_DATE < " << begD.Value() << " ;" << ends ;  // 15.apr.2003
  if (criteria_finished_ok (crt,
				    "I?????Summary::CountNotFinishedBegLaws"))
   {  // -- 0 --
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, "Несвършени дела");
		wtdl->SetHeader(regimeDescr);
		TCountRecsQuery  // TQuery  // 14.jun.2002
			q (crt) ;
    TLawsuit law ;
    long int vNotFinTotal = 0 ;
    long int v2ndNotFin = 0 ;
    long int vPrvNotFin = 0 ;
    long int v2ndNotFinBankruptcy = 0 ;  // 19.feb.2001
    long int vPrvNotFinBankruptcy = 0 ;  // 19.feb.2001

	  totalPartSteps = q.getRecsCount();
		while (q.Read ())
     {
			Tick(false);
			if(wtdl->WantBreak())
				break;
	law << q ;
	NotFinishedBegLawsDistribute (law, vNotFinTotal,
						v2ndNotFin, vPrvNotFin,
                                    v2ndNotFinBankruptcy,  // 19.feb.2001
                                    vPrvNotFinBankruptcy   // 19.feb.2001
                                   ) ;
     }
    codeValueArr.Add(new CodeValue(10465, vNotFinTotal)) ;
    codeValueArr.Add(new CodeValue(10466, v2ndNotFin)) ;
    codeValueArr.Add(new CodeValue(10467, vPrvNotFin)) ;
    codeValueArr.Add(new CodeValue(10515, v2ndNotFinBankruptcy)) ;
    codeValueArr.Add(new CodeValue(10516, vPrvNotFinBankruptcy)) ;
   }  // -- 0 --
  delete [] crt ;
 }

void TIlievaSummary ::
ComeInLawsDistribute (TLawsuit & law, long int & v2ndComeIn,
			    long int & vPrvComeIn,
			    long int & vSlowdownComeIn,
			    long int & vTotalComeInLaws,
											long int & v2ndComeInBankruptcy,
											long int & vPrvComeInBankruptcy,
											long int & vPrvComeInPetition  // 14.jun.2002
										 ) const
 {
	vTotalComeInLaws ++ ;
	if (bankruptcy_law (law))
	 {
		if (law.kind == KIND_CIT_2ND_LAW
	  || law.kind == KIND_TRA_2ND_LAW  // 23.jun.2004
	 )
			v2ndComeInBankruptcy ++ ;
		else
			if (law.kind == KIND_CIT_PRV_LAW
	    || law.kind == KIND_TRA_PRV_LAW  // 23.jun.2004
	   )
				vPrvComeInBankruptcy ++ ;
	 }
	else
	 {
		if (law.kind == KIND_CIT_2ND_LAW
	  || law.kind == KIND_TRA_2ND_LAW  // 23.jun.2004
	 )
			v2ndComeIn ++ ;
		else  // 14.jun.2002 - begin
			if (strchr (KIND_PETITIONS, law.sourceKind))
				vPrvComeInPetition ++ ;  // 14.jun.2002 - end
			else
				if (strchr (KIND_PRIVATE_COMPLAINTS, law.sourceKind))
					vPrvComeIn ++ ;
				else
					if (strchr (KIND_SLOWDOWN_COMPLAINTS, law.sourceKind))
						vSlowdownComeIn ++ ;
	 }
 }

void TIlievaSummary :: CountComeInLaws (const long int vReqWithoutLaw)
 {
	const size_t crtSz = 1024 ;
	char * const crt = new char [crtSz] ;

	crt [crtSz - 1] = '\0' ;
	ostrstream (crt, crtSz - 1)
		<< "SELECT "
				 "F_TYPE, F_YEAR, F_NO, F_SUBJECT, "  // 19.feb.2001
				 "F_KIND, F_SOURCE_KIND FROM T_LAWSUIT WHERE "
				 "F_TYPE IN ("
				 << stringToSQLCharSet (rBMYEMYTCIU.types).getTheSet () <<
									 ") AND "
					"F_DATE BETWEEN " << begD.Value() << " AND "
					<< endD.Value()   // 15.apr.2003 - end
		<< " ;" << ends ;
	if (criteria_finished_ok (crt, "I?????Summary::CountComeInLaws"))
	 {  // -- 0 --
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, "Постъпили дела");
		wtdl->SetHeader(regimeDescr);
		TCountRecsQuery  // TQuery  // 14.jun.2002
			q (crt) ;
		TLawsuit law ;
		long int v2ndComeIn = 0 ;
		long int vPrvComeIn = 0 ;
		long int vSlowdownComeIn = 0 ;
		long int vTotalComeInLaws = 0 ;
		long int v2ndComeInBankruptcy = 0 ;
		long int vPrvComeInBankruptcy = 0 ;
		long int vPrvComeInPetition = 0 ;  // 14.jun.2002

		totalPartSteps = q.getRecsCount();
		while (q.Read ())
		 {
			Tick(false);
			if(wtdl->WantBreak())
				break;
			law << q ;
			ComeInLawsDistribute (law, v2ndComeIn, vPrvComeIn,
				vSlowdownComeIn, vTotalComeInLaws,
				v2ndComeInBankruptcy, vPrvComeInBankruptcy,
				vPrvComeInPetition) ;
     }
    codeValueArr.Add(new CodeValue(10468, vReqWithoutLaw +
                                        vTotalComeInLaws)) ;
    codeValueArr.Add(new CodeValue(10469, v2ndComeIn)) ;
    codeValueArr.Add(new CodeValue(10470, vPrvComeIn)) ;
    codeValueArr.Add(new CodeValue(10471, vSlowdownComeIn)) ;
    codeValueArr.Add(new CodeValue(10517, v2ndComeInBankruptcy)) ;
    codeValueArr.Add(new CodeValue(10518, vPrvComeInBankruptcy)) ;
    codeValueArr.Add(new CodeValue(10567, vPrvComeInPetition)) ;
	 }  // -- 0 --
  delete [] crt ;
 }

void TIlievaSummary ::
FinishedLawsDistribute (TLawsuit & law, TSession & sess,
				long int & vFinTotal, long int & v2ndDecTotal,
				long int & v2ndDecUpTo3, long int & v2ndDecOver3,
				long int & v2ndCancelOpenSess,
				long int & v2ndCancelClosedSess,
				long int & vPrvDefin,
				long int & vSlowdownDefin,
												long int & v2ndDecidedOpenSessBankruptcy,
												long int & v2ndDecidedClosedSessBankruptcy,
								    long int & v2ndCanceledBankruptcy,
												long int & vPrvDecidedBankruptcy,
												long int & vPrvCanceledBankruptcy,
									long int & vPrvDecidedBankruptcyClosedS,   // 20.sep.2001
									long int & vPrvCanceledBankruptcyClosedS,  // 20.sep.2001
												long int & vPrvDefinClosedS,  // 20.sep.2001
												long int & vPrv37DefinClosedS,  // 14.jun.2002
												long int & vPrv37DefinOpenS   // 14.jun.2002
											 ) const
 {
  const bool openSess = ! strchr (KIND_CLOSED_SESSIONS, sess.kind) ;
  const bool decided = sessionDecidesTheLaw (sess, & law, openSess) ;
  const bool canceled = sessionCancelsTheLaw (sess, & law, openSess) ;

  vFinTotal ++ ;
  if (decided == canceled)  // 19.feb.2001 - begin
   {
    unknown_finished_law_status (law.key) ;
    return ;
   }
  if (bankruptcy_law (law))
	 {  // -- bankruptcy --
		if (law.kind == KIND_CIT_2ND_LAW
        || law.kind == KIND_TRA_2ND_LAW  // 23.jun.2004
       )
     {
      if (decided)
        if (openSess)
					v2ndDecidedOpenSessBankruptcy ++ ;
        else
          v2ndDecidedClosedSessBankruptcy ++ ;
			else
				v2ndCanceledBankruptcy ++ ;
     }
    else
      if (law.kind == KIND_CIT_PRV_LAW
          || law.kind == KIND_TRA_PRV_LAW  // 23.jun.2004
         )
       {
        if (decided)
          if (openSess)  // 20.sep.2001
            vPrvDecidedBankruptcy ++ ;
          else  // 20.sep.2001
            vPrvDecidedBankruptcyClosedS ++ ;  // 20.sep.2001
        else
          if (openSess)  // 20.sep.2001
            vPrvCanceledBankruptcy ++ ;
          else  // 20.sep.2001
						vPrvCanceledBankruptcyClosedS ++ ;  // 20.sep.2001
			 }
   }  // -- bankruptcy --
  else  // 19.feb.2001 - end
    if (openSess)
		 {  // -- 0 --
      if (law.kind == KIND_CIT_2ND_LAW
          || law.kind == KIND_TRA_2ND_LAW  // 23.jun.2004
         )
       {  // -- 1 --
				if (decided)
				 {  // -- 2 --
          v2ndDecTotal ++ ;
          if (strchr (AGE_UP_TO_THREES, law.age))
            v2ndDecUpTo3 ++ ;
          else
            if (strchr (AGE_MORE_THAN_THREES_ALL, law.age))
              v2ndDecOver3 ++ ;
         }  // -- 2 --
        else
					v2ndCancelOpenSess ++ ;
       }  // -- 1 --
      else  // 20.sep.2001 - begin
        if (law.kind == KIND_CIT_PRV_LAW
            || law.kind == KIND_TRA_PRV_LAW  // 23.jun.2004
           )
				 {
					if (sess.result == RESULT_CIT_37)  // 14.jun.2002 - begin
						vPrv37DefinOpenS ++ ;
					else  // 14.jun.2002 - end
						if (strchr (KIND_PRIVATE_COMPLAINTS, law.sourceKind))
							vPrvDefin ++ ;
						else  // 14.jun.2002 - begin
							if (strchr (KIND_SLOWDOWN_COMPLAINTS, law.sourceKind))
								vSlowdownDefin ++ ;  // 14.jun.2002 - end (also in Closed)
				 }  // 20.sep.2001 - end
		 }  // -- 0 --
		else
		 {  // -- 3 --
			if (law.kind == KIND_CIT_2ND_LAW
          || law.kind == KIND_TRA_2ND_LAW  // 23.jun.2004
         )
			 {
				if (canceled)
					v2ndCancelClosedSess ++ ;
			 }
			else
				if (law.kind == KIND_CIT_PRV_LAW  // 20.sep.2001
            || law.kind == KIND_TRA_PRV_LAW  // 23.jun.2004
           )
				 {
					if (sess.result == RESULT_CIT_37)  // 14.jun.2002 - begin
						vPrv37DefinClosedS ++ ;
					else  // 14.jun.2002 - end
						if (strchr (KIND_PRIVATE_COMPLAINTS, law.sourceKind))
							vPrvDefinClosedS++;//vPrvDefin-->vPrvDefinClosedS 20.sep.2001
						else
							if (strchr (KIND_SLOWDOWN_COMPLAINTS, law.sourceKind))
								vSlowdownDefin ++ ;  // Also in Open sessions
				 }
		 }  // -- 3 --
 }

void TIlievaSummary :: CountFinishedLaws ()
 {
  long int vFinTotal = 0 ;
  long int v2ndDecTotal = 0 ;
  long int v2ndDecUpTo3 = 0 ;
  long int v2ndDecOver3 = 0 ;
  long int v2ndCancelOpenSess = 0 ;
  long int v2ndCancelClosedSess = 0 ;
  long int vPrvDefin = 0 ;
  long int vSlowdownDefin = 0 ;
  long int v2ndDecidedOpenSessBankruptcy = 0 ;    // 19.feb.2001
  long int v2ndDecidedClosedSessBankruptcy = 0 ;  // 19.feb.2001
  long int v2ndCanceledBankruptcy = 0 ;  // 19.feb.2001
  long int vPrvDecidedBankruptcy = 0 ;   // 19.feb.2001
  long int vPrvCanceledBankruptcy = 0 ;  // 19.feb.2001
  long int vPrvDecidedBankruptcyClosedS = 0 ;   // 21.sep.2001
  long int vPrvCanceledBankruptcyClosedS = 0 ;  // 21.sep.2001
	long int vPrvDefinClosedS = 0 ;  // 21.sep.2001
	long int vPrv37DefinClosedS = 0 ;  // 14.jun.2002
	long int vPrv37DefinOpenS = 0 ;  // 14.jun.2002

	CharPtr regimeDescr(64);

	RegimeStr(regimeDescr, "Свършени дела");
	wtdl->SetHeader(regimeDescr);
	FinalizedSessions finS (rBMYEMYTCIU.types, begD, endD) ;
	totalPartSteps = finS.FinishedLawsNumber();
	 {
		TSession * pSess = new TSession ;
		TLawsuit law ;

		do
		 {
			Tick(false);
			if(wtdl->WantBreak())
				break;
			if (finS.NextSession (law, (* pSess)))
				FinishedLawsDistribute (law, * pSess, vFinTotal, v2ndDecTotal,
					v2ndDecUpTo3, v2ndDecOver3,
					v2ndCancelOpenSess, v2ndCancelClosedSess,
					vPrvDefin, vSlowdownDefin,
					v2ndDecidedOpenSessBankruptcy,
					v2ndDecidedClosedSessBankruptcy,
					v2ndCanceledBankruptcy,
					vPrvDecidedBankruptcy,
					vPrvCanceledBankruptcy,
					vPrvDecidedBankruptcyClosedS,
					vPrvCanceledBankruptcyClosedS,
					vPrvDefinClosedS,
					vPrv37DefinClosedS,  // 14.jun.2002
					vPrv37DefinOpenS   // 14.jun.2002
				 ) ;
     }
    while (TRCDKey_ (law.key).FilledOK ()) ;

    delete pSess ;
   }
  codeValueArr.Add(new CodeValue(10483, vFinTotal)) ;
  codeValueArr.Add(new CodeValue(10484, v2ndDecTotal)) ;
  codeValueArr.Add(new CodeValue(10485, v2ndDecUpTo3)) ;
  codeValueArr.Add(new CodeValue(10486, v2ndDecOver3)) ;
  codeValueArr.Add(new CodeValue(10487, v2ndCancelOpenSess)) ;
  codeValueArr.Add(new CodeValue(10488, v2ndCancelClosedSess)) ;
  codeValueArr.Add(new CodeValue(10489, vPrvDefin)) ;
  codeValueArr.Add(new CodeValue(10490, vSlowdownDefin)) ;
  codeValueArr.Add(new CodeValue(10521, v2ndDecidedOpenSessBankruptcy)) ;
  codeValueArr.Add(new CodeValue(10522, v2ndDecidedClosedSessBankruptcy)) ;
  codeValueArr.Add(new CodeValue(10523, v2ndCanceledBankruptcy)) ;
  codeValueArr.Add(new CodeValue(10524, vPrvDecidedBankruptcy)) ;
  codeValueArr.Add(new CodeValue(10525, vPrvCanceledBankruptcy)) ;
  codeValueArr.Add(new CodeValue(10548, vPrvDecidedBankruptcyClosedS)) ;
  codeValueArr.Add(new CodeValue(10549, vPrvCanceledBankruptcyClosedS)) ;
  codeValueArr.Add(new CodeValue(10550, vPrvDefinClosedS)) ;
	codeValueArr.Add(new CodeValue(10568, vPrv37DefinClosedS)) ;
	codeValueArr.Add(new CodeValue(10569, vPrv37DefinOpenS)) ;
 }

void TIlievaSummary ::
OpenSessionsDistribute (TSession & sess,
				long int & vFixed, long int & v2ndLeftTotal,
				long int & v2ndLeftForNewEvidences,
				long int & v2ndStopped, long int & v2ndUnmoveable,
				long int & v2ndAbolishProgr,
				long int & v2ndLeftInvalidCall,
				long int & v2ndLeft107,
				long int & v2ndLeft65, long int & v2ndLeft157,
				long int & v2ndLeftOtherCauses,
				long int & v2ndFixed,  // 19.feb.2001
				long int & v2ndFixedBankruptcy,  // 19.feb.2001
				long int & vPrvFixed,   // 20.sep.2001
				long int & vPrvFixedBankruptcy   // 20.sep.2001
			     ) const
 {
  const char lawKind = sess.kind ;  // Boza, I know...

  vFixed ++ ;
// 19.feb.2001, 20.sep.2001 - begin
  if (lawKind == KIND_CIT_2ND_LAW || lawKind == KIND_CIT_PRV_LAW
	|| lawKind == KIND_TRA_2ND_LAW  // 23.jun.2004
	|| lawKind == KIND_TRA_PRV_LAW  // 23.jun.2004
     )
   {
    TLawsuit law ;

    law.key = sess.key ;
    if (law.Get ())
     {
      const bool bankruptcy = bankruptcy_law (law) ;

      if (lawKind == KIND_CIT_2ND_LAW
          || lawKind == KIND_TRA_2ND_LAW  // 23.jun.2004
         )
       {
        if (bankruptcy)
          v2ndFixedBankruptcy ++ ;
        else
          v2ndFixed ++ ;
       }
      else
        if (lawKind == KIND_CIT_PRV_LAW
            || lawKind == KIND_TRA_PRV_LAW  // 23.jun.2004
           )
         {
          if (bankruptcy)
            vPrvFixedBankruptcy ++ ;
          else
            vPrvFixed ++ ;
         }
     }
   }
// 19.feb.2001, 20.sep.2001 - end
  if (strchr (RESULT_LEFTS_OR_STOPPEDS, sess.result) ||
      strchr (RESULT_UNMOVEABLES, sess.result))
   {
    if (lawKind == KIND_CIT_2ND_LAW
        || lawKind == KIND_CIT_PRV_LAW  // 20.sep.2001
        || lawKind == KIND_TRA_2ND_LAW  // 23.jun.2004
        || lawKind == KIND_TRA_PRV_LAW  // 23.jun.2004
       )
     {
      v2ndLeftTotal ++ ;
      switch (sess.result)
       {
        case RESULT_APPEAL_LEFT_NEW_EVIDENCES :
          v2ndLeftForNewEvidences ++ ;
          break ;
        case RESULT_APPEAL_LEFT_INVALID_CALL :
          v2ndLeftInvalidCall ++ ;
          break ;
        case RESULT_APPEAL_LEFT_107 :
          v2ndLeft107 ++ ;
          break ;
        case RESULT_APPEAL_LEFT_65 :
          v2ndLeft65 ++ ;
          break ;
        case RESULT_APPEAL_LEFT_157 :
          v2ndLeft157 ++ ;
          break ;
	  default :
	     {
		if (strchr(RESULT_UNDOS, sess.result))
		  v2ndAbolishProgr ++ ;
		else if (strchr (RESULT_STOPPEDS, sess.result))
              v2ndStopped ++ ;
            else
              if (strchr (RESULT_UNMOVEABLES, sess.result))
                v2ndUnmoveable ++ ;
              else
                v2ndLeftOtherCauses ++ ;
           }
          break ;
       }
     }
   }
 }

bool TIlievaSummary ::
OpenSessionsCriteria (char * const dest, const size_t destSz,
			    const char * const types,
			    const CDate begDate, const CDate endDate)
 {
  if (! (dest && destSz > 1 && types && types [0] &&
         begDate.Empty () == false && endDate.Empty () == false))
    return false ;
  dest [destSz - 1] = '\0' ;
  ostrstream (dest, destSz - 1) <<
    "SELECT L.F_KIND, "
      "S.F_TYPE, S.F_YEAR, S.F_NO, S.F_JUDGE, L.F_SUBJECT, " //19.feb.2001
      "S.F_RESULT FROM T_LAWSUIT L, T_SESSION S WHERE "
	"S.F_DATE BETWEEN " << begDate.Value() << " AND "
				  << endDate.Value() << " AND "  // 15.apr.2003 - end
	"S.F_TYPE IN ("
           << stringToSQLCharSet (types).getTheSet () <<
                   ") AND "
      "S.F_KIND NOT IN ("
        << stringToSQLCharSet (KIND_PSEUDO_SESSIONS).getTheSet () <<
                      ") AND "
      "S.F_KIND NOT IN (" <<
               stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
               << ") "
      "AND "
      "S.F_NO = L.F_NO AND "
      "S.F_YEAR = L.F_YEAR AND "
      "S.F_TYPE = L.F_TYPE AND "
// 25.jan.2001 - begin
      "S.F_RESULT != \'" << RESULT_UNFIXED << "\' AND "
      "S.F_RESULT != \'" << RESULT_ABORTED << "\' AND " //20050322
# if JUDGE_CHKUP_EXCLUDE_RES_FIXED
      "S.F_RESULT != \'" << RESULT_FIXED << "\' AND "
# endif  // of JUDGE_CHKUP_EXCLUDE_RES_FIXED
// 25.jan.2001 - end
      "(L.F_FINISHED IS NULL OR S.F_DATE <= L.F_FINISHED) "
      << ";" << ends ;
  return criteria_finished_ok (dest,
                               "I?????Summary::OpenSessionsCriteria") ;
 }

void TIlievaSummary ::
CountOpenSessions (const long int v2ndStopped_OtherActs,
			 const long int v2ndUnmoveable_OtherActs,
			 const long int v2ndAbolishProgr_OtherActs)
 {
  const size_t crtSz = 1024 ;
  char * const crt = new char [crtSz] ;

  if (OpenSessionsCriteria (crt, crtSz, rBMYEMYTCIU.types,
                            begD, endD))
   {  // -- 0 --
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, "Открити заседания");
		wtdl->SetHeader(regimeDescr);
		TCountRecsQuery  // TQuery  // 14.jun.2002
			q (crt) ;
    TSession * pSess = new TSession ;
    long int vFixed = 0 ;
    long int v2ndLeftTotal = 0 ;
    long int v2ndLeftForNewEvidences = 0 ;
    long int v2ndStopped = 0 ;
    long int v2ndUnmoveable = 0 ;
    long int v2ndAbolishProgr = 0 ;
    long int v2ndLeftInvalidCall = 0 ;
    long int v2ndLeft107 = 0 ;
    long int v2ndLeft65 = 0 ;
    long int v2ndLeft157 = 0 ;
    long int v2ndLeftOtherCauses = 0 ;
    long int v2ndFixed = 0 ;  // 19.feb.2001
    long int v2ndFixedBankruptcy = 0 ;  // 19.feb.2001
    long int vPrvFixed = 0 ;  // 20.sep.2001
    long int vPrvFixedBankruptcy = 0 ;  // 20.sep.2001

		totalPartSteps = q.getRecsCount();
		while (q.Read ())
     {  // -- 1 --
			Tick(false);
			if(wtdl->WantBreak())
				break;
	(* pSess) << q ;
      OpenSessionsDistribute (* pSess, vFixed, v2ndLeftTotal,
					v2ndLeftForNewEvidences, v2ndStopped,
                              v2ndUnmoveable, v2ndAbolishProgr,
                              v2ndLeftInvalidCall, v2ndLeft107,
                              v2ndLeft65, v2ndLeft157,
                              v2ndLeftOtherCauses,
                              v2ndFixed,  // 19.feb.2001
                              v2ndFixedBankruptcy,  // 19.feb.2001
                              vPrvFixed,  // 20.sep.2001
                              vPrvFixedBankruptcy   // 20.sep.2001
                             ) ;
     }  // -- 1 --
    codeValueArr.Add(new CodeValue(10473, vFixed)) ;
    codeValueArr.Add(new CodeValue(10474, v2ndLeftTotal +
                                        v2ndStopped_OtherActs +
                                        v2ndUnmoveable_OtherActs +
                                        v2ndAbolishProgr_OtherActs
                                        )) ;
    codeValueArr.Add(new CodeValue(10475, v2ndLeftForNewEvidences)) ;
    codeValueArr.Add(new CodeValue(10476,
                                 v2ndStopped + v2ndStopped_OtherActs)) ;
    codeValueArr.Add(new CodeValue(10477,
                              v2ndUnmoveable + v2ndUnmoveable_OtherActs)) ;
    codeValueArr.Add(new CodeValue(10478,
                          v2ndAbolishProgr + v2ndAbolishProgr_OtherActs)) ;
    codeValueArr.Add(new CodeValue(10479, v2ndLeftInvalidCall)) ;
    codeValueArr.Add(new CodeValue(10480, v2ndLeft107)) ;
    codeValueArr.Add(new CodeValue(10481, v2ndLeft65)) ;
    codeValueArr.Add(new CodeValue(10482, v2ndLeft157)) ;
    codeValueArr.Add(new CodeValue(10496, v2ndLeftOtherCauses)) ;
    codeValueArr.Add(new CodeValue(10519, v2ndFixed)) ;
    codeValueArr.Add(new CodeValue(10520, v2ndFixedBankruptcy)) ;
    codeValueArr.Add(new CodeValue(10546, vPrvFixed)) ;
    codeValueArr.Add(new CodeValue(10547, vPrvFixedBankruptcy)) ;
    delete pSess ;
   }  // -- 0 --
  delete [] crt ;
 }

void TIlievaSummary ::
OtherActsDistribute (TSession & sess,
			   long int &,  // v2ndStopped_Other,  // 15.mar.2002
			   long int &,  // v2ndUnmoveable_Other,  // 15.mar.2002
			   long int & v2ndAbolishProgr_Other,
			   long int & vDec192_193,
			   long int & vDefin205ClosedSess,
			   long int & vDefin192p4ClosedSess,
			   long int & vTotalSessOtherActs,
			   long int & vDefin192p5ClosedSess,
			   long int & vDefin37,
			   long int & vDecis194p1OpenSess,
			   long int & vDecreesClosedSess,
										 long int & vDefin,
										 long int & vStoppedUnmoveableCS,  // 17.jun.2002
			   long int & vDefin192p5OpenSess   // 24.jun.2002
										 ) const
 {
  if ((strchr (RESULT_STOPPEDS, sess.result) ||
       strchr (RESULT_UNMOVEABLES, sess.result) ||
       strchr (RESULT_UNDOS, sess.result)) == false)
     // Attention! This check appears in the
     // IlievaJudgeCheckup :: OtherSessActsDistribute, too
    vTotalSessOtherActs ++ ;
	if ((strchr (RESULT_STOPPEDS, sess.result) ||  // 12.apr.2002 - begin
			 strchr (RESULT_UNMOVEABLES, sess.result)
			) &&
			 strchr (KIND_CLOSED_SESSIONS, sess.kind)
		 )
		 // Attention! This check appears in the
		 // IlievaJudgeCheckup :: OtherSessActsDistribute, too
	 {
		vTotalSessOtherActs ++ ;
		vStoppedUnmoveableCS ++ ;  // 19.jun.2002
	 }   // 12.apr.2002 - end
	if (sess.result == RESULT_CIT_37)  // 08.feb.2001 - begin
    vDefin37 ++ ;
  if (! strchr (KIND_CLOSED_SESSIONS, sess.kind))
    if (sess.result == RESULT_CIT_DECIS_194_1)
      vDecis194p1OpenSess ++ ;
  if (sess.result == RESULT_DEFINITION)
    vDefin ++ ;  // 08.feb.2001 - end
  if (sess.kind == KIND_OTHER_SESSION)
   {
    if (sess.result == RESULT_CIT_192_193)
			vDec192_193 ++ ;
		else  // 24.jun.2002 - begin
			if (sess.result == RESULT_CIT_192_5)
				vDefin192p5OpenSess ++ ;  // 24.jun.2002 - end
	 }
  else
    if (strchr (KIND_CLOSED_SESSIONS, sess.kind))
     {
      if (sess.result == RESULT_CIT_205)
        vDefin205ClosedSess ++ ;
      else
				if (sess.result == RESULT_CIT_192_4)
					vDefin192p4ClosedSess ++ ;
				else  // 08.feb.2001 - begin
					if (sess.result == RESULT_CIT_192_5)
            vDefin192p5ClosedSess ++ ;
					else
            if (sess.result == RESULT_DECREE)
              vDecreesClosedSess ++ ;  // 08.feb.2001 - end
            else
              if (
//            strchr (RESULT_STOPPEDS, sess.result) ||  // 15.mar.2002
//            strchr (RESULT_UNMOVEABLES, sess.result) ||  // 15.mar.2002
                  strchr(RESULT_UNDOS, sess.result))
               {
                TLawsuit law ;

                law.key = sess.key ;
                if (law.Get () &&
                    (law.kind == KIND_CIT_2ND_LAW
                     || law.kind == KIND_TRA_2ND_LAW  // 23.jun.2004
                    ))
                  if (law.finished.Empty () || sess.date <= law.finished)
                   {
//      if (strchr (RESULT_STOPPEDS, sess.result))  // 15.mar.2002 - begin
//        v2ndStopped_Other ++ ;
//      else
//        if (strchr (RESULT_UNMOVEABLES, sess.result))
//          v2ndUnmoveable_Other ++ ;
//        else  // 15.mar.2002 - end
                        if (strchr(RESULT_UNDOS, sess.result))
                          v2ndAbolishProgr_Other ++ ;
                   }
               }
		 }
		else  // 24.jun.2002 - begin
		 {  // non-clodes, non-other sessions
			if (sess.result == RESULT_CIT_192_5)
				vDefin192p5OpenSess ++ ;
		 }  // 24.jun.2002 - end
 }

bool TIlievaSummary :: OtherActsCriteria (char * const dest,
						     const size_t destSz,
						     const char * const types,
						     const CDate begDate,
						     const CDate endDate)
 {
  if (! (dest && destSz && types && types [0] &&
         (begDate.Empty () == false && endDate.Empty () == false)
        ))
    return false ;
  dest [0] = dest [destSz - 1] = '\0' ;
  ostrstream (dest, destSz - 1) <<
    "SELECT S.F_NO, S.F_YEAR, S.F_TYPE, S.F_KIND, S.F_RESULT, "
			"S.F_DATE, S.F_RETURNED, S.F_JUDGE, "
			"L.F_FINISHED "  // 11.jun.2002
      "FROM T_SESSION S, T_LAWSUIT L WHERE "
	"S.F_DATE BETWEEN " << begDate.Value() << " AND "
                           << endDate.Value() << " AND "  // 15.apr.2003 - end
	"S.F_TYPE IN ("
           << stringToSQLCharSet (types).getTheSet () <<
                   ") AND "
      "S.F_RESULT NOT IN ("
           << stringToSQLCharSet (RESULT_FINALIZEDS).getTheSet () <<
                        ") AND "
      "S.F_NO = L.F_NO AND "
      "S.F_YEAR = L.F_YEAR AND "
      "S.F_TYPE = L.F_TYPE AND "
      "S.F_RESULT != \'" << RESULT_FIXED << "\' AND "
      "S.F_RESULT != \'" << RESULT_UNFIXED << "\' AND "  // 25.jan.2001
      "S.F_RESULT != \'" << RESULT_ABORTED << "\' AND "  //20050322
      "(("
        "S.F_KIND NOT IN (" <<
               stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
                     << ") "
        "AND "
        "S.F_RESULT NOT IN ("
        << stringToSQLCharSet (RESULT_LEFTS_OR_STOPPEDS).getTheSet () <<
                            ","
        << stringToSQLCharSet (RESULT_UNMOVEABLES).getTheSet () <<
                          ")"
        ") OR "
       "("
        "S.F_KIND IN (" <<
               stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
                 << ") AND "
        "S.F_RESULT NOT IN ("
        << stringToSQLCharSet (RESULT_LEFTS).getTheSet () <<
                          ")"
       ")"
      ") "
    << ";" << ends ;
  return criteria_finished_ok (dest, "I?????Summary::OtherActsCriteria") ;
 }

void TIlievaSummary ::
CountOtherActs (const long int vResolutions,
		    long int & v2ndStopped_OtherActs,
		    long int & v2ndUnmoveable_OtherActs,
		    long int & v2ndAbolishProgr_OtherActs)
 {
  const size_t crtSz = 1024 * 2 ;
  char * const crt = new char [crtSz] ;

  if (OtherActsCriteria (crt, crtSz, rBMYEMYTCIU.types, begD, endD))
   {  // -- 0 --
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, "Други съдебни актове");
		wtdl->SetHeader(regimeDescr);
		TCountRecsQuery  // TQuery  // 14.jun.2002
			q (crt) ;
    TSession * pSess = new TSession ;
    long int vDec192_193 = 0 ;
    long int vDefin205ClosedSess = 0 ;
    long int vDefin192p4ClosedSess = 0 ;
    long int vTotalSessOtherActs = 0 ;
    long int vDefin192p5ClosedSess = 0 ;  // 08.feb.2001 - begin
    long int vDefin37 = 0 ;
    long int vDecis194p1OpenSess = 0 ;
    long int vDecreesClosedSess = 0 ;
		long int vDefin = 0 ;  // 08.feb.2001 - end
		long int vStoppedUnmoveableCS = 0 ;  // 19.jun.2002
		long int vDefin192p5OpenSess = 0 ;  // 24.jun.2002

		totalPartSteps = q.getRecsCount();
    while (q.Read ())
     {  // -- 1 --
			Tick(false);
			if(wtdl->WantBreak())
				break;
	(* pSess) << q ;
      OtherActsDistribute (* pSess, v2ndStopped_OtherActs,
				   v2ndUnmoveable_OtherActs,
                           v2ndAbolishProgr_OtherActs,
                           vDec192_193, vDefin205ClosedSess,
                           vDefin192p4ClosedSess, vTotalSessOtherActs,
                           vDefin192p5ClosedSess, vDefin37,
                           vDecis194p1OpenSess, vDecreesClosedSess,
													 vDefin, vStoppedUnmoveableCS,
													 vDefin192p5OpenSess) ;
     }  // -- 1 --
    codeValueArr.Add(new CodeValue(10491,
                          vTotalSessOtherActs + vResolutions)) ;
    codeValueArr.Add(new CodeValue(10492, vDec192_193)) ;
    codeValueArr.Add(new CodeValue(10494, vDefin205ClosedSess)) ;
    codeValueArr.Add(new CodeValue(10495, vDefin192p4ClosedSess)) ;
    codeValueArr.Add(new CodeValue(10510, vDefin192p5ClosedSess)) ;
    codeValueArr.Add(new CodeValue(10511, vDefin37)) ;
    codeValueArr.Add(new CodeValue(10512, vDecis194p1OpenSess)) ;
    codeValueArr.Add(new CodeValue(10513, vDecreesClosedSess)) ;
    codeValueArr.Add(new CodeValue(10514, vDefin)) ;
		codeValueArr.Add(new CodeValue(10570, vStoppedUnmoveableCS)) ;
		codeValueArr.Add(new CodeValue(10587, vDefin192p5OpenSess)) ;
		codeValueArr.Add(new CodeValue(10588, vDefin192p5ClosedSess +
		                                    vDefin192p5OpenSess)) ;

    delete pSess ;
   }  // -- 0 --
  delete [] crt ;
 }

void TIlievaSummary :: Execute ()
 {
	long int vReqWithoutLaw = 0 ;
	long int vResolutions = 0 ;
	long int v2ndStopped_OtherActs = 0 ;
	long int v2ndUnmoveable_OtherActs = 0 ;
	long int v2ndAbolishProgr_OtherActs = 0 ;

	try
	{  // -- 1 --
		const int singleSpan = (barHiVal - barLowVal) / 6;

		endingVal = barLowVal + singleSpan;                 // Gossip service
		wtdl->PleaseWait();
		CountRequests (vReqWithoutLaw, vResolutions) ;
		if(wtdl->WantBreak() == false)
		{
			Tick(true);
			endingVal += singleSpan;
			wtdl->PleaseWait();
			currentPartStep = 0;
			CountNotFinishedBegLaws () ;
			if(wtdl->WantBreak() == false)
			{
				Tick(true);
				endingVal += singleSpan;
				wtdl->PleaseWait();
				currentPartStep = 0;
				CountComeInLaws (vReqWithoutLaw) ;
				if(wtdl->WantBreak() == false)
				{
					Tick(true);
					endingVal += singleSpan;
					wtdl->PleaseWait();
					currentPartStep = 0;
					CountFinishedLaws () ;
					if(wtdl->WantBreak() == false)
					{
						Tick(true);
						endingVal += singleSpan;
						wtdl->PleaseWait();
						currentPartStep = 0;
						CountOtherActs (vResolutions, v2ndStopped_OtherActs,
							v2ndUnmoveable_OtherActs, v2ndAbolishProgr_OtherActs) ;
						if(wtdl->WantBreak() == false)
						{
							Tick(true);
							endingVal = barHiVal;
							wtdl->PleaseWait();
							currentPartStep = 0;
							CountOpenSessions (v2ndStopped_OtherActs,
								v2ndUnmoveable_OtherActs, v2ndAbolishProgr_OtherActs) ;
							if(wtdl->WantBreak() == false)
							{
								Tick(true);
								wtdl->AlmostDone();
							}
						}
					}
				}
			}
		}
	}  // -- 1 --
	CATCH_ANYTHING
 }

TIlievaSummary :: TIlievaSummary (const begMYEndMYTypeCIU & bMYEMYTCIU) :
			 rBMYEMYTCIU (bMYEMYTCIU),
									 begD (1, rBMYEMYTCIU.begMonth,
				 rBMYEMYTCIU.begYear),
   endD (lastDayOfThisMonth (CDate (1, rBMYEMYTCIU.endMonth,
						  rBMYEMYTCIU.endYear)))
 {
	MakeFormatFN () ;
 }

TIlievaSummary :: ~TIlievaSummary ()
 {
 }

void TIlievaSummary :: PrintIlievaSummary ()
 {
	Generate () ;  // 14.jun.2002

	collCompBegDEndDBegN cCBDEDBN ;

	cCBDEDBN << rBMYEMYTCIU ;
	writeTBSummary (formatFN, & cCBDEDBN, wtdl->Parent, & codeValueArr,
									NULL, INFORMATION_OF_COURT_WORK  // 14.feb.2002
								 ) ;
 }

// ============================ End of ==================================
// ========================= IlievaSummary ==============================

/////////////////////////////////////////////////////////////////////////

// ========================== IlievaJCData ==============================
// ============================= Begin ==================================
size_t IlievaJCData :: Size () const
 {
  const char * b = (const char *) (& IlievaFin2ndTotal) ;
  const char * e = (const char *) (& IlievaOth192p4) +
                   sizeof (IlievaOth192p4) ;
  return (e - b) ;
 }  // IlievaJCData :: Size

void IlievaJCData :: ClearData ()
 {
  setmem (& IlievaFin2ndTotal, Size (), '\0') ;
 }  // IlievaJCData :: ClearData

bool IlievaJCData :: operator == (const IlievaJCData & otherIJCD) const
 {
  return ! memcmp (& IlievaFin2ndTotal, & (otherIJCD.IlievaFin2ndTotal),
                   Size ()) ;
 }  // IlievaJCData :: operator ==

bool IlievaJCData :: IsEmpty () const
 {
  IlievaJCData rL1 ;

  return ((* this) == rL1) ;
 }  // IlievaJCData :: IsEmpty

IlievaJCData & IlievaJCData ::
  operator += (const IlievaJCData & otherIJCD)
 {
  IlievaFin2ndTotal += otherIJCD.IlievaFin2ndTotal ;
  IlievaFin2ndCancelClosed += otherIJCD.IlievaFin2ndCancelClosed ;
  IlievaFin2ndCancelOpen += otherIJCD.IlievaFin2ndCancelOpen ;
  IlievaFin2nsDecUpTo3Mon += otherIJCD.IlievaFin2nsDecUpTo3Mon ;
  IlievaFin2ndDecOver3Mon += otherIJCD.IlievaFin2ndDecOver3Mon ;
  IlievaFinPrvTotal += otherIJCD.IlievaFinPrvTotal ;

  IlievaFinPrvPrvCompl += otherIJCD.IlievaFinPrvPrvCompl ;
  IlievaFinPrvPrvCompl_Decis  += otherIJCD.IlievaFinPrvPrvCompl_Decis ;  // 20060417
  IlievaFinPrvPrvCompl_Ceased += otherIJCD.IlievaFinPrvPrvCompl_Ceased ; // 20060417

  IlievaFinPrvSlowdown += otherIJCD.IlievaFinPrvSlowdown ;
  IlievaFinPrvSlowdown_Decis  += otherIJCD.IlievaFinPrvSlowdown_Decis ;  // 20060417
  IlievaFinPrvSlowdown_Ceased += otherIJCD.IlievaFinPrvSlowdown_Ceased ; // 20060417

  IlievaOthTotal += otherIJCD.IlievaOthTotal ;
  IlievaOth205Closed += otherIJCD.IlievaOth205Closed ;
  IlievaOthReqResol += otherIJCD.IlievaOthReqResol ;

  inRegResol_AC += otherIJCD.inRegResol_AC ;  //20050804

  Ilieva192_193 += otherIJCD.Ilieva192_193 ;
// 07.feb.2001 - begin
  IlievaBankruptcyFinTotal += otherIJCD.IlievaBankruptcyFinTotal ;
  IlievaBankruptcyDec2nd_OS += otherIJCD.IlievaBankruptcyDec2nd_OS ;
  IlievaBankruptcyDec2nd_CS += otherIJCD.IlievaBankruptcyDec2nd_CS ;
  IlievaBankruptcyDecPrv += otherIJCD.IlievaBankruptcyDecPrv ;
  IlievaOthOpenSessTotal += otherIJCD.IlievaOthOpenSessTotal ;
// 07.feb.2001 - end
// 20.feb.2001 - begin
  IlievaBankruptcyFixed_OS += otherIJCD.IlievaBankruptcyFixed_OS ;
  IlievaBankruptcyDecided_OS += otherIJCD.IlievaBankruptcyDecided_OS ;
  IlievaBankruptcyCanceled_OS += otherIJCD.IlievaBankruptcyCanceled_OS ;
  IlievaBankruptcyCancel2nd += otherIJCD.IlievaBankruptcyCancel2nd ;
  IlievaBankruptcyCancelPrv += otherIJCD.IlievaBankruptcyCancelPrv ;
  IlievaBankruptcyFin_OS += otherIJCD.IlievaBankruptcyFin_OS ;
	IlievaBankruptcyLeft_OS += otherIJCD.IlievaBankruptcyLeft_OS ;
// 20.feb.2001 - end
// 11.jun.2002 - begin
	IlievaFinPrvPetitions += otherIJCD.IlievaFinPrvPetitions ;
	IlievaOthPendingDecree += otherIJCD.IlievaOthPendingDecree ;
	IlievaOthPendingDefinition += otherIJCD.IlievaOthPendingDefinition ;
	IlievaOthAftFinDecisionOS += otherIJCD.IlievaOthAftFinDecisionOS ;
	IlievaOthAftFinDefinitionOS += otherIJCD.IlievaOthAftFinDefinitionOS ;
	IlievaOthAftFinDefinitionCS += otherIJCD.IlievaOthAftFinDefinitionCS ;
	IlievaOthAftFinDecree += otherIJCD.IlievaOthAftFinDecree ;
	IlievaOthPending205 += otherIJCD.IlievaOthPending205 ;
// 11.jun.2002 - end
	IlievaOthStopedUnmoveableCS +=             // 19.jun.2002
		otherIJCD.IlievaOthStopedUnmoveableCS ;  // 19.jun.2002
	IlievaOth192p4 += otherIJCD.IlievaOth192p4 ;
  return (* this) ;
 }  // IlievaJCData & IlievaJCData :: operator +=

// ============================ End of ==================================
// ========================== IlievaJCData ==============================

/////////////////////////////////////////////////////////////////////////

// ===================== IlievaJudgeCheckupData =========================
// ============================= Begin ==================================
void IlievaJudgeCheckupData :: clearData ()
 {
  judgeCheckupData :: clearData () ;
  IlievaData.ClearData () ;
 }  // IlievaJudgeCheckupData :: clearData

bool IlievaJudgeCheckupData :: countingDataEmpty () const
 {
  return (judgeCheckupData :: countingDataEmpty () &&
          IlievaData.IsEmpty ()
	   ) ;
 }  // IlievaJudgeCheckupData :: countingDataEmpty

void
IlievaJudgeCheckupData :: operator += (const judgeCheckupData & other)
 {
  judgeCheckupData :: operator += (other) ;

  const IlievaJudgeCheckupData * otherIlieva =
    dynamic_cast <const IlievaJudgeCheckupData *> (& other) ;

  if (otherIlieva)
   {
    IlievaData += otherIlieva -> IlievaData ;
   }
 }  // IlievaJudgeCheckupData :: operator

// ============================ End of ==================================
// ===================== IlievaJudgeCheckupData =========================

/////////////////////////////////////////////////////////////////////////

// ==================== IlievaJudgeCheckupColumns =======================
// ============================= Begin ==================================
void IlievaJudgeCheckupColumns ::
       MakeColumns (const IlievaJudgeCheckupData & jCD)
 {
  const IlievaJCData & IJCD = jCD.IlievaData ;

  addNmbToClmn (columns [iC_L_IlievaFin2ndTotal],
                IJCD.IlievaFin2ndTotal) ;
  addNmbToClmn (columns [iC_L_IlievaFin2ndCancelClosed],
                IJCD.IlievaFin2ndCancelClosed) ;
  addNmbToClmn (columns [iC_L_IlievaFin2ndCancelOpen],
                IJCD.IlievaFin2ndCancelOpen) ;
  addNmbToClmn (columns [iC_L_IlievaFin2nsDecUpTo3Mon],
                IJCD.IlievaFin2nsDecUpTo3Mon) ;
  addNmbToClmn (columns [iC_L_IlievaFin2ndDecOver3Mon],
                IJCD.IlievaFin2ndDecOver3Mon) ;
  addNmbToClmn (columns [iC_L_IlievaFinPrvTotal],
                IJCD.IlievaFinPrvTotal) ;
  addNmbToClmn (columns [iC_L_IlievaFinPrvPrvCompl],
                IJCD.IlievaFinPrvPrvCompl) ;
  // 20060417 ->
  addNmbToClmn (columns [iC_L_IlievaFinPrvPrvCompl_Decis],
                IJCD.IlievaFinPrvPrvCompl_Decis) ;
  addNmbToClmn (columns [iC_L_IlievaFinPrvPrvCompl_Ceased],
                IJCD.IlievaFinPrvPrvCompl_Ceased) ;
  // 20060417 <-
  addNmbToClmn (columns [iC_L_IlievaFinPrvSlowdown],
                IJCD.IlievaFinPrvSlowdown) ;
  // 20060417 ->
  addNmbToClmn (columns [iC_L_IlievaFinPrvSlowdown_Decis],
                IJCD.IlievaFinPrvSlowdown_Decis) ;
  addNmbToClmn (columns [iC_L_IlievaFinPrvSlowdown_Ceased],
                IJCD.IlievaFinPrvSlowdown_Ceased) ;
  // 20060417 <-

  addNmbToClmn (columns [iC_B_IlievaOthTotal], IJCD.IlievaOthTotal) ;
  addNmbToClmn (columns [iC_S_IlievaOth205Closed],
                IJCD.IlievaOth205Closed) ;
  addNmbToClmn (columns [iC_R_IlievaOthReqResol],
                IJCD.IlievaOthReqResol) ;

// 20050804 ->
  addNmbToClmn (columns [iC_Oth_InReg_Resol_AC],
                IJCD.inRegResol_AC) ;
// 20050804 <-

  addNmbToClmn (columns [iC_S_Ilieva192_193], IJCD.Ilieva192_193) ;
// 07.feb.2001 - begin
  addNmbToClmn (columns [iC_L_IlievaBankruptcyFinTotal],
                IJCD.IlievaBankruptcyFinTotal) ;
  addNmbToClmn (columns [iC_L_IlievaBankruptcyDec2nd_OS],
                IJCD.IlievaBankruptcyDec2nd_OS) ;
  addNmbToClmn (columns [iC_L_IlievaBankruptcyDec2nd_CS],
                IJCD.IlievaBankruptcyDec2nd_CS) ;
  addNmbToClmn (columns [iC_L_IlievaBankruptcyDecPrv],
                IJCD.IlievaBankruptcyDecPrv) ;
  addNmbToClmn (columns [iC_S_IlievaOthOpenSessTotal],
                IJCD.IlievaOthOpenSessTotal) ;
// 07.feb.2001 - end
// 20.feb.2001 - begin
  addNmbToClmn (columns [iC_S_IlievaBankruptcyFixed_OS],
                IJCD.IlievaBankruptcyFixed_OS) ;
  addNmbToClmn (columns [iC_L_IlievaBankruptcyDecided_OS],
                IJCD.IlievaBankruptcyDecided_OS) ;
  addNmbToClmn (columns [iC_L_IlievaBankruptcyCanceled_OS],
                IJCD.IlievaBankruptcyCanceled_OS) ;
  addNmbToClmn (columns [iC_L_IlievaBankruptcyCancel2nd],
                IJCD.IlievaBankruptcyCancel2nd) ;
  addNmbToClmn (columns [iC_L_IlievaBankruptcyCancelPrv],
                IJCD.IlievaBankruptcyCancelPrv) ;
  addNmbToClmn (columns [iC_L_IlievaBankruptcyFin_OS],
                IJCD.IlievaBankruptcyFin_OS) ;
	addNmbToClmn (columns [iC_S_IlievaBankruptcyLeft_OS],
								IJCD.IlievaBankruptcyLeft_OS) ;
// 20.feb.2001 - end
// 11.jun.2002 - begin
	addNmbToClmn (columns [iC_L_IlievaFinPrvPetitions],
								IJCD.IlievaFinPrvPetitions) ;
	addNmbToClmn (columns [iC_S_IlievaOthPendingDecree],
								IJCD.IlievaOthPendingDecree) ;
	addNmbToClmn (columns [iC_S_IlievaOthPendingDefinition],
								IJCD.IlievaOthPendingDefinition) ;
	addNmbToClmn (columns [iC_S_IlievaOthAftFinDecisionOS],
								IJCD.IlievaOthAftFinDecisionOS) ;
	addNmbToClmn (columns [iC_S_IlievaOthAftFinDefinitionOS],
								IJCD.IlievaOthAftFinDefinitionOS) ;
	addNmbToClmn (columns [iC_S_IlievaOthAftFinDefinitionCS],
								IJCD.IlievaOthAftFinDefinitionCS) ;
	addNmbToClmn (columns [iC_S_IlievaOthAftFinDecree],
								IJCD.IlievaOthAftFinDecree) ;
	addNmbToClmn (columns [iC_S_IlievaOthPending205],
								IJCD.IlievaOthPending205) ;
// 11.jun.2002 - end
	addNmbToClmn (columns [iC_S_IlievaOthStopedUnmoveableCS],  // 19.jun.2002
								IJCD.IlievaOthStopedUnmoveableCS) ;          // 19.jun.2002
	addNmbToClmn (columns [iC_S_IlievaOth192p4], IJCD.IlievaOth192p4) ;
 }  // IlievaJudgeCheckupColumns :: MakeColumns

IlievaJudgeCheckupColumns ::
IlievaJudgeCheckupColumns (const char * const formatFN, TWindow * parent,
                           const IlievaJudgeCheckupData & jCD,
                           const CDate * const pDateForMonthColumn,
                           const int serialNo  // 08.jan.2001
                          ) :
              judgeCheckupColumns (formatFN, parent, jCD,
                                   pDateForMonthColumn, serialNo)
 {
  MakeColumns (jCD) ;
  defineMaxRows () ;
 }  // IlievaJudgeCheckupColumns :: IlievaJudgeCheckupColumns

// ============================ End of ==================================
// ==================== IlievaJudgeCheckupColumns =======================

/////////////////////////////////////////////////////////////////////////

void TIlievaJudgeCheckupThread::makeBookFormatFN()
 {
  const char t = strlen(bMYEMYTCIU.types) == 1 ?
    bMYEMYTCIU.types[0] : TYPESIGN_GENERAL_REPORT;  // 23.jun.2004

  scSprintf (sizeof (formatFN), formatFN,
		 trueJudgeCheckup ?
		 "text\\i_jc%c%s" :  // "text\\i_jc%s" :  // 23.jun.2004    Справка по съдии
		 "text\\tj%ccol2%s",  // "text\\tjccol2%s",  // 23.jun.2004 Срочна книга - рекапитулации
		 t,  // 23.jun.2004
		 DEFAULT_FORMAT_FILE_EXTENSION) ;
 }

judgeCheckupData * TIlievaJudgeCheckupThread::newDataItem() const
 {
  return new IlievaJudgeCheckupData ;
 }

judgeCheckupOutFile * TIlievaJudgeCheckupThread::newOutFile() const
 {
  return new IlievaJudgeCheckupOutFile (wtdl->Parent, formatFN) ;
 }

judgeCheckupColumns * TIlievaJudgeCheckupThread::newJudgeCheckupColumns (const char * const formatFN,
	const judgeCheckupData & jCD, const CDate * const pDateForMonthColumn, const int serialNo) const
 {
  const IlievaJudgeCheckupData * pIJCD =
    dynamic_cast <const IlievaJudgeCheckupData *> (& jCD) ;

  if (pIJCD)
   {
    return
	new IlievaJudgeCheckupColumns (formatFN, wtdl->Parent, (* pIJCD),
						 pDateForMonthColumn, serialNo) ;
   }
  else
   {
    fatal ("I?????JudgeCheckup::newJudgeCheckupColumns: "
	     "unexpected type of the jCD parameter") ;
    return NULL ;  // This will never be executed
   }
 }

void TIlievaJudgeCheckupThread::IncrementValues_Judge(const char * const judgeUcn, unsigned int dataCiph)
 {
  if (! ucnPresent (judgeUcn))
    return ;
  addJCDToArrayIfNecessary (judgeUcn) ;
   {  // -- 0b --
    const long int iV = 1 ;
    bool found = false ;

    for (int i = 0 ; ! found && i < pArr -> Count () ;
         i ++)
     {  // -- 1 --
      IlievaJudgeCheckupData * IJCD_Ptr =
        dynamic_cast <IlievaJudgeCheckupData *> ((* pArr) [i]) ;

      if (! IJCD_Ptr)
        continue ;
      if (strcmp (IJCD_Ptr -> judgeUCN, judgeUcn) == 0)
       {  // -- 1a --
        found = true ;
        IlievaJCData & IJCD = IJCD_Ptr -> IlievaData ;

        switch (dataCiph)
         {  // -- 4 --
          case iC_L_IlievaFin2ndTotal :
            IJCD.IlievaFin2ndTotal += iV ;
            break ;
          case iC_L_IlievaFin2ndCancelClosed :
            IJCD.IlievaFin2ndCancelClosed += iV ;
            break ;
          case iC_L_IlievaFin2ndCancelOpen :
            IJCD.IlievaFin2ndCancelOpen += iV ;
            break ;
          case iC_L_IlievaFin2nsDecUpTo3Mon :
            IJCD.IlievaFin2nsDecUpTo3Mon += iV ;
            break ;
          case iC_L_IlievaFin2ndDecOver3Mon :
            IJCD.IlievaFin2ndDecOver3Mon += iV ;
            break ;
          case iC_L_IlievaFinPrvTotal :
            IJCD.IlievaFinPrvTotal += iV ;
            break ;
          case iC_L_IlievaFinPrvPrvCompl :
            IJCD.IlievaFinPrvPrvCompl += iV ;
            break ;
          // 20060417 ->
          case iC_L_IlievaFinPrvPrvCompl_Decis :
            IJCD.IlievaFinPrvPrvCompl_Decis += iV ;
            break ;
          case iC_L_IlievaFinPrvPrvCompl_Ceased :
            IJCD.IlievaFinPrvPrvCompl_Ceased += iV ;
            break ;
          // 20060417 <-
          case iC_L_IlievaFinPrvSlowdown :
            IJCD.IlievaFinPrvSlowdown += iV ;
            break ;
          // 20060417 ->
          case iC_L_IlievaFinPrvSlowdown_Decis :
            IJCD.IlievaFinPrvSlowdown_Decis += iV ;
            break ;
          case iC_L_IlievaFinPrvSlowdown_Ceased :
            IJCD.IlievaFinPrvSlowdown_Ceased += iV ;
            break ;
          // 20060417 <-

          case iC_B_IlievaOthTotal :
            IJCD.IlievaOthTotal += iV ;
            break ;
          case iC_S_IlievaOth205Closed :
            IJCD.IlievaOth205Closed += iV ;
            break ;
          case iC_R_IlievaOthReqResol :
            IJCD.IlievaOthReqResol += iV ;
            break ;
// 20050804 ->
          case iC_Oth_InReg_Resol_AC :
            IJCD.inRegResol_AC += iV ;
            break ;
// 20050804 <-
          case iC_S_Ilieva192_193 :
            IJCD.Ilieva192_193 += iV ;
            break ;
// 07.feb.2001 - begin
          case iC_L_IlievaBankruptcyFinTotal :
            IJCD.IlievaBankruptcyFinTotal += iV ;
            break ;
          case iC_L_IlievaBankruptcyDec2nd_OS :
            IJCD.IlievaBankruptcyDec2nd_OS += iV ;
            break ;
          case iC_L_IlievaBankruptcyDec2nd_CS :
            IJCD.IlievaBankruptcyDec2nd_CS += iV ;
            break ;
          case iC_L_IlievaBankruptcyDecPrv :
            IJCD.IlievaBankruptcyDecPrv += iV ;
            break ;
          case iC_S_IlievaOthOpenSessTotal :
            IJCD.IlievaOthOpenSessTotal += iV ;
            break ;
// 07.feb.2001 - end
// 20.feb.2001 - begin
          case iC_S_IlievaBankruptcyFixed_OS :
            IJCD.IlievaBankruptcyFixed_OS += iV ;
            break ;
          case iC_L_IlievaBankruptcyDecided_OS :
            IJCD.IlievaBankruptcyDecided_OS += iV ;
            break ;
          case iC_L_IlievaBankruptcyCanceled_OS :
            IJCD.IlievaBankruptcyCanceled_OS += iV ;
            break ;
          case iC_L_IlievaBankruptcyCancel2nd :
            IJCD.IlievaBankruptcyCancel2nd += iV ;
            break ;
					case iC_L_IlievaBankruptcyCancelPrv :
						IJCD.IlievaBankruptcyCancelPrv += iV ;
						break ;
					case iC_L_IlievaBankruptcyFin_OS :
						IJCD.IlievaBankruptcyFin_OS += iV ;
						break ;
					case iC_S_IlievaBankruptcyLeft_OS :
						IJCD.IlievaBankruptcyLeft_OS += iV ;
						break ;
// 20.feb.2001 - end
// 11.jun.2002 - begin
					case iC_L_IlievaFinPrvPetitions :
						IJCD.IlievaFinPrvPetitions += iV ;
						break ;
					case iC_S_IlievaOthPendingDecree :
						IJCD.IlievaOthPendingDecree += iV ;
						break ;
					case iC_S_IlievaOthPendingDefinition :
						IJCD.IlievaOthPendingDefinition += iV ;
						break ;
					case iC_S_IlievaOthAftFinDecisionOS :
						IJCD.IlievaOthAftFinDecisionOS += iV ;
						break ;
					case iC_S_IlievaOthAftFinDefinitionOS :
						IJCD.IlievaOthAftFinDefinitionOS += iV ;
						break ;
					case iC_S_IlievaOthAftFinDefinitionCS :
						IJCD.IlievaOthAftFinDefinitionCS += iV ;
						break ;
					case iC_S_IlievaOthAftFinDecree :
						IJCD.IlievaOthAftFinDecree += iV ;
						break ;
					case iC_S_IlievaOthPending205 :
						IJCD.IlievaOthPending205 += iV ;
						break ;
// 11.jun.2002 - end
					case iC_S_IlievaOthStopedUnmoveableCS :  // 19.jun.2002 - begin
					  IJCD.IlievaOthStopedUnmoveableCS += iV ;
					  break ;  // 19.jun.2002 - end
					case iC_S_IlievaOth192p4 :
						IJCD.IlievaOth192p4 += iV ;
						break ;
	   }  // -- 4 --  switch
	 }  // -- 1a --
     }  // -- 1 --  for
   }  // -- 0b --
 }

void TIlievaJudgeCheckupThread::RegimeStr(CharPtr& dest, const char* const regime)
{  //11.jun.2002
	if(dest.Size() > 0)
	{
		{
			ostrstream oss(dest, dest.Size ());

			oss << (trueJudgeCheckup ? "Справка" : "Рекапитулация");
			oss << " по съдии";
			if(regime && regime[0])
				oss << " - " << regime;
			oss << ends;
		}
		dest[dest.Size() - 1] = '\0';
	}
}

void TIlievaJudgeCheckupThread::FinishedLawsDistribute (TLawsuit & law, TSession & sess)
 {
  const char * const judgeUcn = sess.judge ;

// 07.feb.2001 - begin
  if (bankruptcy_law (law))
   {  // -- bankruptcy --
    IncrementValues_Judge (judgeUcn, iC_L_IlievaBankruptcyFinTotal) ;

    const bool openSess = ! strchr (KIND_CLOSED_SESSIONS, sess.kind) ;

    if (openSess)
      IncrementValues_Judge (judgeUcn, iC_L_IlievaBankruptcyFin_OS) ;

    const bool decided = sessionDecidesTheLaw (sess, & law, openSess) ;
    const bool canceled = sessionCancelsTheLaw (sess, & law, openSess) ;

    if (decided == canceled)
     {
      unknown_finished_law_status (law.key) ;
      return ;
     }
    if (openSess)
      if (decided)
        IncrementValues_Judge (judgeUcn,
                               iC_L_IlievaBankruptcyDecided_OS) ;
      else
        IncrementValues_Judge (judgeUcn,
                               iC_L_IlievaBankruptcyCanceled_OS) ;
    if (law.kind == KIND_CIT_2ND_LAW
        || law.kind == KIND_TRA_2ND_LAW  // 23.jun.2004
       )
     {
      if (decided)
        if (openSess)
          IncrementValues_Judge (judgeUcn,
                                 iC_L_IlievaBankruptcyDec2nd_OS) ;
        else
          IncrementValues_Judge (judgeUcn,
                                 iC_L_IlievaBankruptcyDec2nd_CS) ;
      else
        IncrementValues_Judge (judgeUcn,
                               iC_L_IlievaBankruptcyCancel2nd) ;
     }
    else
      if (law.kind == KIND_CIT_PRV_LAW
          || law.kind == KIND_TRA_PRV_LAW  // 23.jun.2004
         )
       {
        if (decided)
          IncrementValues_Judge (judgeUcn,
                                 iC_L_IlievaBankruptcyDecPrv) ;
        else
          IncrementValues_Judge (judgeUcn,
                                 iC_L_IlievaBankruptcyCancelPrv) ;
       }
   }  // -- bankruptcy --
  else
// 07.feb.2001 - end
    if (law.kind == KIND_CIT_2ND_LAW
        || law.kind == KIND_TRA_2ND_LAW  // 23.jun.2004
       )
     {  // -- 0 --
      IncrementValues_Judge (judgeUcn, iC_L_IlievaFin2ndTotal) ;

      if (sessionDecidesTheLaw (sess, & law, true))
       {  // -- 1 --
        if (strchr (AGE_UP_TO_THREES, law.age))
          IncrementValues_Judge (judgeUcn, iC_L_IlievaFin2nsDecUpTo3Mon) ;
        else
          if (strchr (AGE_MORE_THAN_THREES_ALL, law.age))
            IncrementValues_Judge (judgeUcn, iC_L_IlievaFin2ndDecOver3Mon) ;
       }  // -- 1 --
      else
       {  // -- 2 --
        if (strchr (KIND_CLOSED_SESSIONS, sess.kind))
         {
          if (sessionCancelsTheLaw (sess, & law, false))
            IncrementValues_Judge (judgeUcn,
                                   iC_L_IlievaFin2ndCancelClosed) ;
         }
        else
         {
          if (sessionCancelsTheLaw (sess, & law, true))
            IncrementValues_Judge (judgeUcn,
                                   iC_L_IlievaFin2ndCancelOpen) ;
         }
       }  // -- 2 --
     }  // -- 0 --
    else
      if (law.kind == KIND_CIT_PRV_LAW
          || law.kind == KIND_TRA_PRV_LAW  // 23.jun.2004
         )
       {  // -- 3 --
        IncrementValues_Judge (judgeUcn, iC_L_IlievaFinPrvTotal) ;

				if (strchr (KIND_PETITIONS, law.sourceKind))  // 11.jun.2002-begin
					IncrementValues_Judge (judgeUcn, iC_L_IlievaFinPrvPetitions) ;
				else  // 11.jun.2002 - end
        {
           const bool openSess = ! strchr (KIND_CLOSED_SESSIONS, sess.kind) ;  // 20060417
           const bool decided = sessionDecidesTheLaw (sess, & law, openSess) ; // 20060417
           const bool canceled = sessionCancelsTheLaw (sess, & law, openSess) ;// 20060417

					if (strchr (KIND_PRIVATE_COMPLAINTS, law.sourceKind))
          {
						 IncrementValues_Judge (judgeUcn, iC_L_IlievaFinPrvPrvCompl) ;
             if (decided) IncrementValues_Judge (judgeUcn, iC_L_IlievaFinPrvPrvCompl_Decis) ; // 20060417
             if (canceled)IncrementValues_Judge (judgeUcn, iC_L_IlievaFinPrvPrvCompl_Ceased) ; // 20060417
          }
					else
          {
						 if (strchr (KIND_SLOWDOWN_COMPLAINTS, law.sourceKind))
             {
	  				  	IncrementValues_Judge (judgeUcn, iC_L_IlievaFinPrvSlowdown) ; 
                if (decided) IncrementValues_Judge (judgeUcn, iC_L_IlievaFinPrvSlowdown_Decis) ; // 20060417
                if (canceled)IncrementValues_Judge (judgeUcn, iC_L_IlievaFinPrvSlowdown_Ceased) ;// 20060417
             }
          }
        }
			 }  // -- 3 --
 }

void TIlievaJudgeCheckupThread::CountFinishedLaws(const CDate begDate, const CDate endDate)
 {
	CharPtr regimeDescr(64);

	RegimeStr(regimeDescr, "Свършени дела");
	wtdl->SetHeader(regimeDescr);
	FinalizedSessions finS (bMYEMYTCIU.types, begDate, endDate) ;
	totalPartSteps = finS.FinishedLawsNumber();
	 {
		TSession * pSess = new TSession ;
		TLawsuit law ;

		do
		 {
			Tick(false);
			if(wtdl->WantBreak())
				break;
			if (finS.NextSession (law, (* pSess)))
	  FinishedLawsDistribute (law, * pSess) ;
     }
		while (TRCDKey_ (law.key).FilledOK ()) ;
    delete pSess ;
   }
 }

bool TIlievaJudgeCheckupThread::PendingCondition(const TLawsuit & law, const TSession & sess)
 {
	bool pending = law.finished.Empty () ;

	if (pending == false)
	 {
		const CDate mainDate =
			strchr (KIND_CLOSED_SESSIONS, sess.kind) ? sess.returned : sess.date;

		if (mainDate < law.finished
				|| mainDate == law.finished  // I'm not sure about this.
			 )
	pending = true ;
	 }
	return pending ;
 }

void TIlievaJudgeCheckupThread::OtherSessActsDistribute (const TLawsuit & law, TSession & sess)
 {
	const bool pending = PendingCondition (law, sess) ;
	const bool closed = strchr (KIND_CLOSED_SESSIONS, sess.kind) ;

	if ((strchr (RESULT_STOPPEDS, sess.result) ||
			 strchr (RESULT_UNMOVEABLES, sess.result) ||
			 strchr(RESULT_UNDOS, sess.result)) == false)  // 05.feb.2001
		 // Attention! This check appears in the
		 // IlievaSummary :: OtherActsDistribute, too
	 {
		IncrementValues_Judge (sess.judge, iC_B_IlievaOthTotal) ;
		if (closed == false)
				IncrementValues_Judge (sess.judge, iC_S_IlievaOthOpenSessTotal) ;
	 }
	if ((strchr (RESULT_STOPPEDS, sess.result) ||  // 12.apr.2002 - begin
			 strchr (RESULT_UNMOVEABLES, sess.result)
			) &&
			 closed
		 )
		 // Attention! This check appears in the
		 // IlievaSummary :: OtherSessActsDistribute, too
	 {
		IncrementValues_Judge (sess.judge, iC_B_IlievaOthTotal) ;
		IncrementValues_Judge (sess.judge,  // 19.jun.2002 - begin
			iC_S_IlievaOthStopedUnmoveableCS) ;  // 19.jun.2002 - end
	 }  // 12.apr.2002 - end
	if (pending)  // 11.jun.2002 - begin
	 {
//		if (closed)  // Specially commented --> more general condition
		 {
			switch (sess.result)
			 {
				case RESULT_CIT_205 :
					if (strchr (KIND_CLOSED_SESSIONS, sess.kind))
						IncrementValues_Judge (sess.judge, iC_S_IlievaOthPending205) ;
					break ;
				case RESULT_DECREE :
				case RESULT_DECREE_1 :
					IncrementValues_Judge (sess.judge, iC_S_IlievaOthPendingDecree) ;
					break ;
				case RESULT_DEFINITION :
					IncrementValues_Judge (sess.judge,
																 iC_S_IlievaOthPendingDefinition) ;
					break ;
			 }
		 }
	 }
	else
	 {
		switch (sess.result)
		 {
			case RESULT_CIT_192_193 :
			case RESULT_CIT_DECIS_194_1 :
				if (closed == false)
					IncrementValues_Judge (sess.judge,
																 iC_S_IlievaOthAftFinDecisionOS) ;
				break ;
			case RESULT_CIT_192_5 :
				if (closed)
					IncrementValues_Judge (sess.judge,
																 iC_S_IlievaOthAftFinDefinitionCS) ;
				else
					IncrementValues_Judge (sess.judge,
																 iC_S_IlievaOthAftFinDefinitionOS) ;
				break ;
			case RESULT_CIT_37 :
				if (closed == false)
					IncrementValues_Judge (sess.judge,
																 iC_S_IlievaOthAftFinDefinitionOS) ;
				break ;
			case RESULT_CIT_192_4 :
			case RESULT_DEFINITION :
				if (closed)
					IncrementValues_Judge (sess.judge,
																 iC_S_IlievaOthAftFinDefinitionCS) ;
				break ;
			case RESULT_DECREE :
				IncrementValues_Judge (sess.judge, iC_S_IlievaOthAftFinDecree) ;
				break ;
		 }
	 }  // 11.jun.2002 - end
	switch (sess.result)
	 {
		case RESULT_CIT_205 :
			if (strchr (KIND_CLOSED_SESSIONS, sess.kind))
				IncrementValues_Judge (sess.judge, iC_S_IlievaOth205Closed) ;
			break ;
		case RESULT_CIT_192_193 :
			IncrementValues_Judge (sess.judge, iC_S_Ilieva192_193) ;
			break ;
		case RESULT_CIT_192_4 :
			IncrementValues_Judge (sess.judge, iC_S_IlievaOth192p4) ;
			break ;
	 }
 }

void TIlievaJudgeCheckupThread::CountOtherActs (const CDate begDate, const CDate endDate)
 {
  const size_t crtSz = 1024 * 2 ;
  char * const crt = new char [crtSz] ;

  if (TIlievaSummary :: OtherActsCriteria (crt, crtSz, bMYEMYTCIU.types, begDate, endDate))
   {  // -- 0 --
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, "Други съдебни актове");
		wtdl->SetHeader(regimeDescr);
		TCountRecsQuery q (crt) ;
		SessPtr pSess ;
		LawPtr pLaw ;  // 11.jun.2002 - begin
		TDummyGroup finGrp ;
		CDate fin ;

		totalPartSteps = q.getRecsCount();
		finGrp.Add(new TDate ("F_FINISHED", & fin, false)) ; // 11.jun.2002 - end
		while (q.Read ())
		 {  // -- 1 --
			Tick(false);
			if(wtdl->WantBreak())
				break;
			pSess.Ref () << q ;
			finGrp << q ;  // 11.jun.2002 - begin
			pLaw.Ref ().finished = fin ;
			pLaw.Ref ().key = pSess.Ref ().key ;  // 11.jun.2002 - end
			OtherSessActsDistribute (pLaw.Ref (), pSess.Ref ()) ;
		 }  // -- 1 --
	 }  // -- 0 --
	delete [] crt ;
 }

void TIlievaJudgeCheckupThread::RequestsDistribute (TRequest & req, const CDate begDate, const CDate endDate)
 {
  RequestAllResolutions resolutions (req.key) ;
  RequestResolutionPtr pResol ;

  while (resolutions.NextWholeRecord (pResol.Ref ()))
   {
    const TRequestResolution & r = pResol.Ref () ;

    if (r.resolution == '\0' || r.resolution == RESOLUTION_FAKE ||
        r.resolution == RESOLUTION_LAWED ||
        r.resolution == RESOLUTION_NEW)
      continue ;
    if (r.date < begDate)
      continue ;
    if (r.date > endDate)
      continue ;
    IncrementValues_Judge (r.judge, iC_B_IlievaOthTotal) ;
    IncrementValues_Judge (r.judge, iC_R_IlievaOthReqResol) ;
   }
 }

void TIlievaJudgeCheckupThread::CountRequests (const CDate begDate, const CDate endDate)
 {
  const size_t crtSz = 1024 * 2 ;
  char * const crt = new char [crtSz] ;

  if (TIlievaSummary :: RequestsCriteria (crt, crtSz, bMYEMYTCIU.types, begDate, endDate))
   {  // -- 0 --
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, "Разпореждания по преписки");
		wtdl->SetHeader(regimeDescr);
		TCountRecsQuery q (crt) ;
		TRequest req ;

		totalPartSteps = q.getRecsCount();
		while (q.Read ())
     {
			Tick(false);
			if(wtdl->WantBreak())
				break;
      req << q ;
      RequestsDistribute (req, begDate, endDate) ;
     }
   }  // -- 0 --
  delete [] crt ;
 }

// 20050804 ->
void TIlievaJudgeCheckupThread::incrementInRegEventValues (TInRegEvent & rIRE, TInReg& rIR, TLawsuit& rLaw, TRequest& rReq)
{
  rIR << rIRE;
  if(TRCDKey_(rIR.key).FilledOK() && rIR.Get())
  {
    const char lawType = inreg_to_law_type(rIR, &(rLaw.key), NULL, NULL, true,
	&(rReq.key));

    if(lawType && strchr(bMYEMYTCIU.types, lawType)) // @@@
    {
      // ----- These variables are used in skipThislaw() methods. But the
      // gathering of data is time consuming. So, I try simple shecks in order to
      // detect whether the data is necessary (after looking at the code of
      // skipThislaw() methods)
      long int lawSubj = 0;
      char lawKind = '\0';
      // -----
      long int lawComposition = 0;

      if(bMYEMYTCIU.lArrPtr()->Count() > 0 ||
        bMYEMYTCIU.composition > 0)
      {  // lawSubj or lawComposition is required
        if(TRCDKey_(rLaw.key).FilledOK())
        {
          if(rLaw.Get())
          {
            lawSubj = rLaw.subject;
            lawKind = rLaw.kind;
            lawComposition = rLaw.composition;
          }
        }
        else
          if(TRCDKey_(rReq.key).FilledOK())
          {
            if(rReq.Get() && requestColumns::tryToLawsuit(rReq, rLaw))
            {
              lawSubj = rLaw.subject;
              lawKind = rLaw.kind;
              lawComposition = rLaw.composition;
            }
          }
      }
      // lawKind is required anyway...
	if(//trueJudgeCheckup == false &&
        lawKind == '\0'  // still == '\0'
        )
      {
        if(TRCDKey_(rLaw.key).FilledOK())
        {
          if(rLaw.Get())
          {
            lawKind = rLaw.kind;
	    }
        }
        else
          if(TRCDKey_(rReq.key).FilledOK() && rReq.Get())
	    {
		lawKind = rReq.lawKind;
	    }
      }

      if(skipThisLaw(lawSubj))              //@@@
        return;
      if(skipThisLaw(lawKind))
        return;
      if(bMYEMYTCIU.composition > 0 && bMYEMYTCIU.composition != lawComposition)
        return;

      addJCDToArrayIfNecessary(rIRE.judge);

      IncrementValues_Judge (rIRE.judge,//pJCD->judgeUCN,
                                               iC_Oth_InReg_Resol_AC) ;

      IncrementValues_Judge (rIRE.judge,  iC_B_IlievaOthTotal) ;

    }
  }
}

// 20050804 <-

// 20050804 ->
void TIlievaJudgeCheckupThread::CountInRegResolutions (const CDate begDate, const CDate endDate)
 {
  const size_t crtSz = 1024 * 2 ;
  char * const crit = new char [crtSz] ;

  //
  //if (IlievaSummary :: RequestsCriteria (crit, crtSz, bMYEMYTCIU.types,
  //                                       begDate, endDate))
   {  // -- 0 --
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, "Разпореждания по жалби");
		wtdl->SetHeader(regimeDescr);

    {
	ostrstream oss(crit, crtSz); //crit.Size());

	oss << "SELECT F_TYPE, F_YEAR, F_NO, F_JUDGE, F_DATE " // F_FROM_DATE replaced with F_DATE  20051118
        "FROM T_INREG_EVENT "
        "WHERE "
        "F_DATE >= " << begDate.Value() << " AND "            // F_FROM_DATE replaced with F_DATE  20051118
        "F_DATE <= " << endDate.Value() << " AND "            // F_FROM_DATE replaced with F_DATE  20051118
        "F_JUDGE != '" << EMPTY << "' AND "
        "F_JUDGE != '" << INTER << "' "
        ;
      oss << " AND F_RESULT IN(";
      if((RESULT_INREG_RESOLUTIONS && RESULT_INREG_RESOLUTIONS[0]) ||
        (RESULT_TEXTUAL_RESOLUTIONS && RESULT_TEXTUAL_RESOLUTIONS[0]))
      {
        if(RESULT_INREG_RESOLUTIONS && RESULT_INREG_RESOLUTIONS[0])
        {
          oss << SQLCharSet(RESULT_INREG_RESOLUTIONS);
        }
        if((RESULT_INREG_RESOLUTIONS && RESULT_INREG_RESOLUTIONS[0]) &&
          (RESULT_TEXTUAL_RESOLUTIONS && RESULT_TEXTUAL_RESOLUTIONS[0]))
        {
          oss << ",";
        }
        if(RESULT_TEXTUAL_RESOLUTIONS && RESULT_TEXTUAL_RESOLUTIONS[0])
        {
          oss << SQLCharSet(RESULT_TEXTUAL_RESOLUTIONS);
        }
      }
      else
      {
        oss << STUB_CHAR;
      }
      oss << ") ";
	oss << " ;" << ends;
    }
//    crit->End();  @@@

    if(criteria_finished_ok(crit, typeid(*this).name(), "I????JudgeCheckup :: CountInRegResolutions"))
    {
	InRegEventPtr pIRE;
      TInReg inReg;
      LawPtr pLaw;
      TRequest req;

      TCountRecsQuery q(crit);

      const unsigned recs = q.getRecsCount();

      int r = 0;
	char textBufferTmp1[0x100];
	char textBufferTmp2[0x100];

	wtdl->GetText(textBufferTmp1, sizeof textBufferTmp1);

      // I call WantBreak() in the next row, because the process is
      // highly time consuming.
	while(wtdl->WantBreak() == false && q.Read())
      {
        pIRE.Ref() << q;
        incrementInRegEventValues(pIRE.Ref(),
          inReg, pLaw.Ref(), req );
        if((r % 5) == 0)                       // Gossip service - begin
        {
          double perc = 100.0;

          if(recs > 0)
	    {
		perc *= r;
		perc /= recs;
	    }
	    ostrstream(textBufferTmp2, sizeof(textBufferTmp2))
		<< textBufferTmp1 << " (" << resetiosflags(ios::scientific)
		<< setiosflags(ios::fixed | ios::showpoint)
		<< setprecision(5) << perc << "% обработени) ..." << ends;
	    ENDING_ZERO(textBufferTmp2);
	    wtdl->SetText(textBufferTmp2);
	  }
	  r++;
	}
    }
  }

  delete [] crit ;
}

void TIlievaJudgeCheckupThread::OpenSessionsDistribute (TSession & sess, const CDate endD)
 {
  TLawsuit law ;

  law.key = sess.key ;
  if (law.Get () == false)
    return ;
//  if (law.finished.Empty () == false &&
//      sess.date > law.finished)
//    return ;  // not need, included in the SQL criteria

  const char * const judgeUcn = sess.judge ;

  if (bankruptcy_law (law))
   {
    IncrementValues_Judge (judgeUcn, iC_S_IlievaBankruptcyFixed_OS) ;
    if (strchr (RESULT_LEFTS_OR_STOPPEDS, sess.result) ||
        strchr (RESULT_UNMOVEABLES, sess.result))
      IncrementValues_Judge (judgeUcn, iC_S_IlievaBankruptcyLeft_OS) ;
    else
      if (strchr (RESULT_FINALIZEDS, sess.result))
        if (sess.date != law.finished)
          if (law.decMade.Empty () == false && law.decMade <= endD)
            if (sess.date < law.decMade)  // 02.mar.2001  '<=' to '<'
              IncrementValues_Judge (judgeUcn,
                                     iC_S_IlievaBankruptcyLeft_OS) ;
              // Abolished progress
   }
 }


void TIlievaJudgeCheckupThread::CountOpenSessions(const CDate begDate, const CDate endDate)
 {
  const size_t crtSz = 1024 ;
  char * const crt = new char [crtSz] ;

  if (TIlievaSummary :: OpenSessionsCriteria (crt, crtSz, bMYEMYTCIU.types, begDate, endDate))
   {  // -- 0 --
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, "Открити заседания");
		wtdl->SetHeader(regimeDescr);
		TCountRecsQuery q (crt) ;
    TSession * pSess = new TSession ;

		totalPartSteps = q.getRecsCount();
		while (q.Read ())
     {
			Tick(false);
			if(wtdl->WantBreak())
				break;
			(* pSess) << q ;
	OpenSessionsDistribute (* pSess, endDate) ;
		 }
		delete pSess ;
	 }  // -- 0 --
	delete [] crt ;
 }

void TIlievaJudgeCheckupThread::Execute()
{
	if(useInheritedGossips)
	{
		TJudgeCheckupThread::Execute();
		return;
	}

	const CDate begD =
		trueJudgeCheckup ?
		CDate (1, bMYEMYTCIU.begMonth, bMYEMYTCIU.begYear) :
		cCBDEDBN.begD ;
	const CDate endD =
		trueJudgeCheckup ?
		CDate (lastDayOfThisMonth (CDate (1, bMYEMYTCIU.endMonth, bMYEMYTCIU.endYear))) :
		cCBDEDBN.endD ;

	try
	{  // -- 1 --
		if(wtdl->WantBreak() == false)
		{
			const int singleSpan = (barHiVal - barLowVal) / 4;

			endingVal = barLowVal + singleSpan;
			passeds = "Обработени";
			reportFinishedItems = true;
			wtdl->PleaseWait();
			currentPartStep = 0;
			lastShownVal = barLowVal;
			CountFinishedLaws(begD, endD);
			if(wtdl->WantBreak() == false)
			{
				Tick(true);
				endingVal += singleSpan;
				wtdl->PleaseWait();
				currentPartStep = 0;
				CountOtherActs(begD, endD);
				if(wtdl->WantBreak() == false)
				{
					Tick(true);
					endingVal += singleSpan;
					wtdl->PleaseWait();
					currentPartStep = 0;
					CountRequests(begD, endD);
	// 20050804 ->
					if(wtdl->WantBreak() == false)
					{
						Tick(true);
						endingVal += singleSpan;
						wtdl->PleaseWait();
						currentPartStep = 0;
						CountInRegResolutions(begD, endD);
	// 20050804 <-
						if(wtdl->WantBreak() == false)
						{
							Tick(true);
							endingVal = barHiVal;
							wtdl->PleaseWait();
							currentPartStep = 0;
							CountOpenSessions(begD, endD);
							if(wtdl->WantBreak() == false)
							{
								Tick(true);
								wtdl->AlmostDone();
							}
						}
					}
				}
			}
		}
	}  // -- 1 --
	CATCH_ANYTHING
}

void TIlievaJudgeCheckupThread::prepare()
 {
	if (! pArr)
		return ;
	useInheritedGossips = true ;
  sayAlmostDone = false;
  TJudgeCheckupThread :: prepare () ;
	useInheritedGossips = false ;
	Generate () ;  // MakeArray () ;
 }

TIlievaJudgeCheckupThread::TIlievaJudgeCheckupThread(const begMYEndMYTypeCIU & rBMYEMYTCIU) :
	TJudgeCheckupThread(rBMYEMYTCIU)
 {
  useInheritedGossips = true ;
 }

TIlievaJudgeCheckupThread::TIlievaJudgeCheckupThread(const collCompBegDEndDBegN & rCCBDEDBN,
	const unsigned int ciphIfNotTrueGudgeCheckup) :
	TJudgeCheckupThread(rCCBDEDBN, ciphIfNotTrueGudgeCheckup)
 {
  useInheritedGossips = true ;
 }

TIlievaJudgeCheckupThread::~TIlievaJudgeCheckupThread()
 {
 }

#   endif  // of APPEAL
# endif  // of INSTANCE
