#include "cac.h"  // 18.may.2001

void column :: strToRows (const char * str, int leadingBlanks)
 {
  const size_t lenStr = strlen (str) + 2 ;
  char * txt = new char [lenStr + 1] ;

  if (! (cWidth && str))
    return ;
  if (leadingBlanks >= cWidth || leadingBlanks < 0)
    leadingBlanks = 0 ;
  ostrstream (txt, lenStr + 1) << str << " ?" << ends ;
  txt [lenStr] = '\0' ;
   {  // -- 0 --
    char * oneWord = new char [cWidth + 1] ;
    char * row = new char [cWidth + 1] ;
    ostrstream oss (row, cWidth) ;  // cWidth !
    istrstream iss (txt, strlen (txt)) ;
    bool end = false ;
    bool blankIsFirst = (txt [0] == ' ') ;

    setmem (row, cWidth + 1, '\0') ;  // cWidth + 1 !
     {
      for (int i = 0 ; i < leadingBlanks ; i ++)
        oss << ' ' ;
     }
    while (! end)
     {  // -- 1 --
      if (blankIsFirst)
       {  // Skip leading blanks
        long int gP1 = iss.tellg () ;

        blankIsFirst = false ;
        while (txt [gP1] == ' ' && gP1 < (lenStr - 1))
          gP1 ++ ;
        iss.seekg (gP1) ;
       }
      oneWord [0] = '\0' ;
      if (iss.good () && (iss.tellg () < (lenStr - 1)))
       {
        iss.getline (oneWord, cWidth + 1 - leadingBlanks, ' ') ;
        if (strlen (oneWord) == cWidth - leadingBlanks)
          iss.clear () ;
        if (txt [iss.tellg ()] == ' ')
          blankIsFirst = true ;
       }
      oss.clear () ;
      if (oss.tellp () + strlen (oneWord) <= cWidth)
       {  // -- 2 --
        if (oneWord [0])
          oss << oneWord << ' ' ;  // it is not critically if the ' '
                                   // is not added
        else
         {  // -- 3 --
          if (row [0])
           {
            strData.Add (strnew (row)) ;
            setmem (row, cWidth + 1, '\0') ;  // cWidth + 1 !
            oss.clear () ;
            oss.seekp (0) ;
             {
              for (int i = 0 ; i < leadingBlanks ; i ++)
                oss << ' ' ;
             }
           }
          end = true ;  // Nothing is read
         }  // -- 3 --
       }  // -- 2 --
      else
       {  // -- 4 --
        if (row [0])
          strData.Add (strnew (row)) ;
        setmem (row, cWidth + 1, '\0') ;  // cWidth + 1 !
        oss.clear () ;
        oss.seekp (0) ;
         {
          for (int i = 0 ; i < leadingBlanks ; i ++)
            oss << ' ' ;
         }
        oss << oneWord << ' ' ;  // it is not critically if the ' '
                                 // is not added
       }  // -- 4 --
//      end = (! iss.good () || (iss.tellg () >= lenStr - 1)) ;
     }  // -- 1 --
    delete [] row ;
    delete [] oneWord ;
   }  // -- 0 --
  delete [] txt ;
 }  // column :: strToRows

void column :: strToRowsL (const char * const str, size_t len,
                           int leadingBlanks)
 {
  if (! (cWidth && str))
    return ;
   {
    size_t i, sL = 0 ;

    for (i = 0 ; i < len && str [i] ; i ++)
      sL ++ ;
    if (sL < len)
      len = sL ;
   }
  if (! len)
    return ;
  if (leadingBlanks >= cWidth || leadingBlanks < 0)
    leadingBlanks = 0 ;
   {  // -- 0 --
    char * oneWord = new char [cWidth + 1] ;
    char * row = new char [cWidth + 1] ;
    ostrstream oss (row, cWidth) ;  // cWidth !
    istrstream iss (const_cast <char * const> (str), len) ;
    bool end = false ;
    bool blankIsFirst = (str [0] == ' ') ;

    setmem (row, cWidth + 1, '\0') ;  // cWidth + 1 !
     {
      for (int i = 0 ; i < leadingBlanks ; i ++)
        oss << ' ' ;
     }
    while (! end)
     {  // -- 1 --
      if (blankIsFirst)
       {  // Skip leading blanks
        long int gP1 = iss.tellg () ;

        blankIsFirst = false ;
        while (str [gP1] == ' ' && gP1 < (len - 1  // 2  // 25.oct.2001
                                         )
              )
          gP1 ++ ;
        iss.seekg (gP1) ;
       }
      oneWord [0] = '\0' ;
      if (iss.good () && iss.tellg () < len)
       {
        iss.getline (oneWord, cWidth + 1 - leadingBlanks, ' ') ;
        if (strlen (oneWord) == cWidth - leadingBlanks &&
            iss.tellg () < len
           )
          iss.clear () ;
        if (iss.tellg () < len && str [iss.tellg ()] == ' ')
          blankIsFirst = true ;
       }
      oss.clear () ;
      if (oss.tellp () + strlen (oneWord) <= cWidth)
       {  // -- 2 --
        if (oneWord [0])
          oss << oneWord << ' ' ;  // it is not critically if the ' '
                                   // is not added
        else
         {  // -- 3 --
          if (row [0])
           {
            strData.Add (strnew (row)) ;
            setmem (row, cWidth + 1, '\0') ;  // cWidth + 1 !
            oss.clear () ;
            oss.seekp (0) ;
             {
              for (int i = 0 ; i < leadingBlanks ; i ++)
                oss << ' ' ;
             }
           }
          end = true ;  // Nothing is read
         }  // -- 3 --
       }  // -- 2 --
      else
       {  // -- 4 --
        if (row [0])
          strData.Add (strnew (row)) ;
        setmem (row, cWidth + 1, '\0') ;  // cWidth + 1 !
        oss.clear () ;
        oss.seekp (0) ;
         {
          for (int i = 0 ; i < leadingBlanks ; i ++)
            oss << ' ' ;
         }
        oss << oneWord << ' ' ;  // it is not critically if the ' '
                                 // is not added
       }  // -- 4 --
     }  // -- 1 --
    delete [] row ;
    delete [] oneWord ;
   }  // -- 0 --
 }  // column :: strToRowsL

void column :: textToRows (const char * const str, int leadingBlanks)
 {
  if (! (cWidth && str))
    return ;

  size_t pos = 0 ;

  do
   {
    const size_t eL = strlen (str + pos) ;
    size_t l = 0 ;

    while (l < eL && * (str + pos + l) != '\n')
      l ++ ;
    if (l)
      if (* (str + pos + l - 1) != '\r')
       {
        strToRowsL (str + pos, l, leadingBlanks) ;
        encloseStr (str + pos, l) ;  // 21.jan.2005
       }
      else
        if (l > 1)
         {
          strToRowsL (str + pos, l - 1, leadingBlanks) ;
          encloseStr (str + pos, l) ;  // 21.jan.2005
         }
    pos += l ;
    if (str [pos] == '\n')
      pos ++ ;
   }
  while (str [pos]) ;
 }  // column :: textToRows

column :: column (size_t width, const char * str,
                  const int leadingBlanks) :
            position (0), calls (0), columnErr (false), cWidth (width),
            pItems (NULL), oldItemsNumber (0)  // 21.jan.2005
 {
  if (str)
    textToRows (str, leadingBlanks) ;
#if TESTVER
	if (str && strstr(str, "бозабоза голямабоза"))
		MessageBeep(-1);
#endif  // TESTVER
 }  // column :: column

//column :: ~column ()
// {
//  message("column %lx destroyed", (unsigned long) this);
// }  // column :: ~column

size_t column :: getRowsCount () const
 {
  return strData.Count () ;
 }  // column :: getRowsCount

const char * column :: nextRowOrEmptyStr ()
 {
  calls ++ ;
  if (position < strData.Count ())
    return (strData [position ++]) ;
  else
    return "" ;
 }  // column :: nextRowOrEmptyStr

void column :: reWind ()
 {
  calls = position = 0 ;
 }  // column :: reWind

const char * column :: operator [] (size_t index) const
 {
  if (index < strData.Count ())
    return (strData [index]) ;
  else
    return "" ;
 }  // column :: operator []

bool column :: error () const
 {
  return columnErr ;
 }  // column :: error

size_t column :: currPosition () const
 {
  return position ;
 }  // column :: currPosition

size_t column :: nextRowReqs () const
 {
  return calls ;
 }  // column :: nextRowReqs

size_t column :: addText (const char * text, const int leadingBlanks)
 {
  size_t oldItems = strData.Count () ;

  if (text)
    textToRows (text, leadingBlanks) ;
#if TESTVER
	if (text && strstr(text, "2015"))
		MessageBeep(-1);
#endif  // TESTVER
  return strData.Count () - oldItems ;
 }  // column :: addText

size_t column :: addVerticalSeparator ()
{
  size_t oldItems = strData.Count () ;
  strData.Add (strnew ("|")) ;
  encloseStr ("|", 1) ;
  return strData.Count () - oldItems ;
}

size_t column :: addEmptyRow ()
 {
  size_t oldItems = strData.Count () ;

  strData.Add (strnew ("")) ;
  encloseStr ("", 0) ;  // 21.jan.2005
  return strData.Count () - oldItems ;
 }  // column :: addText

size_t column :: width () const
 {
  return cWidth ;
 }  // column :: width

void column :: flush ()
 {
  strData.Flush () ;
 }  // column :: flush

void column :: setItems (TStringArray * itemsPtr)
 {  // 21.jan.2005
  pItems = itemsPtr ;
  if (pItems)
   {
    oldItemsNumber = pItems -> Count () ;
   }
  else
   {
    oldItemsNumber = 0 ;
   }
 }  // column :: setItems

void column :: encloseStr (const char * const str, size_t len)
{  // 21.jan.2005
  METHOD_NAME("encloseStr");

  if(pItems && str)
  {
    char itemNoStr[32];
    SimpleStringsArray attribs;

    // Decrease 'len' if it exceeds the length of 'str'
    {
      size_t i, sL = 0;

      for (i = 0; i < len && str[i]; i ++)
        sL++;
      if(sL < len)
        len = sL;
    }

    // Create an "no" attribute
    ostrstream(itemNoStr, sizeof(itemNoStr))
	<< (pItems->Count() - oldItemsNumber + 1) << ends;
    ENDING_ZERO(itemNoStr);
    attribs.Add(new SimpleStrings("no", itemNoStr));

    // Enclose 'len' symbols in XML '<item>' tag
    {
      // <item no="12345678901234567890123456789012" ></item>
      // 52 symbols
      const int extraLen = 128;  // 128 > 52
      // !!! increase extraLen if you add more attribures !!!

      // The length of the XML text may be greater than the length of the
      // initial string because of escape sequences ("&gt;", "&quot;" etc.)
      const size_t xmlTextLen = xml_text_len(str, len);
      CharPtr enclosedStr(xmlTextLen + extraLen);

      enclosedStr.End();  // Important! It is used in order to determine whether
      // the buffer was long enough (see below)
      {
        ostrstream oss(enclosedStr, enclosedStr.Size());

        {
          XML_ item(oss, "item", &attribs);  // Must be inside a block, because
            // the 'ends' symbol prevents the insertion of the closing tag.

          if(str[0])
          {
            // Use only first 'len' symbols of 'str'
            CharPtr tmp(len + 1);

            strCopy(tmp, str, tmp.Size());
            oss << XmlText(tmp);
          }
        }
        oss << ends;
      }
      if(enclosedStr[enclosedStr.Size() - 1] == '\0')
      {  // therefore the buffer is long enough
        pItems->Add(strnew (enclosedStr));
      }
      else
      {
        ::error("%s::%s\n%s", typeid(*this).name(), methodName,
          BUFFER_TOO_SHORT);
      }
    }
  }
}  // column :: encloseStr

numeratedColumn :: numeratedColumn (size_t width, const int no,
                                    const char * str,
                                    const int leadingBlanks) :
                     column (width, str, leadingBlanks), clmnNo (no)
 {
 }  // numeratedColumn :: numeratedColumn

bool numeratedColumn :: operator == (const numeratedColumn & other)
 {
  return clmnNo == other.clmnNo ;
 }  // bool numeratedColumn ::
    //        operator == (const numeratedColumn & other)

ColumnXmlEncloser::ColumnXmlEncloser(column* columnPtr, int cipher,
  TStringArray* itemsPtr):
  pColumn(columnPtr), code(cipher), pItems(itemsPtr)
{
  if(pItems && pColumn)
  {
    char tmp[64];

    ostrstream(tmp, sizeof(tmp))
      << HTML_TL << "c" << code << HTML_TR << ends;
    ENDING_ZERO(tmp);
    pItems->Add(strnew (tmp));
    pColumn->setItems(pItems);
  }
}  // ColumnXmlEncloser::ColumnXmlEncloser

ColumnXmlEncloser::~ColumnXmlEncloser()
{
  if(pItems && pColumn)
  {
    char tmp[64];

    ostrstream(tmp, sizeof(tmp))
      << HTML_TL << HTML_TE << "c" << code << HTML_TR << ends;
    ENDING_ZERO(tmp);
    pItems->Add(strnew (tmp));
    pColumn->setItems(NULL);
  }
}
