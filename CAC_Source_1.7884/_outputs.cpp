#include "cac.h"  // 18.may.2001
//# include "all.h"

bool lawsuitOutput (const TLawsuit & lS, ostream & os, char * s,
			  const size_t len, const int algn,
			  const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;
  const size_t wrtL = len > skip ? len - skip : len ;
  const int cph = ((long int) dCiph) - OFFSET_LAWSUIT_CIPHER ;    // 1000

  switch (cph)
   {
    case 1 :
      // 2008:303 IRQ: flat_key_no(); 2010:047 flag_
	writeLong (os, flag_key_no(&lS.key), len, algn) ;
      break ;
    case 2 :
      writeLong (os, lS.key.year, len, algn) ;
      break ;
    case 3 :
      typeToString (lS.key.type, s, len + 1) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 4 :
      writeLong (os, lS.source.no, len, algn) ;
      break ;
    case 5 :
      writeLong (os, lS.source.year, len, algn) ;
      break ;
    case 6 :
      typeToString (lS.source.type, s, len + 1) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 7 :
	writeULong (os, lS.date.day, len, algn) ;
      break ;
    case 8 :
      writeULong (os, lS.date.month, len, algn) ;
      break ;
    case 9 :
      writeLong (os, lS.date.year, len, algn) ;
      break ;
    case 10 :
      subjectToString (lS.key.type, lS.subject, s, len + 1) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 11 :
      ZERO_LEN( s );
      compositionToString (lS.composition, s, len + 1) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 12 :
      writeULong (os, lS.age, len, algn) ;
      break ;
    case 13 :
      citizenUCNAndUCNTypeToName (lS.judge, UCN_CITIZEN_UCN,
                                  s, len + 1) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 14 :
      writeULong (os, lS.finished.day, len, algn) ;
      break ;
    case 15 :
      writeULong (os, lS.finished.month, len, algn) ;
      break ;
    case 16 :
     writeLong (os, lS.finished.year, len, algn) ;
      break ;
    case 17 :
      grantToString (lS.grant, s, len + 1) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 18 :
    	// 2009:154 LPR: removed
	break ;
    case 19 :
// 20070112  --> in comment - useless
//    writeLong (os, lS.returned.no, len, algn) ;
// 20070112  <-- in comment - useless
      break ;
    case 20 :
// 20070112  --> in comment - useless
//    writeLong (os, lS.returned.year, len, algn) ;
// 20070112  <-- in comment - useless
      break ;
    case 21 :
// 20070112  --> in comment - useless
//      typeToString (lS.returned.type, s, len + 1) ;
//      writeString (os, s, wrtL, algn, skip) ;
// 20070112  <-- in comment - useless
      break ;
    case 22 :
// 20070112  --> in comment - useless
//    writeULong (os, lS.decMade.day, len, algn) ;
// 20070112  <-- in comment - useless
      break ;
    case 23 :
// 20070112  --> in comment - useless
//    writeULong (os, lS.decMade.month, len, algn) ;
// 20070112  <-- in comment - useless
      break ;
    case 24 :
// 20070112  --> in comment - useless
//      writeLong (os, lS.decMade.year, len, algn) ;
// 20070112  <-- in comment - useless
      break ;
    case 25 :
      kindToString (lS.kind, s, len + 1) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 26 :
    case 61 :  // 27.aug.2002
	if (PRINT_COLLEGE) // 20070703
      {
        collegeToString (lS.college, s, len + 1) ;
        if (cph == 61)
        {  // 01.jul.2003 - bugfix (I hope)
          strConcat (s, " ", len + 1) ;
          strConcat (s, COLLEGE, len + 1) ;
          strConcat (s, " ", len + 1) ;
        }  // 01.jul.2003 - bugfix (I hope)
        writeString (os, s, wrtL, algn, skip) ;
      }
      break ;
    case 31 :
	CDateToString (lS.date, s, len + 1) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 32 :
      CDateToString (lS.decMade, s, len + 1) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 33 :
      kindToStringAbbr (lS.kind, s, len + 1) ;
	writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 34 :
	monthOutput (os, s, len, algn, skip, lS.date.month) ;
      break ;
    case 35 :
	subjectAndExtraTextToString (lS.kind, lS.subject, s, len + 1,
                                   lS.text) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 36 :
      sourceNoAndSenderToString (lS, s, len + 1) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 37 : //Attention! putCompositionToOutStream is slow for AREAL
      putCompositionToOutStream (lS.composition,
                                 os, s, len, algn, skip) ;
      break ;
    case 38 :
# if INSTANCE
       {
        TKind k ;

	  kindToString (lS.kind, k.name, sizeof (k.name)) ;

	  const char * kN = k.name ;

	  if (strlen (kN) - skip > len)
	    kind_to_alias(lS.kind, k.name, sizeof (k.name)) ;
	    
	  writeString (os, kN, wrtL, algn, skip) ;

       }
# else
      kindToString (lS.kind, s, len + 1) ;
      writeString (os, s, wrtL, algn, skip) ;
# endif
      break ;
    case 39 :
    case 53 :
      s [0] = '\0' ;
      if (lS.source.type && strchr (TYPE_REQUESTS, lS.source.type))
       {
        kindToString (lS.sourceKind, s, len + 1) ;
        if (s [0])
          if (cph == 53)
            strtok (s, "-") ;
       }
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 40 :
	if (lS.source.type && strchr (TYPE_REQUESTS, lS.source.type) &&
					TRCDKey_ (lS.source).FilledOK ())
       {
        TRequest req ;

        req.key = lS.source ;
        if (req.Get ())
          request_output (req, os, s, len, algn, skip,
                          12 + OFFSET_REQUEST_CIPHER) ;
       }
      break ;
    case 41 :
    case 42 :
      if (lS.source.type && strchr (TYPE_REQUESTS, lS.source.type) &&
          TRCDKey_ (lS.source).FilledOK ())
       {
        TRequest req ;

        req.key = lS.source ;
        if (req.Get ())
	   {
          int code = 14 ;

          if (cph == 42)
						if (lS.key.type == TYPE_PUNISHMENT_LAW)
              code = 19 ;
            else
              code = 15 ;
          request_output (req, os, s, len, algn, skip,
                          code + OFFSET_REQUEST_CIPHER) ;
         }
       }
	else
        writeString (os, "....", wrtL, algn, skip) ;
      break ;

    case 43 :
    case 51 :
# if INSTANCE
//#   if APPEAL  // 31.jan.2002 commented
//#   else  // of APPEAL  // 31.jan.2002 commented

	// 20070112, 20070219 -->
      // Заменено с търсене на Първоинстанционно дело
      {
         s[0] = '\0';
	   TLawsuit * pLawsuit = new TLawsuit ;
         pLawsuit -> key = lS.key;
         char connType[2]; connType[0]= CONNECT_TYPE_LOWER_INSTANCE; connType[1] = '\0';
         TConnect *c = get_connect(pLawsuit, connType);
         int res = false;
// 20071002 -->
#if COURT_TYPE == COURT_APPEAL
	   if ( c == NULL )
	   {              //Въззивно дело
	     connType[0]= CONNECT_TYPE_RECALL_LAWSUIT; connType[1] = '\0';
	     c = get_connect(pLawsuit, connType);
	   }
#endif  // APPEAL
// 20071002 -->
	   if ( c != NULL )
	   {
		if (   ( c -> connectType == CONNECT_TYPE_LOWER_INSTANCE )
#if COURT_TYPE == COURT_APPEAL
		     ||( c -> connectType == CONNECT_TYPE_RECALL_LAWSUIT )
#endif  // APPEAL
		   )
		{  // ostrstream (s, len + 1)
		   ostrstream oss (s, len + 1) ;
		   oss.clear () ;
		   oss.seekp (0) ;

		   if (c -> connectKind)
               {
			TConnectKind t;
			t.connectKind = c -> connectKind;
// 20070807 ->
# if COURT_TYPE == COURT_ADMIN
			if ( t.connectKind == KIND_LOWER_LAWS_PUNISHMENT )
			   oss << "НАХД";
			else
			   if(t.connectKind && t.Try())
				 oss << t.name << " ";
# else
// 20070807 <-
#if COURT_TYPE == COURT_APPEAL
			if( c -> connectType == CONNECT_TYPE_RECALL_LAWSUIT )
			   oss << "Въззивно" << " ";
#endif

			if(t.connectKind && t.Try())
			   oss << t.name << " ";
#endif
		   }

		   oss << " No " << c -> connectNo << '/' << c -> connectYear
                   << " г. " << ends ;
               res = true;
            }
         }
         if ( (cph == 51) && (! res) )
         {
            ostrstream (s, len + 1) << "...." << ends ;
            s [len] = '\0' ;
         }

         writeString (os, s, wrtL, algn, skip) ;

         delete c;
	   delete pLawsuit;
      }
      // 20070112 <--
//#   endif  // of APPEAL  // 31.jan.2002 commented
# else  // of INSTANCE
# endif  // of INSTANCE
//20070219  writeString (os, s, len, algn, skip) ;
      break ;
    case 44 :
    case 52 :
    case 69 :
      s [0] = '\0' ;
# if INSTANCE
// 20070112  #   if APPEAL  // Извежда се един и същ текст за AREA i APPEAL
      {  // 31.jan.2002 - begin
// I won't implement this right now ... But now (01.jul.2002) I do.

				bool res = false ;
	  // 2009:154 LPR: removed
	  // добавя Първоинстанционен съд или "...." към текста на обявлението в ДВ
        // Търсенето в T_CONNECT  да е същото като код 51  (== 1051)
        {
           TLawsuit * pLawsuit = new TLawsuit ;
           pLawsuit -> key = lS.key;
           TSender sndr ;
           clearGroupData (sndr) ;

	     char connType[2]; connType[0]= CONNECT_TYPE_LOWER_INSTANCE; connType[1] = '\0';

           TConnect *c = get_connect(pLawsuit, connType);

           if ( c != NULL )
           {
            	if (( c -> ucnType )&&( c -> ucn ))
              { // get sender name
                senderUCNAndUCNTypeToName (c -> ucn , c -> ucnType,
                                           sndr.name, sizeof (sndr.name) );
                res = sndr.name[0] != '\0' ;
		    if (res)
                {
                   ostrstream (s, len + 1) << sndr.name  << ends ;
 						       s [len] = '\0' ;
              } }
           }
           else
           {  // Добавено е търсене на Първоинстанционно дело в T_CONNECT към входящия документ.
		  // 20070114 -->
              if (lS.source.type && strchr (TYPE_REQUESTS, lS.source.type) &&
	  				      TRCDKey_ (lS.source).FilledOK ())
              {
                  TRequest req ;
                  req.key = lS.source ;
                  if (req.Get ())
                  {
                      TRequest * pReq = new TRequest ;
                      pReq -> key = req.key;
			    c = get_connect(pReq, connType);
                      if ( c != NULL )
                      {
                      	if (( c -> ucnType )&&( c -> ucn ))
                        { // get sender name
                            senderUCNAndUCNTypeToName (c -> ucn , c -> ucnType,
                                                       sndr.name, sizeof (sndr.name) );
                            res = sndr.name[0] != '\0' ;
                            if (res)
                            {
                                ostrstream (s, len + 1) << sndr.name  << ends ;
 						                    s [len] = '\0' ;
                      } }   }
                      delete pReq;
              }   }
              // 20070114 <--
           }
    			 if (res == false)
           {
    					if (cph == 52 || cph == 69)
		    			 {
    			    		ostrstream (s, len + 1) << "...." << ends ;
		  				    s [len] = '\0' ;
           }   }
           delete c;
           delete pLawsuit;
        }
	}  // 31.jan.2002 - end
	 // 2009:154 LPR: removed
	if (cph == 69)
       {
        mixed_case_cyr_str (s, true) ;
        // Blur some cosmetic defects
         {
          const char * const rc = "Рс-";

          if(strncmp(s, rc, strlen(rc)) == 0)
           {
            s [1] = 'С' ;
           }
         }
         {
          const char * const rc = "Рс -";

          if(strncmp(s, rc, strlen(rc)) == 0)
           {
            s [1] = 'С' ;
           }
         }
         {
          const char * const c = "Гр.";
          char * const pc = strstr(s, c);

          if(pc != NULL)
           {
            pc[0] = 'г' ;
           }
         }
       }
// 20070112  # else  // of INSTANCE
# endif  // of INSTANCE
	writeString (os, s, len, algn, skip) ;
	break ;
    case 45 :
    case 46 :
    case 47 :
    case 55 :
      if (len < WRITE_STRING_LEN_AS_IS)
       {
        preliminary_actions_to_string (s, len + 1, lS,
						   C2S[UCN_INSTANCE_DETECT], cph > 45,
						   cph > 46 ? "по описа на" : NULL,
						   cph == 55) ;
	  writeString (os, s, len, algn, skip) ;
	 }
	else
	  preliminary_actions_to_ostream (os, lS, C2S[UCN_INSTANCE_DETECT],
						    cph > 45,
                                        cph > 46 ? "по описа на" : NULL,
                                        cph == 55) ;
      break ;
    case 48 :
    case 49 :
    case 50 :
    case 56 :
      if (len < WRITE_STRING_LEN_AS_IS)
       {
        preliminary_actions_to_string (s, len + 1, lS,
						   C2S[UCN_INSTANCE_PROSEC], cph > 48,
                                       cph > 49 ? "по описа на" : NULL,
                                       cph == 56) ;
        writeString (os, s, len, algn, skip) ;
       }
      else
	  preliminary_actions_to_ostream (os, lS, C2S[UCN_INSTANCE_PROSEC],
						    cph > 48,
						    cph > 45 ? "по описа на" : NULL,
						    cph == 56) ;
	break ;
    case 54 :
	writeDate (os, lS.forceDate, len, algn, skip) ;
	break ;
    case 57 :
	preliminary_actions_to_string (s, len + 1, lS, C2S[UCN_INSTANCE_DETECT],
                                     false, NULL, true) ;
      if (s [0])
        add_article (s, len + 1, false) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 58 :
      preliminary_actions_to_string (s, len + 1, lS, C2S[UCN_INSTANCE_PROSEC],
                                     false, NULL, true) ;
      if (s [0])
        add_article (s, len + 1, false) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 59 :
       {
        const char * const delim = "\x07""\x07""\x07""\x08""\x08""\x08" ;
        const size_t dL = strlen (delim) ;

        preliminary_actions_to_string (s, len + 1, lS,
						   C2S[UCN_INSTANCE_PROSEC],
						   true, delim) ;

        const char * prosec = strstr (s, delim) ;

        if (prosec == NULL)
          prosec = "" ;
        else
          if (strlen (prosec) > dL)
           {
            prosec += dL ;
            if (prosec [0] == ' ')
              prosec ++ ;
           }
          else
            prosec = "" ;
        writeString (os, prosec, len, algn, skip) ;
       }
      break ;
    case 60 :  // 27.aug.2002
      s [0] = '\0';
       {
        TRCDKey_ k (lS.key) ;

        if (k.FilledOK ())
          k.Print (s, len + 1) ;
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 62 :                                                  
    case 63 :
    case 64 :
    case 67 :
    case 68 :
      ZERO_LEN(s);
      {
        if(TRCDKey_(lS.key).FilledOK())
        {
          LawPtr pLaw;

          pLaw.Ref().key = lS.key;
          if(pLaw.Ref().Get())
          {
            lawRCDKeyToSidePair pairs(pLaw.Ref(), false);
            unsigned claimsCnt = pairs.getClaimantsCount();

            if(claimsCnt > 0)
            {
              TClaimant clm;
              TSideWind clSide;

              if(pairs.nextPair(&clm, NULL, &clSide, NULL, true))
              {
                switch(cph)
                {
                  case 62:
                  case 67:
                    side_to_string(&clSide, s, len + 1);
                    if(cph == 67)
                      mixed_case_cyr_str(s, true);
                    break;
                  case 63:
                  case 68 :
                    strCopy(s, clm.cSettlement, len + 1);
                    if(cph == 68)
                    {
                      const char * const c = "Гр." ;

                      mixed_case_cyr_str (s, true) ;
                      // Blur a cosmetic defect
                      if (strncmp (s, c, strlen (c)) == 0)
                       {
                        s [0] = 'г' ;
                       }
                    }
                    break;
                  case 64:
                    side_to_string(&clSide, s, len + 1);
                    strConcat(s, " от ", len + 1);
                    strConcat(s, clm.cSettlement, len + 1);
                    break;
                }
              }
            }
          }
        }
      }
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 65 :        // useless
    case 66 :        // против ...
      ZERO_LEN(s);
# if INSTANCE
#   if APPEAL
       {

				bool res = false ;

				if (sourceTypeToSenderUCNType (lS,// (lS.source.type,
							// 20061220 lS.sourceKind) == UCN_INSTANCE_COURT)
				  lS.sourceKind) == UCN_INSTANCE_LOWER)
	    if (lS.source.type == TYPE_REQUEST && TRCDKey_(lS.source).FilledOK())
	    {
		TRequest req;

		req.key = lS.source;
		{
              TRequest * pReq = new TRequest ;
              pReq -> key = req.key;
              char connType[2]; connType[0]= CONNECT_TYPE_LOWER_INSTANCE; connType[1] = '\0';
              TConnect *c = get_connect(pReq, connType);

              if ( c != NULL )
		  {
                ostrstream(s, len + 1) << Kind_( c -> decisionKind ) << ends;
                s[len] = '\0';
                res = true;
              }
              delete c;
              delete pReq;
            }
            // 20070114 <--  T_CONNECT
          } // if source type = TYPE_REQUEST
				if (res == false)
					if (cph == 66)
					 {
						ostrstream (s, len + 1) << "[...]" << ends ;
						s [len] = '\0' ;
           }
       }  // 31.jan.2002 - end
#   else  // of APPEAL
       {
        bool res = false ;  //12.dec.2001
        // 20070114 if (lS.regionKind && lS.regionNo && lS.regionYear)
	    if (sourceTypeToSenderUCNType (lS, //(lS.source.type, lS.sender,
                                         lS.kind) == UCN_INSTANCE_LOWER)
            if (lS.source.type == TYPE_REQUEST && TRCDKey_(lS.source).FilledOK())
            {
              TRequest req;

		  req.key = lS.source;
		  { // 20070114 -->  T_CONNECT
		    TRequest * pReq = new TRequest ;
		    pReq -> key = req.key;
		    char connType[2]; connType[0]= CONNECT_TYPE_LOWER_INSTANCE; connType[1] = '\0';
		    TConnect *c = get_connect(pReq, connType);

                if ( c != NULL )
                {
                  ostrstream(s, len + 1) << Kind_( c -> decisionKind ) << ends;
                  s[len] = '\0';
                  res = true;
                }
                delete c;
                delete pReq;
              } // 20070114 <--  T_CONNECT
            }
        if (res == false)
          if (cph == 66)
           {
            ostrstream (s, len + 1) << "[...]" << ends ;
            s [len] = '\0' ;
           }
       }
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE
      writeString (os, s, len, algn, skip) ;
      break ;
    case 70 :  // 16.jul.2002
      s [0] = '\0';
       {
        TRCDKeyContainer_ k (lS.key, lS.kind) ;

        if (k.FilledOK ())
          k.Print (s, len + 1) ;
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 71 :
      ZERO_LEN( s );
      if ( ( lS.composition )
           //&&( use_area_compositions () )
         )
      {
         compositionToStringCompl (lS.composition, s, len + 1) ;
         writeString (os, s, wrtL, algn, skip) ;
      }
    break ;
      // last: 71
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // lawsuitOutput

void sessDateEndToString (char * s, const size_t sSz,
                          const TSession & sess)
 {
  if (! (s && sSz))
    return ;
  if (sSz == 1)
   {
    s [0] = '\0' ;
    return ;
   }
  s [sSz - 1] = '\0' ;
  CDateToString (sess.date, s, sSz) ;

  const size_t l = strlen (s) ;

  if (sess.end.year && sess.end.month && sess.end.day)
   {
    if (sess.end.year != sess.date.year ||
        sess.end.month != sess.date.month ||
        sess.end.day != sess.date.day)
     {
      if (l < sSz - 1)
        ostrstream (s + l, sSz - l) << " - " << ends ;
       {
        const size_t l1 = strlen (s) ;

	  if (l1 < sSz - 1)
	    CDateToString (sess.end, s + l1, sSz - l1) ;
	 }
     }
   }
 }  // sessDateEndToString

bool sessionOutput (const TSession & sess, ostream & os, char * s,
			  const size_t len, const int algn,
			  const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 // OFFSET_SESSION_CIPHER = 2000
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;
  const long int dCp = ((long int) dCiph) - OFFSET_SESSION_CIPHER ;

  switch (dCp)
   {
    case 141 :  // F_NO
      writeLong (os, sess.key.no, len, algn) ;
      break ;
    case 142 :  // F_YEAR
      writeLong (os, sess.key.year, len, algn) ;
      break ;
    case 143 :  // F_TYPE
      typeToString (sess.key.type, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
	break ;
    case 144 :  // F_KIND
      kindToString (sess.kind, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 145 :  // F_DATE
      CDateToString (sess.date, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 146 :  // F_TIME
      CTimeHMToString (sess.time, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 147 :  // F_END
      CDateToString (sess.end, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 148 :  // F_COMPOSITION
      compositionToString (sess.composition, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 149 :  // F_JUDGE
      citizenUCNAndUCNTypeToName (sess.judge, UCN_CITIZEN_UCN,
                                  s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
	break ;
    case 150 :  // F_SECRETAR
      citizenUCNAndUCNTypeToName (sess.secretar, UCN_CITIZEN_UCN,
					    s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 151 :  // F_PROSECUTOR
      citizenUCNAndUCNTypeToName (sess.prosecutor, UCN_CITIZEN_UCN,
                                  s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 152 :  // F_JUROR
      citizenUCNAndUCNTypeToName (sess.juror, UCN_CITIZEN_UCN,
                                  s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 153 :  // F_REJUROR
      citizenUCNAndUCNTypeToName (sess.reJuror, UCN_CITIZEN_UCN,
                                  s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 154 :  // F_RESULT
      resultToString (sess.result, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 155 :  // F_TEXT
      writeString (os, sess.text, len, algn, skip) ;
      break ;
    case 156 :  // the lawsuit's kind
    case 169 :  // the lawsuit's kind (boza short name)
       {
	  TLawsuit lawsuit ;

        if (TRCDKeyToLawsuit (sess.key, lawsuit))
         {
          if (dCp == 156)
		kindToString (lawsuit.kind, s, len + 1) ;
	    else
	    {
		// 2009:072 LPR/LRQ: at last!
		kind_to_alias(lawsuit.kind, s, len + 1) ;
	    }
          writeString (os, s, len, algn, skip) ;
         }
        else
          writeString (os, "", len, algn, skip) ;
       }
      break ;
    case 157 :  // date.month
	monthOutput (os, s, len, algn, skip, sess.date.month) ;
	break ;
    case 158 :  // date.year
      writeLong (os, sess.date.year, len, algn) ;
      break ;
    case 159 :  // F_DATE, F_END
    case 164 :
      sessDateEndToString (s, len + 1, sess) ;
	if (dCp == 164)
        outNormDblStrkStringAtCondStream (os, s, len, algn, skip) ;
      else
        writeString (os, s, len, algn, skip) ;
      break ;
    case 160 :  // F_DATE, F_END, lawsuit's sender
# if INSTANCE
      sessDateEndToString (s, len + 1, sess) ;
       {
        TLawsuit lawsuit ;
        const size_t l = strlen (s) ;

        if (l < len && TRCDKeyToLawsuit (sess.key, lawsuit))
         {
          ostrstream (s + l, len - l) << "  " << ends ;
           {
            const size_t l1 = strlen (s) ;
            const char senderUCNType =
#   if APPEAL  // 25.apr.2000
				 sourceTypeToSenderUCNType (lawsuit, //(lawsuit.source.type,
								    lawsuit.sourceKind) ;
#   else  // APPEAL
				 sourceTypeToSenderUCNType (lawsuit, //(lawsuit.source.type,
										 // lawsuit.sender,
								    lawsuit.kind) ;
#   endif  // APPEAL
// 20070115 -->
//            if (senderUCNType && l1 < len)
//              senderUCNAndUCNTypeToName (lawsuit.sender, senderUCNType,
//                                         s + l1, len - l1) ;
              if (senderUCNType && l1 < len) {}
           }
         }
       }
      writeString (os, s, len, algn, skip) ;
# endif
      break ;
    case 161 :  // F_TIME
      if (sess.time.hour || sess.time.minute)
        CTimeHMToString (sess.time, s, len + 1) ;
      else
        s [0] = '\0' ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 162 :  // F_NO and F_YEAR
      s [0] = '\0' ;
      if (sess.key.no || sess.key.year)
       {
        ostrstream (s, len + 1)
          << sess.key.no << "/" << sess.key.year << ends ;
        s [len] = '\0' ;
        writeString (os, s, len, algn, skip) ;
       }
	break ;
    case 163 :
      s [0] = '\0' ;
	if (sess.key.no || sess.key.year)
       {
        ostrstream oss (s, len + 1) ;

        oss << sess.key.no << "/" ;
        oss.fill ('0') ;
        oss << setw (2) << (sess.key.year % 100) << ends ;
        s [len] = '\0' ;
        writeString (os, s, len, algn, skip) ;
       }
      break ;
    case 165 :  // lawsuit's sender   (2165)
      s [0] = '\0' ;
	{
	   TLawsuit lawsuit ;
         if (TRCDKeyToLawsuit (sess.key, lawsuit))
	   {
             TLawsuit * pLawsuit = new TLawsuit ;
             pLawsuit -> key = lawsuit.key;
		 char connType[2]; connType[0]= CONNECT_TYPE_LOWER_INSTANCE; connType[1] = '\0';
		 TConnect *c = get_connect(pLawsuit, connType);
             if ( c != NULL )
             {
                senderUCNAndUCNTypeToName ( c -> ucn, c -> ucnType, s, len + 1);
             }
             else
             {  // "Първоинстанционно дело" към входящия документ.
                 if (lawsuit.source.type && strchr (TYPE_REQUESTS, lawsuit.source.type) &&
	  		    	       TRCDKey_ (lawsuit.source).FilledOK ())
                 {
                    TRequest req ;
                    req.key = lawsuit.source ;
                    if (req.Get ())
                    {
                        TRequest * pReq = new TRequest ;
                        pReq -> key = req.key;
                        c = get_connect(pReq, connType);
                        if ( c != NULL )
                        {
                            senderUCNAndUCNTypeToName ( c -> ucn, c -> ucnType, s, len + 1);
                        }
                        delete pReq;
             }   }  }
             delete c;
		 delete pLawsuit;
         }
      }
	// 20070114 <-- almost the same as 166
      writeString (os, s, len, algn, skip) ;
      break ;
    case 166 :  // lawsuit's regionXX data
# if INSTANCE
      s [0] = '\0' ;
	// Използва се в "СПИСЪК на ...., които ще се разглеждат ..."
	// колона "Първоинстанционно дело"
      {
         // ? Търсим точно "Първоинстанционно дело" към делото или вх.док. ?
         TLawsuit lawsuit ;
         if (TRCDKeyToLawsuit (sess.key, lawsuit))
         {
             TLawsuit * pLawsuit = new TLawsuit ;
             pLawsuit -> key = lawsuit.key;
             char connType[2]; connType[0]= CONNECT_TYPE_LOWER_INSTANCE; connType[1] = '\0';
             TConnect *c = get_connect(pLawsuit, connType);
             if ( c != NULL )
             {
                ostrstream oss (s, len + 1) ;
                oss.fill ('0') ;
                oss << c -> connectNo << "/" ;
                oss << setw (2) << (c -> connectYear % 100) << ends ;
                s [len] = '\0' ;
             }
		 else
             {  // "Първоинстанционно дело" към входящия документ.
                 if (lawsuit.source.type && strchr (TYPE_REQUESTS, lawsuit.source.type) &&
	  		    	       TRCDKey_ (lawsuit.source).FilledOK ())
                 {
                    TRequest req ;
                    req.key = lawsuit.source ;
			  if (req.Get ())
                    {
                        TRequest * pReq = new TRequest ;
				pReq -> key = req.key;
                        c = get_connect(pReq, connType);
                        if ( c != NULL )
                        {
                            ostrstream oss (s, len + 1) ;
                            oss.fill ('0') ;
                            oss << c -> connectNo << "/" ;
                            oss << setw (2) << (c -> connectYear % 100) << ends ;
                            s [len] = '\0' ;
                        }
                        delete pReq;
             }   }  }
             delete c;
             delete pLawsuit;
         }
	} // 20070114 <--
	writeString (os, s, len, algn, skip) ;
# else  // of INSTANCE
	writeString (os, "", len, algn, skip) ;
# endif  // of INSTANCE

	break ;
    case 167 :  // lawsuit's sender short (boza) name
# if INSTANCE
	 {
	  bool found = false ;
	  TLawsuit lawsuit ;

	  if (TRCDKeyToLawsuit (sess.key, lawsuit))
	  {
	    TSender sender ;
	    char bozaName [sizeof (sender.name)] ;

	    // 20070114 --> almost the same as 166 (2166)
          {
             TLawsuit * pLawsuit = new TLawsuit ;
             pLawsuit -> key = lawsuit.key;
             char connType[2]; connType[0]= CONNECT_TYPE_LOWER_INSTANCE; connType[1] = '\0';
             TConnect *c = get_connect(pLawsuit, connType);
		 if ( c != NULL )
             {  sender.ucnType = c -> ucnType;
                if ( sender.ucnType )
                {
			  scSprintf (sizeof (sender.ucn), sender.ucn, "%s",  c -> ucn) ;
                    // get sender
                    if (sender.Get ())
			  {

                       if (lower_regional_boza_short_name (sender, bozaName, sizeof (bozaName)))
                       {   found = true ;
                           writeString (os, bozaName, len, algn, skip) ;
             }  }   }  }
             else
             {  // "Първоинстанционно дело" към входящия документ.
                 if (lawsuit.source.type && strchr (TYPE_REQUESTS, lawsuit.source.type) &&
	  		    	       TRCDKey_ (lawsuit.source).FilledOK ())
                 {
                    TRequest req ;
                    req.key = lawsuit.source ;
                    if (req.Get ())
                    {
                        TRequest * pReq = new TRequest ;
                        pReq -> key = req.key;
                        c = get_connect(pReq, connType);
                        if ( c != NULL )
                        {
                           sender.ucnType = c -> ucnType;
                           if ( sender.ucnType )
                           {
                              scSprintf (sizeof (sender.ucn), sender.ucn, "%s",  c -> ucn) ;

					// get sender
                              if (sender.Get ())
                              {

                                 if (lower_regional_boza_short_name (sender, bozaName, sizeof (bozaName)))
                                 {   found = true ;
                                  writeString (os, bozaName, len, algn, skip) ;
                        }  }  }  }
                        delete pReq;
             }   }  }
             delete c;
             delete pLawsuit;
          }// 20070114 <-- almost the same as 166

        } // if (TRCDKeyToLawsuit (sess.key, lawsuit))
        if (! found)
          writeString (os, "", len, algn, skip) ;
       }
# else  // of INSTANCE
      writeString (os, "", len, algn, skip) ;
# endif  // of INSTANCE
      break ;
    case 168 :  //
# if INSTANCE
       {
        TLawsuit lawsuit ;

        s [0] = '\0' ;
	  if (TRCDKeyToLawsuit (sess.key, lawsuit))
        // 20070114 in comment  compositionToString (lawsuit.regionComposition, s, len + 1) ;
        { // 20070114 -->  almost the same as 166
	     TLawsuit * pLawsuit = new TLawsuit ;
           pLawsuit -> key = lawsuit.key;
           char connType[2]; connType[0]= CONNECT_TYPE_LOWER_INSTANCE; connType[1] = '\0';
           TConnect *c = get_connect(pLawsuit, connType);
           if ( c != NULL )
           {
              compositionToString (c -> composition, s, len + 1) ;
           }
           else
           {  // "Първоинстанционно дело" към входящия документ.
              if (lawsuit.source.type && strchr (TYPE_REQUESTS, lawsuit.source.type) &&
 		    	       TRCDKey_ (lawsuit.source).FilledOK ())
              {
                  TRequest req ;
		  req.key = lawsuit.source ;
                  if (req.Get ())
                  {
                     TRequest * pReq = new TRequest ;
                     pReq -> key = req.key;
                     c = get_connect(pReq, connType);
                     if ( c != NULL )
                     {
                        compositionToString (c -> composition, s, len + 1) ;
                     }
                     delete pReq;
	     }   }  }
           delete c;
           delete pLawsuit;
	} // 20070114 <-- almost the same as 166
        writeString (os, s, len, algn, skip) ;
       }
# else  // of INSTANCE
      writeString (os, "", len, algn, skip) ;
# endif  // of INSTANCE
      break ;
  case 170 :
  case 176 :
      s [0] = '\0' ;
       {
        SessPtr pNextSess ;

        if (thereIsNextSession (sess, pNextSess.Ref (),
				KIND_CLOSED_SESSIONS))
         {
          if (dCp == 170)
            writeDate (os, pNextSess.Ref ().date, len, algn, skip) ;
          else
           {
            CTimeHMToString (pNextSess.Ref ().time, s, len + 1) ;
	    writeString (os, s, len, algn, skip) ;
           }
         }
        else
	    writeString (os, s, len, algn, skip) ;
       }
      break ;
    case 171 :  // F_JUDGE
      citizenUCNAndUCNTypeToName (sess.judge, UCN_CITIZEN_UCN,
                                  s, len + 1, true, true, false) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 172 :  // Walns
    case 173 :  // Walns
    case 174 :  // Walns
    case 175 :  // Walns
	 {
        TSideWind pattern ;
	const int num = (dCp == 172 || dCp == 173) ? 1 : -1 ;

	  pattern.involvement = STUB_CHAR ;
	  ostrstream (pattern.ucn, sizeof (pattern.ucn))
	    << (dCp == 172 || dCp == 174 ? "1000000000" : "1010000000")
	    << ends ;
	pattern.ucn [sizeof (pattern.ucn) - 1] = '\0' ;
	// LPR: WTF is this for - write into s if it _does not_ fit in s?!
	if (len < WRITE_STRING_LEN_AS_IS)
	 {
	  container_sides_to_string (s, len + 1, sess, pattern, num, num) ;
	  writeString (os, s, len, algn, skip) ;
	 }
	else
	  container_sides_to_ostream (os, sess, pattern, num, num) ;
	 }
	break ;
    case 177 :
	CDateForStatePaperToString (sess.date, s, len + 1) ;
	writeString (os, s, len, algn, skip) ;
	break ;
    case 178 :
	if (sess.time.hour || sess.time.minute)
	CTimeHMForStatePaperToString (sess.time, s, len + 1) ;
      else
        s [0] = '\0' ;
      writeString (os, s, len, algn, skip) ;
      break ;
  case 179 :
  case 180 :
      s [0] = '\0' ;
       {
        SessPtr pNextSess ;

        if (thereIsNextSession (sess, pNextSess.Ref (),
                                KIND_CLOSED_SESSIONS))
         {
	  if (dCp == 180)
           {
            CDateForStatePaperToString (pNextSess.Ref ().date, s, len + 1) ;
            writeString (os, s, len, algn, skip) ;
           }
          else
           {
            CTimeHMForStatePaperToString (pNextSess.Ref ().time, s, len + 1) ;
            writeString (os, s, len, algn, skip) ;
           }
         }
        else
          writeString (os, s, len, algn, skip) ;
       }
      break ;
    // Last : 180
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // sessionOutput

bool sideOutput (const TSideWind & side, ostream & os, char * s,
		     const size_t len, const int algn,
		     const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;

  switch (((long int) dCiph) - OFFSET_SIDE_CIPHER)
   {
    case 41 :
      writeLong (os, side.key.no, len, algn) ;
      break ;
    case 42 :
      writeLong (os, side.key.year, len, algn) ;
      break ;
    case 43 :
      typeToString (side.key.type, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 44 :
      involvementToString (side.involvement, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 45 :
      writeString (os, "", len, algn, skip) ;
      break ;
    case 46 :
      writeString (os, side.ucn, len, algn, skip) ;
      break ;
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // sideOutput

bool begNEndNYearOutput (const begNEndNYear & bneny,
                         ostream & os, char * s,
				 const size_t len, const int algn,
                         const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;

  switch (((long int) dCiph) - OFFSET_BEGN_ENDN_YEAR_CIPHER)
   {
    // See also begNEndNYearTypesOutput if You add new cases to this
    // switch
    case 50 :
      writeLong (os, bneny.begN, len, algn) ;
      break ;
    case 51 :
      writeLong (os, bneny.endN, len, algn) ;
      break ;
    case 52 :
      writeLong (os, bneny.year, len, algn) ;
      break ;
    case 57 :
      if (s && len)
       {
        ostrstream oss (s, len + 1) ;

        if (bneny.begN == bneny.endN)
          oss << "No " << bneny.begN ;
        else
          oss << "от No " << bneny.begN << " до No " <<  bneny.endN ;
        oss << ends ;
        s [len] = '\0' ;
        writeString (os, s, len, algn, skip) ;
       }
      break ;
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // begNEndNYearOutput

bool begNEndNYearTypesOutput (const begNEndNYearTypes & bnenyT,
                              ostream & os, char * s,
                              const size_t len, const int algn,
                              const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output  

  if (begNEndNYearOutput (bnenyT, os, s, len, algn, skip, dCiph))
    return true ;

  bool perceived = true ;
  const long int d_c = ((long int) dCiph) -
                       OFFSET_BEGN_ENDN_YEAR_CIPHER ;

  switch (d_c)
   {
    // See also begNEndNYearTypesACOutput if You add new cases to this
    // switch
    case 53 :
    case 54 :
    case 55 :  // 28.jul.2000
    case 56 :  // 28.jul.2000
    case 59 :  // 07.sep.2000
    case 60 :  // 07.sep.2000
       {
        ostrstream oss (s, len + 1) ;

        s [0] = '\0' ;  // 07.sep.2000
         {
          const size_t tSz = strlen (bnenyT.types) + 1 ;
          char * const t = new char [tSz] ;

          scSprintf (tSz, t, "%s", bnenyT.types) ;
          difference (t, TYPE_INREGS) ;
          types_ToPlural (oss, t, d_c == 53 || d_c == 55 || d_c == 59) ;
          delete [] t ;
         }
        if (d_c == 59 || d_c == 60)
          if (strcmp (bnenyT.types, TYPE_INREGS))
            oss << " дела" ;
        if (d_c == 55 || d_c == 56)
          if (strstr (bnenyT.types, TYPE_INREGS))
           {
            oss << ", " ;

            const size_t fidsLen = strlen (FLYING_INREG_DOCS_STRING) ;
            bool inserted = false ;
            int j = 0 ;

            for (int i = 0 ;
                 i < fidsLen && j < 3 ;
                 i ++)
             {
              if (FLYING_INREG_DOCS_STRING [i] == ' ' && d_c == 56 &&
                  inserted == false)
               {
                oss << "те" ;
                inserted = true ;
               }
              oss << FLYING_INREG_DOCS_STRING [i] ;
              if (FLYING_INREG_DOCS_STRING [i] == ' ')
                j ++ ;
             }
           }
	  oss << ends ;
        s [len] = '\0' ;
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 58 :  // 07.sep.2000
      if (bnenyT.types [0] && strcmp (bnenyT.types, TYPE_INREGS))
        writeString (os, "за документи по обжалване на",
                     len, algn, skip) ;
      break ;
    case 61 :  // 07.sep.2000
      if (strstr (bnenyT.types, TYPE_INREGS))
       {
        ostrstream oss (s, len + 1) ;

        if (strcmp (bnenyT.types, TYPE_INREGS))
          oss << "и " ;
        oss << FLYING_INREG_DOCS_STRING ;
        oss << ends ;
        writeString (os, s, len, algn, skip) ;
       }
      break ;
    case 64 :
    case 65 :  // 27.sep.2001
# if INSTANCE
#   if APPEAL
      if (d_c == 65)  // 18.mar.2002 - begin
        begNEndNYearTypesOutput (bnenyT, os, s, len, algn, skip,
          53 + OFFSET_BEGN_ENDN_YEAR_CIPHER) ;  // 18.mar.2002 - end
#   else  // APPEAL
      if (bnenyT.types [0])
       {
        const char * medW = NULL ;

//        if (DEFAULT_UCLP == PLOVDIV_UCLP ||  // 21.nov.2001
//            d_c == 65)  // 27.sep.2001       // 21.nov.2001 comment
          if (bnenyT.want1stInstLaws != bnenyT.want2ndInstLaws)
            medW = bnenyT.want1stInstLaws ?
              "първоинстанционни" : "второинстанционни" ;

         {
          ostrstream oss (s, len + 1) ;

          s [0] = '\0' ;
          if (d_c == 65)  // 27.sep.2001 - begin
           {
            types_ToPlural (oss, bnenyT.types) ;
            if (medW)
              oss << " " << medW ;
           }
          else  // 27.sep.2001 - end
            lawTypeToPlural (oss, bnenyT.types [0], true, medW) ;
          oss << ends ;
         }
        s [len] = '\0' ;
        writeString (os, s, len, algn, skip) ;
       }
#   endif  // of APPEAL
# endif  // of INSTANCE
      break ;
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // begNEndNYearTypesOutput

bool begNEndNYearTypesACOutput (const begNEndNYearTypesAC & bneny,
                         ostream & os, char * s,
                         const size_t len, const int algn,
                         const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output  
  if (begNEndNYearTypesOutput (bneny, os, s, len, algn, skip, dCiph))
    return true ;

  bool perceived = true ;

  switch (((long int) dCiph) - OFFSET_BEGN_ENDN_YEAR_AC_CIPHER)
   {
    case 62 :
      writeDate (os, bneny.begDate, len, algn, skip) ;
      break ;
    case 63 :
      writeDate (os, bneny.endDate, len, algn, skip) ;
      break ;
    case 66 :
      s [0] = '\0' ;
       {
        ostrstream oss (s, len + 1) ;
        if (bneny.year)
          oss << "От No " << bneny.begN << " до No "
                          << bneny.endN << " за " << bneny.year << "г." ;
        else
          oss << "От " << bneny.begDate << "г. до "
                       << bneny.endDate << "г." ;
        oss << ends ;
        s [len] = '\0' ;
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 67 :  // 29.may.2003
      s [0] = '\0' ;
      if(bneny.kinds[0])
       {
        ostrstream oss (s, len + 1) ;

        kinds_to_plural (oss, bneny.kinds, true) ;
        oss << ends;
        s[len] = '\0';
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 68 :  // 04.aug.2003
      s [0] = '\0' ;
      if(bneny.begDate.Empty() == false && bneny.begDate == bneny.endDate)
      {
        ostrstream(s, len + 1) << "за " << bneny.begDate << " г." << ends;
        s[len] = '\0';
      }
      else
        period_to_string (bneny.begDate, bneny.endDate, s, len + 1,
          true, true) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 69 :  // 15.sep.2003
      {
        CDate tmp ((char)(bneny.begN), (char)(bneny.endN), bneny.year) ;

        if (tmp.Valid ())
          writeDate (os, tmp, len, algn, skip) ;
        else
          writeString (os, "", len, algn, skip) ;
      }
      break ;
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // begNEndNYearTypesACOutput

void tell_the_user_off(ostream& os, begMYEndMYTypeCIU bMYEndMYTypeCIUO)
{  // 01.nov.2002
  const char* const inconsistentData = "ПРОТИВОРЕЧИВИ ДАННИ ЗА ";
  const char* const checkTerminated = "ПРЕКЪСНАТА ПРОВЕРКА НА ";
  const char* const finishedLaws = "СВЪРШЕНИТЕ ДЕЛА!\n";
  const char* const sessions = "ЗАСЕДАНИЯТА!\n";

  if(bMYEndMYTypeCIUO.finLawsInconsistent)
    writeString(os, inconsistentData, WRITE_STRING_LEN_AS_IS);
  else
    if(bMYEndMYTypeCIUO.finLawsCheckTerminated)
      writeString(os, checkTerminated, WRITE_STRING_LEN_AS_IS);
  if(bMYEndMYTypeCIUO.finLawsInconsistent ||
    bMYEndMYTypeCIUO.finLawsCheckTerminated)
      writeString (os, finishedLaws, WRITE_STRING_LEN_AS_IS);

  if(bMYEndMYTypeCIUO.sessInconsistent)
    writeString(os, inconsistentData, WRITE_STRING_LEN_AS_IS);
  else
    if(bMYEndMYTypeCIUO.sessCheckTerminated)
      writeString (os, checkTerminated, WRITE_STRING_LEN_AS_IS);
  if(bMYEndMYTypeCIUO.sessInconsistent ||
    bMYEndMYTypeCIUO.sessCheckTerminated)
      writeString (os, sessions, WRITE_STRING_LEN_AS_IS) ;
}  // tell_the_user_off

bool collCompBegDEndDBegNOutput (const collCompBegDEndDBegN & cCBDEDBN,
                                 ostream & os, char * s,
                                 const size_t len, const int algn,
                                 const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output  

  bool perceived = true ;
  const long int d_cph = ((long int) dCiph) - OFFSET_C_C_BD_ED_BN_CIPHER ;

  switch (d_cph)
   {
    case 171 :
      if (PRINT_COLLEGE) // 20070703
      {
        collegeToString (cCBDEDBN.colleges [0], s, len + 1) ;
        writeString (os, s, len, algn, skip) ;
      }
      break ;
    case 172 :
      if ((cCBDEDBN.composit > 0) && (use_area_compositions ()))
      {
        compositionToString (cCBDEDBN.composit, s, len + 1) ;
        writeString (os, s, len, algn, skip) ;
      }
      break ;
    case 173 :
      CDateToString (cCBDEDBN.begD, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 174 :
      CDateToString (cCBDEDBN.endD, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 175 :
      writeULong (os, cCBDEDBN.begN, len, algn) ;
      break ;
    case 176 :
    case 177 :
    case 185 :  // 26.may.2000
      if (s && len)
       {
        ostrstream oss (s, len + 1) ;
        const char t = cCBDEDBN.colleges [0] ;

        s [0] = '\0' ;
        if (t)
         {
# if INSTANCE
#   if APPEAL
          const char * const medW = NULL ;  // "" ;  // 23.jun.2004
#   else  // of APPEAL
          const char * const medW = t == TYPE_FIRM_LAW ? "" :
             (cCBDEDBN.firstInstLaws ? "първоинстанционни" :
               (
                 t == TYPE_PUNISHMENT_LAW &&  // 30.jan.2002 - begin
                 cCBDEDBN.all2ndPuniLaws ?
                   "въззивни и касационни" :  // 30.jan.2002 - end
                (
                 (t == TYPE_ADMIN_LAW ||
                  (t == TYPE_PUNISHMENT_LAW &&
                   cCBDEDBN.puniAdmConcernLaws)) ?
                   "касационни" :
                  "въззивни"
                )
               )
             ) ;
#   endif  // of APPEAL
# endif
          lawTypeToPlural (oss, t,
                           d_cph == 176 || d_cph == 185
# if INSTANCE
                               , medW
# endif  // of INSTANCE
                                                                 ) ;
# if INSTANCE
#   if APPEAL
          if (d_cph != 185 && cCBDEDBN.keepingMethodTimeBook)
            oss << " - " << KEEPING_METHOD_PRT_STRING ;
#   endif  // of APPEAL
# endif  // of INSTANCE
          oss << ends ;
         }
        s [len] = '\0' ;
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 178 :
    case 179 :
      if (s && len)
       {
        ostrstream oss (s, len + 1) ;
        const char t = cCBDEDBN.colleges [0] ;

        s [0] = '\0' ;
        if (t)
         {
          lawTypeToPlural (oss, t, d_cph == 178) ;
          oss << ends ;
         }
        s [len] = '\0' ;
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 180 :
      if (cCBDEDBN.begD.year == cCBDEDBN.endD.year &&
          cCBDEDBN.begD == firstDayOfThisMonth (cCBDEDBN.begD) &&
          cCBDEDBN.endD == lastDayOfThisMonth (cCBDEDBN.endD)
         )
       {
        const int bM = cCBDEDBN.begD.month ;
        const int eM = cCBDEDBN.endD.month ;
        const int months = eM - bM + 1 ;
        char tmp [32] ;
        ostrstream oss (tmp, sizeof (tmp)) ;

        if (months > 0 && months %6 == 0)
         {

          if (months == 6)
            oss << (bM == 1 ? "  I  " : "  II ") ;
          else
            oss << "I, II" ;
         }
        else
          oss << "     " ;
        oss << "      " << cCBDEDBN.begD.year << ends ;
        tmp [sizeof (tmp) - 1] = '\0' ;
        writeString (os, tmp, len, algn, skip) ;
       }
      break ;
    case 181 :
    case 182 :
      if (cCBDEDBN.begD.month == cCBDEDBN.endD.month &&
          cCBDEDBN.begD.year == cCBDEDBN.endD.year)
       {
        char tmp [32] ;

         {
          ostrstream oss (tmp, sizeof (tmp)) ;

	    monthOutput (oss, s, 255, LEFT_ALIGN, 0, cCBDEDBN.begD.month) ;
	    oss << ends ;
         }
        tmp [sizeof (tmp) - 1] = '\0' ;

        char line [SIZE_OF_LINE] ;

        ostrstream (line, sizeof (line)) << "Месец " << tmp << " "
                                         << cCBDEDBN.begD.year
                                         << "г." << ends ;
        line [sizeof (line) - 1] = '\0' ;
        if (d_cph == 182)
          outNormDblStrkStringAtCondStream (os, line, len, algn, skip) ;
        else
          writeDoubleStrikedString (os, line, len, algn, skip) ;
       }
      break ;
    case 183 :
      s [0] = '\0' ;
# if INSTANCE
      if (cCBDEDBN.whichPartOfDay != WholeDay)
       {
        ostrstream (s, len + 1)
          << "("
          << (cCBDEDBN.whichPartOfDay == Morning ?
                "сутрешни" : "следобедни") << " заседания)" << ends ;
        s [len] = '\0' ;
       }
# endif
      writeString (os, s, len, algn, skip) ;
      break ;
    case 184 :
      s [0] = '\0' ;
# if INSTANCE
      if (cCBDEDBN.whichPartOfDay != WholeDay)
       {
        ostrstream (s, len + 1)
          << "("
          << (cCBDEDBN.whichPartOfDay == Morning ?
                "преди" : "след") << " обяд)" << ends ;
        s [len] = '\0' ;
       }
# endif
      writeString (os, s, len, algn, skip) ;
      break ;
    case 186 :
# if INSTANCE
#   if APPEAL
      if (cCBDEDBN.keepingMethodTimeBook)
        writeString (os, KEEPING_METHOD_PRT_STRING, len, algn, skip) ;
#   endif
# endif
      break ;
    case 187 :
      writeULong (os, cCBDEDBN.begD.year, len, algn) ;
      break ;
    case 188 :
      compositionToStringCompl (cCBDEDBN.composit, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 189 :
      writeLong (os, cCBDEDBN.begNo, len, algn) ;
      break ;
    case 190 :
      writeLong (os, cCBDEDBN.endNo, len, algn) ;
      break ;
    case 191 :
      writeULong (os, cCBDEDBN.year, len, algn) ;
      break ;
    case 192 :
      s [0] = '\0' ;
       {
        ostrstream oss (s, len + 1) ;

        types_ToPlural (oss, cCBDEDBN.colleges) ;
        oss << ends ;
        s [len] = '\0' ;
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 193 :  // 01.nov.2002
      tell_the_user_off (os, cCBDEDBN) ;
      break ;
    // Last : 193
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // collCompBegDEndDBegNOutput

bool monthOutput (ostream & os, char * // s
			, const size_t len,
			const int algn, const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  bool perceived ;
  TMonth month ;

  if (dCiph == OFFSET_MONTH_CIPHER + 12 + 1)
  {
	strcpy(month.name, "Месец");
	perceived = true;
  }
  else
  {
	month.month = dCiph ;
	perceived = month.Try();
  }
  if (perceived)
    writeString (os, month.name, len, algn, skip) ;
  return perceived ;
 }  // monthOutput

bool citizenOutput (const TCitizen & citizen, ostream & os, char * s,
                 const size_t len, const int algn,
                 const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output  

  bool perceived = true ;

  switch (((long int) dCiph) - OFFSET_CITIZEN_CIPHER)
   {
    // the codes 101 .. 108 are not processing in firmLikeCitizenOutput
    case 91 :
    case 101 :
    case 99 :  // Also for BULSTAT in ...LikeCitizenOutput
      writeString (os, citizen.ucn, len, algn, skip) ;
      break ;
    case 92 :
    case 102 :
      writeString (os, citizen.name, len, algn, skip) ;
      break ;
    case 93 :
    case 103 :
      writeString (os, citizen.reName, len, algn, skip) ;
      break ;
    case 94 :
    case 104 :
      writeString (os, citizen.family, len, algn, skip) ;
      break ;
    case 95 :
    case 105 :
      writeString (os, citizen.reFamily, len, algn, skip) ;
      break ;
    case 96 :
    case 106 :
      countryToName (citizen.country, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 97 :
    case 107 :
      countryToName (citizen.reCountry, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 98 :
    case 108 :
      ostrstream (s, len + 1)
        << citizen.name << " " << citizen.reName << " "
        << citizen.family << " "
        << citizen.reFamily << ends ;
      s [len] = '\0' ;
       {
        const size_t l = strlen (s) ;

        if (l && s [l - 1] == ' ')
          s [l - 1] = '\0' ;
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // citisenOutput

bool firmLikeCitizenOutput (const TFirm & firm, ostream & os,
                            char * s, const size_t len, const int algn,
                            const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  return ucnGroupLikeCitizenOutput (firm, os, s, len, algn,
                                    skip, dCiph,
                                    OFFSET_FIRM_LIKE_CITIZEN_CIPHER) ;
 }  // firmLikeCitisenOutput

# if INSTANCE
bool damnLikeCitizenOutput (const TDamn & damn, ostream & os,
                            char * s, const size_t len, const int algn,
                            const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  return ucnGroupLikeCitizenOutput (damn, os, s, len, algn,
                                    skip, dCiph,
                                    OFFSET_DAMN_LIKE_CITIZEN_CIPHER,
                                    true) ;
 }  // damnLikeCitisenOutput

bool damnOutput (const TDamn & damn, ostream & os,
                 char * //s
                 , const size_t len, const int algn,
                 const size_t skip, int dCiph
		     , const TDecision * pDecis  // 16.jun.2000
                )
 // returns true if dCiph is known (perceived)
 {
  if (! (len //&& s
        ))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;

  const long int dCphD = ((long int) dCiph) - OFFSET_DAMN_CIPHER ;

  switch (dCphD)
   {
    case 1 :  // F_UCN
      writeString (os, damn.ucn, len, algn, skip) ;
      break ;
    case 2 :  // F_UCN_TYPE
      break ;
    case 3 :  // F_NAME
      writeString (os, damn.name, len, algn, skip) ;
      break ;
    case 4 :  // F_COMPANY_TYPE
      break ;
    case 5 :  // F_TAX_NO
      break ;
    case 6 :  // F_BULSTAT
      break ;
    case 7 :  // F_NO
      writeLong (os, damn.key.no, len, algn) ;
      break ;
    case 8 :  // F_YEAR
      writeLong (os, damn.key.year, len, algn) ;
      break ;
    case 9 :  // F_TYPE
      break ;
    case 10 : // F_ENTER_DATE
      break ;
    case 11 : // F_REGISTER
    case 12 : // F_VOLUME
    case 13 : // F_PAGE
    case 14 : // F_PARTITION
    case 15 : // F_LEVEL
	 {
	  const TDecision * pDecLoc = pDecis ;
	  TDecision * p_dec = new TDecision ;
	  long int valToWrt = 0 ; ;

	  clearGroupData (* p_dec) ;
	  if (pDecLoc == NULL)
	   {
	    // It is VERY STUPID to search up to 5 times (one time for each
	    // code) the same decision.
	    // However, I'm supporting this possibility.
	    TLawsuit law ;

	    law.key = damn.key ;
	    if (extract_first_decision (law, * p_dec))
		pDecLoc = p_dec ;
	   }
	  if (pDecLoc)
	    switch (dCphD)
	     {
		case 11 :
		  valToWrt = pDecLoc -> registry ;
		  break ;
		case 12 :
		  valToWrt = pDecLoc -> volume ;
		  break ;
		case 13 :
		  valToWrt = pDecLoc -> page ;
		  break ;
		case 14 :
		  valToWrt = pDecLoc -> partition ;
		  break ;
		case 15 :
		  valToWrt = pDecLoc -> level ;
		  break ;
	     }
	  if (valToWrt)
	    writeLong (os, valToWrt, len, algn) ;
	  else
	    writeString (os, "", len, algn) ;
	  delete p_dec ;
	 }
	break ;
    default :
	perceived = false ;
	break ;
   }
  return perceived ;
 }  // damnOutput

# endif

bool miscOutput (ostream & os, char * s,
		     const size_t len, const int algn,
                 const size_t skip, int dCiph
                 , TWindow * pParent   // 20060120
                 )
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;
  const long int dCp = ((long int) dCiph) - OFFSET_MISC_CIPHER ;

  switch (dCp)
   {
    case 0 :
      writeString (os, "", len) ;
      break ;
    case 1 :
    case 15 :
    case 17 :
      thisCourtToString (s, len + 1) ;
      if (dCp == 15)
       {
        for (int i = 0 ; i < strlen (s) ; i ++)
          s [i] = ' ' ;
       }
      if (dCp == 17)
       {
        mixed_case_cyr_str (s, true) ;
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 2 :
    case 18 :
      //if (uclpToName (DEFAULT_UCLP, s, len + 1))  // 05.may.2004
      if (uclpToName (COURT_UCLP, s, len + 1))  // 05.may.2004
      {
        if (dCp == 18)
         {
          const char * const c = "Гр." ;

          mixed_case_cyr_str (s, true) ;
          // Blur a cosmetic defect
          if (strncmp (s, c, strlen (c)) == 0)
           {
            s [0] = 'г' ;
           }
         }
        writeString (os, s, len, algn, skip) ;
      }
      break ;
    case 3 :
      //if (uclpToAreaName (DEFAULT_UCLP, s, len + 1))  // 05.may.2004
      if (uclpToAreaName (COURT_UCLP, s, len + 1))  // 05.may.2004
        writeString (os, s, len, algn, skip) ;
      break ;
    case 4 :
      //if (uclpToMunicipalityName (DEFAULT_UCLP, s, len + 1))  // 05.may.2004
      if (uclpToMunicipalityName (COURT_UCLP, s, len + 1))  // 05.may.2004
        writeString (os, s, len, algn, skip) ;
      break ;
    case 5 :
	 {
	  sprintf(s, "%d.%s.%ld", Today.day, Month->Seek(Today.month), Today.year);
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 6 :  // 04.may.2000
      writeString (os, AppNameMinistry, len, algn, skip) ;
      break ;
    case 7 :  // 05.may.2000
	writeString (os, AppNameMinistryShort, len, algn, skip) ;
      break ;
    case 8 :  // 16.jun.2000
      writeString (os, AppNameTheMinistry, len, algn, skip) ;
      break ;
    case 9 :  // 13.dec.2000
# if INSTANCE
#   if APPEAL
      writeString (os, "ОС", len, algn, skip) ;
#   else  // of APPEAL
      writeString (os, "РС", len, algn, skip) ;
#   endif  // of APPEAL
# else  // of INSTANCE
      writeString (os, "", len, algn, skip) ;
# endif  // of INSTANCE
      break ;
    case 10 :  // 06.apr.2001 - begin      // judgedi.fma // Председател:
       {
        char ucn [sizeof (Default -> ucn)] ;

        s [0] = '\0' ;
        if (user_ucn_at_unique_post (ucn, sizeof (ucn), POST_CHIEF))
          citizenUCNAndUCNTypeToName (ucn, UCN_CITIZEN_UCN, s, len + 1,
                                      true, true, false) ;
        writeString (os, s, len, algn, skip) ;
       }
      break ;
    case 11 :                              // Изготвил сведението; Секретар;
      s [0] = '\0' ;
      // 20070702
      if (SIGN_SUMMONS)
      {  citizenUCNAndUCNTypeToName (Default -> ucn, UCN_CITIZEN_UCN, s,
                                                  len + 1, true, true, false) ;
      }
      writeString (os, s, len, algn, skip) ;
      break ;  // 06.apr.2001 - end
    case 12 :
      s [0] = '\0' ;
      //uclpToName (DEFAULT_UCLP, s, len + 1) ;  // 05.may.2004
      uclpToName (COURT_UCLP, s, len + 1) ;  // 05.may.2004
       {
        const size_t sL = strlen (s) ;

        if (sL < len)
	   {
	    scprintf(s, "  %d.%s.%ldг.", Today.day, Month->Seek(Today.month), Today.year);
	   }
	 }
	writeString (os, s, len, algn, skip) ;
	break ;
    case 13 :
    { // 2011:132 LPR: was sysDate
	mstr m;
	Today.Print(m, "%0D");
	writeString (os, str(m), len, algn, skip) ;
	break ;
    }
    case 14 :
       {
        char tmp [128] ;

        //uclpToName (DEFAULT_UCLP, tmp, sizeof (tmp)) ;  // 05.may.2004
        uclpToName (COURT_UCLP, tmp, sizeof (tmp)) ;  // 05.may.2004
        ostrstream (tmp + strlen (tmp), sizeof (tmp) - strlen (tmp))
          << " " << Today << " г." << ends ;
        tmp [sizeof (tmp) - 1] = '\0' ;
        writeString (os, tmp, len, algn, skip) ;
       }
      break ;
    case 16 :
       {
        char tmp [32] ;

        CDateToString (Today, tmp, sizeof (tmp)) ;
        if (len - skip > 0)
          writeString (os, tmp, len - skip, algn, skip) ;
       }
      break ;
    case 19 :   // 20070123
       {
         bool res = false;
         char tmp [SIZE_OF_EMAIL + 10] ;
         TAddress CourtAddress;
         clearGroupData (CourtAddress) ;
         strcpy(CourtAddress.ucn, INSTANCE_SELF);
         CourtAddress.ucnType = UCN_INSTANCE_SELF;
         if ( (CourtAddress.Try ()))
           if(CourtAddress.email && CourtAddress.email[0])
           {
             ostrstream (tmp, sizeof (tmp))
                << "E-MAIL " << CourtAddress.email << ends ;
             tmp [sizeof (tmp) - 1] = '\0' ;
             res = true;
           }
         if (!res)
         {
             ostrstream (tmp, sizeof (tmp))
                << "E-MAIL ......................................    " << ends ;
             tmp [sizeof (tmp) - 1] = '\0' ;
         }
         writeString (os, tmp, len, algn, skip) ;
       }
      break ;

      // 20071012 -->
      case 20:
        {
          CharPtr tmp_s (len + 1) ;
          ZERO_LEN(tmp_s);

          char hline[118]; memset (hline, '\0', sizeof hline);
          for ( int j = 0; j < 116; j++ ) hline[j] = '-';

          ostrstream oss(tmp_s, tmp_s.Size());
          if (PRINT_PAGE_BREAK)
               oss << char(0x0C) ; // << char(0x23);
          else oss << char(0x0D) << char(0x0A) << hline
                   << char(0x0D) << char(0x0A); // << char(0x23) ;
          oss << ends;
          writeString(os, tmp_s, len, algn, skip);
        }
        break;
      case 21:
        {
            CharPtr tmp_s (len + 1) ;
            ZERO_LEN(tmp_s);

		if (PRINT_PAGE_BREAK)
            {
              ostrstream oss(tmp_s, tmp_s.Size());
              //  oss << "<p style='page-break-before:always'>&nbsp;</p>" ;//const char* const dfltPgBrk =
              oss << "<div style=\"page-break-before: always;\">"
                    "<!--[if IE 7]><br style=\"height:0; line-height:0\"><![endif]-->"
                    "   <!--p style=\"background-color:red;height:600px;\">Page 2</p>-->"
                    "</div>" ;
              //oss << "<br>" ;
              writeString(os, tmp_s, len, algn, skip);
            }
            else
            {
              char hline[118]; memset (hline, '\0', sizeof hline);
              for ( int j = 0; j < 116; j++ ) hline[j] = '-';
              ostrstream oss(tmp_s, tmp_s.Size());
              memset(tmp_s, '\0',  tmp_s.Size());
              oss << "<table width=\"100%\"><tr><td width=\"100%\">";
              oss << hline;
              oss << "</td></tr></table>";
              oss << ends;
              writeString(os, tmp_s, len, algn, skip);
            }
        }
        break;
      // 20071012 <--


    // Last: 21

    case 255 :
      writeString (os, "\n", len, algn) ;
      break ;
    case 254 :
      writeString (os, ".", len, algn) ;
      break ;
    case 253 :
      writeString (os, "¦", len, algn) ;
      break ;
    case 252 :
      writeString (os, "/", len, algn) ;
      break ;
    case 251 :
      writeString (os, "год", len, algn, skip) ;
      break ;
    case MISC_OUTPUT_GIVEN_NUM :
      writeLong (os, skip, len, algn) ;
      break ;
    case 249 :
      if (s && len)
       {
        s [0] = (char) skip ;
        s [1] = '\0' ;
        writeString (os, s, len >= skip ? len - skip : len, algn) ;
       }
      break ;
    case MISC_OUTPUT_STRING_UNCHANGED :  // 248
    case MISC_OUTPUT_STRING_AS_COLUMN :  // 247
      writeString (os, s, len, algn, skip) ;
      break ;
    case MISC_OUTPUT_STRING_AS_COLUMN_BR :  // 246  // 27.aug.2003
      if (s [0])
       {
        //writeString (os, s, len, algn, skip) ;  // 04.mar.2004
        string_to_html_ostream_allowCode (os, s) ;  // 04.mar.2004
        html_br (os, false, false) ;
       }
      break ;
// 20060120 -->
    case MISC_OUTPUT_EXTRA_TXT :   //  257
      if (pParent)
      {
         int textSize = 1024;
         CharPtr text(textSize);
         text.End();
         if(strcmp(text, "\n") == 0)
            ZERO_LEN(text);
         ZERO_LEN(s);
         if(TAnyLetterTextDialog(pParent, text, text.Size(),
            "Допълнителен текст към съобщението", true).Execute() == IDOK)
         {
            column* pColumn = new column(len);

            if( (pColumn)&&(text.Size()>0) )
            {
               pColumn->addText(text);
               const size_t rows = pColumn->getRowsCount() ;
               pColumn->reWind () ;

               for (size_t i = 0 ; i < rows ; i ++)
               {
                  s [0] = 0 ;
                  scSprintf(len+1, s, "%s", pColumn->nextRowOrEmptyStr());
                  strDelLeadingSpaces(s);
                  os << setiosflags (ios :: left) ;
                  os << setw (len);
                  os << s << " \r\n";
                  //writeString (os, s, len, 1,0) ;
                  //writeString (os, " \r\n", len, 1,0) ;
               }
               delete pColumn;
            }
      }  }
      break ;
// 20060120 <--

    case MISC_OUTPUT_STRING_TO_HTML_SWPBR :  // 245  // 04.mar.2004
      string_to_html_ostream_allowCode (os, s) ;
      break ;
    case 312 :
      writeString (os, "Азбучник", len, algn, skip) ;
      break ;
    case 313 :
      writeString (os, "Срочна книга", len, algn, skip) ;
      break ;
    case 314 :
      writeString (os, "Списък на лицата за призоваване",
                   len, algn, skip) ;
      break ;
    case 315 :
      writeString (os, "Описна книга", len, algn, skip) ;
      break ;
    case 316 :
      writeString (os, "Отчет", len, algn, skip) ;
      break ;
    case 318 :
      writeString (os, "Книга закрити заседания", len, algn, skip) ;
      break ;
    case 322 :
      writeString (os, "Списък на насрочените дела за дата",
                   len, algn, skip) ;
      break ;
    case 323 :
      writeString (os, "Отчет II инстанция", len, algn, skip) ;
      break ;
    case 324 :
      writeString (os, "Списък на страните по дело", len, algn, skip) ;
      break ;
    case 325 :
      writeString (os, "Справка по съдии", len, algn, skip) ;
      break ;
    case 326 :
      ZERO_LEN(s);
      {
        bool res = false;
        {
          ostrstream oss(s, len + 1);

          court_logo_image_file(oss);
          if(oss.good())
          {
            if(oss.tellp() < len)
            {
              res = true;
            }
            oss << ends;
          }
        }
        s[len] = '\0';
        if(res == false)
        {
          ZERO_LEN(s);
        }
      }
      writeString (os, s, len, algn, 0) ;
      break ;
    default :
      perceived = false ;
      break ;
   }  // last: ...
  return perceived ;
 }  // miscOutput

bool begMYEndMYTypeCIUOutput (const begMYEndMYTypeCIU
                               & rBMYEndMYTypeCIUO,
                              ostream & os, char * s, const size_t len,
                              const int algn, const size_t skip,
                              int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;
  const long int dCph = ((long int) dCiph) -
                        OFFSET_BMY_EMY_T_C_IU_CIPHER ;

  switch (dCph)
   {
    case 1 :
      monthOutput (os, s, len, algn, skip,
                   (size_t) (rBMYEndMYTypeCIUO.begMonth) +
                               OFFSET_MONTH_CIPHER) ;
      break ;
    case 2 :
      writeLong (os, rBMYEndMYTypeCIUO.begYear, len, algn) ;
      break ;
    case 3 :
      monthOutput (os, s, len, algn, skip,
                   (size_t) (rBMYEndMYTypeCIUO.endMonth) +
                               OFFSET_MONTH_CIPHER) ;
      break ;
    case 4 :
      writeLong (os, rBMYEndMYTypeCIUO.endYear, len, algn) ;
      break ;
    case 5 :
      typeToString (rBMYEndMYTypeCIUO.types [0], s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 6 :
      writeString (os,
                   rBMYEndMYTypeCIUO.internalUse ?
                     "За вътрешно използване" : "" ,
                   len, algn, skip) ;
      break ;
    case 7 :
      if ( use_area_compositions () && (rBMYEndMYTypeCIUO.composition > 0))
      {
        ZERO_LEN( s );
        if (rBMYEndMYTypeCIUO.composition)
        {
           compositionToStringCompl(rBMYEndMYTypeCIUO.composition, s, len + 1) ;
           writeString (os, s, len, algn, skip) ;
        }
        else
          writeString (os, ALL_COMPOSITIONS, len, algn, skip) ;
      }
      break ;
    case 8 :
       {
        char bM [32] ;
        char eM [32] ;

	  monthToString (rBMYEndMYTypeCIUO.begMonth, bM, sizeof (bM)) ;
        monthToString (rBMYEndMYTypeCIUO.endMonth, eM, sizeof (eM)) ;
        if (rBMYEndMYTypeCIUO.begMonth == rBMYEndMYTypeCIUO.endMonth &&
            rBMYEndMYTypeCIUO.begYear == rBMYEndMYTypeCIUO.endYear)
          ostrstream (s, len + 1)
            << "за месец " << bM << " "
            << rBMYEndMYTypeCIUO.begYear << " година" << ends ;
        else
          ostrstream (s, len + 1)
            << "от месец " << bM << " "
            << rBMYEndMYTypeCIUO.begYear << " година до месец "
            << eM << " " << rBMYEndMYTypeCIUO.endYear << " година"
            << ends ;
        s [len] = '\0' ;
        writeString (os, s, len, algn, skip) ;
       }
      break ;
    case 9 :
# if INSTANCE
# else
        writeString (os, (rBMYEndMYTypeCIUO.composition ? "" : "и"),
                     len, algn, skip) ;
# endif
      break ;
    case 10 :
    case 11 :
    case 12 :
    case 26 :  // 20.mar.2002
    case 27 :  // 10.jun.2002
			 {
        ostrstream oss (s, len + 1) ;
        char t ;

        if (strlen (rBMYEndMYTypeCIUO.types) == 1)
          t = rBMYEndMYTypeCIUO.types [0] ;
        else
          if (strlen (rBMYEndMYTypeCIUO.types) > 0)
#if COURT_TYPE == COURT_ADMIN
		t = TYPE_ADMIN_LAW ;
#else
		t = TYPE_CITIZEN_LAW ;
#endif
          else
            t = '\0' ;
				if (dCph == 10 || dCph == 26 || dCph == 27)
					oss << "по " ;

				if (t)
					switch (dCph)
					 {
						case 12 :
							lawTypeToPlural (oss, t, false
# if INSTANCE
                               , rBMYEndMYTypeCIUO.firstInstLaws ?
                               "първоинстанционни" :
                               "въззивни"
# endif
                                                                 ) ;
              break ;
# if INSTANCE
            case 26 :
            case 27 :
							 {
                bool showMany = false ;

                if (strlen (rBMYEndMYTypeCIUO.types) > 1 &&
                    strchr (rBMYEndMYTypeCIUO.types,
                            TYPE_PUNISHMENT_LAW) == false
									 )
								 {
									const int l = strlen (rBMYEndMYTypeCIUO.types) ;

									if (rBMYEndMYTypeCIUO.firstInstLaws)
										showMany = l < 4 ; // 4 --> citizen, trade, firm, admin
									else
										if (dCph == 27)
										  showMany = true;
//                  else
//                    showMany = l < 3 ; // 3 --> citizen, trade, admin
                 }
                if (showMany)
                 {
                  types_ToPlural (oss, rBMYEndMYTypeCIUO.types, false) ;
                  oss << " дела" ;
                 }
                else
                  lawTypeToPlural (oss, t, false) ;
               }
              break ;
# else  // of INSTANCE
# endif  // of INSTANCE
            default :
              lawTypeToPlural (oss, t, false) ;
              break ;
           }
        oss << ends ;
        s [len] = '\0' ;
        if (dCph == 11 || dCph == 26 || dCph == 27)
         {
          for (int i = 0 ; s [i] ; i ++)
            s [i] = upCyr (s [i]) ;
         }
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 13 :
    case 14 :
# if INSTANCE
#   if APPEAL
      writeString (os, dCph == 13 ? "втора" : "ВТОРА" , len, algn, skip) ;
#   else  // of APPEAL
       {
        const char * inst ;

        if (rBMYEndMYTypeCIUO.firstInstLaws)
          if (dCph == 13)
            inst = "първа" ;
          else
            inst = "ПЪРВА" ;
        else
          if (strlen (rBMYEndMYTypeCIUO.types) == 1 &&
              rBMYEndMYTypeCIUO.types [0] == TYPE_ADMIN_LAW)
            if (dCph == 13)
              inst = "касационна" ;
            else
              inst = "КАСАЦИОННА" ;
          else
            if (dCph == 13)
              inst = "втора" ;
            else
              inst = "ВТОРА" ;
        writeString (os, inst, len, algn, skip) ;
       }
#   endif  // of APPEAL
# endif  // of INSTANCE
      break ;
    case 15 :
      if (rBMYEndMYTypeCIUO.begYear == rBMYEndMYTypeCIUO.endYear)
       {
        const int bM = rBMYEndMYTypeCIUO.begMonth ;
        const int eM = rBMYEndMYTypeCIUO.endMonth ;
        const int months = eM - bM + 1 ;
        char tmp [32] ;
        ostrstream oss (tmp, sizeof (tmp)) ;

        if (months > 0 && months %6 == 0)
         {

          if (months == 6)
            oss << (bM == 1 ? "  I  " : "  II ") ;
          else
            oss << "I, II" ;
         }
        else
          oss << "     " ;
        oss << "      " << rBMYEndMYTypeCIUO.begYear << ends ;
        tmp [sizeof (tmp) - 1] = '\0' ;
        writeString (os, tmp, len, algn, skip) ;
       }
      break ;
    case 16 :
# if INSTANCE // 12.apr.2001 - begin
        if (rBMYEndMYTypeCIUO.decidedLawsJudgeCheckup)
          writeString (os, "Разпределение по съдии на свършените",
                       len, algn, skip) ;
        else
# endif  // of INSTANCE  // 12.apr.2001 - end
          writeString (os, "Разпределение по съдии на", len, algn, skip) ;
      break ;
    case 17 :
       {
        const size_t tmpSz = len + 512 + 1 ;
        char * tmp = new char [tmpSz] ;

        tmp [tmpSz - 1] = '\0' ;
        ostrstream (tmp, tmpSz - 1)
          << (* (rBMYEndMYTypeCIUO.lArrPtr ())) << ends ;
        if (len < MISC_SMALLEST_SPECIAL_CIPHER && strlen (tmp) > len)
          error ("Не мога да изпиша целия списък от предмети") ;
        writeString (os, tmp, len, algn, skip) ;
        delete [] tmp ;
       }
      break ;
    case 18 :
    case 19 :
    case 20 :  // 16.feb.2001
      s [0] = '\0' ;
       {
        ostrstream oss (s, len + 1) ;

        oss << (dCph == 18 || dCph == 20 ? "по " : "") ;
        types_ToPlural (oss, rBMYEndMYTypeCIUO.types,
                        dCph == 18 ? false : true) ;
        oss << " дела" << ends ;
       }
      s [len] = '\0' ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 21 :  // 04.apr.2001 - begin
    case 22 :
       {
        const int ind = dCph - 21 ;

	  if (rBMYEndMYTypeCIUO.lArrPtr () -> Count () > ind)
          writeLong (os, (* (rBMYEndMYTypeCIUO.lArrPtr ())) [ind],
                     len, algn) ;
       }
      break ;
    case 23 :
    case 24 :
       {
        const CDate rangeDate =
          nextDay (dCph == 23 ? rBMYEndMYTypeCIUO.BegDate () :
                                rBMYEndMYTypeCIUO.EndDate ()) ;

        ostrstream (s, len + 1) << rangeDate << ends ;
        s [len] = '\0' ;
        writeString (os, s, len, algn, skip) ;
       }
      break ;  // 04.apr.2001 - end
    case 25 :  // 03.may.2001
       {
        const int ind = dCph - 23 ;  // look at 'case 22 :'

	  if (rBMYEndMYTypeCIUO.lArrPtr () -> Count () > ind)
          writeLong (os, (* (rBMYEndMYTypeCIUO.lArrPtr ())) [ind],
                     len, algn) ;
       }
      break ;
    case 28 :  // 27.aug.2002
      writeDate (os,
        CDate (rBMYEndMYTypeCIUO.endDay, rBMYEndMYTypeCIUO.endMonth,
          rBMYEndMYTypeCIUO.endYear),
        len, algn, skip) ;
      break ;
    case 29:  // 28.aug.2002
      s [0] = '\0' ;
      if (rBMYEndMYTypeCIUO.types [0])
       {
        ostrstream oss (s, len + 1) ;

        types_ToPlural (oss, rBMYEndMYTypeCIUO.types,
                        dCph == 18 ? false : true) ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
        if(rBMYEndMYTypeCIUO.firstInstLaws)
          oss << " първоинстанционни" ;
        else
          if(rBMYEndMYTypeCIUO.forBoss)
          {
            if(strlen (rBMYEndMYTypeCIUO.types) == 1 &&
               rBMYEndMYTypeCIUO.types [0] == TYPE_ADMIN_LAW)
              oss << " касационни" ;
            else
              oss << " второинстанционни" ;
          }
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE
        oss << " дела" << ends ;
       }
      s [len] = '\0' ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 30 :  // 01.nov.2002
      tell_the_user_off (os, rBMYEndMYTypeCIUO) ;
      break ;
    // 20070703 -->
    case 31 :
	  if (PRINT_COLLEGE)
          {
              s [0] = '\0' ;

              collegeToString (rBMYEndMYTypeCIUO.types [0], s, len + 1) ;

              strConcat (s, " ", len + 1) ;
              strConcat (s, COLLEGE, len + 1) ;
              strConcat (s, " ", len + 1) ;
              writeString (os, s, len, algn, skip) ;
          }

      break ;
    // 20070703 <--
    // Last : 31
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // begMYEndMYTypeCIUOutput

bool collBegDEndDBegNOutput (const collBegDEndDBegN & rCollBegDEndDBegN,
				     ostream & os, char * s, const size_t len,
				     const int algn, const size_t skip,
				     int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output  

  bool perceived = true ;

  switch (((long int) dCiph) - OFFSET_COL_BD_ED_BN_CIPHER)
   {
    case 1 :
      if (PRINT_COLLEGE) // 20070703
      {
          collegeToString (rCollBegDEndDBegN.college, s, len + 1) ;
          writeString (os, s, len, algn, skip) ;
      }
      break ;
    case 2 :
      CDateToString (rCollBegDEndDBegN.begD, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 3 :
      CDateToString (rCollBegDEndDBegN.endD, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 4 :
      writeLong (os, rCollBegDEndDBegN.begN, len, algn) ;
      break ;
    case 5 :
       {
        collCompBegDEndDBegN cCBDEDBN ;

        cCBDEDBN << rCollBegDEndDBegN ;
        collCompBegDEndDBegNOutput (cCBDEDBN, os, s, len, algn,
                                    skip, 8178) ;
       }
      break ;
    case 6 :
      compositionToStringCompl (rCollBegDEndDBegN.composition, s,
                                len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // collBegDEndDBegNOutput

bool regionalArealOutput (ostream & os, char *  // s
				  ,
				  const size_t len, const int algn,
				  const size_t skip, int dCiph
				  , int composition // 20051228
				  )
 // returns true if dCiph is known (perceived)
 {
//  info("regionalArealOutput %d", dCiph);
  if (! (len //&& s
	  ))
    return true ;  // To stop any subsequent calls of ....Output

    bool perceived = true ;
    long int dCiphLI = ((long int) dCiph) - OFFSET_REGIONAL_AREAL_OUTPUT;

    switch (dCiphLI)
    {
		// 2016:106
		case 1 :
		case 2 :
		{
			if (PRINT_COLLEGE)
			{
				mstr m;

				m.printf("%s ", COLLEGE);

				if (dCiphLI == 2)
					*ncstr(m) = toupper(*str(m));

				writeString (os, str(m), len, algn, skip) ;
			}

			break;
		}
#if COURT_TYPE == COURT_APPEAL
        case 3 :
          writeString (os, "АПЕЛАТИВНИТЕ", len, algn, skip) ;
          break ;
	  case 4 :
          break ;
        case 5 :
          break ;
        case 6 :
	    writeString (os, "ФОРМА СП-4А", len, algn, skip) ;
	break ;
    case 7 :
	writeString (os, "ФОРМА СП-5А", len, algn, skip) ;
	break ;
    case 8 :
      writeString (os, "Апелативен", len, algn, skip) ;
	break ;
    case 9 :
      writeString (os, "До АПЕЛАТИВЕН СЪД", len, algn, skip) ;
      break ;
    case 10 :
      writeString (os, "АПЕЛАТИВЕН", len, algn, skip) ;
      break ;
    case 11 :   // 20051227
	writeString (os, "Апелативен Съд", len, algn, skip) ;
      break ;
    case 12 :   // 20051227
      if ( ( composition ) &&
           ( use_area_compositions () )  // 20070806
	 )
	{
         char buff [32] ;
	   const size_t buffL = sizeof (buff) - 1 ;
	   buff [buffL] = '\0' ;
	   compositionToString (composition, buff, buffL) ;
	   writeString (os, ", ", len, algn, skip) ;
	   writeString (os,  buff, len, algn, skip ) ;
	   writeString (os, " състав,", len, algn, skip) ;
	}
	break ;
#endif  // APPEAL
#if COURT_TYPE == COURT_AREAL
    case 3 :
	writeString (os, "ОКРЪЖНИТЕ", len, algn, skip) ;
	break ;
    case 4 :
	writeString (os, "ФОРМА СП-1А", len, algn, skip) ;
	break ;
    case 5 :
	writeString (os, "ФОРМА СП-3", len, algn, skip) ;
	break ;
    case 6 :
	writeString (os, "ФОРМА СП-4", len, algn, skip) ;
	break ;
    case 7 :
	writeString (os, "ФОРМА СП-5", len, algn, skip) ;
	break ;
    case 8 :
	writeString (os, "Окръжен", len, algn, skip) ;
	break ;
    case 9 :
	writeString (os, "До ОКРЪЖЕН СЪД", len, algn, skip) ;
	break ;
    case 10 :
	writeString (os, "ОКРЪЖЕН", len, algn, skip) ;
	break ;
    case 11 :  // 20051227
	writeString (os, "Окръжен Съд", len, algn, skip) ;
	break;
    case 12 :  // 20051227
	if ( ( composition ) &&
	     ( use_area_compositions () )
	 )
	{
	   char buff [32] ;
	   const size_t buffL = sizeof (buff) - 1 ;
	   buff [buffL] = '\0' ;
	   compositionToString (composition, buff, buffL) ;
	   writeString (os, ", ", len, algn, skip) ;
	   writeString (os,  buff, len, algn, skip ) ;
	   writeString (os, " състав,", len, algn, skip) ;
	}
	break ;
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
    case 3 :
	writeString (os, "ВОЕННИТЕ", len, algn, skip) ;
	break ;
    case 4 :
	writeString (os, "ФОРМА СП-1А", len, algn, skip) ;
	break ;
    case 5 :
	writeString (os, "ФОРМА СП-3", len, algn, skip) ;
	break ;
    case 6 :
	writeString (os, "ФОРМА СП-4", len, algn, skip) ;
	break ;
    case 7 :
	writeString (os, "ФОРМА СП-5", len, algn, skip) ;
	break ;
    case 8 :
	writeString (os, CourtType == COURT_MILITARY ? "Военен" : "Военно-апелативен", len, algn, skip) ;
	break ;
    case 9 :
	writeString (os, CourtType == COURT_MILITARY ? "До ВОЕНЕН СЪД" : "До ВОЕННО-АПЕЛАТИВЕН СЪД", len, algn, skip) ;
	break ;
    case 10 :
	writeString (os, CourtType == COURT_MILITARY ? "ВОЕНЕН" : "ВОЕННО-АПЕЛАТИВЕН", len, algn, skip) ;
	break ;
    case 11 :  // 20051227
	writeString (os, CourtType == COURT_MILITARY ? "Военен Съд" : "Военно-апелативен съд", len, algn, skip) ;
	break;
    case 12 :  // 20051227
	if ( ( composition ) &&
	     ( use_area_compositions () )
	 )
	{
	   char buff [32] ;
	   const size_t buffL = sizeof (buff) - 1 ;
	   buff [buffL] = '\0' ;
	   compositionToString (composition, buff, buffL) ;
	   writeString (os, ", ", len, algn, skip) ;
	   writeString (os,  buff, len, algn, skip ) ;
	   writeString (os, " състав,", len, algn, skip) ;
	}
	break ;
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
    case 3 :
      writeString (os, "РАЙОННИТЕ", len, algn, skip) ;
      break ;
    case 4 :
      writeString (os, "ФОРМА СП-1", len, algn, skip) ;
      break ;
    case 5 :
      writeString (os, "ФОРМА СП-2", len, algn, skip) ;
	break ;
    case 6 :
      break ;
    case 7 :
      break ;
    case 8 :
	writeString (os, "Районен", len, algn, skip) ;
      break ;
    case 9 :
      writeString (os, "До РАЙОНЕН СЪД", len, algn, skip) ;
      break ;
    case 10 :
      writeString (os, "РАЙОНЕН", len, algn, skip) ;
      break ;
    case 11 :  // 20051227
      writeString (os, "Районен Съд", len, algn, skip) ;
      break;
    case 12 :  // 20051227
      if ( ( composition ) &&
           ( use_area_compositions () )
	 )
      {
         char buff [32] ;
	   const size_t buffL = sizeof (buff) - 1 ;
         buff [buffL] = '\0' ;
         compositionToString (composition, buff, buffL) ;
	   writeString (os, ", ", len, algn, skip) ;
         writeString (os,  buff, len, algn, skip ) ;
         writeString (os, " състав,", len, algn, skip) ;
      }
      break ;
#endif  // REGIONAL
#if COURT_TYPE == COURT_ADMIN
    case 3 :
      writeString (os, "АДМИНИСТРАТИВНИТЕ", len, algn, skip) ;
      break ;
    case 4 :
      writeString (os, "ФОРМА СП-1", len, algn, skip) ;
	break ;
    case 5 :
      writeString (os, "ФОРМА СП-2", len, algn, skip) ;
      break ;
    case 6 :
      break ;
    case 7 :
      break ;
    case 8 :
	writeString (os, "Административен", len, algn, skip) ;
      break ;
    case 9 :
      writeString (os, "До АДМИНИСТРАТИВЕН СЪД", len, algn, skip) ;
      break ;
    case 10 :
      writeString (os, "АДМИНИСТРАТИВЕН", len, algn, skip) ;
      break ;
    case 11 :  // 20051227
      writeString (os, "Административен Съд", len, algn, skip) ;
      break;
    case 12 :  // 20051227
	if ( ( composition ) &&
	     ( use_area_compositions () )
	 )
	{
	   char buff [32] ;
	   const size_t buffL = sizeof (buff) - 1 ;
	   buff [buffL] = '\0' ;
	   compositionToString (composition, buff, buffL) ;
	   writeString (os, ", ", len, algn, skip) ;
	   writeString (os,  buff, len, algn, skip ) ;
	   writeString (os, " състав,", len, algn, skip) ;
	}
	break ;
#endif  // ADMIN
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // regionalArealOutput

bool addressOutput (const TAddress & a, ostream & os, char * s,
			  const size_t len, const int algn,
			  const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output  

  bool perceived = true ;
  const long int cph = ((long int) dCiph) - OFFSET_ADDRESS_CIPHER ;

  if (! (s))
    return false ;
  switch (cph)
   {  // 31, 32, 41 - 43 are processed in foreignAddrLikeAddrOutput, too
    case 1 :
    case 31 :
      if (citizenUCNAndUCNTypeToName (a.ucn, a.ucnType, s, len + 1))
        writeString (os, s, len, algn, skip) ;
      break ;
    case 2 :
    case 32 :
    case 47 :
      if (a.uclp && uclpToName (a.uclp, s, len + 1))
       {
        if (cph == 47)
         {
          const char * const c = "Гр." ;

          mixed_case_cyr_str (s, true) ;
          // Blur a cosmetic defect
          if (strncmp (s, c, strlen (c)) == 0)
           {
            s [0] = 'г' ;
           }
         }
        writeString (os, s, len, algn, skip) ;
       }
      break ;
    case 3 :
      if (a.street && a.uclp &&
          streetToName (a.street, a.uclp, s, len + 1))
        writeString (os, s, len, algn, skip) ;
      break ;
    case 4 :
        writeString (os, a.adrNo, len, algn, skip) ;
      break ;
    case 5 :
        writeString (os, a.subNo, len, algn, skip) ;
      break ;
    case 6 :
        writeString (os, a.entrance, len, algn, skip) ;
      break ;
    case 7 :
        writeString (os, a.floor, len, algn, skip) ;
      break ;
    case 8 :
        writeString (os, a.apartment, len, algn, skip) ;
      break ;
    case 9 :
      addressWithSettlStr_SB_ToString (a, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 10 :
    case 49 :
      addressWithStr_SB_ToString (a, s, len + 1) ;
      if (cph == 49)
       {
        mixed_case_cyr_str (s, true) ;
        // Blur some cosmetic defects
         {
          const char * const b = "Бул." ;

          if (strncmp (s, b, strlen (b)) == 0)
            s [0] = 'б' ;
         }
         {
          const char * const b = "Ул." ;

          if (strncmp (s, b, strlen (b)) == 0)
            s [0] = 'у' ;
         }
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 11 :
    case 41 :
      addressToString (a, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 12 :  // 25.feb.2000
    case 13 :  // 25.feb.2000
    case 42 :  // 25.feb.2000
    case 43 :  // 25.feb.2000
      addressToStringForSummons (a, s, len + 1, cph == 13 || cph == 43) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 14 :  // 25.feb.2000
    case 44 :  // 25.feb.2000
      settlStreetBlockNoToString (a, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 45 :
    case 48 :
      uclpToAreaName (a.uclp, s, len + 1) ;
      if (cph == 48)
        mixed_case_cyr_str (s, true) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 46 :
      uclpToMunicipalityName (a.uclp, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
      // Last : 49
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // addressOutput

bool foreignAddrLikeAddrOutput (const TForeignAddress & a,
					  ostream & os, char * s,
					  const size_t len, const int algn,
					  const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;
  const long int cph = ((long int) dCiph) -
                        OFFSET_FRGN_ADR_LIKE_ADDR_CIPHER ;

  if (! (s))
    return false ;
  switch (cph)
   {
    case 31 :
      if (citizenUCNAndUCNTypeToName (a.ucn, a.ucnType, s, len + 1))
        writeString (os, s, len, algn, skip) ;
      break ;
    case 32 :
      countryToName (a.country, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 41 :
    case 42 :  // 25.feb.2000
    case 43 :  // 25.feb.2000. I think, case 44 is wrong.
      writeString (os, a.foreign, len, algn, skip) ;
      break ;
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // addressOutput

bool Trade634DlgDataOutput (const Trade634DlgData & t364DD,
				    ostream & os, char * s,
				    const size_t len, const int algn,
				    const size_t skip, int dCiph)
 // returns true if dCiph is known (perceived)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;
  const long int cph =
    ((long int) dCiph) - OFFSET_TRADE_634_DIALOG_CIPHER ;

  switch (cph)
   {
    case 1 :
      if (! t364DD.begD.Empty ())
       {
        CDateToString (t364DD.begD,  s, len + 1) ;
        writeString (os, s, len, algn, skip) ;
	 }
      break ;
    case 2 :
      if (! t364DD.endD.Empty ())
       {
        CDateToString (t364DD.endD,  s, len + 1) ;
        writeString (os, s, len, algn, skip) ;
       }
      break ;
    case 3 :
      t364DD.lawKey.Print (s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 4 :
      typeToString (t364DD.lawKey.type, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 5 :
      writeLong (os, t364DD.lawKey.no, len, algn) ;
      break ;
    case 6 :
      writeLong (os, t364DD.lawKey.year, len, algn) ;
      break ;
    case 7 :  // 08.may.2002
       {
        bool printed = false ;

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
        if (t364DD.lawKey.FilledOK())
         {
          TLawsuit law ;

          law.key = t364DD.lawKey ;
          if (law.Get () && law.sourceKind &&
	      strchr (KIND_TRADE_BOZA_INDOCS, law.sourceKind))
           {
            writeString (os, law.text, len, algn, skip) ;
            printed = true ;
           }
         }
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE
        if (printed == false)
          writeString (os, "", len, algn, skip) ;
       }
      break ;
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // Trade634DlgDataOtput

bool inreg_output (const TInReg & inReg, ostream & os, char * s,
			 const size_t len, const int algn,
			 const size_t skip, int dCiph)
 {  // This function is stub yet ...
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;
  const long int cph =
    ((long int) dCiph) - OFFSET_INREG_CIPHER ;

  switch (cph)
   {
    case 1 :
       {
        TRCDKey_ key_ (inReg.key) ;
        s [0] = '\0' ;

        if (key_.FilledOK ())
          key_.Print (s, len + 1) ;
        writeString (os, s, len, algn, skip) ;
       }
      break ;
    case 2 :
      s [0] = '\0' ;
       {
        TRCDKeyContainer_ kC_ (inReg.key, inReg.kind) ;

        if (kC_.FilledOK ())
         {
          if (inReg.kind == KIND_OTHER_IN_DOCUMENT)
           {
            scSprintf (len + 1, s, "%s %ld/%ld",
                       inReg.text, inReg.key.no, inReg.key.year) ;
           }
          else
           {
            kC_.Print (s, len + 1) ;
            if (inReg.kind == KIND_IN_LETTER)
             {
              const size_t l = strlen (s) ;

              if (l < len)
                scSprintf (len + 1 - l, s + l, " (%s)", inReg.text) ;
             }
           }
         }
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 3 :
      s [0] = '\0' ;
       {
//        DatedContainer_ dC_ (inReg) ;  // 23.may.2001 - begin
        TRCDKeyContainerD_ dC_ (inReg.key, inReg.kind,
                                inReg.date) ;  // 23.may.2001 - end

        if (dC_.FilledOK ())
         {
          if (inReg.kind == KIND_OTHER_IN_DOCUMENT)
           {
            ostrstream oss (s, len + 1) ;

            oss << TRCDKey_ (inReg.key) << ", " << inReg.text
                << " от " << inReg.date << ends ;
           }
          else
           {
            dC_.Print (s, len + 1) ;
            if (inReg.kind == KIND_IN_LETTER)
             {
              const size_t l = strlen (s) ;

              if (l < len)
                scSprintf (len + 1 - l, s + l, " (%s)", inReg.text) ;
             }
	     }
         }
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 4 :
    case 5 :
      if (inReg.kind == KIND_OTHER_IN_DOCUMENT)
        scSprintf (len + 1, s, "%s", inReg.text) ;
      else
        kindWithArticleToString (inReg.kind, s, len + 1, cph == 4) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 6 :
      CDateToString (inReg.date, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 7 :
      kindToString (inReg.kind, s, len + 1) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 8 :
    case 9 :
    case 10 :
      s [0] = '\0' ;
       {
	  TSideWind waln ;

	  if (inreg_to_any_appealer (inReg, waln))
          switch (cph)
           {
            case 8 :
              ucnAndUcnTypeToName (waln.ucn, waln.ucnType, s, len + 1) ;
              break ;
            case 9 :
              involvementToString (waln.involvement, s, len + 1) ;
              break ;
            case 10 :
              involvementToString (waln.involvement, s, len + 1) ;
              strConcat (s, " ", len + 1) ;
               {
                const size_t l = strlen (s) ;

                if (l < len)
                  ucnAndUcnTypeToName (waln.ucn, waln.ucnType, s + l,
                                       len + 1 - l) ;
               }
              break ;
           }
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 11 :
      s [0] = '\0' ;
      if (strchr (TYPE_LAWSUITS, inReg.decision.type))
       {
        kindWithArticleToString (inReg.decisionKind, s, len + 1, false) ;
        strConcat (s, " по ", len + 1) ;
         {
          const size_t l = strlen (s) ;

          if (l < len)
           {
            TRCDKey_ lawKey (inReg.lawsuit) ;

            if (lawKey.FilledOK ())
             {
              TLawsuit law ;

              law.key = inReg.lawsuit ;
              if (law.Get ())
                TRCDKeyContainer_ (law.key,
                                   law.kind).Print (s + l, len + 1 - l) ;
             }
           }
         }
       }
      else
        if (strchr (TYPE_INREGS, inReg.decision.type))
         {
          scSprintf (len + 1, s, "%s", "Разпореждането по ") ;
          if (TRCDKey_ (inReg.decision).FilledOK ())
           {
            TInReg tmpInReg ;

            tmpInReg.key = inReg.decision ;
            if (tmpInReg.Get ())
             {
              const size_t l = strlen (s) ;

              if (l < len)
                TRCDKeyContainer_ (tmpInReg.key, tmpInReg.kind).
                    Print (s + l, len + 1 - l) ;
             }
           }
         }
        else
          if (strchr (TYPE_REQUESTS, inReg.decision.type))
           {
            scSprintf (len + 1, s, "%s", "Разпореждането по ") ;  // 20070901

            if (TRCDKey_ (inReg.lawsuit).FilledOK ())
             {
              TRequest r ;

              r.key = inReg.lawsuit ;
              if (r.Get ())
               {
                const size_t l = strlen (s) ;

                if (l < len)
                  TRCDKeyContainer_ (r.key,
                                     r.kind).Print (s + l, len + 1 - l) ;
               }
		 }
           }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 15 :
    case 18 :
      s [0] = '\0' ;
      if (strchr (TYPE_LAWSUITS, inReg.decision.type))
        kindWithArticleToString (inReg.decisionKind, s, len + 1, false) ;
      else
        if (strchr (TYPE_INREGS, inReg.decision.type))
         {
          scSprintf (len + 1, s, "%s", "Разпореждането ") ;
          if (cph == 15)
            strConcat (s, "по ", len + 1) ;
         }
        else
          if (strchr (TYPE_REQUESTS, inReg.decision.type))
           {
            scSprintf (len + 1, s, "%s", "Разпореждането ") ;   // 20070901
            if (cph == 15)
              strConcat (s, "по ", len + 1) ;
           }
      writeString (os, s, len, algn, skip) ;
      break ;

    case 27 : // COLLEGE
    case 26 : // състав
    case 16 : // точен вид дело и състав
      s [0] = '\0' ;
      {
         ZERO_LEN(s);
         bool addCompo=false;
         char buff [64] ;
         const size_t buffL = sizeof (buff) - 1 ;
         buff [buffL] = '\0' ;

         if (strchr (TYPE_LAWSUITS, inReg.decision.type))
         {
            TRCDKey_ lawKey (inReg.lawsuit) ;

            if (lawKey.FilledOK ())
            {
               TLawsuit law ;
               law.key = inReg.lawsuit ;
               if (law.Get ())
               {

                  if (cph == 16) TRCDKeyContainer_ (law.key, law.kind).Print(s, len + 1);// точен вид дело
                  // 20060302 -> printComposition
                  if ((cph == 16) || (cph == 26) ) // състав
                  {
                    if ( ( law.composition ) &&
			 ( use_area_compositions () )
                        )
		    {  addCompo= true;
                       compositionToString ( law.composition, buff, buffL) ;
                    }
                  }
                  // 20060302 <- printComposition

                  if (cph == 27)
                  {
                    if ( ( law.college )
                         &&
			                   (PRINT_COLLEGE)
                       )
                    {
                       ZERO_LEN(s);
                       collegeToString (law.college, s, len + 1) ;
                       strConcat (s, " ", len + 1) ;
                       strConcat (s, COLLEGE, len + 1) ;
                       strConcat (s, " ", len + 1) ;
                    }
                  }
               }
            }
         }
         else
           if (strchr (TYPE_INREGS, inReg.decision.type)  && (cph != 27))
           {
              if (TRCDKey_ (inReg.decision).FilledOK ())
              {
                 TInReg tmpInReg ;

                 tmpInReg.key = inReg.decision ;
                 if (tmpInReg.Get ())
                     TRCDKeyContainer_ (tmpInReg.key, tmpInReg.kind).
                 Print (s, len + 1) ;
              }
           }
           else
              if (strchr (TYPE_REQUESTS, inReg.decision.type)  && (cph != 27))
              {
                 if (TRCDKey_ (inReg.lawsuit).FilledOK ())
                 {
                     TRequest r ;

                     r.key.no = inReg.lawsuit.no ;
                     r.key.year = inReg.lawsuit.year ;
                     r.key.type = inReg.lawsuit.type ;
                     if (r.Try ())
                        TRCDKeyContainer_ (r.key, r.kind).Print (s, len + 1) ;
                 }
              }

         if (cph == 27) writeString (os, s, len, algn, skip) ;
         else
         {
            if ( addCompo )
            {

               if (cph == 16)
               {
                  char outbuf[SIZE_OF_LINE];
                  ostrstream (outbuf, sizeof outbuf ) << s << ", "
                                                     << buff <<" състав " << ends ;
                  writeString (os, outbuf, len, algn, skip) ;
               }
               else
               {
                  char outbuf[SIZE_OF_LINE];
                  ostrstream (outbuf, sizeof outbuf ) << buff <<" състав " << ends ;
                  writeString (os, outbuf, len, algn, skip) ;
               }
            }
            else
               writeString (os, s, len, algn, skip) ;
         }
      }
      break ;
    case 12 :
       {
        TRCDKey_ lawKey ;

        s [0] = '\0' ;
        if (inreg_to_law_type (inReg, & lawKey,  // 25.apr.2001
                               NULL, NULL, false   // 30.oct.2001
                              ) && lawKey.FilledOK ())
          lawKey.Print (s, len + 1) ;
        writeString (os, s, len, algn, skip) ;
       }
      break ;
    case 13 :  // Appealers with proxies, new lines
    case 19 :  // Appealers without proxies, new lines
    case 20 :  // Appealers without proxies
    case 21 :  // Appealers with proxies
      s [0] = '\0' ;
      if (TRCDKeyContainer_ (inReg.key, inReg.kind).FilledOK ())
       {
        column clmn (SIZE_OF_LINE - 1) ;
        CharPtr buffer (len + 1 >= 1024 ? 64 : 1024 ) ;
        char * const buff = buffer.Size () > len + 1 ? buffer.Str () : s ;
        const size_t buffSz =
          buffer.Size () > len + 1 ? buffer.Size () : len + 1 ;
        TInReg tmpIR ;

        tmpIR << const_cast <TInReg &> (inReg) ;
        AppealedLawsInRegColumns ::
          AddAppealersToColumn (& clmn, buff, buffSz, tmpIR,
                                cph == 13 || cph == 21) ;
         {
          const size_t rows = clmn.getRowsCount () ;

          if (len < WRITE_STRING_LEN_AS_IS)
           {
            s [0] = 0 ;
            for (size_t i = 0 ; i < rows ; i ++)
             {
              strConcat (s, clmn.nextRowOrEmptyStr (), len + 1) ;
              if (i + 1 < rows)
                if (cph == 20)
                  strConcat (s, ", ", len + 1) ;
                else
                  strConcat (s, "\r\n", len + 1) ;
             }
            if (s [0] && s [strlen (s) - 1] == '\r')
              s [strlen (s) - 1] = '\0' ;
            writeString (os, s, len, algn, skip) ;
           }
          else
           {
            for (size_t i = 0 ; i < rows ; i ++)
             {
              os << clmn.nextRowOrEmptyStr () ;
              if (i + 1 < rows)
                if (cph == 20)
                  os << ", " ;
                else
                  os << endl ;
             }
           }
         }
       }
      writeString (os, s, len, algn, skip) ;
      break ;
    case 14 :
      if (inReg.date.Empty () == false)  // 15.nov.2001 - begin
        ostrstream (s, len)
          << inReg.key.no << "/" << inReg.date << ends ;
      else  // 15.nov.2001 - end
        ostrstream (s, len)
          << inReg.key.no << "/" << inReg.key.year << ends ;
      s [len] = '\0' ;
      writeString (os, s, len, algn, skip) ;
			break ;
		case 17 :
		case 22 :
		case 23 :  // 01.jul.2002
		case 24 :  // 01.jul.2002
			s [0] = '\0' ;
			 {
				TLawsuit law ;

				if (inreg_to_law_type (inReg, & law.key, NULL, NULL, false))
					if (TRCDKey_ (law.key).FilledOK ())
						if (law.Get ())
						 {
							TRCDKeyContainer_ (law.key, law.kind).Print (s, len + 1) ;
							if (cph == 17)
							 {
								if (strlen (s) < len)
								 {
									CharPtr tmp (len + 1) ;

									preliminary_actions_to_string (tmp, tmp.Size (), law,
																C2S[UCN_INSTANCE_DETECT]) ;
									if (tmp [0])
									 {
										strConcat (s, ", ", len + 1) ;
										strConcat (s, tmp, len + 1) ;
									 }
								 }
							 }
# if INSTANCE  // of INSTANCE  // 01.jul.2002 - begin
							if (cph == 23 || cph == 24)
							 {
								CharPtr tmp (len + 1) ;
								CharPtr tmp1 (WRITE_STRING_LEN_AS_IS + 1) ;

								if (cph == 24)
									s [0] = '\0' ;  // Without lawsuit
								 {  // regionKind, region No, regionYear
									ostrstream tmpOss (tmp, tmp.Size ()) ;

									lawsuitOutput (law, tmpOss, tmp1, tmp1.Size () - 1,
										LEFT_ALIGN, 0, 51 + OFFSET_LAWSUIT_CIPHER) ;
									tmpOss << ends ;
								 }
								tmp [tmp.Size () - 1] = '\0' ;
								if (cph == 23)
									strConcat (s, ", ", len + 1) ;
								strConcat (s, tmp, len + 1) ;
								strConcat (s, " на ", len + 1) ;
								 {  // region court
									ostrstream tmpOss (tmp, tmp.Size ()) ;

									lawsuitOutput (law, tmpOss, tmp1, tmp1.Size () - 1,
										LEFT_ALIGN, 0, 52 + OFFSET_LAWSUIT_CIPHER) ;
									tmpOss << ends ;
								 }
								tmp [tmp.Size () - 1] = '\0' ;
								strConcat (s, tmp, len + 1) ;
							 }
# endif  // of INSTANCE  // 01.jul.2002 - end
						 }
			 }
			writeString (os, s, len, algn, skip) ;
			break ;
    case 25 :
      s [0] = '\0' ;
       {
        TRCDKeyContainer_ kC_ (inReg.key, inReg.kind) ;

        if (kC_.FilledOK ())
            kC_.Print (s, len + 1) ;
       }
			writeString (os, s, len, algn, skip) ;
			break ;

		// Last : 27
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // inreg_output

bool inreg_event_output (const TInRegEvent & inRegEvent, ostream & os,
												 char * s, const size_t len, const int algn,
				 const size_t skip, int dCiph)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;
  const long int cph =
    ((long int) dCiph) - OFFSET_INREG_EVENT_CIPHER ;

  switch (cph)
   {
    case 1 :
       {
        TRCDKey_ key_ (inRegEvent.key) ;
        s [0] = '\0' ;

        if (key_.FilledOK ())
          key_.Print (s, len + 1) ;
        writeString (os, s, len, algn, skip) ;
       }
      break ;
    case 2 :
			 {
        TRCDKeyContainer_ kC_ (inRegEvent.key, inRegEvent.kind) ;
        s [0] = '\0' ;

        if (kC_.FilledOK ())
					kC_.Print (s, len + 1) ;
        writeString (os, s, len, algn, skip) ;
       }
      break ;
    case 3 :
    case 4 :
       {
        const CDate d =
          cph == 3 ? inRegEvent.date : inRegEvent.date ;
// 23.may.2001 - begin
//        DatedContainer_ dC_ (inRegEvent.key.type, inRegEvent.key.no,
//                             inRegEvent.key.year, inRegEvent.kind, d) ;
        TRCDKeyContainerD_ dC_ (inRegEvent.key, inRegEvent.kind, d) ;
// 23.may.2001 - end
        s [0] = '\0' ;

        if (dC_.FilledOK ())
          dC_.Print (s, len + 1) ;
				writeString (os, s, len, algn, skip) ;
	 }
	break ;
    case 5 :
	CDateToString (inRegEvent.date, s, len + 1) ;
			writeString (os, s, len, algn, skip) ;
	break ;
    case 6 :
	resultToString (inRegEvent.result, s, len + 1) ;
	writeString (os, s, len, algn, skip) ;
	break ;
    case 7 :
	os << inRegEvent.text ;
	break ;
	case 8 :
	{
		const char * announceAction = "" ;

		if (inRegEvent.result)
		{
			if (strchr (RESULT_IRE_SENDS, inRegEvent.result))
				announceAction = "връчване" ;
			else if (strchr (RESULT_IRE_RETURNEDS, inRegEvent.result))
				announceAction = "връщане" ;
			// 2011:178
			else if (strchr(RESULT_TEXTUAL_RESOLUTIONS, inRegEvent.result))
				announceAction = NULL;
		}

		if (announceAction)
			writeString (os, announceAction, len, algn, skip) ;
		break ;
	}
	case 9 :
	{
		const char * announceAction = "        " ;

		if (inRegEvent.result)
		{
			if (strchr (RESULT_IRE_SENDS, inRegEvent.result))
				announceAction = "връчване" ;
			else if (strchr (RESULT_IRE_RETURNEDS, inRegEvent.result))
				announceAction = "връщане" ;
			// 2011:175
			else if (strchr(RESULT_TEXTUAL_RESOLUTIONS, inRegEvent.result))
				announceAction = NULL;
		}

		if (announceAction)
		{
			scSprintf (len + 1, s, "за %s на      ", announceAction) ;

			const size_t sL = strlen (s) ;

			if (sL < len)
			{
				TInReg inReg ;

				inReg.key = inRegEvent.key ;
				if (inReg.Get("F_KIND"))
					kindToString (inReg.kind, s + sL, len + 1 - sL) ;
			}

			writeString (os, s, len, algn, skip) ;
		}
		break ;
	}
	case 10 :
		writeString (os, inRegEvent.text, len, algn, skip) ;
		break ;
		case 11 :  // 02.jul.2002 - begin
		case 12 :
		case 13 :
			ostrstream (s, len + 1)
				<< "разп. " << inRegEvent.date << "г." << ends ;
			s [len] = '\0' ;
			if (cph == 11 || cph == 12)
			 {
# if INSTANCE
#   if APPEAL
				writeString (os, s, len, algn, skip) ;
#   else  // of APPEAL
				if (cph == 12)
					writeString (os, "", len, algn, skip) ;
#   endif  // of APPEAL
# else  // of INSTANCE
				if (cph == 12)
					writeString (os, "", len, algn, skip) ;
# endif  // of INSTANCE
			 }
			else
				writeString (os, s, len, algn, skip) ;
			break ;  // 02.jul.2002 - end
		// Last : 13
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // inreg_event_output

bool request_output (const TRequest & req, ostream & os,
			   char * s, const size_t len, const int algn,
			   const size_t skip, int dCiph)
{// Ref. sourceTypeToSenderUCNType
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;
  const long int cph =
    ((long int) dCiph) - OFFSET_REQUEST_CIPHER ;  // = 52000
  switch (cph)
   {
    case 1 :
      writeLong (os, req.key.no, len, algn) ;
      break ;
    case 2 :
      writeLong (os, req.key.year, len, algn) ;
      break ;
    case 3 :
      writeDate (os, req.date, len, algn, skip) ;
      break ;
    case 4 :
	kindToString (req.kind, s, len + 1) ;
	writeString (os, s, len, algn, skip) ;
      break ;
    case 5 :
    case 6 :
      kindWithArticleToString (req.kind, s, len + 1, cph == 5) ;
      writeString (os, s, len, algn, skip) ;
      break ;
    case 7 :
      s [0] = '\0' ;
# if INSTANCE
//#   if APPEAL  // 31.jan.2002 commented
//#   else  // of APPEAL  // 31.jan.2002 commented
      { // 20070114 -->  T_CONNECT
         TRequest * pReq = new TRequest ;
         pReq -> key = req.key;
	   char connType[2]; connType[0]= CONNECT_TYPE_LOWER_INSTANCE; connType[1] = '\0';
         TConnect *c = get_connect(pReq, connType);
         if ( c != NULL )
         { // 20070208 -->
           ostrstream oss (s, len + 1) ;
	     TConnectKind t;
	     t.connectKind = c -> connectKind;
	     if(t.connectKind && t.Try()) oss << t.name << " "; //(t.alias[0] ? t.alias : t.name) << " ";
	     oss << c -> connectNo << '/' << c -> connectYear<< " г." << ends;
	     oss << endl ;
	     s [len] = '\0' ;
         } // 20070208 <--
         delete c;
         delete pReq;
      } // 20070114 <--  T_CONNECT

//#   endif  // of APPEAL  // 31.jan.2002 commented
# else  // of INSTANCE
# endif  // of INSTANCE
      writeString (os, s, len, algn, skip) ;
      break ;
    case 8 :
      s [0] = '\0' ;
# if INSTANCE
#   if APPEAL
       {  // 31.jan.2002 - begin

	    // I will not implement this right now ...

	    // 2009:154 LPR: removed
	 }  // 31.jan.2002 - end
#   else  // of APPEAL
	 {
	  // 2009:154 LPR: removed
       }
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE
      writeString (os, s, len, algn, skip) ;
      break ;
    case 9 :
    case 10 :
      if (PRINT_COLLEGE)// 20070703
      {
	    collegeToString( cph == 9 ? request_doc_to_law_type (req) :
						  req.college, s, len + 1) ;
	    writeString (os, s, len, algn, skip) ;
      }
      break ;
    case 11 :
    case 12 :
       {
	  TRequest tmpReq ;

        tmpReq.key = req.key ;
        if (tmpReq.Get ())
         {
          reqRCDKeyToSidePair pairs (tmpReq, false) ;
          TClaimant cl ;
          TDefendant def ;
          const bool useS = len < 255 ;

          s [0] = '\0' ;
           {
            ostrstream oss (s, len + 1) ;

            for (unsigned i = 0 ;
                 i < pairs.getClaimantsCount () && pairs.nextPair (& cl,
                                                                   & def) ;
                 i ++)
		 {
              if (useS)
               {
                oss << cl.cName ;
                if ((i + 1) < pairs.getClaimantsCount ())
                 {
                  if (cph == 12)
                    oss << ", " ;
                  else
                    oss << endl ;
                 }
               }
              else
               {
                os << cl.cName ;
                if ((i + 1) < pairs.getClaimantsCount ())
                 {
                  if (cph == 12)
                    os << ", " ;
                  else
                    os << endl ;
                 }
               }
             }
            if (useS)
              os << ends ;
           }
          s [len] = '\0' ;
          if (useS)
            writeString (os, s, len, algn, skip) ;
         }
       }
      break ;
    case 13 :
    case 14 :
    case 15 :
    case 16 :
    case 17 :
    case 18 :
    case 19 :
      {
        s [0] = '\0' ;

	  if (s [0] == '\0')
        {
          ostrstream oss (s, len + 1) ;

          switch (cph)
           {
            case 13 :
              // Nothing. This is correct in general
              break ;
            case 14 :
              oss << "...." ;// This is correct, too
              break ;
            case 15 :
              oss << "Решение" ;
              break ;
            case 16 :
              oss << "Определение" ;
              break ;
            case 17 :
              oss << "Разпореждане" ;
              break ;
            case 18 :
              oss << "Споразумение" ;
              break ;
            case 19 :
              oss << "Присъда" ;
              break ;
           }
          oss << ends ;
          s [len] = '\0' ;
        }
        writeString (os, s, len, algn, skip) ;
      }
      break ;
    case 20 :
    case 21 :
       {
        CharPtr tmp (len + 1 + 128) ;

        kindWithArticleToString (req.kind, tmp, tmp.Size (), cph == 20) ;
        if (tmp [0])
          strtok (tmp, "-") ;
        writeString (os, tmp, len, algn, skip) ;
       }
	break ;
    case 22 :
    case 23 :
       {
        CharPtr tmp (len + 1 + 128) ;
        char numBuff [64] ;

        kindToString (req.kind, tmp, tmp.Size ()) ;
        if (tmp [0])
          strtok (tmp, "-") ;
        strConcat (tmp, " No ", tmp.Size ()) ;
        ostrstream (numBuff, sizeof (numBuff)) << req.key.no << ends ;
        numBuff [sizeof (numBuff) - 1] = '\0' ;
        strConcat (tmp, numBuff, tmp.Size ()) ;
        strConcat (tmp, "/", tmp.Size ()) ;
        ostrstream (numBuff, sizeof (numBuff)) << req.key.year << ends ;
        numBuff [sizeof (numBuff) - 1] = '\0' ;
        strConcat (tmp, numBuff, tmp.Size ()) ;
        strConcat (tmp, "г.", tmp.Size ()) ;
        if (cph == 23 && algn == RIGHT_ALIGN)
         {
          int l ;

					while ((l = strlen (tmp)) < len && l + 1 < tmp.Size ())
           {
            memmove (tmp + 1, tmp, l + 1) ;
            tmp [0] = (char) 160 ;
           }
         }
        writeString (os, tmp, len, algn, skip) ;
       }
      break ;
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // request_output

# if INSTANCE
# if APPEAL
# else  // of APPEAL
bool BankruptcyWideDlgData_output (const BankruptcyWideDlgData & dlgData,
					     ostream & os, char * s,
					     const size_t len, const int algn,
					     const size_t skip, int dCiph)
 {
  if (! (len && s))
    return true ;  // To stop any subsequent calls of ....Output

  bool perceived = true ;
  const long int cph =
    ((long int) dCiph) - OFFSET_BANKRUPTCY_DLG_DATA ;
  const size_t wrtL = len > skip ? len - skip : len ;

  switch (cph)
   {
    case 1 :
      writeDate (os, dlgData.completedBegDate, len, algn, skip) ;
      break ;
    case 2 :
      writeDate (os, dlgData.completedEndDate, len, algn, skip) ;
      break ;
    case 3 :
    case 4 :
    case 5 :
      period_to_string (dlgData.completedBegDate, dlgData.completedEndDate,
                        s, len + 1, cph > 3, cph > 4) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 6 :
    case 7 :
    case 8 :
       {
        if (dlgData.completedBegDate.Empty () &&
            dlgData.completedEndDate.Empty ())
          scSprintf (len + 1, s, "%s",
                     "ПРИКЛЮЧИЛИ ДЕЛА ПО НЕСЪСТОЯТЕЛНОСТ") ;
        else
          period_to_string (dlgData.completedBegDate,
                            dlgData.completedEndDate,
                            s, len + 1, cph > 6, cph > 7, true, false,
                            "ДЕЛА ПО НЕСЪСТОЯТЕЛНОСТ, ПРИКЛЮЧИЛИ") ;
        writeString (os, s, wrtL, algn, skip) ;
       }
      break ;
    case 9 :
    case 10 :
    case 11 :
      period_to_string (dlgData.lawBegDate, dlgData.lawEndDate,
                        s, len + 1, cph > 9, cph > 10) ;
      writeString (os, s, wrtL, algn, skip) ;
      break ;
    case 12 :
    case 13 :
    case 14 :
       {
        const char * const docName =
	    "ОБРАЗУВАНИ" ;

        period_to_string (dlgData.lawBegDate,
                          dlgData.lawEndDate,
                          s, len + 1, cph > 12, cph > 13, true, false,
                          docName) ;
        writeString (os, s, wrtL, algn, skip) ;
       }
      break ;
    case 15 :
      writeString (os, dlgData.yourOutNo, wrtL, algn, skip) ;
      break ;
    case 16 :  // By idea this code has to write anything only if
    case 17 :  // dlgData.wantExtraTitle is true
      if (dlgData.wantExtraTitle)
       {
        const char * const to = cph == 16 ? "ДО" : "" ;
        const char * const aNM = cph == 16 ? AppNameMinistry : "" ;
        const char * const toYON = cph == 16 ? "На Ваш изх. No " : "" ;
        const char * const yON = cph == 16 ? dlgData.yourOutNo : "" ;

        ostrstream (s, len + 1) << to << endl << aNM << endl << endl
          << toYON << yON << endl << endl << ends ;
        s [len] = '\0' ;
        writeString (os, s, wrtL, algn, skip) ;
       }
      break ;
    case 18 :
    case 19 :
    case 20 :
       {
        if (dlgData.completedBegDate.Empty () &&
            dlgData.completedEndDate.Empty ())
          s [0] = '\0' ;
        else
          period_to_string (dlgData.completedBegDate,
                            dlgData.completedEndDate,
                            s, len + 1, cph > 18, cph > 19, true, false) ;
        writeString (os, s, wrtL, algn, skip) ;
       }
      break ;
      // last : 20
    default :
      perceived = false ;
      break ;
   }
  return perceived ;
 }  // BankruptcyWideDlgData_output
# endif  // of APPEAL
# endif  // of INSTANCE
