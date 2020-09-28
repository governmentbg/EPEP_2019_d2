#include "cac.h"

bool extractDecision (const TRCDKey & key,
                      const char sessKind,  // 15.jan.2001
                      const CDate & date,
                      const char kind, TDecision & decision)
 {
  if (! (TRCDKey_ (key).FilledOK () && sessKind &&
         date.Empty () == false && kind))
    return false ;
  bool res ;

   {
    TDecision * pD1 = new TDecision ;

    pD1 -> key = key ;
    pD1 -> date = date ;
    pD1 -> sessionKind = sessKind ;
    pD1 -> kind = kind ;
    if ((res = pD1 -> Try ()) == true)
      decision << (* pD1) ;
    delete pD1 ;
   }
  return res ;
 }  // extractDecision

bool extractDecision (const TRCDKey & key,
                      const char sessKind,  // 15.jan.2001
                      const CDate & date,
                      TDecision & decision, const char kind)
 {
  if (! (TRCDKey_ (key).FilledOK () && date.Empty () == false))
    return false ;
  if (sessKind && kind)
    return extractDecision (key, sessKind, date, kind, decision) ;
  else
   {  // -- 0 --
    bool res = false ;
    char criteria [256 + 32] ;

     {  // -- 1 --
      ostrstream oss (criteria, sizeof (criteria)) ;

      oss   << "SELECT * FROM T_DECISION "
               "WHERE F_NO = " << key.no
            << " AND F_YEAR = " << key.year
            << " AND F_TYPE = \'" << key.type << "\'"
            << " AND F_DATE = " << date.Value() ;  // 15.apr.2003
	if (sessKind)  // 15.jan.2001 - begin
        oss << " AND F_SESSION_KIND = \'" << sessKind << "\'" ;
      if (kind)
        oss << " AND F_KIND = \'" << kind << "\'" ;  // 15.jan.2001 - end
      oss << " ;" << ends ;
     }  // -- 1 --
    criteria [sizeof (criteria) - 1] = '\0' ;
     {  // -- 2 --
	TCountRecsQuery q (criteria) ;
      unsigned int cnt = 0 ;

      while (q.Read ())
        cnt ++ ;
      q.Restart () ;
      if (cnt == 1)
       {
        if (q.Read ())
         {
          decision << q ;
          res = true ;
         }
       }
      else
        if (cnt > 1)
         {  // Last modified: 15.jan.2001
          char lawString [64] ;
          char dateStr [32] ;
          TKind k ;
          TKind sK ;

          ostrstream (lawString, sizeof (lawString))
            << TRCDKey_ (key) << ends ;
          lawString [sizeof (lawString) - 1] = '\0' ;
          dateStr [0] = dateStr [sizeof (dateStr) - 1] = '\0' ;
          ostrstream (dateStr, sizeof (dateStr) - 1) << date << ends ;

          k.kind = kind ;
          if (! (k.kind && k.Get ()))
            clearGroupData (k) ;
          sK.kind = sessKind ;
          if (! (sK.kind && sK.Get ()))
            clearGroupData (sK) ;

          error ("Повече от един краен документ при следните данни:\n"
                 "%s\n"
                 "%s от %s\n"
                 "%s %s\n",
                 lawString,
                 sK.name [0] ? sK.name : "Заседание", dateStr,
                 k.name [0] ? "Краен документ:" : "", k.name) ;
         }
     }  // -- 2 --
    return res ;
   }  // -- 0 --
 }  // extractDecision
 
bool extract_first_decision (const TRCDKeyGroup & keyGroup,
                             TDecision & dec)  // 16.jun.2000
 {
  bool res = false ;

  if (keyGroup.key.type)
   {
    char crt [128
                  + 128  // 24.jun.2000
                       ] ;

    ostrstream (crt, sizeof (crt))
      << "SELECT F_NO, F_YEAR, F_TYPE, F_DATE, F_KIND FROM T_DECISION "
      << "WHERE F_NO = " << keyGroup.key.no << " AND "
      <<       "F_YEAR = " << keyGroup.key.year << " AND "
      <<       "F_TYPE = \'" << keyGroup.key.type << "\' "
      << "ORDER BY F_DATE ;" << ends ;
    crt [sizeof (crt) - 1] = '\0' ;
    if (
        crt [0] &&  // 24.jun.2000
                   crt [strlen (crt) - 1]
                                          == ';'  // !!!  // 24.jun.2000 
       )
     {
      TQuery q (crt) ;

      if (q.Read ())
       {
        dec << q ;
        res = dec.Get () ;
       }
     }
   }
  if (! res)
    clearGroupData (dec) ;
  return res ;
 }  // extract_first_decision

bool extractLocalJudgement (const TRCDKey & key, char kind,
                            const CDate & date,
                            char ucnType, const char * ucn,
                            char involvement,  // 20060413
                            TJudgement & localJudgement)
 {
  // 21.mar.2001 - begin
  if (! (TRCDKey_ (key).FilledOK () && kind && date.Empty () == false &&
         ucnType && ucn && ucn [0]))
    return false ;  // 21.mar.2001 - begin
         
  bool res ;
  JudgPtr pJudg ;  // 13.jan.2004
  TJudgement& lJ1 = pJudg.Ref () ;

  lJ1.key = key ;
  lJ1.kind = kind ;
  lJ1.date = date ;
  lJ1.ucnType = ucnType ;
  ostrstream (lJ1.ucn, sizeof (lJ1.ucn)) << ucn << ends ;
  lJ1.ucn [sizeof (lJ1.ucn) - 1] = '\0' ;
// 20060413 in comm  lJ1.involvement = INVOLVEMENT_JUDGEMENT ;  // 13.jan.2003
  lJ1.involvement = involvement; // 20060413
  if ((res = lJ1.Try ()) == true)
    localJudgement << lJ1 ;
  return res ;
 }  // extractLocalJudgement

bool extractLocalJudgement (const TRCDKey & key, const CDate & date,
                            const char ucnType, const char * ucn,
                            char involvement,  // 20060413
                            TJudgement & localJudgement,
                            const char kind)
 {
  if (kind)
    return extractLocalJudgement (key, kind, date, ucnType, ucn,
                                  involvement,  // 20060413
                                  localJudgement) ;
  else
   {
    bool res = false ;
    char criteria [256 + 64] ;

    ostrstream oss (criteria, sizeof (criteria)) ;

    oss
      << "SELECT * FROM T_JUDGEMENT "
         "WHERE F_NO = " << key.no
	<< " AND F_YEAR = " << key.year
	<< " AND F_TYPE = '" << key.type << "'" ;
    if (! date.Empty ())
	oss <<
	   " AND F_DATE = " << date.Value() ;
    oss <<
	   " AND F_UCN = '" << ucn << "'"
	   " AND F_UCN_TYPE = '" << ucnType << "';" << ends ;
    criteria [sizeof (criteria) - 1] = '\0' ;
     {
      TQuery q (criteria) ;
      unsigned int cnt = 0 ;

      while (q.Read ())
        cnt ++ ;
      q.Restart () ;
      if (cnt == 1)
       {
        if (q.Read ())
         {
          localJudgement << q ;
          res = true ;
         }
       }
      else
        if (cnt > 1)
         {
          const size_t msgSz = 512 ;
          char * msg = new char [msgSz] ;
          char lT [64] ;
          TUCNType uT ;
          ostrstream ossM (msg, msgSz) ;

          uT.ucnType = ucnType ;
          if (! uT.Get ())
            clearGroupData (uT) ;
          typeToString (key.type, lT, sizeof (lT)) ;
          ossM << MORE_THAN_ONE_L_JUDGEMENT << ":\n"
               << lT << " No " << key.no << "/" << key.year << "г." ;
          if (! date.Empty ())
            ossM
               << " (заседание от " << date << ")" ;
          ossM << ", " << uT.name << " " << ucn << ".\n"
               << "Моля отстранете противоречието, за да получите "
                  "коректен изход." << ends ;
          msg [msgSz - 1] = '\0' ;
          error(msg) ;
          delete [] msg ;
         }
     }
    return res ;
   }
 }  // extractLocalJudgement

bool extractSession (const TRCDKey & key, const CDate & date,
                     TSession & session, const char kind)
 {
  if (kind)
   {
    bool res ;
    TSession s ;

    s.key = key ;
    s.date = date ;
    s.kind = kind ;
    if ((res = s.Get ()) == true)
      session << s ;
    return res ;  
   }
  else
   {
    bool res = false ;
    char criteria [256] ;

    ostrstream (criteria, sizeof (criteria))
      << "SELECT * FROM T_SESSION "
         "WHERE F_NO = " << key.no
      << " AND F_YEAR = " << key.year
      << " AND F_TYPE = \'" << key.type << "\'"
         " AND F_DATE = " << date.Value() << " ;" << ends ;  // 15.apr.2003
    criteria [sizeof (criteria) - 1] = '\0' ;
     {
      TCountRecsQuery q (criteria) ;
      unsigned int cnt = 0 ;

      while (q.Read ())
        cnt ++ ;
      q.Restart () ;
      if (cnt == 1)
       {
        if (q.Read ())
         {
          session << q ;
          res = true ;
         }
       }
      else
        if (cnt > 1)
	    error("Повече от едно заседание по дело за ден") ;
     }
    return res ;
   }
 }  // extractSession

