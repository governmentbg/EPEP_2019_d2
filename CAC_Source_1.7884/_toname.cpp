#include "cac.h"

void citizenUCNAndUCNTypeToName(const char *ucn, char ucnType, mstr &m, bool fathShort, bool nShort, bool includingFatherN,
	bool rank)
{
	TCitizen citizen;

	citizen.ucnType = ucnType;
	strcpy(citizen.ucn, ucn);

	if (citizen.Try())
	{
		if (rank)
			m.cat(citizen.Rank());

		if (nShort)
			m.printf("%c.", *citizen.name);
		else
			m.cat(citizen.name);

		if (includingFatherN && any(citizen.reName))
		{
			m.cat(" ");

			if (fathShort)
				m.printf("%c.", *citizen.reName);
			else
				m.cat(citizen.reName);
		}

		m.printf(" %s", citizen.family);

		if (any(citizen.reFamily))
			m.printf(" %s", citizen.reFamily);
	}
}

static void T_UCNAndUCNTypeToName(const char *ucn, char ucnType, mstr &m, TNamedGroup &ucnGroup)
{
	ucnGroup.ucnType = ucnType;
	strcpy(ucnGroup.ucn, ucn);

	if (ucnGroup.Try(ucnGroup.nameField->Name))
		m.cat(ucnGroup.nameField->S());
}

void firmUCNAndUCNTypeToName(const char *ucn, char ucnType, mstr &m)
{
	T_UCNAndUCNTypeToName(ucn, ucnType, m, TFirm());
}

void senderUCNAndUCNTypeToName(const char *ucn, char ucnType, mstr &m)
{
	T_UCNAndUCNTypeToName (ucn, ucnType, m, TSender());
}

void damnUCNAndUCNTypeToName(const char *ucn, char ucnType, mstr &m)
{
	T_UCNAndUCNTypeToName (ucn, ucnType, m, TDamn());
}

void ucnAndUcnTypeToName(const char *ucn, char ucnType, mstr &m)
{
	switch (ucn2uxn (ucnType))
	{
		case UXN_CITIZEN : citizenUCNAndUCNTypeToName(ucn, ucnType, m); break;
		case UXN_FIRM : firmUCNAndUCNTypeToName(ucn, ucnType, m); break;
		case UXN_SENDER : senderUCNAndUCNTypeToName(ucn, ucnType, m); break;
		case UXN_DAMN : damnUCNAndUCNTypeToName (ucn, ucnType, m); break;
		default : m.cat("??");
	}
}

bool citizenUCNAndUCNTypeToName (const char * ucn, const char ucnType,
					   char * destN, size_t destNSz,
					   bool fathShort, bool nShort,
					   bool includingFatherN,
					   bool rank)
 {
  if (! (destN && destNSz))
    return false ;
  if (ucn && strlen (ucn) && ucnType)
   {
    TCitizen c ;

    c.ucnType = ucnType ;
    ostrstream (c.ucn, sizeof (c.ucn)) << ucn << ends ;
    c.ucn [sizeof (c.ucn) - 1] = '\0' ;
    if (c.Try ())
     {
	 {
	  ostrstream oss (destN, destNSz) ;

	  if (rank)
	    oss << c.Rank();
	  if (nShort)
	    writeShortString (oss, c.name) ;
	  else
	    oss << c.name ;
	  if (includingFatherN)
	   {
	    oss << " " ;
	    if (fathShort)
		writeShortString (oss, c.reName) ;
	    else
		oss << c.reName ;
	   }
	  oss << " " << c.family ;
	  if (c.reFamily [0])
	    oss << " " << c.reFamily ;
	  oss << ends ;
	 }
	destN [destNSz - 1] = '\0' ;
	return true ;
     }
    else
     {
	destN [0] = '\0' ;
      return false ;
     }
   }
  else
   {
    destN [0] = '\0' ;
    return false ;
   }
 }  // citizenUCNAndUCNTypeToName

bool firmUCNAndUCNTypeToName (const char * ucn, const char ucnType,
					char * destN, size_t destNSz)
 {
  TFirm item ;

  return T_UCNAndUCNTypeToName (ucn, ucnType, destN, destNSz, item) ;
 }  // firmUCNAndUCNTypeToName

bool senderUCNAndUCNTypeToName (const char * ucn, const char ucnType,
					  char * dest, size_t destSz)
 {
  TSender item ;

  return T_UCNAndUCNTypeToName (ucn, ucnType, dest, destSz, item) ;
 }  // senderUCNAndUCNTypeToName

bool damnUCNAndUCNTypeToName (const char * ucn, const char ucnType,
					char * dest, size_t destSz)
 {
  TDamn item ;

  return T_UCNAndUCNTypeToName (ucn, ucnType, dest, destSz, item) ;
 }  // damnUCNAndUCNTypeToName

bool lawsuitToSenderName (const TLawsuit & lawsuit,
                          char * dest, size_t destSz,
                          bool courtNameOnly  )
{
  // Look for the old version of this function in _obsolete.cpp (20070130)
  bool res = false ;
  TLawsuit * pLawsuit = new TLawsuit ;
  pLawsuit -> key = lawsuit.key;
  //TConnect *c = main_connect( pLawsuit );
  //if ( c != NULL )
  TConnect *c = get_connect(pLawsuit, CONNECT_TYPE_LOWER_INSTANCES);
  if ( c != NULL )
    //if (strchr (CONNECT_TYPE_LOWER_INSTANCES, c -> connectType))
    {
        dest [0] = '\0' ;
        if (courtNameOnly)
        {
            TSender sender ;
         	  sender.ucnType = c -> ucnType;
            scSprintf (sizeof (sender.ucn), sender.ucn, c -> ucn) ;
            if (c -> ucnType && c -> ucn)
            {
			if ( sender.Get() )
              {
                  size_t len=0, len1=0; //, len2=0;
                  if (dest[0]) len = strlen (dest) ;
                  if (sender.name[0]) len1=strlen(sender.name);
                      if (len1>0)
                            ostrstream (dest + len, len+len1+1+1)
                <<  sender.name << ends ;
        }   } }

        else  connect_to_string( c, dest, destSz ); // withSender = true;
                                              // senderPrefix = NULL
                                              // labelOnly = false
        res = true;
    }

  delete c;
  delete pLawsuit;
  return res;
} // lawsuitToSenderName


//bool judgeUCNToName (const char * // ucn
//                     , char * dest, size_t destSz)
// Look for the old version of this function in _obsolete.cpp (20070130)


bool ucnAndUcnTypeToName (const char * ucn, char ucnType, char * dest,
                          size_t destSz)
 {
  if (! (dest && destSz))
    return false ;
  dest [0] = '\0' ;  // 21.may.2001 - begin
  if (! (ucn && ucnType))
    return false ;  // 21.may.2001 - end
  switch (ucn2uxn (ucnType))
   {
    case UXN_CITIZEN :
      return citizenUCNAndUCNTypeToName (ucn, ucnType, dest, destSz) ;
    case UXN_FIRM :
      return firmUCNAndUCNTypeToName (ucn, ucnType, dest, destSz) ;
    case UXN_SENDER :
      return senderUCNAndUCNTypeToName (ucn, ucnType, dest, destSz) ;
    case UXN_DAMN :  
      return damnUCNAndUCNTypeToName (ucn, ucnType, dest, destSz) ;
    default :
      if (destSz > 1)
        strcpy (dest, "?") ;
      if (destSz)
        dest [destSz - 1] = '\0' ;
      return false ;
   }
 }  // ucnAndUcnTypeToName

bool uclpToName (long uclp, char * dest, size_t destSz)
 {
  TUCLP settlement ;

  if (! (dest && destSz))
    return false ;
  if (! uclpCodeToTUCLP (uclp, settlement))
   {
    dest [0] = '\0' ;
    return false ;
   }
  ostrstream (dest, destSz) << settlement.name << ends ;
  dest [destSz - 1] = '\0' ;
  return true ;
 }  // uclpToName

bool uclpToAreaName (long uclp, char * dest, size_t destSz)
 {
  TUCLP settlement ;
  TArea Area ;

  if (! (dest && destSz))
    return false ;
  if (! uclpCodeToTUCLP (uclp, settlement))
   {
    dest [0] = '\0' ;
    return false ;
   }
  Area.area = settlement.area ;
  if (Area.Try ())
    ostrstream (dest, destSz) << Area.name << ends ;
  else
   {
    dest [0] = '\0' ;
    return false ;
   }
  dest [destSz - 1] = '\0' ;
  return true ;
 }  // uclpToAreaName

bool uclpToMunicipalityName (long uclp, char * dest, size_t destSz)
 {
  TUCLP settlement ;
  TRegion Region ;

  if (! (dest && destSz))
    return false ;
  if (! uclpCodeToTUCLP (uclp, settlement))
   {
    dest [0] = '\0' ;
    return false ;
   }
  Region.area = settlement.area ;
  Region.region = settlement.region ;
  if (Region.Try ())
   {
    ostrstream (dest, destSz) <<
                                 Region // settlement  // 18.jul.2000
                                           .name << ends ;
    dest [destSz - 1] = '\0' ;
    return true ;
   }
  else
   {
    dest [0] = '\0' ;
    return false ;
   }
 }  // uclpToMunicipalityName

bool streetToName (long street, long uclp, char * dest, size_t destSz)
 {
  TStreet Street ;

  if (! (dest && destSz))
    return false ;
  Street.street = street ;
  Street.uclp = uclp ;
  if (Street.Try ())
   {
    ostrstream (dest, destSz) << Street.name << ends ;
    dest [destSz - 1] = '\0' ;
    return true ;
   }
  else
   {
    dest [0] = '\0' ;
    return false ;
   }
 }  // streetToName

bool countryToName (const char * const c,
                    char * const dest, const size_t destSz)
 {
  if (! (dest && destSz))
    return false ;
  dest [0] = '\0' ;
  if (! (c && c [0]))
    return false ;

  TCountry cntr ;

  scSprintf (sizeof (cntr.country), cntr.country, "%.*s",
             sizeof (cntr.country) - 1, c) ;
  if (cntr.Try ())
   {
    scSprintf (destSz, dest, "%.*s", destSz - 1, cntr.name) ;
    return true ;
   }
  return false ;
 }  // countryToName

bool firm_subject_short_name(const long int subj, const char type,
	char* const dest, const size_t destSz, const bool unknownAsCode)
{  // 14.aug.2002
   // An old variant - in VersionArchives\1537\_appname.j --> _toname.cpp
	bool res = false;

	if(subj && type && dest && destSz)
	{
		dest[0] = '\0';
		if(destSz > 1)
		{
			TSubject tmpSubj;

			tmpSubj.type = type;
			tmpSubj.subject = subj;
			if(tmpSubj.Get())
			{
				ostrstream oss(dest, destSz);

        if(tmpSubj.alias[0])
          oss << tmpSubj.alias;
        else
          if(unknownAsCode)
		oss << tmpSubj.Code(0, false);
          else
            oss << tmpSubj.name;
				oss << ends;
			}
		}
		dest[destSz - 1] = '\0';
		if(dest[0])
			res = true;
	}
   else
		error("%s\n%s::firm_subject_short_name", WRONG_ARGS_PASSED_TO);
	return res;
}  // firm_subject_short_name

//

