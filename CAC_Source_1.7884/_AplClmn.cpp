#include "cac.h"  // 18.may.2001
//# include "all.h"

AppealedLawsInRegEnum :: AppealedLawsInRegEnum ()
 {
 }  // AppealedLawsInRegEnum :: AppealedLawsInRegEnum

bool AppealedLawsInRegColumns ::
  MakeInRegEventsCriteria (TInReg & rInReg, char * const crtIRE,
                           const size_t crtIRESz
                          )
 {
  if (! (crtIRE && crtIRESz))
    return false ;
  crtIRE [0] = '\0' ;
   {
    ostrstream ossIRE (crtIRE, crtIRESz) ;

    ossIRE <<
      "SELECT * FROM T_INREG_EVENT "
        "WHERE F_NO = " << rInReg.key.no << " AND "
              "F_YEAR = "  << rInReg.key.year << " AND "
              "F_TYPE = \'"  << rInReg.key.type  // Jimmy recommends
                             << "\' "
        " ORDER BY F_DATE " ;  // F_FROM_DATE replaced with F_DATE  20051118
    ossIRE << ";" << ends ;
   }
  crtIRE [crtIRESz - 1] = '\0' ;
  return criteria_finished_ok (crtIRE,
                   "AppealedLawsInRegColumns::MakeInRegEventsCriteria") ;
 }  // AppealedLawsInRegColumns :: MakeInRegEventsCriteria

void AppealedLawsInRegColumns ::
  MakeEventDependencyColumns (TInReg & rInReg, char * const wrkBuff,
                              const size_t wrkBuffL,
                              TInRegEvent * const pInRegEvent,
                              TOutReg * const pOutReg
                             )
 {
  if (! (wrkBuff && wrkBuffL))
    return ;

  const char * const higherInst1 = HIGHER_INST_1 ;
  const char * const higherInst2 = HIGHER_INST_2 ;

  columnArray forAdjusting;
  const size_t crtIRESz = 512 ;
  char * crtIRE = new char [crtIRESz] ;

  // --- this must be out of the loop
  forAdjusting.Add (columns [iC_DateSend]) ;
  forAdjusting.Add (columns [iC_HigherInstance]) ;
  forAdjusting.Add (columns [iC_DateReturned]) ;
  forAdjusting.Add (columns [iC_ResultAppealCourt]) ;  // TResolution !!
  forAdjusting.Add (columns [iC_ResultSupremeConcernCourt]) ;
  forAdjusting.Add (columns [iC_ResultAppealCourt2]) ;
  forAdjusting.Add (columns [iC_ResultMinistry]) ;
  forAdjusting.Add (columns [iC_Result]) ;
  forAdjusting.Add (columns [iC_ResultWithoutMinistry]) ;
  forAdjusting.Add (columns [iC_ReturnForComplete]) ;
  forAdjusting.Add (columns [iCF_RetForCompl]) ;
  forAdjusting.Add (columns [iC_Remark]) ;
  // ---
  if (MakeInRegEventsCriteria (rInReg, crtIRE, crtIRESz))
   {  // -- 0 --
    TQuery qE (crtIRE) ;
    bool paramEventUsed = pInRegEvent ? false : true ;  // 07.sep.2000
    bool queryReadOK ;  // 07.sep.2000

    while ((queryReadOK = qE.Read ()) == true
           || paramEventUsed == false  // 07.sep.2000
          )
     {  // -- 1 --
      column * pColumn ;
      TInRegEvent * pIRE = new TInRegEvent ;

// 07.sep.2000 - begin
//      (* pIRE) << qE ;
      clearGroupData (* pIRE) ;
      if (queryReadOK)
       {
        (* pIRE) << qE ;
        if (pInRegEvent)
          if (pInRegEvent -> key.no == pIRE -> key.no &&
              pInRegEvent -> key.year == pIRE -> key.year &&
              pInRegEvent -> date == pIRE -> date)
           {
            (* pIRE) << (* pInRegEvent) ;
            paramEventUsed = true ;
           }
       }
      else
        if (paramEventUsed == false)
          if (pInRegEvent)
           {
            (* pIRE) << (* pInRegEvent) ;
            paramEventUsed = true ;
           }
// 07.sep.2000 - end
      if ((pColumn = columns [iC_DateSend]) != NULL)
       {  // -- iC_DateSend --
        if (pIRE -> result == RESULT_IOREG_SEND)
         {
          ostrstream (wrkBuff, wrkBuffL) << pIRE -> date << ends ;
          pColumn -> addText (wrkBuff) ;
         }
       }  // -- iC_DateSend --
      if ((pColumn = columns [iC_HigherInstance]) != NULL)
       {  // -- iC_HigherInstance --
        if (pIRE -> Status () == RESULT_IOREG_SEND)
         {
          TOutReg * pORg = NULL ;  // = NULL is important !!!
          TOutReg * outRgPtr = pOutReg ;

          if (outRgPtr == NULL)
           {
            pORg = new TOutReg ;

            if (no_year_to_outreg (pIRE -> outNo, pIRE -> outYear,
                                   * pORg))
              outRgPtr = pORg ;
           }
          if (outRgPtr)
            AppealedLawsOutRegColumns ::
              AddReceiversToColumn (pColumn, wrkBuff, wrkBuffL,
                                    * outRgPtr) ;  // 03.oct.2000
          if (pORg)
            delete pORg ;
         }
       }  // -- iC_HigherInstance --
      if (strchr (RESULT_IOREG_RETURNEDS, pIRE -> result))
       {  // -- 0 --
        if ((pColumn = columns [iC_DateReturned]) != NULL)
         {  // -- iC_DateReturned --
          ostrstream (wrkBuff, wrkBuffL) << pIRE -> date << ends ;
          pColumn -> addText (wrkBuff) ;
         }  // -- iC_DateReturned --

        TOutReg * pLocOutReg = NULL ;  // = NULL is important !!!
        TOutReg * outRegPtr = pOutReg ;

        if (outRegPtr == NULL)
         {
          pLocOutReg = new TOutReg ;

          if (no_year_to_outreg (pIRE -> outNo, pIRE -> outYear,
                                 * pLocOutReg))
            outRegPtr = pLocOutReg ;
         }
        if (outRegPtr)
         {  // -- 1 ---
# if INSTANCE
//#   if APPEAL  // 06.feb.2001
//#   else  // of APPEAL  // 06.feb.2001
           {  // -- 2 --
            char tmpResolRes [16] ;

            scSprintf (sizeof (tmpResolRes), tmpResolRes, "%c%c",
                       RESOLUTION_RET_FOR_COMPLETE,
                       RESOLUTION_RET_NOT_ALLOWED) ;
            for (int i = 0 ; i < strlen (tmpResolRes) ; i ++)
             {
              const char rr = tmpResolRes [i] ;

              if (outRegPtr -> resultI == rr ||
                  outRegPtr -> resultII == rr ||
                  outRegPtr -> resultKS == rr ||
                  outRegPtr -> resultMJ == rr)
               {
                if ((pColumn = columns [iC_ReturnForComplete]) != NULL)
                 {  // -- iC_ReturnForComplete --
                  add_resolution_name_to_column (rr, pColumn) ;
                 }  // -- iC_ReturnForComplete --
                if ((pColumn = columns [iCF_RetForCompl]) != NULL)
                 {  // -- iCF_RetForCompl --
                  pColumn -> addText (STAR_STRING) ;
                 }  // -- iCF_RetForCompl --
               }
             }
           }  // -- 2 --
//#   endif  // of APPEAL  // 06.feb.2001
# endif  // INSTANCE
          if ((pColumn = columns [iC_ResultAppealCourt]) != NULL)
           {  // -- iC_ResultAppealCourt --
		add_resolution_name_to_column (outRegPtr -> resultI, pColumn) ;
		// 2012:003 HACK
		if (any(outRegPtr->remarkVZ))
		 pColumn -> addText (outRegPtr->remarkVZ);
	     }  // -- iC_ResultAppealCourt --
	    if ((pColumn = columns [iC_ResultSupremeConcernCourt]) != NULL)
	     {  // -- iC_ResultSupremeConcernCourt --
		add_resolution_name_to_column (outRegPtr -> resultKS, pColumn) ;
		// 2012:003 HACK
		if (any(outRegPtr->remarkKS))
		 pColumn -> addText (outRegPtr->remarkKS);
	     }  // -- iC_ResultSupremeConcernCourt --
	    if ((pColumn = columns [iC_ResultAppealCourt2]) != NULL)
	     {  // -- iC_ResultAppealCourt2 --
		add_resolution_name_to_column (outRegPtr -> resultII, pColumn) ;
	     }  // -- iC_ResultAppealCourt2 --
	    if ((pColumn = columns [iC_ResultMinistry]) != NULL)
	     {  // -- iC_ResultMinistry --
		add_resolution_name_to_column (outRegPtr -> resultMJ, pColumn) ;
	     }  // -- iC_ResultMinistry --
	    if ((pColumn = columns [iC_Result]) != NULL)
	     {  // -- iC_Result --
		add_resolution_name_to_column (wrkBuff, wrkBuffL, higherInst1,
							 outRegPtr -> resultI, pColumn) ;
		add_resolution_name_to_column (wrkBuff, wrkBuffL,
							 SUPREME_CONCERN_COURT,
							 outRegPtr -> resultKS,
							 pColumn) ;
		add_resolution_name_to_column (wrkBuff, wrkBuffL, higherInst2,
							 outRegPtr -> resultII, pColumn) ;
		add_resolution_name_to_column (wrkBuff, wrkBuffL,
							 AppNameMinistryShort,
							 outRegPtr -> resultMJ, pColumn) ;
	     }  // -- iC_Result --
	    if ((pColumn = columns [iC_ResultWithoutMinistry]) != NULL)
	     {  // -- iC_ResultWithoutMinistry --
		add_resolution_name_to_column (wrkBuff, wrkBuffL, higherInst1,
							 outRegPtr -> resultI, pColumn) ;
		add_resolution_name_to_column (wrkBuff, wrkBuffL,
							 SUPREME_CONCERN_COURT,
							 outRegPtr -> resultKS,
							 pColumn) ;
		add_resolution_name_to_column (wrkBuff, wrkBuffL, higherInst2,
							 outRegPtr -> resultII, pColumn) ;
	     }  // -- iC_ResultWithoutMinistry --

#if 0  // 2012:003 HACK
	    if ((pColumn = columns [iC_Remark]) != NULL)
	     {  // -- iC_Remark --
// 25.sep.2000 - begin
//            if (pIRE -> text [0])
//              pColumn -> addText (pIRE -> text) ;
		if (any(outRegPtr->remark))
		  pColumn -> addText (outRegPtr->remark);
// 25.sep.2000 - end
	     }  // -- iC_Remark --
#endif
	   }  // -- 1 ---
	  if (pLocOutReg)
	    delete pLocOutReg ;
	 }  // -- 0 --
      adjust_column_rows (forAdjusting) ;  // 30.aug.2000 commented,
                                           // 18.may.2001 - back again
      delete pIRE ;
     }  // -- 1 --  // while
   }  // -- 0 --
  delete [] crtIRE ;
 }  // AppealedLawsInRegColumns ::
    //   MakeEventDependencyColumns (TInReg & rInReg)

void AppealedLawsInRegColumns::AddAppealersToColumn(column* const pColumn,
  char* const buff, const size_t buffL, TInReg& rInReg, const bool withProxies,
  stringArray* pAppealerItems // 21.dec.2004
  )
{
  if(!(pColumn && buff && buffL))
    return;

  TSideWind waln;
  TGArray<TLinkWind> links;

  inreg_to_link_winds(&rInReg, links, false);
  waln << rInReg;
  waln.FloatCriteria(buff);
  if(strlen(buff) < buffL - 2)
  {  // -- ~1~ --
    char* const partArea = buff + strlen(buff) + 1;
    const size_t partLen = buffL - 1 - strlen(buff);
    TQuery q(buff);

    while(q.Read())
    {  // -- ~2~ --
      waln << q;
      if(waln.state == STATE_INDIRECT && withProxies == true)
      {  // -- ~3~ --
        int usedProxies = 0;
        TType t ;

        t.type = rInReg.key.type;
        typeToString(t.type, t.name, sizeof(t.name));
	  if(links.Count())
        {  // -- ~4~ --
          TGArray<TLinkWind> walnLinks;
          int proxyIndex = -1;

          t_to_T_links(waln, links, walnLinks, false);
          {
		for(int i = 0; i < walnLinks.Count(); i ++)
            {
              if(walnLinks[i]->state == STATE_INDIRECT)
              {
                proxyIndex = i;
                usedProxies++;
              }
            }
          }
          if(proxyIndex >= 0 && proxyIndex < walnLinks.Count())
          {
            side_link_to_string(partArea, partLen, *(walnLinks[proxyIndex]),
              true, false);
            if(pAppealerItems)
            {
              pAppealerItems->Add(strnew (partArea));
            }
            else
            {
              pColumn->addText(partArea);
            }
          }
          else
            usedProxies = 0;
        }  // -- ~4~ --
        if (usedProxies != 1)
        {
          ucnAndUcnTypeToName(waln.ucn, waln.ucnType, partArea, partLen);
          if(usedProxies)
            error("Използувано е повече от едно упълномощено лице при "
              "обявяването на %.255s за жалбоподател (%s No %ld/%ld)",
              partArea, t.name, rInReg.key.no, rInReg.key.year);
          else
            error("Липсваща връзка с упълномощено лице за %.255s "
              "(%s No %ld/%ld)",
              partArea, t.name, rInReg.key.no, rInReg.key.year);
          if(pAppealerItems)
          {
            pAppealerItems->Add(strnew (partArea));
          }
          else
          {
            pColumn->addText(partArea);
          }
        }
      }  // -- ~3~ --
      else
        if(waln.state == STATE_APPEALER ||
          (waln.state == STATE_INDIRECT && withProxies == false))
        {
          ucnAndUcnTypeToName(waln.ucn, waln.ucnType, partArea, partLen);
          if(pAppealerItems)
          {
            pAppealerItems->Add(strnew (partArea));
          }
          else
          {
            pColumn->addText(partArea);
          }
        }
        else
	    if(waln.involvement == INVOLVEMENT_SENDER)
	    {
            ucnAndUcnTypeToName(waln.ucn, waln.ucnType, partArea, partLen);
            if(pAppealerItems)
            {
              pAppealerItems->Add(strnew (partArea));
            }
            else
            {
              pColumn->addText(partArea);
            }
          }
    }  // -- ~2~ --
  }  // -- ~1~ --
}  // AppealedLawsInRegColumns :: AddAppealersToColumn

void AppealedLawsInRegColumns :: MakeColumns (TInReg & rInReg,
                                              const int serialNo,
						     TInRegEvent * const pInRegEvent,
                                              TOutReg * const pOutReg,
                                      stringArray* pDocKindItems, // 21.dec.2004
                                      stringArray* pLawsuitItems, // 21.dec.2004
                                      stringArray* pApldActItems, // 21.dec.2004
                                      stringArray* pAppealerItems // 21.dec.2004
                                             )
// look for an old version of this method in
// VersionArchives\860\22Mar01\_appname.j, _aplclmn.cpp
 {
  const size_t buffSz = MAKE_COLUMNS_BUFF_SIZE ;
  const size_t buffL = buffSz - 1 ;
  char * const buff = new char [MAKE_COLUMNS_BUFF_SIZE] ;
  column * pColumn ;
// 18.sep.2000 - begin
  TDecision * pDecision = new TDecision ;
  bool decisionFound = false ;
  TSession * pSession = new TSession ;
  bool sessionFound = false ;
  TRequest * pRequest = new TRequest ;  // 10.apr.2001 - begin
  bool requestFound = false ;
  TInReg * pAplIR = new TInReg ;
  bool aplIRFound = false ;
  TInRegEvent * pAplIREvent = new TInRegEvent ;
  bool aplIREventFound = false ;

  if (rInReg.decision.type)
   {  //  -- rInReg.decision.type --
    if (strchr (TYPE_LAWSUITS, rInReg.decision.type) ||
        strchr (TYPE_INREGS, rInReg.decision.type))
     {  // -- inRegPtr --
      const TInReg * inRegPtr = NULL ;

      if (strchr (TYPE_LAWSUITS, rInReg.decision.type))
        inRegPtr = & rInReg ;
      else
        if (strchr (TYPE_INREGS, rInReg.decision.type))
         {
          pAplIR -> key = rInReg.decision ;
          if (TRCDKey_ (pAplIR -> key).FilledOK () && pAplIR -> Get ())
           {
            inRegPtr = pAplIR ;
            aplIRFound = true ;
           }
          pAplIREvent -> key = rInReg.decision ;
          pAplIREvent -> kind = rInReg.decisionKind ;
          pAplIREvent -> date = rInReg.decisionDate ;
          if (TRCDKey_ (pAplIREvent -> key).FilledOK () &&
              pAplIREvent -> kind &&
              pAplIREvent -> date.Empty () == false &&
              pAplIREvent -> Get ())
            aplIREventFound = true ;
         }
      if (inRegPtr)  // 10.apr.2001 - end
        if (inRegPtr -> decisionDate.Empty () == false &&
		inRegPtr -> lawsuit.type && inRegPtr -> lawsuit.no &&
		inRegPtr -> lawsuit.year)
	   {
	#if 20160179
		TIndexContainer *const appealedAct = inRegPtr->AppealedAct(false, false);

		if (appealedAct)
		{
			if (strchr(KIND_ENDOCS, appealedAct->kind))
			{
				decisionFound = true;

				if (appealedAct->Find("F_EXTRA_NO"))
					pDecision->Init((const TSurroundment *) appealedAct);
				else
					*pDecision << appealedAct;
			}

			delete appealedAct;
		}
	#else  // 20160179
	    decisionFound =
		extractDecision (inRegPtr -> lawsuit,
				     inRegPtr -> sessionKind,  // 15.jan.2001
				     inRegPtr -> decisionDate,
				     * pDecision, inRegPtr -> decisionKind) ;
	#endif  // 20160179
	    sessionFound =
		extractSession (inRegPtr -> lawsuit, inRegPtr -> decisionDate,
				    * pSession, inRegPtr -> sessionKind) ;
	   }
     }  // -- inRegPtr --  // 10.apr.2001 - begin
    else
	if (strchr (TYPE_REQUESTS, rInReg.decision.type))
	 {
	  pRequest -> key = rInReg.lawsuit ;

	  if (TRCDKey_ (pRequest -> key).FilledOK () && pRequest -> Get ())
	    requestFound = true ;
	 }
   }  //  -- rInReg.decision.type --  // 10.apr.2001 -
  if (decisionFound == false)
    clearGroupData (* pDecision) ;
  if (sessionFound == false)
    clearGroupData (* pSession) ;
// 18.sep.2000 - end
  if (requestFound == false)  // 10.apr.2001 - begin
    clearGroupData (* pRequest) ;
  if (aplIRFound == false)
    clearGroupData (* pAplIR) ;
  if (aplIREventFound == false)
    clearGroupData (* pAplIREvent) ;  // 10.apr.2001 - end

  buff [buffL] = '\0' ;
  if ((pColumn = columns [iC_SerialNo]) != NULL)
   {  // -- iC_SerialNo --
    addLongToColumn (pColumn, serialNo, true) ;
   }  // -- iC_SerialNo --
  if ((pColumn = columns [iC_DateReceived]) != NULL)
   {  // -- iC_DateReceived --
    ostrstream (buff, buffL) << rInReg.date << ends ;
    pColumn -> addText (buff) ;
   }  // -- iC_DateReceived --
  if ((pColumn = columns [iC_DocKind]) != NULL)
   {  // -- iC_DocKind --
    if (rInReg.kind == KIND_OTHER_IN_DOCUMENT && rInReg.text [0])
    {
      if(pDocKindItems)
      {
        pDocKindItems->Add(strnew (rInReg.text));
      }
      else
      {
        pColumn -> addText (rInReg.text) ;
      }
    }
    else
     {
      kindToString (rInReg.kind, buff, buffL) ;
      if (rInReg.kind == KIND_IN_LETTER && rInReg.text [0])
       {
        strConcat (buff, " - ", buffL) ;
        strConcat (buff, rInReg.text, buffL) ;
       }
      if(pDocKindItems)
      {
        pDocKindItems->Add(strnew (buff));
      }
      else
      {
        pColumn -> addText (buff) ;
      }
     }
   }  // -- iC_DocKind --
  if ((pColumn = columns [iC_SenderOfComplaint]) != NULL)
   {  // -- iC_SenderOfComplaint --
    AddAppealersToColumn (pColumn, buff, buffL, rInReg,
				  true, pAppealerItems  // 21.dec.2004
                         ) ;
   }  // -- iC_SenderOfComplaint --
   {
    TLawsuit law ;

    clearGroupData (law) ;  // 10.apr.2001 - begin
    if (rInReg.decision.type)
     {
      if (strchr (TYPE_LAWSUITS, rInReg.decision.type)) // 10.apr.2001-end
        law.key = rInReg.lawsuit ;
      else  // 10.apr.2001 - begin
        if (strchr (TYPE_INREGS, rInReg.decision.type))
         {
          if (aplIRFound)
            law.key = pAplIR -> lawsuit ;
         }
     }  // 10.apr.2001 - end
    if (law.key.type)
     {  // -- law.key.type --
      if (law.Get ())
       {  // -- law.Get () --
        if ((pColumn = columns [iC_LawKindLawNoYear]) != NULL)
         {  // -- iC_LawKindLawNoYear --
          ostrstream (buff, buffL)
            << TRCDKeyContainer_ (law.key, law.kind) << ends ;
          if(pLawsuitItems)
          {
            pLawsuitItems->Add(strnew (buff));
          }
          else
          {
            pColumn -> addText (buff) ;
          }
          if (sessionFound)  // 04.oct.2000
            if (! strchr (TYPE_INREGS, rInReg.decision.type)) //10.apr.2001
             {  // 04.oct.2000
              ostrstream (buff, buffL)
                << "\nЗаседание - " << pSession -> date << ends ;
              if(pLawsuitItems)
              {
                pLawsuitItems->Add(strnew (buff));
              }
              else
              {
                pColumn -> addText (buff) ;
              }
             }
          if (strchr (TYPE_INREGS, rInReg.decision.type))  // 10.apr.2001
           {  // 10.apr.2001 - begin
            ostrstream oss (buff, buffL) ;

            oss << "\nРазпореждане от " << rInReg.decisionDate ;
            if (aplIRFound)
              oss << " по "
                  << TRCDKeyContainer_ (pAplIR -> key, pAplIR -> kind) ;
            oss << ends ;
            if(pLawsuitItems)
            {
              pLawsuitItems->Add(strnew (buff));
            }
            else
            {
              pColumn -> addText (buff) ;
            }
           }  // 10.apr.2001 - end
         }  // -- iC_LawKindLawNoYear --
        if ((pColumn = columns [iC_LawKindLawNoYear_Exact]) != NULL)
         {  // -- iC_LawKindLawNoYear_Exact --
          ostrstream (buff, buffL)
            << TRCDKeyContainer_ (law.key, law.kind) << ends ;
          if(pLawsuitItems)
          {
            if(columns[iC_LawKindLawNoYear] == NULL)  // to avoid duplication
            {
              pLawsuitItems->Add(strnew (buff));
            }
          }
          else
          {
            pColumn -> addText (buff) ;
          }
         }  // -- iC_LawKindLawNoYear_Exact --
        if ((pColumn = columns [iC_LawSubject]) != NULL)
	   {  // -- iC_LawSubject --
          addSubjCodeToColumn (pColumn, law.subject, law.kind) ;
	    subjectToString (law.key.type, law.subject, buff, buffL) ;
          pColumn -> addText (buff) ;
         }  // -- iC_LawSubject --
       }  // -- law.Get () --
     }  // -- law.key.type --
    else  // 10.apr.2001 - begin
      if (strchr (TYPE_REQUESTS, rInReg.decision.type))
       {
        if (requestFound)
         {  // -- req.Get () --
          if ((pColumn = columns [iC_LawKindLawNoYear]) != NULL)
           {  // -- iC_LawKindLawNoYear --
            ostrstream (buff, buffL)
              << TRCDKeyContainer_ (pRequest -> key, pRequest -> kind)
              << ends ;
            if (rInReg.decisionDate.Empty () == false)
             {
              ostrstream (buff, buffL)
                << "Разпореждане - " << rInReg.decisionDate << ends ; // 20070901
              if(pLawsuitItems)
              {
                pLawsuitItems->Add(strnew (buff));
              }
              else
              {
                pColumn -> addText (buff) ;
              }
             }
           }  // -- iC_LawKindLawNoYear --
          if ((pColumn = columns [iC_LawKindLawNoYear_Exact]) != NULL)
           {  // -- iC_LawKindLawNoYear_Exact --
            ostrstream (buff, buffL)
              << TRCDKeyContainer_ (pRequest -> key, pRequest -> kind)
              << ends ;
            if(pLawsuitItems)
            {
              if(columns[iC_LawKindLawNoYear] == NULL)  // to avoid duplication
              {
                pLawsuitItems->Add(strnew (buff));
              }
            }
            else
            {
              pColumn -> addText (buff) ;
            }
           }  // -- iC_LawKindLawNoYear_Exact --
         }  // -- req.Get () --
       }  // 10.apr.2001 - end
   }
  if ((pColumn = columns [iC_AppealedAct]) != NULL)
   {  // -- iC_AppealedAct --
    buff [0] = '\0' ;
    if (rInReg.decision.type)  // 10.apr.2001 - begin
     {  // -- rInReg.decision.type --
      if (strchr (TYPE_LAWSUITS, rInReg.decision.type))
       {  // 10.apr.2001 - end
        if (decisionFound)
         {
          kindToString (pDecision -> kind, buff, buffL) ;
          ostrstream (buff + strlen (buff), buffL - strlen (buff))
             << " - " << pDecision -> fromDate << ends ;
         }
        if (decisionFound && sessionFound)
          ostrstream (buff + strlen (buff), buffL - strlen (buff))
            << " \n" << (char) 160 << " \n " << ends ;
        if (sessionFound)
          ostrstream (buff + strlen (buff), buffL - strlen (buff))
            << "Заседание - " << pSession -> date << ends ;
        if (buff [0])
        {
          if(pApldActItems)
          {
            pApldActItems->Add(strnew (buff));
          }
          else
          {
            pColumn -> addText (buff) ;
          }
        }
       }  // 10.apr.2001 - begin
      else
        if (strchr (TYPE_INREGS, rInReg.decision.type))
         {
          if (aplIRFound)
           {
            ostrstream (buff, buffL)
              << TRCDKeyContainer_ (pAplIR -> key, pAplIR -> kind)
              << " \n" << (char) 160 << " \n " << ends ;
            if(pApldActItems)
            {
              pApldActItems->Add(strnew (buff));
            }
            else
            {
              pColumn -> addText (buff) ;
            }
           }
          ostrstream (buff, buffL)
            << "Разпореждане - " << rInReg.decisionDate << ends ;
          if(pApldActItems)
          {
            pApldActItems->Add(strnew (buff));
          }
          else
          {
            pColumn -> addText (buff) ;
          }
         }
        else
          if (strchr (TYPE_REQUESTS, rInReg.decision.type))
           {
            if (requestFound)
             {
              ostrstream (buff, buffL)
                << "Разпореждане - " << rInReg.decisionDate << ends ; // 20070901
              if(pApldActItems)
              {
                pApldActItems->Add(strnew (buff));
              }
              else
              {
                pColumn -> addText (buff) ;
              }
             }
           }
     }  // -- rInReg.decision.type --  // 10.apr.2001 - end
   }  // -- iC_AppealedAct --
  if ((pColumn = columns [iC_Remark]) != NULL)
   {  // -- iC_Remark --
    // Nothing here, MakeEventDependencyColumns may write to this column.
   }  // -- iC_Remark --
  if ((pColumn = columns [iC_InRegNo]) != NULL)
   {  // -- iC_InRegNo --
    ostrstream (buff, buffL)
       << rInReg.key.no << "/" << rInReg.key.year << ends ;
    pColumn -> addText (buff) ;
   }  // -- iC_InRegNo --
  if ((pColumn = columns [iC_InRegNoOnly]) != NULL)
   {  // -- iC_InRegNoOnly --
    ostrstream (buff, buffL) << rInReg.key.no << ends ;
    pColumn -> addText (buff) ;
   }  // -- iC_InRegNoOnly --
  if ((pColumn = columns [iC_RegNoAndDateReceived]) != NULL)
   {  // -- iC_RegNoAndDateReceived --
    ostrstream (buff, buffL)
      << rInReg.key.no << "/" << rInReg.key.year << " "
      << rInReg.date << ends ;
    pColumn -> addText (buff) ;
   }  // -- iC_RegNoAndDateReceived --
  if ((pColumn = columns [iC_Judge]) != NULL)
   {  // -- iC_Judge --  rewritten at 10.apr.2001
    const char * j = NULL ;

    if (strchr (TYPE_LAWSUITS, rInReg.decision.type))
     {
      if (sessionFound)
        j = pSession -> judge ;
     }
    else
      if (strchr (TYPE_INREGS, rInReg.decision.type))
       {
        if (aplIREventFound)
          j = pAplIREvent -> judge ;
       }
      else
        if (strchr (TYPE_REQUESTS, rInReg.decision.type))
         {
          // nothing yet, because there is not enough data
          // from which resolution to get the judge
         }
    if (j && ucnPresent (j) &&
        citizenUCNAndUCNTypeToName (j, UCN_CITIZEN_UCN, buff, buffL))
      pColumn -> addText (buff) ;
   }  // -- iC_Judge --
  if ((pColumn = columns [iC_OutNo]) != NULL)
   {  // -- iC_OutNo --
    // Nuthing yet.
   }  // -- iC_OutNo --
  MakeEventDependencyColumns (rInReg, buff, buffSz, pInRegEvent,
                              pOutReg) ;
  delete pAplIREvent ;  // 10.apr.2001 - begin
  delete pAplIR ;
  delete pRequest ;  // 10.apr.2001 - end
  delete pDecision ;
  delete pSession ;
  delete [] buff ;
 }  // AppealedLawsInRegColumns :: MakeColumns

AppealedLawsInRegColumns ::
AppealedLawsInRegColumns (const char * formatFN, TWindow * parent,
                          TInReg & rInReg, const int serialNo,
                          const size_t minRows,
                          TInRegEvent * const pInRegEvent,
                          TOutReg * const pOutReg,
                          stringArray* pDocKindItems, // 21.dec.2004
                          stringArray* pLawsuitItems, // 21.dec.2004
                          stringArray* pApldActItems, // 21.dec.2004
                          stringArray* pAppealerItems // 21.dec.2004
                         ) :
  AppealedLawsInRegEnum (),
  columnsArr (formatFN, parent, i_columns,
              1 + OFFSET_APPL_LAWS_IN_REG_CIPHER)
 {
  MakeColumns (rInReg, serialNo, pInRegEvent, pOutReg,
               pDocKindItems, pLawsuitItems, pApldActItems, pAppealerItems) ;
  defineMaxRows () ;
  if (maxRows < minRows)
    maxRows = minRows ;
 }  // AppealedLawsInRegColumns :: AppealedLawsInRegColumns

//

