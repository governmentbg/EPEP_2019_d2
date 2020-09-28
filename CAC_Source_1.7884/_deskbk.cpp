#include "cac.h"

class deskBookColumns : public columnsArr, protected descBookEnum
 {
# if INSTANCE
    char companyType ;
# endif
    void makeColumns (TLawsuit & rLawsuit,
			    TStringArray * pItems  // 10.feb.2005
			   ) ;
  public :
    deskBookColumns(const char* formatFN, TWindow* parent, TLawsuit& rLawsuit,
			   TStringArray* pItems  // 10.feb.2005
			   ) ;
# if INSTANCE
    char getDamnType () const ;
# endif
 } ;  // class deskBookColumns

void deskBookColumns :: makeColumns (TLawsuit & rLawsuit,
                      TStringArray * pItems   // 10.feb.2005 for XML
                     )
 {
//  METHOD_NAME("makeColumns");
  const size_t buffSz = MAKE_COLUMNS_BUFF_SIZE ;
  const size_t buffL = buffSz - 1 ;
  char * buff = new char [buffSz] ;
  char * b1 = new char [buffSz] ;
  column * pColumn ;  // To make looking of the program text more easy
  DecPtr pDec;  // 18.feb.2005 - begin
  SessPtr pSess;
  const bool decFound = lawsuit_finalising_decision(rLawsuit, pDec.Ref(), NULL,
    "Описна книга", true, pSess);

  timeBookHeap tBH;  // 18.feb.2005 - end

  if(decFound == false)
  {
    clearGroupData(pDec.Ref());
    clearGroupData(pSess.Ref());
  }

  if (! columns)
    return ;
  if (! (buff && b1))
   {
    if (buff)
      delete [] buff ;
    if (b1)
      delete [] b1 ;
    return ;
   }
  buff [buffSz - 1] = '\0' ;
  b1 [buffSz - 1] = '\0' ;

  if(decFound)  // 18.feb.2005
  {
    markDecGlJResInTBHeap(pDec.Ref().result, pSess.Ref().result, tBH);
  }

  if( ((pColumn = columns [iC_LawNo_Type_Compo_DateCre]) != NULL) ||
      ((pColumn = columns [iC_LawNo_Type_Compo]) != NULL)  )
  {// -- iC_LawNo_Type_Composition --
    ColumnXmlEncloser encloser(pColumn, iC_LawNo_Type_Compo_DateCre + bOC, pItems);

    ostrstream oss (buff, buffL) ;
    oss << rLawsuit.key.no << " " ;
#if COURT_TYPE == COURT_REGIONAL
    if (rLawsuit.sourceKind == KIND_REQUEST_126G)
		oss << "126 Ж " ;
#endif  // REGIONAL

    if ((rLawsuit.quick & QUICK_CONVERTED) == 0)
    {
       if (quick_pure_quick(rLawsuit.quick)) oss << " Б" ;
       else if (quick_pure_immediate(rLawsuit.quick)) oss << " Н" ;
       else {} // no sign
    }
    else // with conversion
    {  if (     quick_formerly_standard_to_quick(rLawsuit.quick)) oss << " ОБ" ;
       else if (quick_formerly_standard_to_immediate(rLawsuit.quick)) oss << " ОН" ;
	 else if (quick_formerly_quick_to_standard(rLawsuit.quick)) oss << " БО" ;
       else if (quick_formerly_immediate_to_standard(rLawsuit.quick)) oss << " НО" ;
       else {pColumn -> addText ("+") ;}  // don't know
    }
    oss << ends;
    pColumn -> addText( buff );

    const size_t sSz = sizeof (Kind -> alias) + 64 ;
    char tmp [sSz] ;
    tmp [0] = '\0' ;
    kind_to_alias (rLawsuit.kind, tmp, sSz) ;
    pColumn -> addText( tmp );

    tmp [0] = '\0' ;
    if (use_area_compositions ()) // 20070806
    {
        compositionToString (rLawsuit.composition, tmp, sSz) ;
        ostrstream (tmp + strlen (tmp), buffL - strlen (tmp))
               << " с-в." << ends ;
    }
    pColumn -> addText ( tmp );

    if ((pColumn = columns [iC_LawNo_Type_Compo_DateCre]) != NULL)
    {
      CDateToString (rLawsuit.date, buff, buffL) ;  // 20080312
      pColumn -> addText (buff) ;
    }
  }// -- iC_LawNo_Type_Composition --

  if ((pColumn = columns [iC_LawNo]) != NULL)
   {  // -- iC_LawNo --
    ColumnXmlEncloser encloser(pColumn, iC_LawNo + bOC, pItems);

    ostrstream (buff, buffL) << rLawsuit.key.no << ends ;
    pColumn -> addText (buff) ;
#if COURT_TYPE == COURT_REGIONAL
    if (rLawsuit.sourceKind == KIND_REQUEST_126G)
	  pColumn -> addText ("126 Ж") ;
#endif  // REGIONAL
   }  // -- iC_LawNo --
  if ((pColumn = columns [iC_CreateDate]) != NULL)
   {  // -- iC_CreateDate --
    ColumnXmlEncloser encloser(pColumn, iC_CreateDate + bOC, pItems);

    CDateToString (rLawsuit.date, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_CreateDate --
  if ((pColumn = columns [iC_Source_Areal]) != NULL)
   {  // -- iC_Source_Areal --
    ColumnXmlEncloser encloser(pColumn, iC_Source_Areal + bOC, pItems);

# if INSTANCE
    // 20070907 -->  Ако делото е образувано по Входящ документ, да се извежда <Точен вид на входящ докумнет, номер и Дата на постъпване>
    // 20070116 -->  DECISIV -- Изпратено по посъдност; възобновено; внесено след доразследване; Върнато за ново разглеждане
    addInDocAndRelatedLawsToColumn(pColumn, rLawsuit, buff, buffSz) ;
    // 20070116 --> само Паркетно дело; Следствено дело; Дознание; Бързо полицейско производство
    add_preliminaryActions_to_column (pColumn, buff, buffL, rLawsuit) ;  // 23.oct.2000
# endif
   }  // -- iC_Source_Areal --
  if ((pColumn = columns [iC_Subject_Areal]) != NULL)
   {  // -- iC_Subject_Areal --
    ColumnXmlEncloser encloser(pColumn, iC_Subject_Areal + bOC, pItems);

    //if (DEFAULT_UCLP == PLOVDIV_UCLP)  // 05.may.2004
    if (COURT_UCLP == PLOVDIV_UCLP)  // 05.may.2004
      if (rLawsuit.key.type == TYPE_PUNISHMENT_LAW)
       {
	  kindToString (rLawsuit.kind, buff, buffL) ;
        pColumn -> addText (buff) ;
       }
# if INSTANCE
#   if APPEAL
#   else       // AREAL
      else
        if (rLawsuit.key.type == TYPE_ADMIN_LAW)
        {
          if (use_area_compositions ())  // 20070806
          {
            compositionToString (rLawsuit.composition, buff, buffL) ;
            ostrstream (buff + strlen (buff), buffL - strlen (buff))
              << " състав" << ends ;
            pColumn -> addText (buff) ;
          }
        }
#   endif
# endif

    buff [0] = '\0' ;
    if (rLawsuit.subject)
     {
      subjectToString (rLawsuit.key.type, rLawsuit.subject, b1, buffL) ;
      if (b1 [0] != (char) 160)
	  ostrstream (buff, buffL) << rLawsuit.Code() << " " << b1
                                 << ends ;
     }
    pColumn -> addText (buff) ;
   }  // -- iC_Subject_Areal --
   {  // --- SIDES ---
    TLawsuit l ;

    l << rLawsuit ;

    // 17.feb.2005
    // Because we add rows into a number of columns
    // several times using a loop,
    // it is necesary to take measures against overlapping of
    // contents of columns.
    TStringArray claimAndSettlem_Addr_ArealItems;
    TStringArray defendAndSettlem_Addr_ArealItems;
    TStringArray claimantAndSettlement_ArealItems;
    TStringArray defendantAndSettlem_ArealItems;
    TStringArray claimantItems;
    TStringArray defendantItems;

    TStringArray h_ClaimAndSettlemAddr_ArealItems;
    TStringArray h_DefendAndSettlmAddr_ArealItems;
    TStringArray h_ClaimantAndSettlem_ArealItems;
    TStringArray h_DefendantAndSettlem_ArealItems;
    TStringArray h_ClaimantItems;
    TStringArray h_DefendantItems;

     {
      // 17.feb.2005
      // Enclosers must be in a block in order to add their closing tags
      // to the arrays prior to moving of contents (after the block)
      ColumnXmlEncloser claimAndSettlem_Addr_ArealEncl(
        columns[iC_ClaimAndSettlementAddr_Areal],
        iC_ClaimAndSettlementAddr_Areal + bOC,
        &claimAndSettlem_Addr_ArealItems);
      ColumnXmlEncloser defendAndSettlem_Addr_ArealEncl(
        columns[iC_DefendAndSettlementAddr_Areal],
        iC_DefendAndSettlementAddr_Areal + bOC,
        &defendAndSettlem_Addr_ArealItems);
      ColumnXmlEncloser claimantAndSettlement_ArealEncl(
        columns[iC_ClaimantAndSettlement_Areal],
        iC_ClaimantAndSettlement_Areal + bOC,
        &claimantAndSettlement_ArealItems);
      ColumnXmlEncloser defendantAndSettlem_ArealEncl(
        columns[iC_DefendantAndSettlement_Areal],
        iC_DefendantAndSettlement_Areal + bOC,
        &defendantAndSettlem_ArealItems);
      ColumnXmlEncloser claimantEncl(columns[iC_Claimant], iC_Claimant + bOC,
        &claimantItems);
      ColumnXmlEncloser defendantEncl(columns[iC_Defendant], iC_Defendant + bOC,
	  &defendantItems);

      ColumnXmlEncloser h_ClaimAndSettlemAddr_ArealEncl(
        columns[iC_H_ClaimAndSettlemAddr_Areal],
        iC_H_ClaimAndSettlemAddr_Areal + bOC,
        &h_ClaimAndSettlemAddr_ArealItems);
      ColumnXmlEncloser h_DefendAndSettlmAddr_ArealEncl(
        columns[iC_H_DefendAndSettlemAddr_Areal],
        iC_H_DefendAndSettlemAddr_Areal + bOC,
        &h_DefendAndSettlmAddr_ArealItems);
      ColumnXmlEncloser h_ClaimantAndSettlem_ArealEncl(
        columns[iC_H_ClaimantAndSettlem_Areal],
        iC_H_ClaimantAndSettlem_Areal + bOC,
        &h_ClaimantAndSettlem_ArealItems);
      ColumnXmlEncloser h_DefendantAndSettlem_ArealEncl(
        columns[iC_H_DefendantAndSettlem_Areal],
        iC_H_DefendantAndSettlem_Areal + bOC,
        &h_DefendantAndSettlem_ArealItems);
      ColumnXmlEncloser h_ClaimantEncl(columns[iC_H_Claimant],
        iC_H_Claimant + bOC, &h_ClaimantItems);
      ColumnXmlEncloser h_DefendantEncl(columns[iC_H_Defendant],
        iC_H_Defendant + bOC, &h_DefendantItems);

      lawRCDKeyToSidePair sides (l, true  // 13.mar.2001
                                ) ;
      unsigned i ;
      const unsigned pairs = sides.getPairsCount (),
                   claims = sides.getClaimantsCount (),
			 defends = sides.getDefendantsCount () ;
	TSideWind claimSide, defendSide ;
	const bool wantClStreet =
	  (columns [iC_ClaimAndSettlementAddr_Areal] ||
         columns [iC_H_ClaimAndSettlemAddr_Areal]) ;
      const bool wantClAddress =
        (wantClStreet ||
         columns [iC_ClaimantAndSettlement_Areal] ||
         columns [iC_H_ClaimantAndSettlem_Areal]) ;
      const bool wantDefStreet =
        (columns [iC_DefendAndSettlementAddr_Areal] ||
         columns [iC_H_DefendAndSettlemAddr_Areal]) ;
      const bool wantDefAddress =
        (wantDefStreet ||
         columns [iC_DefendantAndSettlement_Areal] ||
         columns [iC_H_DefendantAndSettlem_Areal]) ;

      if (sides.statusIsOK ())
// --------------------------------------------------------------------
        for (i = 0 ; i < pairs ; i ++)
         {
          TClaimant claimant ;
          TDefendant defendant ;
          const bool readOK = sides.nextPair (& claimant, & defendant,
							    & claimSide, & defendSide,
                                              wantClAddress,
                                              wantDefAddress,
                                              wantClStreet,
                                              wantDefStreet) ;
          const char * const quest = "?",
                     * const holded = "* ",
                     * const notHolded = "" ;

          if ((pColumn = columns [iC_ClaimAndSettlementAddr_Areal])
              != NULL && i < claims)
           {  // -- iC_ClaimAndSettlementAddr_Areal --
            if (readOK)
             {
              ostrstream (buff, buffL)
                << claimant.cName << ", " << claimant.cSettlement << " "
                << claimant.cAddress << ends ;
              pColumn -> addText (buff) ;
             }
            else
              pColumn -> addText (quest) ;
           }  // -- iC_ClaimAndSettlementAddr_Areal --
          if ((pColumn = columns [iC_DefendAndSettlementAddr_Areal])
              != NULL
              && i < defends)
           {  // -- iC_DefendAndSettlementAddr_Areal --
            if (readOK)
             {
		  ostrstream (buff, buffL)
                << defendant.dName << ", " << defendant.dSettlement
                << " " << defendant.dAddress << ends ;
              pColumn -> addText (buff) ;
             }
            else
              pColumn -> addText (quest) ;
           }  // -- iC_DefendAndSettlementAddr_Areal --

          if ((pColumn = columns [iC_ClaimantAndSettlement_Areal])
              != NULL && i < claims)
           {  // -- iC_ClaimantAndSettlement_Areal --
            if (readOK)
             {
              ostrstream (buff, buffL)
                << claimant.cName << ", " << claimant.cSettlement
                << ends ;
              pColumn -> addText (buff) ;
             }
            else
              pColumn -> addText (quest) ;
           }  // -- iC_ClaimantAndSettlement_Areal --
          if ((pColumn = columns [iC_DefendantAndSettlement_Areal])
              != NULL
              && i < defends)
           {  // -- iC_DefendantAndSettlement_Areal --
            if (readOK)
             {
		  ostrstream (buff, buffL)
                << defendant.dName << ", " << defendant.dSettlement
                << ends ;
              pColumn -> addText (buff) ;
             }
            else
              pColumn -> addText (quest) ;
           }  // -- iC_DefendantAndSettlement_Areal --

          if ((pColumn = columns [iC_Claimant]) != NULL && i < claims)
           {  // -- iC_Claimant --
            pColumn -> addText (readOK ? claimant.cName : quest) ;
           }  // -- iC_Claimant --
          if ((pColumn = columns [iC_Defendant]) != NULL && i < defends)
           {  // -- iC_Defendant --
            pColumn -> addText (readOK ? defendant.dName : quest) ;
           }  // -- iC_Defendant --
// ---- I'm sorry for this repetition of 99% of the text... -----------
          if ((pColumn = columns [iC_H_ClaimAndSettlemAddr_Areal])
              != NULL && i < claims)
           {  // -- iC_H_ClaimAndSettlemAddr_Areal --
            if (readOK)
             {
//              info ("claimSide.state = '%c'", claimSide.state) ;
              ostrstream (buff, buffL)
                << (claimSide.state == STATE_HOLD ? holded : notHolded)
                << claimant.cName << ", " << claimant.cSettlement << " "
                << claimant.cAddress << ends ;
		  pColumn -> addText (buff) ;
             }
            else
              pColumn -> addText (quest) ;
           }  // -- iC_H_ClaimAndSettlemAddr_Areal --
          if ((pColumn = columns [iC_H_DefendAndSettlemAddr_Areal])
              != NULL
              && i < defends)
           {  // -- iC_H_DefendAndSettlemAddr_Areal --
            if (readOK)
             {
//              info ("defendSide.state = '%c'", defendSide.state) ;
              ostrstream (buff, buffL)
                << (defendSide.state == STATE_HOLD ? holded : notHolded)
                << defendant.dName << ", " << defendant.dSettlement
                << " " << defendant.dAddress << ends ;
              pColumn -> addText (buff) ;
             }
            else
              pColumn -> addText (quest) ;
           }  // -- iC_H_DefendAndSettlemAddr_Areal --

          if ((pColumn = columns [iC_H_ClaimantAndSettlem_Areal])
              != NULL && i < claims)
           {  // -- iC_H_ClaimantAndSettlem_Areal --
            if (readOK)
             {
//              info ("claimSide.state = '%c'", claimSide.state) ;
		  ostrstream (buff, buffL)
                << (claimSide.state == STATE_HOLD ? holded : notHolded)
                << claimant.cName << ", " << claimant.cSettlement
                << ends ;
              pColumn -> addText (buff) ;
             }
            else
              pColumn -> addText (quest) ;
           }  // -- iC_H_ClaimantAndSettlem_Areal --
          if ((pColumn = columns [iC_H_DefendantAndSettlem_Areal])
              != NULL
              && i < defends)
           {  // -- iC_H_DefendantAndSettlem_Areal --
            if (readOK)
             {
//              info ("defendSide.state = '%c'", defendSide.state) ;
              ostrstream (buff, buffL)
                << (defendSide.state == STATE_HOLD ? holded : notHolded)
                << defendant.dName << ", " << defendant.dSettlement
                << ends ;
              pColumn -> addText (buff) ;
             }
            else
              pColumn -> addText (quest) ;
           }  // -- iC_H_DefendantAndSettlem_Areal --

          if ((pColumn = columns [iC_H_Claimant]) != NULL && i < claims)
           {  // -- iC_H_Claimant --
		if (readOK)
             {
              ostrstream (buff, buffL)
                << (claimSide.state == STATE_HOLD ? holded : notHolded)
                << claimant.cName
                << ends ;
              pColumn -> addText (buff) ;
             }
            else
              pColumn -> addText (quest) ;
           }  // -- iC_H_Claimant --
          if ((pColumn = columns [iC_H_Defendant]) != NULL &&
              i < defends)
           {  // -- iC_H_Defendant --
            if (readOK)
             {
              ostrstream (buff, buffL)
                << (defendSide.state == STATE_HOLD ? holded : notHolded)
                << defendant.dName
                << ends ;
              pColumn -> addText (buff) ;
             }
            else
              pColumn -> addText (quest) ;
           }  // -- iC_H_Defendant --
         }  // for (i = 0 ; i < pairs ; i ++)
// --------------------------------------------------------------------
     } // if (sides.statusIsOK ())

     // 17.feb.2005
     if(pItems)
     {
      move_string_array_contents(pItems, &claimAndSettlem_Addr_ArealItems);
      move_string_array_contents(pItems, &defendAndSettlem_Addr_ArealItems);
      move_string_array_contents(pItems, &claimantAndSettlement_ArealItems);
      move_string_array_contents(pItems, &defendantAndSettlem_ArealItems);
      move_string_array_contents(pItems, &claimantItems);
      move_string_array_contents(pItems, &defendantItems);

      move_string_array_contents(pItems, &h_ClaimAndSettlemAddr_ArealItems);
      move_string_array_contents(pItems, &h_DefendAndSettlmAddr_ArealItems);
      move_string_array_contents(pItems, &h_ClaimantAndSettlem_ArealItems);
      move_string_array_contents(pItems, &h_DefendantAndSettlem_ArealItems);
      move_string_array_contents(pItems, &h_ClaimantItems);
      move_string_array_contents(pItems, &h_DefendantItems);
     }
   }  // --- SIDES ---
  if ((pColumn = columns [iC_LawsuitKind]) != NULL)
   {  // -- iC_LawsuitKind --
    ColumnXmlEncloser encloser(pColumn, iC_LawsuitKind + bOC, pItems);

    kindToString (rLawsuit.kind, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_LawsuitKind --
  if ((pColumn = columns [iC_Composition]) != NULL)
   {  // -- iC_Composition --
    ColumnXmlEncloser encloser(pColumn, iC_Composition + bOC, pItems);

    compositionToString (rLawsuit.composition, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_Composition --
  if ((pColumn = columns [iC_Subject]) != NULL)
   {  // -- iC_Subject --
    ColumnXmlEncloser encloser(pColumn, iC_Subject + bOC, pItems);

    subjectAndExtraTextToString (rLawsuit.kind, rLawsuit.subject,
                                 buff, buffL, rLawsuit.text) ;
    pColumn -> addText (buff) ;
//20070215 # if INSTANCE
//# else // REGIONAL
    add_preliminaryActions_to_column (pColumn, buff, buffL,
                                      rLawsuit) ;  // 25.jul.2000
//# endif
   }  // -- iC_Subject --
  if ((pColumn = columns [iC_Source]) != NULL)
   {  // -- iC_Source --
    ColumnXmlEncloser encloser(pColumn, iC_Source + bOC, pItems);

    sourceNoAndSenderToString(rLawsuit, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_Source --
  if ((pColumn = columns [iC_DecisionDate]) != NULL)
   {  // -- iC_DecisionDate --
    ColumnXmlEncloser encloser(pColumn, iC_DecisionDate + bOC, pItems);

    CDateToString (rLawsuit.finished, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_DecisionDate --

  // 20060309 ->
  if ((pColumn = columns [iC_AnnounceDate]) != NULL)
  {  // -- iC_AnnounceDate --
    buff [0] = '\0' ;
    ColumnXmlEncloser encloser(pColumn, iC_AnnounceDate + bOC, pItems);
    // 2010:026 LPR: finished date -> first public date
    {
    #if 20170137
	TSession session;
	msql m(session.Name, "F_DATE");

	m.AddLong("F_NO", rLawsuit.key.no);
	m.AddLong("F_YEAR", rLawsuit.key.year);
	m.AddChar("F_TYPE", rLawsuit.key.type);
	m.AddChar("F_RESULT", RESULT_PUBLIC_DECISION);
	m.AddOrder("F_DATE");

	TQuery q(str(m));
    #else  // 20170137
	char s[0x100];
	TSession session;

	sprintf(s,
		"SELECT F_DATE FROM %s "
		"WHERE F_NO = %ld "
		"AND F_YEAR = %ld "
		"AND F_TYPE = '%c' "
		"AND F_RESULT = '%c' "
		"ORDER BY F_DATE;",
		session.Name, rLawsuit.key.no, rLawsuit.key.year,
		rLawsuit.key.type, RESULT_PUBLIC_DECISION
	);

	TQuery q(s);
    #endif  // 20170137
	if (q.Read())
	{
		session << q;
		CDateToString(session.date, buff, buffL);
	}
    }
    pColumn -> addText (buff) ;
  }  // -- iC_AnnounceDate --
  // 20060309 <-

  if ((pColumn = columns [iCF_FastProcedure]) != NULL)
   {  // -- iCF_FastProcedure --
    ColumnXmlEncloser encloser(pColumn, iCF_FastProcedure + bOC, pItems);
// 20060324 ->
    if ((rLawsuit.quick & QUICK_CONVERTED) == 0)
    {
	 if (quick_pure_quick(rLawsuit.quick)) pColumn -> addText ("Б") ;
	 else if (quick_pure_immediate(rLawsuit.quick)) pColumn -> addText ("Н") ;
	 else {} // no sign
    }
    else // with conversion
    {  if (     quick_formerly_standard_to_quick(rLawsuit.quick)) pColumn -> addText ("ОБ") ;
       else if (quick_formerly_standard_to_immediate(rLawsuit.quick)) pColumn -> addText ("ОН") ;
       else if (quick_formerly_quick_to_standard(rLawsuit.quick)) pColumn -> addText ("БО") ;
	 else if (quick_formerly_immediate_to_standard(rLawsuit.quick)) pColumn -> addText ("НО") ;
       else {pColumn -> addText ("+") ;}  // don't know
    }
// 20060324 <-
   }  // -- iCF_FastProcedure --

// 20080312
  if ((pColumn = columns [iC_LawNo_Info_DateCre]) != NULL)
  {  // -- iC_LawNo_Info_DateCre --
    ColumnXmlEncloser encloser(pColumn, iC_LawNo_Info_DateCre + bOC, pItems);

    ostrstream (buff, buffL) << rLawsuit.key.no << ends ;
    pColumn -> addText (buff) ;
#if COURT_TYPE == COURT_REGIONAL
    if (rLawsuit.sourceKind == KIND_REQUEST_126G)
	  pColumn -> addText ("126 Ж") ;
#endif  // REGIONAL

    if ((rLawsuit.quick & QUICK_CONVERTED) == 0)
    {
       if (quick_pure_quick(rLawsuit.quick)) pColumn -> addText ("Б") ;
       else if (quick_pure_immediate(rLawsuit.quick)) pColumn -> addText ("Н") ;
       else {} // no sign
    }
    else // with conversion
    {  if (     quick_formerly_standard_to_quick(rLawsuit.quick)) pColumn -> addText ("ОБ") ;
	 else if (quick_formerly_standard_to_immediate(rLawsuit.quick)) pColumn -> addText ("ОН") ;
       else if (quick_formerly_quick_to_standard(rLawsuit.quick)) pColumn -> addText ("БО") ;
       else if (quick_formerly_immediate_to_standard(rLawsuit.quick)) pColumn -> addText ("НО") ;
       else {pColumn -> addText ("+") ;}  // don't know
    }

     CDateToString (rLawsuit.date, buff, buffL) ;
     pColumn -> addText (buff) ;
  }  // -- iC_LawNo_Info_DateCre --



  if ((pColumn = columns [iC_Remark]) != NULL)
   {  // -- iC_Remark --
    ColumnXmlEncloser encloser(pColumn, iC_Remark + bOC, pItems);

    // nothing
   }  // -- iC_Remark --
  if ((pColumn = columns [iC_SubjectNameOnly]) != NULL)
	 {  // -- iC_SubjectNameOnly --
    ColumnXmlEncloser encloser(pColumn, iC_SubjectNameOnly + bOC, pItems);
    //20050329 ->
    char * tmpStr = new char [buffL] ;

    if (tmpStr)
    {
      ostrstream oss (buff, buffL) ;

      subjectToString (rLawsuit.key.type, rLawsuit.subject, tmpStr, buffL) ;
      oss  << tmpStr
           << "\n" << rLawsuit.text
           << ends ;
      delete [] tmpStr ;
    }
    //20050329 <-
    pColumn -> addText (buff) ;
//Petyo worte this for new books in Areal: subjectToString (rLawsuit.key.type, rLawsuit.subject,
//                                                          buff, buffL) ;
//this is used for books in Regional     : subjectAndExtraTextToString (rLawsuit.kind, rLawsuit.subject,
//                                                                      buff, buffL, rLawsuit.text) ;
   }  // -- iC_SubjectNameOnly --
# if INSTANCE
  if ((pColumn = columns [iC_LawNoAndDateCreated]) != NULL)
   {  // -- iC_LawNoAndDateCreated --
    ColumnXmlEncloser encloser(pColumn, iC_LawNoAndDateCreated + bOC, pItems);

    addLongToColumn (pColumn, rLawsuit.key.no, true) ;
    CDateToString (rLawsuit.date, buff, buffL) ;
    pColumn -> addText (buff) ;
   }  // -- iC_LawNoAndDateCreated --

  if (columns [iC_FirmKind] || columns [iC_FirmAndBoss] ||
      columns [iC_Seat] || columns [iC_Register] ||
      columns [iC_Volume] || columns [iC_Page] || columns [iC_Lot] ||
      rLawsuit.key.type == TYPE_FIRM_LAW)
   {
    TDamn firm ;
    const bool firmFound = lawsuitToDamn (& rLawsuit, & firm) ;

    if (firmFound)
      companyType = firm.companyType ;
    if ((pColumn = columns [iC_FirmKind]) != NULL && firmFound)
     {  // -- iC_FirmKind --
      ColumnXmlEncloser encloser(pColumn, iC_FirmKind + bOC, pItems);
      TCompanyType firmKind ;

      firmKind.companyType = firm.companyType ;
      if (firmKind.Get ())
        pColumn -> addText (firmKind.name) ;
     }  // -- iC_FirmKind --
    if ((pColumn = columns [iC_FirmAndBoss]) != NULL)
     {  // -- iC_FirmAndBoss --
      ColumnXmlEncloser encloser(pColumn, iC_FirmAndBoss + bOC, pItems);

      if (firmFound)
        pColumn -> addText (firm.name) ;
      else
        pColumn -> addEmptyRow () ;


	TSideWind boss ;

	if (lawsuitToDamnBoss (& rLawsuit, & boss, NULL))
       {
        if (strchr (UCN_CITIZENS, boss.ucnType))
         {
          TCitizen citizen ;

          citizen << boss ;
          if (citizen.Get ())
           {
            ostrstream (buff, buffL)
              << citizen.name << " " << citizen.reName << " "
              << citizen.family << " " << citizen.reFamily << ends ;
            pColumn -> addText (buff) ;
           }
          else
            pColumn -> addEmptyRow () ;
         }
        else
         {
          TFirm firm ;

          firm << boss ;
          if (firm.Get ())
           {
            ostrstream (buff, buffL) << firm.name << ends ;
            pColumn -> addText (buff) ;
           }
          else
            pColumn -> addEmptyRow () ;
         }
       }
      else
        pColumn -> addEmptyRow () ;

      ostrstream oss (buff, buffL) ;
      //oss << "Върнато "
      //oss << "Подсъдност " ;
      //oss << "Подсъдност " ;

      sourceNoAndSenderToString (rLawsuit, b1, buffL) ; // 20070115

      if (b1 [0])
        oss << b1 ;
      oss << ends ;
      pColumn -> addText (buff) ;
     }  // -- iC_FirmAndBoss --
    if ((pColumn = columns [iC_Seat]) != NULL && firmFound)
     {  // -- iC_Seat --
      ColumnXmlEncloser encloser(pColumn, iC_Seat + bOC, pItems);
      TAddress firmAddress ;

	// 2009:167 LPR: F_NO crashes on <<
	//firmAddress << firm ;
	firmAddress.ucnType = firm.ucnType;
	strcpy(firmAddress.ucn, firm.ucn);
	if (firmAddress.Get ())
	 {
        TUCLP firmSettlement ;
        const bool fSettlFound = uclpCodeToTUCLP (firmAddress.uclp,
                                                  firmSettlement) ;

        ostrstream (buff, buffL)
          << firmSettlement.uclp << " "
          << (fSettlFound ? firmSettlement.name : "") << ends ;
        pColumn -> addText (buff) ;
       }
     }  // -- iC_Seat --
    if ((columns [iC_Register] || columns [iC_Volume] ||
	   columns [iC_Page] || columns [iC_Lot]))
     {  // An old varianth in VersionArchives\2365\_appname.j --> _deskbk.cpp
      TDecision dec ;
      const bool decExtracted = extract_first_decision (rLawsuit, dec) ;

      //if (extract_first_decision (rLawsuit, dec))
      // {
      if ((pColumn = columns [iC_Register]) != NULL)
       {  // -- iC_Register --
        ColumnXmlEncloser encloser(pColumn, iC_Register + bOC, pItems);

        if(decExtracted)
        {
          addLongToColumn (pColumn, dec.registry) ;
        }
       }  // -- iC_Register --
      if ((pColumn = columns [iC_Volume]) != NULL)
       {  // -- iC_Volume --
	  ColumnXmlEncloser encloser(pColumn, iC_Volume + bOC, pItems);

        if(decExtracted)
        {
          addLongToColumn (pColumn, dec.volume) ;
        }
       }  // -- iC_Volume --
      if ((pColumn = columns [iC_Page]) != NULL)
       {  // -- iC_Page --
        ColumnXmlEncloser encloser(pColumn, iC_Page + bOC, pItems);

        if(decExtracted)
        {
          addLongToColumn (pColumn, dec.page) ;
        }
       }  // -- iC_Page --
      if ((pColumn = columns [iC_Lot]) != NULL)
       {  // -- iC_Lot --
        ColumnXmlEncloser encloser(pColumn, iC_Lot + bOC, pItems);

        if(decExtracted)
        {
          addLongToColumn (pColumn, dec.partition) ;
        }
       }  // -- iC_Lot --
       //}
     }
   }
#   if APPEAL
  if ((pColumn = columns [iC_ArealCourt]) != NULL)
   {  // -- iC_ArealCourt --
    ColumnXmlEncloser encloser(pColumn, iC_ArealCourt + bOC, pItems);

    if (rLawsuit.sourceKind != KIND_CANCEL_ENACTMENT)  // 23.may.2000
      if (lawsuitToSenderName (rLawsuit, buff, buffL, true)) // 15033
        pColumn -> addText (buff) ;
   }  // -- iC_ArealCourt --
#   endif  // of APPEAL
# endif  // of APPEAL
  if ((pColumn = columns [iC_DateReturned]) != NULL)
   {  // -- iC_DateReturned --
    ColumnXmlEncloser encloser(pColumn, iC_DateReturned + bOC, pItems);

    if (rLawsuit.returnedDate.Empty () == false)
     {
	ostrstream (buff, buffL) << rLawsuit.returnedDate << ends ;
	pColumn -> addText (buff) ;
     }
   }  // -- iC_DateReturned --
  if ((pColumn = columns [iC_OutregNo]) != NULL)
   {  // -- iC_OutregNo --
   // 2009:154 LPR: removed
   }  // -- iC_OutregNo --
  // 10.feb.2005 - begin
  if((pColumn = columns[iC_Age_UpTo1Month]) != NULL)
  {  // -- iC_Age_UpTo1Month --
    ColumnXmlEncloser encloser(pColumn, iC_Age_UpTo1Month + bOC, pItems);

    if(strchr(AGE_UP_TO_ONES, rLawsuit.age))
    {
      pColumn->addText(STAR_STRING);
    }
  }
  if((pColumn = columns[iC_Age_UpTo3Months]) != NULL)
  {  // -- iC_Age_UpTo3Months --
    ColumnXmlEncloser encloser(pColumn, iC_Age_UpTo3Months + bOC, pItems);

    if(strchr(AGE_UP_TO_THREES, rLawsuit.age) != NULL &&
      strchr(AGE_UP_TO_ONES, rLawsuit.age) == NULL)
    {
      pColumn->addText(STAR_STRING);
    }
  }
  if((pColumn = columns[iC_Age_UpTo6Months]) != NULL)
  {  // -- iC_Age_UpTo6Months --
    ColumnXmlEncloser encloser(pColumn, iC_Age_UpTo6Months + bOC, pItems);

    if(strchr(AGE_UP_TO_SIXES, rLawsuit.age) != NULL &&
      strchr(AGE_UP_TO_THREES, rLawsuit.age) == NULL)
    {
      pColumn->addText(STAR_STRING);
    }
  }
  if((pColumn = columns[iC_Age_UpTo1Year]) != NULL)
  {  // -- iC_Age_UpTo1Year --
    ColumnXmlEncloser encloser(pColumn, iC_Age_UpTo1Year + bOC, pItems);

    if(strchr(AGE_UP_TO_TWELVES, rLawsuit.age) != NULL &&
      strchr(AGE_UP_TO_SIXES, rLawsuit.age) == NULL)
    {
      pColumn->addText(STAR_STRING);
    }
  }
  if((pColumn = columns[iC_Age_Over1Year]) != NULL)
  {  // -- iC_Age_Over1Year --
    ColumnXmlEncloser encloser(pColumn, iC_Age_Over1Year + bOC, pItems);

    if(strchr(AGE_MORE_THAN_TWELVES, rLawsuit.age))
    {
      pColumn->addText(STAR_STRING);
    }
  }
  if((pColumn = columns[iC_Sides]) != NULL)
  {  // -- iC_Sides --
    ColumnXmlEncloser encloser(pColumn, iC_Sides + bOC, pItems);

    lawRCDKeyToSidePair pairs(rLawsuit, true);

    if (pairs.getPairsCount ())
    {
      unsigned i;
      unsigned claims = pairs.getClaimantsCount();
      unsigned defs = pairs.getDefendantsCount();

      for(i = 0; i < claims; i ++)
      {
        TSideWind clS;
        TClaimant claimant;

        if(pairs.nextPair (&claimant, NULL, &clS))
        {
          if(!defs)
          {
            involvementToString(clS.involvement, buff, buffL);
            pColumn->addText(buff);
          }
          pColumn->addText(claimant.cName);
        }
      }  // for(i = 0; i < claims; i ++)
      if(claims && defs)
      {
        pColumn->addText("срещу");
      }
      pairs.readIndexesToZero();
      for(i = 0; i < defs; i ++)
      {
        TSideWind defS;
        TDefendant defendant;

        if(pairs.nextPair(NULL, & defendant, NULL, & defS))
        {
          if(!claims)
          {
            involvementToString(defS.involvement, buff, buffL);
            pColumn->addText(buff);
          }
          //20050422 -->
          //pColumn->addText(defendant.dName);
          if(defS.state == STATE_HOLD)
          {
              ostrstream (buff, buffL)
                << "* "
                << defendant.dName
                << ends ;
              pColumn -> addText (buff) ;
          }
          else
          pColumn->addText(defendant.dName);
          //20050422 <--
        }
      }  // for (i = 0 ; i < defs ; i ++)
    }  // if (pairs.getPairsCount ())
  }  // -- iC_Sides --
  if((pColumn = columns[iC_InByComplaint]) != NULL)
  {  // -- iC_Age_Over1Year --
    ColumnXmlEncloser encloser(pColumn, iC_InByComplaint + bOC, pItems);

#if INSTANCE // AREAL, APPEAL
    if ( (strchr(KIND_EXACT_COMPLAINTS, rLawsuit.sourceKind))||
         (strchr(KIND_PRIVATE_COMPLAINTS, rLawsuit.sourceKind))  )
    {
      pColumn->addText(STAR_STRING);
    }
#else
#endif
  }  // -- iC_InByComplaint --
  if((pColumn = columns[iC_InByObjection]) != NULL)
  {  // -- iC_InByObjection --
    ColumnXmlEncloser encloser(pColumn, iC_InByObjection + bOC, pItems);

#if INSTANCE // AREAL, APPEAL
    if( (strchr(KIND_EXACT_OBJECTIONS, rLawsuit.sourceKind))||
        (strchr(KIND_PRIVATE_OBJECTIONS, rLawsuit.sourceKind))  )
    {
      pColumn->addText(STAR_STRING);
    }
#else
#endif
  }  // -- iC_InByObjection --
  if((pColumn = columns[iC_SubjectCode]) != NULL)
  {  // -- iC_SubjectCode --
    ColumnXmlEncloser encloser(pColumn, iC_SubjectCode + bOC, pItems);

    addLongToColumn(pColumn, rLawsuit.subject);
  }  // -- iC_SubjectCode --
  if((pColumn = columns[iC_DateDecisionEnacted]) != NULL)
  {  // -- iC_DateDecisionEnacted --
    ColumnXmlEncloser encloser(pColumn, iC_DateDecisionEnacted + bOC, pItems);

    if(decFound)
    {
      add_date_to_column(pColumn, pDec.Ref().fromDate);
    }
  }  // -- iC_DateDecisionEnacted --
  if((pColumn = columns[iC_DecisionFullyUpheld]) != NULL)
  {  // -- iC_DecisionFullyUpheld --
    ColumnXmlEncloser encloser(pColumn, iC_DecisionFullyUpheld + bOC, pItems);

#if INSTANCE
    if(tBH.lDecisionValidityConfirmed)
    {
      pColumn->addText(STAR_STRING);
    }
#endif
  }  // -- iC_DecisionFullyUpheld --
  if((pColumn = columns[iC_DecisionPartiallyChanged]) != NULL)
  {  // -- iC_DecisionPartiallyChanged --
    ColumnXmlEncloser encloser(pColumn, iC_DecisionPartiallyChanged + bOC,
      pItems);
#if INSTANCE
    if(tBH.lDecisionRepealedPartilally)
    {
      pColumn->addText(STAR_STRING);
    }
#endif
  }  // -- iC_DecisionPartiallyChanged --
  if((pColumn = columns[iC_DecisionRepealed_NewDecision]) != NULL)
  {  // -- iC_DecisionRepealed_NewDecision --
    ColumnXmlEncloser encloser(pColumn, iC_DecisionRepealed_NewDecision + bOC,
      pItems);

# if INSTANCE
    if(tBH.lDecisRepealFully_NewDecis)
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_DecisionRepealed_NewDecision --
  if((pColumn = columns[iC_DecRepealed_Reconsideration]) != NULL)
  {  // -- iC_DecRepealed_Reconsideration --
    ColumnXmlEncloser encloser(pColumn, iC_DecRepealed_Reconsideration + bOC,
      pItems);
# if INSTANCE
    if(tBH.lDecisRepeal_Reconsideration)
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_DecRepealed_Reconsideration --

  if((pColumn = columns[iC_DecisionInvalidated]) != NULL)
  {  // -- iC_DecisionInvalidated --
    ColumnXmlEncloser encloser(pColumn, iC_DecisionInvalidated + bOC,
      pItems);
# if INSTANCE
    if(tBH.lDecisRepealFully_Returned)  // Test it to be sure...
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_DecisionInvalidated --
  if((pColumn = columns[iC_LawsuitCanceled]) != NULL)
  {  // -- iC_LawsuitCanceled --
    ColumnXmlEncloser encloser(pColumn, iC_LawsuitCanceled + bOC, pItems);
# if INSTANCE
    if(tBH.lCanceleds)
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_LawsuitCanceled --
  if((pColumn = columns[iC_SessionJudge]) != NULL)
  {  // -- iC_SessionJudge --
    ColumnXmlEncloser encloser(pColumn, iC_SessionJudge + bOC, pItems);

    if(ucnPresent(pSess.Ref().judge))
    {
      if(citizenUCNAndUCNTypeToName(pSess.Ref().judge, UCN_CITIZEN_UCN, buff,
        buffL, true, true))
      {
        pColumn->addText(buff);
      }
    }
  }  // -- iC_SessionJudge --
  if((pColumn = columns[iC_LawsuitJudge]) != NULL)
  {  // -- iC_LawsuitJudge --
    ColumnXmlEncloser encloser(pColumn, iC_LawsuitJudge + bOC, pItems);

    if(ucnPresent(rLawsuit.judge))
    {
      if(citizenUCNAndUCNTypeToName(rLawsuit.judge, UCN_CITIZEN_UCN, buff,
        buffL, true, true))
      {
        pColumn->addText(buff);
      }
    }
  }  // -- iC_LawsuitJudge --
  if((pColumn = columns[iC_DateSentToAnotherInstance]) != NULL)
  {  // -- iC_DateSentToAnotherInstance --
    ColumnXmlEncloser encloser(pColumn, iC_DateSentToAnotherInstance + bOC,
      pItems);

    // Don't know yet
    // See also
    // iC_OutregNo
    // for any idea how this data could be retrieved
    // or why it cannot be retrieved
  }  // -- iC_DateSentToAnotherInstance --
  if((pColumn = columns[iC_DateRetFromAnotherInstance]) != NULL)
  {  // -- iC_DateRetFromAnotherInstance --
    ColumnXmlEncloser encloser(pColumn, iC_DateRetFromAnotherInstance + bOC,
      pItems);

    // Don't know yet
  }  // -- iC_DateRetFromAnotherInstance --
  if((pColumn = columns[iC_Appeal_FullyUpheld]) != NULL)
  {  // -- iC_Appeal_FullyUpheld --
    ColumnXmlEncloser encloser(pColumn, iC_Appeal_FullyUpheld + bOC,
      pItems);

    // Don't know yet
    //
    // This could be a challenge
    // seeking in T_OUTREG - F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE,
    // finding a record (or more), deciding which record to use
    // and examining F_RESULT_I, F_RESULT_II, F_RESULT_SUPER, F_RESULT_MJ,
    // F_RECEIVER (which court ucn should be in F_RECEIVER?).
    // This record should be found once for the method and used for all of
    // iC_Appeal_FullyUpheld, iC_Appeal_PartiallyChanged,
    // iC_Appeal_Repealed_NewDecision and iC_Appeal_Repeal_Reconsideration
    // columns - like the tBH variable
    //
    // See also
    // iC_OutregNo
    // for any idea how this data could be retrieved
    // or why it cannot be retrieved
  }  // -- iC_Appeal_FullyUpheld --
  if((pColumn = columns[iC_Appeal_PartiallyChanged]) != NULL)
  {  // -- iC_Appeal_PartiallyChanged --
    ColumnXmlEncloser encloser(pColumn, iC_Appeal_PartiallyChanged + bOC,
      pItems);

    // Don't know yet
    // See iC_Appeal_FullyUpheld
  }  // -- iC_Appeal_PartiallyChanged --
  if((pColumn = columns[iC_Appeal_Repealed_NewDecision]) != NULL)
  {  // -- iC_Appeal_Repealed_NewDecision --
    ColumnXmlEncloser encloser(pColumn, iC_Appeal_Repealed_NewDecision + bOC,
      pItems);

    // Don't know yet
    // See iC_Appeal_FullyUpheld
  }  // -- iC_Appeal_Repealed_NewDecision --
  if((pColumn = columns[iC_Appeal_Repeal_Reconsideration]) != NULL)
  {  // -- iC_Appeal_Repeal_Reconsideration --
    ColumnXmlEncloser encloser(pColumn, iC_Appeal_Repeal_Reconsideration + bOC,
      pItems);

    // Don't know yet
    // See iC_Appeal_FullyUpheld
  }  // -- iC_Appeal_Repeal_Reconsideration --
  if((pColumn = columns[iC_AppealResult_Admin]) != NULL)
  {  // -- iC_AppealResult_Admin --
    ColumnXmlEncloser encloser(pColumn, iC_AppealResult_Admin + bOC,
      pItems);

    // Don't know yet
    // See iC_Appeal_FullyUpheld
  }  // -- iC_AppealResult_Admin --
  if((pColumn = columns[iC_JudgementUpheld]) != NULL)
  {  // -- iC_JudgementUpheld --
    ColumnXmlEncloser encloser(pColumn, iC_JudgementUpheld + bOC, pItems);
# if INSTANCE
    if(tBH.lJudgemConfirmed)
    {
      pColumn->addText(STAR_STRING);
    }
# endif    
  }  // -- iC_JudgementUpheld --
  if((pColumn = columns[iC_ConditionalConvictingApplied]) != NULL)
  {  // -- iC_ConditionalConvictingApplied --
    ColumnXmlEncloser encloser(pColumn, iC_ConditionalConvictingApplied + bOC,
      pItems);
# if INSTANCE
    if(tBH.lCndJdgApply)
    {
      pColumn->addText(STAR_STRING);
    }
# endif    
  }  // -- iC_ConditionalConvictingApplied --
  if((pColumn = columns[iC_ConditionalConvictingRepealed]) != NULL)
  {  // -- iC_ConditionalConvictingRepealed --
    ColumnXmlEncloser encloser(pColumn, iC_ConditionalConvictingRepealed + bOC,
      pItems);
# if INSTANCE
    if(tBH.lCndJdgRepeal)
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_ConditionalConvictingApplied --
  if((pColumn = columns[iC_PunishmentDecreased]) != NULL)
  {  // -- iC_PunishmentDecreased --
    ColumnXmlEncloser encloser(pColumn, iC_PunishmentDecreased + bOC,
      pItems);
# if INSTANCE
    if(tBH.lJdgChg_PunismDecr)
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_PunishmentDecreased --
  if((pColumn = columns[iC_PunishmentIncreased]) != NULL)
  {  // -- iC_PunishmentIncreased --
    ColumnXmlEncloser encloser(pColumn, iC_PunishmentIncreased + bOC,
      pItems);
# if INSTANCE
    if(tBH.lJdgChg_PunismIncr)
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_PunishmentIncreased --
  if((pColumn = columns[iC_AnotherChangesPunishmentPart]) != NULL)
  {  // -- iC_AnotherChangesPunishmentPart --
    ColumnXmlEncloser encloser(pColumn, iC_AnotherChangesPunishmentPart + bOC,
      pItems);
# if INSTANCE
    if(tBH.lOtherChangesPunishmPart)
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_AnotherChangesPunishmentPart --
  if((pColumn = columns[iC_ChangesCitizenPart]) != NULL)
  {  // -- iC_ChangesCitizenPart --
    ColumnXmlEncloser encloser(pColumn, iC_ChangesCitizenPart + bOC,
      pItems);
# if INSTANCE
    if(tBH.lOtherChangesCitizenPart)
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_ChangesCitizenPart --
  if((pColumn = columns[iC_JudgemRepealPart_Reconsider]) != NULL)
  {  // -- iC_JudgemRepealPart_Reconsider --
    ColumnXmlEncloser encloser(pColumn, iC_JudgemRepealPart_Reconsider + bOC,
      pItems);
# if INSTANCE
    if(tBH.lJdgRepealPartReconsider)
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_JudgemRepealPart_Reconsider --
  if((pColumn = columns[iC_Reconsideration]) != NULL)
  {  // -- iC_Reconsideration --
    ColumnXmlEncloser encloser(pColumn, iC_Reconsideration + bOC, pItems);
# if INSTANCE
    if(tBH.lJdgFullyAboli_Ret)
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_Reconsideration --
  if((pColumn = columns[iC_NewJudgement]) != NULL)
  {  // -- iC_NewJudgement --
    ColumnXmlEncloser encloser(pColumn, iC_NewJudgement + bOC, pItems);
# if INSTANCE
    if(tBH.lJdgFullyAboli_NewJ)
    {
      pColumn->addText(STAR_STRING);
    }
# endif
  }  // -- iC_NewJudgement --
  if((pColumn = columns[iC_LetterToFromAnotherInstData]) != NULL)
  {  // -- iC_NewJudgement --
    ColumnXmlEncloser encloser(pColumn, iC_LetterToFromAnotherInstData + bOC,
      pItems);

    // Don't know yet
  }  // -- iC_LetterToFromAnotherInstData --

///////////////////
  if((pColumn = columns[iC_ArchiveFileNo]) != NULL)
  {  // -- iC_ArchiveFileNo --
    ColumnXmlEncloser encloser(pColumn, iC_ArchiveFileNo + bOC, pItems);

    // Nothing yet
  }  // -- iC_ArchiveFileNo --
  if((pColumn = columns[iC_ArchiveLinkNo]) != NULL)
  {  // -- iC_ArchiveLinkNo --
    ColumnXmlEncloser encloser(pColumn, iC_ArchiveLinkNo + bOC, pItems);
    // Nothing yet
  }  // -- iC_ArchiveLinkNo --

  //for new books REGIONAL and AREAL I inst
  if((pColumn = columns[iC_Source_alter]) != NULL)              //15076
  {  // -- iC_Source_alter --
    ColumnXmlEncloser encloser(pColumn, iC_Source_alter + bOC, pItems);
    // 20070201  sourceNoAndSenderToString_alter (rLawsuit, buff, buffL) ;
    // 20080623  add source kind ->
    {
      TKind k ;
      k.kind = rLawsuit.sourceKind ;
      TRequest req ;
      req.key = rLawsuit.source ;
      // 2008:302 LPR/LRQ: Get() -> Try()
      if (k.Get () && req.Try ())
      {  buff[0] = '\0' ;
         ostrstream (buff, buffSz)
             << k.name << " вх Nо " << req.key.no << "/ " << req.date << "г." <<  ends ;
         buff [buffSz - 1] = '\0' ;
         pColumn -> addText (buff) ;
      }
    }//  20080623  add source kind <-
    buff[0] = '\0' ;
    sourceNoAndSenderToString (rLawsuit, buff, buffL) ;
    pColumn -> addText (buff) ;
  }  // -- iC_Source_alter --

  //for new books AREAL II inst
  if((pColumn = columns[iC_1stInstLawsuitAndCourt]) != NULL)    //15036
  {  // -- iC_1stInstLawsuitAndCourt --
    ColumnXmlEncloser encloser(pColumn, iC_1stInstLawsuitAndCourt + bOC,
      pItems);
//20070215 # if INSTANCE  // AREAL, APPEAL
//20070215    addInDocAndRelatedLawsToColumn (pColumn, rLawsuit, buff, buffSz) ;
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
    addRelated_1st_Inst_LawsToColumn (pColumn, rLawsuit, buff, buffSz ) ;
#endif  // APPEAL || ADMIN || AREAL || MILITARY
  }  // -- iC_1stInstLawsuitAndCourt --

  if((pColumn = columns[iC_LawNoAndFastProcedure]) != NULL)
  {  // -- iC_LawNoAndFastProcedure --
    ColumnXmlEncloser encloser(pColumn, iC_LawNoAndFastProcedure + bOC, pItems);

    ostrstream(buff, buffL) << rLawsuit.key.no << ends ;

# if 0
// from Petyo
    if(quick_formerly_quick(rLawsuit.quick))
    {
      strConcat(buff, " БО", buffL);
    }
    else
      if(quick_formerly_standard(rLawsuit.quick))
      {
        strConcat(buff, " ОБ", buffL);
      }
      else
        if(quick_currently_quick(rLawsuit.quick))
        {
          strConcat(buff, " Б", buffL);
        }
    pColumn -> addText (buff) ;
# endif
// 20060324 ->
    if ((rLawsuit.quick & QUICK_CONVERTED) == 0)
    {
       if (quick_pure_quick(rLawsuit.quick)) pColumn -> addText ("Б") ;
       else if (quick_pure_immediate(rLawsuit.quick)) pColumn -> addText ("Н") ;
       else {} // no sign
    }
    else // with conversion
    {  if (     quick_formerly_standard_to_quick(rLawsuit.quick)) pColumn -> addText ("ОБ") ;
       else if (quick_formerly_standard_to_immediate(rLawsuit.quick)) pColumn -> addText ("ОН") ;
       else if (quick_formerly_quick_to_standard(rLawsuit.quick)) pColumn -> addText ("БО") ;
       else if (quick_formerly_immediate_to_standard(rLawsuit.quick)) pColumn -> addText ("НО") ;
       else {pColumn -> addText ("+") ;}  // don't know
    }
// 20060324  <-
#if COURT_TYPE == COURT_REGIONAL
    if (rLawsuit.sourceKind == KIND_REQUEST_126G)
	  pColumn -> addText ("126 Ж") ;
#endif  // REGIONAL
  }  // -- iC_LawNoAndFastProcedure --
  // 10.feb.2005 - end
  delete [] b1 ;
  delete [] buff ;
 }  // deskBookColumns :: makeColumns

deskBookColumns :: deskBookColumns (const char * formatFN,
                                    TWindow * parent,
                                    TLawsuit & rLawsuit,
					 TStringArray * pItems   // 10.feb.2005
                                   ) :
			   columnsArr (formatFN, parent, i_columns,
                                 OFFSET_DESK_BOOK_CIPHER + 1)
# if INSTANCE
                     , companyType ('\0')
# endif
 {
  makeColumns (rLawsuit, pItems) ;
  defineMaxRows () ;
 }  // deskBookColumns :: deskBookColumns

# if INSTANCE
char deskBookColumns :: getDamnType () const
 {
  return companyType ;
 }  // deskBookColumns :: getDamnType
# endif

// ----------------------- deskBookOutFile - begin ----------------------

class deskBookOutFile : public frmHeadOutText
 {
  protected :
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
  public :
    deskBookOutFile (TWindow * parent,
			   const char * hFName = NULL) ;
    int formattedHead (const begNEndNYearTypes & bnenyt,
			     bool write = true,
			     ostream* altStreamPtr = NULL  // 17.feb.2005
			    ) ;
    void writeItem (deskBookColumns & dBC, bool first) ;
    int linesForLS (unsigned int sidePairs) ;
    void SetPageLines(int n);  // 17.feb.2005
    void IncLines(int n);  // 17.feb.2005
    void IncItemLines(bool first);  // 17.feb.2005

 } ;  // class deskBookOutFile

deskBookOutFile :: deskBookOutFile (TWindow * parent,
						const char * hFName) :
			   frmHeadOutText (parent, hFName)
 {
 }  // deskBookOutFile :: deskBookOutFile

void deskBookOutFile :: writeItem (deskBookColumns & dBC,
					     bool first)
 {
  if (pM.newPageNeed ())
    toBeContinued () ;
  dBC.notConstOutput (* this, first) ;
  pM.incLines (linesOfItem (first)) ;
 } ;  // deskBookOutFile :: writeItem

int deskBookOutFile :: linesForLS (unsigned int sidePairs)
 {
  int l = 0 ;

  if (sidePairs --)
   {
    l += itemLines ;
    l += sidePairs * itemLines1 ;
   }
  return l ;
 }  // deskBookOutFile :: linesForLS

void deskBookOutFile::SetPageLines(int n)
{  // 17.feb.2005
  pM.setParams(n, pM.getLines());
}

void deskBookOutFile::IncLines(int n)
{  // 17.feb.2005
  pM.incLines(n);
}

void deskBookOutFile::IncItemLines(bool first)
{  // 17.feb.2005
  pM.incLines(linesOfItem(first));
}

void deskBookOutFile :: fHOutput (const void * p, char * s, size_t len,
					    int algn, size_t skip, int dCipher)
{  // an old varianth in VersionArchives\2365\_appname.j --> _descbk.cpp
  ostream* const pStream = pAltStream ? pAltStream : this;

  if (! begNEndNYearTypesOutput (* ((const begNEndNYearTypes *) p),
					   * pStream, s, len, algn, skip, dCipher))
    miscOutput (* pStream, s, len, algn, skip, dCipher) ;
}  // deskBookOutFile :: fHOutput

int deskBookOutFile::formattedHead(const begNEndNYearTypes& bnenyt,
  bool write, ostream* altStreamPtr)  // 17.feb.2005 altStreamPtr
{  // an old varianth in VersionArchives\2365\_appname.j --> _descbk.cpp
  int res = 0;

  pAltStream = altStreamPtr;
  try
  {
    res = formHead(&bnenyt, write);
  }
  CATCH_ANYTHING
  pAltStream = NULL;
  return res;
}  // deskBookOutFile :: formattedHead

// ----------------------- deskBookOutFile - end ----------------------
class TDescBookThread : public TWaitThread
{  // 19.jun.2002
	public:
		TDescBookThread(const char coll,
			begNEndNYear& rBneny, const char* formatFN, unsigned& maxLawNo_,
			subjectHeapArray& sHArr, kindHeapArray& kHArr,
			subjectRangeHeapArray& sRHArr, mergedKindsHeapArray& mKHArr,
# if INSTANCE
			orderedDamnTypeHeapArray& dTHArr,
# endif
			unsigned& lawsCount, char* const crit, deskBookOutFile& outf);

	protected:
		const char college;
		begNEndNYear& bneny;
		const char* const formatFName;
		unsigned& maxLawNo;
		subjectHeapArray& sHA;
		kindHeapArray& kHA;
		subjectRangeHeapArray& sRHA;
		mergedKindsHeapArray& mKHA;
# if INSTANCE
		orderedDamnTypeHeapArray& dTHA;
# endif
		unsigned& lawsCnt;
		char* const criteria;
		deskBookOutFile& of;

		bool ArgsOK();
		virtual void Execute();
		void ChangeCurrentItemText(const TRCDKey key,
			const int current, const int total);
    void WriteHeader();  // 17.feb.2005
    void WriteOneLawsuit(TLawsuit& bLawsuit
#if INSTANCE
	, char& companyType
#endif  // of INSTANCE
	);  // 17.feb.2005
};

TDescBookThread::TDescBookThread(const char coll,
	begNEndNYear& rBneny, const char* formatFN, unsigned& maxLawNo_,
	subjectHeapArray& sHArr, kindHeapArray& kHArr,
	subjectRangeHeapArray& sRHArr, mergedKindsHeapArray& mKHArr,
# if INSTANCE
	orderedDamnTypeHeapArray& dTHArr,
# endif
	unsigned& lawsCount, char* const crit, deskBookOutFile& outf) :
		college(coll), bneny(rBneny), formatFName(formatFN),
		maxLawNo(maxLawNo_), sHA(sHArr), kHA(kHArr), sRHA(sRHArr),
		mKHA(mKHArr),
# if INSTANCE
		dTHA(dTHArr),
# endif
		lawsCnt(lawsCount), criteria(crit), of(outf)
{
	if(ArgsOK() == false)
		error("%s\n%s::%s", WRONG_ARGS_PASSED_TO,
					typeid(*this).name(), typeid(*this).name());
}

bool TDescBookThread::ArgsOK()
{
	return (formatFName && formatFName[0] && criteria && criteria[0] && of);
}

void TDescBookThread::ChangeCurrentItemText(const TRCDKey key,
	const int current, const int total)
{
	char buff[128];
	ostrstream oss(buff, sizeof(buff));
	TRCDKey_ k(key);

	if(k.FilledOK())
		oss << k << " ";
	oss << "(" << current << " от " << total << ")";
	oss << ends;
	buff[sizeof(buff) - 1] = '\0';
	wtdl->SetText(buff);
}

void TDescBookThread::WriteHeader()
{  // 17.feb.2005
  begNEndNYearTypes bnenyt;

  bnenyt << bneny;
  ZERO_LEN(bnenyt.types);
  strConcatCh(bnenyt.types, college ,sizeof(bnenyt.types));
  {
    of.title() ;
    of.formattedHead(bnenyt);
    of.head() ;
  }
}

void TDescBookThread::WriteOneLawsuit(TLawsuit& bLawsuit
#if INSTANCE
  , char& companyType
#endif  // of INSTANCE
  )
{
  TStringArray* pItems = NULL;
  deskBookColumns dBC(formatFName, wtdl->Parent, bLawsuit, pItems);
  const int rows = dBC.getMaxRows();

#if INSTANCE
  if(college == TYPE_FIRM_LAW)
  {
    companyType = dBC.getDamnType();
  }
  else
  {
    companyType = '\0';
  }
#endif  // of INSTANCE
  {
    of.nextLSToPrint(rows + 1);
    of.horzLine();
    for(int i = 0; i < rows; i ++)
	of.writeItem(dBC, true);
  }
}

void TDescBookThread::Execute()
{
	if(ArgsOK())
	{
		try
		{
			wtdl->SetHeader("Описна книга");
			wtdl->PleaseWait();

			TCountRecsQuery q (criteria) ;
			TBookLawsuit bLawsuit (formatFName, wtdl->Parent) ;
			const bool fmtLoadOK = bLawsuit.formatLoadIsOK () ;
			const int total = q.getRecsCount();

			wtdl->SetProgressRange(0, total);

	if(total > 0)
	{
	  WriteHeader();
	}

	SimpleStringsArray pageAttribs;

			while (
						 fmtLoadOK  // this is allwais the same
						 && q.Read ())
			{  // -- 3 --
				if(wtdl->WantBreak())
					break;
				lawsCnt ++ ;

				bLawsuit.clearData () ;
				bLawsuit << q ;
				ChangeCurrentItemText(bLawsuit.key, lawsCnt, total);
				if(bLawsuit.Get())
				{  // -- 4 --
					if (bLawsuit.key.no > maxLawNo)
						maxLawNo = bLawsuit.key.no ;
					 {
						subjectHeap sH ;

						sH.nom.subject = bLawsuit.subject ;
						sH.nom.type = bLawsuit.key.type ;

						if (sH.nom.Get ())
							sHA.incItem (& sH) ;
					 }
					 {
						kindHeap kH ;

						kH.nom.kind = bLawsuit.kind ;
						if (kH.nom.Get ())
							kHA.incItem (& kH) ;
					 }
					 {
						TSubject subj ;

						subj.type = bLawsuit.key.type ;
						subj.subject = bLawsuit.subject ;
						sRHA.incSRItems (subj) ;
					 }
					mKHA.incMKItems (bLawsuit.kind) ;

					 {
# if INSTANCE
		char companyType = '\0';
# endif

		WriteOneLawsuit(bLawsuit
# if INSTANCE
		  , companyType
# endif
		  );
# if INSTANCE
						if (college == TYPE_FIRM_LAW)
						 {
							orderedDamnTypeHeap oDTH ;

							oDTH.nom.companyType = companyType;  // dBC.getDamnType () ;
							if (oDTH.nom.companyType)
								if (oDTH.nom.Get ())
									dTHA.incItem (& oDTH) ;
						 }
# endif
					 }
				}  // -- 4 --
				wtdl->SetProgress(lawsCnt);
			}  // -- 3 --
			wtdl->AlmostDone();
			  of.footherAndTail () ;
			if(lawsCnt < total)  // in case of WantBreak() == true
				lawsCnt = total;
		}
		CATCH_ANYTHING
	}
}

void printAnyDescBook(TWindow *pParent, const char college, begNEndNYear bneny, const char *formatFName, unsigned &maxLawNo,
	subjectHeapArray &sHA, kindHeapArray &kHA, subjectRangeHeapArray &sRHA, mergedKindsHeapArray &mKHA,
# if INSTANCE
	orderedDamnTypeHeapArray & dTHA,
# endif
	unsigned & lawsCnt)
{
	if (!(formatFName && formatFName [0]))
		return;

	char criteria [256 + 128] ;

	maxLawNo = 0 ;

	if (!compose_desk_book_criteria(criteria, sizeof(criteria), college, bneny))
		return;

	deskBookOutFile of (pParent, formatFName) ;

	try
	{
	#if SPEEDTESTING
		#pragma message speed testing only
		DWORD ticks = GetTickCount();
	#endif  // SPEEDTESTING
		TDescBookThread(college, bneny, formatFName, maxLawNo, sHA, kHA, sRHA, mKHA,
		#if INSTANCE
			dTHA,
		#endif
			lawsCnt, criteria, of).ExecWait();
	#if SPEEDTESTING
		message("test version: %lu d-seconds\n", (GetTickCount() - ticks) / 100);
	#endif  // SPEEDTESTING
	}
	CATCH_ANYTHING

	show_text_stream_use_settings(NULL, of, formatFName, FLOAT_ALIGNMENT);
}

void summaryColumns :: makeColumns (const summaryData & sD)
 {
  if (! columns)
    return ;
  column * pColumn ;

  if ((pColumn = columns [iC_Name]) != NULL)
   {  // -- iC_Name --
    pColumn -> addText (sD.name) ;
   }  // -- iC_Name --
  if ((pColumn = columns [iC_Heap]) != NULL)
   {  // -- iC_Heap --
    addLongToColumn (pColumn, sD.heap, true) ;
   }  // -- iC_Heap --
 }  // summaryColumns :: makeColumns

summaryColumns :: summaryColumns (const char * const formatFN,
					    TWindow * parent,
					    const summaryData & sD) :
			   columnsArr (formatFN, parent, i_columns,
					   OFFSET_SUMMARY_DATA_CIPHER + 1)
 {
  makeColumns (sD) ;
  defineMaxRows () ;
 }  // summaryColumns :: summaryColumns

struct SummaryDataPercentage: public summaryData
{  // 11.feb.2005
  SummaryDataPercentage();
  void Clear();
  long int percentage;
};  //  SummaryDataPercentage

SummaryDataPercentage::SummaryDataPercentage()
{
  Clear();
}

void SummaryDataPercentage::Clear()
{
  summaryData::clear();
  percentage = 0;
}

void printDescBkSummary(TWindow * pParent, const char college, const begNEndNYear &bneny, const char *formatFName,
	const unsigned maxLawNo, const subjectHeapArray &sHA, const kindHeapArray &kHA, const subjectRangeHeapArray &sRHA,
				 const mergedKindsHeapArray & mKHA,
#if INSTANCE
				 orderedDamnTypeHeapArray & dTHA,
#endif
				 const unsigned lawsCnt)
{
	bool beginning = true;

	if (!(formatFName && pParent))
		return;

	summaryDataOutFile outF(pParent, formatFName);

	{  // -- 2 --
		summaryFmtHeadData sFHD;
		unsigned long it;
		const int sIt = sHA.Count();
		const int kIt = kHA.Count();
		const int sRIt = sRHA.Count();
		const int mKIt = mKHA.Count();
	#if INSTANCE
		const int dTIt = dTHA.Count();
	#endif

		sFHD.bNENY = bneny;
		sFHD.bNENY.endN = maxLawNo;
		scSprintf(sizeof sFHD.cCBDEDBN.colleges, sFHD.cCBDEDBN.colleges, "%c", college);
		for (it = 0 ; it < sIt + kIt + sRIt + mKIt
	#if INSTANCE
			+ dTIt
	#endif
			+ 1; it ++)
		{  // -- 3 --
			summaryData sD;

			if (beginning)
			{
				outF.title();
				outF.formattedHead(sFHD);
				outF.head();
				beginning = false;
			}

			if (it < kIt)
			{
				const kindHeap &kH = *(kHA[it]);

				scSprintf(sizeof sD.name, sD.name, "%s", kH.nom.name);
				sD.heap = kH.heap;
			}
			else

			if (it < mKIt + kIt)
			{
				const mergedKindsHeap &mK = *(mKHA[it - kIt]);

				scSprintf (sizeof sD.name, sD.name, "%s", mK.nom.alias);
				sD.heap = mK.heap;
			}
			else if (it < sIt + mKIt + kIt)
			{
				const subjectHeap &sH = *(sHA[it - (mKIt + kIt)]);

				scSprintf (sizeof sD.name, sD.name, "%s", sH.nom.name);
				sD.heap = sH.heap;
			}
			else if (it < sRIt + sIt + mKIt + kIt)
			{
				const subjectRangeHeap & sH = *(sRHA[it - (sIt + mKIt + kIt)]);

				scSprintf(sizeof sD.name, sD.name, "%s", sH.nom.name);
				sD.heap = sH.heap;
			}
		#if INSTANCE
			else if (it < dTIt + sRIt + sIt + mKIt + kIt)
			{
				const orderedDamnTypeHeap & oDTH = *(dTHA[it - (sRIt + sIt + mKIt + kIt)]);

				scSprintf(sizeof sD.name, sD.name, "%s", oDTH.nom.name);
				sD.heap = oDTH.heap;
				// !!!!!!!
				if (sD.heap == 0)
					continue ;  // for
			}
		#endif
			else
			{
				scSprintf(sizeof sD.name, sD.name, "%s", "ВСИЧКО:");
				sD.heap = lawsCnt;
			}

			{
				summaryColumns sC(formatFName, pParent, sD);
				const int rows = sC.getMaxRows();
				int i;

				outF.nextItemToPrint(rows + 1, sFHD);
			#if INSTANCE
				if (dTIt == 0)
					outF.horzLine();
			#else  // of INSTANCE
				outF.horzLine();
			#endif  // of INSTANCE
				for (i = 0; i < rows; i++)
					outF.writeItem(sC, true);
			}
		}  // -- 3 --

		outF.footherAndTail();
	}  // -- 2 --

	show_text_stream_use_settings (NULL, outF, formatFName, FLOAT_ALIGNMENT);
}  // printDescBkSummary

static constant DATA_FOR_SUMMARY = "Данни за рекапитулация";
static constant PROCESSED_LAWS = "Обработени дела:";

class TDescBkGatherCVSummaryData : public TCheckupThread
{  // 19.jun.2002
	public:
		TDescBkGatherCVSummaryData(const char aCollege,
			const begNEndNYear& rBneny, const char* const fmtFName,
			const unsigned maxLawN, const unsigned lawsCount,
			char* const crit, CodeValueArray& codeValArr);
	protected:
		const char college;
		const begNEndNYear& bneny;
		const char* const formatFName;
		const unsigned maxLawNo;
		const unsigned lawsCnt;
		char* const criteria;
		CodeValueArray& codeValueArr;

		virtual bool ArgsOk();
		virtual void Execute();
};  // class DescBkGatherCVSummaryData

TDescBkGatherCVSummaryData::TDescBkGatherCVSummaryData(const char aCollege,
	const begNEndNYear& rBneny,
	const char* const fmtFName, const unsigned maxLawN, const unsigned lawsCount,
	char* const crit, CodeValueArray& codeValArr) :
		college(aCollege), bneny(rBneny), formatFName(fmtFName),
		maxLawNo(maxLawN), lawsCnt(lawsCount), criteria(crit),
		codeValueArr(codeValArr)
{
}

bool TDescBkGatherCVSummaryData::ArgsOk()
{
	return TCheckupThread::ArgsOk() && formatFName && formatFName[0] &&
		criteria && criteria[0];
}

void TDescBkGatherCVSummaryData::Execute()
{
	try
	{
		long int vTotal = lawsCnt ;  // It may be changed later!

		long int vCit2nd = 0 ;
		long int vCit2ndByCompl = 0 ;
		long int vCit2ndByQuickCompl = 0 ;
		long int vCit2ndBankruptcy = 0 ;  // 24.jun.2002
		long int vCit2ndByOthers = 0 ;
		long int vCitPrv = 0 ;
		long int vCitPrvByPrvCompl = 0 ;
		long int vCitPrvByComplSlowly = 0 ;
		long int vCitPrvByEnsureRequest = 0 ;
		long int vCitPrvByJudgeEliminatingLaw = 0 ;
		long int vCitPrvByJurisdQuarrel = 0 ;
		long int vCitPrvByOthers = 0 ;
		long int vCitPrvBankruptcy = 0 ;  // 09.apr.2002
		long int vCitPrvRestoreAppealTerm = 0 ;  // 19.jun.2002
		long int vCitOthers = 0 ;

		long int vPun2nd = 0 ;
		long int vPun2ndByComplGenCh = 0 ;
		long int vPun2ndByObjection = 0 ;
		long int vPun2ndByComplObject = 0 ;
		long int vPun2ndByOthers = 0 ;
		long int vPunPrv = 0 ;
		long int vPunPrvByCancelEnactment = 0 ;
		long int vPunPrvByKeepMethodOpenSess = 0 ;
		long int vPunPrvByKeepMethodClosedSess = 0 ;
		long int vPunPrvByOthers = 0 ;
		long int vPunOthers = 0 ;

		wtdl->SetHeader(DATA_FOR_SUMMARY);
		wtdl->PleaseWait();
		passeds = PROCESSED_LAWS;
# if INSTANCE
#   if APPEAL
		{  // -- 0 --
			TCountRecsQuery q (criteria) ;
			TLawsuit law ;
	TRequest tmpReq;  // 24.sep.2003

			totalPartSteps = q.getRecsCount();
			wtdl->SetProgressRange(0, totalPartSteps);
			endingVal = totalPartSteps;
			if(vTotal < totalPartSteps)
				vTotal = totalPartSteps;
			while (q.Read ())
			{  // -- 1 --
				Tick(false);
				if(wtdl->WantBreak())
					break;
				law << q ;
				if(law.Get())
				{  // -- 1.5 --
					if (law.key.type == TYPE_CITIZEN_LAW
		  || law.key.type == TYPE_TRADE_LAW  // 28.jun.2004    // APPEAL
		 )
					{  // -- 2 --
						if (law.kind == KIND_CIT_2ND_LAW
		    || law.kind == KIND_TRA_2ND_LAW  // 28.jun.2004
		   )
						{  // -- 3 --
							vCit2nd ++ ;
							if (bankruptcy_law (law))  // 24.jun.2002 - begin
								vCit2ndBankruptcy ++ ;
							else  // 24.jun.2002 - end
								if (law.sourceKind == KIND_COMPLAINT_R)
									vCit2ndByOthers ++ ;
								else
		     {  // 24.sep.2003 - begin
			const bool quickReq =
			  quick_law_source_currently_quick(law.source, tmpReq);
			  // 24.sep.2003 - end

									if (strchr (KIND_EXACT_COMPLAINTS, law.sourceKind) &&
			  // 24.sep.2003 - begin
			    quickReq == false
			  // 24.sep.2003 - end
			   )
			    vCit2ndByCompl ++ ;
			  else
			  // 24.sep.2003 - begin
			    if(quickReq == true)
			  // 24.sep.2003 - end
				vCit2ndByQuickCompl ++ ;
			    else
				vCit2ndByOthers ++ ;
		     }  // 24.sep.2003
						}  // -- 3 --
						else
							if (law.kind == KIND_CIT_PRV_LAW
			|| law.kind == KIND_TRA_PRV_LAW  // 28.jun.2004
		     )
							{  // -- 4 --
								vCitPrv ++ ;
								if (bankruptcy_law (law))  // 09.apr.2002 - begin
									vCitPrvBankruptcy ++ ;
								else  // 09.apr.2002 - end
								 {
									if (lawIsReturnedFrom2ndInstance (law))  // 23.may.2002
										vCitPrvByOthers ++ ;                   // 23.may.2002
									else                                     // 23.may.2002
										switch (law.sourceKind)
										 {
											case KIND_PRV_CT274_CIT :
											case KIND_PRV_CT274_TRA :
											case KIND_PRV_COMPL_CIT :
											case KIND_PRV_COMPL :
			    case KIND_PRV_COMPL_TRA :  // 28.jun.2004
												vCitPrvByPrvCompl ++ ;
												break ;
											case KIND_COMPL_SLOWLY_217 :
			    case KIND_TRADE_COMPL_SLOWLY_217 :  // 28.jun.2004
												vCitPrvByComplSlowly ++ ;
												break ;
											case KIND_ENSURE_REQUEST :
			    case KIND_TRADE_ENSURE_REQUEST :  // 28.jun.2004
												vCitPrvByEnsureRequest ++ ;
												break ;
											case KIND_JUDGE_ELIMINATION :
			    case KIND_TRADE_JUDGE_ELIMINATION :  // 28.jun.2004
												vCitPrvByJudgeEliminatingLaw ++ ;
												break ;
											case KIND_JURISD_QUARREL :
			    case KIND_TRADE_JURISD_QUARREL :  // 28.jun.2004
												vCitPrvByJurisdQuarrel ++ ;
												break ;
											case KIND_PRV_RESTORE_APPEAL_TERM :  // 19.jun.2002
			    case KIND_PRV_RESTORE_APPEAL_TERM_TRA :  // 18.jun.2004
												vCitPrvRestoreAppealTerm ++ ;      // 19.jun.2002
												break ;                            // 19.jun.2002
											default :
												vCitPrvByOthers ++ ;
												break ;
										 }
								 }
							}  // -- 4 --
							else
								vCitOthers ++ ;
					}  // -- 2 --
					else
						if (law.key.type == TYPE_PUNISHMENT_LAW)
						{  // -- 5 --
							if (law.kind == KIND_PUNI_GENERAL_CHAR_LAW_2A)
							{  // -- 6 --
								vPun2nd ++ ;
								switch (law.sourceKind)
								{
									case KIND_PUN_GEN_CH_COMPL :
									case KIND_PUN_GEN_CH_COMPL_NAHD :
										vPun2ndByComplGenCh ++ ;
										break ;
									case KIND_PUN_GEN_CH_OBJECTION :
										vPun2ndByObjection ++ ;
										break ;
									case KIND_PUN_COMPL_OBJECTION :
										vPun2ndByComplObject ++ ;
										break ;
									default :
										vPun2ndByOthers ++ ;
										break ;
								}
							}  // -- 6 --
							else
								if (law.kind == KIND_2ND_PRIVATE_PUNISHMENT_LAW)
								{  // -- 7 --
									vPunPrv ++ ;
									if (law.subject == SUBJECT_9002 ||
											law.subject == SUBJECT_9003)
										vPunPrvByKeepMethodOpenSess ++ ;
									else
										if (law.subject == SUBJECT_9001)
											vPunPrvByKeepMethodClosedSess ++ ;
										else
											if (law.sourceKind == KIND_CANCEL_ENACTMENT)
												vPunPrvByCancelEnactment ++ ;
											else
												vPunPrvByOthers ++ ;
								}  // -- 7 --
								else
									vPunOthers ++ ;
						}  // -- 5 --
				}  // -- 1.5 --
			}  // -- 1 --
			Tick(true);
		}  // -- 0 --
#   endif  // of APPEAL
# endif  // of INSTANCE
		wtdl->AlmostDone();
		codeValueArr.Add (new CodeValue (10402, vTotal)) ;

		codeValueArr.Add (new CodeValue (10403, vCit2nd)) ;
		codeValueArr.Add (new CodeValue (10404, vCit2ndByCompl)) ;
		codeValueArr.Add (new CodeValue (10405, vCit2ndByQuickCompl)) ;
		codeValueArr.Add (new CodeValue (10406, vCitPrv)) ;
		codeValueArr.Add (new CodeValue (10407, vCitPrvByPrvCompl)) ;
		codeValueArr.Add (new CodeValue (10408, vCitPrvByComplSlowly)) ;
		codeValueArr.Add (new CodeValue (10409, vCitPrvByEnsureRequest)) ;
		codeValueArr.Add (new CodeValue (10410, vCitPrvByJudgeEliminatingLaw)) ;
		codeValueArr.Add (new CodeValue (10411, vCitPrvByJurisdQuarrel)) ;
		codeValueArr.Add (new CodeValue (10412, vCit2ndByOthers)) ;
		codeValueArr.Add (new CodeValue (10413, vCitPrvByOthers)) ;
		codeValueArr.Add (new CodeValue (10424, vCitOthers)) ;
		codeValueArr.Add (new CodeValue (10569, vCitPrvBankruptcy)) ;
		codeValueArr.Add (new CodeValue (10571, vCitPrvRestoreAppealTerm)) ;
		codeValueArr.Add (new CodeValue (10585, vCit2ndBankruptcy)) ;

		codeValueArr.Add (new CodeValue (10414, vPun2nd)) ;
		codeValueArr.Add (new CodeValue (10415, vPun2ndByComplGenCh)) ;
		codeValueArr.Add (new CodeValue (10416, vPun2ndByObjection)) ;
		codeValueArr.Add (new CodeValue (10417, vPun2ndByComplObject)) ;
		codeValueArr.Add (new CodeValue (10418, vPunPrv)) ;
		codeValueArr.Add (new CodeValue (10419, vPunPrvByCancelEnactment)) ;
		codeValueArr.Add (new CodeValue (10420, vPunPrvByKeepMethodOpenSess)) ;
		codeValueArr.Add (new CodeValue (10421, vPunPrvByKeepMethodClosedSess)) ;
		codeValueArr.Add (new CodeValue (10422, vPun2ndByOthers)) ;
		codeValueArr.Add (new CodeValue (10423, vPunPrvByOthers)) ;
		codeValueArr.Add (new CodeValue (10425, vPunOthers)) ;
	}
	CATCH_ANYTHING
}

void print_desk_bk_summary (TWindow * pParent, const char college,
														const begNEndNYear & bneny,
														const char * formatFName,
														const unsigned maxLawNo,
														const unsigned lawsCnt)
{
	if(formatFName && formatFName[0])
	{
		char criteria[256 + 128];

		if(compose_desk_book_criteria(criteria, sizeof(criteria), college,
			bneny))
		{
			CodeValueArray codeValueArr;
			collCompBegDEndDBegN cCBDEDBN;
			TDescBkGatherCVSummaryData(college, bneny, formatFName,
				maxLawNo, lawsCnt, criteria, codeValueArr).Generate();
			scSprintf(sizeof(cCBDEDBN.colleges),cCBDEDBN.colleges, "%c",
				college);
			cCBDEDBN.begNo = bneny.begN;
			cCBDEDBN.endNo = maxLawNo;
			cCBDEDBN.year = bneny.year;
			writeTBSummary(formatFName, &cCBDEDBN, pParent, &codeValueArr);
		}
	}
}  // print_desk_bk_summary

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
void print_code_value_desc_bk_summary (TWindow * pParent,
						   const char college,
						   const begNEndNYear & bneny,
						   const char * formatFName,
						   const unsigned maxLawNo,
						   const unsigned lawsCnt
						  )
 {
  if (! (formatFName && formatFName [0]))
    return ;

  char criteria [256 + 128] ;

	if (! compose_desk_book_criteria (criteria, sizeof (criteria),
																		college, bneny))
		return ;

	collCompBegDEndDBegN cCBDEDBN ;

	scSprintf (sizeof (cCBDEDBN.colleges), cCBDEDBN.colleges, "%c",
						 college) ;
	cCBDEDBN.begNo = bneny.begN ;
	cCBDEDBN.endNo = maxLawNo ;
	cCBDEDBN.year = bneny.year ;
	if (bneny.want1stInstLaws != bneny.want2ndInstLaws)
		if (bneny.want1stInstLaws)
			cCBDEDBN.firstInstLaws = true ;

	long int vTotal = lawsCnt ;

	long int v1stPunTotal = 0 ;
	long int v1stPunGenCh = 0 ;
	long int v1stPunPrvNonPreCourt = 0 ;
	long int v1stPunPrvPreCourt = 0 ;
	long int v1stPunAdmCh = 0 ;

	long int v2ndPunTotal = 0 ;
	long int v2ndPunNonPreCourtNonConcern = 0 ;
	long int v2ndPunPreCourt = 0 ;
	long int v2ndPunConcern = 0 ;
	long int v2ndPun237 = 0 ;

	long int v1stCitTotal = 0 ;   // 22.oct.2001 - begin
	long int v2ndCitTotal = 0 ;
	long int v2ndCitNonSlowdown = 0 ;
	long int v2ndCitSlowdown = 0 ;  // 22.oct.2001 - end

	TQuery q (criteria) ;
	TLawsuit law ;

	while (q.Read ())
	 {  // -- 0 --
		law << q ;
		if(law.Get())  // 20.jun.2002
		 {  // -- 0.5 --
			if (law.key.type == TYPE_PUNISHMENT_LAW)  // 22.oct.2001
			 {  // -- 1 --                            // 22.oct.2001
				if (strchr (KIND_1ST_LAWSUITS, law.kind))
				 {  // -- 2 --
					v1stPunTotal ++ ;

					if (law_is_237_enactment (law))
						v2ndPun237 ++ ;
					else
						if (law.kind == KIND_PUNISHMENT_GENERAL_CHAR_LAW)
							v1stPunGenCh ++ ;
						else
							if (law.kind == KIND_PRIVATE_PUNISHMENT_LAW)
							 {
								if (law_is_pre_court (law))
									v1stPunPrvPreCourt ++ ;
								else
									v1stPunPrvNonPreCourt ++ ;
							 }
							else
								if (law.kind == KIND_1ST_ADMIN_PUNISHMENT_LAW)
									v1stPunAdmCh ++ ;
				 }  // -- 2 --
				else
				 {  // -- 3 --
					v2ndPunTotal ++ ;

					if (strchr (KIND_EXACT_COMPLAINTS_CONCERN, law.sourceKind) ||
							strchr (KIND_PRIVATE_COMPLAINTS_CONCERN, law.sourceKind))
						v2ndPunConcern ++ ;
					else
						if (law_is_pre_court (law))
							v2ndPunPreCourt ++ ;
						else
							v2ndPunNonPreCourtNonConcern ++ ;
				 }  // -- 3 --
			 }  // -- 1 --  // 22.oct.2001 - begin
			else
				if (law.key.type == TYPE_CITIZEN_LAW)
				 {  // -- 4 --
					if (strchr (KIND_1ST_LAWSUITS, law.kind))
					 {  // -- 5 --
						v1stCitTotal ++ ;
					 }  // -- 5 --
					else
					 {  // -- 6 --
						v2ndCitTotal ++ ;
						if (strchr (KIND_SLOWDOWN_COMPLAINTS, law.sourceKind))
							v2ndCitSlowdown ++ ;
						else
							v2ndCitNonSlowdown ++ ;
					 }  // -- 6 --
				 }  // -- 4 --  // 22.oct.2001 - end
		 }  // -- 0.5 --
	 }  // -- 0 --

  CodeValueArray codeValueArr ;

  codeValueArr.Add (new CodeValue (10402, vTotal)) ;

  codeValueArr.Add (new CodeValue (10436, v1stPunTotal)) ;
  codeValueArr.Add (new CodeValue (10437, v1stPunGenCh)) ;
  codeValueArr.Add (new CodeValue (10438, v1stPunPrvNonPreCourt)) ;
  codeValueArr.Add (new CodeValue (10439, v1stPunPrvPreCourt)) ;
  codeValueArr.Add (new CodeValue (10440, v1stPunAdmCh)) ;

  codeValueArr.Add (new CodeValue (10441, v2ndPunTotal)) ;
  codeValueArr.Add (new CodeValue (10442, v2ndPunNonPreCourtNonConcern)) ;
  codeValueArr.Add (new CodeValue (10443, v2ndPunPreCourt)) ;
  codeValueArr.Add (new CodeValue (10444, v2ndPunConcern)) ;
  codeValueArr.Add (new CodeValue (10458, v2ndPun237)) ;

  codeValueArr.Add (new CodeValue (10553, v1stCitTotal)) ; // 22.oct.2001-begin
  codeValueArr.Add (new CodeValue (10554, v2ndCitTotal)) ;
  codeValueArr.Add (new CodeValue (10555, v2ndCitNonSlowdown)) ;
  codeValueArr.Add (new CodeValue (10556, v2ndCitSlowdown)) ; //22.oct.2001-end

  writeTBSummary (formatFName, & cCBDEDBN, pParent, & codeValueArr) ;
 }  // print_code_value_desc_bk_summary
#   endif  // of APPEAL
# endif  // of INSTANCE
