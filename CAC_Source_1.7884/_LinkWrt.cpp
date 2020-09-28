#include "cac.h"

LinkWriter :: LinkWriter (stringArray & txtReceiver, char * const buf,
				  const size_t bufSz, const TRCDKey aLawKey) :
		    buffSz (bufSz), buff (buf), textReceiver (txtReceiver),
		    lawKey (aLawKey), useCommaChar(true)
 {
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
  clearGroupData (firmD) ;
  firmDFound = false ;
  firmDSearched = false ;
#   endif  // APPEAL
# endif  // INSTANCE
 }  // LinkWriter :: LinkWriter

LinkWriter :: ~LinkWriter ()
 {
 }  // LinkWriter :: ~LinkWriter

// 2014:216 char[] -> mstr
void LinkWriter::LinkTextItems (const TLinkWind &link, TGArray<TLinkWind> *links)
{
	mstr proxyInvlTxt ;
	mstr redirectInvlTxt ;
	mstr proxyNames ;
	mstr redirectNames ;
	TSideWind ordinary ;
	TSideWind proxy ;
	TSideWind redirect ;
	int count = links ? links->Count() : 1;
	mstr ordinaryNames;

	buff [buffSz - 1] = '\0' ;
	link.GetOrdinary (ordinary) ;

	if (ordinary.ucnType && ucnPresent (ordinary.ucn) && ordinary.involvement)
	{  // -- 0 --
		// 2014:206 involvement
		if (link.FilledOk() && link.Proxied() && link.proxyKind == SIDE_MARK_AS_WITH_INVOLVEMENT)
			ordinaryNames.printf("%s ", Involvement->Seek(ordinary.involvement));

		ordinary.PrintNames(ordinaryNames);

		for (int i = 1; i < count; i++)
		{
			ordinaryNames.cat(", ");
			(*links)[i]->GetOrdinary(ordinary);
			// 2014:206 involvement
			if (link.FilledOk() && link.Proxied() && link.proxyKind == SIDE_MARK_AS_WITH_INVOLVEMENT)
				ordinaryNames.printf("%s ", Involvement->Seek(ordinary.involvement));

			ordinary.PrintNames(ordinaryNames);
		}

		if (link.FilledOk())
		{  // Get proxy and probably redirect data
			link.GetProxy (proxy) ;
			proxy.PrintNames(proxyNames);
			proxyInvlTxt.cat(Involvement->Seek(proxy.involvement));

			if (link.Redirected ())
			{
				link.GetRedirect (redirect) ;
				redirect.PrintNames(redirectNames);
				redirectInvlTxt.cat(Involvement->Seek(redirect.involvement));
			}
		}

		// ------------ Fill receiver text items ---------------
		if (link.FilledOk ())
		{  // -- 1 --
			// 2014:216 support for with involvement
			if (! strchr (SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS, proxy.involvement) &&
				link.proxyKind != SIDE_MARK_AS_WITH_INVOLVEMENT)
			{  // -- 2 --  // без уточняване на връзката - TSidesRelationDialog
				// 20051121 ->
				if (proxy.involvement == LINK_PARENT)
				{  // -- 2' --
					if (link.Redirected ())
					{
						textReceiver.Add (ordinaryNames.newstr()) ;
						ostrstream (buff, buffSz - 1)
							<< "със съгласието на " << str(proxyInvlTxt)
							<< BONEVA_SIDES_DELIMITER << ends ;
						textReceiver.Add (strnew (buff)) ;
						ostrstream (buff, buffSz - 1)
							<< str(proxyNames) << BONEVA_SIDES_DELIMITER << ends ;
						textReceiver.Add (strnew (buff)) ;
						ostrstream (buff, buffSz - 1)
							<< "чрез " << str(redirectInvlTxt) << " " << str(redirectNames)
							<< ends ;
						textReceiver.Add (strnew (buff)) ;
					}
					else
					{
						textReceiver.Add (strnew (" ")) ;
						textReceiver.Add (ordinaryNames.newstr()) ;
						ostrstream (buff, buffSz - 1)
							<< "със съгласието на " << str(proxyInvlTxt)
							<< BONEVA_SIDES_DELIMITER << ends ;
						textReceiver.Add (strnew (buff)) ;
						textReceiver.Add (proxyNames.newstr()) ;
					}
				}  // -- 2' --
				// 20051121 <-
				else
				{
					if (link.Redirected ())
					{
						ostrstream (buff, buffSz - 1)
							<< str(ordinaryNames) << BONEVA_SIDES_DELIMITER << ends ;
						textReceiver.Add (strnew (buff)) ;
						ostrstream (buff, buffSz - 1)
							<< "чрез " << str(proxyInvlTxt) << ends ;
						textReceiver.Add (strnew (buff)) ;
						ostrstream (buff, buffSz - 1)
							<< str(proxyNames) << BONEVA_SIDES_DELIMITER << ends ;
						textReceiver.Add (strnew (buff)) ;
						ostrstream (buff, buffSz - 1)
							<< "чрез " << str(redirectInvlTxt) << " " << str(redirectNames) << ends ;
						textReceiver.Add (strnew (buff)) ;
					}
					else
					{
						textReceiver.Add (strnew (" ")) ;
						ostrstream (buff, buffSz - 1)
							<< str(ordinaryNames) << BONEVA_SIDES_DELIMITER << ends ;
						textReceiver.Add (strnew (buff)) ;
						ostrstream (buff, buffSz - 1)
							<< "чрез " << str(proxyInvlTxt) << ends ;
						textReceiver.Add (strnew (buff)) ;
						textReceiver.Add (proxyNames.newstr()) ;
					}
				}
			}  // -- 2 --
			// Видове "уточнения" на връзките -->
			else if (link.proxyKind == SIDE_MARK_INDIRECT_AS_ANY_1)
			// 20070730 -->
			{
				if (link.Redirected ())
				{
					if (useCommaChar)
						ordinaryNames.cat(",");
					textReceiver.Add (ordinaryNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< " представляван(а) от " << str(proxyInvlTxt) << " " << ends ;
					textReceiver.Add (strnew (buff)) ;
					textReceiver.Add (proxyNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< "  чрез " << str(redirectInvlTxt) << " " << str(redirectNames)
						<< ends ;
					textReceiver.Add (strnew (buff)) ;
				}
				else
				{
					if (useCommaChar)
						ordinaryNames.cat(",");
					textReceiver.Add (ordinaryNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< " представляван(а) от " << str(proxyInvlTxt) << " " << ends ;
					textReceiver.Add (strnew (buff)) ;
					textReceiver.Add (proxyNames.newstr()) ;
				}
			}  // 20070730 <--
			else if (link.proxyKind == SIDE_MARK_INDIRECT_AS_ANY_2)
			// 20080301 -->
			{
				if (link.Redirected ())
				{
					textReceiver.Add (ordinaryNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< " чрез " << str(proxyInvlTxt) << " " << ends ;
					textReceiver.Add (strnew (buff)) ;
					textReceiver.Add (proxyNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< " чрез " << str(redirectInvlTxt) << " " << str(redirectNames)
						<< ends ;
					textReceiver.Add (strnew (buff)) ;
				}
				else
				{
					textReceiver.Add (ordinaryNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< " чрез " << str(proxyInvlTxt) << " " << ends ;
					textReceiver.Add (strnew (buff)) ;
					textReceiver.Add (proxyNames.newstr()) ;
				}
			}
			else if (link.proxyKind == SIDE_MARK_INDIRECT_AS_ANY_3)
			// 20080301 -->
			{
				if (link.Redirected ())
				{
					textReceiver.Add (ordinaryNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< " със съгласието на " << str(proxyInvlTxt) << " " << ends ;
					textReceiver.Add (strnew (buff)) ;
					textReceiver.Add (proxyNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< "  чрез " << str(redirectInvlTxt) << " " << str(redirectNames)
						<< ends ;
					textReceiver.Add (strnew (buff)) ;
				}
				else
				{
					textReceiver.Add (ordinaryNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< " със съгласието на " << str(proxyInvlTxt) << " " << ends ;
					textReceiver.Add (strnew (buff)) ;
					textReceiver.Add (proxyNames.newstr()) ;
				}
			}
			else if (link.proxyKind != SIDE_MARK_CONSENT_OF_INDIRECT)
			{  // -- 3 --
				if (link.Redirected ())
				{
					textReceiver.Add (proxyNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< "като " << str(proxyInvlTxt) << " на"
						<< BONEVA_SIDES_DELIMITER << ends ;
						textReceiver.Add (strnew (buff)) ;
					ostrstream (buff, buffSz - 1)
						<< str(ordinaryNames) << BONEVA_SIDES_DELIMITER << ends ;
					textReceiver.Add (strnew (buff)) ;
					ostrstream (buff, buffSz - 1)
						<< "чрез " << str(redirectInvlTxt) << " " << str(redirectNames)
						<< ends ;
					textReceiver.Add (strnew (buff)) ;
				}
				else
				{
					textReceiver.Add (strnew (" ")) ;
					textReceiver.Add (proxyNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< "като " << str(proxyInvlTxt) << " на"
						<< BONEVA_SIDES_DELIMITER << ends ;
					textReceiver.Add (strnew (buff)) ;
					textReceiver.Add (ordinaryNames.newstr()) ;
				}
			}  // -- 3 --
			else
			{  // -- 4 --
				if (link.Redirected ())
				{
					ostrstream (buff, buffSz - 1)
						<< str(ordinaryNames) << BONEVA_SIDES_DELIMITER << ends ;
					textReceiver.Add (strnew (buff)) ;
					textReceiver.Add (strnew ("със съгласието на"));
					ostrstream (buff, buffSz - 1)
						<< str(proxyNames) << " " << "като " << str(proxyInvlTxt)
						<< BONEVA_SIDES_DELIMITER << ends ;
					textReceiver.Add (strnew (buff)) ;
					ostrstream (buff, buffSz - 1)
						<< "чрез " << str(redirectInvlTxt) << " " << str(redirectNames)
						<< ends ;
					textReceiver.Add (strnew (buff)) ;
				}
				else
				{
					ostrstream (buff, buffSz - 1)
						<< str(ordinaryNames) << BONEVA_SIDES_DELIMITER << ends ;
					textReceiver.Add (strnew (buff)) ;
					textReceiver.Add (strnew ("със съгласието на")) ;
					textReceiver.Add (proxyNames.newstr()) ;
					ostrstream (buff, buffSz - 1)
						<< "като " << str(proxyInvlTxt) << ends ;
					textReceiver.Add (strnew (buff)) ;
				}
			}  // -- 4 --
		}  // -- 1 --
		else
		{
			textReceiver.Add (strnew (" ")) ;
			textReceiver.Add (ordinaryNames.newstr()) ;
			textReceiver.Add (strnew (" ")) ;
			textReceiver.Add (strnew (" ")) ;
		}
	}  // -- 0 --
}  // LinkWriter :: LinkTextItems

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
bool LinkWriter ::
	 DifferFLKeyOK (const TRCDKey * const pDifferentFirmLawKey) const
 {
  return
    strchr (TYPE_INREGS, lawKey.type) &&
    pDifferentFirmLawKey && pDifferentFirmLawKey->type == TYPE_FIRM_LAW &&
    TRCDKey_ (* pDifferentFirmLawKey).FilledOK () ;
 }  // LinkWriter :: DifferFLKeyOK

bool LinkWriter :: FirmDLoaded (const TRCDKey * const pDifferentFirmLawKey)
 {
  const bool diffLK_OK = DifferFLKeyOK (pDifferentFirmLawKey) ;
  bool res = false ;

  if (TRCDKey_ (lawKey).FilledOK () &&
      (lawKey.type == TYPE_FIRM_LAW || diffLK_OK))
   {
    if (firmDFound)
      res = true ;
    else
	if (firmDSearched == false)
       {
        TLawsuit law ;

        firmDSearched = true ;
	  law.key = diffLK_OK ? (* pDifferentFirmLawKey) : lawKey ;
        firmDFound = lawsuitToDamn (& law, & firmD) ;
        if (! firmDFound)
          clearGroupData (firmD) ;
        res = firmDFound ;
       }
   }
  return res ;
 }  // LinkWriter :: FirmDLoaded

void LinkWriter :: FakeReceiver (const TLinkWind & link,
                                 const bool firmInfoAtIndex2IfPossible)
 {
  // ------- preliminay checks - begin --------------------
  if (TRCDKey_ (lawKey).FilledOK () == false)
    return ;

  TRCDKey differentFirmLawKey ;

  differentFirmLawKey.no = differentFirmLawKey.year =
  differentFirmLawKey.type = '\0' ;
  if (strchr (TYPE_INREGS, lawKey.type))
   {
    TInReg inReg ;

    inReg.key = lawKey ;
    if ((inReg.Get () &&
         inreg_to_law_type (inReg, & differentFirmLawKey)
        ) == false)
	return ;
   }

  const bool diffLK_OK = DifferFLKeyOK (& differentFirmLawKey) ;
  // ------- preliminay checks - end --------------------

  if (lawKey.type == TYPE_FIRM_LAW || diffLK_OK)
   {
    if (link.FilledOk ())
     {
      TSideWind w ;
      bool unexpectedInvolvement = false ;

      link.GetProxy (w) ;
      if (strchr (SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS, w.involvement))
        unexpectedInvolvement = true ;
      else
        if (link.Redirected ())
         {
          link.GetRedirect (w) ;
          if (strchr (SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS, w.involvement))
            unexpectedInvolvement = true ;
         }
	if (unexpectedInvolvement)
       {
        error ("Unexpected involvement for firm lawsuits") ;
        return ;
       }
     }

    const int requiredItems = 4 ;

    if (textReceiver.Count () == requiredItems)
     {
      TSideWind ordinary ;
      stringArray tmpSA ;

	link.GetOrdinary (ordinary) ;
       {
        for (int i = 0 ; i < requiredItems ; i ++)
	    tmpSA.Add (strnew (textReceiver [i] )) ;
       }
      textReceiver.Flush () ;
	if (strIsBlank (tmpSA [0] ))
       {
        buff [0] = '\0' ;
	  strConcat (buff, tmpSA [1] , buffSz) ;
        strConcat (buff, " - ", buffSz) ;
         {
          size_t l = strlen (buff) ;

          if (l + 1 < buffSz)
		involvementToString (ordinary.involvement, buff + l,
                                 buffSz - l);
         }
        textReceiver.Add (strnew (buff)) ;
        buff [0] = '\0' ;
        strConcat (buff, "на ", buffSz) ;
	  strConcat (buff, FirmDLoaded (& differentFirmLawKey) ?
                           firmD.name : "?", buffSz) ;
	  if (strIsBlank (tmpSA [2] ) &&
		strIsBlank (tmpSA [3] ) &&
            firmInfoAtIndex2IfPossible)
         {  // I wonder what a boza can I write !
	    textReceiver.Add (strnew (tmpSA [2] )) ;
          textReceiver.Add (strnew (buff)) ;
         }
        else
         {
          textReceiver.Add (strnew (buff)) ;
	    textReceiver.Add (strnew (tmpSA [2] )) ;
         }
       }
      else
       {
        buff [0] = '\0' ;
	  strConcat (buff, tmpSA [0] , buffSz) ;
        strConcat (buff, " - ", buffSz) ;
         {
          size_t l = strlen (buff) ;

	    if (l + 1 < buffSz)
            involvementToString (ordinary.involvement, buff + l, buffSz - l);
          if ((l = strlen (buff) + 1) < buffSz)
            ostrstream (buff + l, buffSz - l)
              << " " << "на "
              << (FirmDLoaded (& differentFirmLawKey) ?
			  firmD.name : "?") << ends ;
         }
        textReceiver.Add (strnew (buff)) ;
	  textReceiver.Add (strnew (tmpSA [1] )) ;
	  textReceiver.Add (strnew (tmpSA [2] )) ;
	 }
	textReceiver.Add (strnew (tmpSA [3] )) ;
     }
    else
      error ("textReceiver: wrong number of items") ;
   }
 }  // LinkWriter :: FakeReceiver

#   endif  // of APPEAL
# endif  // of INSTANCE

void LinkWriter :: MakeLinkTextItems (const TLinkWind & link,
					const bool fake,
				     const bool firmInfoAtIndex2IfPossible,
				     TGArray<TLinkWind> *links)
 {
  TSideWind ordinary ;

  link.GetOrdinary (ordinary) ;
  if (ordinary.ucnType && ucnPresent (ordinary.ucn) &&
	ordinary.involvement)
   {
    LinkTextItems (link, links) ;
    if (fake)
     {
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
	FakeReceiver (link, firmInfoAtIndex2IfPossible) ;
#   endif  // APPEAL
# endif  // INSTANCE
      if (firmInfoAtIndex2IfPossible)
       {
        // suppress the warning
       }
     }
   }
 }  // LinkWriter :: MakeLinkTextItems

const char * LinkWriter :: FirmDName ()
 {
  const char * name = "" ;

# if INSTANCE
#   if APPEAL
#   else  // APPEAL
  if (FirmDLoaded ())
    name = firmD.name ;
#   endif  // APPEAL
# endif  // INSTANCE
  return name ;
 }  // LinkWriter :: FirmDName
