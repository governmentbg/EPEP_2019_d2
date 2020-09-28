#include "cac.h"  // 18.may.2001
//# include "all.h"

bool columnsArr :: knownCCipher (unsigned int c) const
 {
  return (c >= bOC && c < (bOC + columnsTotal)) ;
 }  // columnsArr :: knownTBCCipher

void columnsArr :: newColumn (column * & rpC,
                              unsigned int l, const char * str)
 {
  if (! rpC)
    rpC = new column (l, str) ;
 }  // columnsArr :: newColumn

void columnsArr :: defineMaxRows ()
 {
  int i ;

  maxRows = 0 ;
  if (! columns)
    return ;
  for (i = 0 ; i < columnsTotal ; i ++)
    if (columns [i])
     {
      int a = columns [i] -> getRowsCount () ;

      if (a > maxRows)
        maxRows = a ;
     }
 }  // columnsArr :: defineMaxRows

columnsArr :: columnsArr (const char * formatFN, TWindow * parent,
                          int totalColumns, int basicOutputCipher,
                          const bool complex  // 23.oct.2001
                         ) :
                bookFormats (formatFN, parent),
                columnsTotal (totalColumns), bOC (basicOutputCipher),
                maxRows (0), columns (NULL), twoSets (complex)
 {
  if ((columns = new column * [columnsTotal]) != NULL)
   {
    int i ;

    for (i = 0 ; i < columnsTotal ; i ++)
      columns [i] = NULL ;
    for (i = 0 ; i < areasCnt ; i ++)
      if (knownCCipher (pBA [i].dataCipher))
        newColumn (columns [pBA [i].dataCipher - bOC],
                   pBA [i].areaLen) ;
   }
 }  // columnsArr :: columnsArr

columnsArr :: ~columnsArr ()
 {
  int i ;

  if (columns)
   {
    for (i = 0 ; i < columnsTotal ; i ++)
      if (columns [i])
        delete (columns [i]) ;
    delete [] columns ;
   }
 }  // columnsArr :: ~columnsArr

int columnsArr :: countMaxRows () const
 {
  int i, cnt ;

  if (! columns)
    return 0 ;
  cnt = 0 ;
  for (i = 0 ; i < columnsTotal ; i ++)
    if (columns [i])
     {
      int a = columns [i] -> getRowsCount () ;

      if (a > cnt)
        cnt = a ;
     }
  return cnt ;
 }  // columnsArr :: countMaxRows

int columnsArr :: getMaxRows () const
 {
  return maxRows ;
 }  //  columnsArr :: getMaxRows

void columnsArr :: output (ostream &, bool) const
 {
 }  // columnsArr :: output

void columnsArr :: standardSingleNCOutput (ostream & os,
                                           const bookAreas & bA)
 {  // 23.oct.2001
  if (knownCCipher (bA.dataCipher))
   {
    const int i = bA.dataCipher - bOC ;

    if (columns [i])
      writeString (os, columns [i] -> nextRowOrEmptyStr (),
                   bA.areaLen, bA.align, bA.from) ;
   }
  else
   {
    const size_t len = bA.areaLen + bA.from ;
    CharPtr s (len + 1) ;
    const int algn = bA.align ;
    const size_t skip = bA.from ;
    const unsigned int dCiph = bA.dataCipher ;

    if (! miscOutput (os, s, len, algn, skip, dCiph))
      writeString (os, "", len) ;
   }
 }  // columnsArr :: standardSingleNCOutput

void columnsArr :: notConstOutput (ostream & os, bool first)
 {  // An old variant in VersionArchives\1078\_appname.j --> clmnarr.cpp
  if (twoSets)
   {  // -- 0 --
    if (first)
     {
      for (int c = 0 ; c < areasCnt ; c ++)
       {
        const bookAreas & bA = pBA [c] ;

        if (cipherPresent (bA.dataCipher, false) == false)  // !!!
          standardSingleNCOutput (os, bA) ;
       }
     }
    else
     {
      for (int c = 0 ; c < areas1Cnt ; c ++)
        standardSingleNCOutput (os, pBA1 [c]) ;
     }
   }  // -- 0 --
  else
   {
    for (int c = 0 ; c < areasCnt ; c ++)
      standardSingleNCOutput (os, pBA [c]) ;
   }
 }  // columnsArr :: notConstOutput

bool columnsArr :: cipherPresent (const int cipher,
                                  const bool first,
                                  const bool knownCheck) const
 {
  if (! formatLoadIsOK ())
    return false ;

  const bookAreas * const area = first ? pBA : pBA1 ;
  const int cnt = first ? areasCnt : areas1Cnt ;

  for (int i = 0 ; i < cnt ; i ++)
    if (area [i].dataCipher == cipher)
      if (knownCheck)
       {
        if (knownCCipher (cipher))
          return true ;
       }
      else
        return true ;
  return false ;
 }  // columnsArr :: cipherPresent

void columnsArr :: reWind ()
 {
  if (! columns)
    return ;
  for (int i = 0 ; i < columnsTotal ; i ++)
    if (columns [i])
      columns [i] -> reWind () ;
 }  // columnsArr :: reWind

column * columnsArr::getColumn(const size_t cipher)
{  // 20.nov.2002
	return (columns && (cipher < columnsTotal)) ? columns[cipher] : NULL;
}  // columnsArr::getColumn

int columnsArr::ColumnsTotal() const
{  // 07.feb.2005
  return columnsTotal;
}

int columnsArr::BasicOutputCipher() const
{  // 07.feb.2005
  return bOC;
}

// 11.jan.2002 - begin
ColumnsArrIncMaxRows ::
  ColumnsArrIncMaxRows (const char * formatFN, TWindow * parent,
                        int totalColumns, int basicOutputCipher,
                        const bool complex) :
    columnsArr (formatFN, parent, totalColumns, basicOutputCipher,
                complex) 
 {
 }  // ColumnsArrIncMaxRows :: ColumnsArrIncMaxRows
                           
void ColumnsArrIncMaxRows ::
       IncMaxRowsIfNecessary (const int newMaxRowsVal)
 {
  if (maxRows == 0)
    defineMaxRows () ;
  if (maxRows < newMaxRowsVal)
    maxRows = newMaxRowsVal ;
 }  // ColumnsArrIncMaxRows :: IncMaxRowsIfNecessary

// 11.jan.2002 - end


int own_areas_count(const bookAreas* bkAreas, const int areasCnt,
                    const int basicOutputCipher,
                    const int supportedColumnsCount)
{
	int cnt = 0;

	if(bkAreas && areasCnt && basicOutputCipher && supportedColumnsCount)
  {
  	for(int i = 0; i < areasCnt; i ++)
    {
     const int c = bkAreas[i].dataCipher - basicOutputCipher;

     if(0 <= c && c < supportedColumnsCount)
     cnt ++;
    }

  }
  else
    error("%s\nown_areas_count", WRONG_ARGS_PASSED_TO);
	return cnt;    
}  // own_areas_count

//

