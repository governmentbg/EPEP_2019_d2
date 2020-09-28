#include "cac.h"

bool ucnAndUcnTypeToAddress (const char * u, char t,
				     TAddress * pAddress)
 {
  if (! (u && u [0] && t && pAddress))
    return false ;
  strcpy(pAddress->ucn, u);
  pAddress -> ucnType = t ;
  return pAddress -> Try () ;
 }  // UCNAndUCNTypeToAddress

bool ucnAndUcnTypeToFrgnAddr (const char * ucn, char ucnType,
					TForeignAddress * pAddress)
 {
  strcpy(pAddress->ucn, ucn);
  pAddress -> ucnType = ucnType ;
  return pAddress -> Try () ;
 }  // ucnAndUcnTypeToFrgnAddr

# if INSTANCE
#   if APPEAL    // APPEAL
char sourceTypeToSenderUCNType (const TLawsuit & rLawsuit,
					  const char sourceKind)
#   else         // AREA
char sourceTypeToSenderUCNType (const TLawsuit & rLawsuit,
					  const char lawKind)
#   endif
# else           // REGIONAL
char sourceTypeToSenderUCNType (const TLawsuit & rLawsuit)
# endif

{
# if INSTANCE
#   if APPEAL
// 22.may.2000 - begin
  // ---- Attention! If you change this code, then change the code of
  // request_output function, 'case 8', too.

  if (rLawsuit.sourceKind != sourceKind)
  {//
  }

  if (KIND_CANCEL_ENACTMENT == sourceKind)      //  Постановление по чл.243 НПК
    // 20061220  return UCN_INSTANCE_APROS ;    //R Прокуратури
    return UCN_INSTANCE_PROSEC ;
  else
    if (sourceKind == '\0')
      return '\0' ;                             //R '\0'
    else
// 22.may.2000 - end
      // 20061220  return UCN_INSTANCE_COURT ;
      return UCN_INSTANCE_LOWER ;               //R Подчинени РС
  // ----
#   else  // of APPEAL
  //
  // if You change the processing of the lawKind parameter,
  // do not forget to review/rewrite the
  // generating of the iC_Sender column of requestColumns
  // (see _reqdia.cpp)
  //
  char sourceType = rLawsuit.source.type;  // 20070109

  if (strchr (KIND_1ST_LAWSUITS, lawKind))      // "ODI" "Z" "T" "яН" "ж"
   {  // -- 0 --                                // НОХД, НАХД, ЧНД, ГД, ТД, АД, ФД, Частно административно дело
    switch (sourceType)
     {
      case TYPE_RETURNED :                      // 'V' - Върнат документ
        return UCN_INSTANCE_HIGHER ;            //R За обжалване
      case TYPE_JURISD :                        // 'j' - Пристигнал документ
      	return UCN_INSTANCE_EQUAL ;             //R По подсъдност
      case TYPE_REQUEST :                         // 'r' - Входящ документ
      {   // 20070109 -->
          char res = '\0';
          TLawsuit * pLawsuit = new TLawsuit ;
          pLawsuit -> key = rLawsuit.key;
          TConnect *c = get_connect(pLawsuit, CONNECT_TYPE_JURISDICTIONS);
          if ( c != NULL ) res = UCN_INSTANCE_EQUAL ;
          delete c;
          delete pLawsuit;
          return res;
          // 20070109 <--
      }
      default :
        return '\0' ;                           //R '\0'
     }
   }  // -- 0 --
  else
    //
    // if You change the processing of the lawKind parameter,
    // do not forget to review/rewrite the
    // generating of the iC_Sender column of requestColumns
    // (see _reqdia.cpp)
    //
    if (strchr (KIND_2ND_LAWSUITS, lawKind))    // "WCUHП" "N" "n" "Я" ""
     {  // -- 1 --                              // ВНОХД, ВНЧХД, КНАХД, ВЧНД, ВНАХД,
      switch (sourceType)                       // Гражданско дело (В), Търговско дело (В), Административно дело (К)
       {
        case TYPE_RETURNED :                    // 'V' - Върнат документ
          return UCN_INSTANCE_LOWER ;  // !!!   //R Подчинени РС
        case TYPE_JURISD :                      // 'j' - Пристигнал документ
          return '\0' ;                         //R'\0'
        default :
          return UCN_INSTANCE_LOWER ;           //R Подчинени РС
       }
     }  // -- 1 --
    else
      return '\0' ;
#   endif  // of APPEAL
# else
  char sourceType = rLawsuit.source.type;  // 20070109

  switch (sourceType)
   {
    case TYPE_RETURNED :                        // 'V' - Върнат документ
      return UCN_INSTANCE_HIGHER ;              //R За обжалване
    case TYPE_JURISD :                          // 'j' - Пристигнал документ
      return UCN_INSTANCE_EQUAL ;               //R  По подсъдност
    case TYPE_REQUEST :                           // 'r' - Входящ документ
      {   // 20070109 -->
          char res = '\0';
          TLawsuit * pLawsuit = new TLawsuit ;
          pLawsuit -> key = rLawsuit.key;
          TConnect *c = get_connect(pLawsuit, CONNECT_TYPE_JURISDICTIONS);
          if ( c != NULL ) res = UCN_INSTANCE_EQUAL ;
          delete c;
          delete pLawsuit;
          return res;
          // 20070109 <--
      }

    default :
      return '\0' ;
   }
# endif
 }  // sourceTypeToSenderUcnType

bool TRCDKeyToLawsuit (TRCDKey key, TLawsuit & lawsuit)
 {
  char criteria [256] ;

  ostrstream (criteria, sizeof (criteria))
    << "SELECT * FROM T_LAWSUIT WHERE F_TYPE = \'"
    << key.type << "\' AND F_YEAR = " << key.year
    << " AND F_NO = " << key.no << " ;" << ends ;
  criteria [sizeof (criteria) - 1] = '\0' ;
   {
		TQuery  // TCountRecsQuery  // 14.jun.2002
		q (criteria) ;

		if (q.Read ())
		 {
			lawsuit << q ;
			return true ;
		 }
		else
			return false ;
	 }
 }  // TRCDKeyToLawsuit

bool uclpCodeToTUCLP (long code, TUCLP & Uclp)
 {
  Uclp.uclp = code ;
  return Uclp.Try () ;
 }  // uclpCodeToTUCLP

# if INSTANCE
bool lawsuitToDamn (TLawsuit * pLawsuit, TDamn * pDamn)
 {
  if (pLawsuit && pDamn && pLawsuit -> key.type == TYPE_FIRM_LAW)
   {
    char crit [256] ;

    clearGroupData (* pDamn) ;
    ostrstream (crit, sizeof (crit)) <<
	"SELECT * FROM " << pDamn -> Name << " WHERE "
      "F_TYPE = \'" << pLawsuit -> key.type << "\' AND "
      "F_YEAR = " << pLawsuit -> key.year << " AND "
      "F_NO = " << pLawsuit -> key.no << " ;" << ends ;
    crit [sizeof (crit) - 1] = '\0' ;
    if (criteria_finished_ok (crit, "lawsuitToDamn criteria"))
     {
      char tmp [128] ;
      TQuery q (crit) ;

	TRCDKey_ (pLawsuit -> key).Print (tmp, sizeof (tmp)) ;
	if (q.Read ())
	 {
	  (* pDamn) << q ;
	  if (q.Read ())
         {
           {
            TDamn * pTmpDamn = new TDamn ;
            const bool more = q.Read () ;

            (* pTmpDamn) << q ;
            error ("%s:\nПовече от една фирма по делото. Проверете \n"
			 "\"%s\" и\n"
			 "\"%s\".%s",
			 tmp, pDamn -> name, pTmpDamn -> name,
			 more ? "\nИма и още." : "") ;
		delete pTmpDamn ;
	     }
	    clearGroupData (* pDamn) ;
	    return false ;
	   }
	  else
	    return true ;
	 }
	else
	  return error ("%s:\nЛипсва фирма.", tmp) ;
     }
    else
	return false ;
   }
  else
    return false ;
 }  // lawsuitToDamn

bool lawsuitToDamnBoss (TLawsuit * pLawsuit,
				TSideWind * pSide, TCitizen * pCitizen)
  // it is possible to pass NULL instead of pSide or pCitizen
  // (but only one NULL)
  // used to prepare INV Bk
 {
  FUNC_NAME("lawsuitToDamnBoss");
  bool res = false ;

  if (pLawsuit && (pSide || pCitizen))
   {
    if (pLawsuit -> key.type != TYPE_FIRM_LAW)
      return false ;
             // error ("Cannot search a boss for a non-firm lawsuit") ;

    if (DatedContainer_ (pLawsuit -> key, pLawsuit -> kind, pLawsuit -> date).
      FilledOK ())
     {
      char crit [256 * 2] ;
       {
        ostrstream oss (crit, sizeof (crit)) ;

        oss << "SELECT * FROM T_SIDE_WIND WHERE F_NO = " << pLawsuit -> key.no
            << " AND F_YEAR = " << pLawsuit -> key.year
            << " AND F_TYPE = '" << pLawsuit -> key.type << "'"
            << " AND F_KIND = '" << pLawsuit -> kind << "'"
            << " AND F_DATE = " << pLawsuit -> date.Value() << " "
// 20060413   << " AND F_INVOLVEMENT != '" << INVOLVEMENT_FIRM << "' ;"
            << " ;"   // 20060413
            << ends ;
       }
      ENDING_ZERO (crit);
      if (criteria_finished_ok (crit, funcName))
       {
        TCountRecsQuery q (crit) ;
        const unsigned itCnt = q.getRecsCount () ;

        if (itCnt == 0)
          return false ;

	   {
          const unsigned itToRead = random (itCnt) ;

          for (unsigned i = 0 ; i < itToRead ; i ++)
            if (! q.Read ())
              return false ;
         }
        if (q.Read ())
         {
          res = true ;
          if (pSide)
            (* pSide) << q ;
          if (pCitizen)
           {
            (* pCitizen) << q ;
            if (! pCitizen -> Get ())
              res = false ;
           }
         }
       }
     }
    else
      REPORT_F_INVALID_ARGS;
   }
  else
    REPORT_F_INVALID_ARGS;
  return res ;
 }  // lawsuitToDamnBoss

# endif  // <-- AREA and APPEAL

void link_wind_to_link_winds(TLinkWind& link, TGArray<TLinkWind>& links)
{
  T_to_Ts(link, links);
}

void container_to_link_winds(TRCDKeyContainer& container,
  TGArray<TLinkWind>& links)
{
  links.Flush();
  {
    TLinkWind tmpLW ;

    tmpLW.key = container.key;
    tmpLW.kind = container.kind;
    tmpLW.date = container.date;
    // 2007:310 LPR: was just (tmpLW, links) w/o link_wind_to_link_winds
    link_wind_to_link_winds(tmpLW, links);
  }
}

void dated_container_to_link_winds(TDatedContainer& container,
  TGArray<TLinkWind>& links)
{
  // Code moved to container_to_link_winds on 22.dec.2004
  container_to_link_winds(container, links);
}  // dated_container_to_link_winds

bool inreg_to_link_winds(TInReg* pInReg, TGArray<TLinkWind>& links, const bool)
{
  links.Flush();
  if(!pInReg)
    return false;
  if(pInReg->decision.type == '\0')
    return true;
  if(!inreg_to_law_type(*pInReg))
  {
    char tmp[128];

    TRCDKey_(pInReg->key).Print(tmp, sizeof(tmp));
    info("ВНИМАНИЕ! Вие няма да можете да видите %s в "
          "книгите \"Входящ регистър\", "
          "\"Книга на обжалваните дела\" "
          "и \"Изходящ регистър\", тъй като те се печатат "
          "\"За вид дело\".", tmp);
    return true;
  }
  sweep_invalid_link_winds(*pInReg);
  dated_container_to_link_winds(*pInReg, links);
  return true ;
}  // inreg_to_link_winds

TSideWind* dated_container_to_side_wind(TDatedContainer& dContainer)
{
  return Container2SideWind(&dContainer);
}  // dated_container_to_side_wind

void container_to_side_winds(TRCDKeyContainer& dContainer,
  TGArray<TSideWind>& dSides)
{    // 22.dec.2004
  FUNC_NAME("container_to_side_winds");  
  dSides.Flush();

  CharPtr criteria(1024);
  //TSideWind* pSide = dated_container_to_side_wind(dContainer);  // 22.dec.2004
  TSideWind* pSide = Container2SideWind(&dContainer);  // 22.dec.2004

  (*pSide) << dContainer;
  pSide->FloatCriteria(criteria);
  if(criteria_finished_ok(criteria, funcName))
  {  // -- 3 --
    TQuery q(criteria);

    while(q.Read())
    {  // -- 4 --
      //TSideWind* pNewSW = dated_container_to_side_wind(dContainer);
      TSideWind* pNewSW = Container2SideWind(&dContainer);  // 22.dec.2004

      clearGroupData(*pNewSW);
      (*pNewSW) << q;
      dSides.Add(pNewSW);
    }  // -- 4 --
  }  // -- 3 --
  delete pSide;
}  // container_to_side_winds

void dated_container_to_side_winds(TDatedContainer& dContainer,
  TGArray<TSideWind>& dSides)
{
  // code moved to container_to_side_winds on 22.dec.2004
  container_to_side_winds(dContainer, dSides);
}  // dated_container_to_side_winds

char datedContainerToSubpoenaEvStatus(TDatedContainer& dContainer)
{
  FUNC_NAME("datedContainerToSubpoenaEvStatus");
  bool errReported = false;
  const char res = dContainer.Status();
  if(res == '\0' && errReported == false)
    REPORT_F_UNSUCCESSFUL;
  return res;
}  // datedContainerToSubpoenaEvStatus

bool no_year_to_outreg (const long int no, const long int year,
                        TOutReg & rOutReg)
 {
  clearGroupData (rOutReg) ;
  if (! (no && year))
    return false ;
  rOutReg.key.no = no ;
  rOutReg.key.year = year ;
  rOutReg.key.type = TYPE_OUTREG ;  // Because TOutReg.Get() requires it.
  if (rOutReg.Get ())
    return true ;
  else
   {
    clearGroupData (rOutReg) ;
    return false ;
   }
 }  // no_year_to_outreg

void side_to_sides (TSideWind & side, TGArray <TSideWind> & sides)
 {
  T_to_Ts (side, sides) ;
 }  // side_to_sides

char upCyr (char a)
 {
  int ai = (int) ((unsigned char) a) ;
  int bgLowBeg = (int) ((unsigned char) 'а') ;
  int bgLowEnd = (int) ((unsigned char) 'я') ;
  int dist = (int) ((unsigned char) 'А') - bgLowBeg ;

  if (ai >= bgLowBeg && ai <= bgLowEnd)
    ai += dist ;
  return (char) ai ;
 }  // upCyr

char loCyr (char a)
 {
  int ai = (int) ((unsigned char) a) ;
  int bgUpBeg = (int) ((unsigned char) 'А') ;
  int bgUpEnd = (int) ((unsigned char) 'Я') ;
  int dist = (int) (bgUpBeg - (unsigned char) 'а') ;

  if (ai >= bgUpBeg && ai <= bgUpEnd)
    ai -= dist ;
  return (char) ai ;
 }  // loCyr

char * up_lo_cyr_str (char * const str, const bool up)
 {
  if (str)
   {
    const size_t l = strlen (str) ;

    for (size_t i = 0 ; i < l ; i ++)
      str [i] = up ? upCyr (str [i]) : loCyr (str [i]) ;
   }
  return str ;
 }  // up_cyr_str

char * up_cyr_str (char * const str)
 {
  return up_lo_cyr_str (str, true) ;
 }  // up_cyr_str

char * lo_cyr_str (char * const str)
 {
  return up_lo_cyr_str (str, false) ;
 }  // lo_cyr_str

char * mixed_case_cyr_str (char * const str)
 {
  lo_cyr_str (str) ;
  if (str)
   {
    const size_t l = strlen (str) ;
    const char * const blanks = " .-/""\xA0";
    char prev = ' ' ;

    for (size_t i = 0 ; i < l ; i ++)
     {
      char & c = str[i] ;
      const char tmp = c ;

      if (strchr(blanks, prev) != NULL && strchr(blanks, c) == NULL)
       {
        c = upCyr (c) ;
       }
      prev = tmp ;
     }
   }
  return str ;
 }  // mixed_case_cyr_str

char * mixed_case_cyr_str (char * const str, const bool up2Lo)
 {
  if (str)
   {
    const size_t l = strlen (str) ;
    const char * const blanks = " .-/""\xA0";
    char prev = ' ' ;

    for (size_t i = 0 ; i < l ; i ++)
     {
      char & c = str[i] ;
      const char tmp = c ;

      if(strchr(blanks, c) == NULL)
       {
        if (strchr(blanks, prev) != NULL)
         {
          if(up2Lo == false)
           {
            c = upCyr (c) ;
           }
         }
         else
         {
          if(up2Lo == true)
           {
            c = loCyr (c) ;
           }
         }
       }
      prev = tmp ;
     }
   }
  return str ;
 }  // mixed_case_cyr_str

bool str_del_first (char * const str, const char * const toDel)
 {
  if (! (str && str [0] && toDel && toDel [0]))
    return false ;

  const size_t tdl = strlen (toDel) ;

  if (strlen (str) < tdl)
    return false ;

  char * const f = const_cast <char *> (strstr (str, toDel)) ;

  if (f == NULL)
    return false ;
  memmove (f, f + tdl, strlen (f + tdl) + 1) ;
  return true ;
 }  // str_del_first

bool abbrev (const char * const src, char * const dest,
             const size_t destSz)
 {
  if (! (src && dest && destSz))
    return false ;
  dest [0] = '\0' ;
  if (! (src [0] && destSz > 1))
    return false ;
  setmem (dest, destSz, '\0') ;  

  bool res ;
  const size_t sL = strlen (src) ;
   {
    char * tmp = new char [sL + 1] ;
     {
      istrstream iss (const_cast <char *> (src), sL) ;
      ostrstream oss (dest, destSz) ;

      while (iss.tellg () < sL)
       {
        iss >> tmp ;
        if (strlen (tmp))
         {
          tmp [0] = (char) toupper (tmp [0]) ;
          tmp [0] = upCyr (tmp [0]) ;
          oss << tmp [0] ;
         }
       }
      res = oss.tellp () > 0 && oss.tellp () < destSz ;
      oss << ends ;
     }
    dest [destSz - 1] = '\0' ;
    delete [] tmp ;
   }
  return res ;
 }  // abbrev

bool lower_regional_boza_short_name (const TSender & sender,
                                     char * const dest,
                                     const size_t destSz)
 {
  if (! (dest && destSz))
    return false ;

  dest [0] = '\0' ;
  if (!( ( sender.ucnType == UCN_INSTANCE_LOWER
          ||
          sender.ucnType == UCN_EXECUTOR_UCN
          ||
          sender.ucnType == UCN_EXECUTOR_CODE
         ) && (sender.name [0] && destSz > 1)
     ) )
    return false ;

  if ( sender.ucnType == UCN_EXECUTOR_UCN || sender.ucnType == UCN_EXECUTOR_CODE)
  {
    scSprintf (destSz, dest, "%.*s", destSz - 1, "ЧСИ") ;
    return true ;
  }

  char tmp [sizeof (sender.name)] ;

  scSprintf (sizeof (tmp), tmp, "%s", sender.name) ;
  up_cyr_str (tmp) ;

  bool rcRemoved = false ;
   {
    const char * const rcB [] =
     {"РС ", "РС.", "Р.С ", "Р.С.", "РС-"} ;

    for (int i = 0 ;
         i < sizeof (rcB) / sizeof (rcB [0]) && rcRemoved == false ;
         i ++)
     {  // something like "РС ТУТРАКАН"
      const char * const rc = rcB [i] ;

      if (strncmp (tmp, rc, strlen (rc)) == 0)
        rcRemoved = str_del_first (tmp, rc) ;
     }
   }
   {
    const char * const rcE [] =
     {" РС", " Р.С.", " Р.С", " РС."} ;
    for (int i = 0 ;
         i < sizeof (rcE) / sizeof (rcE [0]) && rcRemoved == false ;
         i ++)
     {
      const char * const rc = rcE [i] ;
      const size_t rcl = strlen (rc) ;
      const size_t tl = strlen (tmp) ;

      if (tl >= rcl && strcmp (tmp + tl - rcl, rc) == 0)
       {  // something like "ДЕВНЕНСКИ РС"
        tmp [tl - rcl] = '\0' ;
        rcRemoved = true ;
       }
     }
   }
  if (rcRemoved == false)
    if (str_del_first (tmp, "РАЙОНЕН ") && str_del_first (tmp, "СЪД"))
      rcRemoved = true ;  // something like "ПРОВАДИЙСКИ РАЙОНЕН СЪД"
    else
      scSprintf (sizeof (tmp), tmp, "%s", sender.name) ;

   {  // replace the '-' and '.' symbols with ' '
    const size_t tl = strlen (tmp) ;

    for (size_t i = 0 ; i < tl ; i ++)
      if (tmp [i] == '-')
        if (i == 0 ||  // At the beginning of the string
            i == tl - 1 ||  // At the end of the string
            tmp [i - 1] == ' ' ||  // Something like " -"
            tmp [i + 1] == ' ')  // Something like "- "
          tmp [i] = ' ' ;
           // Anything like "АС-Д" (Асеновград) remains unchanged.

    for (size_t i = 0 ; i < tl ; i ++)
      if (tmp [i] == '.')
        tmp [i] = ' ' ;
   }
  strDelLastSpaces (tmp) ;
  strDelLeadingSpaces (tmp) ;
  if (! (tmp [0]))
   {
    rcRemoved = false ;
    scSprintf (sizeof (tmp), tmp, "%s", sender.name) ;
   }
  if (! (tmp [0]))
    return false ;
  if (rcRemoved && strchr (tmp, ' ') == NULL)
   {  // there is only one word left
    const size_t tmpL = strlen (tmp) ;
    const char * const rc = "РС" ;
    const size_t rcL = strlen (rc) ;

    if (tmpL > rcL &&
        strcmp (tmp + tmpL - rcL, rc)  // something different from
                                       // "ШРС", "НЗРС"
       )
     {
      lo_cyr_str (tmp + 1) ;

      const size_t smallerSize =
        sizeof (tmp) < destSz ? sizeof (tmp) : destSz ;

      if (smallerSize > 5 && tmpL > smallerSize - 1)
       {  // Try to truncate
        if (strchr (BULGARIAN_VOWELS, tmp [smallerSize - 2]))
         {
          tmp [smallerSize - 2] = '.' ; // !!! The '.' symbol will
        // be last in the dest string, when the function returns.
          for (size_t i = smallerSize - 1 ; i < tmpL ; i ++)
            tmp [i] = ' ' ;
         }
       }
     }
   }
  else
   {
    // I think, it is not fatal if rcRemoved == true, because
    // there are two or more words left --> there will be
    // enough letters in the abbreviature.
    const size_t tSz1 = strlen (tmp) + 1 ;
    char * t1 = new char [tSz1] ;

    abbrev (tmp, t1, tSz1) ;
    scSprintf (sizeof (tmp), tmp, "%s", t1) ;
    delete [] t1 ;
   }

  const bool res = tmp [0] && strlen (tmp) < destSz ;

  scSprintf (destSz, dest, "%.*s", destSz - 1, tmp) ;
  return res ;
 }  // lower_regional_boza_short_name


bool request_to_sender(const TRequest& rRequest, TSender& s)  // 21.dec.2001
{
  // By the idea of requestColumns::makeColumns --> iC_Sender
  bool res = false;
  if(TRCDKey_(rRequest.key).FilledOK())
  {
    // 20070103 -->
    TRequest * pRequest = new TRequest ;
    pRequest -> key = rRequest.key;
    TConnect *c = main_connect( pRequest );

    if ( c != NULL )
    {
      s.ucnType = c -> ucnType;
      if ( s.ucnType )
      {   scSprintf (sizeof (s.ucn), s.ucn, "%s",  c -> ucn) ;
          res = true;
      }
    }

    delete c;
    delete pRequest;

    if ( res ) res = s.Try () ;
    // 20070103 <--
  }
  return res;
}  // request_to_sender

bool kinds_to_types (const char * const kinds, char * const types,
                     const size_t tSz)
 {
  if (! (types && tSz))
    return false ;
  types [0] = '\0' ;
  if (! (kinds [0]))
    return false ;

  size_t typesFound = 0 ;
  const size_t kL = strlen (kinds) ;

  for (int i = 0 ; i < kL ; i++)
   {
    const char t = kind2type (kinds [i]) ;

    if (! strchr (types, t))
     {
      typesFound ++ ;
      if (typesFound < tSz)
       {
        char t2 [2] ;

        t2 [0] = t ;
        t2 [1] = '\0' ;
        strConcat (types, t2, tSz) ;
       }
     }
   }
  return (typesFound && typesFound < tSz) ;
 }  // kinds_to_types

bool user_ucn_at_unique_post (char * const dest, const size_t destSz,
					const char post)
 {
  if (! (post && dest && destSz))
    return false ;
  dest [0] = '\0' ;

  TCitizen cit ;

  if (destSz < sizeof (cit.ucn))
    return false ;

  bool res = false ;

   {
    char crit [128] ;

    ostrstream (crit, sizeof (crit))
      << "SELECT F_UCN FROM T_USER WHERE F_POST = \'"
      << POST_CHIEF << "\' "
      << ";" << ends ;
    crit [sizeof (crit) - 1] = '\0' ;
    if (criteria_finished_ok (crit, "user_ucn_at_unique_post"))
     {
      int courtPresidents = 0 ;
      TQuery q (crit) ;

      while (courtPresidents < 2 && q.Read ())
       {
        if (courtPresidents == 0)
          cit << q ;
        courtPresidents ++ ;
       }
      if (courtPresidents == 1)
       {
        scSprintf (destSz, dest, "%s", cit.ucn) ;
        res = true ;
       }
      else
       {
        TPost p ;

        p.post = post ;
        if (! p.Get ())
         {
          clearGroupData (p) ;
          scSprintf (sizeof (p.name), p.name, "\'%c\'", p.post) ;
         }
        info ("В системата %s потребители на длъжност %s",
              (courtPresidents ? "има повече от един" : "няма"),
              p.name) ;
       }
     }
   }
  return res ;
 }  // user_ucn_at_unique_post

// 2007:269 LPR: brand-new law_types_to_law_kinds()
struct TType2LawKinds
{
	char type;
	const char *lawKinds;
};

static const TType2LawKinds Type2LawKinds[] =
{
#if COURT_TYPE == COURT_APPEAL
	{ TYPE_PUNISHMENT_LAW,	KIND_PUNISHMENT_LAWS },
	{ TYPE_CITIZEN_LAW,	KIND_CITIZEN_LAWS },
	{ TYPE_RECITIZEN_LAW,	KIND_RECITIZEN_LAWS },
	{ TYPE_TRADE_LAW,		KIND_TRADE_LAWS },
	{ TYPE_RETRADE_LAW,	KIND_RETRADE_LAWS },
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	{ TYPE_ADMIN_LAW,		KIND_ADMIN_LAWS },
	{ TYPE_TRICKY_LAW,	KIND_TRICKY_LAWS },
	{ TYPE_CITIZEN_LAW,	KIND_CITIZEN_LAWS },
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	{ TYPE_PUNISHMENT_LAW,	KIND_PUNISHMENT_LAWS },
	{ TYPE_CITIZEN_LAW,	KIND_CITIZEN_LAWS },
	{ TYPE_TRADE_LAW,		KIND_TRADE_LAWS },
	{ TYPE_ADMIN_LAW,		KIND_ADMIN_LAWS },
	{ TYPE_FIRM_LAW,		KIND_FIRM_LAWS },
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	{ TYPE_PUNISHMENT_LAW,	KIND_PUNISHMENT_LAWS },
	{ TYPE_CITIZEN_LAW,	KIND_CITIZEN_LAWS },
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ TYPE_MARRIAGE_LAW,	KIND_MARRIAGE_LAWS },
	{ TYPE_CITIZEN_LAW,	KIND_CITIZEN_LAWS },
	{ TYPE_PUNISHMENT_LAW,	KIND_PUNISHMENT_LAWS },
	{ TYPE_ADMIN_LAW,		KIND_ADMIN_LAWS },
#endif  // REGIONAL
	{ '\0', NULL }
};

bool law_types_to_law_kinds (const char * const types,
			     char * const kinds, const size_t kindsSz)
{
	// 2007:269 LPR: brand-new law_types_to_law_kinds()
	const TType2LawKinds *t2lk;

	kinds[0] = kinds[kindsSz - 1] = '\0';
	for (t2lk = Type2LawKinds; t2lk->type; t2lk++)
		if (strchr(types, t2lk->type))
			strcat(kinds, t2lk->lawKinds);

	if (kinds[kindsSz - 1] != '\0')
		fatal("law_types_to_law_kinds overflow");

	return true;
} // law_types_to_law_kinds

bool inreg_to_any_appealer (const TInReg & inReg, TSideWind & dest)
 {
  bool res = false ;

  if (DatedContainer_ (inReg).FilledOK ())
   {
    TSideWind waln ;

    waln.key = inReg.key ;
    waln.date = inReg.date ;
    waln.kind = inReg.kind ;
     {
      CharPtr crt (1024) ;

      waln.FloatCriteria (crt) ;
       {
        TQuery q (crt) ;
        TSideWind tmp ;

        while (res == false && q.Read ())
         {
          tmp << q ;
          if (tmp.state == STATE_APPEALER ||
		  tmp.state == STATE_INDIRECT ||
		  tmp.involvement == INVOLVEMENT_SENDER)
	     {
            dest << tmp ;
            res = true ;
           }
         }
       }
     }
   }
  return res ;
 }  // inreg_to_any_appealer

static char inreg_to_law_type_recoursive (TInReg & inReg,
                                          TRCDKey * const pLawKey,
							TDatedContainer * pSessKeyData,
                                          TRCDKeyContainer_ * pSurrKeyData,
                                          CharPtr & testedInRegDescription,
                                          const int level,
                                          TRCDKey* pReqKey  // 11.jun.2004
                                         )
// Returns the asotiated lawsuit type. On error returns '\0'.
// An old varianth - in VersionArchives\1375\_appname.j --> _convert.cpp
 {
  char lawType = '\0' ;
  const char * const apl = "обжалва" ;
  const char * const dcr = "разпореждане по Входящ регистър" ;
  const char * const cannotProcess =
    "Ситуацията не може да бъде обработена" ;
  const int OUTRAGE_UPON_DECENCY = 4 ;

  if (inReg.decision.type)
   {
    if (strchr (TYPE_LAWSUITS, inReg.decision.type))
     {
      lawType = inReg.decision.type ;
      if (pLawKey)
        * pLawKey = inReg.lawsuit ;
      if (pSessKeyData)
       {
        pSessKeyData -> key = inReg.lawsuit ;
        pSessKeyData -> date = inReg.decisionDate ;
        pSessKeyData -> kind = inReg.sessionKind ;
       }
      if (pSurrKeyData)
	 {
        pSurrKeyData -> type = TYPE_SURROUND ;
        pSurrKeyData -> no = inReg.surroundNo ;
        pSurrKeyData -> year = inReg.surroundYear ;
        pSurrKeyData -> kind = inReg.surroundKind ;
       }
     }
    else
      if (strchr (TYPE_INREGS, inReg.decision.type))
       {
        if (level >= OUTRAGE_UPON_DECENCY)
         {  // Ooo ?! - what's this ?!
          error ("%s:\n"                     // 0
                 "%d нива на %sне на %s.\n"  // 1, 2, 3
                 "%s.",                      // 4
                 testedInRegDescription.Str(),  // 0
                 level, apl, dcr,               // 1, 2, 3
                 cannotProcess                  // 4
                ) ;
         }
        else
         {
          bool anotherTwist = false;
           {
            TInReg tmp ;

            tmp.key = inReg.decision ;
            if (TRCDKey_ (tmp.key).FilledOK () && tmp.Get ())
             {
              inReg << tmp ;
              anotherTwist = true ;
             }
           }
          if (anotherTwist)
            lawType = inreg_to_law_type_recoursive (inReg, pLawKey,
                        pSessKeyData, pSurrKeyData,
                        testedInRegDescription, level + 1, pReqKey) ;
         }
       }
      else
        if (strchr (TYPE_REQUESTS, inReg.decision.type))
         {
          if  (level >= OUTRAGE_UPON_DECENCY)
           {  // Ooo ?!- - what's this ?!
            const char * const req = "преписка" ;

            error ("%s:\n"                     // 0
                   "%d нива на %sне на %s.\n"  // 1, 2, 3
                   "%s.",                      // 4
                   testedInRegDescription.Str(),  // 0
                   level, apl, req,               // 1, 2, 3
                   cannotProcess                  // 4
                  ) ;
           }
          else
	     {
		TRequest r ;

		r.key = inReg.lawsuit ;

		if (TRCDKey_(r.key).FilledOK() && r.Get("F_LAW_KIND"))
		{
			lawType = kind2type(r.lawKind);

			if (pReqKey)
				*pReqKey = r.key;
		}
	     }
         }
   }
  else
    lawType = inReg.college ;
  return lawType ;
 }  // inreg_to_law_type_recoursive

static  // 11.jun.2004
char inreg_to_law_type_imp (TInReg & inReg, TRCDKey * const pLawKey,
			    TDatedContainer * pSessKeyData,
			    TRCDKeyContainer_ * pSurrKeyData,
			    const bool errMsg,
			    TRCDKey * pReqKey  // 11.jun.2004
			   )
// Returns the asotiated lawsuit type. On error returns '\0'.
// If there is an associated lawsuit, the (* pLawKey) is filled.
// If there is an associated session, the (* pSessKeyData) is filled.
// If there is an associated surround, the (* pSurrKeyData) is filled.
// If there is an associated request, the (* pReqKey) is filled.
 {  // 30.jul.2002
  if (pLawKey)
    pLawKey -> year = pLawKey -> no = pLawKey -> type = '\0' ;
  if (pSessKeyData)
    clearGroupData (* pSessKeyData) ;
  if (pSurrKeyData)
    pSurrKeyData -> Clear () ;

  char lawType;

  // 2008:172 LPR: shortcut, recursive can't be required for lawsuit/request
  if (!pSessKeyData && !pSurrKeyData)
    lawType = inReg2lawType(&inReg, pLawKey, pReqKey);
  else
  {
    CharPtr testedInRegDescription (128) ;

    TRCDKeyContainer_ (inReg.key, inReg.kind).
      Print (testedInRegDescription, testedInRegDescription.Size ()) ;

    lawType =
      inreg_to_law_type_recoursive (inReg, pLawKey, pSessKeyData,
	pSurrKeyData, testedInRegDescription, 0, pReqKey) ;

    if (lawType)
      if (! strchr (TYPE_LAWSUITS, lawType))
	lawType = '\0' ;
  }

  if (lawType == '\0')
    if (errMsg)
     {
      char tmp [128] ;

      TRCDKeyContainer_ (inReg.key, inReg.kind).Print (tmp, sizeof (tmp)) ;
      error ("Не мога да определя за какъв вид дело се отнася %s.", tmp) ;
     }
  return lawType ;
 }  // inreg_to_law_type_imp

char inreg_to_law_type(const TInReg & inReg, TRCDKey * const pLawKey,
  TDatedContainer * pSessKeyData, TRCDKeyContainer_ * pSurrKeyData,
  const bool errMsg,
  TRCDKey* pReqKey  // 11.jun.2004
  )
// An old varianth - in VersionArchives\1375\_appname.j --> _convert.cpp
{  // 30.jul.2002
  TInReg tmp;

  tmp << const_cast<TInReg&>(inReg);
  return inreg_to_law_type_imp(tmp, pLawKey, pSessKeyData, pSurrKeyData,
      errMsg, pReqKey);
}  // inreg_to_law_type

char inreg_to_law_type(TInReg & inReg, TRCDKey * const pLawKey,
  TDatedContainer * pSessKeyData, TRCDKeyContainer_ * pSurrKeyData,
  const bool errMsg,
  TRCDKey* pReqKey  // 11.jun.2004
  )
{  // 30.jul.2002
  TInReg tmp;

  tmp << inReg;
  return inreg_to_law_type_imp(tmp, pLawKey, pSessKeyData, pSurrKeyData,
      errMsg, pReqKey);
}  // inreg_to_law_type

bool surroundment_to_surround (const TSurroundment & ment,
                               TSurround & surround)
 {
  surround.key = ment.key ;
  surround.kind = ment.kind ;

  const bool res = surround.Get () ;

  if (res == false)
    clearGroupData (surround) ;
  return res ;
 }  // surroundment_to_surround

bool surroundment_to_session (const TSurroundment & ment,
					 TSession & sess)
 {
  sess.key = ment.lawsuit ;
  sess.kind = ment.sessionKind ;
  sess.date = ment.sessionDate ;

  const bool res = sess.Get () ;

  if (res == false)
    clearGroupData (sess) ;
  return res ;
 }  // surroundment_to_session

bool session_to_surrounds (TSession & sess,
				   TGArray<TSurround> & surrounds)
 {
  TGArray <TSurroundment> surroundments ;
  bool rT = try2surround (& sess, & surroundments) ;

  surrounds.Flush () ;
  if (rT)
   {
    for (int i = 0 ; rT && i < surroundments.Count () ; i ++)
     {
	TSurroundment & lnk = * (surroundments [i]) ;
	TSurround tmp ;

	if (valid_surroundment (lnk, & tmp, NULL, i != 0))
	 {
	  TSurround * pNewSurr = new TSurround ;

	  (* pNewSurr) << tmp ;
        surrounds.Add (pNewSurr) ;
       }
      else
        if (ask (WANT_CONTINUE_QUESTION) == false)
          rT = false ;
     }
    if (rT)
	if (surrounds.Count () <= 0)
        rT = false ;
   }
  return rT ;
 }  // session_to_surrounds

bool surroundment_key_data (TSurroundment & ment,
				    const TRCDKey surroundKey,
				    const char surroundKind,
				    const TRCDKey sessionKey,
				    const CDate sessionDate,
				    const char sessionKind)
 {
  bool res = false ;

  clearGroupData (ment) ;
  if (TRCDKeyContainer_ (surroundKey, surroundKind).FilledOK ())
    if (DatedContainer_ (sessionKey.type, sessionKey.no, sessionKey.year,
				 sessionKind, sessionDate).FilledOK ())
     {
	ment.key = surroundKey ;
	ment.kind = surroundKind ;
	ment.lawsuit = sessionKey ;
	ment.sessionKind = sessionKind ;
	ment.sessionDate = sessionDate ;
      res = true ;
     }
  return res ;
 }  // surroundment_key_data


bool surround_kind_to_waln_state(const char surroundKind, char &walnState)
{
#if COURT_TYPE == COURT_AREAL
	if (surroundKind == KIND_CHANGE_SURROUND)
		walnState = STATE_CHANGE;
	else  // falldown
#endif  // AREAL
	if (!surroundKind || !strchr(KIND_SURROUNDS, surroundKind))
		walnState = error(SURROUND_KIND_OMITTED);
	else
		walnState = STATE_SURROUND;

	return walnState;
}

char request_doc_to_law_type (const TRequest &request)
{
	char lawType = '\0' ;
	if (request.lawKind)
		lawType = kind2type(request.lawKind);
	return lawType ;
}

// 11.jan.2002 - begin
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
// These "# if" conditional directives are included to avoid
// linker errors (possible because the function is not used
// int the regional and appeal versions).

void inregs_to_all_outregs (LawsuitAllInRegs & inregs,
                            TGSArray <OutRegDatedContainer> & outregs)
 {
  TInReg inReg ;

  outregs.Flush () ;
  while (inregs.NextKey (inReg))
   {  // -- 0 --
    InRegAllEvents events (inReg.key) ;
    InRegEventPtr pIRE ;
    TInRegEvent & iRE = pIRE.Ref () ;

    while (events.NextWholeRecord (iRE))
     {  // -- 1 --
      if (iRE.result == RESULT_IOREG_SEND)
       {
        if (iRE.outNo && iRE.outYear)
         {
          TOutReg outReg ;

          if (no_year_to_outreg (iRE.outNo, iRE.outYear, outReg))
           {
            OutRegDatedContainer tmp ;

            tmp << outReg ;
		if (outregs.Contains (& tmp) == false)
             {
              OutRegDatedContainer * pNewORDC = new OutRegDatedContainer ;

              (* pNewORDC) << tmp ;
              outregs.Add (pNewORDC) ;
             }
           }
         }
       }
     }  // -- 1 --
   }  // -- 0 --
 }  // inregs_to_all_outregs

#   endif  // of APPEAL
# endif  // of INSTANCE
// 11.jan.2002 - end

bool surround_to_surroundments(TSurround& surr,
		TGArray<TSurroundment>& ments)
{
	ments.Flush();  // Look at the return statement if you decide to
	                // (re)move this from here!
	if(TRCDKeyContainer_(surr.key, surr.kind).FilledOK())
	{
		CharPtr mentCrt(1024);
		TSurroundment ment;

		ment << surr;
		ment.FloatCriteria(mentCrt);
		{
			TQuery q(mentCrt);

			while(q.Read())
			{
				TSurroundment* pNewMent = new TSurroundment;

				(*pNewMent) << q;
				ments.Add(pNewMent);
			}
		}
	}
	else
		error("%s\nsurround_to_surroundments", WRONG_ARGS_PASSED_TO);
	return (ments.Count() > 0);
}  // surround_to_surroundments

bool surround_to_brief_sessions(TSurround& surr,
		TGSArray<BriefSessionData>& briefSessions)
{
	briefSessions.Flush();  // Look at the return statement if you decide
	                        // to (re)move this from here!
	if(TRCDKeyContainer_(surr.key, surr.kind).FilledOK())
	{
		TGArray<TSurroundment> ments ;

		if(surround_to_surroundments(surr, ments))
		{
			SessPtr pSess;

			for(int i = 0; i < ments.Count(); i++)
			{
				const TSurroundment& ment = *(ments[i]);

				pSess.Ref().key = ment.lawsuit;
				pSess.Ref().kind = ment.sessionKind;
				pSess.Ref().date = ment.sessionDate;
				if(pSess.Ref().Get())
				{
					BriefSessionData* pNewBSD = new BriefSessionData;

					(*pNewBSD) << pSess.Ref();
					briefSessions.Add(pNewBSD);
				}
			}
		}
	}
	else
		error("%s\nsurround_to_surroundments", WRONG_ARGS_PASSED_TO);
	return (briefSessions.Count() > 0);
}  // surround_to_brief_sessions

bool trunc_BG_string(const char* const src, char* const dest,
		const size_t destSz, const size_t maxLen,
		const size_t dotAlowedFromPosition)
{  // 14.aug.2002
	bool res = false;

	if(dest && destSz)
	{
		dest[0] = '\0';
		if(src && maxLen)
		{
			const size_t sL = strlen(src);
			ostrstream ossDest(dest, destSz);

			for(int i = 0; i < sL && i < maxLen; i++)
				if(dotAlowedFromPosition < maxLen && i == maxLen - 1 &&
          i + 1 != sL && strchr(BULGARIAN_VOWELS, src[i]))
					// Change only the last resulting symbol if it's
					// a Bulgarian vowel
					// and maxLen is more than dotAlowedFromPosition
					ossDest << '.';
				else
					ossDest << src[i];
			ossDest << ends;
		}
		dest[destSz - 1] = '\0';
	}
	return res;
}  // trunc_BG_string

char outreg_to_law_type(TOutReg& outReg, TRCDKey* const pLawKey,
  TDatedContainer* pSessKeyData, TRCDKeyContainer_* pSurrKeyData,
  TRCDKey* pReqKey, const bool errMsg)
// Returns the associated lawsuit type. On error returns '\0'.
// If there is an associated lawsuit, the (*pLawKey) is filled.
// If there is an associated session, the (*pSessKeyData) is filled.
// If there is an associated surround, the (*pSurrKeyData) is filled.
// If there is an associated request, the (*pReqKey) is filled.
{
  const char* const funcName = "outreg_to_law_type";
  char res = '\0';

  if(pLawKey)
    pLawKey->year = pLawKey->no = pLawKey->type = '\0';
  if(pSessKeyData)
    clearGroupData(*pSessKeyData);
  if(pSurrKeyData)
    pSurrKeyData->Clear();
  if(pReqKey)
    pReqKey->year = pReqKey->no = pReqKey->type = '\0';
  if(TRCDKey_(outReg.key).FilledOK())
  {  // -- 0 --
    // Pure outregs (with no relations)
    if(outReg.lawsuit.year == 0 && outReg.inYear == 0)
    {
      if(outReg.lawsuit.type)
      {
        if(strchr(TYPE_LAWSUITS, outReg.lawsuit.type))
          res = outReg.lawsuit.type;
      }
      else
        if(outReg.college)
        {
	  if(strchr(TYPE_LAWSUITS, outReg.college))
	    res = outReg.college;
        }
    }
    else
      // Inreg related outregs
      if(outReg.inNo != 0 && outReg.inYear != 0)
      {
        TInReg inReg;

        inReg.key.type = TYPE_INREG;
        inReg.key.year = outReg.inYear;
        inReg.key.no = outReg.inNo;
        if(inReg.Get())
	  res = inreg_to_law_type(inReg, pLawKey, pSessKeyData, pSurrKeyData,
	    errMsg);
      }
      else
        if(TRCDKey_(outReg.lawsuit).FilledOK())
        {
          // Request related outregs
          if(outReg.lawsuit.type == TYPE_REQUEST)
          {
            TRequest req;

            req.key = outReg.lawsuit;
            if(req.Get())
            {
		  res = request_doc_to_law_type(req);
	      if(res != '\0' && pReqKey)
                *pReqKey = req.key;
            }
          }
          else
            // Just lawsuit sent out
            if(strchr(TYPE_LAWSUITS, outReg.lawsuit.type))
            {
              res = outReg.lawsuit.type;
              if(pLawKey && TRCDKey_(outReg.lawsuit).FilledOK())
                *pLawKey = outReg.lawsuit;
            }
        }
    if(res == '\0' && errMsg == true)
    {
      char tmp[128];

      TRCDKey_(outReg.key).Print(tmp, sizeof(tmp));
      error("Не може да бъде определен видът дело, за който се отнася\n%s",
	tmp);
    }
  }  // -- 0 --
  else
    error("%s\n%s", WRONG_ARGS_PASSED_TO, funcName);
  return res;
}  // outreg_to_law_type
