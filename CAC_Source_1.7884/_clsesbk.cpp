#include "cac.h"

# ifdef WITH_STARS
# error WITH_STARS must not be defined here
# endif
# define WITH_STARS 1

bool closedSessBookColumns ::
  mayShowDateFinished (const CDate lawFinished, const TSession & sess,
                       const collBegDEndDBegN & rCBegDEndDBegN) const
 {
  if ((strchr (RESULT_FINALIZEDS, sess.result) ||
      lawFinished.Empty () == false))
   {
    CDate ctrlDate =
      rCBegDEndDBegN.wantSortByDateReturned ? sess.returned : sess.date ;

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
    if (sess.key.type == TYPE_FIRM_LAW)
      if (! strchr (KIND_CLOSED_SESSIONS, sess.kind))
        ctrlDate = sess.date ;
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE
    if (ctrlDate >= lawFinished)
      return true ;
   }
  return false ;
 }  // closedSessBookColumns :: mayShowDateFinished

void closedSessBookColumns :: makeColumns (long int serialNo,
                                           const TSession & sess,
                                           TLawsuit & lawsuit,
                                   const collBegDEndDBegN & rCBegDEndDBegN
                                          )
 {
  const size_t buffSz = MAKE_COLUMNS_BUFF_SIZE ;
  const size_t buffL = buffSz - 1 ;
  char * buff = new char [buffSz] ;
  column * pColumn ;  // To make looking of the program text more easy

  if (! columns)
    return ;
  if (! buff)
    return ;
  buff [buffSz - 1] = '\0' ;
  if ((pColumn = columns [iC_SerialNo]) != NULL)
   {  // -- iC_SerialNo --
    ColumnXmlEncloser encloser(pColumn, iC_SerialNo + bOC, pItems);//21.jan.2005

    ostrstream (buff, buffL) << serialNo << ends ;
    pColumn -> addText (buff) ;
   }  // -- iC_SerialNo --
  if ((pColumn = columns [iC_Composition]) != NULL)
   {  // -- iC_Composition --
    ColumnXmlEncloser encloser(pColumn, iC_Composition + bOC, pItems);//21.jan.2005

    compositionToString (sess.composition, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_Composition --
  if ((pColumn = columns [iC_LawsuitKind]) != NULL)
   {  // -- iC_LawsuitKind --
    ColumnXmlEncloser encloser(pColumn, iC_LawsuitKind + bOC, pItems);//21.jan.2005

    kindToString (lawsuit.kind, buff, buffL) ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
    if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind) &&
        (strchr (KIND_PRIVATE_COMPLAINTS, lawsuit.sourceKind) ||
         strchr (KIND_PRIVATE_OBJECTIONS, lawsuit.sourceKind)))
     {
      const size_t l = strlen (buff) ;

      if (buffL > l)
        ostrstream (buff + l, buffL - l) << " " << "Ч" << ends ;
     }
#   endif  // of APPEAL
# endif
      pColumn -> addText (buff) ;
   }  // -- iC_LawsuitKind --
  if ((pColumn = columns [iC_LawNoAndYear]) != NULL)
   {  // -- iC_LawNoAndYear --
    ColumnXmlEncloser encloser(pColumn, iC_LawNoAndYear + bOC, pItems);//21.jan.2005

    ostrstream (buff, buffL)
      << sess.key.no << "/" << sess.key.year << ends ;
    pColumn -> addText (buff) ;
# if ! INSTANCE
    addCompositionToColumn (sess.composition, pColumn) ;
# endif
    if (mayShowDateFinished (lawsuit.finished, sess, rCBegDEndDBegN))
      addDateFinishedToColumn (pColumn, buff, buffL, lawsuit, false) ;
   }  // -- iC_LawNoAndYear --
// 20070201 -->
  if ((pColumn = columns [iC_CS_NF_LawNoAndYear]) != NULL)
   {  // -- iC_LawNoAndYear --
    ColumnXmlEncloser encloser(pColumn, iC_LawNoAndYear + bOC, pItems);//21.jan.2005

    ostrstream (buff, buffL)
      << sess.key.no << "/" << sess.key.year << ends ;
    pColumn -> addText (buff) ;
    if (mayShowDateFinished (lawsuit.finished, sess, rCBegDEndDBegN))
      addDateFinishedToColumn (pColumn, buff, buffL, lawsuit, false) ;
   }  // -- iC_CS_NF_LawNoAndYear --
// 20070201 <--
  if ((pColumn = columns [iC_Clause]) != NULL)
   {  // -- iC_Clause --
    ColumnXmlEncloser encloser(pColumn, iC_Clause + bOC, pItems); // 21.jan.2005

    subjectToString (lawsuit.key.type, lawsuit.subject, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_Clause --
  if ((pColumn = columns [iC_Date]) != NULL)
   {  // -- iC_Date --
    ColumnXmlEncloser encloser(pColumn, iC_Date + bOC, pItems);  // 21.jan.2005

    CDateToString (sess.date, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_Date --
  if ((pColumn = columns [iC_Judge]) != NULL)
   {  // -- iC_Judge --
    ColumnXmlEncloser encloser(pColumn, iC_Judge + bOC, pItems);  // 21.jan.2005

    if (! strIsBlank (sess.judge))
      if (citizenUCNAndUCNTypeToName (sess.judge, UCN_CITIZEN_UCN, buff,
                                      buffL, true, true))
        pColumn -> addText (buff) ;
      else
        pColumn -> addText (Q3_STRING) ;
    else
     pColumn -> addText ("-") ;
   }  // -- iC_Judge --
  if ((pColumn = columns [iC_LawsuitText]) != NULL)
   {  // -- iC_LawsuitText --
    ColumnXmlEncloser encloser(pColumn, iC_LawsuitText + bOC, pItems);//21.jan.2005

    if (lawsuit.text [0])
      pColumn -> addText (lawsuit.text) ;
   }  // -- iC_LawsuitText --
  if ((pColumn = columns [iC_LawNoYearKind]) != NULL)
   {  // -- iC_LawNoYearKind --
    ColumnXmlEncloser encloser(pColumn, iC_LawNoYearKind + bOC, pItems);//21.jan.2005

    ostrstream (buff, buffL)
      << sess.key.no << "/" << sess.key.year << ends ;
    pColumn -> addText (buff) ;
        kindToString (lawsuit.kind, buff, buffL) ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
        if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind) &&
            (strchr (KIND_PRIVATE_COMPLAINTS, lawsuit.sourceKind) ||
             strchr (KIND_PRIVATE_OBJECTIONS, lawsuit.sourceKind)))
         {
          const size_t l = strlen (buff) ;

          if (buffL > l)
            ostrstream (buff + l, buffL - l) << " " << "Ч" << ends ;
         }
#   endif  // of APPEAL
# endif
        pColumn -> addText (buff) ;
# if ! INSTANCE
    addCompositionToColumn (sess.composition, pColumn) ;
# endif
    if (mayShowDateFinished (lawsuit.finished, sess, rCBegDEndDBegN))
      addDateFinishedToColumn (pColumn, buff, buffL, lawsuit, false) ;
   }  // -- iC_LawNoYearKind --
  if ((pColumn = columns [iC_LawNoYear_Appeal]) != NULL)   // 18022 ? used for APPEAL COURT only ?
   {  // -- iC_LawNoYear_Appeal --
    ColumnXmlEncloser encloser(pColumn, iC_LawNoYear_Appeal + bOC, pItems);//21.jan.2005

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
    ostrstream (buff, buffL)
	<< sess.key.no << "/" << sess.key.year << ends ;
    pColumn -> addText (buff) ;
    kindToString (lawsuit.kind, buff, buffL) ;
    if ((strchr (KIND_PRIVATE_COMPLAINTS, lawsuit.sourceKind) ||
         strchr (KIND_PRIVATE_OBJECTIONS, lawsuit.sourceKind)))
     {
      const size_t l = strlen (buff) ;

      if (buffL > l)
        ostrstream (buff + l, buffL - l) << " " << "Ч" << ends ;
     }
    // pColumn -> addText (buff) ;

    // 20070502 no comment
    // buff[0]='\0';
    // addRelatedLawsToColumn ( pColumn, lawsuit, buff, buffL, true );  // 200701

//#   if (COURT_TYPE == COURT_APPEAL) // 20070927 -->
//    if (  (lawsuit.kind != KIND_CASS_CITIZEN_LAW) && (lawsuit.kind != KIND_CASS_TRADE_LAW)
//        &&(lawsuit.kind != KIND_PRIVATE_CASS_CITIZEN_LAW) && (lawsuit.kind != KIND_PRIVATE_CASS_TRADE_LAW) )
//#   endif                           // 20070929 <--
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY // 20070927 -->
    // 20070502  -->
    {
	 TLawsuit * pL = new TLawsuit ;
	 pL -> key = lawsuit.key;
	 TConnect *c = get_connect(pL, CONNECT_TYPE_LOWER_INSTANCES);
	 if ( c != NULL )
	 {
	     ConnectType->Seek( c -> connectType );

           const size_t l = strlen (buff) ;

           if (buffL > l)
               ostrstream (buff + l, buffL - l)
               << " по "
               << c -> connectNo << "/" << c -> connectYear << "г"
               << ends ;
	 }
	 delete c;
	 delete pL;
    }
    // 20070502  <--
#endif  // ADMIN || AREAL || MILITARY                        // 20070929 <--

    pColumn -> addText (buff) ;
#endif  // APPEAL || ADMIN || AREAL || MILITARY


   }  // -- iC_LawNoYear_Appeal --
// iC_FromCourt
  if ((pColumn = columns [iC_FromCourt]) != NULL)   // 18023
   {  // -- iC_FromCourt --
    ColumnXmlEncloser encloser(pColumn, iC_FromCourt + bOC, pItems);//21.jan.2005

# if INSTANCE
#   if APPEAL                                             // 20070501
    if (lawsuitToSenderName (lawsuit, buff, buffL, true)) // courtNameOnly = true
      pColumn -> addText (buff) ;
#   endif
# endif
   }  // -- iC_FromCourt --
  if ((pColumn = columns [iC_InDoc]) != NULL)
   {  // -- iC_InDoc --
    ColumnXmlEncloser encloser(pColumn, iC_InDoc + bOC, pItems);  // 21.jan.2005

    kindToString (lawsuit.sourceKind, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_InDoc --
  if (columns [iC_SessionResultAndText] ||                        //18011 new format
      columns [iCF_WithDecision] || columns [iCF_Canceled])
   {
    SessionEndDocs endDocs (sess.key, sess.date, sess.kind) ;
    TDecision * pDec = new TDecision ;
    const bool exact1DecFound =
      (endDocs.DocsFound () == 1 && endDocs.NextDoc (* pDec)) ;

    if ((pColumn = columns [iC_SessionResultAndText]) != NULL)
     {  // -- iC_SessionResultAndText --
      ColumnXmlEncloser encloser(pColumn, iC_SessionResultAndText + bOC,
        pItems);  // 21.jan.2005

      TGArray <TSurroundment> surroundments ;  // 05.oct.2001 - begin

      if (DatedContainer_ (session) == DatedContainer_ (sess))
        if (try2surround (& session, & surroundments) == false)
          surroundments.Flush () ;  // 05.oct.2001-end
        //20050323 ->
        {
# if INSTANCE   // AREAL and APPEAL
           TDamn firm ;
           TCompanyType firmType ;
           const bool firmFound = lawsuitToDamn (& lawsuit, & firm) ;
           bool firmTypeFound = false ;
           if (firmFound)
           {
             firmType.companyType = firm.companyType ;
             firmTypeFound = firmType.Get () ;
           }

           pColumn -> addText (firmTypeFound ? firmType.name : "") ;
           pColumn -> addText ( " \n" );
           pColumn -> addText (firmFound ? firm.name : "");
# endif

        } //20050323 <-
      if (exact1DecFound)
       {
        addKindToColumn (pDec -> kind, pColumn) ;
        if (RESULT_CANCELED == sess.result ||
            (sess.key.type == TYPE_PUNISHMENT_LAW &&
             RESULT_CANCEL_FOR_MORE_LOOK_INTO == sess.result))
            addResultToColumn (sess.result, pColumn) ;
	  if (any(pDec -> text))
          pColumn -> addText (pDec -> text) ;
        add_local_surr_ments_to_column (* pColumn, surroundments,
                                        * pDec, lawsuit) ; // 05.oct.2001
       }
      else
        if (endDocs.DocsFound () > 1)
         {
          if (RESULT_CANCELED == sess.result ||
              (sess.key.type == TYPE_PUNISHMENT_LAW &&
               RESULT_CANCEL_FOR_MORE_LOOK_INTO == sess.result))
            addResultToColumn (sess.result, pColumn) ;
          while (endDocs.NextDoc (* pDec))
           {
            addKindToColumn (pDec -> kind, pColumn) ;
		if (any(pDec -> text))
              pColumn -> addText (pDec -> text) ;
            add_local_surr_ments_to_column (* pColumn, surroundments,
                                            * pDec, lawsuit) ;//05.oct.2001
           }
         }
        else
         {
          addResultToColumn (sess.result, pColumn) ;
          if (any(sess.text))
            pColumn -> addText (sess.text) ;
         }
     }  // -- iC_SessionResultAndText --
    if ((pColumn = columns [iCF_WithDecision]) != NULL)
     {  // -- iCF_WithDecision --
      ColumnXmlEncloser encloser(pColumn, iCF_WithDecision + bOC, pItems);  // 21.jan.2005

# if WITH_STARS
      if (sessionDecidesTheLaw (sess, & lawsuit,
                                ! rCBegDEndDBegN.wantSortByDateReturned,
                                exact1DecFound ? pDec : NULL,
                                endDocs.DocsFound () == 0))
        addAgeToColumn (lawsuit.age, pColumn) ;
# endif
     }  // -- iCF_WithDecision --
    if ((pColumn = columns [iCF_Canceled]) != NULL)
     {  // -- iCF_Canceled --
      ColumnXmlEncloser encloser(pColumn, iCF_Canceled + bOC, pItems);//21.jan.2005

# if WITH_STARS
      if (sessionCancelsTheLaw (sess, & lawsuit,
                                ! rCBegDEndDBegN.wantSortByDateReturned,
                                exact1DecFound ? pDec : NULL,
                                endDocs.DocsFound () == 0))
        addAgeToColumn (lawsuit.age, pColumn) ;
# endif
     }  // -- iCF_Canceled --
    delete pDec ;
   }
# if INSTANCE
  if ((pColumn = columns [iC_LawKindWithSuffix]) != NULL)
   {  // -- iC_LawKindWithSuffix --
    ColumnXmlEncloser encloser(pColumn, iC_LawKindWithSuffix + bOC, pItems);//21.jan.2005
    const char * suffix = lawsuitKindSuffix (lawsuit) ;

    ostrstream (buff, buffL) << suffix << " " << ends ;
     {
      const size_t bSrtl = strlen (buff) ;
      const long int bytesLeft = buffL - bSrtl ;

      if (bytesLeft > 0)
        kindToString (lawsuit.kind, buff + bSrtl, bytesLeft) ;
#if COURT_TYPE == COURT_AREAL
	  if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind) &&
		(strchr (KIND_PRIVATE_COMPLAINTS, lawsuit.sourceKind) ||
		 strchr (KIND_PRIVATE_OBJECTIONS, lawsuit.sourceKind)))
	   {
	    const size_t l = strlen (buff) ;

	    if (buffL > l)
		ostrstream (buff + l, buffL - l) << " " << "Ч" << ends ;
	   }
#endif  // AREAL
     }
    pColumn -> addText (buff) ;
   }  // -- iC_LawKindWithSuffix --
  if ((pColumn = columns [iC_FromCourtOrSubjectCode]) != NULL)
   {  // -- iC_FromCourtOrSubjectCode --
    ColumnXmlEncloser encloser(pColumn, iC_FromCourtOrSubjectCode + bOC,
      pItems);//21.jan.2005

#   if APPEAL
    if (lawsuitToSenderName (lawsuit, buff, buffL))
      pColumn -> addText (buff) ;
#   else  // of APPEAL
    if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind))
     {
	addSubjCodeToColumn(pColumn, lawsuit.subject, lawsuit.kind);
	if (lawsuitToSenderName (lawsuit, buff, buffL))
        pColumn -> addText (buff) ;
     }
    else
     {
	ostrstream (buff, buffL) << lawsuit.Code() << ends ;
      pColumn -> addText (buff) ;
     }
#   endif  // of APPEAL
   }  // -- iC_FromCourtOrSubjectCode --
  if ((pColumn = columns [iC_FromCourtOrSubject]) != NULL)
   {  // -- iC_FromCourtOrSubject --
    ColumnXmlEncloser encloser(pColumn, iC_FromCourtOrSubject + bOC, pItems);//21.jan.2005

#   if APPEAL
      if (lawsuitToSenderName (lawsuit, buff, buffL))
        pColumn -> addText (buff) ;
#   else  // of APPEAL
      if (strchr (KIND_2ND_LAWSUITS, lawsuit.kind))
       {
        if (lawsuitToSenderName (lawsuit, buff, buffL))
          pColumn -> addText (buff) ;
       }
      else
       {
        TSubject subj ;

        subj << lawsuit ;
        if (! subj.Get ())
	    clearGroupData (subj) ;
	  ostrstream (buff, buffL)
	    << lawsuit.Code() << " " << subj.name << ends ;
	  pColumn -> addText (buff) ;
	 }
#   endif  // of APPEAL
   }  // -- iC_FromCourtOrSubject --
# else
  if ((pColumn = columns [iC_LawKindWithSuffix]) != NULL)
   {  // -- iC_LawKindWithSuffix --
    ColumnXmlEncloser encloser(pColumn, iC_LawKindWithSuffix + bOC, pItems);//21.jan.2005

    kindToString (lawsuit.kind, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_LawKindWithSuffix --
  if ((pColumn = columns [iC_FromCourtOrSubjectCode]) != NULL)
   {  // -- iC_FromCourtOrSubjectCode --
    ColumnXmlEncloser encloser(pColumn, iC_FromCourtOrSubjectCode + bOC,
	pItems);//21.jan.2005

    ostrstream (buff, buffL) << lawsuit.Code() << ends ;
    pColumn -> addText (buff) ;
   }  // -- iC_FromCourtOrSubjectCode --
# endif
  if ((pColumn = columns [iC_WhenSpeakerHasServe]) != NULL)
   {  // -- iC_WhenSpeakerHasServe --
    ColumnXmlEncloser encloser(pColumn, iC_WhenSpeakerHasServe + bOC, pItems);//21.jan.2005

    CDateToString (sess.returned, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_WhenSpeakerHasServe --
  if ((pColumn = columns [iC_Membership]) != NULL)  // 21.jan.2005
   {  // -- iC_Membership --         18026
    ColumnXmlEncloser encloser(pColumn, iC_Membership + bOC, pItems);
# if ! INSTANCE // 20070201 --> REGIONAL ONLY
    if ( use_area_compositions () )
      if (sess.composition>0)
      {
         char comp_t [SIZE_OF_NAME] ;
         comp_t [0] = '\0' ;
         if (use_area_compositions ())  // 20070806
         {  compositionToString (sess.composition, comp_t, sizeof (comp_t)) ;
            ostrstream (buff, buffL) << " "<< comp_t << " състав" << ends ;
            pColumn -> addText (buff) ;
         }
      }
# else          // 20070201 <-- REGIONAL ONLY

    if(columns[iC_Judge] == NULL || strcmp(sess.president, sess.judge) != 0)
    {
      timeBookColumns :: addMemberToMembership (sess.president, buff, buffL,
        pColumn, JUDGE_STRING, sess.judge) ;
    }
    if(columns[iC_Judge] == NULL || strcmp(sess.judgeX, sess.judge) != 0)
    {
      timeBookColumns :: addMemberToMembership (sess.judgeX, buff, buffL,
        pColumn, JUDGE_STRING, sess.judge) ;
    }
    if(columns[iC_Judge] == NULL || strcmp(sess.reJudgeX, sess.judge) != 0)
    {
      timeBookColumns :: addMemberToMembership (sess.reJudgeX, buff, buffL,
        pColumn, JUDGE_STRING, sess.judge) ;
    }
    timeBookColumns :: addMemberToMembership (sess.juror, buff, buffL,
      pColumn, JUROR_STRING) ;
    timeBookColumns :: addMemberToMembership (sess.reJuror, buff, buffL,
      pColumn, REJUROR_STRING) ;
    timeBookColumns :: addMemberToMembership (sess.jurorX, buff, buffL,
      pColumn, JURORX_STRING) ;
    timeBookColumns :: addMemberToMembership (sess.secretar, buff, buffL,
      pColumn, SECRETAR_STRING) ;
    timeBookColumns :: addMemberToMembership (sess.prosecutor, buff, buffL,
      pColumn, PROSECUTOR_STRING) ;
# endif
   }  // -- iC_Membership --

  // 20060412 - to replace iC_DecEnterDate used in old format ->
  //    iC_CS_NF_DateAnnounced,         // 29  20060412
  //    iC_CS_NF_DL_DateAnnounced,      // 30  20060412 - only for DLawsuits
  if ((pColumn = columns [iC_CS_NF_DateAnnounced]) != NULL)
  {  // -- iC_CS_NF_DateAnnounced --
      ColumnXmlEncloser encloser(pColumn, iC_CS_NF_DateAnnounced + bOC, pItems);
      // like iC_DecEnterDate
      SessionEndDocs endDocs (sess.key, sess.date, sess.kind) ;
      DecPtr pDec ;
      bool decFound = endDocs.NextDoc (pDec.Ref ()) ;
      if ( decFound)
      {
         do
         {
           if (! decFound)
             clearGroupData (pDec.Ref ()) ;
           else
           {
              CDateToString (pDec.Ref ().returned, buff, buffL) ;
              pColumn -> addText (buff) ;
           }
         } while ((decFound = endDocs.NextDoc (pDec.Ref ())) == true) ;
      }
      else
      {
         clearGroupData (pDec.Ref ()) ;
         CDateToString (sess.returned, buff, buffL) ;
         pColumn -> addText (buff) ;
      }
  }

  if ((pColumn = columns [iC_CS_NF_DL_DateAnnounced]) != NULL)
  {  // -- iC_CS_NF_DL_DateAnnounced --  firm lawsuits only
      ColumnXmlEncloser encloser(pColumn, iC_CS_NF_DL_DateAnnounced + bOC, pItems);
      // like iC_DecEnterDate
      SessionEndDocs endDocs (sess.key, sess.date, sess.kind) ;
      DecPtr pDec ;
      bool decFound = endDocs.NextDoc (pDec.Ref ()) ;
      if ( decFound)
      {
         do
         {
           if (! decFound)
             clearGroupData (pDec.Ref ()) ;
           else
           {
              CDateToString (pDec.Ref ().returned, buff, buffL) ;
              pColumn -> addText (buff) ;
           }
         } while ((decFound = endDocs.NextDoc (pDec.Ref ())) == true) ;
      }
      else
      {
         clearGroupData (pDec.Ref ()) ;
         CDateToString (sess.returned, buff, buffL) ;
         pColumn -> addText (buff) ;
      }
  }
  // 20060412 <-

  if(columns[iC_JudicialActNo] || columns[iC_iC_DateJudicialActAnnounced])  //18027 || 18028
  {  // 21.jan.2005
    columnArray synchronized;

    // Because we add rows into a number of columns
    // several times using a loop,
    // it is necesary to take measures against overlapping of
    // contents of columns.
    TStringArray judicialActNoItems;
    TStringArray dateJudicialActAnnouncedItems;

    {
      // Enclosers must be in a block in order to add their closing tags
      // to the arrays prior to moving of contents (after the block)
      ColumnXmlEncloser judicialActNoEncloser(
        columns[iC_JudicialActNo],
        iC_JudicialActNo + bOC, &judicialActNoItems);
      ColumnXmlEncloser dateJudicialActAnnouncedEncloser(
        columns[iC_iC_DateJudicialActAnnounced],
        iC_iC_DateJudicialActAnnounced + bOC, &dateJudicialActAnnouncedItems);

      // The iC_JudicialActNo and iC_iC_DateJudicialActAnnounced must be
      // synchronozed.
      // The adjust_column_rows function deals with NULL columns, too,
      // but let's be accurate.
      if(columns[iC_JudicialActNo])
      {
        synchronized.Add(columns[iC_JudicialActNo]);
      }
      if(columns[iC_iC_DateJudicialActAnnounced])
      {
        synchronized.Add(columns[iC_iC_DateJudicialActAnnounced]);
      }

      {
        SessionEndDocs endDocs(sess.key, sess.date, sess.kind);
        DecPtr pDec;
        TDecision& dec = pDec.Ref();

        //20050407 ->
        if ((pColumn = columns [iC_JudicialActNo]) != NULL)
        {  // -- iC_JudicialActNo --
           const bool exact1DecFound =
                     (endDocs.DocsFound () == 1 && endDocs.NextDoc (* pDec)) ;
           if (exact1DecFound)
           {
              if(dec.eventNo > 0)
              {
                 char tmp[SIZE_OF_NAME + 64];
                 kind_to_alias(dec.kind, tmp, sizeof(tmp));
                 if(PRINT_JUDGEMENTS)
                 {
                    strConcat(tmp, " No ", sizeof(tmp));
                    strConcatLong(tmp, dec.eventNo, sizeof(tmp));
                 }
                 pColumn->addText(tmp);
              }
           }
           else
              if (endDocs.DocsFound () > 1)
              {
                 while (endDocs.NextDoc (* pDec))
                 {
                    if(dec.eventNo > 0)
                    {
                       char tmp[SIZE_OF_NAME + 64];
                       kind_to_alias(dec.kind, tmp, sizeof(tmp));
                       if(PRINT_JUDGEMENTS)
                       {
                          strConcat(tmp, " No ", sizeof(tmp));
                          strConcatLong(tmp, dec.eventNo, sizeof(tmp));
                       }
                       pColumn->addText(tmp);
                    }
                 }
              }
              else
              {
                 if(sess.eventNo > 0)
                 {
                   char tmp[SIZE_OF_NAME + 64];
                   tmp[0] = '\0';
                   //addResultToColumn (sess.result, pColumn) ;
                   if (strchr (RESULT_DECISIONS_CLSESBK, sess.result))
                   {
                      strCopy(tmp, "Решение", sizeof(tmp));
                   }
                   else if((sess.result==RESULT_DECREE)||
                           (sess.result==RESULT_DECREE_1)
# if INSTANCE
                           ||(sess.result==RESULT_DECREE_TERMINATING)
# endif

                           )
                   {
                      strCopy(tmp, "Разпореждане", sizeof(tmp));
                   }
                   else
                   {
                      strCopy(tmp, "Определение", sizeof(tmp));
                   }
                   if (PRINT_JUDGEMENTS)
                   {
                       strConcat(tmp, " No ", sizeof(tmp));
                       strConcatLong(tmp, sess.eventNo, sizeof(tmp));
                   }
                   pColumn->addText(tmp);
                 }
              }
        } //if ((pColumn = columns [iC_JudicialActNo]) != NULL)
        //20050407 <-

        while(endDocs.NextDoc(dec))
        {
/* 20050407 -->
          if ((pColumn = columns [iC_JudicialActNo]) != NULL)
          {  // -- iC_JudicialActNo --
            char tmp[SIZE_OF_NAME + 64];

            kind_to_alias(dec.kind, tmp, sizeof(tmp));

            if((dec.eventNo > 0)&&
               PRINT_JUDGEMENTS)
            {
               strConcat(tmp, " No ", sizeof(tmp));
               strConcatLong(tmp, dec.eventNo, sizeof(tmp));
            }
            pColumn->addText(tmp);
          }  // -- iC_JudicialActNo --
<--  20050407 */
          if ((pColumn = columns [iC_iC_DateJudicialActAnnounced]) != NULL)
          {  // -- iC_iC_DateJudicialActAnnounced --
            add_date_to_column(pColumn, dec.fromDate);
          }  // -- iC_iC_DateJudicialActAnnounced --

          adjust_column_rows(synchronized);
        }
      }
      if(sess.eventNo > 0)
      {
/* 20050407 -->
        if ((pColumn = columns [iC_JudicialActNo]) != NULL)
        {  // -- iC_JudicialActNo --
          char tmp[SIZE_OF_NAME + 64];

          strCopy(tmp, "Определение", sizeof(tmp));
          if (PRINT_JUDGEMENTS)
          {
            strConcat(tmp, " No ", sizeof(tmp));
            strConcatLong(tmp, sess.eventNo, sizeof(tmp));
          }
          pColumn->addText(tmp);
        }  // -- iC_JudicialActNo --
<--  20050407 */
        if ((pColumn = columns [iC_iC_DateJudicialActAnnounced]) != NULL)
        {  // -- iC_iC_DateJudicialActAnnounced --
          add_date_to_column(pColumn, sess.returned);  // sess.date ?
        }  // -- iC_iC_DateJudicialActAnnounced --
      }

      adjust_column_rows(synchronized);
    }
    moveArrayContents(judicialActNoItems);
    moveArrayContents(dateJudicialActAnnouncedItems);
  }  // columns[iC_JudicialActNo] || columns[iC_iC_DateJudicialActAnnounced]

# if INSTANCE
  if (columns [iC_DecResultAndDamnName] || columns [iC_DecEnterDate] ||
      columns [iCF_DamnWithDec] || columns [iCF_DamnCanceled]
      || columns [iC_DecResultAndDamnName_Short]
     )
   {
    // 21.jan.2005 - begin
    // Because we add rows into a number of columns
    // several times using a loop,
    // it is necesary to take measures against overlapping of
    // contents of columns.
    TStringArray decResultAndDamnNameItems;
    TStringArray decEnterDateItems;
    TStringArray damnWithDecItems;
    TStringArray damnCanceledItems;
    TStringArray decResultAndDamnName_ShortItems;

    {
      // Enclosers must be in a block in order to add their closing tags
      // to the arrays prior to moving of contents (after the block)
      ColumnXmlEncloser decResultAndDamnNameEncloser(
        columns[iC_DecResultAndDamnName],
        iC_DecResultAndDamnName + bOC, &decResultAndDamnNameItems);
      ColumnXmlEncloser decEnterDateEncloser(columns[iC_DecEnterDate],
        iC_DecEnterDate + bOC, &decEnterDateItems);
	ColumnXmlEncloser damnWithDecEncloser(columns[iCF_DamnWithDec],
        iCF_DamnWithDec + bOC, &damnWithDecItems);
      ColumnXmlEncloser damnCanceledEncloser(columns[iCF_DamnCanceled],
        iCF_DamnCanceled + bOC, &damnCanceledItems);
      ColumnXmlEncloser decResultAndDamnNameShrtEncloser(
        columns [iC_DecResultAndDamnName_Short],
        iC_DecResultAndDamnName_Short + bOC, &decResultAndDamnName_ShortItems);
      // 21.jan.2005 - end

      SessionEndDocs endDocs (sess.key, sess.date, sess.kind) ;
      DecPtr pDec ;
      bool decFound = endDocs.NextDoc (pDec.Ref ()) ;
      TGArray <TSurroundment> surroundments ;  // 05.oct.2001 - begin

      if (columns [iC_DecResultAndDamnName])
        if (DatedContainer_ (session) == DatedContainer_ (sess))
          if (try2surround (& session, & surroundments) == false)
            surroundments.Flush () ;  // 05.oct.2001-end
      do
       {
        if (! decFound)
          clearGroupData (pDec.Ref ()) ;
        if ((pColumn = columns [iC_DecResultAndDamnName]) != NULL)    //18017  стар формат
         {
             add_dec_res_damn_name_to_column (pColumn, lawsuit, buff, buffSz,
                                           pDec, decFound, sess, true) ;

             add_local_surr_ments_to_column (* pColumn, surroundments,
                                          * pDec, lawsuit) ;
         }  // -- iC_DecResultAndDamnName --
        if ((pColumn = columns [iC_DecEnterDate]) != NULL)
         {
          if (decFound)
           {
            CDateToString (pDec.Ref ().enterDate, buff, buffL) ;
            pColumn -> addText (buff) ;
           }
         }  // -- iC_DecEnterDate --
  #   if APPEAL
  #   else  //  AREAL only ->
        if (columns [iCF_DamnWithDec] || columns [iCF_DamnCanceled])
         {
          const bool canceled =
             strchr (RESULT_FIRM_CLSBK_MUST_INCLUDE,
                     sess.result) ||  // This will work until the
              // RESULT_FIRM_CLSBK_MUST_INCLUDE string contains
              // canceling results only.
            (pDec.Ref ().enterDate.Empty () == false &&
             (strchr (RESULT_FIRM_CANCELEDS, pDec.Ref ().result) ||
              pDec.Ref ().result == RESULT_FIRM_ERASING
             )
            ) ;
          const char * const yesStr = "да" ;

          if ((pColumn = columns [iCF_DamnWithDec]) != NULL)
           {
  # if WITH_STARS
            if (! canceled)
              pColumn -> addText (yesStr) ;
  # endif
           }  // -- iCF_DamnWithDec --
          if ((pColumn = columns [iCF_DamnCanceled]) != NULL)
           {
  # if WITH_STARS
            if (canceled)
              pColumn -> addText (yesStr) ;
  # endif
           }  // -- iCF_DamnCanceled --
         }
        if ((pColumn = columns [iC_DecisionDate]) != NULL)
         {  // -- iC_DecisionDate --
          CDateToString (pDec.Ref ().fromDate, buff, buffL) ;
          pColumn -> addText (buff) ;
         }  // -- iC_DecisionDate --
  #   endif  //  AREAL only <-
        if ((pColumn = columns [iC_DecResultAndDamnName_Short]) != NULL)  //18025  old format
         {
          add_dec_res_damn_name_to_column (pColumn, lawsuit, buff, buffSz,
                                           pDec, decFound, sess, false) ;
         }  // -- iC_DecResultAndDamnName_Short --
       }
      while ((decFound = endDocs.NextDoc (pDec.Ref ())) == true) ;
    }
    moveArrayContents(decResultAndDamnNameItems);  // 21.jan.2005 - begin
    moveArrayContents(decEnterDateItems);
    moveArrayContents(damnWithDecItems);
    moveArrayContents(damnCanceledItems);
    moveArrayContents(decResultAndDamnName_ShortItems);  // 21.jan.2005 - end
   }  // (columns [iC_DecResultAndDamnName] ||
      //  columns [iC_DecEnterDate] ||
      //  columns [iCF_DamnWithDec] || columns [iCF_DamnCanceled])
      //  || columns [iC_DecResultAndDamnName_Short]
# endif
  delete [] buff ;
 }  // closedSessBookColumns :: makeColumns

void closedSessBookColumns::moveArrayContents(TStringArray& src)
{ // 25.jan.2005
  if(pItems)
  {
    move_string_array_contents(pItems, &src);
  }
}  // closedSessBookColumns::moveArrayContents

closedSessBookColumns :: closedSessBookColumns (const char * formatFN,
								TWindow * parent,
								long int serialNo,
								TSession & sess,
								TLawsuit & law,
						    const collBegDEndDBegN & cBegDEndDBegN,
# if INSTANCE
								const CDate & decisionED,
# endif
				    TStringArray * itemsPtr  // 21.jan.2005
							     ) :
  columnsArr (formatFN, parent, i_columns, 1 + OFFSET_CLOSED_SESS_BOOK_CIPHER),
  session (sess), rBegDEndDBegN (cBegDEndDBegN),
# if INSTANCE
  decisionEnterDate (decisionED),
# endif
  pItems (itemsPtr)
 {
  makeColumns (serialNo, sess, law, cBegDEndDBegN) ;
  defineMaxRows () ;
 }  // closedSessBookColumns :: closedSessBookColumns

void closedSessBookColumns :: notConstOutput (ostream & os, bool first)
 {
  if (first)
    columnsArr :: notConstOutput (os, first) ;
  else
   {
    const bookAreas * areasPtr = pBA1 ;
    const int aCnt = areas1Cnt ;
    int i ;

    if (! loadOK)
      return ;
    for (i = 0 ; i < aCnt && os.good () ; i ++)
     {  // -- 0 --
      const size_t len = areasPtr [i].areaLen + areasPtr [i].from ;
      char * s = new char [len + 1] ;
      const int algn = areasPtr [i].align ;
      const size_t skip = areasPtr [i].from ;
      const unsigned int dCiph = areasPtr [i].dataCipher ;
      TSession * pTmpSess = new TSession ;

      (* pTmpSess) << session ;
      s [0] = '\0' ;
      if (rBegDEndDBegN.wantSortByDateReturned)
       {  // -- Boza_0 ... --
        const CDate d = pTmpSess -> date ;

# if INSTANCE  
        if (enterDateNeed (rBegDEndDBegN))
          pTmpSess -> date = decisionEnterDate ;
        else
         {
          pTmpSess -> date = pTmpSess -> returned ;
          pTmpSess -> returned = d ;
         }
# else
        pTmpSess -> date = pTmpSess -> returned ;
        pTmpSess -> returned = d ;
# endif
       }  // -- Boza_0 ... --
# if INSTANCE 
#   if APPEAL
#   else  // of APPEAL
      if (dCiph == MISC_OUTPUT_STRING_UNCHANGED)
       {  // -- Boza_1 ... --
        if (pTmpSess -> key.type == TYPE_FIRM_LAW)
          if (pTmpSess -> date.Empty () == false)
	     {
		strcpy(s, "Месец");
		 {
		  const int l = strlen (s) ;
		  scSprintf (len + 1 - l, s + l, " ") ;
		 }
		 {
		  const int l = strlen (s) ;
              monthToString (pTmpSess -> date.month,
                             s + l, len + 1 - l) ;
             }
             {
              const int l = strlen (s) ;
              scSprintf (len + 1 - l, s + l, " %d г.",
                         pTmpSess -> date.year) ;
             }
           }
          else
            scSprintf (len + 1, s, "%s",
                       "Съдебни актове, които не се вписват "
                       "в Търговския регистър") ;

       }  // -- Boza_1 ... --
#   endif  // of APPEAL
# endif  // of INSTANCE
      if (! monthOutput (os, s, len, algn, skip, dCiph) &&
	    ! sessionOutput ((* pTmpSess), os, s, len, algn, skip, dCiph) &&
	    ! miscOutput (os, s, len, algn, skip, dCiph))
	  {  // -- 1 --
	   if (len)
	     scSprintf (len + 1, s, "?") ;
         s [len] = '\0' ;
         writeString (os, s, len, algn, skip) ;
        }  // -- 1 --
       delete [] s ;
       delete pTmpSess ;
      }  // -- 0 --   (for)
   }
 }  // closedSessBookColumns :: notConstOutput

// ====================== closedSessBookOutFile - begin ======================

class closedSessBookOutFile : public frmHeadOutText
 {
  protected :
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
  public :
    closedSessBookOutFile (TWindow * parent,
				   const char * hFName = NULL) ;
    int formattedHead (const collBegDEndDBegN & rCollBegDEndDBegN,
			     bool write = true,
			     ostream* altStreamPtr = NULL  // 25.jan.2005
			    ) ;
    void writeItem (closedSessBookColumns & cSBC, bool first) ;
    void IncItemLines(bool first);  // 25.jan.2005
    void IncLines(int n);  // 25.jan.2005
 } ;  // class closedSessBookOutFile

closedSessBookOutFile :: closedSessBookOutFile (TWindow * parent,
								const char * hFName) :
			     frmHeadOutText (parent, hFName)
 {
 }  // closedSessBookOutFile :: closedSessBookOutFile

void closedSessBookOutFile::fHOutput(const void* p, char* s, size_t len,
  int algn, size_t skip, int dCipher)
{  // An old varianth in VersionArchives\2365\_appname.j --> _clsesbk.cpp
  if(p != NULL)
  {
    const collBegDEndDBegN* pCBDEDBN =
      reinterpret_cast<const collBegDEndDBegN*>(p);
    ostream* osPtr = pAltStream ? pAltStream : this;

    collBegDEndDBegNOutput(*pCBDEDBN, *osPtr, s, len, algn, skip, dCipher);
  }
}  // closedSessBookOutFile :: fHOutput

int closedSessBookOutFile ::
      formattedHead (const collBegDEndDBegN & rCollBegDEndDBegN, bool write,
                     ostream* altStreamPtr  // 25.jan.2005
                    )
 {  // An old varianth in VersionArchives\2365\_appname.j --> _clsesbk.cpp
  int res = 0;

  pAltStream = altStreamPtr;
  try
  {
    //collBegDEndDBegN tmp = rCollBegDEndDBegN ;  // never used

    //res = formHead (& tmp, write) ;  // never used
    res = formHead(&rCollBegDEndDBegN, write);
    // Simulate text output - in order to have a rough approximation
    // to the length of the page.
    if(write == false && pAltStream != NULL)
    {
      pM.incLines(res);
    }
  }
  CATCH_ANYTHING
  pAltStream = NULL;
  return res;
 }  // closedSessBookOutFile :: formattedHead

void closedSessBookOutFile :: writeItem (closedSessBookColumns & cSBC,
                                         bool first)
 {
// 10.may.2000 - begin
  if (pM.newPageNeed ())
    toBeContinued () ;
// 10.may.2000 - end
  cSBC.notConstOutput (* this, first) ;
  pM.incLines (linesOfItem (first)) ;
 }  // closedSessBookOutFile :: writeItem

void closedSessBookOutFile::IncItemLines(bool first)
{  // 25.jan.2005
  pM.incLines(linesOfItem(first));
}

void closedSessBookOutFile::IncLines(int n)
{  // 25.jan.2005
  pM.incLines(n);
}

// ====================== closedSessBookOutFile - end ======================

class TClosedSessionBookThread : public TWaitThread
 {
  protected :
    collBegDEndDBegN &cBegDEndDBegN ;
    char formatFN [128] ;
    char criteria [2 * 1024] ;  // 18.dec.2003 (2 *)
    TCountRecsQuery * pQ ;  // TQuery * pQ ;  // 27.may.2002
    closedSessBookOutFile of;

    bool fakeRun;  // 26.may.2003
    void makeBookFormatFN () ;
    bool makeCriteria () ;
    void makeQuery () ;
    char * makeSummaryFormatFN (const char * bookFFN) ;
	// returns a pointer to a newly created string or NULL
# if INSTANCE
#   if APPEAL
    void appealNonPunishmentSummary (const char * const formatFNS,
					  const collCompBegDEndDBegN & rCCBDEDBN) ;
    void appealPunishmentSummary (const char * const formatFNS,
		    const collCompBegDEndDBegN & rCCBDEDBN) ;
#   else  // of APPEAL
    void arealPunishmentSummary (const char * const formatFNS,
					   const collCompBegDEndDBegN & rCCBDEDBN) ;
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE
    virtual void writeSummary (const char * bookFFN) ;
    virtual void Execute();
    void WriteHeader(); //25.jan.2005
    void WriteOneSession(long int serialNo, TSession& session,
	TLawsuit& law,
	int& currMonth, long& currYear
# if INSTANCE
	, const CDate decEnterDate
# endif
	);  // 25.jan.2005
    bool MonthRowNeed(
# if INSTANCE
	const CDate decEnterDate,
# endif
	int& currMonth, long& currYear, const TSession& session);  // 25.jan.2005
    void ChangePage();  // 25.jan.2005
  public :
    TClosedSessionBookThread(collBegDEndDBegN &ref_cBegDEndDBegN);
    virtual ~TClosedSessionBookThread ();
    virtual void printClosedSessBook () ;
 } ;

void TClosedSessionBookThread :: makeBookFormatFN ()
 {
  const bool doubtful = cBegDEndDBegN.jan2005Format ;  // 24.jan.2005 - begin
  const char * nameSuffix = "col" ;  // Be sure it is 3 symbols long!
									 // 24.jan.2005 - end
   {
    ostrstream oss (formatFN, sizeof (formatFN));

    oss << "text\\clsb" << cBegDEndDBegN.college ;

    // We have to support the following formats:
    // - the old book format as text file
    // - the new book format as text file
    if (doubtful)  // 24.jan.2005 - begin
     {  // the new books format
	 // text
	 nameSuffix = "_t_" ;
     }
    oss << nameSuffix ;
    oss << DEFAULT_FORMAT_FILE_EXTENSION << ends ;
   }
  ENDING_ZERO (formatFN) ;
# if INSTANCE
#   if APPEAL
#   else     // AREAL
   {
    const int bPos = 8 ;

    if (cBegDEndDBegN.wantShortFormat &&
	  strlen (formatFN) > bPos && formatFN [bPos] == 'b')
	formatFN [bPos] = 'e' ;
   }
#   endif
# endif
 }

bool TClosedSessionBookThread :: makeCriteria ()
 {
  return composeClosedSessBSCriteria (criteria, sizeof (criteria),
	    "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_DATE, S.F_KIND, S.F_RETURNED",
						  cBegDEndDBegN) ;
 }

void TClosedSessionBookThread :: makeQuery ()
 {
  if (makeCriteria ())
    pQ = new TCountRecsQuery (criteria) ;
 }

TClosedSessionBookThread :: TClosedSessionBookThread (collBegDEndDBegN &ref_cBegDEndDBegN) :
  cBegDEndDBegN(ref_cBegDEndDBegN), pQ (NULL), of(wtdl->Parent, NULL)
 {
  fakeRun = false;  // 26.may.2003
  formatFN[0] = '\0';  // 25.jan.2005

   {
# if INSTANCE
#   if APPEAL
#   else      // AREAL
    if (cBegDEndDBegN.college == TYPE_FIRM_LAW)
	cBegDEndDBegN.wantShortFormat =
	  ! ask ("Потвърдете отпечатването на всички колони от книгата") ;
#   endif
# endif
    makeBookFormatFN () ;
    of.initFF (formatFN);
   }
 }

TClosedSessionBookThread :: ~TClosedSessionBookThread ()
 {
  if (pQ)
    delete pQ ;
 }  // TClosedSessionBookThread :: TClosedSessionBookThread

char * TClosedSessionBookThread :: makeSummaryFormatFN (const char * bookFFN)
  // returns a pointer to a newly created string or NULL
 {
  const int bPos = 8 ;

  if (bookFFN && strlen (bookFFN) > 9 &&
	(bookFFN [bPos] == 'b'
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
				     || bookFFN [bPos] == 'e'
#   endif  // of APPEAL
# endif  // INSTANCE
	))
   {  // -- 0 --
    char * formatFN = new char [strlen (bookFFN) + 1] ;

    if (formatFN)
     {
	strcpy (formatFN, bookFFN) ;
	formatFN [bPos] = 's' ;
     }
    return formatFN ;
   }  // -- 0 --
  else
    return NULL ;
 }  // TClosedSessionBookThread :: makeSummaryFormatFN

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
class TClSessArealPunSummaryThread : public TCheckupThread
{
	protected:
    const char * const formatFNS;
		const collCompBegDEndDBegN & rCCBDEDBN;
    const collBegDEndDBegN & cBegDEndDBegN ;
	TCountRecsQuery* const pQ;
    CodeValueArray& codeValueArr;
    virtual void Execute();
    virtual bool ArgsOk();
  public:
	TClSessArealPunSummaryThread(const char* const fmtFNS, const collCompBegDEndDBegN& cCBDEDBN,
	const collBegDEndDBegN& rCBegDEndDBegN, TCountRecsQuery* const qPtr,
	CodeValueArray& codeValArr);
};

TClSessArealPunSummaryThread::
TClSessArealPunSummaryThread(const char* const fmtFNS, const collCompBegDEndDBegN& cCBDEDBN,
  const collBegDEndDBegN& rCBegDEndDBegN, TCountRecsQuery* const qPtr,
  CodeValueArray& codeValArr):
    formatFNS(fmtFNS), rCCBDEDBN(cCBDEDBN), cBegDEndDBegN(rCBegDEndDBegN),
    pQ(qPtr), codeValueArr(codeValArr)
{
}

bool TClSessArealPunSummaryThread::ArgsOk()
{
	return (TCheckupThread::ArgsOk() && pQ != NULL &&
	formatFNS && formatFNS [0] &&
    strchr (rCCBDEDBN.colleges, TYPE_PUNISHMENT_LAW));
}

void TClSessArealPunSummaryThread::Execute()
{
 try
  {
    long int vExactTotalSessions = 0 ;
    long int vTotalSessions = 0 ;
    long int vFin1stTotal = 0 ;       //
    long int vDec1stNonPreCourt = 0 ;
    long int vCanc1stNonPreCourt = 0 ;
    long int vFin1stPreCourt = 0 ;    //
    long int vNonFin1st = 0 ;
    long int vFin2ndTotal = 0 ;       //
    long int vFin2ndCONonPreCourt = 0 ;
    long int vFin2ndPrvCPrvONonPreCourt = 0 ;
    long int vFin2ndPreCourt = 0 ;    //
    long int vFin2nd237 = 0 ;
    long int vNonFin2nd = 0 ;

    totalPartSteps = pQ->getRecsCount();
    wtdl->SetHeader("Данни за рекапитулация "
			    "(заседания)");
    wtdl->PleaseWait();
    endingVal = totalPartSteps;
    wtdl->SetProgressRange(0, totalPartSteps);
//    currentTickRate = 5; // works, but unnecessary // Gossip service
    pQ -> Restart () ;
     {  // -- 2 --
	TSession * pSess = new TSession ;

	while (pQ -> Read ())
			 {  // -- 3 --
				Tick(false);
				if(wtdl->WantBreak())
					break;
				(* pSess) << (* pQ) ;
				if (pSess -> Get ())
				 {  // -- 4 --
					vExactTotalSessions ++ ;

          TLawsuit law ;
          const CDate dateField =
            cBegDEndDBegN.wantSortByDateReturned ?
              pSess -> returned : pSess -> date ;

          law << (* pQ) ;  // Do not write "law << (* pSess)" - there
                           // will be "wrong import from field" error
          if (law.Get ())
           {  // -- 5 --
            if (law.finished.Empty () ||
                pSess -> date <= law.finished)
             {  // -- 6 --
              vTotalSessions ++ ;

              if (strchr (RESULT_FINALIZEDS, pSess -> result))
               {  // -- 7 --
                if (law.finished.Empty () == false)
                 {  // -- 8 --
									if (rCCBDEDBN.begD <= law.finished &&
                      law.finished <= rCCBDEDBN.endD)
                    if (dateField <= law.finished)
                     {  // -- 9 --
											if (strchr (KIND_1ST_LAWSUITS, law.kind))
                       {  // -- 10 --
                        vFin1stTotal ++ ;
                        if (law_is_pre_court (law))
                          vFin1stPreCourt ++ ;
				else
				  if (law_is_237_enactment (law))
				    vFin2nd237 ++ ;
                          else
				    if (sessionDecidesTheLaw (* pSess, & law,
                                ! cBegDEndDBegN.wantSortByDateReturned))
                              vDec1stNonPreCourt ++ ;
                            else
                              if (sessionCancelsTheLaw (* pSess, & law,
                                ! cBegDEndDBegN.wantSortByDateReturned))
                                vCanc1stNonPreCourt ++ ;
											 }  // -- 10 --
                      else
                       {  // -- 11 --
                        vFin2ndTotal ++ ;
												if (law_is_pre_court(law))
                          vFin2ndPreCourt ++ ;
                        else
                          if (strchr (KIND_EXACT_COMPLAINTS,
                                      law.sourceKind) ||
                              strchr (KIND_EXACT_OBJECTIONS,
                                      law.sourceKind)
                             )
                            vFin2ndCONonPreCourt ++ ;
                          else
                            if (strchr (KIND_PRIVATE_COMPLAINTS,
                                        law.sourceKind) ||
                                strchr (KIND_PRIVATE_OBJECTIONS,
                                        law.sourceKind)
                               )
                              vFin2ndPrvCPrvONonPreCourt ++ ;
				    else
					if (law_is_237_enactment (law))
						vFin2nd237 ++ ;
			     }  // -- 11 --
										 }  // -- 9 --
		     }  // -- 8 --
               }  // -- 7 --
              else
               {
                if (strchr (KIND_1ST_LAWSUITS, law.kind))
                  vNonFin1st ++ ;
                else
                  vNonFin2nd ++ ;
		   }
             }  // -- 6 --
           }  // -- 5 --
         }  // -- 4 --
       }  // -- 3 --
			if(wtdl->WantBreak() == false)
				Tick(true);
			wtdl->AlmostDone();
			delete pSess ;
		 }  // -- 2 --
		codeValueArr.Add (new CodeValue (10457, vExactTotalSessions)) ;
		codeValueArr.Add (new CodeValue (10456, vTotalSessions)) ;
		codeValueArr.Add (new CodeValue (10445, vFin1stTotal)) ;
		codeValueArr.Add (new CodeValue (10450, vDec1stNonPreCourt)) ;
		codeValueArr.Add (new CodeValue (10451, vCanc1stNonPreCourt)) ;
		codeValueArr.Add (new CodeValue (10446, vFin1stPreCourt)) ;
		codeValueArr.Add (new CodeValue (10454, vNonFin1st)) ;
		codeValueArr.Add (new CodeValue (10447, vFin2ndTotal)) ;
		codeValueArr.Add (new CodeValue (10452, vFin2ndCONonPreCourt)) ;
		codeValueArr.Add (new CodeValue (10453, vFin2ndPrvCPrvONonPreCourt)) ;
		codeValueArr.Add (new CodeValue (10448, vFin2ndPreCourt)) ;
    codeValueArr.Add (new CodeValue (10389, vFin2nd237)) ;
    codeValueArr.Add (new CodeValue (10455, vNonFin2nd)) ;

    if (WINDOWS_PRINT != 0)
    {  // 09.feb.2005  something like diagram
	if(vTotalSessions > 0)
	{
	  int bNL =  // bar normal length
	    get_section_int(formatFNS, "***_BAR_NORMAL_LEN", 100);

	  if(bNL > 0)
	  {
	    const double coeff = (1.0 * bNL) / vTotalSessions;

	    long int vExactTotalSessionsLen = vExactTotalSessions * coeff;
	    long int vTotalSessionsLen = bNL;  // vTotalSessionsLen * coeff;
	    long int vFin1stTotalLen = vFin1stTotal * coeff;
	    long int vDec1stNonPreCourtLen = vDec1stNonPreCourt * coeff;
	    long int vCanc1stNonPreCourtLen = vCanc1stNonPreCourt * coeff;
	    long int vFin1stPreCourtLen = vFin1stPreCourt * coeff;
	    long int vNonFin1stLen = vNonFin1st * coeff;
	    long int vFin2ndTotalLen = vFin2ndTotal * coeff;
	    long int vFin2ndCONonPreCourtLen = vFin2ndCONonPreCourt * coeff;
	    long int vFin2ndPrvCPrvONonPreCourtLen = vFin2ndPrvCPrvONonPreCourt * coeff;
	    long int vFin2ndPreCourtLen = vFin2ndPreCourt * coeff;
	    long int vFin2nd237Len = vFin2nd237 * coeff;
	    long int vNonFin2ndLen = vNonFin2nd * coeff;

	    codeValueArr.Add(new CodeValue(10597, vExactTotalSessionsLen));  // 10457
	    codeValueArr.Add(new CodeValue(10598, vTotalSessionsLen));       // 10456
	    codeValueArr.Add(new CodeValue(10599, vFin1stTotalLen));         // 10445
	    codeValueArr.Add(new CodeValue(10600, vDec1stNonPreCourtLen));   // 10450
	    codeValueArr.Add(new CodeValue(10601, vCanc1stNonPreCourtLen));  // 10451
	    codeValueArr.Add(new CodeValue(10602, vFin1stPreCourtLen));      // 10446
	    codeValueArr.Add(new CodeValue(10603, vNonFin1stLen));           // 10454
	    codeValueArr.Add(new CodeValue(10604, vFin2ndTotalLen));         // 10447
	    codeValueArr.Add(new CodeValue(10605, vFin2ndCONonPreCourtLen)); // 10452
	    codeValueArr.Add(new CodeValue(10606, vFin2ndPrvCPrvONonPreCourtLen));//10453
	    codeValueArr.Add(new CodeValue(10607, vFin2ndPreCourtLen));      // 10448
	    codeValueArr.Add(new CodeValue(10608, vFin2nd237Len));           // 10389
	    codeValueArr.Add(new CodeValue(10609, vNonFin2ndLen));           // 10455
        }
      }
    }
  }
  CATCH_ANYTHING
}

#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE

# if INSTANCE
#   if APPEAL
class TAppealNonPunCodeValueArrFiller : public TCheckupThread
{  // 21.jun.2002
	public:
		TAppealNonPunCodeValueArrFiller(const collCompBegDEndDBegN & cCBDEDBN, CodeValueArray& codeValArr,
			TCountRecsQuery* pQuery, const collBegDEndDBegN& rCBegDEndDBegN);
	protected:
		const collCompBegDEndDBegN& rCCBDEDBN;
		CodeValueArray& codeValueArr;
		TCountRecsQuery* pQ;
		const collBegDEndDBegN& cBegDEndDBegN;

		virtual void Execute();
		virtual bool ArgsOk();
};

TAppealNonPunCodeValueArrFiller::
TAppealNonPunCodeValueArrFiller(const collCompBegDEndDBegN & cCBDEDBN, CodeValueArray& codeValArr,
	TCountRecsQuery* pQuery, const collBegDEndDBegN& rCBegDEndDBegN) :
		rCCBDEDBN(cCBDEDBN), codeValueArr(codeValArr), pQ(pQuery),
		cBegDEndDBegN(rCBegDEndDBegN)
{
	if(ArgsOk() == false)
		error("%s\n%s::%s", WRONG_ARGS_PASSED_TO,
			typeid(*this).name(), typeid(*this).name());
}

bool TAppealNonPunCodeValueArrFiller::ArgsOk()
{
	return (TCheckupThread::ArgsOk() && pQ &&
		strchr(rCCBDEDBN.colleges, TYPE_PUNISHMENT_LAW) == NULL);
}

void TAppealNonPunCodeValueArrFiller::Execute()
{
	try
	{
		long int vTotal = 0 ;
		long int vFin2ndCitTotal = 0 ;
		long int vDecided2nd = 0 ;  // 21.jun.2001
		long int vDecided2ndBankruptcy = 0 ;
		long int vCanceled2ndBankruptcy = 0 ;
		long int vFin2ndCitCancel = 0 ;
		long int vFinPrvCitTotal = 0 ;
		long int vFinPrvCitPrvCmpl = 0 ;
		long int vFinPrvCitCmplSlowly = 0 ;
		long int vFinPrvPetitions = 0 ;  // 21.jun.2002
		long int vDecidedPrvBankruptcy = 0 ;
		long int vCanceledPrvBankruptcy = 0 ;
		long int vNonFinTotal = 0 ;
		long int vNonFinRes205 = 0 ;  // RESULT_CIT_205
		long int vNonFinDecree = 0 ;  // RESULT_DECREE_1 // 09.apr.2002
		long int vNonFinDefinition = 0 ;  // RESULT_DEFINITION // 09.apr.2002
		long int vAbolishedResults = 0 ;
		long int vAbolishings = 0 ;
		long int vAfterFinishActivity = 0 ;
		long int vAfterFinishRes192_4 = 0 ;
		long int vAfterFinishRes192_5 = 0 ;
		long int vAfterFinishRes37 = 0 ;
		long int vAfterFinishDecree = 0 ;  // RESULT_DECREE_1 // 21.jun.2002
		long int vAfterFinishDefinition = 0 ; //RESULT_DEFINITION //21.jun.2002

		totalPartSteps = pQ->getRecsCount();
		wtdl->SetHeader("Данни за рекапитулация "
			    "(заседания)");
		wtdl->PleaseWait();
		endingVal = totalPartSteps;
		wtdl->SetProgressRange(0, totalPartSteps);
		pQ -> Restart () ;
		{  // -- 2 --
			TSession * pSess = new TSession ;

			while (pQ -> Read ())
			{  // -- 3 --
				Tick(false);
				if(wtdl->WantBreak())
					break;
				(* pSess) << (* pQ) ;
				if (pSess -> Get ())
				{  // -- 4 --
					vTotal ++ ;
					TLawsuit law ;
					const CDate dateField =
						cBegDEndDBegN.wantSortByDateReturned ?
							pSess -> returned : pSess -> date ;

					law << (* pQ) ;  // Do not write "law << (* pSess)" - there
													 // will be "wrong import from field F_TEXT"
													 // error
					if (law.Get ())
					{  // -- 7 --
						if (law.finished.Empty () ||
//                  pSess -> date <= law.finished)    // 09.apr.2002
								pSess -> returned <= law.finished)  // 09.apr.2002
						{  // -- 10 --
							if (strchr (RESULT_FINALIZEDS, pSess -> result))
							{  // -- 5 --
								CDate abolishingDate ;  // for syntax only yet ..

								if (sessResOverridden_AbolishedMove (* pSess,
																										 abolishingDate))
									vAbolishedResults ++ ;
								else
								{  // -- 6 --
									if (law.finished.Empty () == false)
									{  // -- 8 --
										if (rCCBDEDBN.begD <= law.finished &&
												law.finished <= rCCBDEDBN.endD)
											if (dateField <= law.finished)
											{  // -- 9 --
												const bool bankruptcy =
																 bankruptcy_law (law) ;
												const bool decided =
													sessionDecidesTheLaw (* pSess, & law,
															! cBegDEndDBegN.wantSortByDateReturned) ;
												const bool canceled =
													sessionCancelsTheLaw (* pSess, & law,
															! cBegDEndDBegN.wantSortByDateReturned) ;

												if (decided == canceled)
												{
													unknown_finished_law_status (law.key) ;
													continue ;
												}
												switch (law.kind)
												{
													case KIND_CIT_2ND_LAW :
													case KIND_TRA_2ND_LAW :  // 29.jun.2004
														vFin2ndCitTotal ++ ;
														if (canceled)
															if (bankruptcy)
																vCanceled2ndBankruptcy ++ ;
															else
																vFin2ndCitCancel ++ ;
														else
															if (bankruptcy)
																vDecided2ndBankruptcy ++ ;
															else                // 21.jun.2002
																vDecided2nd ++ ;  // 21.jun.2002
														break ;
													case KIND_CIT_PRV_LAW :
													case KIND_TRA_PRV_LAW :  // 29.jun.2004
														vFinPrvCitTotal ++ ;
														if (bankruptcy)
															if (decided)
																vDecidedPrvBankruptcy ++ ;
															else
																vCanceledPrvBankruptcy ++ ;
														else
															if(strchr(KIND_PETITIONS, //21.jun.2002-begin
																law.sourceKind))
																vFinPrvPetitions ++ ;
															else                     // 21.jun.2002-end
																if (strchr (KIND_PRIVATE_COMPLAINTS,
																						law.sourceKind))
																	vFinPrvCitPrvCmpl ++ ;
																else
																	//if (law.sourceKind ==  // 18.jun.2004-begin
																	//		KIND_SLOWDOWN_APPEAL)
																	if (strchr (KIND_SLOW_REQUESTS,
                                    law.sourceKind))       // 18.jun.2004 - end
																		vFinPrvCitCmplSlowly ++ ;
														break ;
												}
											}  // -- 9 --
									}  // -- 8 --
								}  // -- 6 --
							}  // -- 5 --
							else
							{
								vNonFinTotal ++ ;
								if (strchr(RESULT_UNDOS, pSess -> result))
									vAbolishings ++ ;
								if (pSess -> result == RESULT_CIT_205)
									vNonFinRes205 ++ ;
								if (pSess->result == RESULT_DECREE ||  // 09.apr.2002-begin
										pSess -> result == RESULT_DECREE_1)
									vNonFinDecree ++ ;
								if (pSess -> result == RESULT_DEFINITION)
									vNonFinDefinition ++ ;  // 09.apr.2002 - end
							}
						}  // -- 10 --
						else
						{
							vAfterFinishActivity ++ ;
							if (pSess -> result == RESULT_CIT_192_4)
								vAfterFinishRes192_4 ++ ;
							if (pSess -> result == RESULT_CIT_192_5)
								vAfterFinishRes192_5 ++ ;
							if (pSess -> result == RESULT_CIT_37)
								vAfterFinishRes37 ++ ;
							if (pSess->result == RESULT_DECREE ||  // 21.apr.2002 - begin
									pSess -> result == RESULT_DECREE_1)
								vAfterFinishDecree ++ ;
							if (pSess -> result == RESULT_DEFINITION)
								vAfterFinishDefinition ++ ;  // 21.apr.2002 - end
						}
					}  // -- 7 --
				}  // -- 4 --
			}  // -- 3 --
			if(wtdl->WantBreak() == false)
				Tick(true);
			wtdl->AlmostDone();
			delete pSess ;
		}  // -- 2 --
		codeValueArr.Add (new CodeValue (10391, vTotal)) ;
		codeValueArr.Add (new CodeValue (10392, vFin2ndCitTotal)) ;
		codeValueArr.Add (new CodeValue (10393, vFin2ndCitCancel)) ;
		codeValueArr.Add (new CodeValue (10394, vFinPrvCitTotal)) ;
		codeValueArr.Add (new CodeValue (10395, vFinPrvCitPrvCmpl)) ;
		codeValueArr.Add (new CodeValue (10396, vFinPrvCitCmplSlowly)) ;
		codeValueArr.Add (new CodeValue (10397, vNonFinTotal)) ;
		codeValueArr.Add (new CodeValue (10398, vNonFinRes205)) ;
		codeValueArr.Add (new CodeValue (10399, vAfterFinishRes192_4)) ;
		codeValueArr.Add (new CodeValue (10400, vAbolishedResults)) ;
		codeValueArr.Add (new CodeValue (10401, vAbolishings)) ;
		codeValueArr.Add (new CodeValue (10460, vAfterFinishActivity)) ;
		codeValueArr.Add (new CodeValue (10504, vAfterFinishRes192_5)) ;
		codeValueArr.Add (new CodeValue (10505, vAfterFinishRes37)) ;
		codeValueArr.Add (new CodeValue (10531, vDecided2ndBankruptcy)) ;
		codeValueArr.Add (new CodeValue (10532, vCanceled2ndBankruptcy)) ;
		codeValueArr.Add (new CodeValue (10533, vDecidedPrvBankruptcy)) ;
		codeValueArr.Add (new CodeValue (10534, vCanceledPrvBankruptcy)) ;
		codeValueArr.Add (new CodeValue (10583, vNonFinDecree)) ;
		codeValueArr.Add (new CodeValue (10584, vNonFinDefinition)) ;
		codeValueArr.Add (new CodeValue (10579, vDecided2nd)) ;
		codeValueArr.Add (new CodeValue (10580, vFinPrvPetitions)) ;
		codeValueArr.Add (new CodeValue (10581, vAfterFinishDecree)) ;
		codeValueArr.Add (new CodeValue (10582, vAfterFinishDefinition)) ;
	}
	CATCH_ANYTHING
}

void TClosedSessionBookThread ::
appealNonPunishmentSummary (const char * const formatFNS, const collCompBegDEndDBegN & rCCBDEDBN)
{
	if (formatFNS && formatFNS [0])
	{
		if (strchr (rCCBDEDBN.colleges, TYPE_PUNISHMENT_LAW) == NULL)
		{  // -- 0 --
			if (pQ)
			{  // -- 1 --
				CodeValueArray codeValueArr ;
				TAppealNonPunCodeValueArrFiller(rCCBDEDBN, codeValueArr, pQ, cBegDEndDBegN).Generate();
				writeTBSummary(formatFNS, &rCCBDEDBN, wtdl->Parent, &codeValueArr) ;
			}  // -- 1 --
		}  // -- 0 --
	}
}

void TClosedSessionBookThread ::
appealPunishmentSummary (const char * const formatFNS, const collCompBegDEndDBegN & rCCBDEDBN)
{
	if (! (formatFNS && formatFNS [0]))
		return ;
	if (strchr (rCCBDEDBN.colleges, TYPE_PUNISHMENT_LAW))
	 {  // -- 0 --
		if (pQ)
		 {  // -- 1 --
			CodeValueArray codeValueArr ;
			long int vExactTotalSessions = 0 ;
			long int vTotalSessions = 0 ;
			long int vAfterFinish = 0 ;

			pQ -> Restart () ;
			 {  // -- 2 --
				TSession * pSess = new TSession ;
				TWaitWindow ww (wtdl->Parent, PLEASE_WAIT) ;

				while (pQ -> Read ())
				 {  // -- 3 --
					(* pSess) << (* pQ) ;
					if (pSess -> Get ())
					 {  // -- 4 --
						vExactTotalSessions ++ ;

		TLawsuit law ;
		const CDate dateField =
		  cBegDEndDBegN.wantSortByDateReturned ?
		    pSess -> returned : pSess -> date ;

		law << (* pQ) ;  // Do not write "law << (* pSess)" - there
				     // will be "wrong import from field" error
		if (law.Get ())
             {  // -- 5 --
              if (law.finished.Empty () ||
                  pSess -> date <= law.finished)
               {  // -- 6 --
                vTotalSessions ++ ;
               }  // -- 6 --
              else
                vAfterFinish ++ ;
             }  // -- 5 --
	     }  // -- 4 --
	   }  // -- 3 --
        delete pSess ;
       }  // -- 2 --
      if (vAfterFinish + vTotalSessions != vExactTotalSessions)
        error ("vAfterFinish + vTotalSessions != vExactTotalSessions") ;
	codeValueArr.Add (new CodeValue (10462, vExactTotalSessions)) ;
	codeValueArr.Add (new CodeValue (10463, vTotalSessions)) ;
	codeValueArr.Add (new CodeValue (10464, vAfterFinish)) ;
      writeTBSummary (formatFNS, & rCCBDEDBN, wtdl->Parent, & codeValueArr) ;
     }  // -- 1 --
   }  // -- 0 --
 }  // TClosedSessionBookThread :: appealPunishmentSummary

#   else  // of APPEAL
void TClosedSessionBookThread ::
arealPunishmentSummary (const char * const formatFNS,
                        const collCompBegDEndDBegN & rCCBDEDBN)
 {  // An old varianth in VersionArchives\2365\_appname.j --> _clsesbk.cpp
  if (! (formatFNS && formatFNS [0]))
    return ;
  if (strchr (rCCBDEDBN.colleges, TYPE_PUNISHMENT_LAW))
   {  // -- 0 --
    if (pQ)
     {  // -- 1 --
      CodeValueArray codeValueArr ;
      TClSessArealPunSummaryThread(formatFNS, rCCBDEDBN, cBegDEndDBegN, pQ, codeValueArr).Generate();
      writeTBSummary (formatFNS, & rCCBDEDBN, wtdl->Parent, & codeValueArr) ;
     }  // -- 1 --
   }  // -- 0 --
 }

#   endif  // of APPEAL
# else  // INSTANCE
# endif  // INSTANCE

void TClosedSessionBookThread :: writeSummary (const char * bookFFN)
 {
  char * formatFNS = makeSummaryFormatFN (bookFFN) ;
  collCompBegDEndDBegN cCBDEDBN ;

  if (! formatFNS)
    return ;
  if (pQ == NULL)
	makeQuery () ;
  if (pQ == NULL)
    return ;                  // 27.may.2002 - end
  cCBDEDBN << cBegDEndDBegN ;
# if INSTANCE
#   if APPEAL
  if (! strchr (cCBDEDBN.colleges, TYPE_PUNISHMENT_LAW))
  {
    register_usage(REGIME_CLOSED_SESS_SUMMARY, 1, cCBDEDBN.colleges,
	"КЗЗ - Рекапит.");
    appealNonPunishmentSummary (formatFNS, cCBDEDBN) ;
  }
  else
  {
    register_usage(REGIME_CLOSED_SESS_SUMMARY, 2, cCBDEDBN.colleges,
	"КЗЗ - Рекапит.");
    appealPunishmentSummary (formatFNS, cCBDEDBN) ;
  }
#   else  // of APPEAL
  if (strchr (cCBDEDBN.colleges, TYPE_PUNISHMENT_LAW))
  {
    register_usage(REGIME_CLOSED_SESS_SUMMARY, 2, cCBDEDBN.colleges,
	"КЗЗ - Рекапит.");
    arealPunishmentSummary (formatFNS, cCBDEDBN) ;
  }
  else
  {
    register_usage(REGIME_CLOSED_SESS_SUMMARY, 1, cCBDEDBN.colleges,
	"КЗЗ - Рекапит.");
    writeTBSummary (formatFNS, & cCBDEDBN, wtdl->Parent) ;
  }
#   endif  // of APPEAL
# else  // of INSTANCE
  register_usage(REGIME_CLOSED_SESS_SUMMARY, 1, cCBDEDBN.colleges,
    "КЗЗ - Рекапит.");
  writeTBSummary (formatFNS, & cCBDEDBN, wtdl->Parent) ;
# endif  // of INSTANCE
  if (pQ)
    if (cCBDEDBN.wantResultsCheckup)
    {
	register_usage(REGIME_CLOSED_SESS_RES_SUMMARY, 1, cCBDEDBN.colleges,
	  "КЗЗ-Рекап.Рез.");
	results_set_book(wtdl->Parent, cCBDEDBN, pQ, false);
    }

  delete [] formatFNS ;
 }

void TClosedSessionBookThread::WriteHeader()
{
  {
    of.title();
    of.formattedHead(cBegDEndDBegN);
    of.head();
  }
}

bool TClosedSessionBookThread::MonthRowNeed(
# if INSTANCE
  const CDate decEnterDate,
# endif
  int& currMonth, long& currYear, const TSession& session)
{
  bool monthRowNeed = false;

# if INSTANCE
  if(cBegDEndDBegN.wantSortByDateReturned)
  {
    if(enterDateNeed(cBegDEndDBegN))
    {
	if(decEnterDate.month != currMonth ||
	  decEnterDate.year != currYear)
	{
	  monthRowNeed = true;
	  currMonth = decEnterDate.month;
	  currYear = decEnterDate.year;
	}
    }
    else
    {
	if(session.returned.month != currMonth ||
	  session.returned.year != currYear)
	{
	  monthRowNeed = true;
	  currMonth = session.returned.month;
	  currYear = session.returned.year;
	}
    }
  }
  else
  {
    if(session.date.month != currMonth ||
      session.date.year != currYear)
    {
      monthRowNeed = true;
      currMonth = session.date.month;
      currYear = session.date.year;
    }
  }
# else  // of INSTANCE
  if(cBegDEndDBegN.wantSortByDateReturned)
  {
    if(session.returned.month != currMonth ||
      session.returned.year != currYear)
    {
      monthRowNeed = true;
      currMonth = session.returned.month;
	currYear = session.returned.year;
    }
  }
  else
  {
    if(session.date.month != currMonth ||
	session.date.year != currYear)
    {
	monthRowNeed = true;
	currMonth = session.date.month;
	currYear = session.date.year;
    }
  }
# endif
  return monthRowNeed;
}

void TClosedSessionBookThread::ChangePage()
{
  {
    of.footherLine () ;
    of.newPage () ;
    of.formattedHead (cBegDEndDBegN) ;
    of.head () ;
  }
}

void TClosedSessionBookThread::WriteOneSession(long int serialNo, TSession& session,
  TLawsuit& law,
  int& currMonth, long& currYear
# if INSTANCE
  , const CDate decEnterDate
# endif
  )
{

  TStringArray* pXmlItems = NULL;

  closedSessBookColumns cSBC(formatFN, wtdl->Parent, serialNo, session, law,
    cBegDEndDBegN,
# if INSTANCE
    decEnterDate,
# endif
    pXmlItems);
  const bool monthRowNeed = MonthRowNeed(
# if INSTANCE
    decEnterDate,
# endif
    currMonth, currYear, session);

  if(monthRowNeed)
  {
    if (of.pageLinesLeft () <
	  of.linesOfItem (false) + 1 + cSBC.getMaxRows () + 1)
    {
	ChangePage();
    }
  }
  else
  {
    if (of.pageLinesLeft () < cSBC.getMaxRows () + 1)
    {
	ChangePage();
    }
  }

  {
    if(monthRowNeed)
    {
	of.mediumFootherLine () ;
	of.writeItem (cSBC, false) ;
	of.mediumTopLine () ;
    }
    else
    {
	of.horzLine () ;
    }
    for (int i = 0 ; i < cSBC.getMaxRows () ; i ++)
    {
	of.writeItem (cSBC, true) ;
    }
  }
}

void TClosedSessionBookThread::Execute()
{
  if(fakeRun == false)
  {  // -- -1 --
    try
    {  // -- 0 --
	wtdl->SetHeader("Книга Закрити заседания");
	wtdl->PleaseWait();

	if(pQ == NULL)
	  makeQuery();
	if (of && pQ)
	{  // -- 1 --
	  LawPtr pLaw;  // 24.jan.2005
	  TLawsuit& law = pLaw.Ref();  // 24.jan.2005
# if INSTANCE  // AREAL and APPEAL
	  TDummyGroup enterDateGroup;
	  CDate decEnterDate ;

	  enterDateGroup.Add( new TDate ("F_ENTER_DATE",
						& decEnterDate, false)) ;
	  decEnterDate.year = decEnterDate.month =
				    decEnterDate.day = '\0' ;
# endif
	  SessPtr pSess ;
	  TSession & session = pSess.Ref () ;
	  int currMonth = 0;
	  long currYear = 0;  // 25.jan.2005
	  long int serialNo = cBegDEndDBegN.begN ;
	  const long int sessTotal = pQ->getRecsCount();

# if INSTANCE
#   if APPEAL
#   else    // AREAL
	  if (session.key.type == TYPE_FIRM_LAW)
	    currMonth = (char) 127 ;  // Because I have to show any
		  // decisions with empty enterDate
#   endif
# endif
	  wtdl->SetProgressRange(serialNo, serialNo + sessTotal);
	  WriteHeader();
	  {  // -- 2 --
	    int pageNo = 1;
	    char tmp[32];
	    SimpleStringsArray pageAttribs;

	    {
		ostrstream(tmp, sizeof(tmp)) << (pageNo) << ends;
		ENDING_ZERO(tmp);
		pageAttribs.Add(new SimpleStrings("no", tmp));
	    }

	    while (pQ -> Read () && wtdl->WantBreak() == false)
	    {  // -- 3 --
		session << (* pQ) ;
		if (! (DatedContainer_(session).FilledOK () && session.Get ()))
		  continue ;
		{
		  char sessDesc[128];

		  ostrstream(sessDesc, sizeof(sessDesc))
		     << DatedContainer_(session)
		     << " (" << (serialNo + 1 - cBegDEndDBegN.begN)
		     << " от " << sessTotal << ")" << ends;
		  sessDesc [sizeof(sessDesc) - 1] = '\0';
		  wtdl->SetText(sessDesc);
		}
# if INSTANCE // AREAL and APPEAL
		if (enterDateNeed (cBegDEndDBegN))
		  enterDateGroup << (* pQ) ;
#   if APPEAL // APPEAL
#   else      // AREAL
		if (session.key.type == TYPE_FIRM_LAW &&
		    cBegDEndDBegN.wantShortFormat &&
		    ( strchr (RESULT_FIRM_CLSBK_MUST_INCLUDE, session.result)  ||
			strchr (RESULT_FIRM_CL_S_BK_EXCLUDEDS, session.result) )  // 20060412
		    )
		  continue ;
#   endif
# endif

		law.key = session.key ;
		if (law.Get ())
		{  // -- 4 --
		  WriteOneSession(serialNo, session, law,
		    currMonth, currYear
# if INSTANCE  // AREAL and APPEAL
		    , decEnterDate
# endif
		    );
		}  // -- 4 --
		serialNo ++ ;
		wtdl->SetProgress(serialNo);
	    }  // -- 3 --
	    wtdl->SetProgress(serialNo + sessTotal);
	    wtdl->AlmostDone();
		of.footherAndTail();
	  }  // -- 2 --
	}  // -- 1 --
    }  // -- 0 --
    CATCH_ANYTHING
  }  // -- -1 --  // 26.may.2003
}

void TClosedSessionBookThread :: printClosedSessBook ()
 {
   {  // -- 0 --
    {
	char t[2];

	t[0] = cBegDEndDBegN.college;
	t[1] = '\0';
	register_usage(REGIME_CLOSED_SESS_BOOK, 1, t, "КЗЗ");
    }
    try
    {
	ExecWait();
    }
    CATCH_ANYTHING

    show_text_stream_use_settings(NULL, of, formatFN, LEFT_ALIGNMENT);
# if INSTANCE
    if (cBegDEndDBegN.college != TYPE_FIRM_LAW)
# endif
	writeSummary (formatFN) ;
   }  // -- 0 --
 }

void print_closed_book(TWindow *parent)
{
	collBegDEndDBegN cBegDEndDBegN;

	if (TPrintClosedSessBkDialog(parent, &cBegDEndDBegN).Execute() == IDOK)
	{
		TWaitThreadDialog wtdl;
		TClosedSessionBookThread(cBegDEndDBegN).printClosedSessBook();
	}
}

# undef WITH_STARS
