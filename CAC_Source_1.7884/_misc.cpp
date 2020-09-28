#include "cac.h"

stringToSQLCharSet :: stringToSQLCharSet (const char * str) :
                        rSBuff (NULL)
 {
  const size_t bSz = 4 * (str ? strlen (str) : 0) ;
  size_t i ;

  if (bSz)
   {  // -- 0 --
    rSBuff = new char [bSz] ;

    if (rSBuff)
     {  // -- 1 --
      setmem (rSBuff, bSz, '\0') ;
       {
        ostrstream oss (rSBuff, bSz) ;

        for (i = 0 ; i < (bSz / 4) ; i ++)
          oss << '\'' << str [i] << "\'," ;
        rSBuff [bSz - 1] = '\0' ;
       }
     }  // -- 1 --
   }  // -- 0 --
  else
   {
    rSBuff = new char [2] ;

    if (rSBuff)
      rSBuff [0] = '\0' ;
   }
 }  // stringToSQLCharSet :: stringToSQLCharSet

stringToSQLCharSet :: ~stringToSQLCharSet ()
 {
  if (rSBuff)
    delete [] rSBuff ;
 }  // stringToSQLCharSet :: ~stringToSQLCharSet

const char * stringToSQLCharSet :: getTheSet () const
 {
  return rSBuff ? rSBuff : "" ;
 }  // stringToSQLCharSet :: getTheSet

SQLCharSet :: SQLCharSet (const char * str) :
                stringToSQLCharSet (str)
 {
 }  // SQLCharSet :: SQLCharSet

ostream & operator << (ostream & os, const SQLCharSet & cS)
 {
  os << cS.getTheSet () ;
  return os ;
 }

doubleStrikedString :: doubleStrikedString (const char * const str,
                                            const bool uCC) :
                         s (str ? str : ""), upCCyr (uCC)
 {
 } ;  // doubleStrikedString :: doubleStrikedString

// TIS_ABCCitizenArray implementation moved to _abcbook.cpp on 13.jan.2004
// TIS_ABCFirmArray implementation moved to _abcbook.cpp on 13.jan.2004
// TIS_ABCDamnArray implementation moved to _abcbook.cpp on 13.jan.2004

ostream & operator << (ostream & strm, const CDate d)
 {
  char oldFill = strm.fill ('0') ;

  strm << setw (2) << (int) d.day << '.'
       << setw (2) << (int) d.month << '.'
       << setw (4) << d.year ;
  strm.fill (oldFill) ;
  return strm ;
 }  // ostream & operator <<

ostream & operator << (ostream & strm, const doubleStrikedString s) 
 {
  const size_t l = strlen (s.s) ;

  for (size_t i = 0 ; i < l ; i ++)
   {
    const char c = s.upCCyr ? upCyr (s.s [i]) : s.s [i] ;

    strm << c << '\b' << c ;
   }
  return strm ;
 }  // ostream & operator <<

ostream & operator << (ostream & strm, const CTime t)
 {
  char oldFill = strm.fill ('0') ;

  strm << setw (2) << (int) t.hour << ':'
       << setw (2) << (int) t.minute ;
  strm.fill (oldFill) ;
  return strm ;
 }  // ostream & operator << (ostream & strm, const CTime t)
 
// --------------------------------------------------------------------

int scVSprintf (size_t buffSz, char * buff, const char * format,
		va_list arglist)
 {
  if (buffSz)
   {
    int res ;
    const long int bbLSz = buffSz + 512 ;
    const size_t bigBuffSz = (size_t) bbLSz ;
    char * bigBuff = new char [bigBuffSz] ;

    if (bbLSz != bigBuffSz)
     {  // I hope, this will never be displayed
      fatal ("scSprintf : target buffer too long") ;
      return -1 ;
     }
    if (! bigBuff)
     {
      fatal ("No memory") ;
      return -1 ;
     }
    bigBuff [bigBuffSz - 1] = '\0' ;
    res = vsprintf (bigBuff, format, arglist) ;
    if (bigBuff [bigBuffSz-1])
      fatal ("scSprintf :  buffer overflow") ;
    bigBuff [buffSz - 1] = '\0' ;
    strcpy (buff, bigBuff) ;
    delete [] bigBuff ;
    return res ;
   }
  else
    return -1 ;
 }  // scVSprintf

int scSprintf (size_t buffSz, char * buff, const char * format, ...)
 {
  va_list ap;
  int res ;

  va_start (ap, format) ;
  res = scVSprintf (buffSz, buff, format, ap) ;
  va_end (ap) ;
  return res ;
 }  // scSprintf

bool referenceToFmtFile(char *fmtFN, size_t fmtFNSz)
{
	static constant referencesFName = "text\\missing.fmt";
	bool res = false;
	size_t fmtFNLen;

	if (fmtFN && fmtFNSz && ((fmtFNLen = strlen (fmtFN)) != 0))
	{  // -- 0 --
		size_t buffSz = SIZE_OF_LINE;
		char *buff = new char[buffSz];
		ifstream ifs(referencesFName);
		bool endSectionOccured = false;

		if (buff && ifs.good() && findSection(ifs, referencesFName, "[MISSING_FILES]"))
		{ // -- 0a --
			while (!res && ifs.good() && endSectionOccured == false)
			{  // -- 1 --
				ifs.getline (buff, buffSz);

				if ((ifs.good() || ifs.rdstate() == ios::eofbit) && (endSectionOccured = endSection(buff)) == false)
				{  // -- 1a --
					{
						char *semicolonPtr = strchr(buff, ';');

						if (semicolonPtr)
							(*semicolonPtr) = '\0';
					}

					if (!strncmpi(buff, fmtFN, fmtFNLen) && (buff[fmtFNLen] == ' ' || buff[fmtFNLen] == '='))
					{  // -- 2 --
						char *eqPtr = strchr (buff + fmtFNLen, '=');

						if (eqPtr && eqPtr[1])
						{  // -- 3 --
							eqPtr = &(eqPtr[1]);

							while (eqPtr [0] == ' ')
								eqPtr = &(eqPtr[1]);

							{  // -- 4 --
								size_t l = strlen (eqPtr);

								if (l && l < fmtFNSz)
								{  // -- 5 --
									strcpy (fmtFN, eqPtr) ;
									res = true ;
								}  // -- 5 --
							}  // -- 4 --
						}  // -- 3 --
					}  // -- 2 --
				}  // -- 1a --
			}  // -- 1 --

			if (buff)
				delete[] buff;
		} // -- 0a --
	}  // -- 0 --

	if (!res)
	{
		size_t sz = strlen(fmtFN) + 128;
		char *m;

		if ((m = new char [sz]) != NULL)
		{
			scSprintf (sz, m, "%s :  файлът липсва. Не мога да открия и "
				"препратка към друг форматен файл (виж %s)", fmtFN, referencesFName);

			error("%s", m);
			delete[] m;
		}
	}

	return res ;
}

bool cannotOpenFmtFile(const char *fN)
{
	if (fN)
	{
		static constant pref = "Не мога да отворя форматния файл ";
		const size_t lSz = strlen (pref) + strlen (fN) + 10;
		char *const l = new char[lSz];

		scSprintf(lSz, l, "%s%s", pref, fN);
		error("%s", l);
		delete []l;
	}

	return false ;
}

void elapsed_24h_time_nessage (const CTime start, const CTime end)
 {
  const int sixty = 60 ;
  const long int startSec = start.second + start.minute * sixty +
                            start.hour * sixty * sixty ;
  const long int endSec = end.second + end.minute * sixty +
                          end.hour * sixty * sixty ;
  long int secondsElapsed = 0 ;

  if (end < start)
    secondsElapsed = 24 * sixty * sixty ;
  secondsElapsed += endSec ;
  secondsElapsed -= startSec ;

  CTime elapsedTime;

   {
    long int tmpElapsedSec = secondsElapsed ;

    elapsedTime.second = (char) (tmpElapsedSec % sixty) ;
    tmpElapsedSec -= elapsedTime.second ;
    tmpElapsedSec /= sixty ;
    elapsedTime.minute = (char) (tmpElapsedSec % sixty) ;
    tmpElapsedSec -= elapsedTime.minute ;
    tmpElapsedSec /= sixty ;
    elapsedTime.hour = (char) tmpElapsedSec ;
   }
  info ("Начало: %02dч.%02dм.%02dс.\n"
        "Край: %02dч.%02dм.%02dс.\n"
        "Изразходвано време: %dч.%dм.%dс.  (%ldс.)",
        start.hour, start.minute, start.second,
	  end.hour, end.minute, end.second,
        elapsedTime.hour, elapsedTime.minute, elapsedTime.second,
        secondsElapsed
       ) ;
    // message --> info  30.mar.2001
 }  // elapsed_time_nessage

#if COURT_TYPE == COURT_APPEAL
int judges_count_expecting (const char lawKind, const char sessKind,
                            const bool ret0ForClosed)
 {
  const char * const funcName = "judges_count_expecting" ;

  if (! (lawKind && sessKind))
   {
    error ("%s: wrong arguments are passed.", funcName) ;
    return 0 ;
   }

  const bool closed = strchr (KIND_CLOSED_SESSIONS, sessKind) ;

  if (closed && ret0ForClosed)
    return 0 ;
  if (strchr (KIND_PUNISHMENT_LAWS, lawKind))
    return closed ? 3 : 3 ;
  if ((strchr (KIND_CITIZEN_LAWS, lawKind)) ||
      (strchr (KIND_RECITIZEN_LAWS, lawKind))        // 20070928
     )
    return closed ? 3 : 3 ;
  if ((strchr (KIND_TRADE_LAWS, lawKind))  ||
      (strchr (KIND_RETRADE_LAWS, lawKind))          // 20070928
     )
    return closed ? 3 : 3 ;
   {  // What else?

    TKind k ;

    k.kind = lawKind ;
    if (! k.Get ())
      clearGroupData (k) ;
    error ("%s: The \"%s\" is not processed yet.", funcName, k.name) ;
    return 0 ;
   }
 }  // judges_count_expecting

#endif  // of APPEAL

#if COURT_TYPE == COURT_AREAL
int judges_count_expecting (const char lawKind, const char sessKind,
				    const bool ret0ForClosed)
 {
  const char * const funcName = "judges_count_expecting" ;

  if (! (lawKind && sessKind))
   {
    error ("%s: wrong arguments are passed.", funcName) ;
    return 0 ;
   }

  const bool closed = strchr (KIND_CLOSED_SESSIONS, sessKind) ;

  if (closed && ret0ForClosed)
    return 0 ;
  if (strchr (KIND_1ST_PUNISHMENT_LAWS, lawKind))
    return closed ? 1 : 0 ;  // 2 ;   // because the law allows 1 or 2
  if (strchr (KIND_1ST_CITIZEN_LAWS, lawKind))
    return closed ? 1 : 1 ;

  if (strchr (KIND_1ST_TRADE_LAWS, lawKind))
    return closed ? 1 : 1 ;	// 2007:131 LPR: Huh? Nice...

  if (strchr (KIND_1ST_ADMIN_LAWS, lawKind))
	return closed ? 1 : 3 ;
  if (strchr (KIND_FIRM_LAWS, lawKind))
    return closed ? 1 : 1 ;

  if (strchr (KIND_2ND_PUNISHMENT_LAWS, lawKind))
    return closed ? 3 : 3 ;
  if (strchr (KIND_2ND_CITIZEN_LAWS, lawKind))
    return closed ? 1 : 3 ;

  if (strchr (KIND_2ND_TRADE_LAWS, lawKind))
    return closed ? 3 : 3 ;

  if (strchr (KIND_2ND_ADMIN_LAWS, lawKind))
    return closed ? 3 : 3 ;

   {  // What else?
    TKind k ;

    k.kind = lawKind ;
    if (! k.Get ())
	clearGroupData (k) ;
    error ("%s: The \"%s\" is not processed yet.", funcName, k.name) ;
    return 0 ;
   }
 }  // judges_count_expecting
#endif  // AREAL

#if COURT_TYPE == COURT_ADMIN    // 20071004 -->
int judges_count_expecting (const char lawKind, const char sessKind,
				    const bool ret0ForClosed)
 {
  const char * const funcName = "judges_count_expecting" ;

  if (! (lawKind && sessKind))
   {
    error ("%s: wrong arguments are passed.", funcName) ;
    return 0 ;
   }
  const bool closed = strchr (KIND_CLOSED_SESSIONS, sessKind) ;

  if (closed && ret0ForClosed)
    return 0 ;

  if (strchr (KIND_1ST_PUNISHMENT_LAWS, lawKind))
    return closed ? 1 : 0 ;  // 2 ;   // because the law allows 1 or 2
  if (strchr (KIND_1ST_CITIZEN_LAWS, lawKind))
    return closed ? 1 : 1 ;
  if (strchr (KIND_1ST_ADMIN_LAWS, lawKind))
    return closed ? 1 : 1 ;
  if (strchr (KIND_1ST_LAWSUITS, lawKind))
    return closed ? 1 : 1 ;


  if (strchr (KIND_2ND_PUNISHMENT_LAWS, lawKind))
    return closed ? 3 : 3 ;
  if (strchr (KIND_2ND_CITIZEN_LAWS, lawKind))
    return closed ? 1 : 3 ;
  if (strchr (KIND_2ND_ADMIN_LAWS, lawKind))
    return closed ? 3 : 3 ;
  if (strchr (KIND_2ND_LAWSUITS, lawKind))
    return closed ? 3 : 3 ;
  if (strchr (KIND_TRICKY_LAWS, lawKind))
    return closed ? 3 : 3 ;

   {  // What else?
    TKind k ;

    k.kind = lawKind ;
    if (! k.Get ())
      clearGroupData (k) ;
    error ("%s: The \"%s\" is not processed yet.", funcName, k.name) ;
    return 0 ;
   }
 }  // judges_count_expecting
#endif  // ADMIN                       // 20071004 <--

#if COURT_TYPE == COURT_MILITARY
int judges_count_expecting (const char lawKind, const char sessKind,
				    const bool ret0ForClosed)
 {
  const char * const funcName = "judges_count_expecting" ;

  if (! (lawKind && sessKind))
   {
    error ("%s: wrong arguments are passed.", funcName) ;
    return 0 ;
   }

  const bool closed = strchr (KIND_CLOSED_SESSIONS, sessKind) ;

  if (closed && ret0ForClosed)
    return 0 ;
  if (strchr (KIND_1ST_PUNISHMENT_LAWS, lawKind))
    return closed ? 1 : 0 ;  // 2 ;   // because the law allows 1 or 2
  if (strchr (KIND_1ST_CITIZEN_LAWS, lawKind))
    return closed ? 1 : 1 ;

  if (strchr (KIND_2ND_PUNISHMENT_LAWS, lawKind))
    return closed ? 3 : 3 ;
  if (strchr (KIND_2ND_CITIZEN_LAWS, lawKind))
    return closed ? 1 : 3 ;

   {  // What else?
    TKind k ;

    k.kind = lawKind ;
    if (! k.Get ())
	clearGroupData (k) ;
    error ("%s: The \"%s\" is not processed yet.", funcName, k.name) ;
    return 0 ;
   }
 }  // judges_count_expecting
#endif  // MILITARY

#if COURT_TYPE == COURT_REGIONAL
int judges_count_expecting (const char lawKind, const char sessKind,
				    const bool ret0ForClosed)
 {
  const char * const funcName = "judges_count_expecting" ;

  if (! (lawKind && sessKind))
   {
    error ("%s: wrong arguments are passed.", funcName) ;
    return 0 ;
   }

  const bool closed = strchr (KIND_CLOSED_SESSIONS, sessKind) ;

  if (closed && ret0ForClosed)
    return 0 ;
  if (strchr (KIND_PUNISHMENT_LAWS, lawKind))
    return closed ? 1 : 1 ;
  if (strchr (KIND_CITIZEN_LAWS, lawKind))
    return closed ? 1 : 1 ;
  if (strchr (KIND_MARRIAGE_LAWS, lawKind))
    return closed ? 1 : 1 ;
  // 2016:202 FIX: was missing
  if (strchr (KIND_ADMIN_LAWS, lawKind))
    return closed ? 1 : 1 ;
   {  // What else?
    TKind k ;

    k.kind = lawKind ;
    if (! k.Get ())
	clearGroupData (k) ;
    error ("%s: The \"%s\" is not processed yet.", funcName, k.name) ;
    return 0 ;
   }
 }  // judges_count_expecting
#endif  // REGIONAL

int judges_in_session (const BriefSessionsToCheckGroup & sess)
 {
  int count = 0 ;

  if (ucnPresent (sess.president))
    count ++ ;
  if (ucnPresent (sess.judgeX))
    count ++ ;
  if (ucnPresent (sess.reJudgeX))
    count ++ ;
  return count ;
 }  // judges_in_session

void CharPtr :: NewData (const size_t size_)
 {
  size = size_ ;
  s = new char [size] ;
  setmem (s, size, '\0') ;
 }  // CharPtr :: NewData

void CharPtr :: EmptyData ()
 {
  size = 0 ;
  if (s)
   {
    delete [] s ;
    s = NULL ;
   }
 }  // CharPtr :: EmptyData

CharPtr :: CharPtr (const size_t size_)
 {
  if (size_ == 0)
    error("%s\n%s::%s", WRONG_ARGS_PASSED_TO,
          typeid(*this).name(), typeid(*this).name());
  NewData (size_ ? size_ : 1) ;
 }  // CharPtr :: CharPtr

CharPtr :: CharPtr (const CharPtr & source)
 {
  (* this) = source ;
 }  // CharPtr :: CharPtr

CharPtr :: CharPtr (const char * const str)
 {
  const char * const ptr = str ? str : "" ;

  NewData (strlen (ptr) + 1) ;
  strcpy (s, ptr) ;
 }  // CharPtr :: CharPtr

const CharPtr & CharPtr :: operator = (const CharPtr & source)
 {
  if (this != & source)
   {
    EmptyData () ;
    NewData (source.size) ;
    memmove (s, source.s, size) ;
   }
  return (* this) ;
 }  // CharPtr :: operator =

size_t CharPtr :: Size () const
 {
  return size ;
 }  // CharPtr :: Size

char * const CharPtr :: Str () const
 {
  return s ;
 }  // CharPtr :: Str

CharPtr :: operator char * const ()
 {
  return s ;
 }  // CharPtr :: operator char * const

void CharPtr :: End ()
 {  // 18.feb.2003
  if (s && size)
    s[size - 1] = '\0' ;
 }  // CharPtr :: End ()

bool CharPtr :: operator == (const CharPtr & other) const
 {
  if (this == & other)
    return true ;
  else
    if (strcmp (s, other.s))
      return false ;
    else
      return true ;
 }  // CharPtr :: operator ==

JustifiedString :: JustifiedString (const char * str, size_t width) :
  CharPtr (width + 1 ? width + 1 : 1)
 {
  ostrstream (Str (), Size ()) << (str ? str : "") << ends ;
  Str () [Size () - 1] = '\0' ;
  Justify () ;
 }  // JustifiedString :: JustifiedString

void JustifiedString :: Justify ()
 {
  CharPtr courier_ (Size ()) ;
  char * courier = courier_.Str () ;
  CharPtr src_ (Size ()) ;
  char * const src = src_.Str () ;
  char * const dest = Str () ;
  const size_t rowSz = Size () ;
  long int pos = 0L, pos1 ;
  int tmpLen ;

  strDelLeadingSpaces (dest) ;
  strDelLastSpaces (dest) ;
  ostrstream (src, rowSz) << dest << ends ;
  src [rowSz - 1] = '\0' ;
  if (strchr (src, ' ') && strlen (src) + 1 < rowSz)
   {  // To ensure that there are at least two words in the string
    do
     {  // -- 0 --
      ostrstream (src, rowSz) << dest << ends ;
      src [rowSz - 1] = '\0' ;
      dest [(int) pos] = '\0' ;
       {
        istrstream iss (src, rowSz) ;
        ostrstream oss (dest, rowSz) ;

        iss.seekg (pos) ;
        oss.seekp (pos) ;
        iss >> courier ;
        pos1 = iss.tellg () ;
        if (pos1 > (tmpLen = strlen (src)))
          pos1 = tmpLen ;
        if (pos > 0)
          oss << setw ((int) (pos1 - pos) - strlen (courier) + 1) << ' ' ;
        oss << courier ;
        pos = iss.tellg () ;
        if (pos < rowSz - 1)
         {
          iss.get (courier, rowSz, '\0') ;
          oss << courier ;
          pos ++ ;
         }
        else
          pos = 0L ;
       }
      dest [rowSz - 1] = '\0' ;
     }  // -- 0 --
    while (strlen (dest) < rowSz - 1) ;
   }  // if (strchr (src, ' '))
 }  // JustifiedString :: Justify

ostream & operator << (ostream & os, const JustifiedString & jS)
 {
  os << jS.Str () ;
  return os ;
 }  // ostream & operator << (ostream & os, const JustifiedString & jS)

bool cannot_load_format_info (const char * fmtS)
 {
  if (fmtS == NULL)
    fmtS = "" ;
  return error ("%s - невалиден код \'%.*s\'",
                CANNOT_LOAD_THE_FORMAT_INFO,
                strlen (fmtS) < 70 ? strlen (fmtS) : 70, fmtS) ;
 }  // cannot_load_format_info

bool cannot_load_fmt_file_name_data (const char * const docName)
 {
  return error ("Неуспешно извличане на данните, необходими за "
                "съставянето на името на форматния файл за %s",
                docName ? docName : "") ;
 }  // cannot_load_fmt_file_name_data

int cmp_str(const char* const a, const char* const b)
{  // 04.mar.2003
  int res = 0;

  if(a != b)
  {
    if(a == NULL)
	res = -1;
    else
	if(b == NULL)
	  res = 1;
	else
	{
	  const int c = strcmp(a, b);

	  if(c < 0)
	    res = -1;
	  else
	    if(c > 0)
		res = 1;
	}
  }
  return res;
}  // cmp_str

void court_logo_image_file(ostream& os)
{
  FUNC_NAME("court_logo_image_file");
  bool res = false;
  const char* fNames[] =
  {
    "image\\court\\logo.bmp",
    "image\\court\\logo.gif",
    "image\\court\\logo.jpg"
  };
  int ind = -1;

//  os << "image\\" << DEFAULT_UCLP << "\\crt_logo.gif";
  for(int i = 0; ind == -1 && i < (sizeof(fNames) / sizeof(fNames[0])); i++)
  {
    if(access(fNames[i], 0) == 0)
    {
      ind = i;
    }
  }
  if(ind == -1)
  {
    ind = 0;
  }
  if(0 <= ind && ind < (sizeof(fNames) / sizeof(fNames[0])))
  {
    os << fNames[ind];
    if(os.good())
    {
      res = true;
    }
  }
  if(res == false)
    REPORT_F_UNSUCCESSFUL;
} // court_logo_image_file

char *gettext(ostrstream &oss)
{
	oss.flush();
	char *iText = oss.str();
	int iTextLen = oss.pcount();
	char *tText = new char[iTextLen + 1];

	memcpy(tText, iText, iTextLen);
	delete[] iText;
	tText[iTextLen] = '\0';
	return tText;
}

char *getcrlf(const char *tText)
{
	int iLen = strlen(tText);

	for (const char *s = tText; *s; s++)
		if (*s == '\n')
			iLen++;

	char *iText = new char[iLen + 1];
	char *p = iText;

	for (const char *s = tText; *s; s++)
	{
		if (*s == '\n')
			*p++ = '\r';

		*p++ = *s;
	}

	*p = '\0';
	return iText;
}
