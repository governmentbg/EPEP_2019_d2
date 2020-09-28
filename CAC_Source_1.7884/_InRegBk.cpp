#include "cac.h"  // 18.may.2001
//# include "all.h"

bool AppealedLawsInputDiary ::
	 InregsByLawTypesPartialCriteria (ostream & os, char * const types)
 {
  bool res = false ;

  if (types && types [0] && os.good ())
   {
    char lawKinds [SIZE_OF_KINDS] ;

    if ( law_types_to_law_kinds (types, lawKinds, sizeof (lawKinds)) )
     {
      const stringToSQLCharSet typesSet (types) ;
      const char * const tS = typesSet.getTheSet () ;
      const stringToSQLCharSet typeInDocs (TYPE_REQUESTS) ;
      const char * const inDocs = typeInDocs.getTheSet () ;

      os
       << "AND ("  // = 0 =
               "("  // = 1a =
                "((NOT (I.F_DECISION_TYPE IN (" << inDocs << ")))"
                  " OR I.F_DECISION_TYPE IS NULL"  // 25.jul.2001
                ")"
                " AND "
                "("  // = 2a =
                 "(I.F_LAWSUIT_TYPE IS NOT NULL AND "
                  "I.F_LAWSUIT_TYPE IN (" << tS << ")"
                 ") OR "
                 "(I.F_LAWSUIT_TYPE IS NULL "
                  "AND I.F_COLLEGE IN (" << tS << ")"
                 ")"
                ")"  // = 2a =
               ")"  // = 1a =
                " OR "
               "("  // = 1b =
                "I.F_DECISION_TYPE IN (" << inDocs << ")"
                " AND "
                "("  // = 2b =
                  "(SELECT COUNT (R.F_NO) FROM T_REQUEST R WHERE "
                   "R.F_TYPE = I.F_LAWSUIT_TYPE AND "
                   "R.F_YEAR = I.F_LAWSUIT_YEAR AND "
                   "R.F_NO = I.F_LAWSUIT_NO AND " ;
      request_by_lawKinds_part_crit (os, lawKinds, "R.") ;
      os <<       ") != 0"
                ")"  // = 2b =
               ")"  // = 1b =
              ") " ;  // = 0 =
      res = os.good () ;
     }
   }
  if (! res)
    error ("%s",
           "AppealedLawsInputDiary :: InregsByLawTypesPartialCriteria"
           " failed!") ;
  return res ;
 }  // AppealedLawsInputDiary :: InregsByLawTypesPartialCriteria

bool AppealedLawsInputDiary :: MakeCriteria ()
 {
  criteria [0] = '\0' ;
  if (! (bnenyt.types [0]))
    return false ;
   {
    ostrstream oss (criteria, sizeof (criteria)) ;

    oss << "SELECT I.* FROM T_INREG I "
           "WHERE " 
           "I.F_YEAR = " << bnenyt.year << " AND "
           "I.F_NO " ;
    if (bnenyt.endN > 0)
      oss
        << "BETWEEN " << bnenyt.begN << " AND " << bnenyt.endN ;
    else
      oss << ">= " << bnenyt.begN ;
    oss << " " ;
    if (! InregsByLawTypesPartialCriteria (oss, bnenyt.types))
      return false ;
    oss
        << " ORDER BY I.F_NO ;" << ends ;
   }     
  criteria [sizeof (criteria) - 1] = '\0' ;
  return criteria_finished_ok (criteria,
                               "AppealedLawsInputDiary::MakeCriteria") ;
 }  // AppealedLawsInputDiary :: MakeCriteria

void AppealedLawsInputDiary :: MakeQuery ()
 {
  if (MakeCriteria ())
   {
    TWaitWindow ww (pParent, PLEASE_WAIT) ;

    pQ = new TQuery (criteria) ;
   } 
 }  // AppealedLawsInputDiary :: MakeQuery

void AppealedLawsInputDiary :: MakeBookFormatFN ()
 {
  scSprintf (sizeof (formatFN), formatFN,
             "text\\aplinreg%s", DEFAULT_FORMAT_FILE_EXTENSION) ;
 }  // AppealedLawsInputDiary :: MakeBookFormatFN

AppealedLawsInputDiary ::
  AppealedLawsInputDiary (TWindow * parent,
                          const begNEndNYearTypesAC & const rBNENYT
                         ) :
                bnenyt (rBNENYT), pParent (parent), pQ (NULL)
 {
  MakeQuery () ;
  MakeBookFormatFN () ;
 }  // AppealedLawsInputDiary :: AppealedLawsInputDiary

AppealedLawsInputDiary :: ~AppealedLawsInputDiary ()
 {
  if (pQ)
    delete pQ ;
 }  // AppealedLawsInputDiary :: ~AppealedLawsInputDiary

void AppealedLawsInputDiary :: GetQueryMaxInRegNo ()
 {
  if (! (pQ && bnenyt.types [0]))
    return ;
  pQ -> Restart () ;
  while (pQ -> Read ())
   {
    TInReg r ;

    r << (* pQ) ;
    if (bnenyt.endN < r.key.no)
      bnenyt.endN = r.key.no ;
   }
  pQ -> Restart () ;
 }  // AppealedLawsInputDiary :: GetQueryMaxInRegNo

void AppealedLawsInputDiary ::
  PrintOneItem (TInReg & rInReg, AppealedLawsInputDiaryOutFile & of,
                const int serialNo, TInRegEvent * const pInRegEvent)
 {
  AppealedLawsInRegColumns iRC (formatFN, pParent, rInReg, serialNo,
                                of.minRowsForInreg (), pInRegEvent) ;
  const int rows = iRC.getMaxRows () ;
  int i ;

  of.nextReqToPrint (rows + 1,
                     NULL  // & bnenyt  // 29.sep.2000
                    ) ;
  of.horzLine () ;
  for (i = 0 ; i < rows ; i ++)
    of.writeItem (iRC, true) ;
 }  // AppealedLawsInputDiary :: PrintOneItem

void AppealedLawsInputDiary::PrintAppealedLawsInputDiary()
{
	if (!(pQ && bnenyt.types[0]))
		return;

	AppealedLawsInputDiaryOutFile of(pParent, formatFN);

	{
		TWaitWindow ww(pParent, PLEASE_WAIT);
		TInReg inReg;
		int serialNo = 0;
		bool beginning = true;

		if (bnenyt.endN == 0)
			GetQueryMaxInRegNo();

		pQ->Restart();

		while (pQ->Read())
		{
			serialNo++;

			if (beginning)
			{
				of.title();
				of.formattedHead(&bnenyt);
				of.head();
				beginning = false;
			}

			clearGroupData(inReg);
			inReg << (*pQ);
			PrintOneItem(inReg, of, serialNo);
		}  // -- 3 --

		of.footherAndTail();
	}

	show_text_stream_use_settings(NULL, of, formatFN, FLOAT_ALIGNMENT);
}
