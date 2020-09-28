#include "cac.h"

# ifdef WITH_STARS
# error WITH_STARS must not be defined here
# endif
# define WITH_STARS 1

timeBookEnum :: timeBookEnum ()
 {
 }  // timeBookEnum :: timeBookEnum

void timeBookColumns :: addMemberToMembership (const char * ucn,
                                               char * buff,
                                               size_t buffLen,
                                               column * pColumn,
                                               const char * prefix,
                                               const char * speakerUCN
                                              )
 {
  if (! (buff && buffLen))
    return ;
  if (strIsBlank (ucn) || ucnPresent (ucn) == false)
    return ;
  if (! citizenUCNAndUCNTypeToName (ucn, UCN_CITIZEN_UCN, buff, buffLen,
                                    true, true, false))
   {
    pColumn -> addText (Q3_STRING) ;
    return ;
   }

  const size_t tmpNSz = strlen (buff) + 1 ;
  char * tmpNames = new char [tmpNSz] ;

  scSprintf (tmpNSz, tmpNames, "%s", buff) ;

  const char * pref0 = prefix ;
  const char * suff = "" ;

  if (! strcmp (prefix, JUDGE_STRING))
    pref0 = "" ;
  if (speakerUCN)
    if (! strcmp (ucn, speakerUCN))
     {
      pref0 = "" ;
      suff = prefix ;
     }

  const char * pref1 = "" ;

   {
    char post = '\0' ;
     {
      TUser user ;

      scSprintf (sizeof (user.ucn), user.ucn, "%s", ucn) ;
      if (user.Try ())
        post = user.post ;
     }
    if (post == POST_APPRENTICE)
      pref1 = "мс" ;
   }
   {
    ostrstream oss (buff, buffLen) ;

    if (pref0 [0])
      oss << pref0 << " " ;
    if (pref1 [0])
      oss << pref1 << " " ;
    oss << tmpNames ;
    if (suff [0])
      oss << " - " << suff ;
    oss << ends ;
   }
  buff [buffLen - 1] = '\0' ;
  delete [] tmpNames ;
  pColumn -> addText (buff) ;
 }  // timeBookColumns :: addMemberToMembership

void timeBookColumns :: sidesAndInvolvements (const TSession &,
							    TLawsuit & lawsuit,
							    char * buff, size_t buffL)
 {
  if (! (buff && buffL))
    return ;
  // 27.jan.2005 - begin
  // Because we add rows
  // into a number of columns several times using a loop,
  // it is necesary to take measures against overlapping of
  // contents of columns.
  TStringArray subjInvolvemItems;
  TStringArray sidesItems;
   {
    // Enclosers must be in a block in order to add their closing tags
    // to the arrays prior to moving of contents (after the block)
    ColumnXmlEncloser subjInvolvemEncloser(columns[iC_SubjInvolvem],
      iC_SubjInvolvem + bOC, &subjInvolvemItems);
    ColumnXmlEncloser sidesEncloser(columns[iC_Sides], iC_Sides + bOC,
      &sidesItems);  // 27.jan.2005 - end

    if (columns [iC_SubjInvolvem])
     {  // -- iC_SubjInvolvem --
	ostrstream (buff, buffL) << lawsuit.Code() << ends ;
      columns [iC_SubjInvolvem] -> addText (buff) ;
     }  // -- iC_SubjInvolvem --
    if (columns [iC_Sides])
     {  // -- iC_Sides, iC_SubjInvolvem --
      if (columns [iC_SubjInvolvem])
        addEmptyRowsIfDiffer (columns [iC_SubjInvolvem],
                              columns [iC_Sides]) ;
       {  // -- -1 --
        if(DatedContainer_(lawsuit.key, lawsuit.kind, lawsuit.date).FilledOK())
         {  // -- 0 --
          CharPtr criteria (1024) ;

           {
            TSideWind tmp ;

            tmp.key = lawsuit.key ;
            tmp.kind = lawsuit.kind ;
            tmp.date = lawsuit.date ;
            tmp.FloatCriteria (criteria) ;
           }
           {  // -- 1 --
            TQuery q (criteria) ;
            TGArray <TSideWind> suitorsArr ;
            TGArray <TSideWind> defendantsArr ;

            while (q.Read ())
             {  // -- 2 --  load sides
              TSideWind s ;

              s << q ;
              if (lawsuit.Suitor (s.involvement))
               {
                TSideWind * pS = new TSideWind ;

                if (pS)
                 {
                  (* pS) << s ;
                  suitorsArr.Add (pS) ;
                 }
               }
              else
                if (lawsuit.Defendant (s.involvement))
                 {
                  TSideWind * pS = new TSideWind ;

                  if (pS)
                   {
                    (* pS) << s ;
                    defendantsArr.Add (pS) ;
                   }
                 }
             }  // -- 2 --
             {  // -- 4 --  add text to columns
              int i ;

              for (i = 0 ; i < suitorsArr.Count () ; i ++)
               {  // -- 6 --
                if (ucnAndUcnTypeToName (suitorsArr [i] -> ucn,
                                         suitorsArr [i] -> ucnType,
                                         buff, buffL))
                 {
                  columns [iC_Sides] -> addText (buff) ;
                  if (columns [iC_SubjInvolvem])
                   {
                    if (involvementToString (suitorsArr [i] -> involvement,
                                             buff, buffL))
                    columns [iC_SubjInvolvem] -> addText (buff) ;
                    addEmptyRowsIfDiffer (columns [iC_SubjInvolvem],
                                          columns [iC_Sides]) ;
                   }
                 }
               }  // -- 6 --
              for (i = 0 ; i < defendantsArr.Count () ; i ++)
               {  // -- 7 --
                if (ucnAndUcnTypeToName (defendantsArr [i] -> ucn,
                                         defendantsArr [i] -> ucnType,
                                         buff, buffL))
                 {
                  columns [iC_Sides] -> addText (buff) ;
                  if (columns [iC_SubjInvolvem])
                   {
                    if (involvementToString (defendantsArr [i] ->
                                               involvement,
                                             buff, buffL)) ;
                    columns [iC_SubjInvolvem] -> addText (buff) ;
                    addEmptyRowsIfDiffer (columns [iC_SubjInvolvem],
                                          columns [iC_Sides]) ;
                   }
                 }
               }  // -- 7 --
             }  // -- 4 --  add text to columns
           }  // -- 1 --
         }  // -- 0 --
       }  // -- -1 --
     }  // -- iC_Sides, iC_SubjInvolvem --
   }  // 27.jan.2005 - begin
  moveArrayContents(subjInvolvemItems);
  moveArrayContents(sidesItems);  // 27.jan.2005 - end
 } // timeBookColumns :: sidesAndInvolvements

void timeBookColumns ::
       defaultDateReturnedProcessing (column * const pColumn,
						  const bool abolishedProgress,
                               const CDate * const pDateReturnedAbolish,
                                      char * const buff,
                                      const size_t buffL)
 {  // 24.sep.2001
  if (pColumn && buff && buffL)
   {
    if (abolishedProgress)
     {
      if (pDateReturnedAbolish)
        CDateToString (* pDateReturnedAbolish, buff, buffL) ;
      else
        scSprintf (buffL, buff, "?") ;
     }
    else
    {
       CDateToString (sess.returned, buff, buffL) ;
    }
    pColumn -> addText (buff) ;
   }
 }  // timeBookColumns :: defaultDateReturnedProcessing

# if INSTANCE
#   if APPEAL
bool timeBookColumns :: defaultDecDateAndHowCondition ()
 {  // 24.sep.2001
  return strchr (RESULT_M_COL_FINISHEDS, sess.result) ||
         resMustBeTreatAsDecision (sess.result) ;
 }  // timeBookColumns :: defaultDecDateAndHowCondition

bool timeBookColumns :: firstManyDecCase (const TLawsuit &)
 {  // 24.sep.2001
  return false ;
 }  // timeBookColumns :: firstManyDecCase

#   else  // of APPEAL
bool timeBookColumns :: defaultDecDateAndHowCondition ()
 {  // 24.sep.2001
  return (strchr (RESULT_M_COL_FINISHEDS, sess.result) ||
	    resMustBeTreatAsDecision (sess.result) ||
	    (
		(sess.key.type == TYPE_FIRM_LAW) &&
#if COURT_TYPE == COURT_AREAL
		// 2010:113 LPR: areal only
		(! strchr (RESULT_FIRM_CANCELEDS, sess.result)) &&
		(RESULT_LENDERS_MEETING != sess.result) &&
#endif  // AREAL
		(! strchr (RESULT_M_COL_NOT_FINISHEDS, sess.result))
	    )
	   ) ;
 }  // timeBookColumns :: defaultDecDateAndHowCondition

bool timeBookColumns :: firstManyDecCase (const TLawsuit & lawsuit)
 {  // 24.sep.2001
  bool res = false ;

  if (defaultDecDateAndHowCondition () == false)
    if (strchr (KIND_1ST_LAWSUITS, lawsuit.kind))
	if (
#if COURT_TYPE == COURT_AREAL   // 20070614
	  lawsuit.key.type == TYPE_TRADE_LAW ||
#endif  // AREAL
	  lawsuit.key.type == TYPE_CITIZEN_LAW)
	  if (sess.result == RESULT_PUBLIC_DECISION)
	    res = true ;
#if COURT_TYPE == COURT_AREAL
	  if (sess.result == RESULT_LENDERS_MEETING || sess.result == RESULT_INCONSISTENCE)
	    res = true ;
#endif  // AREAL	    
  return res ;
 }  // timeBookColumns :: firstManyDecCase

#   endif  // of APPEAL
# else  // of INSTANCE
bool timeBookColumns :: defaultDecDateAndHowCondition ()
 {  // 24.sep.2001
  return strchr (RESULT_M_COL_FINISHEDS, sess.result) ||
                (sess.result == RESULT_SESS_DEC_TEMP_MEASURES) ||  // 20060310
                (sess.result == RESULT_BOZA_LIKE_0)            ||  // 20070208 REGIONAL COURT ONLY
         resMustBeTreatAsDecision (sess.result) ;
 }  // timeBookColumns :: defaultDecDateAndHowCondition

bool timeBookColumns :: firstManyDecCase (const TLawsuit &)
 {  // 24.sep.2001
  return false ;
 }  // timeBookColumns :: firstManyDecCase

# endif  // of INSTANCE

void timeBookColumns::moveArrayContents(TStringArray& src)
{  // 27.jan.2005
  if(pItems)
  {
    move_string_array_contents(pItems, &src);
  }
}  // closedSessBookColumns::moveArrayContents

void timeBookColumns :: makeColumns (TSession & sess,
                                     TLawsuit & lawsuit,
                                     timeBookHeap & tBH,
                                     accusedsDistribution & accDistrHeap,
                                     size_t sNo,
                                     const collCompBegDEndDBegN * pCBDEDBN,
                                     const bool allFmtFileColumns,
                                     const CDate * pDateAbolish,
                                     const CDate * const pDateReturnedAbolish,
						 const int
# if INSTANCE
#   if APPEAL  // 17.jan.2002
#   else // of APPEAL  // 17.jan.2002
                                             additionalRowsAfterSess
#   endif // of APPEAL  // 17.jan.2002
# endif  // of INSTANCE
                                     , const TSession * pAbolishingSess

                                     )
 {
  const size_t buffSz = MAKE_COLUMNS_BUFF_SIZE ;
  const size_t buffL = buffSz - 1 ;
  char * buff = new char [buffSz] ;
  long int notMajorDefends = 0 ;
  accusedsDistribution accDistr ;
  timeBookHeap tBH2nd ;
  TSession nextSess ;
  bool nextSessionOccured = thereIsNextSession (sess, nextSess
								  , KIND_CLOSED_SESSIONS
                                                                    ) ;
  column * pColumn ;  // To make looking of the program text more easy
  const bool abolishedProgress =
    pDateAbolish && ! pDateAbolish -> Empty () ;
  const bool countThisSess = countThisSession (sess, lawsuit)
				     && ! abolishedProgress ;
		   ;
  const bool countThisSessNF = countThisSession (sess, lawsuit, true) ;
# if INSTANCE
  const bool countThisSessResFin = countThisSess &&
					     strchr (RESULT_FINALIZEDS,
							 sess.result) ;
# endif
  bool countDefendants = true ;           // Печат на срочна книга 13116

   {
    TSubject subj ;

    subj.subject = lawsuit.subject ;
    subj.type = lawsuit.key.type ;
    if (subj.Get () &&
	  strchr (subj.Attrib(lawsuit.kind), ATTRIB_DO_NOT_COUNT_DEFENDANTS))
	countDefendants = false ;
   }

  if (! columns)
    return ;
  if (! buff)
    return ;
  buff [buffSz - 1] = '\0' ;
// 08.sep.1999 - begin
// because of allFmtFileColumns the body of the function was
// re-arranged (3 groups:
//                1: iC_Defendants, iC_DecisionsJudgements,
//                   iC_DecisionsJudgementsCatchOth
//                   iC_DecisionsJudgements_TB_NF_PL // 20060331 is added
//                   (with heaping, but there is an argument of
//                    defendantsAndJudgements,
//                    that has allFmtFileColumns dependence (because of
//                    working speed)) ;
//                2: iC_.... columns and iCF_... columns without
//                   heaping ;
//                3: iCF_... columns with heaping.
//             )

  if((pColumn = columns[iC_Nature]) != NULL)
  {  // -- iC_Nature 20060320
     ColumnXmlEncloser encloser(pColumn, iC_Nature + bOC, pItems);
     char tmp[64];
     kind_to_alias (lawsuit.kind, tmp, sizeof (tmp)) ;
     pColumn->addText(tmp);
  }  // -- iC_Nature
  if ((pColumn = columns [iC_GivingAndReturnDate]) != NULL)
   {  // -- iC_GivingAndReturnDate --  // Moved here at 03.aug.2001.
	  // The old processing place is marked
	  // with -- iC_GivingAndReturnDate --
	  // 24.sep.2001 changed
	  //
	  // Moved before the call of defendantsAndJudgements on 04.feb.2005
    //defaultDateReturnedProcessing (pColumn, abolishedProgress,
    //                               pDateReturnedAbolish, buff, buffL) ;
   }  // -- iC_GivingAndReturnDate --

  // CHECK_THIS
  {  // -- iC_Defendants, iC_DecisionsJudgements,
	//                  iC_DecisionsJudgementsCatchOth --
	//                  iC_DecisionsJudgements_TB_NF_PL  // 20060331 is added
      // An old varianth of this processing -
      // in VersionArchives\2365\_appname.j --> _tbclmns.cpp

    int defendsCipher = -1;
    column* defendsColumn = NULL;
    int decisionsJudgementsCipher = -1;
    column* decisionsJudgementsColumn = NULL;

    SUPPRESS_UNUSED(decisionsJudgementsColumn);

    if(allFmtFileColumns)    // При печат на срочна книга allFmtFileColumns = ?
    {
	if(columns[iC_Defendants])                    // Печат на срочна книга 13116
	{
	  defendsColumn = columns[iC_Defendants];
	  defendsCipher = iC_Defendants;
      }
      else
      {
        defendsColumn = columns[iC_DefendantsWithAddr];
        if(defendsColumn)
        {
	    defendsCipher = iC_DefendantsWithAddr;
        }
      }
    }


    if(columns[iC_DecisionsJudgementsCatchOth])
    {
	decisionsJudgementsColumn = columns[iC_DecisionsJudgementsCatchOth];
	decisionsJudgementsCipher = iC_DecisionsJudgementsCatchOth;
    }
    else
    {
	if(columns[iC_DecisionsJudgements_TB_NF_PL])  // 20060331
	{
	  decisionsJudgementsColumn = columns[iC_DecisionsJudgements_TB_NF_PL];
	  decisionsJudgementsCipher = iC_DecisionsJudgements_TB_NF_PL;
	}
	else
	{
	  decisionsJudgementsColumn = columns[iC_DecisionsJudgements];
	  if(decisionsJudgementsColumn)
	  {
	    decisionsJudgementsCipher = iC_DecisionsJudgements;
	  }
	}
    }


    const bool wantDefSettl =                                             // Печат на срочна книга 13116
      defendsColumn &&
      (void *) defendsColumn ==
        (void *) (columns [iC_DefendantsWithAddr]) ;

    // Because defendantsAndJudgements adds rows
    // into a number of columns several times using a loop,
    // it is necesary to take measures against overlapping of
    // contents of columns.
    TStringArray defendantsItems;
    TStringArray decisionsJudgementsItems;
    TStringArray dateReturnedItems;

    {
      // Enclosers must be in a block in order to add their closing tags
      // to the arrays prior to moving of contents (after the block)
      ColumnXmlEncloser defendantsEncloser(defendsColumn, defendsCipher + bOC,
        &defendantsItems);
	ColumnXmlEncloser decisionsJudgementsEncloser(decisionsJudgementsColumn,
	  decisionsJudgementsCipher + bOC, &decisionsJudgementsItems);
	ColumnXmlEncloser dateReturnedEncloser(columns[iC_GivingAndReturnDate],
	  iC_GivingAndReturnDate + bOC, &dateReturnedItems);

	if(columns[iC_GivingAndReturnDate])  // Moved here on 04.feb.2004
	{
	  defaultDateReturnedProcessing(columns[iC_GivingAndReturnDate],
	    abolishedProgress, pDateReturnedAbolish, buff, buffL);
	}
	// 20060713 --> да се забрани само извеждане на текста на наказанието

	// 20060713 // iC_DecisionsJudgements    13117
	// 20060713 if ( (! columns[iC_DecisionsJudgements])
	// 20060713     || // 20060330 -->
	// 20060713     ((columns[iC_DecisionsJudgements])&&(! strchr(TB_SUPPRESS_SESS_RESULTS, sess.result))
	// 20060713      )// 20060330  <--
	// 20060713    )
	// 20060719
	if( (! columns[iC_DecisionsJudgements])     // 13117
	    ||
	    ((columns[iC_DecisionsJudgements])&&(! strchr(TB_SUPPRESS_SESS_RESULTS, sess.result)))
	  )
	    defendantsAndJudgements(sess, lawsuit, & tBH2nd, accDistr,
		 notMajorDefends, defendsColumn, decisionsJudgementsColumn,
		 columns[iC_GivingAndReturnDate], true, wantDefSettl, pDateAbolish,
		 pAbolishingSess);
	else                                        // 13153 (TB, 'p', II inst), ....
	    defendantsAndJudgements(sess, lawsuit, & tBH2nd, accDistr,
		 notMajorDefends, defendsColumn, decisionsJudgementsColumn,
		 columns[iC_GivingAndReturnDate], true, wantDefSettl, pDateAbolish,
		 pAbolishingSess, false);
    }
    moveArrayContents(defendantsItems);
    moveArrayContents(decisionsJudgementsItems);
    moveArrayContents(dateReturnedItems);
  }  // -- iC_Defendants, iC_DecisionsJudgements
     //                   iC_DecisionsJudgementsCatchOth --
     //                   iC_DecisionsJudgements_TB_NF_PL  // 20060331


  if (allFmtFileColumns)  // ============================    // Печат на срочна книга 13116   стар формат
   {
    if ((pColumn = columns [iC_SerialNo]) != NULL)
     {  // -- iC_SerialNo --
      ColumnXmlEncloser encloser(pColumn, iC_SerialNo + bOC, pItems);  // 27.jan.2005

      ostrstream (buff, buffL) << sNo << ends ;
      pColumn -> addText (buff) ;
      if (strchr (KIND_CLOSED_SESSIONS, sess.kind))
        pColumn -> addText ("РЗ") ;
      if(quick_law_source_currently_quick(lawsuit.source))
        pColumn -> addText ("Б") ;
      if (strchr (KIND_PSEUDO_SESSIONS, sess.kind))
        pColumn -> addText ("#") ;
     }  // -- iC_SerialNo --
    if ((pColumn = columns [iC_NoAndMakeDate]) != NULL)  // (allFmtFileColumns)
     {  // -- iC_NoAndMakeDate --
      ColumnXmlEncloser encloser(pColumn, iC_NoAndMakeDate + bOC,
        pItems);  // 27.jan.2005

      if (pCBDEDBN)  // 08.feb.2002 - begin
       {
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
        if (sess.key.type == TYPE_PUNISHMENT_LAW &&
            pCBDEDBN -> all2ndPuniLaws)
         {
          kindToString (lawsuit.kind, buff, buffL) ;
          pColumn -> addText (buff) ;
         }
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE
       }  // 08.feb.2002 - end
      ostrstream (buff, buffL) << sess.key.no << "/"
                               << sess.key.year << ends ;
      pColumn -> addText (buff) ;
      CDateToString (lawsuit.date, buff, buffL) ;
      pColumn -> addText (buff) ;
      if (strchr (RESULT_FINALIZEDS, sess.result) ||
          ! lawsuit.finished.Empty ()
         )
        if ((sess.date >= lawsuit.finished
             || (sess.key.type == TYPE_PUNISHMENT_LAW &&
                 sess.end >= lawsuit.finished)
            )
            && ! abolishedProgress)
            // Change any text with the function below
            addDateFinishedToColumn (pColumn, buff, buffL, lawsuit) ;
     }  // -- iC_NoAndMakeDate --
    if ((pColumn = columns [iC_Kind]) != NULL)           // (allFmtFileColumns)
     {  // -- iC_Kind --
      ColumnXmlEncloser encloser(pColumn, iC_Kind + bOC, pItems);  // 27.jan.2005

      kindToString (lawsuit.kind, buff, buffL) ;
      pColumn -> addText (buff) ;
     }  // -- iC_Kind --
    if ((pColumn = columns [iC_Subject]) != NULL)        // (allFmtFileColumns)
     {  // -- iC_Subject --
      ColumnXmlEncloser encloser(pColumn, iC_Subject + bOC, pItems);  // 27.jan.2005

      subjectToString (lawsuit.key.type, lawsuit.subject, buff, buffL) ;
      pColumn -> addText (buff) ;
     }  // -- iC_Subject --
    if ((pColumn = columns [iC_CancLeftStop_reason]) != NULL)
     {  // -- iC_CancLeftStop_reason --
      ColumnXmlEncloser encloser(pColumn, iC_CancLeftStop_reason + bOC,
        pItems);  // 27.jan.2005
      // 20060714 --> T14 Да не се извежда дата на следващo ОЗ, ако то е със статус "Отменено" или "Отложено без ново насрочване"
      if (nextSessionOccured)
      {  if ( (nextSess.Status() == RESULT_ABORTED) || (nextSess.Status() == RESULT_UNFIXED) )
             nextSessionOccured = false;
      }
      // 20060714 <--
      addCancLeftStopReasonToColumn (pColumn, sess, lawsuit, false,
                                     nextSessionOccured ? & nextSess : NULL) ;
     }  // -- iC_CancLeftStop_reason --

    // 20060405 -> TB_NF_PL
    if ((pColumn = columns [iC_CancLeftStop_TB_NF_PL]) != NULL)
     {
      ColumnXmlEncloser encloser(pColumn, iC_CancLeftStop_TB_NF_PL + bOC,
        pItems);

      // 20060714 --> T14 Да не се извежда дата на следващo ОЗ, ако то е със статус "Отменено" или "Отложено без ново насрочване"
      if (nextSessionOccured)
      {  if ( (nextSess.Status() == RESULT_ABORTED) || (nextSess.Status() == RESULT_UNFIXED) )
               nextSessionOccured = false;
      }// 20060714 <--

      if ( ! strchr(TB_SUPPRESS_SESS_RESULTS, sess.result) )
        addCancLeftStopReasonToColumn (pColumn, sess, lawsuit, false,
                                     nextSessionOccured ? & nextSess : NULL) ;
     }
    // 20060405 <-

    if ((pColumn = columns [iC_Membership]) != NULL)     // (allFmtFileColumns=true) // Печат на срочна книга /  стар формат
     {  // -- iC_Membership --
      ColumnXmlEncloser encloser(pColumn, iC_Membership + bOC, pItems);  // 27.jan.2005

      addMemberToMembership (sess.president, buff, buffL, pColumn,
                             JUDGE_STRING, sess.judge) ;
      addMemberToMembership (sess.judgeX, buff, buffL, pColumn,
                             JUDGE_STRING, sess.judge) ;
      addMemberToMembership (sess.reJudgeX, buff, buffL, pColumn,
                             JUDGE_STRING, sess.judge) ;
      addMemberToMembership (sess.juror, buff, buffL, pColumn,
                             JUROR_STRING) ;
      addMemberToMembership (sess.reJuror, buff, buffL, pColumn,
                             REJUROR_STRING) ;
      addMemberToMembership (sess.jurorX, buff, buffL, pColumn,
                             JURORX_STRING) ;
      addMemberToMembership (sess.secretar, buff, buffL, pColumn,
                             SECRETAR_STRING) ;
      addMemberToMembership (sess.prosecutor, buff, buffL, pColumn,
                             PROSECUTOR_STRING) ;
     }  // -- iC_Membership --                           // (allFmtFileColumns)
    if ((pColumn = columns [iC_Blank]) != NULL)
     {  // -- iC_Blank --
      ColumnXmlEncloser encloser(pColumn, iC_Blank + bOC, pItems);  // 27.jan.2005

     }  // -- iC_Blank --
    if (columns [iC_Sides] || columns [iC_SubjInvolvem]) // (allFmtFileColumns)
     {  // -- iC_Sides, iC_SubjInvolvem --
      // Enclosers are inside the method.
      sidesAndInvolvements (sess, lawsuit, buff, buffL) ;
     }  // -- iC_Sides, iC_SubjInvolvem --
    if ((pColumn = columns [iC_MembershipForMColl]) != NULL)  // (allFmtFileColumns)
     {  // -- iC_MembershipForMColl --
      ColumnXmlEncloser encloser(pColumn, iC_MembershipForMColl + bOC,
        pItems);  // 27.jan.2005

      addMemberToMembership (sess.president, buff, buffL, pColumn,
                             JUDGE_STRING, sess.judge) ;
      addMemberToMembership (sess.judgeX, buff, buffL, pColumn,
                             JUDGE_STRING, sess.judge) ;
      addMemberToMembership (sess.reJudgeX, buff, buffL, pColumn,
                             JUDGE_STRING, sess.judge) ;
      addMemberToMembership (sess.secretar, buff, buffL, pColumn,
                             SECRETAR_STRING) ;
      addMemberToMembership (sess.prosecutor, buff, buffL, pColumn,
                             PROSECUTOR_STRING) ;
      addMemberToMembership (sess.juror, buff, buffL, pColumn,
                             JUROR_STRING) ;
      addMemberToMembership (sess.reJuror, buff, buffL, pColumn,
                             REJUROR_STRING) ;
      addMemberToMembership (sess.jurorX, buff, buffL, pColumn,
                             JURORX_STRING) ;
     }  // -- iC_MembershipForMColl --



    if ((pColumn = columns [iC_Result_TB_NF_AL]) != NULL) // 20060406 (TB; snippet allFmtFileColumns)
     {  // -- iC_Result_TB_NF_AL --                       // copied from 13130
      bool sessTxtAdded = false;
      ColumnXmlEncloser encloser(pColumn, iC_Result_TB_NF_AL + bOC, pItems);
# if INSTANCE
#   if APPEAL           // APPEAL  court ->
      if (defaultDecDateAndHowCondition ())
      {  // -- RESULT_M_COL_FINISHEDS --
         char tmpSessRes = '\0' ;

         if (! strchr (RESULT_FINALIZEDS, sess.result) ||
             strchr (KIND_CLOSED_SESSIONS, sess.kind) ||
             sess.key.type != TYPE_PUNISHMENT_LAW)

                 tmpSessRes = sess.result ;

         if (  (! strIsBlank (sess.text)) && (!sessTxtAdded))
         {
                 pColumn -> addText (sess.text) ;
                 sessTxtAdded = true;
         }
         addDecisionToColumn ( lawsuit.key, sess.date, sess.kind,
                               pColumn, NULL, tmpSessRes) ;
      }  // -- RESULT_M_COL_FINISHEDS --

      if (abolishedProgress)add_abolishing_to_column (pColumn, (* pDateAbolish),
                                                             pAbolishingSess ) ;
                        // APPEAL court <-
#   else
				// AREAL, ADMIN courts ->
      if (defaultDecDateAndHowCondition ())
      {  // -- RESULT_M_COL_FINISHEDS --
         char tmpSessRes = '\0' ;

         if (  ! strchr (RESULT_FINALIZEDS, sess.result) ||
               strchr (KIND_CLOSED_SESSIONS, sess.kind) ||
               sess.key.type != TYPE_PUNISHMENT_LAW ||
               (sess.key.type == TYPE_FIRM_LAW &&
                   ! strchr (RESULT_FIRM_CANCELEDS, sess.result)
               ) ||
               lawsuit.kind == KIND_1ST_ADMIN_PUNISHMENT_LAW ||
               lawsuit.kind == KIND_2ND_ADMIN_PUNISHMENT_LAW
            )
               tmpSessRes = sess.result ;
         else
         {
           if (strchr (KIND_1ST_LAWSUITS, lawsuit.kind))
              if (lawsuit.key.type == TYPE_FIRM_LAW)
                 if (strchr (RESULT_FIRM_CL_S_BK_EXCLUDEDS, sess.result))
                     addResultToColumn (sess.result, pColumn) ;
         }

         if ((! strIsBlank (sess.text)) && (! sessTxtAdded))
         {   pColumn -> addText (sess.text) ;
		 sessTxtAdded = true;
	   }

	   addDecisionToColumn (lawsuit.key, sess.date,
					 sess.kind,  // 15.jan.2001
					 pColumn, NULL, tmpSessRes) ;
	}  // -- RESULT_M_COL_FINISHEDS --
	else
	{
	  if (firstManyDecCase (lawsuit))
	  {         many_end_docs_and_ret_dates (
				     columns [iC_Result_TB_NF_AL],
				     columns [iC_GivingAndReturnDateManyDec],
				     sess, lawsuit, additionalRowsAfterSess) ;
	  }
	  // Attention! This function must be called BEFORE the
	  // processing of columns [iC_GivingAndReturnDateManyDec]
	  // as alone column.
	}
	if (abolishedProgress) add_abolishing_to_column (pColumn, (* pDateAbolish),
										 pAbolishingSess ) ;

	if (!sessTxtAdded) { } // 2011:252 LPR: clear warning
#   endif  // of APPEAL // AREAL court <-
# else  // of INSTANCE
                        // REGIONAL court ->
      if (defaultDecDateAndHowCondition ())
      {  // -- RESULT_M_COL_FINISHEDS --
         char tmpSessRes = '\0' ;

         if (! strchr (RESULT_FINALIZEDS, sess.result) ||
             strchr (KIND_CLOSED_SESSIONS, sess.kind) ||
             sess.key.type != TYPE_PUNISHMENT_LAW ||
             lawsuit.kind == KIND_ADMIN_PUNISHMENT_LAW
             )  tmpSessRes = sess.result ;

         if ((! strIsBlank (sess.text)) && (! sessTxtAdded))
         {    pColumn -> addText (sess.text) ;
              sessTxtAdded = true;
         }

         addDecisionToColumn (lawsuit.key, sess.date,
                              sess.kind,  // 15.jan.2001
                              pColumn, NULL, tmpSessRes) ;
      }  // -- RESULT_M_COL_FINISHEDS --
      if (abolishedProgress) add_abolishing_to_column (pColumn, (* pDateAbolish),
                                                             pAbolishingSess ) ;
# endif                 // REGIONAL court <-




      // process unfinished session  // copied from 13131 ->
      if ((strchr (RESULT_M_COL_NOT_FINISHEDS, sess.result)
# if INSTANCE           // APPEAL and AREAL court ->
										||
#if COURT_TYPE == COURT_AREAL
           // 2010:113 LPR: areal only
	     RESULT_LENDERS_MEETING == sess.result ||
#endif  // AREAL
	     RESULT_DECISION_ALLOW_SPLIT == sess.result ||
	     RESULT_SPLIT_PROTOCOL == sess.result
# endif                 // all courts ->
	    )
          && ! resMustBeTreatAsDecision (sess.result)
         )
      {
# if INSTANCE          // APPEAL and AREAL  ->
	  if (
#if COURT_TYPE == COURT_AREAL
	      // 2010:113 LPR: areal only
		RESULT_LENDERS_MEETING != sess.result &&
#endif  // AREAL
		RESULT_LEFT_1_PHASE_DECISION != sess.result
#   if APPEAL          // 05.feb.2001 - begin
			     // APPEAL  - nothing
#   else               // AREAL   ->
		&& (strchr (KIND_1ST_LAWSUITS, lawsuit.kind) &&
		    lawsuit.key.type == TYPE_FIRM_LAW &&
		    strchr (RESULT_FIRM_CL_S_BK_EXCLUDEDS, sess.result)
		   ) == false
#   endif      // 05.feb.2001 - begin
	     )
# else                 // REGIONAL ->
	  if (RESULT_DECISION_ALLOW_SPLIT != sess.result &&
		RESULT_SPLIT_PROTOCOL != sess.result)
# endif
	  {}

			    // ALL COURTS ->
//           if ( ! sessTxtAdded )
//           { addResultToColumn (sess.result, pColumn) ; }//sessTxtAdded = true;}





//        if ((! strIsBlank (sess.text)) && (! sessTxtAdded))
//        {
//          pColumn -> addText (sess.text) ;
//          //sessTxtAdded = true;
//        }



# if INSTANCE         // APPEAL and AREAL ->
	  if (
#if COURT_TYPE == COURT_AREAL
		// 2010:113 LPR: areal only
		RESULT_LENDERS_MEETING == sess.result ||
#endif  // AREAL
		RESULT_LEFT_1_PHASE_DECISION == sess.result // 25.feb.2000
	     )
# else                // REGIONAL ->
	  if (RESULT_DECISION_ALLOW_SPLIT == sess.result
		||
		RESULT_SPLIT_PROTOCOL == sess.result
		||
		sess.result == RESULT_SESS_LEFT_TEMP_MEASURES  // 20060310
		)
# endif
	    addDecisionToColumn (sess.key, sess.date,
                               sess.kind,  // 15.jan.2001
                               pColumn, NULL, sess.result) ;


        // 20060714 --> T14 Да не се извежда дата на следващo ОЗ, ако то е със статус "Отменено" или "Отложено без ново насрочване"
        if (nextSessionOccured)
        {  if ( (nextSess.Status() == RESULT_ABORTED) || (nextSess.Status() == RESULT_UNFIXED) )
                nextSessionOccured = false;
        }
        // 20060714 <--
        addCancLeftStopReasonToColumn (pColumn, sess, lawsuit, false,
                                     nextSessionOccured ? & nextSess : NULL) ;
      }
     }  // -- iC_Result_TB_NF_AL --

////
                                                                 // 13130
    if ((pColumn = columns [iC_DecisionDateAndHow]) != NULL)     // (TB; snippet allFmtFileColumns)
     {  // -- iC_DecisionDateAndHow --
        // 24.sep.2001
        ColumnXmlEncloser encloser(pColumn, iC_DecisionDateAndHow + bOC, pItems);// 27.jan.2005
# if INSTANCE
#   if APPEAL           // APPEAL  court ->
      if (defaultDecDateAndHowCondition ())
      {  // -- RESULT_M_COL_FINISHEDS --
         char tmpSessRes = '\0' ;

         if (! strchr (RESULT_FINALIZEDS, sess.result) ||
             strchr (KIND_CLOSED_SESSIONS, sess.kind) ||
             sess.key.type != TYPE_PUNISHMENT_LAW)

                 tmpSessRes = sess.result ;

         if (! strIsBlank (sess.text))
                 pColumn -> addText (sess.text) ;

         addDecisionToColumn ( lawsuit.key, sess.date,
                               sess.kind,  // 15.jan.2001
                               pColumn, NULL, tmpSessRes) ;
      }  // -- RESULT_M_COL_FINISHEDS --

      if (abolishedProgress)  // 29.may.2001
        add_abolishing_to_column (pColumn, (* pDateAbolish),
                                  pAbolishingSess  // 13.dec.2001
                                 ) ;  // 29.may.2001
                        // APPEAL court <-
#   else
                        // AREAL court ->
      if (defaultDecDateAndHowCondition ())
	{  // -- RESULT_M_COL_FINISHEDS --
         char tmpSessRes = '\0' ;

         if (  ! strchr (RESULT_FINALIZEDS, sess.result) ||
               strchr (KIND_CLOSED_SESSIONS, sess.kind) ||
               sess.key.type != TYPE_PUNISHMENT_LAW ||
               (sess.key.type == TYPE_FIRM_LAW &&
                   ! strchr (RESULT_FIRM_CANCELEDS, sess.result)
               ) ||
               lawsuit.kind == KIND_1ST_ADMIN_PUNISHMENT_LAW ||
               lawsuit.kind == KIND_2ND_ADMIN_PUNISHMENT_LAW
            )
               tmpSessRes = sess.result ;
         else
         {
           if (strchr (KIND_1ST_LAWSUITS, lawsuit.kind))
              if (lawsuit.key.type == TYPE_FIRM_LAW)
                 if (strchr (RESULT_FIRM_CL_S_BK_EXCLUDEDS, sess.result))
                     addResultToColumn (sess.result, pColumn) ;
         }

         if (! strIsBlank (sess.text))
            pColumn -> addText (sess.text) ;

         addDecisionToColumn (lawsuit.key, sess.date,
                               sess.kind,  // 15.jan.2001
                               pColumn, NULL, tmpSessRes) ;
      }  // -- RESULT_M_COL_FINISHEDS --
	else
      {
         if (firstManyDecCase (lawsuit))
         {
                 many_end_docs_and_ret_dates (
                             columns [iC_DecisionDateAndHow],
                             columns [iC_GivingAndReturnDateManyDec],
                             sess, lawsuit, additionalRowsAfterSess) ;
         }
         // Attention! This function must be called BEFORE the
         // processing of columns [iC_GivingAndReturnDateManyDec]
         // as alone column.
      }
      if (abolishedProgress)  // 29.may.2001
        add_abolishing_to_column (pColumn, (* pDateAbolish),
                                  pAbolishingSess  // 13.dec.2001
                                 ) ;  // 29.may.2001
#   endif  // of APPEAL // AREAL court <-
# else  // of INSTANCE
                        // REGIONAL court ->
      if (defaultDecDateAndHowCondition ())   // Specific to court
       {  // -- RESULT_M_COL_FINISHEDS --
        char tmpSessRes = '\0' ;

        if (! strchr (RESULT_FINALIZEDS, sess.result) ||
            strchr (KIND_CLOSED_SESSIONS, sess.kind) ||
            sess.key.type != TYPE_PUNISHMENT_LAW ||
            lawsuit.kind == KIND_ADMIN_PUNISHMENT_LAW
	     )
          tmpSessRes = sess.result ;
        if (! strIsBlank (sess.text))
          pColumn -> addText (sess.text) ;
        addDecisionToColumn (lawsuit.key, sess.date,
                             sess.kind,  // 15.jan.2001
                             pColumn, NULL, tmpSessRes) ;
       }  // -- RESULT_M_COL_FINISHEDS --
      if (abolishedProgress)  // 29.may.2001
        add_abolishing_to_column (pColumn, (* pDateAbolish),
                                  pAbolishingSess  // 13.dec.2001
                                 ) ;  //29.may.2001
# endif  // of INSTANCE // REGIONAL court <-
     }  // -- iC_DecisionDateAndHow --


    if ((pColumn = columns [iC_LeftsStoppedsAndCauses]) != NULL)   // 13131
     {  // -- iC_LeftsCancsAndCauses --
      ColumnXmlEncloser encloser(pColumn, iC_LeftsStoppedsAndCauses + bOC, pItems);  // 27.jan.2005

      if ((strchr (RESULT_M_COL_NOT_FINISHEDS, sess.result)
# if INSTANCE           // APPEAL and AREAL court ->
										||
#if COURT_TYPE == COURT_AREAL
	     // 2010:113 LPR: areal only
	     RESULT_LENDERS_MEETING == sess.result ||
#endif  // AREAL
           RESULT_DECISION_ALLOW_SPLIT == sess.result ||
	     RESULT_SPLIT_PROTOCOL == sess.result // This is included in
								// RESULT_M_COL_NOT_FINISHEDS, but, let it be ...
# endif                 // all courts ->
	    )
          && ! resMustBeTreatAsDecision (sess.result)
         )
      {
# if INSTANCE          // APPEAL and AREAL  ->
	  if (
#if COURT_TYPE == COURT_AREAL
		// 2010:113 LPR: areal only
		RESULT_LENDERS_MEETING != sess.result &&
#endif  // AREAL
		RESULT_LEFT_1_PHASE_DECISION != sess.result
#   if APPEAL          // 05.feb.2001 - begin
                       // APPEAL  - nothing
#   else               // AREAL   ->
            && (strchr (KIND_1ST_LAWSUITS, lawsuit.kind) &&
                lawsuit.key.type == TYPE_FIRM_LAW &&
                strchr (RESULT_FIRM_CL_S_BK_EXCLUDEDS, sess.result)
               ) == false
#   endif      // 05.feb.2001 - begin
           )
# else                 // REGIONAL ->
        if (RESULT_DECISION_ALLOW_SPLIT != sess.result &&
            RESULT_SPLIT_PROTOCOL != sess.result)
# endif

                      // ALL COURTS ->
        addResultToColumn (sess.result, pColumn) ;
        if (! strIsBlank (sess.text))
          pColumn -> addText (sess.text) ;


# if INSTANCE         // APPEAL and AREAL ->
	  if (
#if COURT_TYPE == COURT_AREAL
	      // 2010:113 LPR: areal only
		RESULT_LENDERS_MEETING == sess.result ||
#endif  // AREAL
		RESULT_LEFT_1_PHASE_DECISION == sess.result // 25.feb.2000
	     )
# else                // REGIONAL ->
        if (RESULT_DECISION_ALLOW_SPLIT == sess.result
            ||
            RESULT_SPLIT_PROTOCOL == sess.result
            ||
            sess.result == RESULT_SESS_LEFT_TEMP_MEASURES  // 20060310
            )
# endif
          addDecisionToColumn (sess.key, sess.date,
                               sess.kind,  // 15.jan.2001
                               pColumn, NULL, sess.result) ;

         // 20060714 --> T14 Да не се извежда дата на следващo ОЗ, ако то е със статус "Отменено" или "Отложено без ново насрочване"
         if (nextSessionOccured)
         {  if ( (nextSess.Status() == RESULT_ABORTED) || (nextSess.Status() == RESULT_UNFIXED) )
                nextSessionOccured = false;
         }
         // 20060714 <--
         if (nextSessionOccured)
         {
          addNextSessionToColumn (pColumn, buff, buffL, nextSess) ;
         }
       }
     }  // -- iC_LeftsStoppedsAndCauses --
    // This was the old processing place of -- iC_GivingAndReturnDate --
    if ((pColumn = columns [iC_GivingAndReturnDateManyDec]) != NULL)
     {  // -- iC_GivingAndReturnDateManyDec --
// Attention! This processing must be performed AFTER the processing
// of columns [iC_DecisionDateAndHow] and AFTER the possible call
// of many_end_docs_and_ret_dates!
      ColumnXmlEncloser encloser(pColumn, iC_GivingAndReturnDateManyDec + bOC,
        pItems);  // 27.jan.2005

      if (firstManyDecCase (lawsuit))  // 24.sep.2001
       {  // 24.sep.2001
        if (abolishedProgress)
         {
          if (pDateReturnedAbolish)
            CDateToString (* pDateReturnedAbolish, buff, buffL) ;
          else
            scSprintf (buffL, buff, "?") ;
          pColumn -> addText (buff) ;
         }
       }  // 24.sep.2001 - begin
      else
        defaultDateReturnedProcessing (pColumn, abolishedProgress,
                                       pDateReturnedAbolish, buff,
                                       buffL) ;  // 24.sep.2001 - end
     }  // -- iC_GivingAndReturnDateManyDec --


# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
    if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind))
     {  // -- if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind)) --
      if ((pColumn = columns [iC_1stInstCourtAndOthData]) != NULL)
       {  // -- iC_1stInstCourtAndOthData --
        ColumnXmlEncloser encloser(pColumn, iC_1stInstCourtAndOthData + bOC,
          pItems);  // 27.jan.2005

         {
          char * b1 = new char [buffSz] ;

          if (b1)
           {  // -- if (b1) --
            ostrstream oss (buff, buffL) ;

            b1 [buffSz - 1] = '\0' ;
            if (lawsuit.sourceKind)
             {
              kindToString (lawsuit.sourceKind, b1, buffL) ;
              oss << b1 << " " ;
             }
            // 20070103 -->
            TLawsuit * pLawsuit = new TLawsuit ;
		pLawsuit -> key = lawsuit.key;
            TConnect *c = get_connect(pLawsuit, CONNECT_TYPE_LOWER_INSTANCES);
            if ( c != NULL )
            {
               char tmpKN [SIZE_OF_NAME] ;
               connectKindToString (c -> connectKind , tmpKN, sizeof (tmpKN)) ;
               oss << (lawsuit.sourceKind ? "\n" : "")
                  << " " << tmpKN << " " << c -> connectNo << "/"
                  << c -> connectYear << " " ;
               if (c -> composition)
               {
                  if (use_area_compositions ()) // 20070806
                  {
                      char tmp [32] ;
                      compositionToString (c -> composition, tmp, sizeof (tmp)) ;
                      oss << " " << tmp << " състав " ;
                  }
               }
               if (( c -> ucn ) && ( c -> ucnType ))
		   {
                  TSender sndr ;
                  clearGroupData (sndr) ;

                  senderUCNAndUCNTypeToName (c -> ucn , c -> ucnType,
                                             sndr.name, sizeof (sndr.name) );
                  oss << sndr.name;
               }
            }
            delete c;
            delete pLawsuit;
            // 20070103 <--

            // 20070103  if (lawsuitToSenderName (lawsuit, b1, buffL))
            // 20070103  oss << b1 ;

            oss << ends ;
            pColumn -> addText (buff) ;
            delete [] b1 ;
           }  // -- if (b1) --
         }
        if (lawsuit.source.type == TYPE_RETURNED
            && (lawsuit.key.type == TYPE_PUNISHMENT_LAW ||
                strchr (KIND_1ST_LAWSUITS, lawsuit.kind))
           )
         {

            //ostrstream (buff, buffL)
            //   << "Върнато дело No " << lawsuit.returned.no << "/"
            //   << lawsuit.returned.year << ends ;
            // pColumn -> addText (buff) ;

            // 20070103 -->
            {
                TLawsuit * pLawsuit = new TLawsuit ;
                pLawsuit -> key = lawsuit.key;
                char connType[2]; connType[0]=CONNECT_TYPE_RETURNED_LAWSUIT; connType[1]='\0';
                TConnect *c = get_connect(pLawsuit, connType);
                if ( c != NULL )
                {
                  ostrstream (buff, buffL)
                  << "Върнато дело No " << c -> connectNo << "/"
                                        << c -> connectYear << ends ;
                  pColumn -> addText (buff) ;
                }
                delete c;
                delete pLawsuit;
            }
            // 20070103 <--
        }
        if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind))
          ostrstream (buff, buffL) << lawsuit.text << ends ;
        else
          subjectAndExtraTextToString (lawsuit.kind, lawsuit.subject,
                                       buff, buffL, lawsuit.text,
                                       strchr (KIND_1ST_LAWSUITS,
                                               lawsuit.kind) ?
                                         true : false
                                       ) ;
        pColumn -> addText (buff) ;
       }  // -- iC_1stInstCourtAndOthData --
     }  // -- if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind)) --
    if ((pColumn = columns [iC_NumberOfHoldedsLawNotFinish]) != NULL)
     {  // -- iC_NumberOfHoldedsLawNotFinish --
      ColumnXmlEncloser encloser(pColumn, iC_NumberOfHoldedsLawNotFinish + bOC,
        pItems);  // 27.jan.2005

      // nothing
     }  // -- iC_NumberOfHoldedsLawNotFinish --
#   endif  // of APPEAL

    if ((pColumn = columns [iC_InDocAndRelatedLaws]) != NULL)
     {  // -- iC_InDocAndRelatedLaws --
      ColumnXmlEncloser encloser(pColumn, iC_InDocAndRelatedLaws + bOC,
        pItems);  // 27.jan.2005

 #   if ! APPEAL
      if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind))
 #   endif
        addInDocAndRelatedLawsToColumn (pColumn, lawsuit, buff, buffSz) ;
     }  // -- iC_InDocAndRelatedLaws --

# endif

    if ((pColumn = columns [iC_SubjectWithText]) != NULL)
     {  // -- iC_SubjectWithText --
      ColumnXmlEncloser encloser(pColumn, iC_SubjectWithText + bOC,
	  pItems);  // 27.jan.2005

      subjectAndExtraTextToString (lawsuit.kind, lawsuit.subject,
                                   buff, buffL, lawsuit.text) ;
      pColumn -> addText (buff) ;
     }  // -- iC_SubjectWithText --

    if ((pColumn = columns [iC_ClaimantsVsDefendants]) != NULL)
     {  // -- iC_ClaimantsVsDefendants --
      ColumnXmlEncloser encloser(pColumn, iC_ClaimantsVsDefendants + bOC,
        pItems);  // 27.jan.2005

// 03.sep.2001 - begin
      // ???
      // Looks like this correction is not finished.
      // May pe the pairs variable should be commented
      // and sides should be used instead
      // ???
      lawRCDKeyToSidePair pairs (lawsuit,
                                 true  // 13.mar.2001
                                ) ;
      SessRCDKeyToSidePair sides (sess, & (sess.date), true) ;
// 03.sep.2001 - end

      if (pairs.getPairsCount ())
       {
        unsigned i ;
        unsigned claims = pairs.getClaimantsCount () ;
	  unsigned defs = pairs.getDefendantsCount () ;

        for (i = 0 ; i < claims ; i ++)
         {
	    TSideWind clS ;
          TClaimant claimant ;

          if (pairs.nextPair (& claimant, NULL, & clS))
           {
            if (! defs)
             {
              involvementToString (clS.involvement, buff, buffL) ;
              pColumn -> addText (buff) ;
             }
            pColumn -> addText (claimant.cName) ;
           }
         }  // for (i = 0 ; i < claims ; i ++)
        if (claims && defs)
          pColumn -> addText ("срещу") ;
        pairs.readIndexesToZero () ;
        for (i = 0 ; i < defs ; i ++)
         {
	    TSideWind defS ;  // TSide defS ;  // 03.sep.2001
          TDefendant defendant ;

          if (pairs.nextPair (NULL, & defendant, NULL, & defS))
           {
            if (! claims)
             {
              involvementToString (defS.involvement, buff, buffL) ;
              pColumn -> addText (buff) ;
             }
            pColumn -> addText (defendant.dName) ;
           }
         }  // for (i = 0 ; i < defs ; i ++)
       }  // if (pairs.getPairsCount ())
     }  // -- iC_ClaimantsVsDefendants --

    if ((pColumn = columns [iC_InDocAndSubjectWithText]) != NULL)
     {  // -- iC_InDocAndSubjectWithText --
      ColumnXmlEncloser encloser(pColumn, iC_InDocAndSubjectWithText + bOC,
        pItems);  // 27.jan.2005

      if (lawIsCreatedByInDoc (lawsuit))
       {
        kindToString (lawsuit.sourceKind, buff, buffL) ;
        pColumn -> addText (buff) ;
       }
      subjectAndExtraTextToString (lawsuit.kind, lawsuit.subject,
                                   buff, buffL, lawsuit.text) ;
      pColumn -> addText (buff) ;
     }  // -- iC_InDocAndSubjectWithText --

    if ((pColumn = columns [iC_CancLeftStop_reasonCatchOth]) != NULL)  // 13154
     {  // -- iC_CancLeftStop_reasonCatchOth --
      ColumnXmlEncloser encloser(pColumn, iC_CancLeftStop_reasonCatchOth + bOC,
        pItems);  // 27.jan.2005

        // 20060714 --> T14 Да не се извежда дата на следващo ОЗ, ако то е със статус "Отменено" или "Отложено без ново насрочване"
        if (nextSessionOccured)
        {  if ( (nextSess.Status() == RESULT_ABORTED) || (nextSess.Status() == RESULT_UNFIXED) )
               nextSessionOccured = false;
        }
        // 20060714 <--

        addCancLeftStopReasonToColumn (pColumn, sess, lawsuit, true,
                               nextSessionOccured ? & nextSess : NULL) ;
     }  // -- iC_CancLeftStop_reasonCatchOth --
    if ((pColumn = columns [iC_Claimants]) != NULL)
     {  // -- iC_Claimants --
      ColumnXmlEncloser encloser(pColumn, iC_Claimants + bOC, pItems);  // 27.jan.2005

// 03.sep.2001 - begin
//      lawRCDKeyToSidePair sides (lawsuit,
//                                 true  // 13.mar.2001
//                                ) ;
      SessRCDKeyToSidePair sides (sess, & (sess.date), true) ;
// 03.sep.2001 - end
      unsigned i ;
      const unsigned claims = sides.getClaimantsCount () ;

      if (sides.statusIsOK ())
        for (i = 0 ; i < claims ; i ++)
         {
          TClaimant claimant ;
	    TDefendant defendant ;

          if (sides.nextPair (& claimant, & defendant))
            pColumn -> addText (claimant.cName) ;
         }
     }  // -- iC_Claimants --
   }  // if (allFmtFileColumns)  // ============================


// 08.sep.1999 - end
// Moved out of the allFmtFileColumns dependece (01.oct.1999) - begin
    if (countDefendants)                  // Печат на срочна книга 13116   стар формат
    {  // -- iC_AccusedsNum, iCF_FromCancLaws --
      unsigned accuseds = 0 ;

      if ((pColumn = columns [iC_AccusedsNum]) != NULL ||
           columns [iCF_FromCancLaws])
        // Be careful with pColumn (it may be NULL) !!!!!!!!
       {  // -- iC_AccusedsNum --
        ColumnXmlEncloser encloser(pColumn, iC_AccusedsNum + bOC, pItems);  // 27.jan.2005

        if (strchr (RESULT_FINALIZEDS, sess.result) &&
            countThisSess
           )
         {
          lawRCDKeyToSidePair dP (lawsuit,
                                  true  // 13.mar.2001
                                 ) ;
	    unsigned defsCnt = dP.getDefendantsCount () ;
          bool clRet, defRet ;
          unsigned i ;

           {
		TSideWind side ;

		for (i = 0 ; i < defsCnt ; i ++)
             {
              dP.nextPair (NULL, & side, clRet, defRet) ;
              if (defRet && strchr (UCN_CITIZENS, side.ucnType))
                accuseds ++ ;
             }
           }
          if (pColumn)
            if (! strchr (RESULT_TO_UP_LOOK_INTO_RETURNEDS,
                          sess.result))  // 04.sep.2000
              addLongToColumn (pColumn,
                               tBH.lcAccusedsNum += accuseds
                               ) ;
         }
       }  // -- iC_AccusedsNum --
      if ((pColumn = columns [iCF_FromCancLaws]) != NULL)
       {  // -- iCF_FromCancLaws --
        ColumnXmlEncloser encloser(pColumn, iCF_FromCancLaws + bOC, pItems);  // 27.jan.2005

        if (countThisSess)
         {
          if (sessionCancelsTheLaw (sess, & lawsuit, true) &&             // Печат на срочна книга 13116   стар формат
              sess.result != RESULT_WITH_BULWARK)
           {
            if (! strchr (RESULT_TO_UP_LOOK_INTO_RETURNEDS,
                          sess.result))
             {
              accDistr.cFromCancLaws = accuseds ;
              addLongToColumn (pColumn,
                               accDistrHeap.cFromCancLaws + accuseds) ;
             }
           }
          else
            if (accDistr.cFromCancLaws)
              addLongToColumn (pColumn, accDistr.cFromCancLaws +
                                        accDistrHeap.cFromCancLaws) ;
         }
       }  // -- iCF_FromCancLaws --
     }  // -- iC_AccusedsNum, iCF_FromCancLaws --
    else  // of countDefendants  // 27.jan.2005 - begin
     {
      if ((pColumn = columns [iC_AccusedsNum]) != NULL)
       {  // -- iC_AccusedsNum --
        ColumnXmlEncloser encloser(pColumn, iC_AccusedsNum + bOC, pItems);  // 27.jan.2005

       }
      if ((pColumn = columns [iCF_FromCancLaws]) != NULL)
       {  // -- iCF_FromCancLaws --
        ColumnXmlEncloser encloser(pColumn, iCF_FromCancLaws + bOC, pItems);  // 27.jan.2005

       }
     }  // 27.jan.2005 - end

// accDistr changed to accDistrHeap (01.oct.1999) - begin --------------
    accDistrHeap += accDistr ;
    if (! countDefendants)
      accDistrHeap.cJudgedsTotal -= accDistr.cJudgedsTotal ;
//    if (countDefendants)  // 27.jan.2005 - begin
//     {
//      if ((pColumn = columns [iCF_JudgedsTotal]) != NULL
//          && accDistr.cJudgedsTotal)
//        addLongToColumn (pColumn, accDistrHeap.cJudgedsTotal) ;
//     }
    if((pColumn = columns[iCF_JudgedsTotal]) != NULL)
    {
      ColumnXmlEncloser encloser(pColumn, iCF_JudgedsTotal + bOC, pItems);  // 27.jan.2005

      if(countDefendants)
      {
        addLongToColumn (pColumn, accDistrHeap.cJudgedsTotal) ;
      }
    }  // 27.jan.2005 - end
    if ((pColumn = columns [iCF_JudgedsCond]) != NULL)
        // && accDistr.cJudgedsCond)
    {
      ColumnXmlEncloser encloser(pColumn, iCF_JudgedsCond + bOC, pItems);  // 27.jan.2005

      if(accDistr.cJudgedsCond)  // 27.jan.2005
      {
        addLongToColumn (pColumn, accDistrHeap.cJudgedsCond) ;
      }
    }
    if ((pColumn = columns [iCF_Accquits]) != NULL)
        //&& accDistr.cAccquits)  // 27.jan.2005
    {
      ColumnXmlEncloser encloser(pColumn, iCF_Accquits + bOC, pItems);  // 27.jan.2005

      if(accDistr.cAccquits)  // 27.jan.2005
      {
        addLongToColumn (pColumn, accDistrHeap.cAccquits) ;
      }
    }
    if ((pColumn = columns [iCF_Freeds]) != NULL)
        //&& accDistr.cFreeds)  // 27.jan.2005
    {
      ColumnXmlEncloser encloser(pColumn, iCF_Freeds + bOC, pItems);  // 27.jan.2005

      if(accDistr.cFreeds)  // 27.jan.2005
      {
        addLongToColumn (pColumn, accDistrHeap.cFreeds) ;
      }
    }
    if ((pColumn = columns [iCF_GenRecidive]) != NULL)
        //&& accDistr.cGenRecidive)  // 27.jan.2005
    {
      ColumnXmlEncloser encloser(pColumn, iCF_GenRecidive + bOC, pItems);  // 27.jan.2005

	if(accDistr.cGenRecidive)  // 27.jan.2005
	{
	  addLongToColumn (pColumn, accDistrHeap.cGenRecidive) ;
      }
    }
    if ((pColumn = columns [iCF_SpecRecidive]) != NULL)
	  //&& accDistr.cSpecRecidive)  // 27.jan.2005
    {
	ColumnXmlEncloser encloser(pColumn, iCF_SpecRecidive + bOC, pItems);  // 27.jan.2005

	if(accDistr.cSpecRecidive)  // 27.jan.2005
	{
	  addLongToColumn (pColumn, accDistrHeap.cSpecRecidive) ;
	}
    }
    if ((pColumn = columns [iCF_cNewCrimeDuringTestTime]) != NULL)
        //&& accDistr.cNewCrimeDuringTestTime)  // 27.jan.2005
    {
      ColumnXmlEncloser encloser(pColumn, iCF_cNewCrimeDuringTestTime + bOC,
        pItems);  // 27.jan.2005

      if(accDistr.cNewCrimeDuringTestTime)  // 27.jan.2005
      {
        addLongToColumn (pColumn, accDistrHeap.cNewCrimeDuringTestTime) ;
      }
    }
    if ((pColumn = columns [iCF_DangerousRecidive]) != NULL)  // 27.jan.2005
    {
      ColumnXmlEncloser encloser(pColumn, iCF_DangerousRecidive + bOC, pItems);

      if(accDistr.cDangerousRecidive)
	{
	  // 2010:056 TRQ: FIX: was cSpecRecidive
	  addLongToColumn(pColumn, accDistrHeap.cDangerousRecidive);
	}
    }
// Moved out of allFmtFileColumns dependence (01.oct.1999) - end
// accDistr changed to accDistrHeap (01.oct.1999) - end ----------------

  // 20051208 ->
# if INSTANCE
  if ((pColumn = columns [iCF_IIinst_Decision]) != NULL) // 13197 // 20051208
   {  // -- iCF_IIinst_Decision --
    ColumnXmlEncloser encloser(pColumn, iCF_IIinst_Decision + bOC, pItems);
#   if WITH_STARS
      if (  ((sess.result == RESULT_ANNOUNCE_TO_BE_DECIDED)   // = 'p'
              ||(sess.result == RESULT_WITH_DECISION)            // = 'Z'
              )
              && countThisSess
          )
          addLongToColumn (pColumn, tBH.s_IIinst_Decision += 1) ;
#   endif
   }  // -- iCF_IIinst_Decision --

  if ((pColumn = columns [iCF_IIinst_Cease]) != NULL) // 13198 // 20051208
   {  // -- iCF_IIinst_Cease --
    ColumnXmlEncloser encloser(pColumn, iCF_IIinst_Cease + bOC, pItems);

#   if WITH_STARS
      if ( ((sess.result == RESULT_CANCELED)            //  = 'c'
             ||(sess.result == RESULT_CANCEL_BY_AGREEMENT) //  = 'P'
             ||(sess.result == RESULT_SEND_BY_JURIDICTION) //  = 's'
            )&& countThisSess
          )
          addLongToColumn (pColumn, tBH.s_IIinst_Cease += 1) ;
#   endif
   }  // -- iCF_IIinst_Cease --

# endif
  // 20051208 <-
  if ((pColumn = columns [iCF_DecisJudgem]) != NULL)
   {  // -- iCF_DecisJudgem --
    ColumnXmlEncloser encloser(pColumn, iCF_DecisJudgem + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (strchr (RESULT_WITH_DECISIONS, sess.result) &&
        countThisSess)
      addLongToColumn (pColumn, tBH.lWithDec += 1) ;
# endif
   }  // -- iCF_DecisJudgem --
  if ((pColumn = columns [iCF_Returned]) != NULL)
   {  // -- iCF_Returned --
    ColumnXmlEncloser encloser(pColumn, iCF_Returned + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (strchr (RESULT_RETURNEDS, sess.result) &&
        countThisSess)
      addLongToColumn (pColumn, tBH.lReturneds += 1) ;
# endif
   }  // -- iCF_Returned --
  if ((pColumn = columns [iCF_Send]) != NULL)
   {  // -- iCF_Send --
    ColumnXmlEncloser encloser(pColumn, iCF_Send + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (RESULT_SEND == sess.result &&
        countThisSess)
      addLongToColumn (pColumn, tBH.lSends += 1) ;
# endif
   }  // -- iCF_Send --
  if ((pColumn = columns [iCF_CanceledOthReasons]) != NULL)
   {  // -- iCF_CanceledOthReasons --
    ColumnXmlEncloser encloser(pColumn, iCF_CanceledOthReasons + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    char tmp [2] ;

    tmp [1] = '\0' ;
    tmp [0] = RESULT_SEND ;
    scSprintf (buffL, buff, "%s",
//               RESULT_CANCELEDS_OR_RETURNEDS
               RESULT_CANCELEDS
              ) ;
    difference (buff, tmp) ;
//    if (columns [iCF_Returned])
//      difference (buff, RESULT_RETURNEDS) ;
    if (strchr (buff, sess.result) &&
        countThisSess)
      addLongToColumn (pColumn, tBH.lCancOthReasons += 1) ;
# endif
   }  // -- iCF_CanceledOthReasons --
  if ((pColumn = columns [iCF_Left]) != NULL)
   {  // -- iCF_Left --
    ColumnXmlEncloser encloser(pColumn, iCF_Left + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    scSprintf (buffL, buff, "%s%s", RESULT_LEFTS, RESULT_UNMOVEABLES) ;
    if ((strchr (buff, sess.result) &&
         countThisSessNF) || abolishedProgress
       )
      addLongToColumn (pColumn, tBH.sLefts += 1) ;
# endif
   }  // -- iCF_Left --
  if ((pColumn = columns [iCF_Stopped]) != NULL)
   {  // -- iCF_Stopped --
    ColumnXmlEncloser encloser(pColumn, iCF_Stopped + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (strchr (RESULT_STOPPEDS, sess.result) &&
        (countThisSessNF))
      addLongToColumn (pColumn, tBH.sStoppeds += 1) ;
# endif
   }  // -- iCF_Stopped --  113 --
  if ((pColumn = columns [iCF_UpTo3MonLong]) != NULL)
   {  // -- iCF_UpTo3MonLong --
    ColumnXmlEncloser encloser(pColumn, iCF_UpTo3MonLong + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if((strchr (AGE_UP_TO_THREES, lawsuit.age) != NULL) && countThisSess)
	//if (strchr (RESULT_FINALIZEDS, sess.result))	// 2010:011 LRQ: comout
	  addLongToColumn (pColumn, tBH.lUpTo3Months += 1) ;
# endif
   }  // -- iCF_UpTo3MonLong --  114 --
  if ((pColumn = columns [iCF_Over3MonLong]) != NULL)
   {  // -- iCF_Over3MonLong --
    ColumnXmlEncloser encloser(pColumn, iCF_Over3MonLong + bOC, pItems);  // 27.jan.2005

# if WITH_STARS //AGE_MORE_THAN_THREES
    if (strchr (AGE_MORE_THAN_THREES_ALL, lawsuit.age) && countThisSess) //20050318
	//if (strchr (RESULT_FINALIZEDS, sess.result))	// 2010:011 LRQ: comout
	  addLongToColumn (pColumn, tBH.lOver3Months += 1) ;
# endif
   }  // -- iCF_Over3MonLong --
# if INSTANCE
  if (countThisSess && strchr (RESULT_CANCELEDS, sess.result))
    tBH2nd.lCanceleds += 1 ;
// 21.feb.2000 - begin ------------------- !!! -----------------
  if (sess.result == RESULT_DEFINITION_READY)
    tBH2nd.clearData () ;
// 21.feb.2000 - end   ------------------- !!! -----------------
# endif
  if ((pColumn = columns [iCF_NotMajorDefendants]) != NULL)
   {  // -- iCF_NotMajorDefendants --
    ColumnXmlEncloser encloser(pColumn, iCF_NotMajorDefendants + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (strchr (RESULT_FINALIZEDS, sess.result) &&
	  countThisSess && notMajorDefends && countDefendants
	 )
	addLongToColumn (pColumn,
			     tBH.lNotMajorDefendants += notMajorDefends
			    ) ;
# endif
   }  // -- iCF_NotMajorDefendants --
  if ((pColumn = columns [iCF_NewlyCreated]) != NULL)
   {  // -- iCF_NewlyCreated --
    ColumnXmlEncloser encloser(pColumn, iCF_NewlyCreated + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (lawIsCreatedByInDoc (lawsuit) && countThisSessNF)
      addLongToColumn (pColumn, tBH.lNewlyCreateds += 1) ;
# endif
   }  // -- iCF_NewlyCreated --


  if ((pColumn = columns [iCF_Received]) != NULL)
   {  // -- iCF_Received --
    ColumnXmlEncloser encloser(pColumn, iCF_Received + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (lawIsReceivedByJurisd (lawsuit) && countThisSessNF)
      addLongToColumn (pColumn, tBH.lReceiveds += 1) ;
# endif
   }  // -- iCF_Received --
  if ((pColumn = columns [iCF_RetFrom2ndInstance]) != NULL)
   {  // -- iCF_RetFrom2ndInstance --
    ColumnXmlEncloser encloser(pColumn, iCF_RetFrom2ndInstance + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (lawIsReturnedFrom2ndInstance (lawsuit) && countThisSessNF)
      addLongToColumn (pColumn, tBH.lRetFrom2ndInstance += 1) ;
# endif
   }  // -- iCF_RetFrom2ndInstance --
  if ((pColumn = columns [iCF_ClaimFullyGranted]) != NULL)
   {  // -- iCF_ClaimFullyGranted --
    ColumnXmlEncloser encloser(pColumn, iCF_ClaimFullyGranted + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (lawsuit.grant == GRANT_ENTIRE &&
	  //strchr (RESULT_FINALIZEDS, sess.result) &&	// 2010:011 LPR: comout
	  countThisSess)
	addLongToColumn (pColumn, tBH.lClaimFullyGranted += 1) ;
# endif
   }  // -- iCF_ClaimFullyGranted --
  if ((pColumn = columns [iCF_ClaimPartialGranted]) != NULL)
   {  // -- iCF_ClaimPartialGranted --
    ColumnXmlEncloser encloser(pColumn, iCF_ClaimPartialGranted + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (lawsuit.grant == GRANT_PARTIAL &&
	  //strchr (RESULT_FINALIZEDS, sess.result) &&	// 2010:011 LPR: comout
	  countThisSess)
	addLongToColumn (pColumn, tBH.lClaimPartialGranted += 1) ;
# endif
   }  // -- iCF_ClaimPartialGranted --
  if ((pColumn = columns [iCF_ClaimNonsuit]) != NULL)
   {  // -- iCF_ClaimNonsuit --
    ColumnXmlEncloser encloser(pColumn, iCF_ClaimNonsuit + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (
//        lawsuit.grant == GRANT_REJECTED &&  // 20.apr.2001
        strchr (GRANT_REJECTEDS, lawsuit.grant) &&  // 20.apr.2001
	  //strchr (RESULT_FINALIZEDS, sess.result) &&	// 2010:011 LPR: comout
	  countThisSess)
	addLongToColumn (pColumn, tBH.lClaimNonsuit += 1) ;
# endif
   }  // -- iCF_ClaimNonsuit --
  if ((pColumn = columns [iCF_CancelByAgreement]) != NULL)
   {  // -- iCF_CancelByAgreement --
    ColumnXmlEncloser encloser(pColumn, iCF_CancelByAgreement + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    if (sess.result == RESULT_CANCEL_BY_AGREEMENT &&
        countThisSess)
      addLongToColumn (pColumn, tBH.lCancelByAgreement += 1) ;
# endif
   }  // -- iCF_CancelByAgreement --
  if ((pColumn = columns [iCF_Cancel_ExactByOtherCauses]) != NULL)
   {  // -- iCF_Cancel_ExactByOtherCauses --
    ColumnXmlEncloser encloser(pColumn, iCF_Cancel_ExactByOtherCauses + bOC,
      pItems);  // 27.jan.2005

# if WITH_STARS
    if (strchr (RESULT_CANCELEDS_OR_RETURNEDS, sess.result) &&
        sess.result != RESULT_CANCEL_BY_AGREEMENT &&
        sess.result != RESULT_SEND &&
        countThisSess)
      addLongToColumn (pColumn, tBH.lCancel_ExactByOtherCauses += 1) ;
# endif
   }  // -- iCF_Cancel_ExactByOtherCauses --   132 --
  if ((pColumn = columns [iCF_UpTo1MonthLong]) != NULL)
   {  // -- iCF_UpTo1MonthLong --
    ColumnXmlEncloser encloser(pColumn, iCF_UpTo1MonthLong + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    // nothing 20050318 ->
    if ((strchr (AGE_UP_TO_ONES, lawsuit.age) != NULL)&& countThisSess)
	//if (strchr (RESULT_FINALIZEDS, sess.result))	// 2010:011 LRQ: comout
	  addLongToColumn (pColumn, tBH.lUpTo1MonthLong += 1) ;
# endif
   }  // -- iCF_UpTo1MonthLong --  133 --
  if ((pColumn = columns [iCF_Betw1And3MonthLong]) != NULL)
   {  // -- iCF_Betw1And3MonthLong --
    ColumnXmlEncloser encloser(pColumn, iCF_Betw1And3MonthLong + bOC, pItems);  // 27.jan.2005

# if WITH_STARS
    // nothing 20050318 ->
    if((strchr (AGE_UP_TO_THREES, lawsuit.age) != NULL &&
	  strchr (AGE_UP_TO_ONES,   lawsuit.age) == NULL)&& countThisSess)
	//if (strchr (RESULT_FINALIZEDS, sess.result))	// 2010:011 LRQ: comout
        addLongToColumn (pColumn, tBH.lBetw1And3MonthsLong += 1) ;
# endif
   }  // -- iCF_Betw1And3MonthLong --



/*****************************************************************************/

# if INSTANCE

#   if APPEAL
#   else  // of APPEAL
  if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind))                                          
#   endif  // of APPEAL
   {  // -- if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind)) --

    if ((pColumn = columns [iCF_ReceivedsByComplaint]) != NULL)
     {  // -- iCF_ReceivedsByComplaint --
      ColumnXmlEncloser encloser(pColumn, iCF_ReceivedsByComplaint + bOC,
        pItems);  // 27.jan.2005

#     if WITH_STARS
      if (countThisSessNF)
       {
        if (strchr (KIND_EXACT_COMPLAINTS, lawsuit.sourceKind))
          if (thisIsFirstSession (sess))
            addLongToColumn (pColumn, tBH.lByComplaints += 1) ;
          else
            pColumn -> addText (STAR_STRING) ;
       }
#     endif
     }  // -- iCF_ReceivedsByComplaint --
    // 25.jan.2005 - begin
//    if ((pColumn = columns [iCF_RcvByComplIncludingNext]) != NULL &&
//        ! columns [iCF_ReceivedsByComplaint])
//     {  // -- iCF_RcvByComplIncludingNext --
//#     if WITH_STARS
//      if (countThisSessNF &&
//          strchr (KIND_EXACT_COMPLAINTS, lawsuit.sourceKind))
//        addLongToColumn (pColumn, tBH.lByComplaints += 1) ;
//#     endif
//     }  // -- iCF_RcvByComplIncludingNext --
    if ((pColumn = columns [iCF_RcvByComplIncludingNext]) != NULL)
     {  // -- iCF_RcvByComplIncludingNext --
      ColumnXmlEncloser encloser(pColumn, iCF_RcvByComplIncludingNext + bOC,
        pItems);  // 27.jan.2005

      if(!columns[iCF_ReceivedsByComplaint])
       {
#     if WITH_STARS
        if (countThisSessNF &&
            strchr (KIND_EXACT_COMPLAINTS, lawsuit.sourceKind))
          addLongToColumn (pColumn, tBH.lByComplaints += 1) ;
#     endif
       }
     }  // -- iCF_RcvByComplIncludingNext --  // 25.jan.2005 - end
    if ((pColumn = columns [iCF_ReceivedsBYObjection]) != NULL)
     {  // -- iCF_ReceivedsBYObjection --
      ColumnXmlEncloser encloser(pColumn, iCF_ReceivedsBYObjection + bOC,
        pItems);  // 27.jan.2005

#     if WITH_STARS
      if (countThisSessNF)
       {
        if (strchr (KIND_EXACT_OBJECTIONS, lawsuit.sourceKind))
          if (thisIsFirstSession (sess))
            addLongToColumn (pColumn, tBH.lByObjections += 1) ;
          else
            pColumn -> addText (STAR_STRING) ;
       }
#     endif
     }  // -- iCF_ReceivedsBYObjection --
    // 25.jan.2005 - begin
//    if ((pColumn = columns [iCF_RcvBYObjectIncludingNext]) != NULL &&
//        ! columns [iCF_ReceivedsBYObjection])
//     {  // -- iCF_RcvByObjectIncludingNext --
//#     if WITH_STARS
//      if (countThisSessNF &&
//          strchr (KIND_EXACT_OBJECTIONS, lawsuit.sourceKind))
//        addLongToColumn (pColumn, tBH.lByObjections += 1) ;
//#     endif
//     }  // -- iCF_RcvByObjectIncludingNext --
    if ((pColumn = columns [iCF_RcvBYObjectIncludingNext]) != NULL)
     {  // -- iCF_RcvByObjectIncludingNext --
      ColumnXmlEncloser encloser(pColumn, iCF_RcvBYObjectIncludingNext + bOC,
        pItems);  // 27.jan.2005

      if(!columns[iCF_ReceivedsBYObjection])
       {
#     if WITH_STARS
        if (countThisSessNF &&
            strchr (KIND_EXACT_OBJECTIONS, lawsuit.sourceKind))
          addLongToColumn (pColumn, tBH.lByObjections += 1) ;
#     endif
       }
     }  // -- iCF_RcvByObjectIncludingNext --  // 25.jan.2005 - end
    if ((pColumn = columns [iCF_JudgementValidityConfirmed]) != NULL)
     {  // -- iCF_JudgementValidityConfirmed --
      ColumnXmlEncloser encloser(pColumn, iCF_JudgementValidityConfirmed + bOC,
        pItems);  // 27.jan.2005

#     if WITH_STARS
      if (countThisSessResFin && tBH2nd.lJudgemConfirmed)
        addLongToColumn (pColumn, tBH.lJudgemConfirmed += 1) ;
#     endif
     }  // -- iCF_JudgementValidityConfirmed --
    if ((pColumn = columns [iCF_ConditConvictingApplied]) != NULL)
     {  // -- iCF_ConditConvictingApplied --
      ColumnXmlEncloser encloser(pColumn, iCF_ConditConvictingApplied + bOC,
        pItems);  // 27.jan.2005

#     if WITH_STARS
      if (countThisSessResFin && tBH2nd.lCndJdgApply)
        addLongToColumn (pColumn, tBH.lCndJdgApply += 1) ;
#     endif
     }  // -- iCF_ConditConvictingApplied --
    if ((pColumn = columns [iCF_JudgementRepealedPartilally]) != NULL)
     {  // -- iCF_JudgementRepealedPartilally --
      ColumnXmlEncloser encloser(pColumn, iCF_JudgementRepealedPartilally + bOC,
        pItems);  // 27.jan.2005

#     if WITH_STARS
      if (countThisSessResFin && tBH2nd.lJudgAbolishedPart)
        addLongToColumn (pColumn, tBH.lJudgAbolishedPart += 1) ;
#     endif
     }  // -- iCF_JudgementRepealedPartilally --
    if ((pColumn = columns [iCF_JudgemRepealFully_Returned]) != NULL)
     {  // -- iCF_JudgemRepealFully_Returned --
      ColumnXmlEncloser encloser(pColumn, iCF_JudgemRepealFully_Returned + bOC,
        pItems);  // 27.jan.2005

#     if WITH_STARS
      if (countThisSessResFin && tBH2nd.lJdgFullyAboli_Ret)
        addLongToColumn (pColumn, tBH.lJdgFullyAboli_Ret += 1) ;
#     endif
     }  // -- iCF_JudgemRepealFully_Returned --
    if ((pColumn = columns [iCF_JudgemRepealFully_Canceled]) != NULL)
     {  // -- iCF_JudgemRepealFully_Canceled --
      ColumnXmlEncloser encloser(pColumn, iCF_JudgemRepealFully_Canceled + bOC,
        pItems);  // 27.jan.2005

#     if WITH_STARS
# if INSTANCE
      if (countThisSess && tBH2nd.lCanceleds)
        addLongToColumn (pColumn, tBH.lCanceleds += 1) ;
# endif
#     endif
     }  // -- iCF_JudgemRepealFully_Canceled --
    if ((pColumn = columns [iCF_JudgemRepealFully_NewJudgem]) != NULL)
     {  // -- iCF_JudgemRepealFully_NewJudgem --
      ColumnXmlEncloser encloser(pColumn, iCF_JudgemRepealFully_NewJudgem + bOC,
        pItems);  // 27.jan.2005

#     if WITH_STARS
      if (countThisSessResFin && tBH2nd.lJdgFullyAboli_NewJ)
        addLongToColumn (pColumn, tBH.lJdgFullyAboli_NewJ += 1) ;
#     endif
     }  // -- iCF_JudgemRepealFully_NewJudgem --
    if ((pColumn = columns [iCF_DecisionValidityConfirmed]) != NULL)
     {  // -- iCF_DecisionValidityConfirmed --
      ColumnXmlEncloser encloser(pColumn, iCF_DecisionValidityConfirmed + bOC,
        pItems);  // 27.jan.2005
#     if WITH_STARS
      if (countThisSessResFin && tBH2nd.lDecisionValidityConfirmed)
        addLongToColumn (pColumn, tBH.lDecisionValidityConfirmed += 1) ;
#     endif
     }  // -- iCF_DecisionValidityConfirmed --

    if ((pColumn = columns [iCF_DecisionRepealedPartilally]) != NULL)
     {  // -- iCF_DecisionRepealedPartilally --
      ColumnXmlEncloser encloser(pColumn, iCF_DecisionRepealedPartilally + bOC,
        pItems);  // 27.jan.2005
# if INSTANCE
#     if WITH_STARS
      if (countThisSessResFin && tBH2nd.lDecisionRepealedPartilally)
        addLongToColumn (pColumn, tBH.lDecisionRepealedPartilally += 1) ;
#     endif
#endif

     }  // -- iCF_DecisionRepealedPartilally --
    if ((pColumn = columns [iCF_DecisRepealFully_Returned]) != NULL)
     {  // -- iCF_DecisRepealFully_Returned --
      ColumnXmlEncloser encloser(pColumn, iCF_DecisRepealFully_Returned + bOC,
        pItems);  // 27.jan.2005

#     if WITH_STARS
# if INSTANCE
      if (countThisSessResFin && tBH2nd.lDecisRepealFully_Returned)
        addLongToColumn (pColumn, tBH.lDecisRepealFully_Returned += 1) ;
# endif
#     endif
     }  // -- iCF_DecisRepealFully_Returned --
    if ((pColumn = columns [iCF_DecisRepealFully_Canceled]) != NULL)
     {  // -- iCF_DecisRepealFully_Canceled --
      ColumnXmlEncloser encloser(pColumn, iCF_DecisRepealFully_Canceled + bOC,
        pItems);  // 27.jan.2005

#     if WITH_STARS
# if INSTANCE
      if (countThisSess &&
          columns [iCF_JudgemRepealFully_Canceled] == NULL &&
          tBH2nd.lCanceleds)
        addLongToColumn (pColumn, tBH.lCanceleds += 1) ;
# endif
#     endif
     }  // -- iCF_DecisRepealFully_Canceled --
    if ((pColumn = columns [iCF_DecisRepealFully_NewDecis]) != NULL)
     {  // -- iCF_DecisRepealFully_NewDecis --
      ColumnXmlEncloser encloser(pColumn, iCF_DecisRepealFully_NewDecis + bOC,
        pItems);  // 27.jan.2005

#     if WITH_STARS
# if INSTANCE
      if (countThisSessResFin && tBH2nd.lDecisRepealFully_NewDecis)
        addLongToColumn (pColumn, tBH.lDecisRepealFully_NewDecis += 1) ;
# endif
#     endif
     }  // -- iCF_DecisRepealFully_NewDecis --
/*****************************************************************************/

  }  // -- if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind)) --

# endif    //INSTANCE

/*****************************************************************************/
// 27.jan.2005 - begin
  if((pColumn = columns[iC_SessionDate]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_SessionDate + bOC, pItems);

    add_date_to_column(pColumn, sess.date);
  }  // -- iC_SessionDate --
  if((pColumn = columns[iC_PresidentAndMembers]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_PresidentAndMembers + bOC, pItems);

    // Do not add the president if he is a speaker(judge)
    // and the iC_Judge column is present
    // ???
    //if(columns[iC_Judge] == NULL || strcmp(sess.president, sess.judge) != 0)
    //{
    if(columns[iC_President] == NULL)
    {
      addMemberToMembership(sess.president, buff, buffL, pColumn,
                             JUDGE_STRING, sess.judge);
    }
    //}

    // Do not add the judgeX if he is a speaker(judge)
    // and the iC_Judge column is present
    if(columns[iC_Judge] == NULL || strcmp(sess.judgeX, sess.judge) != 0)
    {
      addMemberToMembership(sess.judgeX, buff, buffL, pColumn,
                             JUDGE_STRING, sess.judge);
    }

    // Do not add the reJudgeX if he is a speaker(judge)
    // and the iC_Judge column is present
    if(columns[iC_Judge] == NULL || strcmp(sess.reJudgeX, sess.judge) != 0)
    {
      addMemberToMembership(sess.reJudgeX, buff, buffL, pColumn,
                             JUDGE_STRING, sess.judge);
    }
    addMemberToMembership(sess.juror, buff, buffL, pColumn,
                           JUROR_STRING);
    addMemberToMembership(sess.reJuror, buff, buffL, pColumn,
                           REJUROR_STRING);
    addMemberToMembership(sess.jurorX, buff, buffL, pColumn,
                           JURORX_STRING);
    if(columns[iC_Secretar] == NULL)
    {
      addMemberToMembership(sess.secretar, buff, buffL, pColumn,
                             SECRETAR_STRING);
    }
    if(columns[iC_Prosecutor] == NULL)
    {
      addMemberToMembership(sess.prosecutor, buff, buffL, pColumn,
                             PROSECUTOR_STRING);
    }
  }  // -- iC_PresidentAndMembers --
  if((pColumn = columns[iC_Jurors]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_Jurors + bOC, pItems);

    addMemberToMembership(sess.juror, buff, buffL, pColumn,
                           JUROR_STRING);
    addMemberToMembership(sess.reJuror, buff, buffL, pColumn,
                           REJUROR_STRING);
    addMemberToMembership(sess.jurorX, buff, buffL, pColumn,
                           JURORX_STRING);
  }  // -- iC_Jurors --
  if((pColumn = columns[iC_Judge]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_Judge + bOC, pItems);

    if(ucnPresent(sess.judge) &&
      citizenUCNAndUCNTypeToName(sess.judge, UCN_CITIZEN_UCN, buff, buffL,
      true, true))
    {
      pColumn->addText(buff);
    }
  }  // -- iC_Judge --
  if((pColumn = columns[iC_Prosecutor]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_Prosecutor + bOC, pItems);

    if(ucnPresent(sess.prosecutor) &&
      citizenUCNAndUCNTypeToName(sess.prosecutor, UCN_CITIZEN_UCN, buff, buffL,
      true, true))
    {
      pColumn->addText(buff);
    }
  }  // -- iC_Prosecutor --
  if((pColumn = columns[iC_Secretar]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_Secretar + bOC, pItems);

    if(ucnPresent(sess.secretar) &&
      citizenUCNAndUCNTypeToName(sess.secretar, UCN_CITIZEN_UCN, buff, buffL,
      true, true))
    {
      pColumn->addText(buff);
    }
  }  // -- iC_Secretar --
  if((pColumn = columns[iC_President]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_President + bOC, pItems);

    if(ucnPresent(sess.president) &&
      citizenUCNAndUCNTypeToName(sess.president, UCN_CITIZEN_UCN, buff, buffL,
      true, true))
    {
      pColumn->addText(buff);
    }
  }  // -- iC_President --
  if((pColumn = columns[iC_ComplaintMaker_Citizen]) != NULL)   // 13176
  {
    ColumnXmlEncloser encloser(pColumn, iC_ComplaintMaker_Citizen + bOC,
      pItems);

    SessRCDKeyToSidePair sides (sess, &(sess.date), true);
    unsigned i ;
    const unsigned claims = sides.getClaimantsCount();

    if(sides.statusIsOK())
    {
      TClaimant claimant;
      TSideWind s;

      for(i = 0; i < claims; i ++)
      {
        if(sides.nextPair (&claimant, NULL, &s))
        {
          if(  (s.involvement == INVOLVEMENT_COMPLAINTLODGER)
          //20050831 ->
#if INSTANCE
#  if APPEAL
             ||(s.involvement == INVOLVEMENT_COMPLAINT_CLAIMANT)
             ||(s.involvement == INVOLVEMENT_COMPLAINT_ACCUSER)
             ||(s.involvement == INVOLVEMENT_COMPLAINT_DEFENDANT)
             ||(s.involvement == INVOLVEMENT_COMPLAINT_ACCUSED)
#  endif
#endif
          //20050831 <-
            )
          {
            pColumn->addText(claimant.cName);
          }
        }
      }
    }
  }  // -- iC_ComplaintMaker_Citizen --
  if((pColumn = columns[iC_Defendant_Citizen]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_Defendant_Citizen + bOC, pItems);

    SessRCDKeyToSidePair sides (sess, &(sess.date), true);
    unsigned i ;
    const unsigned defends = sides.getDefendantsCount();

    if(sides.statusIsOK())
    {
	TDefendant defendant;
	TSideWind s;

	for(i = 0; i < defends; i ++)
	{
//TRCDKeyToSidePair <T, S> :: nextPair (S * pClSide, S * pDefSide,
// 20050831                                  bool & clReturned,
//                                           bool & defReturned)
	  if(sides.nextPair (NULL, &defendant, NULL, &s))
        {
// 20060426 in comm          if(s.involvement == INVOLVEMENT_DEFENDANT)
	  //if( strchr(INVOLVEMENT_FROM_RIGHTS, s.involvement) != NULL)  // 20060426
	  // 2007:268 LPR: this seems corrent, but in fact I have no idea  
	  if( sess.Defendant(s.involvement) )
          {
            pColumn->addText(defendant.dName);
          }
        }
      }
    }
  }  // -- iC_ComplaintMaker_Citizen --
  if((pColumn = columns[iC_SessResult_Decided]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_SessResult_Decided + bOC,
      pItems);

    if(sessionDecidesTheLaw(sess, &lawsuit,
      strchr(KIND_CLOSED_SESSIONS, sess.kind) == NULL))
    {
      pColumn->addText(STAR_STRING);
    }
  }  // -- iC_SessResult_Decided --
  if((pColumn = columns[iC_SessResult_Canceled]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_SessResult_Canceled + bOC,
      pItems);

    if(sessionCancelsTheLaw(sess, &lawsuit,
      strchr(KIND_CLOSED_SESSIONS, sess.kind) == NULL))
    {
      pColumn->addText(STAR_STRING);
    }
  }  // -- iC_SessResult_Canceled --
  if((pColumn = columns[iC_SessResult_Stopped]) != NULL)
  {  // See also iCF_Stopped
    ColumnXmlEncloser encloser(pColumn, iC_SessResult_Stopped + bOC, pItems);

    if(strchr(RESULT_STOPPEDS, sess.result) && countThisSessNF)
    {
      pColumn->addText(STAR_STRING);
    }
  }  // -- iC_SessResult_Stopped --
  if((pColumn = columns[iC_SessResult_Left]) != NULL)
  {  // See also iCF_Left
    ColumnXmlEncloser encloser(pColumn, iC_SessResult_Left + bOC, pItems);

    strCopy(buff, RESULT_LEFTS, buffL);
    strConcat(buff, RESULT_UNMOVEABLES, buffL);
    if((strchr (buff, sess.result) && countThisSessNF) || abolishedProgress)
    {
      // 20050826 in coment -->
      //if(sess.text[0])
      //{
      //  pColumn->addText(sess.text);
      //}
      //else
      //{
      //  pColumn->addText(STAR_STRING);
      //}
      // 20050826 in coment <--

      pColumn->addText(STAR_STRING);

    }
  }  // -- iC_SessResult_Left --

  if((pColumn = columns[iC_JudicialActNo]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_JudicialActNo + bOC, pItems);

      {
        SessionEndDocs endDocs(sess.key, sess.date, sess.kind);
        DecPtr pDec;
        TDecision& dec = pDec.Ref();

        while(endDocs.NextDoc(dec))
        {
          char tmp[128];	// 2010:014 LPR: 64 -> 128

          kind_to_alias(dec.kind, tmp, sizeof(tmp));
          if((dec.eventNo > 0)&&
             PRINT_JUDGEMENTS) //20050401
	    {
		 strConcat(tmp, " No ", sizeof(tmp));
		 strConcatLong(tmp, dec.eventNo, sizeof(tmp));
	    }
	    // 2010:033 TRQ/FIX: cat_date_result() -> date only
	    mstr m;

	    dec.fromDate.Print(m, " от %D");
	    strConcat(tmp, str(m), sizeof(tmp));
	    pColumn->addText(tmp);
	  }
	}
	if(sess.eventNo > 0)
	{
	    char tmp[128];	// 2010:014 LPR: 64 -> 128
	    strCopy(tmp, "Определение", sizeof(tmp));
	    if (PRINT_JUDGEMENTS) //20050401
	    {
		strConcat(tmp, " No ", sizeof(tmp));
		strConcatLong(tmp, sess.eventNo, sizeof(tmp));
	    }
	    // 2010:014 LRQ/LPR
	    mstr m;

	    sess.date.Print(m, " от %D");
	    strConcat(tmp, str(m), sizeof(tmp));
	    pColumn->addText(tmp);
	}
  }  // -- iC_JudicialActNo --
  if((pColumn = columns[iC_SubjectCode]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_SubjectCode + bOC, pItems);

    addSubjCodeToColumn(pColumn, lawsuit.subject, lawsuit.kind);
  }  // -- iC_SubjectCode --
  if((pColumn = columns[iC_NewlyCreatedOrTransformed]) != NULL)     //13184
  {
    ColumnXmlEncloser encloser(pColumn, iC_NewlyCreatedOrTransformed + bOC,
	pItems);
    // tbc_t_.fmt - кол. "Делото е новообразувано / преобразувано под нов номер"
    // tbp_t_.fmt - кол. "Насрочени дела  - според източника на постъпление - Новообразувано, преобразувано под нов номер"
    // 2009:154 LPR: removed
	  // Търсим
        // CONNECT_TYPE_RETURNED_LAWSUIT	= 'R'  -"Върнато дело"
        // CONNECT_TYPE_JURISDICTION	= 'J'      -"По подсъдност"
        // 20070112 -->
        {
         	TGArray<TConnect> connects;
          TConnect *connect = new TConnect;
          TLawsuit *pLaw = new TLawsuit;
          pLaw -> key = lawsuit.key;

         	if (load_connects(pLaw, &connects))
          {
        		  for (int i = 0; i < connects.Count(); i++)
        		  {
         			  *connect << connects[i];
                if ( connect -> connectType == CONNECT_TYPE_RETURNED_LAWSUIT )
                {
                    ostrstream (buff, buffL)
                          << "Върнато дело No " << connect -> connectNo << "/"
                          << connect -> connectYear << ends ;
                    pColumn -> addText (buff) ;
                }
                else if ( connect -> connectType == CONNECT_TYPE_JURISDICTION )
                {
                    char tmp[SIZE_OF_NAME + 64];
                    ostrstream oss(buff, buffL);
                    oss << "По подсъдност";
                    if( ucnAndUcnTypeToName( connect -> ucn, connect -> ucnType, tmp, sizeof(tmp)) )
                    {
                       oss << " от " << tmp;
                    }
                    oss << ends;
                    pColumn->addText(buff);
                }
        			}
        	}
        	delete connect;
          delete pLaw;
        }
        // 20070112 <--
  }  // -- iC_NewlyCreatedOrTransformed --
  if((pColumn = columns[iC_MiscellaneousObstacles_Rgn]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_MiscellaneousObstacles_Rgn + bOC,
      pItems);

    // If the session result does not fit in any of the following columns:
    // iC_SessResult_Decided_Rgn_Cit,
    // iC_SessResult_Stopped_Rgn_Cit,
    // iC_SessResult_Left_Rgn_Cit;
    // then diplay the information in this column.
    if((sessionDecidesTheLaw(sess, &lawsuit,
      strchr(KIND_CLOSED_SESSIONS, sess.kind) == NULL) ||
      strchr(RESULT_STOPPEDS, sess.result) ||
      strchr(RESULT_LEFTS, sess.result)) == false)
    {
      if(!strIsBlank(sess.text))
      {
        pColumn->addText(sess.text);
      }
      addDecisionToColumn(lawsuit.key, sess.date, sess.kind, pColumn,
        NULL, sess.result);
    }

    // Appealing
    {
      TIArray<DatedContainer_> inRegKeys;
      DatedContainer_ sessKey(sess.key, sess.kind, sess.date);

      if(sess_result_appealed(sessKey, &inRegKeys))
      {
        if(pColumn->getRowsCount() > 0)
        {
          pColumn->addEmptyRow();
        }
        for(int i = 0; i < inRegKeys.Count(); i++)
        {
          DatedContainer_& key = *(inRegKeys[i]);

          {
            ostrstream oss (buff, buffL);

            oss << Kind_(key.kind) << " No " << key.no;
            if(key.year != key.date.year)
            {
              oss << "/" << key.year;
            }
            oss << " от " << key.date << ends;
          }
          pColumn->addText(buff);
        }
      }
    }
  }  // -- iC_MiscellaneousObstacles_Rgn --
  if((pColumn = columns[iC_SessResult_Decided_Rgn_Cit]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_SessResult_Decided_Rgn_Cit + bOC,
      pItems);

    if(sessionDecidesTheLaw(sess, &lawsuit,
      strchr(KIND_CLOSED_SESSIONS, sess.kind) == NULL))
    {
      if (defaultDecDateAndHowCondition ())
      {
        char tmpSessRes = '\0';
        if(!strchr(RESULT_FINALIZEDS, sess.result) ||
            strchr(KIND_CLOSED_SESSIONS, sess.kind) ||
#if INSTANCE
#else   // REGIONAL
            lawsuit.kind == KIND_ADMIN_PUNISHMENT_LAW ||
#endif
            sess.key.type != TYPE_PUNISHMENT_LAW
           )
          tmpSessRes = sess.result;
        if(!strIsBlank(sess.text))
        {
          pColumn->addText(sess.text);
        }
        addDecisionToColumn(lawsuit.key, sess.date, sess.kind, pColumn,
          NULL, tmpSessRes);
      }

      if(abolishedProgress)
      {
        add_abolishing_to_column(pColumn, (*pDateAbolish), pAbolishingSess);
      }
    }
  }  // -- iC_SessResult_Decided_Rgn_Cit --
  if((pColumn = columns[iC_SessResult_Stopped_Rgn_Cit]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_SessResult_Stopped_Rgn_Cit + bOC,
      pItems);

    if(strchr(RESULT_STOPPEDS, sess.result))
    {
      addResultToColumn(sess.result, pColumn);
      if(!strIsBlank(sess.text))
      {
        pColumn->addText(sess.text);
      }
    }
  }  // -- iC_SessResult_Stopped_Rgn_Cit --
  if((pColumn = columns[iC_SessResult_Left_Rgn_Cit]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_SessResult_Left_Rgn_Cit + bOC,
      pItems);

    if(strchr(RESULT_LEFTS, sess.result))
    {
      addResultToColumn(sess.result, pColumn);
      if(!strIsBlank(sess.text))
      {
        pColumn->addText(sess.text);
      }
    }
  }  // -- iC_SessResult_Left_Rgn_Cit --
#if INSTANCE
#  if APPEAL
#  else  //   AREAL ->
  if((pColumn = columns[iC_FixedPunishmentGeneral]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_FixedPunishmentGeneral + bOC,
      pItems);

    if(lawsuit.kind == KIND_PUNISHMENT_GENERAL_CHAR_LAW)
    {
      TRCDKey_(lawsuit.key).PrintShortest(buff, buffL);
      pColumn->addText(buff);
    }
  }  // -- iC_FixedPunishmentGeneral --
  if((pColumn = columns[iC_FixedPunishmentPrivateChar]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_FixedPunishmentPrivateChar + bOC,
      pItems);

    if(lawsuit.kind == KIND_PUNISHMENT_PRIVATE_CHAR_LAW)
    {
      TRCDKey_(lawsuit.key).PrintShortest(buff, buffL);
      pColumn->addText(buff);
    }
  }  // -- iC_FixedPunishmentPrivateChar --
#  endif  // <- AREAL
#else  // REGIONAL ->
  if((pColumn = columns[iC_FixedPunishmentGeneral]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_FixedPunishmentGeneral + bOC,
      pItems);

    if(lawsuit.kind == KIND_PUNISHMENT_GENERAL_CHAR_LAW)
    {
      TRCDKey_(lawsuit.key).PrintShortest(buff, buffL);
      pColumn->addText(buff);
    }
  }  // -- iC_FixedPunishmentGeneral --
  if((pColumn = columns[iC_FixedPunishmentPrivateChar]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_FixedPunishmentPrivateChar + bOC,
      pItems);

    if(lawsuit.kind == KIND_PUNISHMENT_PRIVATE_CHAR_LAW)
    {
      TRCDKey_(lawsuit.key).PrintShortest(buff, buffL);
      pColumn->addText(buff);
    }
  }  // -- iC_FixedPunishmentPrivateChar --
#endif // <- REGIONAL

  if((pColumn = columns[iC_FixedAdminPunishment]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_FixedAdminPunishment + bOC,
      pItems);

#if INSTANCE
#else  // of INSTANCE
    if(lawsuit.kind == KIND_ADMIN_PUNISHMENT_LAW)
    {
      TRCDKey_(lawsuit.key).PrintShortest(buff, buffL);
      pColumn->addText(buff);
    }
#endif  // of INSTANCE
  }  // -- iC_FixedAdminPunishment --
  if((pColumn = columns[iC_FixedPrivatePunishment]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_FixedPrivatePunishment + bOC,
      pItems);

#if INSTANCE
#  if APPEAL
#  else  //   AREAL ->
    if(lawsuit.kind == KIND_PRIVATE_PUNISHMENT_LAW)
    {
      TRCDKey_(lawsuit.key).PrintShortest(buff, buffL);
      pColumn->addText(buff);
    }
#  endif  // <- AREAL
#else  // REGIONAL ->
    if(lawsuit.kind == KIND_PRIVATE_PUNISHMENT_LAW)
    {
      TRCDKey_(lawsuit.key).PrintShortest(buff, buffL);
      pColumn->addText(buff);
    }
#endif  // <- REGIONAL
  }  // -- iC_FixedPrivatePunishment --
  if((pColumn = columns[iC_FixedBySourceCancAppealRet]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_FixedBySourceCancAppealRet + bOC,
      pItems);

    // Nothing yet
  }  // -- iC_FixedBySourceCancAppealRet --
// 27.jan.2005 - end
 delete [] buff ;

/*********************************************************/
 }  // timeBookColumns :: makeColumns

timeBookColumns :: timeBookColumns (const char * formatFN,
                                    TWindow * parent,
                                    TSession & rSess,
                                    TLawsuit & lawsuit, size_t sNo,
                                    timeBookHeap & tBH,
                                    accusedsDistribution & accDistrHeap,
                             TStringArray * itemsPtr,  // 27.jan.2005
                                    const collCompBegDEndDBegN * pCBDEDBN,
                                    size_t minRows,
                                    const bool allFmtFileColumns,
                                    const CDate * pDateAbolish,
                                    const CDate * const pDateReturnedAbolish,
                                    const int additionalRowsAfterSess,
                                    const TSession * pAbolishingSess
					     ) :
			   timeBookEnum (),
			   columnsArr (formatFN, parent, i_columns,
					   101 + OFFSET_TIME_BOOK_CIPHER),
			   sess (rSess), pItems (itemsPtr)
 {
  makeColumns (sess, lawsuit, tBH, accDistrHeap, sNo, pCBDEDBN,
		   allFmtFileColumns, pDateAbolish, pDateReturnedAbolish,
		   additionalRowsAfterSess, pAbolishingSess) ;
  defineMaxRows () ;
  if (maxRows < minRows)
    maxRows = minRows ;
 }  // timeBookColumns :: timeBookColumns

// ----------------- one session print service -----------------------
void timeBookColumns :: addResKindDResTxtToColumn (char result,
								   char kind,
								   char dJResult,
								   const char * text,
								   column * pColumn,
								   long event_no)
 {
  if (! pColumn)
    return ;
  TKind   tK ;
  const size_t sSz = max(sizeof Result->name, sizeof tK.name) + 64;
  char * s  = new char [sSz];

  if (result && ! strchr (RESULT_FINALIZEDS, result))
  {// addResultToColumn (result, pColumn) ; // 20051202 in comment
	//TResult tR ;  //      sSz = sizeof (tR.name) + 64;
	if ((s) != NULL)
	   resultToString (result, s, sSz) ;
	// 20051202 ->
	if ((s) != NULL)
	{
	    if((event_no > 0)&&
		 PRINT_JUDGEMENTS)
	    {
		   strConcat(s, " No ", sSz);
		   strConcatLong(s, event_no, sSz);
	    }
	    pColumn->addText(s);
	}
	// 20051202 <-
  }
  if (kind)
  {// addKindToColumn (kind, pColumn) ;     // 20051202 in comment
	//TKind tK ;    //      sSz = sizeof (tK.name) + 64;
	if ((s) != NULL)
	   kindToString (kind, s, sSz) ;

	// 20051202 ->
	if ((s) != NULL)
	{
	    if((event_no > 0)&&
		 PRINT_JUDGEMENTS)
	    {
		   strConcat(s, " No ", sSz);
		   strConcatLong(s, event_no, sSz);
	    }
	    pColumn->addText(s);
	}
	// 20051202 <-
  }

  if (dJResult)
  {// addResultToColumn (dJResult, pColumn) ;  // 20051202 in comment
	//TResult tR ;  //      sSz = sizeof (tR.name) + 64;
	if ((s) != NULL)
	   resultToString (dJResult, s, sSz) ;
  }

  if ((s) != NULL) delete [] s ;

  if (text && text [0])
    pColumn -> addText (text) ;
 }  // timeBookColumns :: addResKindDResTxtToColumn

void timeBookColumns :: arrangeDesJudgColumns (char result, char kind,   // session result // used when snippet is printed
							     char dJResult,
							     const char * text,
							     const bool flushAtFirst,
							     size_t minRows,
							     long event_no)
 {
  column * pColumn ;

  if (flushAtFirst)  // 09.jun.2000
   {
    if ((pColumn = columns [iC_DecisionsJudgements]) != NULL)
     {  // -- iC_DecisionsJudgements --
	pColumn -> flush () ;
	addResKindDResTxtToColumn (result, kind, dJResult, text, pColumn, event_no) ;
     }  // -- iC_DecisionsJudgements --

    if ((pColumn = columns [iC_DecisionsJudgements_TB_NF_PL]) != NULL) // 20060331
     {  // -- iC_DecisionsJudgements_TB_NF_PL --
	pColumn -> flush () ;
	addResKindDResTxtToColumn (result, kind, dJResult, text, pColumn, event_no) ;
     }  // -- iC_DecisionsJudgements_TB_NF_PL --

    if ((pColumn = columns [iC_Result_TB_NF_AL]) != NULL)   // 13202 20060406
     {  // -- iC_Result_TB_NF_AL --
	pColumn -> flush () ;
# if INSTANCE
	addResKindDResTxtToColumn (result, kind, dJResult, text, pColumn, event_no) ; //20060502
# else
      // Exclude output for new result RESULT_SESS_LEFT_TEMP_MEASURES = 'E', // 'Отложено с привременни мерки'
      if (result != RESULT_SESS_LEFT_TEMP_MEASURES)
         addResKindDResTxtToColumn (result, kind, dJResult, text, pColumn, event_no) ;
# endif
     }  // -- iC_Result_TB_NF_AL --

    if ((pColumn = columns [iC_DecisionDateAndHow]) != NULL)   // 13130
     {  // -- iC_DecisionDateAndHow --
      pColumn -> flush () ;
# if INSTANCE
      addResKindDResTxtToColumn (result, kind, dJResult, text, pColumn, event_no) ; // 20060502
# else
      // 20060310 Exclude output for new result RESULT_SESS_LEFT_TEMP_MEASURES = 'E', // 20060310 'Отложено с привременни мерки'
      if (result != RESULT_SESS_LEFT_TEMP_MEASURES)  //20060310
         addResKindDResTxtToColumn (result, kind, dJResult, text, pColumn, event_no) ;
# endif
     }  // -- iC_DecisionDateAndHow --
    if ((pColumn = columns [iC_DecisionsJudgementsCatchOth]) != NULL)
     {  // -- iC_DecisionsJudgementsCatchOth --
      pColumn -> flush () ;
      addResKindDResTxtToColumn (result, kind, dJResult, text, pColumn, event_no) ;
     }  // -- iC_DecisionsJudgementsCatchOth --
   }
  else
   {
    // Nothing, I do not change the columns (they are created yet).
   }
  defineMaxRows () ;
  if (maxRows < minRows)
    maxRows = minRows ;
 }  // timeBookColumns :: arrangeDesJudgColumns
// -------------------------------------------------------------------

void timeBookColumns :: notConstOutput (ostream & os, bool first)
 {
  if (! columns)
    return ;
  if (first)
   {  // -- 0 --
    for (int c = 0 ; c < areasCnt ; c ++)
      if (knownCCipher (pBA [c].dataCipher))
       {
        const int i = pBA [c].dataCipher - bOC ;

        if (columns [i])
          writeString (os, columns [i] -> nextRowOrEmptyStr (),
                       pBA [c].areaLen, pBA [c].align, pBA [c].from) ;
       }
      else
       {
        const size_t len = pBA [c].areaLen + pBA [c].from ;
        char * s = new char [len + 1] ;
        const int algn = pBA [c].align ;
        const size_t skip = pBA [c].from ;
        const unsigned int dCiph = pBA [c].dataCipher ;

        if (! (sessionOutput (sess, os, s, len, algn, skip,
                              dCiph) ||
               miscOutput (os, s, len, algn, skip, dCiph)))
          writeString (os, "", len) ;
        delete [] s ;
       }
   }  // -- 0 --
  else
   {  // -- 1 --
    int c1 ;

    for (c1 = 0 ; c1 < areas1Cnt ; c1 ++)
     {
      const size_t len = pBA1 [c1].areaLen + pBA1 [c1].from ;
      char * s = new char [len + 1] ;
      const int algn = pBA1 [c1].align ;
      const size_t skip = pBA1 [c1].from ;
      const unsigned int dCiph = pBA1 [c1].dataCipher ;

      if (! (sessionOutput (sess, os, s, len, algn, skip,
                            dCiph) ||
             miscOutput (os, s, len, algn, skip, dCiph)))
        writeString (os, "", len) ;
      delete [] s ;
     }
   }  // -- 1 --
 }  // timeBookColumns :: notConstOutput

const CDate * timeBookColumns :: sessDatePtr () const
 {
  return & (sess.date) ;
 }  // timeBookColumns :: sessDatePtr

void timeBookColumns :: sessionLabel (char * const dest,
                                      const size_t destSz) const
 {
  if (dest && destSz)
   {
    const size_t destL = destSz - 1 ;

    dest [destL] = '\0' ;
    ostrstream (dest, destL) << "заседание от " << sess.date
      << " по дело No " << sess.key.no << "/" << sess.key.year << ends ;
   }
 }  // timeBookColumns :: sessionLabel

bool timeBookColumns :: showDecisionNumber ()
 {
  if (showDecNoIsSet == false)
    setShowDecisionNumber () ;
  return showDecisionNo ;
 }  // timeBookColumns :: showDecisionNumber

void timeBookColumns :: setShowDecisionNumber ()
 {
  showDecisionNo = PRINT_JUDGEMENTS;
  showDecNoIsSet = true ;
 }  // timeBookColumns :: setShowDecisionNumber

void timeBookColumns ::
  decisionsAndSurroundments (const TGArray <TSurroundment> & surroundments, // used when snippet is printed
                             TSession & rSess, const TLawsuit & law)
 {
  if (columns [iC_DecisionDateAndHow])
    columns [iC_DecisionDateAndHow] -> flush () ;

  if (columns [iC_Result_TB_NF_AL])             // 20060406
    columns [iC_Result_TB_NF_AL] -> flush () ;

  if (columns [iC_GivingAndReturnDate])
    columns [iC_GivingAndReturnDate] -> flush () ;
  if (columns [iC_DecisionDateAndHow])
      sess_end_docs_to_columns (columns [iC_DecisionDateAndHow],
                            columns [iC_GivingAndReturnDate],
                            rSess, law, surroundments,
                            KIND_DEFINITIONS_BY_NAME) ;
  if (columns [iC_Result_TB_NF_AL])             // 20060406
      sess_end_docs_to_columns (columns [iC_Result_TB_NF_AL],
                            columns [iC_GivingAndReturnDate],
                            rSess, law, surroundments,
                            KIND_DEFINITIONS_BY_NAME) ;

  defineMaxRows () ;
 }  // timeBookColumns :: decisionsAndSurroundments

bool timeBookColumns :: showDecisionNo = false ;
bool timeBookColumns :: showDecNoIsSet = false ;

# undef WITH_STARS

//

