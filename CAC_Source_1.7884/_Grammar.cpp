#include "cac.h"  // 18.may.2001
//# include "all.h"


void lawTypeToPlural (ostream & os, const char t, const bool end1,
                      const char * mediumWord)
 {
  char tmpType [SIZE_OF_NAME] ;

  typeToString (t, tmpType, sizeof (tmpType)) ;

  int spCount = 0;   // 20070927 -->
  for (int i = 0; i < strlen(tmpType); i++)
  {  if (isspace(tmpType[i])) spCount++;   // 20070927 <--
  }
  int wordN = 0;
// 04.jan.2000 - begin
  if (tmpType [0])
// 04.jan.2000 - end
   {
    char tmp [SIZE_OF_NAME] ;
    istrstream iss (tmpType, strlen (tmpType)) ;
    size_t l ;

    tmp [0] = '\0' ;
    iss >> tmp ;
    if ((l = strlen (tmp)) > 0)
      writeString (os, tmp, l - 1, 0, 0) ;
    os << (end1 ? "и " : "ите ") ;
    wordN += 1;

    // 20070927 -->
    while  (wordN < spCount)
    {
      tmp [0] = '\0' ;
      iss >> tmp ;
      if ((l = strlen (tmp)) > 0)
        writeString (os, tmp, l - 1, 0, 0) ;
      os <<  "и " ;
      wordN += 1;
    }
    // 20070927 <--

    if (mediumWord)
      os << mediumWord << " " ;
    tmp [0] = '\0' ;
    iss >> tmp ;
    if ((l = strlen (tmp)) > 0)
      writeString (os, tmp, l - 1, 0, 0) ;
    os << "а" ;
   }
 }  // lawTypeToPlural

void types_ToPlural (ostream & os, const char * const types,
                        const bool end1) 
 {
  if (! (types && types [0]))
    return ;

  int i ;
  const size_t tL = strlen (types) ;

  for (i = 0 ; i < tL ; i ++)
   {  // -- 0 --
    char tmpType [SIZE_OF_NAME] ;
     {  // -- 1 --
      ostrstream oss (tmpType, sizeof (tmpType)) ;

      lawTypeToPlural (oss, types [i], end1) ;
      oss << ends ;
     }  // -- 1 --
    tmpType [sizeof (tmpType) - 1] = '\0' ;
    strtok (tmpType, " ") ;
    os << tmpType ;
    os << (i == tL - 1 ? "" : ", " ) ;
   }  // -- 0 --
 }  // types_ToPlural

const char * male_noun_article (const char * const noun, const bool full)
 {
  if (! (noun && noun [0]))
    return "" ;

  const char lastLetter = noun [strlen (noun) - 1] ;
  const bool capital = 'А' <= lastLetter && lastLetter <= 'Я' ;
  const char * article ;

  // The analysis is complex :
  // писатЕЛ - ЯТ <--> протокОЛ - ЪТ
  // другАР - ЯТ  <--> компютЪР - ЪТ
  // писатЕЛ - Я <--> протокОЛ - А
  // другАР - Я  <--> компютЪР - А
  // and so on and I shall not implement it now.
  // I'm using "ът" and "а" for now.

  if (capital)
    article = full ? "ЪТ" : "А" ;
  else
    article = full ? "ът" : "а" ;
  return article ;
 }  // male_noun_article

const char * noun_article (const char * const noun, const bool full)
 {
  if (! (noun && noun [0]))
    return "" ;

  const char lastLetter = noun [strlen (noun) - 1] ;
  const char * article ;

  switch (lastLetter)
   {
    case 'о' :
    case 'е' :
      article = "то" ;
      break ;
    case 'О' :
    case 'Е' :
      article = "ТО" ;
      break ;
    case 'а' :  // female
    case 'я' :
      article = "та" ;
      break ;
    case 'А' :  // female
    case 'Я' :  
      article = "ТА" ;
      break ;
    case '.' :  // 14.nov.2001
      article = "" ;
      break ;
    default :  // male
      article = male_noun_article (noun, full) ;
      break ;
   }
  return article ; 
 }  // noun_article

char * add_noun_article (char * const noun, const size_t nounSz,
                         const bool full)
// noun must contain any substantive. Returns noun.                         
 {
  if (noun && nounSz)
   {
    const size_t l = strlen (noun) ;

    if (l + 1 < nounSz)
     {
      ostrstream (noun + l, nounSz - l)
        << noun_article (noun, full) << ends ;
      noun [nounSz - 1] = '\0' ;
     }
   }
  return noun ;
 }  // add_noun_article

const char * male_adjective_article (const char * const adjective,
                                     const bool full)
 {
  if (! (adjective && adjective [0]))
    return "" ;

  const char lastLetter = adjective [strlen (adjective) - 1] ;
  const bool capital = 'А' <= lastLetter && lastLetter <= 'Я' ;
  const char * article ;

  if (upCyr (lastLetter) == 'Н')
    if (capital)
      article = full ? "НИЯТ" : "НИЯ" ;
    else
      article = full ? "ният" : "ния" ;
  else
    if (capital)
      article = full ? "ИЯТ" : "ИЯ" ;
    else
      article = full ? "ият" : "ия" ;
  return article ;
 }  // male_adjective_article

const char * adjective_article (const char * const adjective,
                                const bool full)
 {
  if (! (adjective && adjective [0]))
    return "" ;

  const char lastLetter = adjective [strlen (adjective) - 1] ;
  const char * article ;

  switch (lastLetter)
   {
    case 'о' :
      article = "то" ;
      break ;
    case 'О' :
      article = "ТО" ;
      break ;
    case 'а' :  // female
      article = "та" ;
      break ;
    case 'А' :  // female
      article = "ТА" ;
      break ;
    case '.' :  // 14.nov.2001
      article = "" ;
      break ;
    default :  // male
      article = male_adjective_article (adjective, full) ;
      break ;
   }
  return article ;
 }  // adjective_article

char * add_adjective_article (char * const adjective,
                              const size_t adjectiveSz,
                              const bool full)
// adjective must not be empty.                              
 {
  if (adjective && adjectiveSz)
   {
    const size_t l = strlen (adjective) ;

    if (l + 1 < adjectiveSz)
     {
      if (l > 2)
       {
        const char e1 = upCyr (adjective [l - 2]) ;   // 28.sep.2001  -1
        const char e2 = upCyr (adjective [l - 1]) ;   // and -2 swapped

        if (e1 == 'Е' && e2 == 'Н')
          ostrstream (adjective + (l - 2), adjectiveSz - (l - 2))
            << adjective_article (adjective, full) << ends ;
        else
          ostrstream (adjective + l, adjectiveSz - l)
            << adjective_article (adjective, full) << ends ;
       }
      else
        ostrstream (adjective + l, adjectiveSz - l)
          << adjective_article (adjective, full) << ends ;
      adjective [adjectiveSz - 1] = '\0' ;
     }
   }
  return adjective ;
 }  // add_noun_article

char * add_article (char * const str, const size_t strSz, const bool full)
 {  // 06.dec.2001 (extracted from kindWithArticleToString)
  char * const dest = str ;
  const size_t destSz = strSz ;

  if (dest && destSz)
   {
    if (destSz > 1)
     {
      const char * const blankPtr = strchr (dest, ' ') ;//03.may.2001-begin

      if (blankPtr)
       {
        CharPtr buff (destSz) ;

        strConcat (buff, dest, buff.Size ()) ;
        strtok (buff, " ") ;
        if (buff [0])  // 18.may.2001 - begin
         {
      //    add_noun_article (buff, destSz, full) ;
          add_adjective_article (buff, destSz, full) ;  // 18.may.2001-end
          strConcat (buff, blankPtr, buff.Size ()) ;
          dest [0] = '\0' ;
          strConcat (dest, buff, destSz) ;
         }  // 18.may.2001
       }
      else  // 03.may.2001 - end
        add_noun_article (dest, destSz, full) ;
     }
    else
      dest [0] = '\0' ;
   }
  return dest ; 
 }  // add_article

const char * male_passive_article (const char * const linkedNoun,
                                   const bool full, const bool the)
 {
  if (! (linkedNoun && linkedNoun [0]))
    return "" ;

  const char lastLetter = linkedNoun [strlen (linkedNoun) - 1] ;
  const bool capital = 'А' <= lastLetter && lastLetter <= 'Я' ;
  const char * article = "" ;

  if (capital)
   {
    if (full)
      if (the)
        article = "ИЯТ" ;
      else
        article = "ИЯ" ;
   }
  else
   {
    if (full)
      if (the)
        article = "ият" ;
      else
        article = "ия" ;
   }
  return article ;
 }  // male_passive_article

const char * passive_article (const char * const linkedNoun,
                              const bool full, const bool the)
 {
  if (! (linkedNoun && linkedNoun [0]))
    return "" ;

  const char lastLetter = linkedNoun [strlen (linkedNoun) - 1] ;
  const char * article ;

  switch (lastLetter)
   {
    case 'о' :
    case 'е' :
      article = the ? "ото" : "о" ;
      break ;
    case 'О' :
    case 'Е' :
      article = the ? "ОТО" : "О" ;
      break ;
    case 'а' :  // female
    case 'я' :
      article = the ? "ата" : "а" ;
      break ;
    case 'А' :  // female
    case 'Я' :  
      article = the ? "АТА" : "А" ;
      break ;
    default :  // male
      article = male_passive_article (linkedNoun, full, the) ;
      break ;
   }
  return article ;
 }  // passive_article

void law_kind_to_plural (ostream & os, const char * const name,
                         const bool end1) 
 {
  int wordNo = 0 ;
  const size_t l = strlen (name) ;
  unsigned long int lawWordBegInd = l ;
  unsigned long int lawWordEndInd = l ;

  if (l)
   {
    char * tmp = new char [l + 1] ;
    const char * const lW = "ДЕЛО" ;

    strcpy (tmp, name) ;

    const char * lawsuitWord = strstr (up_cyr_str (tmp), lW) ;

    if (lawsuitWord)
     {
      lawWordBegInd = lawsuitWord - tmp ;
      lawWordEndInd = lawWordBegInd + strlen (lW) ;
     }
    delete [] tmp ;
   }
  for (size_t i = 0 ; i < l ; i ++)
   {
    const bool endOfWord =
      (i == l - 1) ?
      true : name [i + 1] == ' ' || name [i + 1] == (char) 160 ;

    if (endOfWord)
     {
      const bool up = name [i] == upCyr (name [i]) ;

      if (lawWordBegInd <= i && i <= lawWordEndInd) // We are in the word "ДЕЛО"
        os << (up ? "А" : "а") ;
      else
        if (wordNo == 0 && end1 == false)
          os << (up ? "ИТЕ" : "ите") ;
        else
          if (lawWordBegInd < l)  // 30.may.2003  // The word "ДЕЛО" exists
           {
            const unsigned char uch = (unsigned char) (name [i]) ;

            if (((unsigned char) ('А') <= uch &&
                                          uch <= (unsigned char) ('Я')) ||
                ((unsigned char) ('а') <= uch)
  //               && uch <= (unsigned char) ('я'))  // cond. is always true
               )
              os << (up ? "И" : "и") ;
            else
              os << (char) uch ;
           }
          else  // 30.may.2003
            os << name [i] ;  // 30.may.2003
      wordNo ++ ;
     }
    else
      os << name [i] ;
   }
 }  // law_kind_to_plural

void kind_to_plural (const char kC, ostream & os,
                     const bool end1)
 {
  bool abbrev = true ;
  TKind k ;

  k.kind = kC ;
  if (! (k.kind && k.Get ()))
    return ;
  if (strchr (k.name, ' ') || strchr (k.name, (char) 160))
    abbrev = false ;
  if (abbrev)
    if (string_is_upper (k.name) == false)
      abbrev == false ;
  if (abbrev || ! strchr (KIND_LAWSUITS, k.kind))
    os << k.name ;
  else
    law_kind_to_plural (os, k.name, end1) ;
 }  //  kind_to_plural

void kinds_to_plural (ostream & os, const char * const kinds,
                      const bool end1)
 {
  if (kinds && kinds [0])
   {
    const size_t l = strlen (kinds) ;

    for (size_t i = 0 ; i < l ; i ++)
     {
      kind_to_plural (kinds [i], os, end1) ;
      if (i != l - 1)
        os << ", " ;
     }
   }
 }  // kinds_to_plural

const char * const BULGARIAN_VOWELS = "АЕИОУЪЮЯаеиоуъюя" ;
  
//
