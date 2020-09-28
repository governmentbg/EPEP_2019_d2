#include "cac.h"  // 18.may.2001
//# include "all.h"

requestColumnsEnum :: requestColumnsEnum ()
 {
 }  // requestColumnsEnum :: requestColumnsEnum

void requestColumns ::
judgesAndResolutions (TRequest & rRequest, TLawsuit & law,
                      const bool lawFound,
                      column * pJudgeColumn, column * pResolutionColumn)
 {
  RequestAllResolutions resolutions (rRequest.key) ;
  RequestResolutionPtr pResol ;
  bool lawJudgePrinted = false ;
  char ln [SIZE_OF_LINE] ;
  columnArray forAdjusting ;

  forAdjusting.Add (pJudgeColumn) ;
  forAdjusting.Add (pResolutionColumn) ;
  while (resolutions.NextWholeRecord (pResol.Ref ()))
   {  // -- 0 --
    const TRequestResolution & r = pResol.Ref () ;

    if (r.resolution == RESOLUTION_NEW)
      continue ;
    if (r.resolution == RESOLUTION_LAWED)
     {
      if (pJudgeColumn && lawFound && ucnPresent (law.judge))
        if (citizenUCNAndUCNTypeToName (law.judge, UCN_CITIZEN_UCN,
                                        ln, sizeof (ln),
                                        false, false, false))
         {
          pJudgeColumn -> addText (ln) ;
          lawJudgePrinted = true ;
         }
      if (pResolutionColumn)
        add_req_resolution_to_column (r, pResolutionColumn,
                                       ln, sizeof (ln)) ;
     }
    else
      if (r.resolution)
       {
        if (pJudgeColumn)
          if (ucnPresent (r.judge))
            if (citizenUCNAndUCNTypeToName (r.judge, UCN_CITIZEN_UCN,
                                            ln, sizeof (ln),
                                            false, false, false))
              pJudgeColumn -> addText (ln) ;
        if (pResolutionColumn)
          add_req_resolution_to_column (r, pResolutionColumn,
                                         ln, sizeof (ln)) ;
       }
    adjust_column_rows (forAdjusting) ;
   }  // -- 0 --
  if (pJudgeColumn && lawJudgePrinted == false && lawFound &&
      ucnPresent (law.judge))  // This normally will never be true
    if (citizenUCNAndUCNTypeToName (law.judge, UCN_CITIZEN_UCN,
                                    ln, sizeof (ln),
                                    false, false, false))
      pJudgeColumn -> addText (ln) ;
 }  // requestColumns :: judgesAndResolutions

bool requestColumns :: tryToLawsuit (const TRequest & rRequest,
                                     TLawsuit & law)
 {
  bool lawFound = false ;
  const char * const funcName = "requestColumns::tryToLawsuit" ;

  clearGroupData (law) ;
  if (TRCDKey_(rRequest.key).FilledOK())
   {
    char crit [512] ;

     {
      ostrstream oss (crit, sizeof (crit)) ;

      oss
        << "SELECT * FROM T_LAWSUIT WHERE " ;
      if (rRequest.kind)
       {
        const char lT = request_doc_to_law_type (rRequest) ;

        if (lT)
          oss << "F_TYPE = '" << lT << "' AND " ;
       }
      oss <<
           "F_SOURCE_NO = " << rRequest.key.no << " AND "
           "F_SOURCE_YEAR = " << rRequest.key.year << " AND "
           "F_SOURCE_TYPE = '" << rRequest.key.type << "' ;"
        << ends ;
     }
    ENDING_ZERO (crit) ;
    if (criteria_finished_ok (crit, funcName))
     {
      int laws = 0 ;
      TQuery qL (crit) ;

      while (qL.Read ())
       {
        law << qL ;
        laws ++ ;
       }
      if (laws == 1)
        lawFound = true ;
      else
       {
        if (laws > 1)
         {
          TType t ;

          typeToString (rRequest.key.type, t.name, sizeof (t.name)) ;
          error ("По %s %ld/%ld са образувани %d дела",
                 t.name, rRequest.key.no, rRequest.key.year, laws) ;
         }
        clearGroupData (law) ;
       }
     }
   }
  else
    error("%s\n%s", WRONG_ARGS_PASSED_TO, funcName);
   return lawFound;
 }  // requestColumns :: tryToLawsuit

void requestColumns :: makeColumns (TRequest & rRequest,
                           const JudgeNameRoll* const pJudges,    // 02.dec.2002
                           const SubjectCodeRoll* pSubjectCodes   // 02.dec.2002
                                   )
 {
  if (! columns)
    return ;

  column * pColumn ;
  TLawsuit law ;
  const bool lawFound = tryToLawsuit (rRequest, law) ;  

  if ((pColumn = columns [iC_No]) != NULL)
   {  // -- iC_No --
    char tmp [256] ;  // [32] ;  26.sep.2001

     {
      ostrstream oss (tmp, sizeof (tmp)) ;

      oss << rRequest.key.no << "/" ;
      if (rRequest.date.Empty ())
        oss << "Неизвестна" ;
      else
        oss << rRequest.date ;
      oss << ends ;
     }
    tmp [sizeof (tmp) - 1] = '\0' ;
    pColumn -> addText (tmp) ;
# if INSTANCE  // 26.sep.2001 - begin
#   if APPEAL
#   else  // of APPEAL
    tmp [0] = '\0' ;
    //
//    F_ucn_type == I//  -  Подчинени РС
// 20070115 --> in comment
/*
    if (ucnPresent (rRequest.sender))
      if (ucnAndUcnTypeToName (rRequest.sender, UCN_INSTANCE_LOWER, tmp,
                               sizeof (tmp)) == false)
        tmp [0] = '\0' ;  '
*/
// 20070115 <-- in comment

    if (tmp [0])  // 21.dec.2004 - begin
      strConcat (tmp, " ", sizeof (tmp)) ;
    {
      const size_t l = strlen(tmp);

      // Code separated in a function on 21.dec.2004
      region_act_to_string(rRequest, tmp + l, sizeof(tmp) - l);
    }  // 21.dec.2004 - end
    tmp [sizeof (tmp) - 1] = '\0' ;
    if (tmp [0])
      pColumn -> addText (tmp) ;
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE  // 26.sep.2001 - end
   }  // -- iC_No --
  if ((pColumn = columns [iC_LawKind]) != NULL)
   {  // -- iC_LawKind --
    TKind k ;
// 17.apr.2000 - begin
    char tmp [sizeof (k.name) + 10] ;
    ostrstream oss (tmp, sizeof (tmp) - 1) ;

    tmp [sizeof (tmp) - 1] = '\0' ;
// 17.apr.2000 - end
    {
	oss << Kind->Seek(rRequest.lawKind);
	if (quick_currently_quick(rRequest.quick))
		oss << " Б";
	oss << ends;
	pColumn->addText (tmp) ;
    }
   }  // -- iC_LawKind --
  if ((pColumn = columns [iC_College]) != NULL)
   {  // -- iC_College --
    TCollege c ;

    c.college = rRequest.college ;
    if (c.Get ())
      pColumn -> addText (c.name) ;
   }  // -- iC_College --
  if ((pColumn = columns [iC_Composition]) != NULL)
   {  // -- iC_Composition --
    if (lawFound)
     {
      TComposition c ;

      c << law ;
      if (c.Get ())
        pColumn -> addText (c.name) ;
     }
   }  // -- iC_Composition --
  if (columns [iC_Claimant] || columns [iC_Defendant]
  	|| columns [iC_SidesBrief]  // 02.dec.2002
  	)
   {
    reqRCDKeyToSidePair pairs (rRequest, true) ;
    TClaimant cl ;
    TDefendant def ;

    for (unsigned i = 0 ;
         i < pairs.getPairsCount () && pairs.nextPair (& cl, & def) ;
         i ++)
     {
      if ((pColumn = columns [iC_Claimant]) != NULL)
       {  // -- iC_Claimant --
        if (cl.cName [0])
          pColumn -> addText (cl.cName) ;
       }  // -- iC_Claimant --
      if ((pColumn = columns [iC_Defendant]) != NULL)
       {  // -- iC_Defendant --
        if (def.dName [0])
          pColumn -> addText (def.dName) ;
       }  // -- iC_Defendant --
      if ((pColumn = columns [iC_SidesBrief]) != NULL)  // 02.dec.2002 - begin
       {  // -- iC_SidesBrief --
       	const size_t clCnt = pairs.getClaimantsCount () ;
       	const size_t defCnt = pairs.getDefendantsCount () ;
         const bool bothPresent = clCnt && defCnt ;
         const char * const andOthers = "и други" ;

         if (i == 0)
          {
           if (cl.cName [0])
             pColumn -> addText (cl.cName) ;
           if (clCnt > 1)
             pColumn -> addText (andOthers) ;
           if (bothPresent)
             pColumn -> addText ("срещу") ;
           if (def.dName [0])
             pColumn -> addText (def.dName) ;
           if (defCnt > 1)
             pColumn -> addText (andOthers) ;
          }
       }  // -- iC_SidesBrief --  // 02.dec.2002 - end
     }
   }
  if ((pColumn = columns [iC_Sender]) != NULL)
   {  // -- iC_Sender --
    // The code separated in a function on 21.dec.2004
    TSender sender;

    if(request_to_sender(rRequest, sender))
    {
      pColumn->addText(sender.name);
    }
   }  // -- iC_Sender --
  if ((pColumn = columns [iC_Remark]) != NULL)
   {  // -- iC_Remark --
   }  // -- iC_Remark --
// 02.dec.2002 - begin
	if ((pColumn = columns[iC_JudgeHTMLSelect]) != NULL)
	{  // -- iC_JudgeHTMLSelect --
		if(pJudges)
		{
			CharPtr tmp(10 * 1024);

			tmp[0] = '\0';
			{
				const char type = request_doc_to_law_type(rRequest);

			   if(type)
					ostrstream (tmp, tmp.Size())
						<< CharMatchHTMLSelectWriter (*pJudges, type, true, false, 0)
						<< ends;
      }
      tmp[tmp.Size() - 1] = '\0';
      if(strlen(tmp) < tmp.Size() - 1)
			  pColumn->addText(tmp);
      else
        error("%s::makeColumns:\n"
          "the buffer for iC_JudgeHTMLSelect is too small",
          typeid(*this).name());
		}
	}  // -- iC_JudgeHTMLSelect --
	if ((pColumn = columns[iC_SubjectCodeHTMLSelect]) != NULL)
	{  // -- iC_SubjectHTMLSelect --
		if(pSubjectCodes)
		{
			CharPtr tmp(10 * 1024);

			tmp[0] = '\0';
			{
				const char type = request_doc_to_law_type(rRequest);

			   if(type)
					ostrstream (tmp, tmp.Size())
						<< CharMatchHTMLSelectWriter (*pSubjectCodes, type, true,
              false, 0)
						<< ends;
      }
      tmp[tmp.Size() - 1] = '\0';
      if(strlen(tmp) < tmp.Size() - 1)
      {
			  pColumn->addText(tmp);
        //::MessageBox(NULL, tmp,	AppNameName,MB_OK);
      }
      else
        error("%s::makeColumns:\n"
          "the buffer for iC_SubjectCodeHTMLSelect is too small",
          typeid(*this).name());
		}
	if(rRequest.text[0])
		pColumn->addText(rRequest.text);
	}  // -- iC_SubjectHTMLSelect --
	if ((pColumn = columns[iC_DayOfMonth]) != NULL)
	{  // -- iC_DayOfMonth --
	 addLongToColumn (pColumn, rRequest.date.day, true) ;
	}  // -- iC_DayOfMonth --
	if ((pColumn = columns[iC_RequestNo]) != NULL)
	{  // -- iC_RequestNo --
	 // To be written...
	}  // -- iC_RequestNo --
// 02.dec.2002 - end
  judgesAndResolutions (rRequest, law, lawFound,
                        columns [iC_JudgeAndLawNo],
				columns [iC_RemarkAreal]) ;  // 27.oct.2000
 }  // requestColumns :: makeColumns

requestColumns :: requestColumns (const char * formatFN,
                                  TWindow * parent,
                                  TRequest & rRequest,
                           const JudgeNameRoll* const pJudges,  // 02.dec.2002
                           const SubjectCodeRoll* pSubjectCodes   // 02.dec.2002
                                 ) :
                     requestColumnsEnum (),
                     columnsArr (formatFN, parent, i_columns,
                                 1 + OFFSET_INPUT_DIARY_CIPHER)
 {
  makeColumns (rRequest, pJudges, pSubjectCodes) ;
  defineMaxRows () ;
 }  // requestColumns :: requestColumns
