#include "cac.h"  // 18.may.2001
//# include "all.h"

bool tSCountOutput (const collCompBegDEndDBegN * cCBEDBNPtr,
                    ostream & os, const size_t len, const int algn,
                    const size_t
# if INSTANCE
                                skip          
# endif
                                    , int dCiph
                    , const CodeValueArray * const pCodeValueArr
# if INSTANCE
                    , const TISArray <reportData> * const pRDArr
# endif
                   )
 // returns true if dCiph is known (perceived)
 {
  bool perceived = true ;
  char b [2] ;
  const char gN = GRANT_NONE_YET ;
  const char gE = GRANT_ENTIRE ;
  const char gP = GRANT_PARTIAL ;
  const char gR = GRANT_REJECTED ;
  const char * const kP = KIND_PSEUDO_SESSIONS ;
  const size_t buffL = 128 + 2 * countRecsInTable ("T_RESULT") ;
  const size_t buffSz = buffL + 1 ;
  char * buff = new char [buffSz] ;
  collBegDEndDBegN cBDEDBN ;
  const long int dCphr =
    ((long int) dCiph) - OFFSET_TIME_SUMMARY_CIPHER ;

  if (! buff)
    return false ;
  buff [0] = buff [buffL] = '\0' ;
  b [sizeof (b) - 1] = '\0' ;
  if (cCBEDBNPtr)  // 15.feb.2002
    cBDEDBN << (* cCBEDBNPtr) ;  // because of closed sess's book summary
  switch (dCphr)
   {
    case 181 :
      writeLong (os, sessionsForTSCount (RESULT_FIXEDS, cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;
      break ;
    case 182 :
      writeLong (os, sessionsForTSCount (RESULT_LEFTS, cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;
      break ;
    case 183 :
    case 282 :
    case 354 :  // 15.may.2000
	if (cCBEDBNPtr)  // 15.feb.2002
       {
        ostrstream (buff, buffL)
          << RESULT_CANCELEDS_OR_RETURNEDS << RESULT_SEND << ends ;

        const long int v0 =
          sessionsForTSCount (RESULT_WITH_DECISIONS, cCBEDBNPtr, NULL,
                              NULL, kP, false, NULL, NULL, dCphr == 183) ;

# if INSTANCE  // (patch)
        ostrstream (buff, buffL)
          << RESULT_DEFINITION_TERMINATING << ends ;

	  const long int v1 = sessionsForTSCount (buff, cCBEDBNPtr,
								NULL, NULL, kP, false,
								NULL, NULL,
								dCphr == 183, RESULT_CANCELAWS) ;
        const int v2 =
          dCphr == 354 ? cCBEDBNPtr -> extraVal0 : 0 ;  // 15.may.2000
# else
        const long int v1 = 0 ;
        const long int v2 = 0 ;  // 15.may.2000
# endif
        writeLong (os, v0 - v1 - v2
                     - definTermin_grantNone_cnt_patch (cCBEDBNPtr, NULL,
                                                        kP, dCphr == 183)
                                        , len, algn) ;
       }
      break ;
    case 355 :  // 16.may.2000
       {
        const long int v0 =
          sessionsForTSCount (RESULT_WITH_DECISIONS, cCBEDBNPtr, NULL,
                              NULL, kP, false, NULL, NULL, false,
                              NULL, true, true) ;

# if INSTANCE  // (patch)
        ostrstream (buff, buffL)
          << RESULT_DEFINITION_TERMINATING << ends ;

	  const long int v1 = sessionsForTSCount (buff, cCBEDBNPtr,
								NULL, NULL, kP, false,
								NULL, NULL, false,
                                                RESULT_CANCELAWS, true, true) ;
# else
        const long int v1 = 0 ;
# endif
        writeLong (os, v0 - v1, len, algn) ;
       }
      break ;
    case 184 :
    case 300 :
      writeLong (os,
                 sessionsForTSCount (RESULT_CANCELEDS, cCBEDBNPtr,
                                     NULL, NULL, kP, false,
                                         NULL, NULL, dCphr == 184),
                 len, algn) ;
      break ;
    case 185 :
      writeLong (os,
                 sessionsForTSCount (NULL, cCBEDBNPtr, AGE_UP_TO_THREES,
                                     NULL, kP, false),
                 len, algn) ;
      break ;
    case 186 :
      writeLong (os,
                 sessionsForTSCount (NULL, cCBEDBNPtr,
                                     AGE_MORE_THAN_THREES_ALL,
                                     NULL, kP, false),
                 len, algn) ;
      break ;
    case 187 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
#  if INSTANCE  // Area only  // 19.dec.2003
        writeLong (os, 0, len, algn) ;
#  else  // Region only
        char kR [2] ;

        kR [0] = KIND_RECONCILIATION_SESSION ;
        kR [1] = '\0' ;
        writeLong (os,
                   sessionsForTSCount (//RESULT_LEFT_RECONCILES,
                                       RESULT_LEFTS,  // I'm guessing...
                                       cCBEDBNPtr,
                                       NULL, NULL,
                                       kR),  // I'm guessing...
                     len, algn) ;
# endif
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;

// 20060310 ->
    case 610 :
#  if INSTANCE
#  else  // Region only
      {
         char t_buff[128] ;
         ostrstream (t_buff, strlen(t_buff) - 1 )
                    << KIND_TEMP_MEASURES_SESSIONS << ends ;

         writeLong (os, sessionsForTSCount (RESULT_LEFTS, cCBEDBNPtr, NULL, NULL, t_buff),
                    len, algn) ;
      }
# endif
      break ;

    case 611 :
#  if INSTANCE
#  else  // Region only
      {  // Session result = RESULT_SESS_DEC_TEMP_MEASURES
         //  and
         // Session type not in KIND_PSEUDO_OR_CLOSED_SESSIONS
         char t_buff[128] ;
         ostrstream (t_buff, strlen(t_buff) - 1 )
                    << RESULT_SESS_DEC_TEMP_MEASURES << ends ;
         buff [0] = '\0' ;
         ostrstream (buff, buffL) << KIND_PSEUDO_OR_CLOSED_SESSIONS << ends ;

         writeLong (os, sessionsForTSCount (t_buff, cCBEDBNPtr, NULL, NULL, buff, false),
                    len, algn) ;
      }
# endif
      break ;
// 20060310 <-

    case 188 :
    case 347 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char k1 [2] ;

        k1 [0] = KIND_FIRST_SESSION ;
        k1 [1] = '\0' ;
        ostrstream (buff, buffL) << RESULT_LEFTS << ends ;
        b [0] = dCphr == 188 ? RESULT_LEFT : '\0' ;
        writeLong (os,
                   sessionsForTSCount (//RESULT_LEFT_1ST_SESS,
                                       //RESULT_LEFTS, // I'm guessing...
                                       difference (buff, b),  // I'm
                                                           // guessing...
                                       cCBEDBNPtr,
                                       NULL, NULL,
                                       k1),  // I'm guessing...
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 189 :
    case 345 :
    case 348 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char k2 [2] ;
        char rL [3] ;

        rL [0] = dCphr == 189 || dCphr == 345 ? RESULT_LEFT : '\0' ;
        rL [1] = dCphr == 189 ? RESULT_LEFT_3_PHASE : '\0' ;
        rL [2] = '\0' ;
        k2 [0] = KIND_SECOND_SESSION ;
        k2 [1] = '\0' ;
        ostrstream (buff, buffL) << RESULT_LEFTS << ends ;
        writeLong (os,
                   sessionsForTSCount (//RESULT_LEFT_2ST_SESS,
                                       //RESULT_LEFTS, // I'm guessing...
                                       difference (buff, rL),  // I'm
                                                           // guessing...
                                       cCBEDBNPtr,
                                       NULL, NULL,
                                       k2),  // I'm guessing...
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 190 :
    case 346 :
       {
        char rL [3] ;

        rL [0] = RESULT_LEFT ;
        rL [1] = dCphr == 190 ? RESULT_LEFT_3_PHASE : '\0' ;
        rL [2] = '\0' ;
        writeLong (os, sessionsForTSCount (rL, cCBEDBNPtr,
                                           NULL, NULL, kP, false),
                   len, algn) ;
       }
      break ;
    case 191 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS, cCBEDBNPtr,
                                     NULL, & gE, kP, false),
                 len, algn) ;
      break ;
    case 192 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS, cCBEDBNPtr,
                                     NULL, & gP, kP, false),
                 len, algn) ;
      break ;
    case 193 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS, cCBEDBNPtr,
                                     NULL, & gR, kP, false),
                 len, algn) ;
      break ;
    case 194 :
      b [0] = RESULT_DECISION_ALLOW_SPLIT ;
      writeLong (os, sessionsForTSCount (b, cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;
      break ;
    case 195 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS,
                                     cCBEDBNPtr, AGE_UP_TO_THREES,
                                     NULL, kP, false),
                 len, algn) ;
      break ;
    case 196 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS,
                                     cCBEDBNPtr, AGE_MORE_THAN_THREES_ALL, //AGE_MORE_THAN_THREES,
                                     NULL, kP, false),
                 len, algn) ;
      break ;
    case 197 :
    case 295 :
      b [0] = RESULT_CANCEL_BY_AGREEMENT ;
      writeLong (os, sessionsForTSCount (b, cCBEDBNPtr,
                                         NULL, NULL, kP, false,
                                         NULL, NULL, dCphr == 197),
                 len, algn) ;
      break ;
    case 198 :
      b [0] = RESULT_CANCEL_BY_AGREEMENT ;
      ostrstream (buff, buffL) << RESULT_CANCELEDS << ends ;
      writeLong (os,
                 sessionsForTSCount (difference (buff, b), cCBEDBNPtr,
                                     NULL, NULL, kP, false),
                 len, algn) ;
      break ;
    case 199 :
    case 285 :
      b [0] = RESULT_SEND ;
      writeLong (os, sessionsForTSCount (b, cCBEDBNPtr,
                                         NULL, NULL, kP, false,
                                         NULL, NULL, dCphr == 199),
                 len, algn) ;
      break ;
    case 200 :
      writeLong (os,
                 sessionsForTSCount (RESULT_CANCELEDS,
                                     cCBEDBNPtr, AGE_UP_TO_THREES,
                                     NULL, kP, false),
                 len, algn) ;
      break ;
    case 201 :
      writeLong (os,
                 sessionsForTSCount (RESULT_CANCELEDS,
                                     cCBEDBNPtr, AGE_MORE_THAN_THREES_ALL,//AGE_MORE_THAN_THREES,
                                     NULL, kP, false),
                 len, algn) ;
      break ;
    case 202 :
      writeLong (os, sessionsForTSCount (RESULT_STOPPEDS, cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;
      break ;
    case 203 :
    case 286 :
      writeLong (os, sessionsForTSCount (RESULT_RETURNEDS, cCBEDBNPtr,
                                         NULL, NULL, kP, false,
                                         NULL, NULL, dCphr == 203),
                 len, algn) ;
      break ;
    case 204 :
    case 283 :
      writeLong (os, sessionsForTSCount (RESULT_CANCELEDS_OR_RETURNEDS,
                                         cCBEDBNPtr,
                                         NULL, NULL, kP, false,
                                         NULL, NULL, dCphr == 204),
                 len, algn) ;
      break ;
    case 205 :
      writeLong (os, sessionsForTSCount ("", cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;
      break ;
    case 206 :
      writeLong (os, sessionsForTSCount (RESULT_UNMOVEABLES,
                                         cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;
      break ;
    case 207 :
    case 292 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS,
                                     cCBEDBNPtr, AGE_UP_TO_THREES,
                                     NULL, kP, false,
                                     NULL, NULL, dCphr == 207)
                   - definTermin_grantNone_cnt_patch (cCBEDBNPtr,
                                                      AGE_UP_TO_THREES,
                                                      kP, dCphr == 207)
                 , len, algn) ;
      break ;
    case 208 :
    case 293 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS,
                                     cCBEDBNPtr, AGE_MORE_THAN_THREES_ALL,//AGE_MORE_THAN_THREES,
                                     NULL, kP, false,
                                     NULL, NULL, dCphr == 208)
                   - definTermin_grantNone_cnt_patch (cCBEDBNPtr,
                                                    AGE_MORE_THAN_THREES_ALL,//AGE_MORE_THAN_THREES,
                                                      kP, dCphr == 208)
                 , len, algn) ;
      break ;
    case 209 :
# if INSTANCE
        ostrstream (buff, buffL)
          << RESULT_LEFTS << RESULT_UNMOVEABLES << ends ;
        writeLong (os, sessionsForTSCount (buff, cCBEDBNPtr,
                                        NULL, NULL, kP, false),
                   len, algn) ;
# else
        {
           ostrstream (buff, buffL)
                 << RESULT_LEFTS << RESULT_UNMOVEABLES
                 << RESULT_SESS_LEFT_TEMP_MEASURES       // 20060310 <-
                 << ends ;
           writeLong (os, sessionsForTSCount (buff, cCBEDBNPtr, NULL, NULL,
                                              kP, false),len, algn) ;
        }
# endif
      break ;
    case 210 :
      writeLong (os, sessionsForTSCount (RESULT_UNMOVEABLES,
                                         cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;
      break ;
    case 211 :
    case 351 :  // 13.apr.2000
      ostrstream (buff, buffL)
        << RESULT_LEFTS_OR_STOPPEDS << RESULT_UNMOVEABLES << ends ;
      writeLong (os, sessionsForTSCount (buff, cCBEDBNPtr,
                                         NULL, NULL, kP, false)
                     + (dCphr == 351 ? cCBEDBNPtr -> extraVal0 : 0)
                 , len, algn) ;
      break ;
    case 212 :
    case 299 :
       {
        b [0] = RESULT_SEND ;
        ostrstream (buff, buffL)
          << RESULT_CANCELEDS_OR_RETURNEDS << ends ;
        writeLong (os, sessionsForTSCount (difference (buff, b),
                                           cCBEDBNPtr,
                                           NULL, NULL, kP, false,
                                           NULL, NULL, dCphr == 212)
                     + definTermin_grantNone_cnt_patch (cCBEDBNPtr, NULL,
                                                        kP, dCphr == 212)
                       , len, algn) ;
       }
      break ;
    case 213 :
    case 287 :
      ostrstream (buff, buffL) << RESULT_WITH_DECS_OR_CANCELEDS
                               << RESULT_RETURNEDS << RESULT_SEND
                               << ends ;
      writeLong (os,
                 sessionsForTSCount (buff, cCBEDBNPtr, AGE_UP_TO_THREES,
                                     NULL, kP, false,
                                     NULL, NULL, dCphr == 213),
                 len, algn) ;
      break ;
    case 214 :
    case 288 :
      ostrstream (buff, buffL) << RESULT_WITH_DECS_OR_CANCELEDS
                               << RESULT_RETURNEDS << RESULT_SEND
                               << ends ;
      writeLong (os,
                 sessionsForTSCount (buff, cCBEDBNPtr,
                                     AGE_MORE_THAN_THREES_ALL,
                                     NULL, kP, false,
                                     NULL, NULL, dCphr == 214),
                 len, algn) ;
      break ;
    case 215 :
    case 294 :
// 14.feb.2000
	 {
        ostrstream (buff, buffL)
          << RESULT_CANCELEDS_OR_RETURNEDS << RESULT_SEND << ends ;

        const long int v0 = sessionsForTSCount (buff, cCBEDBNPtr,
                                                NULL, NULL, kP, false,
                                                NULL, NULL,
                                                dCphr == 215) ;

# if INSTANCE  // (patch)
        ostrstream (buff, buffL)
          << RESULT_DEFINITION_TERMINATING << ends ;

	  const long int v1 = sessionsForTSCount (buff, cCBEDBNPtr,
								NULL, NULL, kP, false,
								NULL, NULL,
                                                dCphr == 215, RESULT_CANCELAWS) ;
# else
        const long int v1 = 0 ;
# endif
        writeLong (os, v0 + v1
                   + definTermin_grantNone_cnt_patch (cCBEDBNPtr, NULL,
                                                      kP, dCphr == 215)
                              , len, algn) ;
       }
      break ;
    case 216 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS, cCBEDBNPtr,
                                     AGE_NONES, NULL, kP, false),
                 len, algn) ;
      break ;
    case 217 :
      b [0] = RESULT_SEND ;
      ostrstream (buff, buffL)
        << RESULT_CANCELEDS_OR_RETURNEDS << b << ends ;
      writeLong (os,
                 sessionsForTSCount (buff, cCBEDBNPtr, AGE_NONES,
                                     NULL, kP, false),
                 len, algn) ;
      break ;
    case 218 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS, cCBEDBNPtr,
                                     AGE_NONES, NULL, kP, false),
                 len, algn) ;
      break ;
    case 219 :
      ostrstream (buff, buffL) << RESULT_WITH_DECS_OR_CANCELEDS
                               << RESULT_RETURNEDS << RESULT_SEND
                               << ends ;
      writeLong (os,
                 sessionsForTSCount (buff, cCBEDBNPtr,
                                     AGE_NONES, NULL, kP, false),
                 len, algn) ;
      break ;
    case 220 :
    case 289 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS, cCBEDBNPtr,
                                     NULL, & gE, kP, false,
                                     NULL, NULL, dCphr == 220),
                 len, algn) ;
      break ;
    case 221 :
    case 290 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS, cCBEDBNPtr,
                                     NULL, & gP, kP, false,
                                     NULL, NULL, dCphr == 221),
                 len, algn) ;
      break ;
    case 222 :
    case 291 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS, cCBEDBNPtr,
                                     NULL, & gR, kP, false,
                                     NULL, NULL, dCphr == 222),
                 len, algn) ;
      break ;
    case 223 :
      writeLong (os,
                 sessionsForTSCount (RESULT_WITH_DECISIONS, cCBEDBNPtr,
                                     NULL, & gN, kP, false),
                 len, algn) ;
      break ;
    case 224 :
       {
        char kO [2] ;

        kO [0] = KIND_OTHER_SESSION ;
        kO [1] = '\0' ;
        writeLong (os,
                   sessionsForTSCount (NULL,
                                       cCBEDBNPtr, NULL, NULL, kO),
                   len, algn) ;
       }
      break ;
    case 225 :
    case 297 :
      b [0] = RESULT_SEND ;
      ostrstream (buff, buffL)
        << RESULT_CANCELEDS_OR_RETURNEDS << b << ends ;
      writeLong (os,
                 sessionsForTSCount (buff, cCBEDBNPtr, AGE_UP_TO_THREES,
                                     NULL, kP, false, NULL, NULL,
                                     dCphr == 225)
                   + definTermin_grantNone_cnt_patch (cCBEDBNPtr,
                                                      AGE_UP_TO_THREES,
                                                      kP, dCphr == 225)
                 , len, algn) ;
      break ;
    case 226 :
    case 298 :
      b [0] = RESULT_SEND ;
      ostrstream (buff, buffL)
        << RESULT_CANCELEDS_OR_RETURNEDS << b << ends ;
      writeLong (os,
                 sessionsForTSCount (buff, cCBEDBNPtr,
                                     AGE_MORE_THAN_THREES_ALL, NULL, kP, false,
                                     NULL, NULL, dCphr == 226)
                   + definTermin_grantNone_cnt_patch (cCBEDBNPtr,
                                                    AGE_MORE_THAN_THREES_ALL,
                                                      kP, dCphr == 226)
                 , len, algn) ;
      break ;
    case 227 :
    case 296 :
       {
        b [0] = RESULT_CANCEL_BY_AGREEMENT ;
        ostrstream (buff, buffL)
          << RESULT_CANCELEDS_OR_RETURNEDS << ends ;
        writeLong (os,
                   sessionsForTSCount (difference (buff, b), cCBEDBNPtr,
                                       NULL, NULL, kP, false,
                                       NULL, NULL, dCphr == 227)
                   + definTermin_grantNone_cnt_patch (cCBEDBNPtr, NULL,
                                                      kP, dCphr == 227)
                   , len, algn) ;
       }
      break ;
    case 228 :
# if INSTANCE
# if APPEAL  // 30.may.2000
# else  // 30.may.2000
       {
        char kCM [2] ;

        kCM [0] = KIND_CREDITORS_MEETING ;
        kCM [1] = '\0' ;

        writeLong (os, sessionsForTSCount (NULL, cCBEDBNPtr,
                                           NULL, NULL, kCM),
                   len, algn) ;
       }
#   endif  // APPEAL // 30.may.2000
# endif
      break ;
    case 229 :
      writeLong (os, sessionsForTSCount (NULL, cCBEDBNPtr, NULL, NULL,
                                         KIND_CLOSED_SESSIONS),
                 len, algn) ;
      break ;
    case 230 :  // For report's summary
    case 551 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // of APPEAL  // 30.may.2000  (without INSTANCE check)
       {
#   if INSTANCE
        const char * const results =
          dCphr == 230 ? RESULT_LEFTS : RESULT_LEFTS_1ST_SESS ;
#   else  // of INSTANCE
        const char * const results = RESULT_LEFTS ;
#   endif  // of INSTANCE
        char k1 [2] ;

        k1 [0] = KIND_FIRST_SESSION ;
        k1 [1] = '\0' ;
        writeLong (os,
                   sessionsForRSCount (results,//RESULT_LEFTS,//24.sep.2001
                                       cCBEDBNPtr, NULL, NULL, k1),
                   len, algn) ;
       }
# endif  // of APPEAL  // 30.may.2000  (without INSTANCE check)
      break ;
    case 552 :  // For the summary of report
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
        writeLong (os,
                   sessionsForRSCount (RESULT_LEFTS_1ST_SESS,
                                       cCBEDBNPtr, NULL, NULL,
                                       KIND_PSEUDO_OR_CLOSED_SESSIONS),
                   len, algn) ;
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE
      break ;
    case 231 :  // For report's summary
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char k2 [2] ;

        k2 [0] = KIND_SECOND_SESSION ;
        k2 [1] = '\0' ;
        ostrstream (buff, buffL) << RESULT_LEFTS
                                 << RESULT_UNMOVEABLES << ends ;
        writeLong (os,
                   sessionsForRSCount (buff, cCBEDBNPtr, NULL, NULL, k2),
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 232 :  // For report's summary
    case 312 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char kL [3] ;

        kL [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        kL [1] = KIND_PUNISHMENT_PRIVATE_CHAR_LAW ;
        kL [2] = '\0' ;
        writeLong (os,
                   sessionsForRSCount (NULL, cCBEDBNPtr, NULL, NULL,
                                       dCphr == 312 ? kP : NULL,
                                       false, kL),
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 233 :  // For report's summary
    case 313 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char kL [2] ;

        kL [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        kL [1] = '\0' ;
        writeLong (os,
                   sessionsForRSCount (NULL, cCBEDBNPtr, NULL, NULL,
                                       dCphr == 313 ? kP : NULL,
                                       false, kL),
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 234 :  // For report's summary
    case 314 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char kL [3] ;

        kL [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        kL [1] = KIND_PUNISHMENT_PRIVATE_CHAR_LAW ;
        kL [2] = '\0' ;
        writeLong (os,
                   sessionsForRSCount (RESULT_LEFTS, cCBEDBNPtr,
                                       NULL, NULL,
                                       dCphr == 314 ? kP : NULL,
                                       false, kL),
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 235 :  // For report's summary
    case 315 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char kL [2] ;

        kL [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        kL [1] = '\0' ;
        writeLong (os,
                   sessionsForRSCount (RESULT_LEFTS, cCBEDBNPtr,
                                       NULL, NULL,
                                       dCphr == 315 ? kP : NULL,
                                       false, kL),
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 236 :  // For report's summary
        writeLong (os,
                   sessionsForRSCount (RESULT_RETURNEDS, cCBEDBNPtr,
                                       NULL, NULL, NULL,
                                       true, NULL, false, true),
                   len, algn) ;
      break ;

    case 619 : // 20080624 - report summary - шифър 3320 - Изпратени за доразследване в открито заседание
        writeLong (os,
                   sessionsForRSCount (RESULT_RETURNEDS, cCBEDBNPtr,
                                       NULL, NULL, KIND_OPEN_SESSIONS,
                                       true, NULL, false, true),
                   len, algn) ;
      break ;

    case 620 : // 20080624 - report summary - шифър 3350 - От влезли в сила решени, брой дела, изпратени за доразследване
	{  long int val620 = 0 ;
	   writeLong (os, val620, len, algn) ; // nothing yet
	}
	break ;

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
    case 621 : // 20080625
	{ long int cumulation = 0;
	  if (cCBEDBNPtr)
        {
	    CharPtr crit (1024) ;
	    char judgwarks[3];

	    strcpy(judgwarks, KIND_JUDGEMENTS);
	    strcach(judgwarks, KIND_BULWARK);
	    const char * const what = "DISTINCT J.F_NO, J.F_YEAR, J.F_TYPE" ;
	    if ( composeReportJudgementCriteria ( crit, crit.Size (), what,
								cCBEDBNPtr -> colleges,
								cCBEDBNPtr -> composit,
								& (cCBEDBNPtr -> begD),
								& (cCBEDBNPtr -> endD),
								judgwarks,
								JUDGEMENT_Y_APPENDIX_25
                                             ) )
          {
             TQuery q (crit);

             while (q.Read ())
               cumulation ++ ;
          }
        }
        writeLong (os, cumulation, len, algn) ;
	}
	break ;

    case 622 : // 20080625  // от решените дела колко са с "изтекъл" срок за мотиви
	{
        long int howMany = 0 ;
        const size_t crSz = 1024;
        char * crit = new char [crSz] ;
        const char * const what =
         " D.F_NO, D.F_YEAR, D.F_TYPE, D.F_KIND, D.F_DATE ";

        if ((crit) && composeReportDecisionCriteria (crit, crSz, what,
                                 cCBEDBNPtr -> colleges,
                                 cCBEDBNPtr -> composit,
                                 & (cCBEDBNPtr -> begD),
                                 & (cCBEDBNPtr -> endD),

                                 KIND_JUDGEMENTS //const char * decisionKinds
                               ))
        {
		TQuery q (crit) ;
            TDecision dec ;

            while (q.Read ())
            {
                dec << q ;
		    if (dec.Get ())
		    {
			 CDate termDate = !dec.fromDate.Empty() ? dec.fromDate : dec.date;
			 termDate.Advance(INTERVAL_DAYS, 15, 0);

                   if (!dec.returned.Empty())
                   { if (dec.returned > termDate) howMany ++ ;
                   }
                   else
                   { if ((cCBEDBNPtr -> endD > termDate) // ||
									   // ( (Today > cCBEDBNPtr -> endD)&&(Today > termDate) )
                         )howMany ++ ;
                   }
                }

            }
        }
        if (crit)  delete [] crit ;
        writeLong (os, howMany, len, algn) ;
      }
      break ;
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

    case 237 :  // For report's summary
	writeLong (os, sessionsForRSCount (RESULT_RETURNEDS, cCBEDBNPtr,
						     NULL, NULL, KIND_CLOSED_SESSIONS
						     , true, NULL, false, true
						     ),
		     len, algn) ;
	break ;
    case 322 :  // For report's summary
    case 323 :  // 20071002 - unusable
      writeLong (os,
                 sessionsForRSCountMembership (RESULT_RETURNEDS,
                                               cCBEDBNPtr, NULL, NULL,
                                               NULL, false, NULL, false,
                                               true,
                                               dCphr == 322  // judgeOnly
                                              ),
                 len, algn) ;
      break ;
    case 238 :  // For report's summary
        writeLong (os,
                   sessionsForRSCount (RESULT_RETURNEDS, cCBEDBNPtr,
                                       NULL, NULL, NULL, false, NULL,
                                       false),
                   len, algn) ;
      break ;
    case 239 :  // For report's summary
    case 317 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char lK [3] ;
        // lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        // lK [1] = '\0' ;

# if INSTANCE       // AREAL       // 20060717  -->
        lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        lK [1] = '\0' ;
# else              // REGIONAL
        lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        lK [1] = KIND_PUNISHMENT_PRIVATE_CHAR_LAW ;
        lK [2] = '\0' ;
# endif                            // 20060717  <--
        writeLong (os,
                   reportSidesCount (cCBEDBNPtr -> colleges,
                                     cCBEDBNPtr -> composit,
                                     & (cCBEDBNPtr -> endD),
                                     & (cCBEDBNPtr -> begD),
                                     NULL, JUDGED_CONVICTEDS,
                                     NULL, NULL, NULL, NULL,
                                     dCphr == 239 ? lK : NULL),
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 240 :  // For report's summary
    case 318 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char lK [3] ;
        char recidS [2] ;

        //lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        //lK [1] = recidS [1] = '\0' ;

# if INSTANCE       // AREAL       // 20060717  -->
        lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        lK [1] = '\0' ;
# else              // REGIONAL
        lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        lK [1] = KIND_PUNISHMENT_PRIVATE_CHAR_LAW ;
        lK [2] = '\0' ;
# endif                            // 20060717  <--

        recidS [0] = RECIDIVE_GENERAL ;
        writeLong (os,
                   reportSidesCount (cCBEDBNPtr -> colleges,
                                     cCBEDBNPtr -> composit,
                                     & (cCBEDBNPtr -> endD),
                                     & (cCBEDBNPtr -> begD),
                                     NULL, JUDGED_CONVICTEDS,
                                     NULL, NULL, NULL, recidS,
                                     dCphr == 240 ? lK : NULL),
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 241 :  // For report's summary
    case 319 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char lK [3] ;
        char recidS [2] ;

        // lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        // lK [1] = recidS [1] = '\0' ;

# if INSTANCE       // AREAL       // 20060717  -->
        lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        lK [1] = '\0' ;
# else              // REGIONAL
        lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        lK [1] = KIND_PUNISHMENT_PRIVATE_CHAR_LAW ;
        lK [2] = '\0' ;
# endif                            // 20060717  <--

        recidS [0] = RECIDIVE_SPECIAL ;
        writeLong (os,
                   reportSidesCount (cCBEDBNPtr -> colleges,
                                     cCBEDBNPtr -> composit,
                                     & (cCBEDBNPtr -> endD),
                                     & (cCBEDBNPtr -> begD),
                                     NULL, JUDGED_CONVICTEDS,
                                     NULL, NULL, NULL, recidS,
                                     dCphr == 241 ? lK : NULL),
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 242 :  // For report's summary
    case 320 :
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char lK [3] ;
        char recidS [2] ;

        // lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        // lK [1] = recidS [1] = '\0' ;

# if INSTANCE       // AREAL       // 20060717  -->
        lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        lK [1] = '\0' ;
# else              // REGIONAL
        lK [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        lK [1] = KIND_PUNISHMENT_PRIVATE_CHAR_LAW ;
        lK [2] = '\0' ;
# endif                            // 20060717  <--


        recidS [0] = RECIDIVE_DANGEROUS ;
        writeLong (os,
                   reportSidesCount (cCBEDBNPtr -> colleges,
                                     cCBEDBNPtr -> composit,
                                     & (cCBEDBNPtr -> endD),
                                     & (cCBEDBNPtr -> begD),
                                     NULL, JUDGED_CONVICTEDS,
                                     NULL, NULL, NULL, recidS,
                                     dCphr == 242 ? lK : NULL),
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 321 :  // For report's summary
// 27.jul.2001 - begin
//      writeLong (os, reportSidesCount (cCBEDBNPtr -> colleges,
//                                       cCBEDBNPtr -> composit,
//                                       & (cCBEDBNPtr -> endD),
//                                       & (cCBEDBNPtr -> begD),
//                                       NULL, NULL, NULL, NULL, NULL, NULL,
//                                       NULL, FREE_FREEDS),
//                 len, algn) ;
      writeLong (os, reportSidesCount (cCBEDBNPtr -> colleges,
                                       cCBEDBNPtr -> composit,
                                       & (cCBEDBNPtr -> endD),
                                       & (cCBEDBNPtr -> begD),
                                       NULL, JUDGED_FREEDS),
                 len, algn) ;  // 27.jul.2001 - end
      break ;
    case 243 :  // For report's summary
# if INSTANCE
# else
       {
        char oNM [2] ;

        oNM [1] = '\0' ;
        oNM [0] = OLD_NOT_MAJOR ;
        writeLong (os,
                   reportSidesCount (cCBEDBNPtr -> colleges,
                                     cCBEDBNPtr -> composit,
                                     & (cCBEDBNPtr -> endD),
                                     & (cCBEDBNPtr -> begD),
                                     NULL, NULL, NULL, oNM,
                                     PUNISHMENT_WORK_EDUCATE_SCHOOL_S),
                   len, algn) ;
       }
# endif
      break ;
    case 500 :  // For report's summary  // 11.jan.2001
# if INSTANCE
# else
      writeLong (os,
                 reportSidesCount (cCBEDBNPtr -> colleges,
                                   cCBEDBNPtr -> composit,
                                   & (cCBEDBNPtr -> endD),
                                   & (cCBEDBNPtr -> begD), NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL, NULL,
                                   PS2_KIND_INVESTIGATIONS), 
                 len, algn) ;
# endif
      break ;
    case 244 :  // For report's summary, see 205
      writeLong (os, sessionsForRSCount (NULL, cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;


      break ;
    case 245 :  // For report's summary, see 209
      ostrstream (buff, buffL)
        << RESULT_LEFTS_OR_STOPPEDS
        << RESULT_UNMOVEABLES << ends ;
      writeLong (os, sessionsForRSCount (buff, cCBEDBNPtr,
                                         NULL, NULL, kP, false)
                     + cCBEDBNPtr -> extraVal7,
                 len, algn) ;
      break ;
    case 246 :  // For report's summary, see 202
      writeLong (os, sessionsForRSCount (RESULT_STOPPEDS, cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;
      break ;
    case 247 :  // For report's summary, see 187
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
#  if INSTANCE  // Area only  // 19.dec.2003
        writeLong (os, 0, len, algn) ;
#  else  // Region only
        char kR [2] ;

        kR [0] = KIND_RECONCILIATION_SESSION ;
        kR [1] = '\0' ;
        writeLong (os,
                   sessionsForRSCount (//RESULT_LEFT_RECONCILES,
                                       RESULT_LEFTS,  // I'm guessing...
                                       cCBEDBNPtr,
                                       NULL, NULL,
                                       kR),  // I'm guessing...
                     len, algn) ;
# endif
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 248 :  // For report's summary, see 187
      b [0] = RESULT_LEFT ;
      writeLong (os, sessionsForRSCount (b, cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;
      break ;
    case 249 :  // for closed sess's book summary
    case 305 :
       {
        char r [2] ;

        r [0] = RESULT_CANCEL_FOR_MORE_LOOK_INTO ;
        r [1] = '\0' ;
        ostrstream (buff, buffL) << RESULT_CANCELEDS_OR_RETURNEDS
                                 << ends ;
        writeLong (os, closedSessCount (cBDEDBN, difference (buff, r),
                                        NULL,
# if INSTANCE
                                        1,
# endif
                                        dCphr == 249
                                        ),
                   len, algn) ;
       }
      break ;
    case 503 :
      writeLong (os, closedSessCount (cBDEDBN,
                                      RESULT_CANCELEDS_OR_RETURNEDS,
                                      NULL,
# if INSTANCE
                                      1,
# endif
                                      false
                                      ),
                 len, algn) ;
      break ;
    case 250 :  // for closed sess's book summary
    case 306 :
# if INSTANCE
       {
        char r [2] ;

        r [0] = RESULT_CANCEL_FOR_MORE_LOOK_INTO ;
        r [1] = '\0' ;
        ostrstream (buff, buffL) << RESULT_CANCELEDS_OR_RETURNEDS
                                 << ends ;
        writeLong (os, closedSessCount (cBDEDBN, difference (buff, r),
                                        NULL, 2, dCphr == 250),
                   len, algn) ;
       }
# endif
      break ;
    case 251 :  // for closed sess's book summary
    case 307 :

      writeLong (os, closedSessCount (cBDEDBN,
						  RESULT_TO_UP_LOOK_INTO_RETURNEDS,
                                      NULL,
# if INSTANCE
                                       0,
# endif
                                      dCphr == 251
                                      ),
                 len, algn) ;
      break ;
    case 252 :  // for closed sess's book summary
    case 308 :
# if INSTANCE
       {
        char dtr [2] ;

        dtr [0] = RESULT_DEFINITION_TERMINATING ;
        dtr [1] = '\0' ;
        writeLong (os, closedSessCount (cBDEDBN, dtr, NULL, 1,
                                        dCphr == 252),
                   len, algn) ;
       }
# else  // INSTANCE
      writeLong (os, closedSessCount (cBDEDBN,
                                      RESULT_DEFIN_TERMIN_CLSESBK),
                 len, algn) ;
# endif  // INSTANCE
      break ;
    case 253 :  // for closed sess's book summary
    case 309 :
# if INSTANCE
        writeLong (os, closedSessCount (cBDEDBN,
                                        RESULT_DEFIN_2ND_TERMIN_CLSESBK,
                                        NULL, 2, dCphr == 253),
                   len, algn) ;
# endif  // INSTANCE
      break ;
    case 254 :  // for closed sess's book summary
      writeLong (os, closedSessCount (cBDEDBN,
                                      RESULT_DECISIONS_CLSESBK),
                 len, algn) ;
      break ;
    case 255 :  // for closed sess's book summary
      writeLong (os, closedSessCount (cBDEDBN,
                                      RESULT_DEFINITIONS_CLSESBK),
                 len, algn) ;
      break ;
    case 502 :  // for closed sess's book summary  12.jan.2001
      b [0] = RESULT_DECREE ;
      writeLong (os, closedSessCount (cBDEDBN, b), len, algn) ;
      // 20070202 - ? AND (L.F_FINISHED IS NULL OR S.F_DATE <= L.F_FINISHED)
      break ;
    case 256 :  // for closed sess's book summary
      writeLong (os, closedSessCount (cBDEDBN, ""),
                 len, algn) ;
      break ;
    case 257 :  // For report's summary, see 205, 244
      writeLong (os, sessionsForRSCount (
                     NULL,       //const char * resultsString, // session result
                     cCBEDBNPtr, //const collCompBegDEndDBegN * cCBDEDBNPtr,
                     NULL,       //const char * ageString = NULL,
                     NULL,       //const char * pGrantChar = NULL,
                     KIND_PSEUDO_OR_CLOSED_SESSIONS,//const char * pSessKindChar = NULL,
                     false),     //bool skcEq = true,
                                 //const char * lawKindStr = NULL,
                                 //const bool followSessDate = true,
                                 //const bool includingClosedSessions = false
                 len, algn) ;
      break ;
    case 258 :  // For report's summary, see 209, 245
      ostrstream (buff, buffL)
        << RESULT_LEFTS_OR_STOPPEDS
        << RESULT_UNMOVEABLES
//# if REGIONAL
//# else
//        << RESULT_SESS_LEFT_TEMP_MEASURES   // 20060314  // added in RESULT_LEFTS_OR_STOPPEDS
//# endif
        << ends ;
      writeLong (os, sessionsForRSCount (buff, cCBEDBNPtr, NULL, NULL,
                                         KIND_PSEUDO_OR_CLOSED_SESSIONS,
                                         false),
                 len, algn) ;
      break ;
    case 260 :  // For report's summary
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char kL [3] ;

        kL [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        kL [1] = KIND_PUNISHMENT_PRIVATE_CHAR_LAW ;
        kL [2] = '\0' ;
        writeLong (os,
                   sessionsForRSCount (NULL, cCBEDBNPtr,
                                       NULL, NULL, NULL, false, kL),
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 261 :  // For report's summary
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char kL [3] ;

        kL [0] = KIND_PUNISHMENT_GENERAL_CHAR_LAW ;
        kL [1] = KIND_PUNISHMENT_PRIVATE_CHAR_LAW ;
        kL [2] = '\0' ;
        writeLong (os,
                   sessionsForRSCount (RESULT_LEFTS, cCBEDBNPtr,
                                       NULL, NULL, NULL, false, kL),
                   len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 262 :  // For 2nd admin report's summary only !!!
                // Налични дела - общо
# if INSTANCE
      writeLong (os, adm2ndLawsCount (cCBEDBNPtr -> endD,
                                      cCBEDBNPtr -> begD,
                                  cCBEDBNPtr -> composit,  // 27.feb.2001
                                      false,
                                      true
                                                        ),
                 len, algn) ;
# endif
      break ;
    case 263 :  // For 2nd admin report's summary only !!!
                // Налични дела
                // от тях свършени
# if INSTANCE
      writeLong (os, adm2ndLawsCount (cCBEDBNPtr -> endD,
                                      cCBEDBNPtr -> begD,
                                   cCBEDBNPtr -> composit  // 27.feb.2001
                                     ),
                 len, algn) ;
# endif
      break ;
    case 267 :
# if ! INSTANCE
      writeLong (os, sessionsForTSCount (NULL, cCBEDBNPtr, NULL, NULL,
                                         kP, false, NULL,
                                         // 25.sep.2003 - begin
                                         NULL,
                                         true, NULL, true, false,
                                         true  // 25.sep.2003 - end
                                        ),
                 len, algn) ;
# endif
      break ;
    case 269 :
      b [0] = RESULT_SPLIT_PROTOCOL ;
      writeLong (os, sessionsForTSCount (b, cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;
      break ;
    case 270 :  // For report's summary (regional, 1st Inst areal)
      writeLong (os, cCBEDBNPtr -> extraVal0, len, algn) ;
      break ;
    case 271 :  // For report's summary (regional, 1st Inst areal)
      writeLong (os, cCBEDBNPtr -> extraVal1, len, algn) ;
      break ;
    case 272 :  // For report's summary (regional, 1st Inst areal)
      writeLong (os, cCBEDBNPtr -> extraVal2, len, algn) ;
      break ;
    case 324 :  // For report's summary (regional, 1st Inst areal)
      writeLong (os, cCBEDBNPtr -> extraVal3, len, algn) ;
      break ;
    case 273 :
      writeLong (os, sessionsForTSCount (NULL, cCBEDBNPtr,
                                         NULL, NULL, ""),
                 len, algn) ;
      break ;
    case 274 :
      writeLong (os, sessionsForTSCount (NULL, cCBEDBNPtr,
                                         NULL, NULL, kP),
                 len, algn) ;
      break ;
    case 275 :  // For 2nd Inst report's summary
# if INSTANCE
      writeLong (os, cCBEDBNPtr -> extraVal3, len, algn) ;
# endif
      break ;
    case 276 :  // For 2nd Inst report's summary
# if INSTANCE
      writeLong (os, cCBEDBNPtr -> extraVal4, len, algn) ;
# endif
      break ;
    case 277 :
      writeLong (os,
                 countFinishedsWithDecJudgDefin (cCBEDBNPtr,
                                                 KIND_JUDGEMENTS_BY_NAME),
                 len, algn) ;
      break ;
    case 278 :
      writeLong (os,
                 countFinishedsWithDecJudgDefin (cCBEDBNPtr,
                                                 KIND_DECISIONS_BY_NAME),
                 len, algn) ;
      break ;
    case 279 :
# if ! INSTANCE
      writeLong (os,
                 countFinishedsWithDecJudgDefin (cCBEDBNPtr,
                                                 KIND_JUDGEMENTS_BY_NAME,
                                                 NULL,
                           NULL,
                                                 true  // 25.sep.2003
                                                ),
                 len, algn) ;
# endif
      break ;
    case 280 :
    case 301 :
# if ! INSTANCE
      writeLong (os, sessionsForTSCount (RESULT_CANCELEDS_OR_RETURNEDS,
                                         cCBEDBNPtr, NULL, NULL,
                                         kP, false, NULL,
                          NULL,
                                         dCphr == 280,
                                         NULL, true, false, true  // 25.sep.2003
                                        ),
                 len, algn) ;
# endif
      break ;
    case 501 :  // 12.jan.2001
# if ! INSTANCE
      b [0] = RESULT_WITH_BULWARK ;
      writeLong (os, sessionsForTSCount (b, cCBEDBNPtr, NULL, NULL,
                                         kP, false, NULL,
                                         // 25.sep.2003 - begin
                                         NULL,
                                         true, NULL, true, false,
                                         true  // 25.sep.2003
                                        ),
                 len, algn) ;
# endif
      break ;
    case 281 :
    case 284 :
      scSprintf (buffL, buff, "%s", RESULT_CANCELEDS_OR_RETURNEDS) ;
       {
        char tmp [2] ;

        tmp [1] = '\0' ;
        tmp [0] = RESULT_SEND ;
        difference (buff, tmp) ;
        difference (buff, RESULT_RETURNEDS) ;
        tmp [0] = RESULT_WITH_BULWARK ;  // 01.aug.2000
        difference (buff, tmp) ;  // 01.aug.2000
       }
      writeLong (os,
                 buff [0] == '\0' ? 0 :  // 01.aug.2000
                 sessionsForTSCount (buff, cCBEDBNPtr, NULL, NULL, kP,
                                     false, NULL, NULL, dCphr == 281),
                 len, algn) ;
      break ;
    case 302 :  // For 2nd Inst report's summary
# if INSTANCE
      writeLong (os, cCBEDBNPtr -> extraVal5, len, algn) ;
# endif
      break ;
    case 303 :  // For 2nd Inst report's summary
# if INSTANCE
      writeLong (os, cCBEDBNPtr -> extraVal6, len, algn) ;
# endif
      break ;
    case 304 :  // For report's summary, see 230, 247
# if APPEAL  // 30.may.2000  (without INSTANCE check)
# else  // 30.may.2000  (without INSTANCE check)
       {
        char kR [3] ;

#  if INSTANCE  // Area only  // 19.dec.2003
        kR [0] = KIND_FIRST_SESSION ;
        kR [1] = '\0' ;
#  else  // Region only
        kR [0] = KIND_RECONCILIATION_SESSION ;
        kR [1] = KIND_FIRST_SESSION ;
        kR [2] = '\0' ;
# endif
        writeLong (os,
                   sessionsForRSCount (RESULT_LEFTS,  // I'm guessing...
                                       cCBEDBNPtr, NULL, NULL,
                                       kR),  // I'm guessing...
                     len, algn) ;
       }
# endif  // 30.may.2000  (without INSTANCE check)
      break ;
    case 310 :
    case 311 :
      writeLong (os,
                 sessionsForTSCount (RESULT_FINALIZEDS, cCBEDBNPtr,
                                     NULL, NULL, kP, false,
                                     NULL, NULL, dCphr == 310),
                 len, algn) ;
      break ;
    case 316 :
       {
        const size_t tmpSz = strlen (RESULT_CANCELEDS_OR_RETURNEDS) + 1
                                                                    + 1 ;
        char * tmp316 = new char [tmpSz] ;

        scSprintf (buffL, buff, "%s", RESULT_FINALIZEDS) ;
        scSprintf (tmpSz, tmp316, "%s%c",
                   RESULT_CANCELEDS_OR_RETURNEDS, RESULT_SEND) ;
        difference (buff, tmp316) ;
        writeLong (os,
                   sessionsForTSCount (buff, cCBEDBNPtr, NULL, NULL, kP,
                                       false, NULL, NULL, false) -
                   countFinishedsWithDecJudgDefin (cCBEDBNPtr,
                                               KIND_JUDGEMENTS_BY_NAME),
                   len, algn) ;
        delete [] tmp316 ;
       }
      break ;
    case 325 :  // for closed sess's book summary
    case 326 :  // for closed sess's book summary
      writeLong (os,
                 closedSessDecidsOrCancelsCount (cBDEDBN,
# if INSTANCE
                                                 1,
# endif
                                                 dCphr == 325),
                 len, algn) ;
      break ;


    // RESULT_DEFINITIONS_CLSESBK
    // "h"  // Без движение   - REGIONAL only
    // "S"  // Спряно         - ALL
    // "g"  // С отменен ход по същество  - REGIONAL only
    // "J"  // С определение  - ALL
    //char *resClsSess_DefinOnly = ( RESULT_DEFINITION,'\0');
    case 327 : // for closed sess's book summary
    case 328 :
      writeLong (os, closedSessCount (cBDEDBN, RESULT_FINALIZEDS, NULL,
# if INSTANCE
                                      dCphr - 326,
# endif
                                      true, NULL, false),
                 len, algn) ;
      break ;


    case 329 :  // for closed sess's book summary
    case 330 :
//    case 389 :  // 30.aug.2000, 17.oct.2000 commented here.
# if INSTANCE
#   if APPEAL
      if (dCphr == 329)
        scSprintf (buffL, buff, "%s%s",
                   KIND_EXACT_COMPLAINTS, KIND_EXACT_OBJECTIONS) ;
      else
        scSprintf (buffL, buff, "%s%s",
                   KIND_PRIVATE_COMPLAINTS, KIND_PRIVATE_OBJECTIONS) ;

      writeLong (os, closedSessCount (cBDEDBN, RESULT_FINALIZEDS, NULL,
                                      2, false, buff), len, algn) ;
#   else  // APPEAL
      if (dCphr == 329)
        scSprintf (buffL, buff, "%s%s",
                   KIND_EXACT_COMPLAINTS, KIND_EXACT_OBJECTIONS) ;
      else // dCphr == 330
//        if (dCphr == 330)  // 30.aug.2000, 17.oct.2000 commented
          scSprintf (buffL, buff, "%s%s",
                     KIND_PRIVATE_COMPLAINTS, KIND_PRIVATE_OBJECTIONS) ;
//        else  // 30.aug.2000, 17.oct.2000 commented
//   scSprintf (buffL, buff, "%c",
//              KIND_ENACTMENT_237) ;//30.aug.2000, 17.oct.2000 commented
      writeLong (os, closedSessCount (cBDEDBN, RESULT_FINALIZEDS, NULL,
                                      2, false, buff), len, algn) ;
#   endif  // APPEAL
# endif
      break ;



    case 459 :
# if INSTANCE
      writeLong (os, closedSessCount (cBDEDBN, RESULT_FINALIZEDS, NULL,
                                      2, false, KIND_SLOWDOWN_COMPLAINTS),
                 len, algn) ;
# endif
      break ;
    case 331 :  // For 2nd admin report's summary only !!!
                // в т. ч. отменени актове
                // (instead of "case 303 :")
# if INSTANCE
      writeLong (os, adm2ndLawsCount (cCBEDBNPtr -> endD,
                                      cCBEDBNPtr -> begD,
                                   cCBEDBNPtr -> composit  // 27.feb.2001
                                     ) -
                     cCBEDBNPtr -> extraVal5,
                 len, algn) ;
# endif
      break ;
    case 332 :  // For 1st instance firm report only.
# if INSTANCE
      if (pRDArr)
       {
        bool found = false ;
        int i ;
        long int v = 0 ;

        for (i = 0 ;
             ! found && i < pRDArr -> Count () ;
             i ++)
         {
          const reportData & rD = * ((* pRDArr) [i]) ;

          if (rD.subject == skip
             )
           {
		found = true ;
            const int a = algn -
//                                 OFFSET_REPORT_BOOK_CIPHER -
                                 1 ;

            switch (a)
             {
//              case iC_L_Completeds_Grant_Entire :
              case 37 - 1 :
                v = rD.l.completeds.grant.entire ;
                break ;
              case 58 - 1 :  // iC_L_Completeds_Canc_ByOthC_Firm
                v = rD.l.completeds.canceleds.byOthCausFirm ;
                break ;
              case 59 - 1 :
                v = rD.l.changedsFirm ;
             }
           }
         }
        writeLong (os, v, len - skip, RIGHT_ALIGN) ;
       }
      else
        writeString (os, "", len - skip) ;
# endif
      break ;
    case 333 :  // With decision
    case 334 :
    case 335 :
	break ;
    case 336 :  // canceleds
	break ;
    case 339 :
    case 340 :
    case 341 :
	break ;
    case 342 :  // With decision
    case 343 :
    case 344 :
      break ;
    case 349 :
# if INSTANCE
      scSprintf (buffL, buff, "%c", RESULT_LEFT_1_PHASE_DECISION) ;
      writeLong (os, sessionsForTSCount (buff, cCBEDBNPtr,
                                         NULL, NULL, kP, false),
                 len, algn) ;
# endif
      break ;
    case 350 :  // For the time book summary (abolished progress)
      writeLong (os, cCBEDBNPtr -> extraVal0, len, algn) ;
      break ;
    case 352 :  // For the 2nd report summary
# if INSTANCE
#   if APPEAL
      writeLong (os, cCBEDBNPtr -> extraVal7, len, algn) ;
#   else
      if (cCBEDBNPtr -> extraVal7)
        writeLong (os, cCBEDBNPtr -> extraVal7, len, algn) ;
      else
        writeString (os, "", len, algn, skip) ;
#   endif
# endif
      break ;
    case 353 :  // For the 2nd report summary
# if INSTANCE
      writeLong (os, cCBEDBNPtr -> extraVal7, len, algn) ;
# endif
      break ;
    case 380 :  // For the closed sess book summary
# if APPEAL
      scSprintf (buffL, buff, "%c",
                 KIND_CANCEL_ENACTMENT) ;
      writeLong (os, closedSessCount (cBDEDBN, RESULT_FINALIZEDS, NULL,
                                      2, false, buff),
                 len, algn) ;
# endif  // APPEAL
      break ;
    case 381 :  // For the closed sess book summary
# if APPEAL
      scSprintf (buffL, buff, "%c", RESULT_DEFINITION_326) ;
      writeLong (os, closedSessCount (cBDEDBN, buff, NULL, 2),
                 len, algn) ;
# endif  // APPEAL
      break ;
    case 382 :  // For the closed sess book summary
# if APPEAL
      scSprintf (buffL, buff, "%c", RESULT_DECREE) ;
      writeLong (os, closedSessCount (cBDEDBN, buff, NULL, 2),
                 len, algn) ;
# endif  // APPEAL
      break ;
    case 384 :  // For 2nd non-punishment report's summary only !!!
		    // Касационни производства по ЗСПЗЗ
                // - Несв. дела в нач. на периода
                // В т.ч. неразгл. в нач. на отч.период
# if INSTANCE
      writeLong (os, cCBEDBNPtr -> extraVal0, len, algn) ;
# endif
      break ;
    case 387 :
    case 388 :
      b [0] = RESULT_WITH_BULWARK ;
      writeLong (os, sessionsForTSCount (b, cCBEDBNPtr, NULL, NULL, kP,
                                         false, NULL, NULL, dCphr == 387),
                 len, algn) ;
      break ;

    case 545 :  // 17.sep.2001
      b [0] = RESULT_DEFINITION ;
      writeLong (os, sessionsForTSCount (b, cCBEDBNPtr, NULL, NULL,
                                         KIND_CLOSED_SESSIONS, false),
                 len, algn) ;
      break ;
    case 557 :  // 17.feb.2002  ?? I'm not sure about the exact day
      if (cCBEDBNPtr)
       {  // -- 0 --
        long int arresteds = 0;
        CharPtr crit (896) ;
        const char * const what = "F_NO, F_YEAR, F_TYPE" ;

	  if (composeReportLawsCriteria (crit, crit.Size (), what,
                                       cCBEDBNPtr -> colleges,
                                       cCBEDBNPtr -> composit,
                                       & (cCBEDBNPtr -> endD),
                                       & (cCBEDBNPtr -> begD)))
         {  // -- 1 --
          TQuery q (crit) ;
          TLawsuit law ;
	    long int count = 0 ;
	    TDummyGroup cntGroup;
          CharPtr arrestCrit (512) ;

	    cntGroup.Add(new TLong ("COUNT", & count)) ;
	    while (q.Read ())
           {  // -- 2 --
            law << q ;
            if (law.Get ())
             {  // -- 3 --
              ostrstream oss (arrestCrit, arrestCrit.Size ()) ;

              oss << "SELECT COUNT (F_NO) FROM T_ACCUSATION "
                     "WHERE F_TYPE = '" << law.key.type << "' AND "
                           "F_YEAR = " << law.key.year << " AND "
                           "F_NO = " << law.key.no << " AND "
				   "F_RESOLUTION IN ('"
                             << RESOLUTION_ACC_ARREST << "')" ;
              oss << " ;" << ends ;
              arrestCrit [arrestCrit.Size () - 1] = '\0' ;
              if (criteria_finished_ok (arrestCrit, "arrest - accusation"))
               {  // -- 4 --
                TQuery qa (arrestCrit) ;

                if (qa.Read ())
                 {
                  cntGroup << qa ;
                  arresteds += count ;
                 }
               }  // -- 4 --
             }  // -- 3 --
           }  // -- 2 --
         }  // -- 1 --
        writeLong (os, arresteds, len, algn) ;
       }  // -- 0 --
      break ;
    case 558 :
    case 559 :
# if APPEAL
# else  // of APPEAL
      if (cCBEDBNPtr)
       {  // -- 0 --
        long int holdeds = 0;
        CharPtr crit (896) ;
	  const char * const what = "F_NO, F_YEAR, F_TYPE" ;
        char lK [SIZE_OF_KINDS] ;
        bool criteriaOk ;

        ostrstream (lK, sizeof (lK))
         << KIND_PUNISHMENT_GENERAL_CHAR_LAW << ends ;
        lK [sizeof (lK) - 1] = '\0' ;
        if (dCphr == 558)
          criteriaOk =
            composeReportLawsCriteria (crit, crit.Size (), what,
                                       cCBEDBNPtr -> colleges,
                                       cCBEDBNPtr -> composit,
                                       & (cCBEDBNPtr -> endD),
                                       & (cCBEDBNPtr -> begD),
                                       NULL, NULL, NULL, false, NULL, NULL,
                                       NULL, false, true, true, NULL,
                                       lK) ;
        else
          criteriaOk =
            composeReportLawsCriteria (crit, crit.Size (), what,
                                       cCBEDBNPtr -> colleges,
						   cCBEDBNPtr -> composit,
						   & (cCBEDBNPtr -> endD), NULL,
						   NULL, NULL, NULL, true, NULL, NULL,
						   NULL, false, true, true, NULL,
						   lK) ;
	  if (criteriaOk)
	   {  // -- 1 --
	    TQuery q (crit) ;
	    LawPtr pLaw ;
	    long int count = 0 ;
	    TDummyGroup cntGroup;
          CharPtr holdCrit (512 + 64) ;

	    cntGroup.Add(new TLong ("COUNT", & count)) ;
	    while (q.Read ())
	     {  // -- 2 --
		pLaw.Ref () << q ;
		if (pLaw.Ref ().Get ())
		 {  // -- 3 --
		  ostrstream oss (holdCrit, holdCrit.Size ()) ;

		  oss << "SELECT COUNT (F_NO) FROM T_SIDE_WIND "
			   "WHERE F_TYPE = '" << pLaw.Ref ().key.type << "' AND "
				   "F_YEAR = " << pLaw.Ref ().key.year << " AND "
				   "F_NO = " << pLaw.Ref ().key.no << " AND "
				   "F_KIND = '" << pLaw.Ref ().kind << "' AND "
				   "F_DATE = " << pLaw.Ref ().date.Value() << " AND "
				   "F_STATE IN ('" << STATE_HOLD << "')" ;
		  oss << " ;" << ends ;
		  holdCrit [holdCrit.Size () - 1] = '\0' ;
		  if (criteria_finished_ok (holdCrit, "hold - side"))
		   {  // -- 4 --
		    TQuery qa (holdCrit) ;

                if (qa.Read ())
                 {
                  cntGroup << qa ;
                  holdeds += count ;
                 }
               }  // -- 4 --
             }  // -- 3 --
           }  // -- 2 --
         }  // -- 1 --
        writeLong (os, holdeds, len, algn) ;
       }  // -- 0 --
# endif  // of APPEAL
      break ;
    case 560 :
    case 561 :
    case 562 :
    case 563 :
# if INSTANCE
       {
        long int othActs = 0 ;

        if (cCBEDBNPtr -> colleges [0])
         {
          ReportOthActsCriteria * pCrit = NULL ;
          const char * const types =
		(dCphr == 560 || dCphr == 561) ?
              cCBEDBNPtr -> colleges :
#   if APPEAL
              cCBEDBNPtr -> colleges ;
#   else  // 0f APPEAL
              C2S[TYPE_ADMIN_LAW] ;
#   endif  // 0f APPEAL

          if (dCphr == 560 || dCphr == 562)
            pCrit =
              new ReportOthActsOpenSesCriteria2 (types,
                    cCBEDBNPtr -> composit,
                    cCBEDBNPtr -> begD, cCBEDBNPtr -> endD) ;
          else
            if (dCphr == 561 || dCphr == 563)
              pCrit = new
                ReportOthActsClosedSesCriteria2(types,
                  cCBEDBNPtr -> composit,
                  cCBEDBNPtr -> begD, cCBEDBNPtr -> endD) ;
          if (pCrit)
           {
            if (pCrit -> Compose ())
             {
              TQuery q (pCrit -> Criteria ()) ;

              while (q.Read ())
                othActs ++ ;
             }
	     }
          if (pCrit)
            delete pCrit ;
         }
        writeLong (os, othActs, len, algn) ;
       }
# else  // of INSTANCE
# endif  // of INSTANCE
      break ;
    case 596 :  // 02.jun.2004  For the 1st areal punishment summary
      if (cCBEDBNPtr)
       {
        long int probations = 0;
# if INSTANCE
# if APPEAL
# else  // of APPEAL

        // As in _report.cpp: citsDistribute (iC_C_Convicteds_Others, ...
        CharPtr crit (1664) ;
        const char * const what = "L.F_SUBJECT"", ""L.F_TYPE" ;

        if (composeReportSidesCriteria (crit, crit.Size (), what,
                                        cCBEDBNPtr -> colleges,
                                        cCBEDBNPtr -> composit,
                                        & (cCBEDBNPtr -> endD),
                                        & (cCBEDBNPtr -> begD),
                                        NULL, JUDGED_CONVICTEDS,
						    NULL, NULL, C2S[PUNISHMENT_PROBATION]))
	   {
           TQuery q (crit);

           while (q.Read ())
             probations ++ ;
         }
# endif  // of APPEAL
# endif  // of INSTANCE
        writeLong (os, probations, len, algn) ;
       }
      break ;


// 20060323 new codes ->
    case 612 :  // For the 1st punishment report summary SPR VI quick
    case 613 :  // For the 1st punishment report summary SPR VII
    case 614 :  // For the 1st punishment report summary SPR VII
    case 615 :  // For the 1st punishment report summary SPR VII
    {
        unsigned long int count612 = 0 ;
        unsigned long int count613 = 0 ;
        unsigned long int count614 = 0 ;
        unsigned long int count615 = 0 ;

        const size_t crSz = 512 ;
        char * crit = new char [crSz] ;

        if( composeFinishWithDecJudgDef_Crit (crit, crSz, cCBEDBNPtr))
	  {
          TQuery q (crit) ;
          LawPtr l ;  // TLawsuit l ;

          while (q.Read ())
          {
              l.Ref() << q ;
              if(TRCDKey_(l.Ref().key).FilledOK() && l.Ref().Get())
              {
                 if (dCphr == 612) // бързи производства
                 {
                    if (quick_currently_quick( l.Ref().quick ))
                       count612++;
                 }
                 if (dCphr == 613) // незабавни производства
                 {
                    if (quick_currently_immediate( l.Ref().quick ))
                       count613++;
                 }
                 if (dCphr == 614)
                 {  // nothing
                    // 20060713 -> да се отчитат само дела по вх.документ "Искане по чл.368 НПК"
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
			  if ((l.Ref().source.type == TYPE_REQUEST) && (l.Ref().sourceKind == KIND_REQUEST_368))
			    count614++;
#endif  // AREAL || MILITARY || REGIONAL
                 }
                 if (dCphr == 615) // съкратени производства 10615
		     {
                    if (  l.Ref().cutDown )     // 20080507; 2014:163 cutdown flag -> value
			     count615++;
                 }
              }
          }
        }

        delete [] crit ;

        if ( dCphr == 612) writeLong (os, count612, len, algn) ;
        else if (dCphr == 613) writeLong (os, count613, len, algn) ;

        else if (dCphr == 614) writeLong (os, count614, len, algn) ; // 20060714
                               // writeString (os, "", len, algn) ;  // temp

        else if (dCphr == 615) writeLong (os, count615, len, algn) ;
    }
    break ;
// 20060323 new codes <-


    case 356 :  // For the 2nd punishment report summary
    case 357 :  // For the 2nd punishment report summary
    case 358 :  // For the 2nd punishment report summary
    case 359 :  // For the 2nd punishment report summary
    case 383 :  // For the 2nd punishment report summary
    case 390 :  // For the 2nd punishment report summary
    case 449 :  // For the 2nd punishment report summary

    case 360 :  // For the 1st punishment report summary
    case 361 :  // For the 1st punishment report summary
    case 362 :  // For the 1st punishment report summary
    case 363 :  // For the 1st punishment report summary

    case 364 :  // For the 1st non-punishment report summary
    case 365 :  // For the 1st non-punishment report summary
    case 366 :  // For the 1st non-punishment report summary
    case 367 :  // For the 1st non-punishment report summary
    case 368 :  // For the 1st non-punishment report summary
    case 369 :  // For the 1st non-punishment report summary

    case 259 :  // For the 2nd punishment report summary
    case 265 :  // For the 2nd punishment report summary
    case 266 :  // For the 2nd punishment report summary
    case 370 :  // For the 2nd punishment report summary
    case 371 :  // For the 2nd punishment report summary
    case 264 :  // For the 2nd punishment report summary
    case 268 :  // For the 2nd punishment report summary

    case 372 :  // For the appeal punishment report summary
    case 373 :  // For the appeal punishment report summary
    case 374 :  // For the appeal punishment report summary

    case 375 :  // For the 2nd punishment report summary
    case 376 :  // For the 2nd punishment report summary
    case 377 :  // For the 2nd punishment report summary
    case 378 :  // For the 2nd punishment report summary
    case 379 :  // For the 2nd punishment report summary

    case 535 :  // For the 2nd punishment report summary
    case 536 :  // For the 2nd punishment report summary
    case 537 :  // For the 2nd punishment report summary
    case 538 :  // For the 2nd punishment report summary
    case 539 :  // For the 2nd punishment report summary
    case 540 :  // For the 2nd punishment report summary
    case 541 :  // For the 2nd punishment report summary
    case 542 :  // For the 2nd punishment report summary
    case 543 :  // For the 2nd punishment report summary
    case 544 :  // For the 2nd punishment report summary

    case 385 :  // For the regional punishment report summary
    case 386 :  // For the regional punishment report summary

    case 391 :  // For the appeal closed sess non-punishment summary
    case 392 :  // For the appeal closed sess non-punishment summary
    case 393 :  // For the appeal closed sess non-punishment summary
    case 394 :  // For the appeal closed sess non-punishment summary
    case 395 :  // For the appeal closed sess non-punishment summary
    case 396 :  // For the appeal closed sess non-punishment summary
    case 397 :  // For the appeal closed sess non-punishment summary
    case 398 :  // For the appeal closed sess non-punishment summary
    case 399 :  // For the appeal closed sess non-punishment summary
    case 400 :  // For the appeal closed sess non-punishment summary
    case 401 :  // For the appeal closed sess non-punishment summary
    case 460 :  // For the appeal closed sess non-punishment summary
    case 504 :  // For the appeal closed sess non-punishment summary
    case 505 :  // For the appeal closed sess non-punishment summary
    case 531 :  // For the appeal closed sess non-punishment summary
    case 532 :  // For the appeal closed sess non-punishment summary
    case 533 :  // For the appeal closed sess non-punishment summary
    case 534 :  // For the appeal closed sess non-punishment summary
		case 579 :  // For the appeal closed sess non-punishment summary
		case 580 :  // For the appeal closed sess non-punishment summary
		case 581 :  // For the appeal closed sess non-punishment summary
		case 582 :  // For the appeal closed sess non-punishment summary
		case 583 :  // For the appeal closed sess non-punishment summary
		case 584 :  // For the appeal closed sess non-punishment summary

		case 462 :  // For the appeal closed sess punishment summary
		case 463 :  // For the appeal closed sess punishment summary
		case 464 :  // For the appeal closed sess punishment summary

		case 402 :  // For the appeal description book summary

		case 403 :  // For the appeal description book non-punishment summary
		case 404 :  // For the appeal description book non-punishment summary
		case 405 :  // For the appeal description book non-punishment summary
		case 406 :  // For the appeal description book non-punishment summary
		case 407 :  // For the appeal description book non-punishment summary
		case 408 :  // For the appeal description book non-punishment summary
		case 409 :  // For the appeal description book non-punishment summary
		case 410 :  // For the appeal description book non-punishment summary
		case 411 :  // For the appeal description book non-punishment summary
		case 412 :  // For the appeal description book non-punishment summary
		case 413 :  // For the appeal description book non-punishment summary
		case 424 :  // For the appeal description book non-punishment summary
		case 571 :  // For the appeal description book non-punishment summary
		case 585 :  // For the appeal description book non-punishment summary

		case 414 :  // For the appeal description book punishment summary
		case 415 :  // For the appeal description book punishment summary
		case 416 :  // For the appeal description book punishment summary
		case 417 :  // For the appeal description book punishment summary
		case 418 :  // For the appeal description book punishment summary
		case 419 :  // For the appeal description book punishment summary
		case 420 :  // For the appeal description book punishment summary
		case 421 :  // For the appeal description book punishment summary
		case 422 :  // For the appeal description book punishment summary
		case 423 :  // For the appeal description book punishment summary
		case 425 :  // For the appeal description book punishment summary

		case 426 :  // For the appealed laws book summary
    case 427 :  // For the appealed laws book summary
    case 428 :  // For the appealed laws book summary
    case 429 :  // For the appealed laws book summary
    case 430 :  // For the appealed laws book summary
    case 431 :  // For the appealed laws book summary
    case 432 :  // For the appealed laws book summary
    case 433 :  // For the appealed laws book summary
    case 434 :  // For the appealed laws book summary
    case 435 :  // For the appealed laws book summary

    case 436 :  // For the areal punishment deck book summary
    case 437 :  // For the areal punishment deck book summary
    case 438 :  // For the areal punishment deck book summary
    case 439 :  // For the areal punishment deck book summary
    case 440 :  // For the areal punishment deck book summary
    case 441 :  // For the areal punishment deck book summary
    case 442 :  // For the areal punishment deck book summary
    case 443 :  // For the areal punishment deck book summary
    case 444 :  // For the areal punishment deck book summary
    case 458 :  // For the areal punishment deck book summary

    case 553 :  // For the areal citizen decc book summary
    case 554 :  // For the areal citizen decc book summary
    case 555 :  // For the areal citizen decc book summary
    case 556 :  // For the areal citizen decc book summary

    case 445 :  // For the areal punishment closed sess book summary
    case 446 :  // For the areal punishment closed sess book summary
    case 447 :  // For the areal punishment closed sess book summary
    case 448 :  // For the areal punishment closed sess book summary
    case 450 :  // For the areal punishment closed sess book summary
    case 451 :  // For the areal punishment closed sess book summary
    case 452 :  // For the areal punishment closed sess book summary
    case 453 :  // For the areal punishment closed sess book summary
    case 454 :  // For the areal punishment closed sess book summary
    case 455 :  // For the areal punishment closed sess book summary
    case 456 :  // For the areal punishment closed sess book summary
    case 389 :  // For the areal punishment closed sess book summary
    case 457 :  // For the areal punishment closed sess book summary

    case 597 :  // For the areal punishment closed sess book summary
    case 598 :  // For the areal punishment closed sess book summary
    case 599 :  // For the areal punishment closed sess book summary
    case 600 :  // For the areal punishment closed sess book summary
    case 601 :  // For the areal punishment closed sess book summary
    case 602 :  // For the areal punishment closed sess book summary
    case 603 :  // For the areal punishment closed sess book summary
    case 604 :  // For the areal punishment closed sess book summary
    case 605 :  // For the areal punishment closed sess book summary
    case 606 :  // For the areal punishment closed sess book summary
    case 607 :  // For the areal punishment closed sess book summary
    case 608 :  // For the areal punishment closed sess book summary
    case 609 :  // For the areal punishment closed sess book summary

    case 461 :  // For the appeal citizen time book summary
    case 497 :  // For the appeal citizen time book summary
    case 498 :  // For the appeal citizen time book summary
    case 499 :  // For the appeal citizen time book summary
    case 506 :  // For the appeal citizen time book summary
    case 507 :  // For the appeal citizen time book summary
    case 508 :  // For the appeal citizen time book summary
    case 509 :  // For the appeal citizen time book summary
    case 526 :  // For the appeal citizen time book summary
    case 527 :  // For the appeal citizen time book summary
    case 528 :  // For the appeal citizen time book summary
    case 529 :  // For the appeal citizen time book summary
    case 530 :  // For the appeal citizen time book summary
    case 564 :  // For the appeal citizen time book summary
    case 565 :  // For the appeal citizen time book summary
    case 566 :  // For the appeal citizen time book summary
		case 572 :  // For the appeal citizen time book summary
		case 573 :  // For the appeal citizen time book summary
		case 574 :  // For the appeal citizen time book summary
		case 575 :  // For the appeal citizen time book summary
		case 576 :  // For the appeal citizen time book summary
		case 577 :  // For the appeal citizen time book summary
		case 578 :  // For the appeal citizen time book summary
		case 586 :  // For the appeal citizen time book summary

    case 465 :  // For the appeal Ilieva summary (citizen)
    case 466 :  // For the appeal Ilieva summary (citizen)
    case 467 :  // For the appeal Ilieva summary (citizen)
    case 468 :  // For the appeal Ilieva summary (citizen)
    case 469 :  // For the appeal Ilieva summary (citizen)
    case 470 :  // For the appeal Ilieva summary (citizen)
    case 471 :  // For the appeal Ilieva summary (citizen)
    case 472 :  // For the appeal Ilieva summary (citizen)
    case 473 :  // For the appeal Ilieva summary (citizen)
    case 474 :  // For the appeal Ilieva summary (citizen)
    case 475 :  // For the appeal Ilieva summary (citizen)
    case 476 :  // For the appeal Ilieva summary (citizen)
    case 477 :  // For the appeal Ilieva summary (citizen)
    case 478 :  // For the appeal Ilieva summary (citizen)
    case 479 :  // For the appeal Ilieva summary (citizen)
    case 480 :  // For the appeal Ilieva summary (citizen)
    case 481 :  // For the appeal Ilieva summary (citizen)
    case 482 :  // For the appeal Ilieva summary (citizen)
    case 483 :  // For the appeal Ilieva summary (citizen)
    case 484 :  // For the appeal Ilieva summary (citizen)
    case 485 :  // For the appeal Ilieva summary (citizen)
    case 486 :  // For the appeal Ilieva summary (citizen)
    case 487 :  // For the appeal Ilieva summary (citizen)
    case 488 :  // For the appeal Ilieva summary (citizen)
    case 489 :  // For the appeal Ilieva summary (citizen)
    case 490 :  // For the appeal Ilieva summary (citizen)
    case 491 :  // For the appeal Ilieva summary (citizen)
    case 492 :  // For the appeal Ilieva summary (citizen)
    case 493 :  // For the appeal Ilieva summary (citizen)
    case 494 :  // For the appeal Ilieva summary (citizen)
    case 495 :  // For the appeal Ilieva summary (citizen)
    case 496 :  // For the appeal Ilieva summary (citizen)
    case 510 :  // For the appeal Ilieva summary (citizen)
    case 511 :  // For the appeal Ilieva summary (citizen)
    case 512 :  // For the appeal Ilieva summary (citizen)
    case 513 :  // For the appeal Ilieva summary (citizen)
    case 514 :  // For the appeal Ilieva summary (citizen)
    case 515 :  // For the appeal Ilieva summary (citizen)
    case 516 :  // For the appeal Ilieva summary (citizen)
    case 517 :  // For the appeal Ilieva summary (citizen)
    case 518 :  // For the appeal Ilieva summary (citizen)
    case 519 :  // For the appeal Ilieva summary (citizen)
    case 520 :  // For the appeal Ilieva summary (citizen)
    case 521 :  // For the appeal Ilieva summary (citizen)
    case 522 :  // For the appeal Ilieva summary (citizen)
    case 523 :  // For the appeal Ilieva summary (citizen)
    case 524 :  // For the appeal Ilieva summary (citizen)
    case 525 :  // For the appeal Ilieva summary (citizen)
    case 546 :  // For the appeal Ilieva summary (citizen)
    case 547 :  // For the appeal Ilieva summary (citizen)
    case 548 :  // For the appeal Ilieva summary (citizen)
    case 549 :  // For the appeal Ilieva summary (citizen)
    case 550 :  // For the appeal Ilieva summary (citizen)
		case 567 :  // For the appeal Ilieva summary (citizen)
		case 568 :  // For the appeal Ilieva summary (citizen)
		case 569 :  // For the appeal Ilieva summary (citizen)
		case 570 :  // For the appeal Ilieva summary (citizen)
		case 587 :  // For the appeal Ilieva summary (citizen)
		case 588 :  // For the appeal Ilieva summary (citizen)

    case 589 :  // For the 2nd punishment report summary  // 11.dec.2003 - begin
    case 590 :  // For the 2nd punishment report summary
    case 591 :  // For the 2nd punishment report summary
    case 592 :  // For the 2nd punishment report summary
    case 593 :  // For the 2nd punishment report summary
    case 594 :  // For the 2nd punishment report summary
    case 595 :  // For the 2nd punishment report summary  // 11.dec.2003 - end
       {
        long int valToWrt = 0 ;
        bool found = false ;

        if (pCodeValueArr)
         {
          CodeValue key ;

          key.code = dCiph ;  // Attantion!   dCiph is not dCphr

	    const int index = pCodeValueArr -> Find (&key) ;

          if (index != INT_MAX)
           {
		found = true ;
            valToWrt = ((* pCodeValueArr) [index])->value ;
	     }
         }
        if (found)
          writeLong (os, valToWrt, len, algn) ;
        else
          writeString (os, " ", len, algn) ;
       }
      break ;

      case 616 : // not finished,  2-instance lawsuits   ADMIN_COURT
	{
        long int count616 = 0;
# if (COURT_TYPE == COURT_ADMIN)
        if ((cCBEDBNPtr)&&(cCBEDBNPtr -> firstInstLaws == false) )
        {  // -- 0 --
          CharPtr crit (512) ;
          ostrstream oss ( crit, crit.Size ()) ;
          char kindsString[SIZE_OF_KINDS];  memset (kindsString, '\0', sizeof kindsString);
          if (cCBEDBNPtr -> colleges [0])
          {
             ostrstream osk ( kindsString, sizeof kindsString ) ;
             if ( strchr (cCBEDBNPtr -> colleges, TYPE_ADMIN_LAW))  osk << KIND_2ND_ADMIN_LAWS;
             if ( //strchr (cCBEDBNPtr -> colleges, TYPE_PUNISHMENT_LAW)
                  strchr (cCBEDBNPtr -> colleges, TYPE_TRICKY_LAW)) osk << KIND_2ND_TRICKY_LAWS;
             if ( strchr (cCBEDBNPtr -> colleges, TYPE_CITIZEN_LAW))osk << KIND_2ND_CITIZEN_LAWS;
             osk << ends;
          }

          oss << "SELECT COUNT (F_NO) FROM T_LAWSUIT WHERE ";
          oss << "F_TYPE IN (" << //kindsTS.getTheSet ()
                     stringToSQLCharSet (cCBEDBNPtr -> colleges).getTheSet ()
                               << ") " ;
          if (strlen(kindsString)>0)
          {
              oss << "AND F_KIND IN (" <<
                         stringToSQLCharSet ( kindsString ).getTheSet ()
                                   << ") " ;
          }
	    if ( cCBEDBNPtr -> composit > 0 )
          {
              oss << "AND F_COMPOSITION=" << cCBEDBNPtr -> composit ;
          }
          oss << "AND F_DATE<" << cCBEDBNPtr -> begD.Value() <<" ";
          oss << "AND ((F_FINISHED IS NULL) ";
          oss <<       "OR (F_FINISHED >= " << cCBEDBNPtr -> begD.Value() <<")) " ;

          oss << " ;" << ends ;
          crit [crit.Size () - 1] = '\0' ;

          TQuery q (crit) ;

          TGroup cntGroup ("COUNT_GROUP", NULL) ;
          cntGroup.Add( new TLong ("COUNT", & count616) );

          if (q.Read ())
          {
             cntGroup << q ;
          }
        }  // -- 0 --
# endif
        writeLong (os, count616, len, algn) ;
      }
      break ;

    // For 2nd admin report's summary only !!! ADMIN_COURT  // 20070923 ->
    case 617 :  // Налични дела - общо
    case 618 :  // Налични дела - от тях свършени
# if (COURT_TYPE == COURT_ADMIN)
        if ((cCBEDBNPtr)&&(cCBEDBNPtr -> firstInstLaws == false) )
        {  // -- 0 --
          char typesString[SIZE_OF_TYPES];  memset (typesString, '\0', sizeof typesString);
          char kindsString[SIZE_OF_KINDS];  memset (kindsString, '\0', sizeof kindsString);
          if (cCBEDBNPtr -> colleges [0])
          {
             ostrstream ost ( typesString, sizeof typesString ) ;
             ostrstream osk ( kindsString, sizeof kindsString ) ;
             if ( strchr (cCBEDBNPtr -> colleges, TYPE_ADMIN_LAW))
             {   ost << TYPE_ADMIN_LAW;
                 osk << KIND_2ND_ADMIN_LAWS;
             }
             if ( //strchr (cCBEDBNPtr -> colleges, TYPE_PUNISHMENT_LAW)
                  strchr (cCBEDBNPtr -> colleges, TYPE_TRICKY_LAW))
             {   ost << TYPE_TRICKY_LAW;
                 osk << KIND_2ND_TRICKY_LAWS;
             }
             if ( strchr (cCBEDBNPtr -> colleges, TYPE_CITIZEN_LAW))
             {   ost << TYPE_CITIZEN_LAW;
                 osk << KIND_2ND_CITIZEN_LAWS;
             }
             ost << ends;
             osk << ends;
          }

          if ( dCphr == 617 )
              writeLong (os, adm2ndLawsCount (cCBEDBNPtr -> endD,
                                              cCBEDBNPtr -> begD,
                                              cCBEDBNPtr -> composit,  // 27.feb.2001
                                              false,  // const bool finisheds = true,
                                              true,
                                              typesString,
                                              kindsString ),
                         len, algn) ;
          if ( dCphr == 618 )
              writeLong (os, adm2ndLawsCount (cCBEDBNPtr -> endD,
                                              cCBEDBNPtr -> begD,
                                              cCBEDBNPtr -> composit,  // 27.feb.2001
                                              true, // const bool finisheds = true,
                                              true,
                                              typesString,
                                              kindsString ),
                         len, algn) ;

        }
# endif
      break ;

    case 623 :  // For ADMIN report's summary  Брой заседания всичко
      { long int count623 = 0;
        if (cCBEDBNPtr)
        {  // -- 0 --
	    CharPtr crit (512) ;   ostrstream oss ( crit, crit.Size ()) ;
          oss << "SELECT COUNT(*) FROM T_SESSION " ;
          oss << "WHERE F_TYPE IN (" <<
                     stringToSQLCharSet (cCBEDBNPtr -> colleges).getTheSet ()
                         << ") " ;
          if ( cCBEDBNPtr -> composit > 0 )
              oss << "AND F_COMPOSITION=" << cCBEDBNPtr -> composit ;

	    oss << "AND F_DATE BETWEEN "
		  << cCBEDBNPtr -> begD.Value() << " AND "
		  << cCBEDBNPtr -> endD.Value()  << " " ;

          oss << "AND F_RESULT <> '" << RESULT_ABORTED << "' " ;

          oss << " ;" << ends ;
          crit [crit.Size () - 1] = '\0' ;

	    { long int count = 0;
		TDummyGroup cntGroup;
		cntGroup.Add(new TLong ("COUNT", & count)) ;

            TQuery q (crit) ;
		if (q.Read ())
            {
               cntGroup << q ;
               count623 += count;
          } }


          // from T_INREG_EVENT
          crit [0] = '\0' ;     ostrstream oss_I ( crit, crit.Size ()) ;
          oss_I << "SELECT COUNT(*) FROM T_INREG_EVENT E, T_INREG I, T_LAWSUIT L " ;
	    oss_I << "WHERE E.F_DATE BETWEEN " << cCBEDBNPtr -> begD.Value() << " AND "
								<< cCBEDBNPtr -> endD.Value()  << " " ;

          oss_I << "AND E.F_RESULT NOT IN(" << SQLCharSet(RESULT_INREG_RESOLUTIONS) << ")" ;

          oss_I <<   "AND I.F_NO = E.F_NO AND I.F_YEAR = E.F_YEAR " <<
                   "AND L.F_NO = I.F_LAWSUIT_NO AND L.F_YEAR = I.F_LAWSUIT_YEAR AND L.F_TYPE = I.F_LAWSUIT_TYPE " ;

          oss_I <<   "AND L.F_TYPE IN (" <<
                     stringToSQLCharSet (cCBEDBNPtr -> colleges).getTheSet ()
                               << ") " ;
          if ( cCBEDBNPtr -> composit > 0 )
              oss_I << "AND L.F_COMPOSITION=" << cCBEDBNPtr -> composit ;

          oss_I << " ;" << ends ;
          crit [crit.Size () - 1] = '\0' ;

	    { long int count = 0;
		TDummyGroup cntGroup;
		cntGroup.Add(new TLong ("COUNT", & count)) ;

            TQuery q (crit) ;
		if (q.Read ())
            {
               cntGroup << q ;
               count623 += count;
          } }

        }
        writeLong (os, count623, len, algn) ;
      }
      break ;

    case 624 :  // For report's summary
      writeLong (os, sessionsForRSCount (
                     NULL,       //const char * resultsString, // session result
                     cCBEDBNPtr, //const collCompBegDEndDBegN * cCBDEDBNPtr,
                     NULL,       //const char * ageString = NULL,
                     NULL,       //const char * pGrantChar = NULL,
                     KIND_OPEN_SESSIONS, //const char * pSessKindChar = NULL,
                     true),     //bool skcEq = true,
                                 //const char * lawKindStr = NULL,
                                 //const bool followSessDate = true,
                                 //const bool includingClosedSessions = false
                 len, algn) ;
      break ;
    case 625 :  // For ADMIN report's summary
     {
      ostrstream (buff, buffL)    // results - as 10258
        << RESULT_LEFTS_OR_STOPPEDS
        << RESULT_UNMOVEABLES
        << ends ;
      writeLong (os, sessionsForRSCount (
                     buff,       //const char * resultsString, // session result
                     cCBEDBNPtr, //const collCompBegDEndDBegN * cCBDEDBNPtr,
                     NULL,       //const char * ageString = NULL,
                     NULL,       //const char * pGrantChar = NULL,
                     KIND_OPEN_SESSIONS,//const char * pSessKindChar = NULL,
                     true),      //bool skcEq = true,
                                 //const char * lawKindStr = NULL,
                                 //const bool followSessDate = true,
                                 //const bool includingClosedSessions = false
                 len, algn) ;
      }
      break ;

    // last :   625  // 20060323 new codes  10612, 10613, 10614, 10615
                     // 20070923            10616, 10617, 10618
                     // 20080625 new codes - 10619 - 10622
                     // 20080710
    default :
      perceived = false ;
      break ;
   }
  if (buff)
    delete [] buff ;
  return perceived ;
 }  // tSCountOutput

