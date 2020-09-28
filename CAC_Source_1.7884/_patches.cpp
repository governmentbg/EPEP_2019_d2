long int
definTermin_grantNone_cnt_patch (
                            const collCompBegDEndDBegN * const cCBEDBNPtr,
					   const char * const ageStr,
                                 const char * const kP,
                                 const bool followSessDate
                                )
//
// Now the period before PATCH_04_AUG_2000_RDT_DATE is excluded from the
// processing
 {
  long int v3 = 0 ;

  if (cCBEDBNPtr)
   {  // -- 0 --
# if INSTANCE
#   if APPEAL
    if (followSessDate && ageStr && kP)
     {
      // Nothing. I'm suppressing the warnings.
     }
#   else  // APPEAL
    if (cCBEDBNPtr -> firstInstLaws &&
        strchr (cCBEDBNPtr -> colleges, TYPE_PUNISHMENT_LAW) == NULL)
     {  // -- 1 --
#     if PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
      const char grantNone = GRANT_NONE_YET ;
      char tmpResStr [2] ;

      scSprintf (sizeof (tmpResStr), tmpResStr, "%c",
                 RESULT_DEFINITION_TERMINATING) ;
      if (cCBEDBNPtr -> endD >= PATCH_04_AUG_2000_RDT_DATE)
       {  // -- 2 --
        collCompBegDEndDBegN fakedCBDEDBN (* cCBEDBNPtr) ;

        if (fakedCBDEDBN.begD < PATCH_04_AUG_2000_RDT_DATE)
          fakedCBDEDBN.begD = PATCH_04_AUG_2000_RDT_DATE ;
        v3 = sessionsForTSCount (tmpResStr, & fakedCBDEDBN, ageStr,
                                 & grantNone,
                                 kP, false, NULL, NULL, followSessDate) ;
       }  // -- 2 --
#     else  // PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
      if (followSessDate && ageStr && kP)
       {
        // Nothing. I'm suppressing the warnings.
       }
#     endif  // PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
     }  // -- 1 --
#   endif  // APPEAL
# else  // INSTANCE
    if (strchr (cCBEDBNPtr -> colleges, TYPE_PUNISHMENT_LAW) == NULL)
     {  // -- 3 --
#     if PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
      const char grantNone = GRANT_NONE_YET ;
      char tmpResStr [2] ;

      scSprintf (sizeof (tmpResStr), tmpResStr, "%c",
                 RESULT_DEFINITION_TERMINATING) ;
      if (cCBEDBNPtr -> endD >= PATCH_04_AUG_2000_RDT_DATE)
       {  // -- 4 --
        collCompBegDEndDBegN fakedCBDEDBN (* cCBEDBNPtr) ;

        if (fakedCBDEDBN.begD < PATCH_04_AUG_2000_RDT_DATE)
          fakedCBDEDBN.begD = PATCH_04_AUG_2000_RDT_DATE ;
        v3 = sessionsForTSCount (tmpResStr, & fakedCBDEDBN, ageStr,
                                 & grantNone,
                                 kP, false, NULL, NULL, followSessDate) ;
       }  // -- 4 --
#     else  // PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
      if (followSessDate && ageStr && kP)
       {
        // Nothing. I'm suppressing the warnings.
       }
#     endif  // PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
     }  // -- 3 --
# endif  // INSTANCE
   }  // -- 0 --
  return v3 ;
 }  // definTermin_grantNone_cnt_patch

//
