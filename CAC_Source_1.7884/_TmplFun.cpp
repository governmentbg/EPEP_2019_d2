// from _abcbook.cpp
template <class T, class arrOfT>
void loadUCNGroupsForABC (arrOfT & ucnGroupsArr,
				  const begNLEndNLYear & bnlenly,
                          const char college,
                          T & ucnGroup, const bool checkLett = true)
 {
  char criteria [512 + 256 + 64] ;

  ucnGroupsArr.Flush () ;  // 27.feb.2001
   {
    setmem (ucnGroup.name, sizeof (ucnGroup.name) - 1, (char) 0xFF) ;
    ucnGroup.name [0] = bnlenly.endL ;
    ucnGroup.name [sizeof (ucnGroup.name) - 1] = '\0' ;
    if (composeABCCriteria (bnlenly, college, criteria, sizeof (criteria),
                            ucnGroup.name, ucnGroup.Name) == false)
      return ;  // 27.feb.2001
   }
   {
    TQuery q (criteria) ;
    T * pABCGroup ;
    const unsigned char bL = (const unsigned char) bnlenly.begL ;
    const unsigned char eL = (const unsigned char) bnlenly.endL ;

//    ucnGroupsArr.Flush () ;  // 27.feb.2001
    while (q.Read ())
     {
      pABCGroup = new T ;
      (* pABCGroup) << q ;
       {
        const unsigned char l = pABCGroup -> name [0] ;

        if ((checkLett == false) || (bL <= l && l <= eL))
          ucnGroupsArr.Add (pABCGroup) ;
        else
          delete pABCGroup ;
       }
     }
   }
 }  // loadUCNGroupsForABC

// from _outputs.cpp
template <class T>
bool ucnGroupLikeCitizenOutput (const T & ucnGroup, ostream & os,
                                char *, const size_t len,
                                const int algn,
                                const size_t skip, int dCiph,
                                const unsigned int offset_of_cipher,
                                bool nameInQuotes = false)
 // returns true if dCiph is known (perceived)
 {
  if (! (len //&& s
        ))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;

  switch (((long int) dCiph) - offset_of_cipher)
   {
    case 91 :
      writeString (os, ucnGroup.ucn, len, algn, skip) ;
      break ;

    case 8007:  // 20070126   // Този код трябва да остане заради извеждането на данни
                              // за юридически лица в азбучника - нов формат 12007 - 4000 = 8007
                              // OFFSET_CITIZEN_CIPHER = 4000;
    case 92 :
    case 98 :
      if (nameInQuotes)
       {
        const size_t tmpSz = strlen (ucnGroup.name) + 2 + 1 ;
        char * tmp = new char [tmpSz] ;

        ostrstream (tmp, tmpSz) << "\"" << ucnGroup.name
                                << "\"" << ends ;
        tmp [tmpSz - 1] = '\0' ;
        writeString (os, tmp, len, algn, skip) ;
        delete [] tmp ;
       }
      else
        writeString (os, ucnGroup.name, len, algn, skip) ;
      break ;
    case 93 :
    case 94 :
    case 95 :
    case 96 :
    case 97 :
      writeString (os, "", len, algn, 0) ;
      break ;
    case 99 :  // 15.feb.2005
      {
	  const TCitizen* pCit = dynamic_cast<const TCitizen*> (&ucnGroup);

	  if(pCit)
	  {
	    writeString(os, pCit->ucn, len, algn, 0);
        }
        else
        {
        }
      writeString (os, "", len, algn, 0) ;
      }
      break ;
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // ucnGroupLikeCitisenOutput

// from _toname.cpp
template <class T>
bool T_UCNAndUCNTypeToName (const char * ucn, const char ucnType,
                            char * destN, size_t destNSz,
                            T & item)
 {
  clearGroupData (item) ;
  if (! (destN && destNSz && ucn && ucnType))
   {
    if (destN && destNSz)
      destN [0] = '\0' ;
    return false ;
   }
  if (strlen (ucn) && ucnType)
   {
    item.ucnType = ucnType ;
    ostrstream (item.ucn, sizeof (item.ucn)) << ucn << ends ;
    item.ucn [sizeof (item.ucn) - 1] = '\0' ;
    if (item.Try ())
     {
      ostrstream (destN, destNSz) << item.name << ends ;
      destN [destNSz - 1] = '\0' ;
      return true ;
     }
    else
     {
      clearGroupData (item) ;
      destN [0] = '\0' ;
      return false ;
     }
   }
  else
   {
    destN [0] = '\0' ;
    return false ;
   }
 }  // T_UCNAndUCNTypeToName

// from _convert.cpp
template <class T> void T_to_Ts (T & item, TGArray <T> & items)
 {
  char * criteria = new char [1024] ;

  items.Flush () ;
  item.FloatCriteria (criteria) ;
   {  // -- 3 --
    TQuery q (criteria) ;

    while (q.Read ())
     {  // -- 4 --
      T * pNewT = new T ;

      clearGroupData (* pNewT) ;
      (* pNewT) << q ;
      items.Add (pNewT) ;
     }  // -- 4 --
   }  // -- 3 --
  delete [] criteria ;
 }  // T_to_Ts

// from _links.cpp
template <class S, class I>
bool side_present (const TGArray <S> & sides, const I & item,
  int * pAtIndex = NULL)
 {
  bool found = false ;
  const int sCnt = sides.Count () ;
  int j ;

  if (pAtIndex)
    (* pAtIndex) = -1 ;
  for (j = 0 ; ! found && j < sCnt ; j ++)
   {
    const I & aS = * (sides [j]) ;

    found = item.ucnType == aS.ucnType &&
            strcmp (item.ucn, aS.ucn) == 0 &&
            item.involvement == aS.involvement ;
    if (pAtIndex && found)
      (* pAtIndex) = j ;
   }
  return found ;
 }  // side_present

template <class S, class L>
void t_to_T_links (const S & side, TGArray <L> & links,
			 TGArray <L> & result,
			 const bool includingAloneSide)
 {
  result.Flush () ;
  if (includingAloneSide)
   {
    L * pNewLink = new L ;

    pNewLink -> SetOrdinary (side) ;
    result.Add (pNewLink) ;
   }
  if (side.ucnType && ucnPresent (side.ucn) && side.involvement)
   {
    const int items = links.Count () ;
    S ordinary ;

    for (int i = 0 ; i < items ; i ++)
     {
      L & aLink = * (links [i]) ;

      aLink.GetOrdinary (ordinary) ;
      if (side.involvement == ordinary.involvement &&
          side.ucnType == ordinary.ucnType &&
          strcmp (side.ucn, ordinary.ucn) == 0)
       {
        L * pNewLink = new L ;

        (* pNewLink) << aLink ;
        result.Add (pNewLink) ;
       }
     }
   }
 }  // t_to_T_links
