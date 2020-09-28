#include "cac.h"  // 18.may.2001
//# include "all.h"

AppealedLawsOutRegEnum :: AppealedLawsOutRegEnum ()
 {
 }  // AppealedLawsOutRegEnum :: AppealedLawsOutRegEnum

void AppealedLawsOutRegColumns ::
  AddLinkedInregsToColumn (column * pColumn, char * const buff,
                           const size_t buffL, const TOutReg & rOutReg)
 {
  if (! (pColumn && buff && buffL))
    return ;

  char crit [256] ;

  ostrstream (crit, sizeof (crit)) <<
    "SELECT DISTINCT F_NO, F_YEAR FROM T_INREG_EVENT "
    "WHERE F_OUT_NO = " << rOutReg.key.no << " AND "
          "F_OUT_YEAR = " << rOutReg.key.year << " ;" << ends ;
  crit [sizeof (crit) - 1] = '\0' ;
  if (crit [strlen (crit) - 1] == ';')
   {  // -- 3 --
    TQuery q (crit) ;

    while (q.Read ())
     {  // -- 4 --
      buff [0] = '\0' ;

      TInReg inReg ;  // It does not matter from which table
                      // have been given .key.no and .key.year

      inReg << q ;
      inReg.key.type = TYPE_INREG ;
      if (inReg.Get ())
      {
        if (inReg.kind != KIND_OTHER_IN_DOCUMENT) // 20080627 ->
        {
          if (inReg.kind)
          {  // -- 5 --
            TKind k ;
            k.kind = inReg.kind ;
            if (k.Get ())
               ostrstream (buff + strlen (buff),
                           buffL - strlen (buff))
                           << "отн. " << k.name << " " << ends ;
          }  // -- 5 --
          ostrstream (buff + strlen (buff),
                      buffL - strlen (buff))
                      << " вх.No "<< inReg.key.no << "/"
                      << inReg.key.year << "г." << ends ;
        }
        else
        {
           if (inReg.text[0])
               ostrstream (buff + strlen (buff),
                           buffL - strlen (buff))
                           << "отн. " << inReg.text << " "
                           << " вх.No "<< inReg.key.no << "/"
                           << inReg.key.year << "г." << ends ;
        }
      }
      if (buff [0])
        pColumn -> addText (buff) ;
     }  // -- 4 --
   }  // -- 3 --
 }  // AppealedLawsOutRegColumns :: AddLinkedInregsToColumn

void AppealedLawsOutRegColumns ::
  AddReceiversToColumn (column * pColumn, char * const buff,
                        const size_t buffL, TOutReg & rOutReg)
 {  // Keep this logic in conformity with TRegMailListWindow::GetReceiver
  if (! (pColumn && buff && buffL))
    return ;

  if (ucnPresent (rOutReg.receiver))
   {
  		if (senderUCNAndUCNTypeToName (rOutReg.receiver,
                     // 20061220     UCN_INSTANCE_THIS,
                                     UCN_INSTANCE_HIGHER,
																	   buff, buffL))
			pColumn -> addText (buff) ;
	 }
	else
	 {  // 09.jul.2002 - begin
		bool walnsChecked = false ;
		bool walnReceiver = false ;  // 09.jul.2002 - end

		if (DatedContainer_ (rOutReg).FilledOK ())  // 12.feb.2002
		 {  // -- 0 --
			TSideWind waln ;

			waln << rOutReg ;
			waln.FloatCriteria (buff) ;
			if (strlen (buff) < buffL - 2)
			 {  // -- 1 --
				char * const partArea = buff + strlen (buff) + 1 ;
				const size_t partLen = buffL - 1 - strlen (buff) ;
				TQuery q (buff) ;

				walnsChecked = true ;  // 09.jul.2002
				while (q.Read ())
				 {  // -- 2 --
					waln << q ;
					ucnAndUcnTypeToName (waln.ucn, waln.ucnType,
															 partArea, partLen) ;
					pColumn -> addText (partArea) ;
					walnReceiver = true ;  // 09.jul.2002
				 }  // -- 2 --
			 }  // -- 1 --
		 }  // -- 0 --
		if (walnReceiver == false)  // 09.jul.2002 - begin
			if (outrg_retLawToLowerInst_weakCond(rOutReg, walnsChecked == false))
				if (TRCDKey_ (rOutReg.lawsuit).FilledOK ())
				 {
					LawPtr pLaw ;

					pLaw.Ref ().key = rOutReg.lawsuit ;
					if (pLaw.Ref ().Get())
					 {
						char sn [SIZE_OF_NAME] ;
						 {
							ostrstream oss (sn, sizeof (sn)) ;
							char sn1 [SIZE_OF_NAME] ;

							lawsuitOutput (pLaw.Ref (), oss, sn1, sizeof (sn1) - 1,
								LEFT_ALIGN, 0, 44 + OFFSET_LAWSUIT_CIPHER) ;
						 }
						ENDING_ZERO (sn) ;
						strDelLastSpaces (sn) ;
						if (sn [0])
						  pColumn -> addText (sn) ;
					 }
				 }
	 }  // 09.jul.2002 - end
 }  // AppealedLawsOutRegColumns :: AddReceiversToColumn

void AppealedLawsOutRegColumns :: MakeColumns (TOutReg & rOutReg,
                                               const int // serialNo
                                              )
 {
  const size_t buffSz = MAKE_COLUMNS_BUFF_SIZE ;
  const size_t buffL = buffSz - 1 ;
  char * const buff = new char [MAKE_COLUMNS_BUFF_SIZE] ;
  column * pColumn ;

  buff [buffL] = '\0' ;
  if ((pColumn = columns [iC_OutRegNoOnly]) != NULL)
   {  // -- iC_OutRegNoOnly --
    ostrstream (buff, buffL) << rOutReg.key.no << ends ;
    pColumn -> addText (buff) ;
   }  // -- iC_OutRegNoOnly --
  if ((pColumn = columns [iC_DateSent]) != NULL)
   {  // -- iC_DateSent --
    ostrstream (buff, buffL) << rOutReg.date << ends ;
    pColumn -> addText (buff) ;
   }  // -- iC_DateSent --

  if ((pColumn = columns [iC_DocKind]) != NULL)
  { // -- iC_DocKind --   36003
    if (rOutReg.kind != KIND_OTHER_OUT_DOCUMENT)
    {
        kindToString (rOutReg.kind, buff, buffL) ;
        pColumn -> addText (buff) ;

        if ((rOutReg.inNo > 0) && (rOutReg.inYear > 0))
           AddLinkedInregsToColumn (pColumn, buff, buffL, rOutReg) ;

	  if (any(rOutReg.text)) pColumn -> addText (rOutReg.text) ;

    }
    else
    { if (any(rOutReg.text)) pColumn -> addText (rOutReg.text) ;
    }
  } // -- iC_DocKind --

  if ((pColumn = columns [iC_ComeInDate]) != NULL)
   {  // -- iC_ComeInDate --
//    if (rOutReg.inDate.Empty () == false)
//     {
//       {
//        TType t ;
//
//        t.type = TYPE_INREG ;
//        if (t.Get ())
//         {
//          ostrstream (buff, buffL) << "Дата на " << t.name
//                                   << ":" << ends ;
//          pColumn -> addText (buff) ;
//         }
//       }
//      ostrstream (buff, buffL) << rOutReg.inDate << ends ;
//      pColumn -> addText (buff) ;
//     }
   }  // -- iC_ComeInDate --
   {  // 18.apr.2001 :
      // An old processing of the iC_LawKindLawNoYear column -
      // in VersionArchives\860\_appname.j  --> _OutRgBk.cpp
    TInReg inReg ;

    inReg.key.type = TYPE_INREG ;
    inReg.key.no = rOutReg.inNo ;
    inReg.key.year = rOutReg.inYear ;
    if (TRCDKey_ (inReg.key).FilledOK () && inReg.Get ())
     {  // inReg.Get () ===
      TLawsuit law ;

      clearGroupData (law) ;
      if (inReg.decision.type)
       {
        if (strchr (TYPE_LAWSUITS, inReg.decision.type))
          law.key = inReg.lawsuit ;
        else
          if (strchr (TYPE_INREGS, inReg.decision.type))
           {
            TInReg * pAplIR = new TInReg ;

            pAplIR -> key = inReg.decision ;
            if (TRCDKey_ (pAplIR -> key).FilledOK () && pAplIR -> Get ())
              law.key = pAplIR -> lawsuit ;
            delete pAplIR ;
           }
       }
      if (law.key.type)
       {  // law.key.type ===
        if (law.Get ())
         {
          if ((pColumn = columns [iC_LawKindLawNoYear]) != NULL)
           {  // -- iC_LawKindLawNoYear --
            ostrstream (buff, buffL)
              << TRCDKeyContainer_ (law.key, law.kind) << ends ;
            if (strlen (buff) < buffL)  // 27.mar.2002 - begin
             {
              CharPtr tmp (256) ;  // I hope, 256 is enough

              preliminary_actions_to_string (tmp, tmp.Size (), law,
							   C2S[UCN_INSTANCE_DETECT]) ;
              if (tmp [0])
               {
                strConcat (buff, ", ", buffL) ; //buffL+1 is acceptable,too
                strConcat (buff, tmp, buffL) ;
               }
             }  // 27.mar.2002 - end
            pColumn -> addText (buff) ;
           }  // -- iC_LawKindLawNoYear --
         }
       }  // law.key.type ===
      else
        if (strchr (TYPE_REQUESTS, inReg.decision.type))
         {
          TRequest * pRequest = new TRequest ;

          pRequest -> key = inReg.lawsuit ;
          if (pRequest -> key.type && pRequest -> Get ())
           {
            if ((pColumn = columns [iC_LawKindLawNoYear]) != NULL)
             {  // -- iC_LawKindLawNoYear --
              ostrstream (buff, buffL)
                << TRCDKeyContainer_ (pRequest -> key, pRequest -> kind)
                << ends ;
              pColumn -> addText (buff) ;
             }  // -- iC_LawKindLawNoYear --
           }
          delete pRequest ;
         }
     }  // inReg.Get () ===
    else
      if (rOutReg.inNo == 0 && rOutReg.inYear == 0 &&
          TRCDKey_ (rOutReg.lawsuit).FilledOK ())
       {
        if (strchr (TYPE_LAWSUITS, rOutReg.lawsuit.type))  // 31.jan.2002
         {
          TLawsuit law ;

          law.key = rOutReg.lawsuit ;
          if (law.Get ())
            if ((pColumn = columns [iC_LawKindLawNoYear]) != NULL)
             {  // -- iC_LawKindLawNoYear --
              ostrstream (buff, buffL)
                << TRCDKeyContainer_ (law.key, law.kind) << ends ;
              pColumn -> addText (buff) ;
             }  // -- iC_LawKindLawNoYear --
         }
        else  // 31.jan.2002 - begin
          if (strchr (TYPE_REQUESTS, rOutReg.lawsuit.type))  
           {
            TRequest req ;

            req.key = rOutReg.lawsuit ;
            if (req.Get ())
              if ((pColumn = columns [iC_LawKindLawNoYear]) != NULL)
               {  // -- iC_LawKindLawNoYear --
                ostrstream (buff, buffL)
                  << TRCDKeyContainer_ (req.key, req.kind) << ends ;
                pColumn -> addText (buff) ;
               }  // -- iC_LawKindLawNoYear --
           }  // 31.jan.2002 - end
       }
   }
  if ((pColumn = columns [iC_Destination]) != NULL)
   {  // -- iC_Destination --
		AddReceiversToColumn (pColumn, buff, buffL, rOutReg) ;
   }  // -- iC_Destination --
	if ((pColumn = columns [iC_DateReturned]) != NULL)
   {  // -- iC_DateReturned --
    if (rOutReg.returned.Empty () == false)
     {
			ostrstream (buff, buffL) << rOutReg.returned << ends ;
      pColumn -> addText (buff) ;
     }
   }  // -- iC_DateReturned --
  if ((pColumn = columns [iC_Remark]) != NULL)
   {  // -- iC_Remark --
    // 2012:003 ERR
    if (any(rOutReg.remark))
	pColumn -> addText (rOutReg.remark) ;
   }  // -- iC_Remark --
  delete [] buff ;
 }  // AppealedLawsOutRegColumns :: MakeColumns
