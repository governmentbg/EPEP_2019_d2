// ----- LeftRight -----------------------------------------------------------
LeftRight :: LeftRight (const char * const delim) :
		   leftWidth (0), rightWidth (0), delimiter (delim ? delim : "|")
 {
 }  // LeftRight :: LeftRight

bool LeftRight :: operator == (const LeftRight & other) const
 {
  if (this == & other)
    return true ;
  else
    if (leftWidth == other.leftWidth &&
	  rightWidth == other.rightWidth &&
	  strcmp (delimiter, other.delimiter) == 0)
     {
	const int lItems = left.Count () ;
	const int rItems = right.Count () ;

	if (lItems == other.left.Count () &&
	    rItems == other.right.Count ())
	 {
	  for (int i = 0 ; i < lItems ; i ++)
	    if ((* (left [i]) == * (other.left [i])) == false)
		return false ;
	  for (int i = 0 ; i < rItems ; i ++)
	    if ((* (right [i]) == * (other.right [i])) == false)
		return false ;
	  return true ;
	 }
	else
	  return false ;
     }
    else
	return false ;
 }   // LeftRight :: operator ==

// ----- SummonsReducer ------------------------------------------------------
SummonsReducer :: SummonsReducer (char **tText) :
			  iText (tText)
 {
 }  // SummonsReducer :: SummonsReducer

bool SummonsReducer :: Load (stringArray & lines)
{
  istrstream iss (*iText) ;
  CharPtr row (SIZE_OF_LINE) ;

  lines.Flush () ;

  while (iss.getline (row, row.Size ()))
  {
	strDelLastSpaces (row) ;
	if (row [0])
	  lines.Add (strnew (row)) ;
  }

  return true;
 }  // SummonsReducer :: Load

bool SummonsReducer :: Write (const stringArray & lines)
 {
  ostrstream oss;

  for (int i = 0 ; i < lines.Count () ; i ++)
    oss << (lines [i]) << endl ;

  delete[] *iText;
  *iText = gettext(oss);
  return true;
 }  // SummonsReducer :: Write

bool SummonsReducer :: Reduce ()
 {
  stringArray lines ;

  return Load (lines) && Write (lines) ;
 }  // SummonsReducer :: Reduce

// ----- SummonsCenterer -----------------------------------------------------
SummonsCenterer :: SummonsCenterer (char **tText) :
			   iText(tText), TabWarningShowed (false),
			   tabSize (8), noDelimiterWarningShowed (false),
			   action ("Центриране на призовка/съобщение: ")
 {
 }  // SummonsCenterer :: SummonsCenterer

int SummonsCenterer :: EffectiveLen (char * const str)
 {
  int visibleSymbols = 0 ;

  if (str && str [0])
   {
     {
      int i = 0 ;
      unsigned char c ;

      while ((c = str [i]) != '\0')
       {
        if (c > 31)
          visibleSymbols ++ ;
        else
          if (c == '\b' && i)
            visibleSymbols -= 2 ;  // 3 symbols will be printed as 1
          else
            if (c == '\t')
             {
              visibleSymbols += tabSize ;
              if (TabWarningShowed == false)
               {
                TabWarningShowed = true ;
                info ("%sВ документа има символ(и) за табулация. "
                      "Ще приема, че те заемат място колкото %d "
                      "интервала.", action, tabSize) ;
               }
		 }
        i ++ ;
       }
     }
    if (visibleSymbols < 0)
      visibleSymbols = 0 ;
   }
  return visibleSymbols ;
 }  // SummonsCenterer :: EffectiveLen

int SummonsCenterer :: PageWidth (const CharPtrArray & page)
 {
  const int items = page.Count () ;
  int pgWidth = 0 ;

  for (int i = 0 ; i < items ; i ++)
   {
    const int lL = strlen (page [i] -> Str ()) ;

    if (pgWidth < lL)
      pgWidth = lL ;
   }
  return pgWidth ;
 }  // SummonsCenterer :: PageWidth

void SummonsCenterer :: RefreshWidths (TIArray <LeftRight> & pages)
 {
  const int items = pages.Count () ;

  for (int i = 0 ; i < items ; i ++)
   {
    LeftRight & lr = * (pages [i]) ;

    lr.leftWidth = PageWidth (lr.left) ;
    lr.rightWidth = PageWidth (lr.right) ;
   }
 }  // SummonsCenterer :: RefreshWidths

void SummonsCenterer :: IncreasePageWidth (const CharPtrArray & page,
                                           const int additionalBlanks)
 {
  if (additionalBlanks >= 0)
   {
    const int items = page.Count () ;

    for (int i = 0 ; i < items ; i ++)
     {
      CharPtr tmp (additionalBlanks + strlen (page [i] -> Str ()) + 1) ;
      const int tmpSz = tmp.Size () ;

      tmp [0] = '\0' ;
      for (int j = 0 ; j < additionalBlanks ; j ++)
        strConcat (tmp, " ", tmpSz) ;
      strConcat (tmp, page [i] -> Str (), tmpSz) ;
      (* (page [i])) = tmp ;
     }
   }
 }  // SummonsCenterer :: IncreasePageWidth

void SummonsCenterer :: BalanceLeftRight (LeftRight & lr)
 {
  const int widthDiff = lr.leftWidth - lr.rightWidth ;

  if (widthDiff)
   {
    const int wd = abs (widthDiff) ;

    if (widthDiff < 0)
     {
      IncreasePageWidth (lr.left, wd) ;
      lr.leftWidth += wd ;
     }
    else
     {
      IncreasePageWidth (lr.right, wd) ;
      lr.rightWidth += wd ;
     }
   }
 }  // SummonsCenterer :: BalanceLeftRight

void SummonsCenterer :: BalanceAll (TIArray <LeftRight> & pages)
 {
  const int items = pages.Count () ;

  for (int i = 0 ; i < items ; i ++)
    BalanceLeftRight (* (pages [i])) ;
 }  // SummonsCenterer :: BalanceAll

void SummonsCenterer :: ExpandTo (LeftRight & lr, const int newWidth)
 {
  if (newWidth > 0)
   {
    if (newWidth > lr.leftWidth)
      IncreasePageWidth (lr.left, newWidth - lr.leftWidth) ;
    if (newWidth > lr.rightWidth)
      IncreasePageWidth (lr.right, newWidth - lr.rightWidth) ;
   }   
 }  // SummonsCenterer :: ExpandTo

void SummonsCenterer :: ExpandAll (TIArray <LeftRight> & pages,
                                   const int newWidth)
 {
  const int items = pages.Count () ;

  for (int i = 0 ; i < items ; i ++)
    ExpandTo (* (pages [i]), newWidth) ;
 }  // SummonsCenterer :: ExpandAll

int SummonsCenterer :: MaxWidth (const TIArray <LeftRight> & pages)
 {
  const int items = pages.Count () ;
  int maxWidth = 0 ;

  for (int i = 0 ; i < items ; i ++)
   {
    LeftRight & lr = * (pages [i]) ;

    if (maxWidth < lr.leftWidth)
      maxWidth = lr.leftWidth ;
    if (maxWidth < lr.rightWidth)
      maxWidth = lr.rightWidth ;
   }
  return maxWidth ;
 }  // SummonsCenterer :: MaxWidth

bool SummonsCenterer :: SplitRowAndAddToLR (LeftRight & lr,
                                            char * const row)
 {
  bool rT = true ;

  if (row && row [0])
   {
    char * const d = strstr (row, lr.delimiter) ;

    if (d)
     {
      * d = '\0' ;
      lr.left.Add (new CharPtr (row)) ;
      lr.right.Add (new CharPtr (d + strlen (lr.delimiter))) ;
     }
    else
     {  // -- problems ... --
      bool solved = false ;
	const int lItems = lr.left.Count () ;

      if (lItems > 0)
       {
        const int delimIndex = strlen (lr.left [0] -> Str ()) ;

        if (strlen (row) <= delimIndex)
	   {
	    lr.left.Add (new CharPtr (row)) ;
          lr.right.Add (new CharPtr ("")) ;
          solved = true ;
         }
        else
          if (row [delimIndex] == ' ')
           {
            row [delimIndex] = '\0' ;
            lr.left.Add (new CharPtr (row)) ;
            lr.right.Add (new CharPtr (row + delimIndex + 1)) ;
            solved = true ;
           }
       }
      if (solved == false)
	 {
        lr.left.Add (new CharPtr (row)) ;
        lr.right.Add (new CharPtr ("")) ;
        if (noDelimiterWarningShowed == false)
         {
          noDelimiterWarningShowed = true ;
          rT = ask ("%sВ документа има ред(ове) без вертикален "
                    "разделител. Първият такъв ред е:\n"
                    "\"%s\".\n"
                    "Все пак да се опитам ли да извърша центриране?",
                    action, row) ;
         }
       }
     }  // -- problems ... --
   }
  return rT ;
 }  // SummonsCenterer :: SplitRowAndAddToLR

bool SummonsCenterer :: LoadLR (LeftRight & lr, istrstream & iss)
 {
  CharPtr buff (SIZE_OF_LINE) ;
  const int buffSz = buff.Size () ;
  char ch2 [2] ;
  char & ch = ch2 [0] ;
  bool endLoop = false ;
  bool rT = true ;
  bool firstCh = true ;

  ch2 [0] = ch2 [1] = '\0' ;
  buff [0] ='\0' ;
  while (rT && !endLoop && iss.get (ch))
   {
	if ((firstCh && ch == '\n') == false)  // because of "\f\n"
	 {
	  if (ch == '\n' || ch == '\f')
	   {
	    rT = SplitRowAndAddToLR (lr, buff) ;
	    buff [0] ='\0' ;
	    if (rT == false || ch == '\f')
		endLoop = true ;
	   }
	  else
	    strConcat (buff, ch2, buffSz) ;
	 }
	firstCh = false ;
   }
  if (rT)
    if (lr.left.Count () !=
	  lr.right.Count ())
     {
	const char * const theNumerOf = "the number of " ;

	rT = error ("%s%sleft rows is not equal to %sright rows",
			action, theNumerOf, theNumerOf) ;
     }

  return rT ;
 }  // SummonsCenterer :: LoadLR

bool SummonsCenterer :: Load (TIArray <LeftRight> & pages)
 {
  istrstream iss (*iText) ;
  long iTextLen = strlen(*iText);
  bool rT = true;

  while (iss.tellg() < iTextLen)
  {
	LeftRight * pNewLR = new LeftRight ;

	rT = LoadLR (* pNewLR, iss) ;
	if (rT)
	  pages.Add (pNewLR) ;
	else
	  delete pNewLR ;
  }

  return rT;
 }  // SummonsCenterer :: Load

bool SummonsCenterer :: Write (const TIArray <LeftRight> & pages)
 {
  ostrstream oss ;

    for (int i = 0 ; i < pages.Count () ; i ++)
     {
	const LeftRight & lr = * (pages [i]) ;
	const int pgRows = lr.left.Count () ;

	for (int j = 0 ; j < pgRows ; j ++)
	 {
	  const char * const lS = lr.left [j] -> Str () ;
	  const char * delimStr = " " ;

	   {
	    const size_t l = strlen (lS) ;

	    for (size_t i = 0 ; i < l ; i ++)
	     {
		const unsigned char c = lS [i] ;

		if (c > ' ')
		 {
		  delimStr = lr.delimiter ;
		  break ;
		 }
	     }
	   }
	  oss << lS << delimStr << lr.right [j] -> Str () << endl ;
	 }

	 {
	  if (pgRows)
	    oss << '\f' << endl ;
	 }
     }

     {
	 delete[] *iText;
	 *iText = gettext(oss);
     }

  return true ;
 }  // SummonsCenterer :: Write

bool SummonsCenterer :: Center ()
 {
  TIArray <LeftRight> pages ;
  bool rT = Load (pages) ;

  if (rT)
   {
    RefreshWidths (pages) ;
    BalanceAll (pages) ;
    ExpandAll (pages, MaxWidth (pages)) ;
    rT = Write (pages) ;
   }
  return rT ;
 }  // SummonsCenterer :: Center
//
