#include "cac.h"

struct TABCRCDKey : public TRCDKey
 {
  public :
    TABCRCDKey (long n, long y, char t) ;
    TABCRCDKey (const TRCDKey & source) ;
    TABCRCDKey & operator = (const TABCRCDKey & copy) ;
    bool operator == (const TABCRCDKey & object) const ;  // required
    bool operator < (const TABCRCDKey & object) const ;  // required
    bool operator <= (const TABCRCDKey & object) const ;
    bool operator != (const TABCRCDKey & object) const ;
    bool operator > (const TABCRCDKey & object) const ;
 } ;  // struct TABCRCDKey

TABCRCDKey :: TABCRCDKey (long n, long y, char t)
 {
  no = n ;
  year = y ;
  type = t ;
 }  // TABCRCDKey :: TABCRCDKey

TABCRCDKey :: TABCRCDKey (const TRCDKey & source)
 {
  no = source.no ;
  year = source.year ;
  type = source.type ;
 }  // TABCRCDKey :: TABCRCDKey

TABCRCDKey & TABCRCDKey :: operator = (const TABCRCDKey & copy)
 {
  no = copy.no ;
  year = copy.year ;
  type = copy.type ;
  return (* this) ;
 }  // TABCRCDKey :: operator =

bool TABCRCDKey :: operator == (const TABCRCDKey & object) const
 {
  return no == object.no &&
	   year == object.year &&
	   type == object.type ;
 }  // TABCRCDKey :: operator ==

bool TABCRCDKey :: operator < (const TABCRCDKey & object) const
 {
  if (year < object.year)
    return true ;
  else
    if (year > object.year)
	return false ;
    else
	if (no < object.no)
	  return true ;
	else
	  if (no > object.no)
	    return false ;
	  else
	    return (type < object.type) ;
 }  // TABCRCDKey :: operator <

bool TABCRCDKey :: operator <= (const TABCRCDKey & object) const
 {
  return (* this) < object || (* this) == object ;
 }  // TABCRCDKey :: operator <=

bool TABCRCDKey :: operator > (const TABCRCDKey & object) const
 {
  return ! ((* this) <= object) ;
 }  // TABCRCDKey :: operator >

bool TABCRCDKey :: operator != (const TABCRCDKey & object) const
 {
  return ! ((* this) == object) ;
 }  // TABCRCDKey :: operator !=


class TABCLawsuit : public TLawsuit
 {
  public :
    TABCLawsuit () ;
//    TABCLawsuit & operator = (const TABCLawsuit & copy) ;  // required ?
    bool operator == (const TABCLawsuit & object) const ;  // required
    bool operator < (const TABCLawsuit & object) const ;  // required
    bool operator <= (const TABCLawsuit & object) const ;
 } ;  // class TABCLawsuit

TABCLawsuit :: TABCLawsuit () : TLawsuit ()
 {
 }  // TABCLawsuit :: TABCLawsuit

//TABCLawsuit & TABCLawsuit :: operator = (const TABCLawsuit & copy)
// {
//  (* this) << copy ;
//  return (* this) ;
// }  // TABCLawsuit :: operator =
//
bool TABCLawsuit :: operator == (const TABCLawsuit & object) const
 {
  return date.month == object.date.month &&
	   key.no == object.key.no ;
 }  // TABCLawsuit :: operator ==

bool TABCLawsuit :: operator < (const TABCLawsuit & object) const
 {
  if (date.month < object.date.month)
    return true ;
  else
    if (date.month > object.date.month)
	return false ;
    else
	return TABCRCDKey (key.no, key.year, key.type) <
		 TABCRCDKey (object.key.no, object.key.year,
				 object.key.type) ;
 }  // TABCLawsuit :: operator <

bool TABCLawsuit :: operator <= (const TABCLawsuit & object) const
 {
  return (* this) < object || (* this) == object ;
 }  // TABCLawsuit :: operator <=

typedef TGSArray<TABCLawsuit> TIS_ABCLawsuitArray ;  // 13.jan.2004

class TABCSide : public TSideWind
 {
  public :
    TABCSide () ;
    bool operator == (const TABCSide & object) const ;  // required
    bool operator < (const TABCSide & object) const ;  // required
    bool operator <= (const TABCSide & object) const ;
 } ;  // class TABCSide

TABCSide :: TABCSide () : TSideWind ()
 {
 }  // TABCSide :: TABCSide

bool TABCSide :: operator == (const TABCSide & object) const
 {
  TABCRCDKey thisK (key.no, key.year, key.type) ;
  TABCRCDKey objK (object.key.no, object.key.year,
				object.key.type) ;

  return thisK == objK &&
	   ucnType == object.ucnType &&
	   ! strcmp (ucn, object.ucn) ;
 }  // TABCSide :: operator ==

bool TABCSide :: operator < (const TABCSide & object) const
 {
  TABCRCDKey thisK (key.no, key.year, key.type) ;
  TABCRCDKey objK (object.key.no, object.key.year,
				object.key.type) ;

  if (thisK < objK)
    return true ;
  else
    if (thisK > objK)
	return false ;
    else
	if ((unsigned char) ucnType < (unsigned char) (object.ucnType))
	  return true ;
	else
	  if ((unsigned char) ucnType > (unsigned char) (object.ucnType))
	    return false ;
	  else
	    return strcmp (ucn, object.ucn) < 0 ;
 }  // TABCSide :: operator <

bool TABCSide :: operator <= (const TABCSide & object) const
 {
  return (* this) < object || (* this) == object ;
 }  // TABCSide :: operator <=

class TABCCitizen : public TCitizen
 {
  public :
    TABCCitizen () ;
//    TABCCitizen & operator = (const TABCCitizen & copy) ;  // required ?
    bool operator == (const TABCCitizen & object) const ;  // required
    bool operator < (const TABCCitizen & object) const ;  // required
    bool operator <= (const TABCCitizen & object) const ;
 } ;  // class TABCCitizen

TABCCitizen :: TABCCitizen () : TCitizen ()
 {
 }  // TABCCitizen :: TABCCitizen

//TABCCitizen & TABCCitizen :: operator = (const TABCCitizen & copy)
// {
//  operator << (copy) ;
//  return (* this) ;
// }  // TABCCitizen :: operator =
//
bool TABCCitizen :: operator == (const TABCCitizen & object) const
 {
  return name [0] == object.name [0] ;
 }  // TABCCitizen :: operator ==

bool TABCCitizen :: operator < (const TABCCitizen & object) const
 {
  return (unsigned char) (name [0]) < (unsigned char) (object.name [0]) ;
 }  // TABCCitizen :: operator <

bool TABCCitizen :: operator <= (const TABCCitizen & object) const
 {
  return (* this) == object || (* this) < object ;
 }  // TABCCitizen :: operator <=


class TABCFirm : public TFirm
 {
  public :
    TABCFirm () ;
//    TABCFirm & operator = (const TABCFirm & copy) ;  // required ?
    bool operator == (const TABCFirm & object) const ;  // required
    bool operator < (const TABCFirm & object) const ;  // required
    bool operator <= (const TABCFirm & object) const ;
 } ;  // class TABCFirm

TABCFirm :: TABCFirm () : TFirm ()
 {
 }  // TABCFirm :: TABCFirm

//TABCFirm & TABCFirm :: operator = (const TABCFirm & copy)
// {
//  operator << (copy) ;
//  return (* this) ;
// }  // TABCFirm :: operator =
//
bool TABCFirm :: operator == (const TABCFirm & object) const
 {
  return name [0] == object.name [0] ;
 }  // TABCFirm :: operator ==

bool TABCFirm :: operator < (const TABCFirm & object) const
 {
  return (unsigned char) (name [0]) < (unsigned char) (object.name [0]) ;
 }  // TABCFirm :: operator <

bool TABCFirm :: operator <= (const TABCFirm & object) const
 {
  return (* this) == object || (* this) < object ;
 }  // TABCFirm :: operator <=


class TABCDamn : public TDamn
 {
  public :
    TABCDamn () ;
    bool operator==(const TABCDamn &that) const { return name[0] == that.name[0]; }
    bool operator<(const TABCDamn &that) const { return name[0] < that.name[0]; }
    bool operator<=(const TABCDamn &that) const { return name[0] <= that.name[0]; }
 } ;  // class TABCDamn

TABCDamn :: TABCDamn () : TDamn ()
 {
 }  // TABCDamn :: TABCDamn

class TIS_ABCSideArray : public TGSArray<TABCSide>
 {
  public :
    TIS_ABCSideArray () { }
    bool indexOfFirstMatch (const TRCDKey & k, unsigned & i) const ;
 } ;  // class TIS_ABCSideArray


bool TIS_ABCSideArray :: indexOfFirstMatch (const TRCDKey & k,
                                            unsigned & i) const
 {
  const unsigned sACount = Count () ;
  unsigned s ;
  bool found = false, bigNo = false ;

  for (s = 0 ;
       s < sACount && ! found && ! bigNo ;
       s ++)
   {  // -- 1 --
    const TRCDKey sK = ((* this) [s]) -> key ;
    const TABCRCDKey sAbcK (sK.no, sK.year, sK.type) ;
    const TABCRCDKey abcK (k.no, k.year, k.type) ;

    if (sAbcK == abcK)
     {
      found = true ;
      i = s ;
     }
    else
      if (sAbcK > abcK)
        bigNo = true ;
   }  // -- 1 --
  return found ;
 }  //  TIS_ABCSideArray :: indexOfFirstMatch

class TIS_ABCCitizenArray : public TGSArray<TABCCitizen>
 {
  public :
    TIS_ABCCitizenArray () { }
    bool indexOfMatch (const char ucnT, const char * ucn,
			     const char lett, unsigned & i) const ;
 } ;  // class TIS_ABCCitizenArray

bool TIS_ABCCitizenArray :: indexOfMatch (const char ucnT,
                                          const char * ucn,
                                          const char lett,
                                          unsigned & i) const
 {
  const unsigned cACount = Count () ;
  unsigned c ;
  bool bigL = false ;
  bool found = false ;
  const unsigned char uLett = lett ;

  for (c = 0 ;
       c < cACount && ! found && ! bigL ;
       c ++)
   {  // -- 2 --
    const unsigned char cL = ((* this) [c]) -> name [0] ;

    if (cL == uLett)
     {
      if (((* this) [c]) -> ucnType == ucnT &&
          ! strcmp (((* this) [c]) -> ucn, ucn))
       {
        found = true ;
        i = c ;
       }
     }
    else
      if (cL > uLett)
        bigL = true ;
   }  // -- 2 --
  return found ;
 }  // TIS_ABCCitizenArray :: indexOfMatch

 
class TIS_ABCFirmArray : public TGSArray<TABCFirm>
 {
  public :
    TIS_ABCFirmArray () { }
    bool indexOfMatch (const char ucnT, const char * ucn,
			     const char lett, unsigned & i) const ;
 } ;  // class TIS_ABCFirmArray

bool TIS_ABCFirmArray :: indexOfMatch (const char ucnT,
                                       const char * ucn,
                                       const char lett,
                                       unsigned & i) const
 {
  const unsigned fACount = Count () ;
  unsigned f ;
  bool bigL = false ;
  bool found = false ;
  const unsigned char uLett = lett ;

  for (f = 0 ;
       f < fACount && ! found && ! bigL ;
       f ++)
   {  // -- 2 --
    const unsigned char fL = ((* this) [f]) -> name [0] ;

    if (fL == uLett)
     {
      if (((* this) [f]) -> ucnType == ucnT &&
          ! strcmp (((* this) [f]) -> ucn, ucn))
       {
        found = true ;
        i = f ;
       }
     }
    else
      if (fL > uLett)
        bigL = true ;
   }  // -- 2 --
  return found ;
 }  // TIS_ABCFirmArray :: indexOfMatch


class TIS_ABCDamnArray : public TGSArray <TABCDamn>
 {
  public :
    TIS_ABCDamnArray () { }
    bool indexOfMatch (const char ucnT, const char * ucn,
			     const char lett, unsigned & i) const ;
    bool indexOfMatch (const TRCDKey & key,
			     const char lett, unsigned & i) const ;
 } ;  // class TIS_ABCDamnArray

bool TIS_ABCDamnArray :: indexOfMatch (const char ucnT, const char * ucn,
                                       const char lett,
                                       unsigned & i) const
 {
  const unsigned dACount = Count () ;
  unsigned d ;
  bool bigL = false ;
  bool found = false ;
  const unsigned char uLett = lett ;

  for (d = 0 ;
       d < dACount && ! found && ! bigL ;
       d ++)
   {  // -- 2 --
    const unsigned char dL = ((* this) [d]) -> name [0] ;

    if (uLett == 0 || dL == uLett)
     {
      if (((* this) [d]) -> ucnType == ucnT &&
          ! strcmp (((* this) [d]) -> ucn, ucn))
       {
        found = true ;
        i = d ;
       }
     }
    else
      if (dL > uLett)
        bigL = true ;
   }  // -- 2 --
  return found ;
 }  // TIS_ABCDamnArray :: indexOfMatch

bool TIS_ABCDamnArray :: indexOfMatch (const TRCDKey & key,
                                       const char lett,
                                       unsigned & i) const
 {
  const unsigned dACount = Count () ;
  unsigned d ;
  bool bigL = false ;
  bool found = false ;
  const unsigned char uLett = lett ;

  for (d = 0 ;
       d < dACount && ! found && ! bigL ;
       d ++)
   {  // -- 2 --
    const unsigned char dL = ((* this) [d]) -> name [0] ;

    if (uLett == 0 || dL == uLett)
     {
      if (TABCRCDKey (((* this) [d]) -> key) == TABCRCDKey (key))
       {
        found = true ;
        i = d ;
       }
     }
    else
      if (dL > uLett)
        bigL = true ;
   }  // -- 2 --
  return found ;
 }  // TIS_ABCDamnArray :: indexOfMatch (const TRCDKey & key,
//                                       const char lett,
//                                       unsigned & i) const


class TABCBookSide : public TSideWind, public bookFormats
 {
  public :
    TLawsuit lawsuit ;
    TABCCitizen citizen ;
    TABCFirm firm ;
# if INSTANCE
    TABCDamn damn ;
    TAddress citAddress ;  // 04.jan.2000
    TForeignAddress citForgnAddress ;  // 04.jan.2000
    TAddress firmAddress ;  // 16.mar.2000 - end
    TDecision * pFirstDamnDec ; // 16.jun.2000
# endif
    TABCBookSide (const char * formatFN, TWindow * parent) ;
    virtual ~TABCBookSide () ;
    virtual void output (ostream & os, bool first) const ;
    virtual void clearData () ;
    virtual bool firmsRequired () ;
 } ;  // class TABCBookSide

void TABCBookSide :: clearData ()
 {
  clearSideData (* this) ;
  clearLawsuitData (lawsuit) ;
  clearCitizenData (citizen) ;
  clearFirmData (firm) ;
# if INSTANCE
  clearGroupData (damn) ;
  clearGroupData (citAddress) ;
  clearGroupData (citForgnAddress) ;
  clearGroupData (firmAddress) ;
  clearGroupData (* pFirstDamnDec) ;
# endif
 }  // TABCBookSide :: clearData

TABCBookSide :: TABCBookSide (const char * formatFN, TWindow * parent) :
               bookFormats (formatFN, parent), TSideWind ()
 {
# if INSTANCE
  pFirstDamnDec = new TDecision ;
# endif  // INSTANCE
  clearData () ;
 }  // TABCBookSide :: TABCBookSide

TABCBookSide :: ~TABCBookSide ()
 {
# if INSTANCE
  delete pFirstDamnDec ;
# endif  // INSTANCE
 }  // TABCBookSide :: TABCBookSide

void TABCBookSide :: output (ostream & os, bool first) const
 {
  const bookAreas * areasPtr = first ? pBA : pBA1 ;
  const int aCnt = first ? areasCnt : areas1Cnt ;
  int i ;

  if (! loadOK)
    return ;

  const bool citizenLoaded = citizen.ucnType && citizen.ucn ;
#if INSTANCE
  TDamn tmpDamn ;

  clearGroupData (tmpDamn) ;
  if (citizenLoaded)
    if (key.type == TYPE_FIRM_LAW && (! (damn.ucnType && damn.ucn [0])))
     {
	TLawsuit tmpLaw ;

	tmpLaw.key = lawsuit.key ;
	tmpLaw.kind = lawsuit.kind ;  // I'm not sure is this necessary
	tmpLaw.date = lawsuit.date ;  // I'm not sure is this necessary
	if (! lawsuitToDamn (& tmpLaw, & tmpDamn))
	  clearGroupData (tmpDamn) ;
     }
# endif
  for (i = 0 ; i < aCnt && os.good () ; i ++)
   {  // -- 0 --
    const size_t len = areasPtr [i].areaLen + areasPtr [i].from ;
    char * s = new char [len + 1] ;
    const int algn = areasPtr [i].align ;
    const size_t skip = areasPtr [i].from ;
    const unsigned int dCiph = areasPtr [i].dataCipher ;

# if INSTANCE
    if (! lawsuitOutput (lawsuit, os, s, len, algn, skip, dCiph) &&
	  ! sideOutput ((* this), os, s, len, algn, skip, dCiph) &&
	  ! monthOutput (os, s, len, algn, skip, dCiph) &&
	  ! (citizenLoaded ?
	     citizenOutput (citizen, os, s, len, algn, skip, dCiph) :
	     ((firm.ucnType && firm.ucn [0]) ?
		 firmLikeCitizenOutput (firm, os, s, len, algn, skip,
						dCiph) :
		 damnLikeCitizenOutput (damn, os, s, len, algn, skip, dCiph)
	     )
	    ) &&
	  ! miscOutput (os, s, len, algn, skip, dCiph)
	 )
# else
    if (! lawsuitOutput (lawsuit, os, s, len, algn, skip, dCiph) &&
	  ! sideOutput ((* this), os, s, len, algn, skip, dCiph) &&
	  ! monthOutput (os, s, len, algn, skip, dCiph) &&
	  ! (citizenLoaded ?
	     citizenOutput (citizen, os, s, len, algn, skip, dCiph) :
	     firmLikeCitizenOutput (firm, os, s, len, algn, skip, dCiph)) &&
	  ! miscOutput (os, s, len, algn, skip, dCiph))
# endif
     {  // -- 1 --
	bool outputOK = false ;

# if INSTANCE
	if (citAddress.ucnType)
	  outputOK = addressOutput (citAddress, os, s, len, algn,
					    skip, dCiph) ;
	else
	  if (citForgnAddress.ucnType)
	    outputOK = foreignAddrLikeAddrOutput (citForgnAddress, os, s,
								len, algn, skip, dCiph) ;
	if (! outputOK)
	  if (firmAddress.ucnType)
	    outputOK = addressOutput (firmAddress, os, s, len, algn,
						skip, dCiph) ;
	if (! outputOK)
	  if (key.type == TYPE_FIRM_LAW)  // *****
	    outputOK =
		 ((citizenLoaded && (! (damn.ucnType && damn.ucn [0]))) ?
		    damnOutput (tmpDamn, os, s, len, algn, skip, dCiph,
				    pFirstDamnDec) :
		    damnOutput (damn, os, s, len, algn, skip, dCiph,
				    pFirstDamnDec)
		 ) ;
# endif
	if (! outputOK)
	 {
	  if (len)
	    strcpy (s, "?") ;
	  s [len] = '\0' ;
	  writeString (os, s, len, algn, skip) ;
//          break ;
//       }  // -- 1 --      (switch)
	 }
     }  // -- 1 --
    delete [] s ;
   }  // -- 0 --   (for)
 }  // TABCBookSide :: output

bool TABCBookSide :: firmsRequired ()
 {
  int i ;
  char buff [16] ;
  char buff1 [16] ;
  ostrstream oss (buff, sizeof (buff) - 1) ;
  TFirm f ;
  bool perceived = false ;

  if (! loadOK)
    return false ;
  setmem (buff, sizeof (buff), '\0') ;
  clearFirmData (f) ;
  for (i = 0 ; i < areasCnt && ! perceived ; i ++)
    perceived = firmLikeCitizenOutput (f, oss,
                                       buff1, sizeof (buff1) - 1,
                                       RIGHT_ALIGN, 0,
                                       pBA [i].dataCipher) ;
  for (i = 0 ; i < areas1Cnt && ! perceived ; i ++)
    perceived = firmLikeCitizenOutput (f, oss,
                                       buff1, sizeof (buff1) - 1,
                                       RIGHT_ALIGN, 0,
                                       pBA1 [i].dataCipher) ;
  return perceived ;
 }  // TABCBookSide :: firmsRequired

// -------------------- TABCBookColumns - begin --------------------

class TABCBookColumns: public columnsArr
{  // 15.feb.2005
public:  // Construction / destruction
  TABCBookColumns(const char* formatFN, TWindow* parent, long int serialNo,
    const TABCBookSide& bS, TStringArray* pItems);
protected:  // Methods
  void MakeColumns(long int serialNo, const TABCBookSide& bS,
    TStringArray* pItems);
protected:  // Data         OFFSET_ABC_BOOK_CIPHER = 12000, - _CONST_.H
  enum
  {
    iC_CitizenLawsuit = 0,            //  1   Гражданско
    iC_PunishmentGeneralCharLawsuit,  //  2   Наказателно - Общ характер
    iC_PunishmentPrivateCharLawsuit,  //  3   Наказателно - Частен характер
    iC_AdminPunishmentLawsuit,        //  4   Наказателно - Административно
    iC_PrivateCitizenLawsuit,         //  5   Частно производство - Гражданско
    iC_PrivatePunishmentLawsuit,      //  6   Частно производство - Наказателно
    iC_Side,                          //  7
    iC_Ucn,                           //  8
    iC_SerialNo,                      //  9

    i_columns  // i_columns == (the number of columns)
  };
};  // class TABCBookColumns

TABCBookColumns::TABCBookColumns(const char* formatFN, TWindow* parent,
  long int serialNo, const TABCBookSide& bS, TStringArray* pItems):
  columnsArr(formatFN, parent, i_columns, 1 + OFFSET_ABC_BOOK_CIPHER, true)
{
  MakeColumns(serialNo, bS, pItems);
  defineMaxRows();
}

void TABCBookColumns::MakeColumns(long int serialNo, const TABCBookSide& bS,
  TStringArray* pItems)
{
  column* pColumn;
  CharPtr buff(1024);

  if((pColumn = columns[iC_CitizenLawsuit]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_CitizenLawsuit + bOC, pItems);

#if COURT_TYPE == COURT_AREAL
    if(bS.lawsuit.key.type != TYPE_PUNISHMENT_LAW && !strchr(KIND_PRIVATE_CITIZALIKE_LAWS, bS.lawsuit.kind))
#else  // 20110175 && AREAL
    if(bS.lawsuit.key.type != TYPE_PUNISHMENT_LAW
#if INSTANCE
#  if APPEAL
	&& ( bS.lawsuit.kind != KIND_PRIVATE_CITIZEN_LAW )
	&& ( bS.lawsuit.kind != KIND_PRIVATE_CASS_CITIZEN_LAW ) // 20070927
	&& ( bS.lawsuit.kind != KIND_PRIVATE_CASS_TRADE_LAW )   // 20070927
#  else      // AREAL
	&& bS.lawsuit.kind != KIND_PRIVATE_ADMIN_LAW   //Частно Адм. Дело
#  endif
#else        // REGION
	&& bS.lawsuit.kind != KIND_PRIVATE_CITIZEN_LAW
	&& bS.lawsuit.kind != KIND_PRIVATE_ADMIN_LAW	// 20140112
#endif
	)
#endif  // AREAL
    {
//    TRCDKey_(bS.lawsuit.key).PrintAlias(buff, buff.Size());//20050315
//    TRCDKey_(bS.lawsuit.key).PrintShortest(buff, buff.Size());
	TRCDKeyContainer_(bS.lawsuit).PrintAlias(buff, buff.Size());
	pColumn->addText(buff);
    }
  }  // -- iC_CitizenLawsuit --

  if((pColumn = columns[iC_PunishmentGeneralCharLawsuit]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_PunishmentGeneralCharLawsuit + bOC,
      pItems);

    if(bS.lawsuit.key.type == TYPE_PUNISHMENT_LAW
#if INSTANCE
#  if APPEAL // APPEAL
	&& ( bS.lawsuit.kind == KIND_PUNI_GENERAL_CHAR_LAW_2A ||    //'W' ВНОХД
	     bS.lawsuit.kind == KIND_1ST_RENEW_COMMON_PUNISHMENT_LAW )    //'[' НОХД/въз./
#  else      // AREAL
	&& ( bS.lawsuit.kind == KIND_PUNISHMENT_GENERAL_CHAR_LAW ||    //'O' НОХД
	     bS.lawsuit.kind == KIND_2ND_COMMON_PUNISHMENT_LAW )     //'W' ВНОХД
#  endif
#else        // REGIONAL
	&&   bS.lawsuit.kind == KIND_PUNISHMENT_GENERAL_CHAR_LAW
#endif
	)
    {

//    TRCDKey_(bS.lawsuit.key).PrintAlias(buff, buff.Size());//20050315
//    TRCDKey_(bS.lawsuit.key).PrintShortest(buff, buff.Size());
	TRCDKeyContainer_(bS.lawsuit).PrintAlias(buff, buff.Size());
	pColumn->addText(buff);
    }
  }  // -- iC_PunishmentGeneralCharLawsuit --

  if((pColumn = columns[iC_PunishmentPrivateCharLawsuit]) != NULL)     // 12003
  {
    ColumnXmlEncloser encloser(pColumn, iC_PunishmentPrivateCharLawsuit + bOC,
	pItems);

    buff[0]='\0';

    if(bS.lawsuit.key.type == TYPE_PUNISHMENT_LAW
#if INSTANCE
#  if APPEAL // APPEAL
	 && (bS.lawsuit.kind == KIND_2ND_PUNISHMENT_PRIVCHAR_LAW    //'!' ВНЧХД
		|| bS.lawsuit.kind == KIND_2ND_RENEW_PRIVCHAR_LAW)	// НЧХД/въз./
#  else      // AREAL
	 && bS.lawsuit.kind == KIND_2ND_PUNISHMENT_VNCHHD_LAW      //'C' ВНЧХД
#  endif
#else        // REGIONAL
	 && bS.lawsuit.kind == KIND_PUNISHMENT_PRIVATE_CHAR_LAW
#endif
	)
    {
//    TRCDKey_(bS.lawsuit.key).PrintAlias(buff, buff.Size());//20050315
//    TRCDKey_(bS.lawsuit.key).PrintShortest(buff, buff.Size());
	TRCDKeyContainer_(bS.lawsuit).PrintAlias(buff, buff.Size());
	pColumn->addText(buff);
    }
  }  // -- iC_PunishmentPrivateCharLawsuit --


  if((pColumn = columns[iC_AdminPunishmentLawsuit]) != NULL)   // 12004
  {
    ColumnXmlEncloser encloser(pColumn, iC_AdminPunishmentLawsuit + bOC, pItems);

    if( bS.lawsuit.key.type == TYPE_PUNISHMENT_LAW
#if INSTANCE
#  if APPEAL // APPEAL
	  && (bS.lawsuit.kind == KIND_2ND_PUNISHMENT_VNAHD_LAW ||
		bS.lawsuit.kind == KIND_2ND_RENEW_ADMCHAR_LAW)
#  else      // AREAL
	  && (bS.lawsuit.kind == KIND_PUNISHMENT_NAHD_LAW ||
		bS.lawsuit.kind == KIND_2ND_PUNISHMENT_KNAHD_LAW ||
		bS.lawsuit.kind == KIND_2ND_PUNISHMENT_VNAHD_LAW )       //   20060717
#  endif
#else        // REGIONAL
	  && bS.lawsuit.kind == KIND_ADMIN_PUNISHMENT_LAW
#endif
	)
    {
	//TRCDKey_(bS.lawsuit.key).PrintAlias(buff, buff.Size());//20050315
	//TRCDKey_(bS.lawsuit.key).PrintShortest(buff, buff.Size());
	TRCDKeyContainer_(bS.lawsuit).PrintAlias(buff, buff.Size());
	pColumn->addText(buff);
    }
  } // -- iC_AdminPunishmentLawsuit --

  if((pColumn = columns[iC_PrivateCitizenLawsuit]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_PrivateCitizenLawsuit + bOC,
      pItems);

	buff[0]='\0';
#if COURT_TYPE == COURT_AREAL
	if (strchr(KIND_PRIVATE_CITIZALIKE_LAWS, bS.lawsuit.kind))
#else  // 20110175
#if INSTANCE
#  if APPEAL // APPEAL
    if (  (bS.lawsuit.kind == KIND_PRIVATE_CITIZEN_LAW)
	  ||(bS.lawsuit.kind == KIND_PRIVATE_CASS_CITIZEN_LAW)  // 20070927
	  ||(bS.lawsuit.kind == KIND_PRIVATE_CASS_TRADE_LAW))   // 20070927
#  else      // AREAL  20050315
    if(bS.lawsuit.key.type == TYPE_ADMIN_LAW
	 && bS.lawsuit.kind == KIND_PRIVATE_ADMIN_LAW)
#  endif
#else        // REGIONAL
    if( (bS.lawsuit.kind == KIND_PRIVATE_CITIZEN_LAW)
	 || (bS.lawsuit.kind == KIND_PRIVATE_ADMIN_LAW))	// 20140112
#endif  // of INSTANCE
#endif  // AREAL
    {
//    TRCDKey_(bS.lawsuit.key).PrintAlias(buff, buff.Size());//20050315
//    TRCDKey_(bS.lawsuit.key).PrintShortest(buff, buff.Size());
	TRCDKeyContainer_(bS.lawsuit).PrintAlias(buff, buff.Size());
	pColumn->addText(buff);
    }
  }  // -- iC_PrivateCitizenLawsuit --


  if((pColumn = columns[iC_PrivatePunishmentLawsuit]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_PrivatePunishmentLawsuit + bOC,
      pItems);
    buff[0]='\0';
    if(
#if INSTANCE
#  if APPEAL // APPEAL
	  bS.lawsuit.key.type == TYPE_PUNISHMENT_LAW && (
	  bS.lawsuit.kind == KIND_2ND_PRIVATE_PUNISHMENT_LAW  ||     //'H'   ВЧНД
	  bS.lawsuit.kind == KIND_1ST_PRIVATE_PUNISHMENT_LAW  ||     //'U'   ЧНД
	  bS.lawsuit.kind == KIND_1ST_RENEW_PRIVATE_PUNISHMENT_LAW)  //']'   ЧНД/въз./
#  else      // AREA
	  bS.lawsuit.key.type == TYPE_PUNISHMENT_LAW && (
	  bS.lawsuit.kind == KIND_PRIVATE_PUNISHMENT_LAW  ||    //'I'   ЧНД
	  bS.lawsuit.kind == KIND_2ND_PRIVATE_PUNISHMENT_LAW )  //'H'   ВЧНД
#  endif
#else        // REGIONAL
      bS.lawsuit.kind == KIND_PRIVATE_PUNISHMENT_LAW
#endif  // of INSTANCE
	)
    {
//    TRCDKey_(bS.lawsuit.key).PrintAlias(buff, buff.Size());//20050315
//    TRCDKey_(bS.lawsuit.key).PrintShortest(buff, buff.Size());
      TRCDKeyContainer_(bS.lawsuit).PrintAlias(buff, buff.Size());
      pColumn->addText(buff);
    }
  }  // -- iC_PrivatePunishmentLawsuit --
  if((pColumn = columns[iC_Side]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_Side + bOC, pItems);
    buff[0]='\0';

    // By idea of TABCBookSide :: output
    if(bS.citizen.ucnType && ucnPresent(bS.citizen.ucn))
    {
      if(citizenUCNAndUCNTypeToName(bS.citizen.ucn, bS.citizen.ucnType,
        buff, buff.Size()))
      {
        pColumn->addText(buff);
      }
    }
    else
      if(bS.firm.ucnType && ucnPresent(bS.firm.ucn))
      {
        if(firmUCNAndUCNTypeToName(bS.firm.ucn, bS.firm.ucnType, buff,
          buff.Size()))
        {
          pColumn->addText(buff);
        }
      }
#if INSTANCE
#  if APPEAL
#  else  // of APPEAL
      else
        if(bS.damn.ucnType && ucnPresent(bS.damn.ucn))
        {
          if(damnUCNAndUCNTypeToName(bS.damn.ucn, bS.damn.ucnType, buff,
            buff.Size()))
          {
            pColumn->addText(buff);
          }
        }
#  endif  // of APPEAL
#else  // of INSTANCE
#endif  // of INSTANCE
  }  // -- iC_Side --
  if((pColumn = columns[iC_Ucn]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_Side + bOC, pItems);
    buff[0]='\0';

    // By idea of TABCBookSide :: output
    if(bS.citizen.ucnType && ucnPresent(bS.citizen.ucn))
    {
      //20050401
	if(PRINT_UCN&&
	   (bS.citizen.ucnType==UCN_CITIZEN_UCN))
      {
           pColumn->addText(bS.citizen.ucn);
      }
    }
    else
      if(bS.firm.ucnType && ucnPresent(bS.firm.ucn))
      {
        pColumn->addText(bS.firm.bulstat);
      }
#if INSTANCE
#  if APPEAL
#  else  // of APPEAL
      else
        if(bS.damn.ucnType && ucnPresent(bS.damn.ucn))
        {
          pColumn->addText(bS.damn.bulstat);
        }
#  endif  // of APPEAL
#else  // of INSTANCE
#endif  // of INSTANCE
  }  // -- iC_Ucn --
  if((pColumn = columns[iC_SerialNo]) != NULL)
  {
    ColumnXmlEncloser encloser(pColumn, iC_SerialNo + bOC, pItems);

    addLongToColumn(pColumn, serialNo);
  }  // -- iC_Side --

}  // TABCBookColumns::MakeColumns

// -------------------- TABCBookColumns - end --------------------

class TABCBookOutFile : public frmHeadOutText
 {
  protected :
    struct bnenyPtrL
     {
	const begNEndNYear * pBneny ;
	char l ;
	char college ;
     } ;  // struct bnenyPtrL
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
  public :
    TABCBookOutFile (TWindow * parent,
			  const char * hFName = NULL) : frmHeadOutText (parent, hFName) { }

    int formattedHead (const begNEndNYear * bnenyPtr, const char l,
			     const char college, bool write = true,
			     ostream* altStreamPtr = NULL  // 15.feb.2005
			    ) ;
    void writeItem (const TABCBookSide & bS, bool first) ;
    void IncLines(int n);  // 15.feb.2005
    void WriteItem(TABCBookColumns& abcC, bool first);  // 15.feb.2005
    void IncItemLines(bool first);  // 15.feb.2005
 } ;  // class TABCBookOutFile

void TABCBookOutFile :: fHOutput (const void * p, char * s,
					   size_t len, int algn,
					   size_t skip, int dCipher)
 {
  const bnenyPtrL * dPtr = reinterpret_cast <const bnenyPtrL *> (p) ;

  if(dPtr)  // 15.feb.2005
  {
    ostream* pStream = pAltStream ? pAltStream : this;  // 15.feb.2005

    if (! (begNEndNYearOutput (* (dPtr -> pBneny), * pStream, s, len,
					 algn, skip, dCipher) ||
	     regionalArealOutput (* pStream, s, len, algn, skip, dCipher)))
	switch (((long int) dCipher) - OFFSET_ABC_BOOK_CIPHER)    // 12000
	 {
	  case 56 :
	    if (len)
	     {
		s [0] = dPtr -> l ;
		s [1] = '\0' ;
		:: writeString (* pStream, s, len, algn, skip) ;
	     }
	    break ;
	  case 57 :                                               // 12057
	    if (len)
	   {
	    // 20070703
	    if (PRINT_COLLEGE)
	    {
		collegeToString (dPtr -> college, s, len + 1) ;
		:: writeString (* pStream, s, len, algn, skip) ;
	     }}
	    break ;
	  case 58 :
	    if (len)
	     {
		ostrstream oss (s, len + 1) ;

		lawTypeToPlural (oss, dPtr -> college) ;
		oss << ends ;
		s [len] = '\0' ;
		:: writeString (* pStream, s, len, algn, skip) ;
	     }
	    break ;
	  default :
	     break ;
	 }
  }
 }  // TABCBookOutFile :: fHOutput

int TABCBookOutFile :: formattedHead (const begNEndNYear * bnenyPtr,
						 const char l, const char college,
						 bool write,
						 ostream* altStreamPtr  // 15.feb.2005
						 )
 {  // An old varianth in VersionArchives\2365\_appname.j --> _abcbook.cpp
  int res = 0;

  if(bnenyPtr)
  {
    bnenyPtrL d ;

    pAltStream = altStreamPtr;
    d.pBneny = bnenyPtr ;
    d.l = l ;
    d.college = college ;
    try
    {
	res = formHead (& d, write) ;
	// Simulate text output - in order to have a rough approximation
	// to the length of the page.
	if(write == false && pAltStream != NULL)
	{
	  pM.incLines(res);
	}
    }
    CATCH_ANYTHING
    pAltStream = NULL;
  }
  return res;
}  // TABCBookOutFile :: formattedHead

void TABCBookOutFile :: writeItem (const TABCBookSide & bS, bool first)
 {
  if (pM.newPageNeed ())
    toBeContinued () ;
  bS.output (* this, first) ;
  pM.incLines (linesOfItem (first)) ;
 }  // TABCBookOutFile :: writeItem

void TABCBookOutFile::IncLines(int n)
{  // 15.feb.2005
  pM.incLines(n);
}

void TABCBookOutFile::IncItemLines(bool first)
{  // 15.feb.2005
  pM.incLines(linesOfItem(first));
}

void TABCBookOutFile::WriteItem(TABCBookColumns& abcC, bool first)
{
  abcC.notConstOutput(*this, first);
}

static  // 26.may.2003
void loadLawsuitsForABC (TIS_ABCLawsuitArray & lawsArr,
				 const begNEndNYear & bneny,
				 const char college)
 {
  char criteria [256] ;

   {
    ostrstream oss (criteria, sizeof (criteria)) ;

    oss
      << "SELECT * FROM T_LAWSUIT "
         "WHERE "
           "F_NO " ;
    if (bneny.endN > 0)
      oss
        << "BETWEEN " << bneny.begN << " AND " << bneny.endN ;
    else
	oss << ">= " << bneny.begN ;
    oss
      << " "
         "AND "
             "F_YEAR = " << bneny.year << " "
         "AND "
# if FOLLOW_LAWSUIT_TYPES
             "F_TYPE"
# else
             "F_COLLEGE"
# endif
                        " = \'" << college << "\'" ;
    oss
      << " ;" << ends ;
   }
  criteria [sizeof (criteria) - 1] = '\0' ;
  if (criteria_finished_ok (criteria, "loadLawsuitsForABC")) //27.feb.2001
   {
    TQuery q (criteria) ;
    TABCLawsuit * pABCLawsuit ;

    lawsArr.Flush () ;
    while (q.Read ())
     {
      pABCLawsuit = new TABCLawsuit ;
      (* pABCLawsuit) << q ;
      lawsArr.Add (pABCLawsuit) ;
     }
   }
 }  // loadLawsuitsForABC

static  // 26.may.2003
void loadSidesForABC (TIS_ABCSideArray & sidArr,
                      const begNEndNYear & bneny,
                      const char college)
 {
  char criteria [512 + 64] ;

   {
    ostrstream oss (criteria, sizeof (criteria)) ;

    oss
      << "SELECT * FROM "
	   "T_SIDE_WIND S, "
         "T_LAWSUIT L "
	   "WHERE "
           "L.F_NO " ;
    if (bneny.endN > 0)
      oss << "BETWEEN " << bneny.begN << " AND " << bneny.endN ;
    else
      oss << ">= " << bneny.begN ;
    oss << " "
         "AND "
           "L.F_YEAR = " << bneny.year << " "
         "AND "
           "L."
# if FOLLOW_LAWSUIT_TYPES
               "F_TYPE"
# else
               "F_COLLEGE"
# endif
                          " = \'" << college << "\' "
         "AND "
           "S.F_NO = L.F_NO "
         "AND "
           "S.F_YEAR = " << bneny.year << " "
         "AND "
           "S.F_TYPE = L.F_TYPE"
	   " AND "
	     "S.F_KIND = L.F_KIND "
	   "AND "
	     "S.F_DATE = L.F_DATE" ;
    oss
//      << "ORDER BY F_NO ;" << ends ;
      << " ;" << ends ;
   }
  criteria [sizeof (criteria) - 1] = '\0' ;
  if (criteria_finished_ok (criteria, "loadSidesFofABC"))  // 27.feb.2001
   {
    TQuery q (criteria) ;
    TABCSide * pABCSide ;

    sidArr.Flush () ;
    while (q.Read ())
     {
      pABCSide = new TABCSide ;
      (* pABCSide) << q ;
      sidArr.Add (pABCSide) ;
     }
   }
 }  // loadSidesForABC

bool composeABCCriteria (const begNLEndNLYear & bnlenly,
                         const char college,
                         char * criteria, const size_t critSz,
                         const char * //lastName
                         , const char * table)
 {
  if (! criteria && critSz > 1 && table)  // 27.feb.2001
    return false ;  // 27.feb.2001

//  char bS [2] ;
  ostrstream oss (criteria, critSz) ;

//  setmem (bS, sizeof (bS), '\0') ;
//  bS [0] = bnlenly.begL ;
  oss
    << "SELECT C.* FROM " << table << " C, "
	 "T_SIDE_WIND S, "
       "T_LAWSUIT L "
	 "WHERE "
          "L.F_NO " ;
  if (bnlenly.endN > 0)
    oss
      << "BETWEEN " << bnlenly.begN << " AND " << bnlenly.endN ;
  else
    oss << ">= " << bnlenly.begN ;
  oss << " "
       "AND "
          "L.F_YEAR = " << bnlenly.year << " "
       "AND "
         "L."
# if FOLLOW_LAWSUIT_TYPES
             "F_TYPE"
# else
             "F_COLLEGE"
# endif
                        " = \'" << college << "\' "
       "AND "
         "S.F_NO = L.F_NO "
       "AND "
//         "S.F_YEAR = L.F_YEAR "
         "S.F_YEAR = " << bnlenly.year << " "
       "AND "
         "S.F_TYPE = L.F_TYPE "
	 "AND "
	   "S.F_KIND = L.F_KIND "
	 "AND "
	   "S.F_DATE = L.F_DATE "
       "AND "
	   "C.F_UCN = S.F_UCN "
       "AND "
         "C.F_UCN_TYPE = S.F_UCN_TYPE "
//       "AND "                                      // In this case the
//         "(C.F_NAME BETWEEN \"" << bS << "\" "     // work is very slow
//                     "AND \"" << lastName << "\")" // if <table> is
                                                     // indexed by F_NAME
      " ;" << ends ;
  criteria [critSz - 1] = '\0' ;
  return criteria_finished_ok (criteria, "composeABCCriteria") ;
 }  // composeABCCriteria

# if INSTANCE  // 16.mar.2004 - begin
#   if APPEAL
#   else  // of APPEAL  // 16.mar.2004 - end
static  // 26.may.2003
bool composeABCDamnCriteria (const begNLEndNLYear & bnlenly,
                             const char college,
                             char * criteria, const size_t critSz)
 {  // 27.feb.2001
  if (! criteria && critSz > 1)
    return false ;

  ostrstream oss (criteria, critSz) ;

  oss
    << "SELECT C.* FROM T_DAMN C, T_LAWSUIT L "
       "WHERE "
          "L.F_NO " ;
  if (bnlenly.endN > 0)
    oss
      << "BETWEEN " << bnlenly.begN << " AND " << bnlenly.endN ;
  else
    oss << ">= " << bnlenly.begN ;
  oss << " "
       "AND "
          "L.F_YEAR = " << bnlenly.year << " "
       "AND "
         "L."
# if FOLLOW_LAWSUIT_TYPES
             "F_TYPE"
# else
             "F_COLLEGE"
# endif
                        " = \'" << college << "\' "
       "AND "
         "C.F_NO = L.F_NO "
       "AND "
         "C.F_YEAR = " << bnlenly.year << " "
       "AND "
         "C.F_TYPE = L.F_TYPE "
      " ;" << ends ;
  criteria [critSz - 1] = '\0' ;
  return criteria_finished_ok (criteria, "composeABCDamnCriteria") ;
 }  // composeABCDamnCriteria

static  // 26.may.2003
void loadDamnsForABC (TIS_ABCDamnArray & damnArr,
                      const begNLEndNLYear & bnlenly,
                      const char college,
    TIS_ABCSideArray & sideArr,  // Dirty hack, because since
             // 23.feb.2001 the firms are not lawsuits' sides
                      const bool checkLett = true)
 {  // 27.feb.2001
  char criteria [512 + 256] ;

  damnArr.Flush () ;
  if (composeABCDamnCriteria (bnlenly, college, criteria,
                              sizeof (criteria)) == false)
    return ;
   {
    TQuery q (criteria) ;
    TABCDamn * pABCDamn ;
    const unsigned char bL = (const unsigned char) bnlenly.begL ;
    const unsigned char eL = (const unsigned char) bnlenly.endL ;

    while (q.Read ())
     {
      pABCDamn = new TABCDamn ;
      (* pABCDamn) << q ;
       {
        const unsigned char l = pABCDamn -> name [0] ;

        if ((checkLett == false) || (bL <= l && l <= eL))
         {
          damnArr.Add (pABCDamn) ;
            {
            TABCSide * pNewABCSide = new TABCSide ;

            (* pNewABCSide) << (* pABCDamn) ;

             // 20060413  pNewABCSide -> involvement = INVOLVEMENT_FIRM ;
             pNewABCSide -> involvement = 'F';

            if (sideArr.Contains (pNewABCSide))
              delete pNewABCSide ;
            else
              sideArr.Add (pNewABCSide) ;
           }
         }
        else
          delete pABCDamn ;
       }
     }
   }
 }  // loadDamnsForABC
#   endif  // of APPEAL  // 16.mar.2004
# endif  // of INSTANCE  // 16.mar.2004


// Look for the loadUCNGroupsForABC function in _TmplFun.h
# if INSTANCE
static  // 26.may.2003
bool isSideForABCBook (TLawsuit * lawsuit, const char involvement,
                       const char companyType)
 {
  if (lawsuit -> key.type != TYPE_FIRM_LAW)
    return lawsuit -> Suitor (involvement) ||
           lawsuit -> Defendant (involvement) ;
  else
// 20060413      if (involvement == INVOLVEMENT_FIRM)
// 20060413      return true ;
// 20060413    else
      if (involvement == 'F' )
         return true ;
      else
      if (involvement == INVOLVEMENT_FIRM_OWNER &&
          companyType == COMPANY_TYPE_SHOPKEEPER)
        return true ;
      else
        return false ;
 }  // isSideForABCBook
# else  // of INSTANCE

static  // 26.may.2003
bool isSideForABCBook (TLawsuit * lawsuit, const char involvement)
 {
  return lawsuit -> Suitor (involvement) ||
         lawsuit -> Defendant (involvement) ;
 }  // isSideForABCBook
# endif  // of INSTANCE

static  // 26.may.2003
bool abcRCDKeyEq (const TRCDKey & a, const TRCDKey & b)
 {
  return TABCRCDKey (a.no, a.year, a.type) ==
         TABCRCDKey (b.no, b.year, b.type) ;
 }  // abcRCDKeyEq

static  // 26.may.2003
bool letterExists (const TIS_ABCLawsuitArray & lawsArr,
                   const TIS_ABCSideArray & sidArr,
                   const TIS_ABCCitizenArray & citArr,
                   const TIS_ABCFirmArray & firmArr,
# if INSTANCE
                   const TIS_ABCDamnArray & damnArr,
# endif  // of INSTANCE
                   const unsigned char lett)
 {
  const unsigned lACount = lawsArr.Count () ;
  const unsigned sACount = sidArr.Count () ;
  unsigned l, s, c, f ;
# if INSTANCE
  unsigned d ;
# endif  // of INSTANCE
  bool found = false ;

  for (l = 0 ; l < lACount && ! found ; l ++)
   {
    if (! sidArr.indexOfFirstMatch ((lawsArr [l]) -> key, s))
      continue ;
    for (s = s ;
         s < sACount && ! found &&
         abcRCDKeyEq ((lawsArr [l]) -> key, (sidArr [s]) -> key) ;
         s ++)
# if INSTANCE
     {
      bool damnFound =  // first, looking in DamnArr for
                        // a damn with any letter
        damnArr.indexOfMatch ((lawsArr [l]) -> key, '\0', d) ;

      found = citArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                   (sidArr [s]) -> ucn, lett, c) ;
      if (! found)
        found = firmArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                      (sidArr [s]) -> ucn, lett, f) ;
      if (! found)
       {
        damnFound =  // if not citizen or firm found
                     // looking for damn with 'lett' letter
          damnArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                (sidArr [s]) -> ucn, lett, d) ;
        if (damnFound)
          found = true ;
       }
      if (found)
        found = isSideForABCBook (lawsArr [l],
                                  (sidArr [s]) -> involvement,
                                  damnFound ?
                                  (damnArr [d]) -> companyType : '\0') ;
     }
# else  // of INSTANCE
      found = (citArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                    (sidArr [s]) -> ucn, lett, c) ||
               firmArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                     (sidArr [s]) -> ucn, lett, f)) &&
              isSideForABCBook (lawsArr [l],
                                (sidArr [s]) -> involvement) ;
# endif  // of INSTANCE
   }
  return found ;
 }  // letterExists

static  // 26.may.2003
long int maxAbcBookLawNo (const TIS_ABCLawsuitArray & lawsArr,
                          const TIS_ABCSideArray & sidArr,
                          const TIS_ABCCitizenArray & citArr,
                          const TIS_ABCFirmArray & firmArr,
# if INSTANCE  // of INSTANCE
                          const TIS_ABCDamnArray & damnArr,
# endif
                          begNLEndNLYear bnlenly)
 {
  long int maxLawNo = 0 ;
  unsigned char uch ;

  uch = bnlenly.begL ;
  while (true)
   {  // -- 3 --
    if (letterExists (lawsArr, sidArr, citArr, firmArr,
# if INSTANCE
                      damnArr,
# endif
                      uch))
     {  // -- 4 --
	const unsigned lACount = lawsArr.Count () ;
	const unsigned sACount = sidArr.Count () ;
      unsigned l, s, c, f ;
# if INSTANCE
      unsigned d ;
# endif

      for (l = 0 ; l < lACount ; l ++)
       {  // -- 5 --
        if (! sidArr.indexOfFirstMatch ((lawsArr [l]) -> key, s))
          continue ;
        for (s = s ;
             s < sACount && abcRCDKeyEq ((lawsArr [l]) -> key,
                                         (sidArr [s]) -> key) ;
             s ++)
         {  // -- 6 --
# if INSTANCE
          bool damnFound =  // first, looking in DamnArr
                            // for damn with any letter
            damnArr.indexOfMatch ((lawsArr [l]) -> key, '\0', d) ;

          if ((citArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                    (sidArr [s]) -> ucn, uch, c) ||
               firmArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                     (sidArr [s]) -> ucn, uch, f) ||
               (damnFound =  // if not citizen or firm found
                             // looking for damn with 'uch' letter
                damnArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                      (sidArr [s]) -> ucn, uch, d)
               ) == true
              ) &&
              isSideForABCBook (lawsArr [l],
                                (sidArr [s]) -> involvement,
                                damnFound ?
                                  (damnArr [d]) -> companyType : '\0')
             )
# else  // of INSTANCE
          if ((citArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                    (sidArr [s]) -> ucn, uch, c) ||
               firmArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                     (sidArr [s]) -> ucn, uch, f)) &&
              isSideForABCBook (lawsArr [l],
                                (sidArr [s]) -> involvement))
# endif  // of INSTANCE
	     {
		TLawsuit lawsuit ;

		lawsuit << lawsArr [l] ;
		if (maxLawNo < lawsuit.key.no)
		  maxLawNo = lawsuit.key.no ;
	     }
	   }  // -- 6 --
	 }  // -- 5 --
     }  // -- 4 --
    if (uch >= (unsigned char) (bnlenly.endL))
	break ;
    uch ++ ;
   }  // -- 3 --
  return maxLawNo ;
 }  // maxAbcBookLawNo

static void abcBookViewBrokenPrint (char *tmpText, char * const offsetsFText, TWindow * pParent, const char * formatFName)
 {
    if (! (formatFName))
	return ;

    static const char * const EXTRACT_ERR = "Грешка при извличането на буква от азбучника" ;
    istrstream offsetsFS (offsetsFText) ;

    int offsSS ;

    offsetsFS.seekg (0, ios :: end) ;

    const long int offsSSEndPos = offsetsFS.tellg () ;

    offsetsFS.seekg (0) ;
    while ((offsSS = offsetsFS.rdstate ()) == 0 &&
		offsetsFS.tellg () < offsSSEndPos)
    {
	 long int begPos = 0 ;
	 long int endPos = 0 ;

	 offsetsFS >> begPos >> endPos ;

	 if (endPos && endPos > begPos)
	 {
	    istrstream src (tmpText) ;
	    ostrstream partFS ;
	    char ch ;
	    long int srcLen ;
	    long int srcPos ;

	    src.seekg (0, ios :: end) ;
	    srcLen = src.tellg () ;
	    src.seekg (begPos) ;
	    while ((srcPos = src.tellg ()) < endPos && src.good ())
	    {
		src.read (& ch, sizeof (ch)) ;
		partFS.write (& ch, sizeof (ch)) ;
	    }
	    partFS.flush () ;

	    const int srcState = src.rdstate () ;
	    const bool srcOK = (! srcState ||
					  ((srcState == ios :: eofbit) &&
					   (endPos == srcLen) &&
					   (srcPos == endPos))
					 ) ;

	    if (! (srcOK ))
		error(EXTRACT_ERR) ;

	    show_text_stream_use_settings (pParent, partFS, formatFName, LEFT_ALIGNMENT);
	 }
    }

    if (offsSS & (~ (ios :: eofbit)))
	error(EXTRACT_ERR) ;
 }  // abcBookViewBrokenPrint

class TAbcBookThread : public TWaitThread
{
public:
	TAbcBookThread(const char clg, begNLEndNLYear& dlgData, const char *fmtFName,
		bool &offsFSGood, TABCBookOutFile &ofAbc, ostream &ofChk, int &chkFLines, ostream &offsFS);

protected:
	virtual void Execute();
	void PrintAnyAbcBook();
	void PrintOneBookSide(const TABCBookSide& bS, int& currMonth, long int& oldOffset);
	void WriteHeader(const char letter);
	void ChangePageOnNewLetter(long int& oldOffset);

	const char college;
	begNLEndNLYear &bnlenly;
	const char *formatFName;
	bool &offsetsFSGood;
	TABCBookOutFile &of;
	ostream &chkOfs;
	int &checkupFLines;
	ostream &offsetsFS;
};

TAbcBookThread::TAbcBookThread(const char clg, begNLEndNLYear &dlgData, const char *fmtFName,
	bool &offsFSGood, TABCBookOutFile &ofAbc, ostream &ofChk, int &chkFLines, ostream &offsFS)
:
	college(clg), bnlenly(dlgData), formatFName(fmtFName), offsetsFSGood(offsFSGood), of(ofAbc), chkOfs(ofChk),
	checkupFLines(chkFLines), offsetsFS(offsFS)
{
}

void TAbcBookThread::Execute()
{
	try
	{
		PrintAnyAbcBook();
	}
	CATCH_ANYTHING
}

void TAbcBookThread::PrintAnyAbcBook()
 {
  TABCBookSide bS (formatFName, wtdl->Parent) ;
  unsigned char uch ;
  bool notPrintedYet = true ;
  TIS_ABCLawsuitArray lawsArr ;
  TIS_ABCSideArray sidArr ;
  TIS_ABCCitizenArray citArr ;
  TIS_ABCFirmArray firmArr ;
# if INSTANCE
  TIS_ABCDamnArray damnArr ;
# endif
  long int oldOffset = 0 ;
  const int maxPBValue = 1000;
  const int loadingEndPBVal = 333;
  char ciDescr[192];
  const char* const loading = "Зареждане на ";
  const char* const dots3 = "...";
  int loadSteps = 3;
  int loadStepsDone = 0;
  // 15.feb.2005 - begin

  if (college != TYPE_FIRM_LAW && bS.firmsRequired ())
    loadSteps ++ ;
# if INSTANCE
  if (college == TYPE_FIRM_LAW)
    loadSteps ++ ;
# endif
  if (! bnlenly.endN)
    loadSteps ++ ;

  wtdl->SetHeader("Азбучник");
  wtdl->PleaseWait();
  wtdl->SetProgressRange(0, maxPBValue);
  wtdl->SetProgress(0);

  offsetsFSGood = true ;

  strCopy(ciDescr, loading, sizeof(ciDescr));         // Gossip service
  strConcat(ciDescr, "дела", sizeof(ciDescr));        // Gossip service
  strConcat(ciDescr, dots3, sizeof(ciDescr));         // Gossip service
  wtdl->SetText(ciDescr);
  loadLawsuitsForABC (lawsArr, bnlenly, college) ;

  if(loadSteps != 0)
    wtdl->SetProgress((++loadStepsDone) * loadingEndPBVal / loadSteps);

  if(wtdl->WantBreak())
    return;                                           // Gossip service

  strCopy(ciDescr, loading, sizeof(ciDescr));         // Gossip service
  strConcat(ciDescr, "страни", sizeof(ciDescr));      // Gossip service
  strConcat(ciDescr, dots3, sizeof(ciDescr));         // Gossip service
  wtdl->SetText(ciDescr);
  loadSidesForABC (sidArr, bnlenly, college) ;
  if(loadSteps != 0)
    wtdl->SetProgress((++loadStepsDone) * loadingEndPBVal / loadSteps);

  if(wtdl->WantBreak())
    return;                                           // Gossip service

  strCopy(ciDescr, loading, sizeof(ciDescr));         // Gossip service
  strConcat(ciDescr, "физически лица",
    sizeof(ciDescr));                                 // Gossip service
  strConcat(ciDescr, dots3, sizeof(ciDescr));         // Gossip service
  wtdl->SetText(ciDescr);
  loadUCNGroupsForABC (citArr, bnlenly, college, bS.citizen) ;
  if(loadSteps != 0)
    wtdl->SetProgress((++loadStepsDone) * loadingEndPBVal / loadSteps);
  if(wtdl->WantBreak())
    return;                                           // Gossip service

  if (college != TYPE_FIRM_LAW && bS.firmsRequired ())
  {
    strCopy(ciDescr, loading, sizeof(ciDescr));       // Gossip service
    strConcat(ciDescr, "юридически лица",
      sizeof(ciDescr));                               // Gossip service
    strConcat(ciDescr, dots3, sizeof(ciDescr));       // Gossip service
    wtdl->SetText(ciDescr);
    loadUCNGroupsForABC (firmArr, bnlenly, college, bS.firm) ;
    if(loadSteps != 0)
	wtdl->SetProgress((++loadStepsDone) * loadingEndPBVal / loadSteps);

    if(wtdl->WantBreak())
      return;                                         // Gossip service
  }
  else
    firmArr.Flush () ;
# if INSTANCE
  if (college == TYPE_FIRM_LAW)
  {
    strCopy(ciDescr, loading, sizeof(ciDescr));       // Gossip service
    strConcat(ciDescr, "фирми", sizeof(ciDescr));     // Gossip service
    strConcat(ciDescr, dots3, sizeof(ciDescr));       // Gossip service
    wtdl->SetText(ciDescr);
#   if APPEAL
#   else
    loadDamnsForABC (damnArr, bnlenly, college, sidArr, false) ;
#   endif  // of APPEAL
    if(loadSteps != 0)
	wtdl->SetProgress((++loadStepsDone) * loadingEndPBVal / loadSteps);
    if(wtdl->WantBreak())
      return;                                         // Gossip service
  }
  else
    damnArr.Flush () ;
# endif
  if (! bnlenly.endN)
  {
    strCopy(ciDescr, "Определяне на последното дело, включено в изхода",
      sizeof(ciDescr));                               // Gossip service
    strConcat(ciDescr, dots3, sizeof(ciDescr));       // Gossip service
    wtdl->SetText(ciDescr);
    bnlenly.endN = maxAbcBookLawNo (lawsArr, sidArr, citArr,
                                    firmArr,
# if INSTANCE
                                    damnArr,
# endif
						bnlenly) ;
    if(loadSteps != 0)
	wtdl->SetProgress((++loadStepsDone) * loadingEndPBVal / loadSteps);
    if(wtdl->WantBreak())
	return;                                         // Gossip service
  }
  wtdl->SetProgress(loadingEndPBVal);
  of.title () ;
  uch = bnlenly.begL ;
  chkOfs << "СПРАВКА ЗА ОТКРИТИТЕ БУКВИ" // Checkup
         << endl << endl ;               // for letters service
  checkupFLines += 2 ;
  // 15.feb.2005 - begin

  int pageNo = 1;
  char pgAttribsTmp[32];
  SimpleStringsArray pageAttribs;

  {
    ostrstream(pgAttribsTmp, sizeof(pgAttribsTmp)) << (pageNo++) << ends;
    ENDING_ZERO(pgAttribsTmp);
    pageAttribs.Add(new SimpleStrings("no", pgAttribsTmp));
  }
  // 15.feb.2005 - end
  while (wtdl->WantBreak() == false)
   {  // -- 3 --
    char letterDescr[32];
    const int lettersTotal =                        // Gossip service
      (unsigned char)(bnlenly.endL) - (unsigned char)(bnlenly.begL) + 1;

    ostrstream(letterDescr, sizeof(letterDescr))
      << "Буква " << (char)uch << " ("
      << (int)(uch - (unsigned char)(bnlenly.begL) + 1) << " от "
      << lettersTotal << ")" << ends;
    ENDING_ZERO(letterDescr);
    strCopy(ciDescr, letterDescr, sizeof(ciDescr));   // Gossip service
    wtdl->SetText(ciDescr);
    if (letterExists (lawsArr, sidArr, citArr, firmArr,
# if INSTANCE
                      damnArr,
# endif
                      uch))
     {  // -- 4 --
	const unsigned lACount = lawsArr.Count () ;
	const unsigned sACount = sidArr.Count () ;
      unsigned l, s, c, f ;
# if INSTANCE
      unsigned d ;
# endif
      int currMonth = 0 ;

	// New page
      if (notPrintedYet)
        notPrintedYet = false ;
      else
       {
	  ChangePageOnNewLetter(oldOffset);
       }
      WriteHeader(uch);
      chkOfs << (char) uch << ": " ;  // Checkup for letters service
	for (l = 0 ; wtdl->WantBreak() == false && l < lACount ; l ++)
	 {  // -- 5 --
	  char letterLawDescr[256];                     // Gossip service
        char lawDescr[128];                           // Gossip service
        char ld[64];                                  // Gossip service
# if INSTANCE
	  bool firmFound = false ;
# endif


	  TRCDKey_((lawsArr [l]) -> key).Print(ld, sizeof(ld));
	  ostrstream(lawDescr, sizeof(lawDescr))
	    << " " << ld << " (" << (l + 1) << " от " << lACount << ")" << ends;
	  ENDING_ZERO(lawDescr);
	  strCopy(letterLawDescr, ciDescr, sizeof(letterLawDescr));
	  strConcat(letterLawDescr, lawDescr,
	    sizeof(letterLawDescr));                    // Gossip service
	  wtdl->SetText(letterLawDescr);
	  if(lettersTotal != 0 && lACount != 0)
	  {
	    const int oneLetterPBSpan =
		(maxPBValue - loadingEndPBVal) / lettersTotal;

	    wtdl->SetProgress(loadingEndPBVal +
		oneLetterPBSpan * (uch - (unsigned char)(bnlenly.begL)) +
		oneLetterPBSpan * l / lACount);
	  }

	  if (! sidArr.indexOfFirstMatch ((lawsArr [l]) -> key,
						    s))
	    continue ;
	  for (s = s ;
		 s < sACount && abcRCDKeyEq ((lawsArr [l]) -> key,
						     (sidArr [s]) -> key) ;
		 s ++)
	   {  // -- 6 --
	    bool citizenFound ;

# if INSTANCE
	    bool damnFound =  // first, looking in DamnArr for
				    // damn with any letter
		damnArr.indexOfMatch ((lawsArr [l]) -> key, '\0',
					    d) ;

	    if (((citizenFound =
		    citArr.indexOfMatch ((sidArr [s]) -> ucnType,
						 (sidArr [s]) -> ucn, uch,
						 c)
		   ) == true ||
		   (firmFound =
                firmArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                      (sidArr [s]) -> ucn, uch,
                                      f)
               ) == true ||
               (damnFound =  // if not citizen or firm found
				     // looking for damn with 'uch' letter
                damnArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                      (sidArr [s]) -> ucn, uch,
                                      d)
               ) == true
              )
              &&
              isSideForABCBook (lawsArr [l],
                                (sidArr [s]) -> involvement,
                                damnFound ?
                                  (damnArr [d]) -> companyType :
                                  '\0'
                               )
		 )
# else  // of INSTANCE
          if (((citizenFound =
                citArr.indexOfMatch ((sidArr [s]) -> ucnType,
						 (sidArr [s]) -> ucn, uch,
                                     c))
                == true ||
                firmArr.indexOfMatch ((sidArr [s]) -> ucnType,
                                      (sidArr [s]) -> ucn, uch,
                                      f))
              &&
              isSideForABCBook (lawsArr [l],
                                (sidArr [s]) -> involvement))
# endif  // of INSTANCE
           {  // write to output file
            bS.clearData () ;
            bS << sidArr [s] ;
            bS.lawsuit << lawsArr [l] ;
# if INSTANCE
		extract_first_decision (* (lawsArr [l]),
						(* (bS.pFirstDamnDec))) ;
# endif  // INSTANCE
		if (citizenFound)
             {
              bS.citizen << citArr [c] ;
# if INSTANCE
              bS.citAddress << bS.citizen ;
              if (! (bS.citAddress.Try ()))
               {
                clearGroupData (bS.citAddress) ;
                bS.citForgnAddress << bS.citizen ;
                if (! (bS.citForgnAddress.Try ()))
                  clearGroupData (bS.citForgnAddress) ;
		   }
# endif
             }
            else
# if INSTANCE
              if (firmFound)
               {  // 16.mar.2000
                bS.firm << firmArr [f] ;
                bS.firmAddress << bS.firm ;
                if (! (bS.firmAddress.Try ()))
                  clearGroupData (bS.firmAddress) ;
               }
              else
		    if (damnFound)  // 16.jun.2000
                  bS.damn << damnArr [d] ;
# else
              bS.firm << firmArr [f] ;
# endif
            // Code separated in this method on 15.feb.2005
		PrintOneBookSide(bS, currMonth, oldOffset);
	     }
         }  // -- 6 --
       }  // -- 5 --
       {  // Checkup for letters service
        chkOfs << of.pageLinesUsed () << " реда" << endl ;
        chkOfs.flush () ;
        if (bnlenly.wantCheckup && ! chkOfs.good ())
          error("Грешка при запис във файла-справка за буквите.") ;
        checkupFLines ++ ;  // Checkup for letters service
       }
     }  // -- 4 --
    if(lettersTotal != 0)
	wtdl->SetProgress(loadingEndPBVal +
	  (maxPBValue - loadingEndPBVal) *
	  (uch + 1 - (unsigned char)(bnlenly.begL)) /
	  lettersTotal);
    if (uch >= (unsigned char) (bnlenly.endL))
      break ;
    uch ++ ;
   }  // -- 3 --
  wtdl->SetProgress(maxPBValue);
  wtdl->AlmostDone();
  // 15.feb.2005
  {
    of.footherAndTail () ;
    of.resetPrintedLines () ;
     {  // Broken print service
	offsetsFS << oldOffset << " " << of.pcount () ;  // No endl !
     }
     {
	const char * const offsFWrtErr =
        "Грешка при запис във временния файл за "
        "местоположенията на буквите/страниците в азбучника" ;

      if (! offsetsFS.good ())
	  offsetsFSGood = error (offsFWrtErr) ;
	offsetsFS.flush () ;
	if (! offsetsFS.good ())
	  offsetsFSGood = error (offsFWrtErr) ;
     }
  }
 }

void TAbcBookThread::PrintOneBookSide(const TABCBookSide& bS, int& currMonth,
  long int& oldOffset)
{  // 15.feb.2004
  const bool monthRowNeed = bS.lawsuit.date.month != currMonth;

  {
    const char * const fullPage = "  Пълна страница, " ;
					   // Checkup for letters service
    if(bnlenly.jan2005Format)
    {
	TABCBookColumns abcClmns(formatFName, wtdl->Parent, 0, bS, NULL);
      const int rowsCnt = abcClmns.getMaxRows();

      if(monthRowNeed  == false)
      {
        if(of.pageLinesLeft() < rowsCnt + 1)
        {
          of.footherAndTail();
          of.resetPrintedLines();
          if(bnlenly.wantPrintByPages)
          {  // Broken print service
		const long int newOffset = of.pcount();

            offsetsFS << oldOffset << " " << newOffset << endl;
            oldOffset = newOffset;
          }
          of.head();
          chkOfs << endl << fullPage;  // Checkup for letters service
          checkupFLines++;  // Checkup for letters service
	  }
        of.horzLine();
      }
      else
      {
        if(of.pageLinesLeft() < of.linesOfItem(false) + 1 + rowsCnt + 1)
        {
          of.footherAndTail();
	    of.resetPrintedLines();
          if (bnlenly.wantPrintByPages)
          {  // Broken print service
		const long int newOffset = of.pcount();

		offsetsFS << oldOffset << " " << newOffset << endl;
            oldOffset = newOffset;
          }
          of.head();
	    chkOfs << endl << fullPage;  // Checkup for letters service
          checkupFLines++;  // Checkup for letters service
        }
	  of.mediumFootherLine();
        of.writeItem(bS, false);  // I especially do not use abcClmns here.
          // This way is simpler
        of.mediumTopLine();
        //currMonth = bS.lawsuit.date.month;
      }
      for(int i = 0; i < rowsCnt; i++)
      {
        of.WriteItem(abcClmns, true);
      }
    }
    else
    {
      if (monthRowNeed  == false)
       {
        if (of.pageLinesLeft () < of.linesOfItem () + 1)
         {
          of.footherAndTail () ;
          of.resetPrintedLines () ;
          if (bnlenly.wantPrintByPages)
           {  // Broken print service
		const long int newOffset = of.pcount () ;

            offsetsFS << oldOffset << " " << newOffset << endl ;
            oldOffset = newOffset ;
           }
	    of.head () ;
          chkOfs << endl << fullPage ;  // Checkup for letters service
          checkupFLines ++ ;  // Checkup for letters service
         }
        of.horzLine () ;
       }
      else
      {
	  if (of.pageLinesLeft() <
            of.linesOfItem(false) + 1 + of.linesOfItem() + 1)
        {
          of.footherAndTail () ;
          of.resetPrintedLines () ;
	    if (bnlenly.wantPrintByPages)
          {  // Broken print service
		const long int newOffset = of.pcount () ;

		offsetsFS << oldOffset << " " << newOffset << endl ;
            oldOffset = newOffset ;
          }
          of.head () ;
          chkOfs << endl << fullPage ;  // Checkup for letters service
          checkupFLines ++ ;  // 10.may.2000 Checkup for letters service
        }
        of.mediumFootherLine () ;
        of.writeItem (bS, false) ;
        of.mediumTopLine () ;
        //currMonth = bS.lawsuit.date.month ;
      }
      of.writeItem (bS, true) ;
    }
  }
  if(monthRowNeed)
  {
    currMonth = bS.lawsuit.date.month ;
  }
}

void TAbcBookThread::WriteHeader(const char letter)
{  // 15.feb.2005
  {
    of.formattedHead(&bnlenly, letter, college);
    of.head();
  }
}

void TAbcBookThread::ChangePageOnNewLetter(long int& oldOffset)
{  // 15.feb.2005
  {
    of.footherAndTail();
    of.resetPrintedLines();
    {  // Broken print service
	const long int newOffset = of.pcount () ;

	offsetsFS << oldOffset << " " << newOffset << endl ;
	oldOffset = newOffset ;
    }
  }
}

static void print_thread_abc_book(TWindow *pParent, const char college, begNLEndNLYear &bnlenly, const char *formatFName)
{
	int checkupFLines = 0;
	bool offsetsFSGood = false;

	TABCBookOutFile of(pParent, formatFName);
	ostrstream chkOfs;
	ostrstream offsetsFS;

	try
	{
		TWaitThreadDialog wtdl;
	#if SPEEDTESTING
		#pragma message speed testing only
		DWORD ticks = GetTickCount();
	#endif  // SPEEDTESTING
		TAbcBookThread(college, bnlenly, formatFName, offsetsFSGood, of, chkOfs, checkupFLines, offsetsFS).ExecWait();
	#if SPEEDTESTING
		message("test version: %lu d-seconds\n", (GetTickCount() - ticks) / 100);
	#endif  // SPEEDTESTING
	}
	CATCH_ANYTHING

	chkOfs << (char) 12 << endl;
	chkOfs.flush();
	checkupFLines++;

	if (bnlenly.wantCheckup)
	{
		const int pagelen = DEFAULT_PAGE_LEN;

		if (++checkupFLines < pagelen)
			checkupFLines = pagelen;

		show_text_stream_use_settings (NULL, chkOfs, formatFName, LEFT_ALIGNMENT);
	}

	if (bnlenly.wantBrokenPrint || bnlenly.wantPrintByPages)
	{
		char *tmpText = gettext(of);
		char *offsetsFText = gettext(offsetsFS);

		abcBookViewBrokenPrint(tmpText, offsetsFText, pParent, formatFName);
		delete[] offsetsFText;
		delete[] tmpText;
	}
	else
		show_text_stream_use_settings(NULL, of, formatFName, LEFT_ALIGNMENT);
}

static bool make_abc_book_format_fname(char* const fmtFName, const size_t fFNSz,
  begNLEndNLYear& bnlenly, const char lawType)
{  // 16.feb.2005
  FUNC_NAME("make_abc_book_format_fname");
  bool res = false;

  if(fmtFName != NULL && fFNSz > 1 && lawType != '\0')
  {
    const char* suffix = "col";

    // We have to support the following formats:
    // - the old book format as text file
    // - the new book format as text file
    if(bnlenly.jan2005Format)
    { // the new books format
	// New formats are available only in text   20050314
	suffix = "_t_";
    }

    ostrstream(fmtFName, fFNSz)
	<< "text\\abcb" << lawType << suffix
	<< DEFAULT_FORMAT_FILE_EXTENSION << ends;
    fmtFName[fFNSz - 1] = '\0';
    if(strlen(fmtFName) + 1 < fFNSz)
    {
	res = true;
    }
    if(res == false)
	REPORT_F_UNSUCCESSFUL;
  }
  else
    REPORT_F_INVALID_ARGS;
  return res;
}  // make_abc_book_format_fname

#include "_prabcb_.h"
#include "_prabcbk.cpp"

void print_abc_book(TWindow *parent)
{
	begNLEndNLYear bnlenly;
	char cC;
	char fmtFName[128];

	if (TPrintAbcBookDialog(parent, &bnlenly, &cC).Execute() == IDOK)
	{
		if (make_abc_book_format_fname(fmtFName, sizeof(fmtFName), bnlenly, cC))
		{
			register_usage(REGIME_ABC_BOOK, cC, 1, "Азбучник");
			print_thread_abc_book(parent, cC, bnlenly, fmtFName);
		}
	}
}
