#include "cac.h"  // 18.may.2001
//# include "all.h"

static
void type_to_name_or_alias(char tC, char* dest, size_t destSz, const bool alias)
{  // 26.jun.2003
  if(!(dest && destSz))
    return;
  if(destSz > 1)
  {
    TType& t = *Type;

    t.type = tC;
    if(t.type && t.Try())
      ostrstream (dest, destSz)
        << ((alias && t.alias[0]) ? t.alias : t.name) << ends;
    else
      dest[0] = '\0';
  }
  if(destSz)
    dest[destSz - 1] = '\0';
}  // type_to_name_or_alias

void typeToString (char tC, char * dest, size_t destSz)
{  // An old variant in VersionArchives\1680\_appname.j --> _tostrng.cpp
  type_to_name_or_alias(tC, dest, destSz, false);
}  // typeToString

void type_to_alias (char tC, char * dest, size_t destSz)
{  // An old variant in VersionArchives\1680\_appname.j --> _tostrng.cpp
  type_to_name_or_alias(tC, dest, destSz, true);
}  // type_to_alias

static
void kind_to_name_or_alias(char kC, char* dest, size_t destSz, const bool alias)
{  // 26.jun.2003
  if(!(dest && destSz))
    return;
  if(destSz > 1)
  {
    TKind& k = *Kind;

    k.kind = kC;
    if(k.kind && k.Try())
	ostrstream(dest, destSz)
	  << ((alias && k.alias[0]) ? k.alias : k.name) << ends;
    else
	dest[0] = '\0';
  }
  if(destSz)
    dest[destSz - 1] = '\0';
}  // kind_to_name_or_alias

void kindToString (char kC, char * dest, size_t destSz)
 {  // An old variant in VersionArchives\1680\_appname.j --> _tostrng.cpp
  kind_to_name_or_alias(kC, dest, destSz, false);
 }  // kindToString

void connectKindToString(char kC, char * dest, size_t destSz)
{
	strzcpy(dest, kC ? ConnectKind->Seek(kC) : "", destSz);
}

void kind_to_alias (char kC, char * dest, size_t destSz)
{  // 26.jun.2003
  kind_to_name_or_alias(kC, dest, destSz, true);
}  // kind_to_alias

void kindWithArticleToString (char kC, char * dest, size_t destSz,
                              const bool full)  // 02.feb.2001
 {
  if (! (kC && dest && destSz))  // 03.may.2001
    return ;  // 03.may.2001
  kindToString (kC, dest, destSz) ;
  if (! (dest [0]))  // 18.may.2001
    return ;  // 18.may.2001
  add_article (dest, destSz, full) ;  // 06.dec.2001 instead of inline text
 }  // kindWithArticleToString

void kindToStringAbbr (char kC, char * dest, size_t destSz)
 {  // An old variant in VersionArchives\1232\_appname.j --> _tostrng.cpp
  if (! (dest && destSz))
    return ;
  dest [0] = '\0' ;
  if (destSz > 1)
   {
    TKind & k = * Kind ;

    k.kind = kC ;
    if (k.kind && k.Try ())
      abbrev (k.name, dest, destSz) ;
   }
 }  // kindToStringAbbr

void collegeToString (char cC, char * dest, size_t destSz)
{ // An old variant in VersionArchives\1232\_appname.j --> _tostrng.cpp
  if (! (dest && destSz))
    return ;
  dest [0] = '\0';

  // 20070703
  if (PRINT_COLLEGE) // 20060406
  {
    if (destSz > 1)
     {
      TCollege & c = * College ;

      c.college = cC ;
      if (c.college && c.Try ())
        ostrstream (dest, destSz) << c.name << ends ;
      else
        dest [0] = '\0' ;
     }
    if (destSz)
      dest [destSz - 1] = '\0' ;
  }

}  // collegeToString

void grantToString (char gC, char * dest, size_t destSz)
 {  // An old variant in VersionArchives\1232\_appname.j --> _tostrng.cpp
  if (! (dest && destSz))
    return ;
  if (destSz > 1)
   {
    TGrant & g = * Grant ;

    g.grant = gC ;
    if (g.grant && g.Try ())
      ostrstream (dest, destSz) << g.name << ends ;
    else
      dest [0] = '\0' ;
   }
  if (destSz)
    dest [destSz - 1] = '\0' ;
 }  // grantToString

void CDateToString (const CDate & d, char * dest, size_t destSz)
 {
  if (! (dest && destSz))
    return ;
  if (! (d.day || d.month || d.year))
   {
//    mkStrBlank (dest, destSz) ;
    dest [0] = '\0' ;
    return ;
   }
  if (destSz > 1)
   {
    ostrstream oss (dest, destSz) ;

    oss.fill ('0') ;
    oss << setw (2) << (int) (d.day) << '.'
        << setw (2) << (int) (d.month) << '.'
        << setw (4) << d.year << ends ;
   }
  if (destSz)
    dest [destSz - 1] = '\0' ;
 }  // CDateToString

void CTimeHMToString (const CTime & t, char * dest, size_t destSz)
 {
  if (! (dest && destSz))
    return ;
  if (! (t.hour || t.minute))
   {
//    mkStrBlank (dest, destSz) ;
    dest [0] = '\0' ;
    return ;
   }
  if (destSz > 1)
   {
    ostrstream oss (dest, destSz) ;

    oss.fill ('0') ;
    oss << setw (2) << (int) (t.hour) << ':'
        << setw (2) << (int) (t.minute) << ends ;
   }
  if (destSz)
    dest [destSz - 1] = '\0' ;
 }  // CTimeHMToString

void CDateForStatePaperToString(const CDate& d, char* dest, size_t destSz)
{
  if(!(dest && destSz))
    return;
  if(!(d.day || d.month || d.year))
  {
    dest[0] = '\0';
    return;
  }
  if(destSz > 1)
  {
    ostrstream oss(dest, destSz);
    const char* m = "?";

    switch(d.month)
    {
      case  1: m = "I";    break;
      case  2: m = "II";   break;
      case  3: m = "III";  break;
      case  4: m = "IV";   break;
      case  5: m = "V";    break;
      case  6: m = "VI";   break;
      case  7: m = "VII";  break;
      case  8: m = "VIII"; break;
      case  9: m = "IX";   break;
      case 10: m = "X";    break;
      case 11: m = "XI";   break;
      case 12: m = "XII";  break;
    }
    oss.fill('0') ;
    oss << setw(2) << (int)(d.day) << '.'
        << m << '.'
        << setw(4) << d.year << ends;
  }
  if(destSz)
    dest[destSz - 1] = '\0';
}  // CDateForStatePaperToString

void CTimeHMForStatePaperToString(const CTime& t, char* dest,
  size_t destSz)
{
  if(!(dest && destSz))
    return;
  if(!(t.hour || t.minute))
  {
//    mkStrBlank (dest, destSz) ;
    dest[0] = '\0';
    return;
  }
  if(destSz > 1)
  {
    ostrstream oss(dest, destSz);

    oss.fill('0');
    oss << setw(2) << (int)(t.hour) << " ч.";
    if(t.minute)
    {
      oss << " " << setw(2) << (int)(t.minute) << " мин.";
    }
    oss << ends ;
  }
  if(destSz)
    dest[destSz - 1] = '\0';
}  // CTimeHMForStatePaperToString

static void subjectToNameOrAlias (char type, long subject, bool alias,
					    char * dest, size_t destSz, size_t skip)
 {  // 15.jan.2004
  if (! (dest && destSz))
    return ;
  if (destSz > 1)
   {
    TSubject & s = * Subject ;

    s.type = type ;
    s.subject = subject ;
    if (s.type && s.Try ())
     {
	const char* const n =
	  alias ? (s.alias [0] ? s.alias : s.name) : s.name ;
	const size_t l = strlen (n) ;
	size_t from = skip ;

	if (from > l)
	  from = l ;
	ostrstream (dest, destSz) << (n + from) << ends ;
     }
    else
	dest [0] = '\0' ;
   }
  if (destSz)
    dest [destSz - 1] = '\0' ;
 }  // subjectToNameOrAlias

void subjectToString (char type, long subject, char * dest, size_t destSz,
			    size_t skip)
 {  // An old variant in VersionArchives\1820\_appname.j --> _tostrng.cpp
  subjectToNameOrAlias (type, subject, false, dest, destSz, skip) ;
 }  // subjectToString

void subjectToAlias (char type, long subject, char * dest, size_t destSz,
			   size_t skip)
 {  // 15.jan.2004
  subjectToNameOrAlias (type, subject, true, dest, destSz, skip) ;
 }  // subjectToAlias

void subjectAndExtraTextToString (char kind, long subject, char * dest,
					    size_t destSz, const char * extraText,
					    const bool write_sL)
 {
  char type = kind2type(kind);

  if (! (dest && destSz))
    return ;

  if (destSz > 1)
   {  // -- 0 --
    char * tmpStr = new char [destSz] ;

    if (tmpStr)
     {  // -- 1 --
	ostrstream oss (dest, destSz) ;

	subjectToString (type, subject, tmpStr, destSz) ;
	if (write_sL)
	  oss << TSubject::Code(subject, kind) << "  " ;
	oss << tmpStr << "\n" ;
	if (extraText)
	  oss << extraText ;
	oss << ends ;
	delete[] tmpStr ;
     }  // -- 1 --
    else
      dest [0] = '\0' ; 
   }  // -- 0 --
  if (destSz)
    dest [destSz - 1] = '\0' ;
 }  // subjectAndExtraTextToString

void compositionToString (long cL, char * dest, size_t destSz)
 {  // An old variant in VersionArchives\1232\_appname.j --> _tostrng.cpp
  if (! (dest && destSz))
    return ;

  if (    (destSz > 1)
       && (use_area_compositions () )  // 20070806
     )
  {
    TComposition & c = * Composition ;

    c.composition = cL ;
    if (c.composition && c.Try ())
      ostrstream (dest, destSz) << c.name << ends ;
    else
      dest [0] = '\0' ;
  }

  if (destSz)
    dest [destSz - 1] = '\0' ;

 }  // compositionToString

void compositionToStringCompl (long cL, char * dest, size_t destSz)
{
  if (dest && destSz)
  {
     dest [0] = '\0' ;
     if (use_area_compositions ())  // 20070806
     {
        if (cL)
        {
            compositionToString (cL, dest, destSz) ;

            const size_t l = strlen (dest) ;

            if (l < destSz - 1)
            {
                ostrstream (dest + l, destSz - 1 - l) << " състав" << ends ;
                dest [destSz - 1] = '\0' ;
  }  }  }   }
}  // compositionToStringCompl

bool involvementToString (char iC, char * dest, size_t destSz,
                          const bool upper)
{  // An old variant in VersionArchives\1232\_appname.j --> _tostrng.cpp
  bool res = false ;

  if (! (dest && destSz))
    return res ;
  if (destSz > 1)
   {
    TInvolvement & i = * Involvement ;

    i.involvement = iC ;
    if ((res = (i.involvement && i.Try ())) == true)
      ostrstream (dest, destSz) << i.name << ends ;
    else
      dest [0] = '\0' ;
   }
  if (destSz)
    dest [destSz - 1] = '\0' ;
  if (upper)
   {
    const size_t l = strlen (dest) ;

    for (size_t i = 0 ; i < l ; i ++)
      dest [i] = upCyr (dest [i]) ;
   }
  return res ;
 }  // involvementToString

bool resultToString (char rC, char * dest, size_t destSz)
 {  // An old variant in VersionArchives\1232\_appname.j --> _tostrng.cpp
  Result->result = rC ;
  bool res = Result->Try() ;
  if (dest != Result->name)
	strzcpy(dest, Result->name, destSz);
  return res;
 }  // resultToString

bool resolutionToString (char rC, char * dest, size_t destSz)
 {  // An old variant in VersionArchives\1232\_appname.j --> _tostrng.cpp
  bool res = false ;

  if (! (dest && destSz))
    return res ;
  if (destSz > 1)
   {
    TResolution & r = * Resolution ;

    r.resolution = rC ;
    if ((res = (r.resolution && r.Try ())) == true)
	ostrstream (dest, destSz) << r.name << ends ;
    else
      dest [0] = '\0' ;
   }
  if (destSz)
    dest [destSz - 1] = '\0' ;
  return res ;
 }  // resolutionToString

const char
 * const BL2 = "  ",
 * const NO = "N ",
 * const SUBNO = "Подн.",
 * const ENTRANCE = "Вх.",
 * const FLOOR = "Ет.",
 * const APARTMENT = "Ап."
  ;

void addressWithStr_SB_ToString (const TAddress & addr, ostream & os)
 {  // An old variant in VersionArchives\1232\_appname.j --> _tostrng.cpp
  const long int begPos = os.tellp () ;

   {
    TStreet & s = * Street ;

    s.uclp = addr.uclp ;
    s.street = addr.street ;
    if (s.uclp && s.street && s.Try ())
      os << s.name ;
   }
  // 2008:304 LRQ: no before blockNo
  if (! strIsBlank (addr.adrNo))
    os << (os.tellp () == begPos ? "" : BL2)
	 << NO << addr.adrNo ;
  if (! strIsBlank (addr.blockNo))
    os << (os.tellp () == begPos ? "" : BL2)
       << BLOCK_NO << addr.blockNo ;
  if (! strIsBlank (addr.subNo))
    os << (os.tellp () == begPos ? "" : BL2)
       << SUBNO << addr.subNo ;
  if (! strIsBlank (addr.entrance))
    os << (os.tellp () == begPos ? "" : BL2)
       << ENTRANCE << addr.entrance ;
  if (! strIsBlank (addr.floor))
    os << (os.tellp () == begPos ? "" : BL2)
       << FLOOR << addr.floor ;
  if (! strIsBlank (addr.apartment))
    os << (os.tellp () == begPos ? "" : BL2)
       << APARTMENT << addr.apartment ;
  if (! strIsBlank (addr.remark))
    os << (os.tellp () == begPos ? "" : BL2)
       << addr.remark ;
 }  // addressWithStr_SB_ToString

void addressWithStr_SB_ToString (const TAddress & addr,
                                 char * dest, const size_t destSz)
 {
  if (! (dest && destSz))
    return ;
  dest [0] = '\0' ;

   {
    ostrstream oss (dest, destSz) ;

    addressWithStr_SB_ToString (addr, oss) ;
    oss << ends ;
   }
  dest [destSz - 1] = '\0' ;
  strDelLastSpaces (dest) ;
 }  // addressWithStr_SB_ToString

void addressWithSettlStr_SB_ToString (const TAddress & addr,
                                      char * dest, size_t destSz,
                                      const bool includingArea)
 {  // An old variant in VersionArchives\1232\_appname.j --> _tostrng.cpp
  if (! (dest && destSz))
    return ;

  ostrstream oss (dest, destSz) ;

   {
    TUCLP & u = * UCLP ;

    u.uclp = addr.uclp ;
    if (u.uclp && u.Try ())
     {
      oss << u.name << BL2 ;
      if (includingArea)  // 02.feb.2001 - begin
       {
        TArea & a = * Area ;

        a.area = u.area ;
        if (a.area && a.Try ())
          oss << "обл. " << a.name << BL2 ;
       }  // 02.feb.2001 - end
     }
   }
  addressWithStr_SB_ToString (addr, oss) ;
  oss << ends ;
  dest [destSz - 1] = '\0' ;
  strDelLastSpaces (dest) ;
 }  // addressWithSettlStr_SB_ToString

void streetAndAddressToString (const TStreet & street,
                               const TAddress & addr,
                               char * dest, size_t destSz)
 {
  if (! (dest && destSz))
    return ;
  ostrstream (dest, destSz)
    << street.name << BL2
    << NO << addr.adrNo << BL2
    << SUBNO << addr.subNo << BL2
    << ENTRANCE << addr.entrance << BL2
    << FLOOR << addr.floor << BL2
    << APARTMENT << addr.apartment << ends ;
  dest [destSz - 1] = '\0' ;
 }  // streetAndAddressToString

void addressToString (const TAddress & addr,
                      char * dest, size_t destSz)
 {
  if (! (dest && destSz))
    return ;
  ostrstream (dest, destSz)
    << NO << addr.adrNo << BL2
    << SUBNO << addr.subNo << BL2
    << ENTRANCE << addr.entrance << BL2
    << FLOOR << addr.floor << BL2
    << APARTMENT << addr.apartment << ends ;
  dest [destSz - 1] = '\0' ;
 }  // addressToString

void addressToStringForSummons (const TAddress & addr, char * dest,
				size_t destSz,
				const bool includingBlockNo)
 {
  if (! (dest && destSz))
    return ;
  ostrstream oss (dest, destSz) ;

  // 2008:304 LRQ: no before blockNo
  oss
    << NO << addr.adrNo << BL2;
  if (includingBlockNo)
    oss << BLOCK_NO << addr.blockNo << BL2 ;
  oss
    << SUBNO << addr.subNo << BL2
    << ENTRANCE << addr.entrance << BL2
    << FLOOR << addr.floor << BL2
    << APARTMENT << addr.apartment << BL2
		 << addr.remark
    << ends ;
  dest [destSz - 1] = '\0' ;
 }  // addressToStringForSummons

void sourceNoAndSenderToString (const TLawsuit & lawsuit,
				char * dest, size_t destSz)
 // sourceNo is TLawsuit.source.no
 {  // An old variant in VersionArchives\1232\_appname.j --> _tostrng.cpp
  if (! (dest && destSz))
    return ;

  if (destSz > 1)
   {  // -- 0 --
    char * tmpStr = new char [destSz] ;

    if (tmpStr)
     {  // -- 1 --
      ostrstream oss (dest, destSz) ;
      // 20070121 -->
      {

          char decisiv_acts[7] = {
            	CONNECT_TYPE_JURISDICTION, //	= 'J',        // Дело по подсъдност
            	CONNECT_TYPE_RENEWED_LAWSUIT, //	= 'N',    // Възобновено дело
            	CONNECT_TYPE_RERETURNED_LAWSUIT, //	= 'U',  // Внесено след доразследване
            	CONNECT_TYPE_RETURNED_LAWSUIT, //	= 'R',    // Върнато дело
            	//CONNECT_TYPE_LOWER_INSTANCE, //	= 'L',      // Първоинстанционно дело
            	CONNECT_TYPE_HIGHER_INSTANCE, //	= 'H',    // Дело на висша инстанция
              '\0'};

         	TGArray<TConnect> connects;
          TConnect *connect = new TConnect;
          TLawsuit *pLaw = new TLawsuit;
          pLaw -> key = lawsuit.key;

          TSender s ;
          char tmp_law_kind [SIZE_OF_NAME + SIZE_OF_REQUEST_TEXT ] ;

         	if (load_connects(pLaw, &connects))
          {
        		  for (int i = 0; i < connects.Count(); i++)
        		  {
           			  *connect << connects[i];
                  {
                     tmp_law_kind [0] = '\0' ;
                     clearGroupData (s) ;

                     ConnectType->Seek(connect -> connectType);

			               if (strchr ( decisiv_acts, connect -> connectType) )
			               {
                        if (( connect -> ucnType )&&( connect -> ucn ))
                        { // get sender name
                           senderUCNAndUCNTypeToName (connect -> ucn , connect -> ucnType,
                                                      s.name, sizeof (s.name) );
				}
				// 2009:167 WTF?
				//if ((connect -> connectNo && connect -> connectNo[0]) || connect -> connectYear || s.name [0])
				if (connect->connectKind)
				{  // connected law kind
				   connectKindToString (connect -> connectKind , tmp_law_kind, sizeof (tmp_law_kind)) ;
                        }

//                        connect_to_string( connect, tmp, sizeof (tmp) ); // withSender = true;
//                                                                         // senderPrefix = NULL
//                                                                         // labelOnly = false

                        // compose output
                        if (connect -> connectType == CONNECT_TYPE_LOWER_INSTANCE)//Първоинстанционно дело
                        {
                           oss << " По ";
                           if (tmp_law_kind[0]) oss << tmp_law_kind << " " ;
                           else                 oss << "дело";
                        }
                        if (   ( connect -> connectType == CONNECT_TYPE_HIGHER_INSTANCE)//Дело на висша инстанция
                             ||( connect -> connectType == CONNECT_TYPE_RERETURNED_LAWSUIT)//Внесено след доразследване
                           )
                        {
                           oss << ConnectType -> name << " " ;
                           if (tmp_law_kind[0]) oss << tmp_law_kind ;
                           else                 oss << "дело";
                        }

                        if(   (connect -> connectType == CONNECT_TYPE_JURISDICTION)//Дело по подсъдност
                            ||(connect -> connectType == CONNECT_TYPE_RENEWED_LAWSUIT)//Възобновено дело
                            ||(connect -> connectType == CONNECT_TYPE_RETURNED_LAWSUIT)// Върнато дело
                          )
                        {
                           oss << ConnectType -> alias << " " ;
                           if (tmp_law_kind[0]) oss << tmp_law_kind ;
                           else                 oss << "дело";
                        }

                        oss << " No " << connect -> connectNo ;
                        if (connect -> connectYear)
                        oss << "/" << connect -> connectYear <<  "г." ;
                        if (  (connect -> composition)
                             && ( use_area_compositions () )  // 20070806
                           )
                        {
                            char comp_t [SIZE_OF_NAME + SIZE_OF_REQUEST_TEXT ] ;
				    comp_t [0] = '\0' ;
				    // 2012:009; 2012:009 LRQ: reverted
				    //compositionToString (connect -> composition, comp_t, sizeof (comp_t)) ;
				    strcpy(comp_t, connect->CompositionName());
                            oss << " " << comp_t << " състав " ;
                        }
                        if (s.name [0])
                        oss << " " << s.name ;

                        if(i < connects.Count()-1) oss << '\n' ;
                     } // if (strchr ( decisiv_acts,
                  }
          		} // for
        	} // if (load_connects(..
        	delete connect;
          delete pLaw;
      }
// 20070121 <--

      oss << ends ;
      delete[] tmpStr ;
     }  // -- 1 --
    else
      dest [0] = '\0' ;
   }  // -- 0 --
  if (destSz)
    dest [destSz - 1] = '\0' ;
 }  // sourceNoAndSenderToString


void monthToString (int month, char * dest, size_t destSz)
 {
  if (! (dest && destSz))
    return ;
  if (destSz > 1)
   {
    strcpy(dest, Month->Seek(month));
   }
  if (destSz)
    dest [destSz - 1] = '\0' ;
 }  // monthToString

void thisCourtToString (char * dest, size_t destSz)
 {

  if (! (dest && destSz))
    return ;
  ostrstream (dest, destSz) << AppNameCourt << ends ;
  dest [destSz - 1] = '\0' ;
 }  // thisCourtToString

void side_to_string(const TSideWind* const pSide, char* const dest,
  const size_t destSz, const bool includingUCN, const bool includingInvolvement)
{
  if (! (dest && destSz))
    return ;
  dest [0] = '\0' ;
  if (! pSide)
    return ;
  dest [destSz - 1] = '\0' ;

  ostrstream oss (dest, destSz - 1) ;

  if (includingInvolvement)
  {
    TInvolvement invl ;

    invl.involvement = pSide -> involvement ;
    if (invl.Get ())
	oss << setiosflags (ios :: left) << setw (sizeof (invl.name) - 1)
	    << invl.name << " " ;
  }
  if (includingUCN)
    oss << setiosflags (ios :: left) << setw (sizeof (pSide -> ucn) - 1)
	  << pSide -> ucn << " " ;
  oss << ends ;

  const size_t begDestInd = strlen (dest) ;
  const size_t spaceLeft = destSz - begDestInd ;

  if (spaceLeft > 1)
    if (strchr (UCN_CITIZENS, pSide -> ucnType))
	citizenUCNAndUCNTypeToName (pSide -> ucn, pSide -> ucnType,
				  dest + begDestInd, spaceLeft) ;
    // 2008:297 LRQ: support for unknowns; LPR: for anonymous
    // 2013:343 LPR: strchr -> ==
    else if (pSide -> ucnType == UCN_UNKNOWN_CODE || pSide -> ucnType == UCN_ANONYMOUS_CODE)
	senderUCNAndUCNTypeToName (pSide -> ucn, pSide -> ucnType,
				   dest + begDestInd, spaceLeft) ;
    else
	firmUCNAndUCNTypeToName (pSide -> ucn, pSide -> ucnType,
				 dest + begDestInd, spaceLeft) ;
}  // side_to_string

void settlStreetBlockNoToString (const TAddress & a, char * const dest,
				 const size_t destSz)
 {
  if (! (dest && destSz))
    return ;

  char * tmp0 = new char [destSz] ;
  char * tmp1 = new char [destSz] ;
  char tmp2 [sizeof (a.blockNo) + 10] ;

  tmp2 [0] = '\0' ;
  // 2008:304 LRQ: 11039 -blockNo
  //if (a.blockNo [0])
  //  ostrstream (tmp2, sizeof (tmp2))
  //    << BLOCK_NO << a.blockNo << ends ;
  tmp2 [sizeof (tmp2) - 1] = '\0' ;
  uclpToName (a.uclp, tmp0, destSz) ;
  streetToName (a.street, a.uclp, tmp1, destSz) ;

  const size_t l0 = strlen (tmp0) ;
  size_t l1 = strlen (tmp1) ;
  const size_t l2 = strlen (tmp2) ;

  while (l0 + l1 + l2 + 1 + 1 > destSz - 1 &&
	 (l1 = strlen (tmp1)) > SIZE_OF_STREET_NAME / 2)
   {
    if (l1 < 5)
      break ;
    tmp1 [l1 - 1] = '\0' ;
   }
  ostrstream (dest, destSz)
    << tmp0 << " " << tmp1 << " " << tmp2 << ends ;
  dest [destSz - 1] = '\0' ;
  delete[] tmp0 ;
  delete[] tmp1 ;
 }  // settlStreetBlockNoToString

void side_link_to_string(char* const buff, const size_t buffSz,
  const TLinkWind& rLink, const bool useNewLines, const bool fake,
  TGArray<TLinkWind> *links)
{
  if(buff && buffSz)
  {  // -- 0 --
    buff[0] = '\0';
    if(buffSz > 1)
    {  // -- 1 --
      TSideWind o;

      rLink.GetOrdinary(o);
      {
	stringArray textReceiver;

	{
	  LinkWriter linkWriter(textReceiver, buff, buffSz, o.key);
	  linkWriter.useCommaChar = !useNewLines;
	  linkWriter.MakeLinkTextItems(rLink, fake, false, links);
	}
	buff[0] = '\0';
	for(int i = 0; i < textReceiver.Count(); i ++)
	{
	  const char * const txtToAdd = (textReceiver[i]);
	  const size_t l = strlen(buff);

	  if(strIsBlank(txtToAdd) == false)
	    if(l + 1 < buffSz)
		ostrstream(buff + l, buffSz - l)
		<< (textReceiver[i]) << endl << ends;
	  buff[buffSz - 1] = '\0';
	}
	}
	buff[buffSz - 1] = '\0';
	if(useNewLines == false)
	{
	char* nL;

	while((nL = strchr(buff, '\n')) != NULL)
	  *nL = ' ';
	strDelLeadingSpaces(buff);
	}
    }  // -- 1 --
  }  // -- 0 --
}  // side_link_to_string

// 20070103 -->
void connect_to_string (const TConnect * pConnect, char * const dest,
			const size_t destSz,
			//const bool is1,
			//const char sourceKind, const char psKind,
			const bool withSender,            //= true; // 06.dec.2001
			const char * const senderPrefix,  //= NULL  // 06.dec.2001
			const bool labelOnly,             //= false // 17.dec.2001
                        const bool alias_txt              // 20070201
                       )
 {
  if (! (dest && destSz))
    return ;
  dest [destSz - 1] = '\0' ;

  ostrstream oss (dest, destSz) ;

  oss.clear () ;
  oss.seekp (0) ;


  {  // -- 0 --
    char * label = "" ;
    char tmp_kind[SIZE_OF_NAME];   tmp_kind [0] = '\0';

    ConnectType->Seek( pConnect -> connectType );
    if (alias_txt)
         strcpy(label, ConnectType -> alias );
  	else strcpy(label, ConnectType -> name  ); //следствено дело, ...

    oss  << label;

 	  // 20070116 label += strlen(label) - 5;
	  if (pConnect -> connectKind)
	{
		   sprintf(tmp_kind, //label + (strcmp(label, " дело") ? 5 : 0),
				 " %s", ConnectKind->Seek( pConnect -> connectKind)  );
	 oss  << tmp_kind;
  	}

    if (labelOnly == false)  // 17.dec.2001
    {  // 17.dec.2001
      oss
          << " "
          << "No "  // 17.dec.2001
          << pConnect -> connectNo << "/" << pConnect -> connectYear
          << "г." ;  // 17.dec.2001
    }  // 17.dec.2001
  }  // -- 0 --

  oss << ends ;
  dest [destSz - 1] = '\0';
  if (labelOnly == false)
  {
    if (withSender)
    {
      TSender sender ;
     	sender.ucnType = pConnect -> ucnType;
      scSprintf (sizeof (sender.ucn), sender.ucn, pConnect -> ucn) ;
      if (pConnect -> ucnType && pConnect -> ucn)
      {
      	if ( sender.Get() )
        {
	  size_t len=0, len1=0; //, len2=0;
          if (dest[0]) len = strlen (dest) ;
	  if (sender.name[0]) len1=strlen(sender.name);
          if (senderPrefix && senderPrefix[0])
          {}
//        if (senderPrefix && senderPrefix[0]) len2=strlen(senderPrefix);
          if (len1>0)
            ostrstream (dest + len, len+len1+1+1)
                << ' '
//                << (senderPrefix && senderPrefix [0] ? senderPrefix : "")
//                << (senderPrefix && senderPrefix [0] ? " " : "")
		<<  sender.name << ends ;
  } } } }

 }  // connect_to_string

// 20070103 <--

void surroundment_to_string (const TSurroundment & ment,
			     char * const dest, const size_t destSz)
 {
  if (dest && destSz)
   {  // -- 0 --
    dest [0] = '\0' ;
    if (destSz > 1)
     {  // -- 1 --
      ostrstream (dest, destSz)
        << "\""
        << DatedContainer_ (ment.lawsuit.type, ment.lawsuit.no,
                            ment.lawsuit.year,
                            ment.sessionKind, ment.sessionDate)
        << "\" по \""
        << TRCDKeyContainer_ (ment.key.type, ment.key.no, ment.key.year,
                              ment.kind)
	<< "\"" << ends ;
      dest [destSz - 1] = '\0' ;
     }  // -- 1 --
   }  // -- 0 --
 }  // surroundment_to_string

void surround_to_string (const TSurround & surround,
                         char * const dest, const size_t destSz)
 {
  if (dest && destSz)
   {  // -- 0 --
    dest [0] = '\0' ;
    if (destSz > 1)
     {  // -- 1 --
      ostrstream oss (dest, destSz) ;

      if (surround.text [0])
        oss << surround.text << " No " << surround.key.no
            << "/" << surround.key.year ;
      else
        oss << TRCDKeyContainer_ (surround.key.type, surround.key.no,
                                  surround.key.year, surround.kind) ;
      oss << ends ;
     }  // -- 1 --
    dest [destSz - 1] = '\0' ;
   }  // -- 0 --
 }  // surround_to_string

void ucn_type_to_string (char ucnT, char * dest, size_t destSz)
 {  // An old variant in VersionArchives\1232\_appname.j --> _tostrng.cpp
  if (dest && destSz)
   {
    if (destSz > 1)
     {
      if (ucnT)
       {
        TUCNType & t = * UCNType ;

        t.ucnType = ucnT ;
        if (t.ucnType && t.Try ())
          ostrstream (dest, destSz) << t.name << ends ;
        else
          dest [0] = '\0' ;
       }
      else
        dest [0] = '\0' ;
     }
    if (destSz)
      dest [destSz - 1] = '\0' ;
   }
 }  // ucn_type_to_string

void container_sides_to_string (char * const dest, const size_t destSz,
					  const TRCDKeyContainer & container,
                                const TSideWind & pattern,
					  const int suitors,
                                const int defendants,
                                const int others)
 {
  if (dest && destSz)
   {
    if (destSz > 1)
     {
      ostrstream oss (dest, destSz) ;

      container_sides_to_ostream (oss, container, pattern,
                                  suitors, defendants, others) ;
      oss << ends ;
     }
    dest [destSz - 1] = '\0' ;
   }
 }  // container_sides_to_string

void result_as_faked_kind_to_string (const char rC, char * const dest,
                                     const size_t destSz,
                                     const bool article,
                                     const bool fullArticle,
                                     const bool msgWhenTroubled)
 {
  if (dest && destSz)
   {
    if (destSz > 1)
     {
      if (rC)
       {
        resultToString (rC, dest, destSz) ;
        const char * const s = "С " ;
        const size_t sL = strlen (s) ;

        if (dest [0])
         {
          if (    (strncmp (dest, s, sL) == 0)
               && dest [sL]
             //&& strchr (dest + sL, ' ') == NULL  // 20060705
             )
		memmove (dest, dest + sL, strlen (dest + sL) + 1) ;
          else
           {
            const char * const cA = "Съдебен акт" ;

            if (msgWhenTroubled)
              message ("Внимание! Поради проблем при определянето на "
                       "вида %s, в документа ще бъде изписано "
                       "\"%s\" вместо Решение/Определение/Присъда/...",
                       cA, cA) ;
            ostrstream (dest, destSz) << cA << ends ;
            dest [destSz - 1] = '\0' ;
           }
          dest [0] = upCyr (dest [0]) ;
          if (article)
            add_article (dest, destSz, fullArticle) ;
         }
       }
      else
        dest [0] = '\0' ;
     }
    if (destSz)
      dest [destSz - 1] = '\0' ;
   }
 }  // result_as_faked_kind_to_string

void preliminary_actions_to_string (char * const buff,
                                    const size_t buffSz,
                                    const TLawsuit & law,
                                    const char * const senderInstances,
                                    const bool withSender,
                                    const char * const senderPrefix,
                                    const bool labelOnly
                                   )
 {  // Attention! Keep the logic of this function in
    // conformity with the code of add_preliminaryActions_to_column.
  if (buff && buffSz)
   {
    buff [0] = '\0' ;

    if (buffSz > 1)
     {
      ostrstream oss (buff, buffSz) ;

      preliminary_actions_to_ostream (oss, law, senderInstances,
                                      withSender, senderPrefix,
                                      labelOnly) ;
      oss << ends ;
     }
    buff [buffSz - 1] = '\0' ;
   }
 }  // preliminary_actions_to_string

void period_to_string (CDate begD, CDate endD,
                       char * const dest, const size_t destSz,
                       bool mergeToMonthIfPossible,
                       const bool mergeToYearIfPossible,
                       const bool upper, const bool shortMYTxt,
                       const char * const prefix,
                       const char * const suffix)
 {
  if (dest && destSz)
   {  // -- 0 --
    const bool begDEmpty = begD.Empty () ;
    const bool endDEmpty = endD.Empty () ;

    if (destSz > 1)
     {  // -- 1 --
      ostrstream oss (dest, destSz) ;
      const char * const mTxt = shortMYTxt ? "м." : "месец" ;
      const char * const yTxt = shortMYTxt ? "г." : "година" ;

      if (mergeToYearIfPossible)
        mergeToMonthIfPossible = true ;
      if (prefix)
        oss << prefix << " " ;
       {  // -- 2 --
        bool merged = false ;

        if (mergeToMonthIfPossible)
         {  // -- 3 --
          if (begD.day == 1 && begD.month == 1 &&
              endD == lastDayOfThisMonth (endD))
           {  // -- 4 --
            if (mergeToYearIfPossible)
             {  // -- 5 --
              if (begD.month == 1 && endD.month == 12)
               {  // -- 6 --
                if (begD.year == endD.year)
                  oss << "през " << begD.year << " " << yTxt ;
                else
                  oss << "от " << begD.year << " " << yTxt << " до "
                               << endD.year << " " << yTxt ;
                merged = true ;
               }  // -- 6 --
             }  // -- 5 --
            if (merged == false)
		 {
		  char bM [64] ;
		  char eM [64] ;
		  strcpy(bM, Month->Seek(begD.month));
		  strcpy(eM, Month->Seek(begD.month));
		  if (begD.year == endD.year && begD.month == endD.month)
		    oss << "през " << mTxt << " " << bM << " "
			  << begD.year << " " << yTxt ;
		  else
		    oss << "от " << mTxt << " " << bM << " "
                    << begD.year << " " << yTxt << " "
                       "до " << mTxt << " " << eM << " "
                    << endD.year << " " << yTxt ;
              merged = true ;
             }
           }  // -- 4 --
         }  // -- 3 --
        if (merged == false)
         {
          // I think, here "г." is always suitable.
# if 0
          if (begDEmpty == false && endDEmpty == false)
            oss << "от " << begD << " г. до " << endD  << " г." ;
          else
            if (begDEmpty == true && endDEmpty == false)
              oss << "преди " << endD  << " г." ;
            else
              if (begDEmpty == false && endDEmpty == true)
                oss << "след " << begD << " г." ;
# else  // of 0
          if (begDEmpty == false)
            oss << "от " << begD << " г." ;
          if (begDEmpty == false && endDEmpty == false)
            oss << " "  ;
          if (endDEmpty == false)
            oss << "до " << endD  << " г." ;
# endif  // of 0
         }
       }  // -- 2 --
      if (suffix)
        oss << " " << suffix ;
      oss << ends ;
     }  // -- 1 --
    dest [destSz - 1] = '\0' ;
    if (begDEmpty == true && endDEmpty == true)
      dest [0] = '\0' ;
    if (upper)
      up_cyr_str (dest) ;
   }  // -- 0 --
 }  // period_to_string

static void add_brackets(char* const dest, const size_t destSz,
  bool enclose)
{
  if(dest && destSz > 2)
  {
    if(enclose == true)
    {
      CharPtr tmp(strlen(dest) + 1);

      strCopy(tmp, dest, tmp.Size());
      strCopy(dest, "[", destSz);
      strConcat(dest, tmp, destSz);
      strConcat(dest, "]", destSz);
    }
    else
    {
      strConcat(dest, "[", destSz);
      strConcat(dest, "]", destSz);
    }
  }
}  // add_brackets

static void noun_with_gender_to_string(const char* ucn, const char ucnType,
  const char* const femaleVersion, char* const dest, const size_t destSz,
  const bool fullArticle, const bool brackets)
{
  if(dest && destSz && femaleVersion)
  {
    bool identified = false;

    if(ucnType == UCN_CITIZEN_UCN)
    {
      identified = true;
      if(ucnPresent(ucn) && strlen(ucn) > 9 && (ucn[8] - '0') % 2)
      {
        strCopy(dest, femaleVersion, destSz);
      }
    }
    else
    {
      if(ucnType && strchr(UCN_CITIZENS, ucnType) && ucnPresent(ucn))
      {
        TCitizen cit;
        cit.ucnType = ucnType;
        strCopy(cit.ucn, ucn, sizeof(cit.ucn));
        if(cit.Try() && strlen(cit.family) > 0)
        {
          const char lastLetter = cit.family[strlen(cit.family) - 1];

          if(strchr("аА", lastLetter))
          {
            strCopy(dest, femaleVersion, destSz);
          }
        }
      }
    }
    add_noun_article(dest, destSz, fullArticle);
    if(identified == false && brackets == true)
    {
      add_brackets(dest, destSz, false);
    }
  }
}  // noun_with_gender_to_string

bool involvementWithGenderToString(const char* ucn, const char ucnType,
  const char involvement, char* const dest, const size_t destSz,
  const bool upper, const bool fullArticle, const bool brackets)
{
  bool res = false;

  if(dest && destSz)
  {
    if(involvementToString(involvement, dest, destSz, upper))
    {
      const char* texts[][2] =
      {
        {"Ответник", "Ответница"},
        {"Ищец", "Ищца"},
        {"Пълномощник", "Пълномощничка"},
        {"Настойник", "Настойница"},
        {"Попечител", "Попечителка"},
      };
      const size_t txtPairs = sizeof(texts) / sizeof(texts[0]);
      int pair = -1;

      for(int i = 0; pair == -1 && i < txtPairs; i++)
      {
        if(strcmpi(dest, texts[i][0]) == 0)
        {
          pair = i;
        }
      }
      if(0 <= pair && pair < txtPairs)
      {
        noun_with_gender_to_string(ucn, ucnType, texts[pair][1], dest, destSz,
          fullArticle, brackets);
      }
      else
        if(brackets)
        {
          add_brackets(dest, destSz, false);
        }
      res = true;
    }
    if(res == false)
      ZERO_LEN(dest);
  }
  return res;
}  // involvementWithGenderToString

void region_act_to_string(const TRequest& rRequest, char* const dest,
  const size_t destSz)
{  // 21.dec.2004
  // By the idea of requestColumns::makeColumns --> iC_No
  if(dest && destSz > 0)
  {
    ZERO_LEN(dest);
#if INSTANCE
    if(TRCDKey_(rRequest.key).FilledOK())
    {
// 20070112 -->  in comment
/*
	if(rRequest.regionKind || rRequest.regionNo || rRequest.regionYear)
	{                          //Преписка/действие на съдия
	  if(rRequest.regionKind == KIND_REGION_REQUEST && rRequest.regionText[0])
	  {
	    ostrstream oss(dest, destSz);

	    oss << rRequest.regionText;
	    if(rRequest.regionNo)
	    {
		oss << " No " << rRequest.regionNo;
	    }
	    if(rRequest.regionNo && rRequest.regionYear)
	    {
		oss << "/";
	    }
	    if(rRequest.regionYear)
	    {
		oss << (rRequest.regionNo ? "" : " ")
		    << rRequest.regionYear << " г.";
	    }
	    oss << ends;
	  }
	  else
	    if(rRequest.regionKind && rRequest.regionNo &&
		rRequest.regionYear)
	    {
		ostrstream(dest, destSz)
		  << TRCDKeyContainer_(kind2type(rRequest.regionKind),
			rRequest.regionNo, rRequest.regionYear, rRequest.regionKind)
		  << ends;
	    }
	}
*/
// 20070112 <-- in comment
// 20070112 -->
		TGArray<TConnect> connects;
        TConnect *connect = new TConnect;
        TRequest * pReq = new TRequest ;
        pReq -> key = rRequest.key;

       	if (load_connects(pReq, &connects))
        {
       	    for (int i = 0; i < connects.Count(); i++)
       	    {
           	  *connect << connects[i];

              CharPtr buff_tmp(1024);
              ZERO_LEN(buff_tmp) ;

              //Преписка/действие на съдия
		  if( connect -> connectKind == CONNECT_KIND_REGION_REQUEST &&
                  connect -> text[0])
              {
                  if( connect -> decisionKind )
                  {
                     kindToString( connect -> decisionKind, buff_tmp, strlen(buff_tmp));
                  }

                  ostrstream oss(dest, destSz);
                  if ( strlen(buff_tmp) > 0 )
                     oss << "срещу " << buff_tmp << " ";
                  oss << connect -> text;
                  oss << " No " << connect -> connectNo << "/"
                      <<  connect -> connectYear << " г.";

                  if((connect -> ucn) && (connect -> ucnType))
                  {
                      TSender sender;

                      sender.ucnType = connect -> ucnType;
                      if ( sender.ucnType )
                      {
                         scSprintf (sizeof (sender.ucn), sender.ucn, "%s", connect -> ucn);
                         if ( sender.Try () )
                         {
                         oss << " " << sender.name << " ";
                         }
                      }
                  }
                  oss << ends;
              }
              else
                  if(( connect -> connectKind )&&( connect -> connectNo )&&( connect -> connectYear ))
                  {
                     ostrstream oss(dest, destSz);
                          // 20070208 ->
                          //<< TRCDKeyContainer_(kind2type(connect -> connectKind),
                          //                     connect -> connect.no,
                          //                     connect -> connect.year,
				  //                     connect -> connectKind)
				  // 20070208 <-
				  TConnectKind t;
				  t.connectKind = connect -> connectKind;
				  if(t.connectKind && t.Try())
				    oss << t.name << " ";
				  oss
				  << " No " << connect -> connectNo << "/"
                          <<  connect -> connectYear << " г.";
                  if((connect -> ucn) && (connect -> ucnType))
                  {
                      TSender sender;
                      sender.ucnType = connect -> ucnType;
                      if ( sender.ucnType )
                      {
                         scSprintf (sizeof (sender.ucn), sender.ucn, "%s", connect -> ucn);
                         if ( sender.Try () )
                         {
                         oss << " " << sender.name << " ";
                         }
                      }
                  }
                  oss << ends;
                  }
            } // for
        } // if (load_connects( ...
        delete connect;
        delete pReq;
// 20070112 <--
    }
#else  // of INSTANCE
    SUPPRESS_UNUSED(&rRequest);
#endif  // of INSTANCE
    dest[destSz - 1] = '\0';
  }
} // region_act_to_string
