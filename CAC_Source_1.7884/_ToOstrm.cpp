#include "cac.h"

void container_sides_to_ostream (ostream & os,
					   const TRCDKeyContainer & container,
					   const TSideWind & pattern,
					   const int suitors, const int defendants,
					   const int others)
 {
  const TDatedContainer * pDated =
    dynamic_cast <const TDatedContainer *> (& container) ;
  const bool contOK =
    (pDated && DatedContainer_ (* pDated).FilledOK ()) ||
    (pDated == false &&
     TRCDKeyContainer_ (container.key, container.kind).FilledOK ()
    ) ;

  if (contOK)
   {
    TSideWind * pSide ;

    if (pDated)
      pSide =
        dated_container_to_side_wind(*(const_cast<TDatedContainer*>(pDated)));
    else
      pSide = new TSideWind ;



    const long int startPos = os.tellp () ;

    if (pSide)
     {  // -- 0 --
      CharPtr crit (1024) ;

      (*pSide) << (const_cast <TRCDKeyContainer &> (container)) ;
      pSide->FloatCriteria (crit) ;

       {  // -- 1 --
        TQuery q (crit) ;
        int suitorsShown = 0 ;
        int defendantsShown = 0 ;
	int othersShown = 0 ;

        while (q.Read ())
         {  // -- 2 --
          (*pSide) << q ;

          const bool other =
            pDated ?
	      (const_cast <TDatedContainer *> (pDated)) ->
                 Other (pSide->involvement)
		:
              (const_cast <TRCDKeyContainer &> (container)).
                 Other (pSide->involvement) ;
          const bool suitor =
	    pDated ?
              ((const_cast <TDatedContainer *> (pDated)) ->
                 Suitor (pSide->involvement) ||
             (other && strchr (INVOLVEMENT_FROM_LEFTS, pSide->fromSide))
              )
                :
              ((const_cast <TRCDKeyContainer &> (container)).
               Suitor (pSide->involvement) ||
	     (other && strchr (INVOLVEMENT_FROM_LEFTS, pSide->fromSide))
              ) ;
	  const bool defendant =
            pDated ?
              ((const_cast <TDatedContainer *> (pDated)) ->
                 Defendant (pSide->involvement) ||
	     (other && strchr (INVOLVEMENT_FROM_RIGHTS, pSide->fromSide))
              )
              :
            ((const_cast <TRCDKeyContainer &> (container)).
               Defendant (pSide->involvement) ||
             (other && strchr (INVOLVEMENT_FROM_RIGHTS, pSide->fromSide))
            ) ;

	  if (suitor || defendant || other)
            if (ucn2uxn (pSide->ucnType) != UXN_ANONYMOUS &&
		ucn2uxn (pSide->ucnType) != UXN_UNKNOWN)
             {  // -- 3 --
              if (suitor)
               {
		if (suitors >= 0)
                  if (suitorsShown < suitors)
                    suitorsShown ++ ;
                  else
                    continue ;
               }
              if (defendant)
               {
		if (defendants >= 0)
                  if (defendantsShown < defendants)
		    defendantsShown ++ ;
                  else
                    continue ;
               }
	      if (other)
               {
                if (others >= 0)
                  if (othersShown < others)
                    othersShown ++ ;
                  else
                    continue ;
               }

              CharPtr tmp (256) ;

	      os << (startPos == os.tellp () ? "" : ", ") ;
              if (pattern.involvement)
               {
		involvementToString (pSide->involvement, tmp,
                                     tmp.Size ()) ;
                os << (startPos == os.tellp () ? "" : " ") << tmp.Str () ;
               }
              if (ucnPresent (pattern.ucn))
	       {  // -- 4 --
		char testChar ;

		testChar = pattern.ucn [0] ;
		// LPR: huh?!
		if ('0' < testChar && testChar <= '9')
		 {  // Names
		  ucnAndUcnTypeToName (pSide->ucn, pSide->ucnType, tmp,
				       tmp.Size ()) ;
		  os << (startPos == os.tellp () ? "" : " ")
		     << tmp.Str () ;
		 }
		testChar = pattern.ucn [1] ;
		// LPR: what if I'm born in a year divisible by 10?!
		if ('0' < testChar && testChar <= '9')
		 {  // UCN (and) ucnType
		  if (pattern.ucnType)
		   {
		    ucn_type_to_string (pSide->ucnType, tmp,
				       tmp.Size ()) ;
		    os << (startPos == os.tellp () ? "" : " ")
		       << tmp.Str () ;
		   }
		  os << (startPos == os.tellp () ? "" : " ")
		     << pSide->ucn ;
		 }
		testChar = pattern.ucn [2] ;
		// LPR: november and december?!
		if ('0' < testChar && testChar <= '9')
		 {  // Address
		  TLinkWind link ;
		  mstr m;

		  link.SetOrdinary (*pSide) ;
		  // 2011:262 ADDRESS_SECOND -> 2
		  print_link_address(m, link, 2);
		  strcat(tmp, str(m));
		  // LPR: february?!
		  //link_address_to_string (link, tmp, tmp.Size (),
		  //			  testChar == '2',
		  //			  false  // 04.oct.2004
		  //			  ) ;
		  os << (startPos == os.tellp () ? "" : " ")
		     << tmp.Str () ;
		 }
	       }  // -- 4 --
	     }  // -- 3 --
         }  // -- 2 --
       }  // -- 1 --
     }  // -- 0 --
   }
  else error ("%s\ncontainer_sides_to_ostream",
              WRONG_ARGS_PASSED_TO) ;
 }  // container_sides_to_ostream

void preliminary_actions_to_ostream (ostream & os,
						 const TLawsuit & law,
						 const char * const senderInstances,
                                     const bool withSender,
                                     const char * const senderPrefix,
                                     const bool labelOnly
                                    )
{ // Attention! Keep the logic of this function in
  // conformity with the code of add_preliminaryActions_to_column.
  const size_t buffSz = 256 ;
  CharPtr buff (CONNECT_TYPES_MAX*buffSz) ;
  CharPtr buff1 (buffSz) ;

  buff [0] = buff1 [0] = '\0' ;
  if (TRCDKeyContainer_ (law.key, law.kind).FilledOK ())
  {

# if 0
/*
    if (ps1_supported (law.sourceKind) &&
        (law.ps1.no || law.ps1.year || law.ps1.sender [0])
       )
     {
      const char * const sT1 = ps1_sender_type (law.sourceKind) ;

      if (senderInstances == NULL ||
          (sT1 && sT1 [0] && strstr (senderInstances, sT1))
         )
      prodet_to_string (law.ps1, buff, buffSz, true, law.sourceKind,
                        PS2_KIND_FAKE, withSender, senderPrefix,
                        labelOnly) ;
     }
    if (ps2_supported (law.sourceKind) &&
        (law.ps2.no || law.ps2.year || law.ps2.sender [0])
       )
     {
      const char * const sT2 = ps2_sender_type (law.sourceKind) ;

      if (senderInstances == NULL ||
          (sT2 && sT2 [0] && strstr (senderInstances, sT2))
         )
      prodet_to_string (law.ps2, buff1, buffSz, false, law.sourceKind,
                        law.ps2Kind, withSender, senderPrefix,
                        labelOnly) ;
     }
*/
# endif
    // 20070103 -->
 	  TGArray<TConnect> connects;
    TConnect *connect = new TConnect;
    TLawsuit *pLaw = new TLawsuit;
    pLaw -> key = law.key;

 	  if (load_connects(pLaw, &connects))
    {
		    for (int i = 0; i < connects.Count(); i++)
		    {
			     *connect << connects[i];

		if ( strchr ( CONNECT_TYPE_PRELIMITARY_ACTS, connect -> connectType ))
            {

               if ( ( senderInstances == NULL ) ||
                    ( connect->connectKind && strchr (senderInstances, connect->connectKind)) // F_CONNECT_KIND
                  )
               {
                  connect_to_string( connect, buff1, buffSz,
                                     withSender, senderPrefix, labelOnly) ;
                  os << buff << (buff [0] && buff1 [0] ? ", " : "") << buff1 ;
                  buff1 [0] = '\0' ;
               }

	  }		}   }
  	delete connect;
    delete pLaw;
    // 20070103 <--

    // os << buff << (buff [0] && buff1 [0] ? ", " : "") << buff1 ;
  }
}  // preliminary_actions_to_ostream



