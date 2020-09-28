#include "cac.h"

SQLCriteria::SQLCriteria(const size_t size) :
	critSz(size),
  crit(size ? new char[size] : new char[1]),
  startWithAnd(false), composedOk(false)
{
	crit[0] = '\0';
}  // SQLCriteria::SQLCriteria

bool SQLCriteria::AbstractMethodCalled(const char* methodName)
{
	if(methodName == NULL)
  	methodName = "";
	return error("Abstract method called (%s::%s).",
	typeid(* this).name(), methodName);
}  // SQLCriteria::AbstractMethodCalled

void SQLCriteria::What(ostream& os)
{
	os << SQL_SELECT << " COUNT (*) ";
  AbstractMethodCalled("What");
}  // SQLCriteria::What

void SQLCriteria::From(ostream& os)
{
	os << " FROM " << " T_TYPE ";
  AbstractMethodCalled("From");
}  // SQLCriteria::From

void SQLCriteria::Where(ostream&)
{
}  // SQLCriteria::Where

void SQLCriteria::And(ostream& os)
{
	os << (startWithAnd ? " AND " : " ");
}  // SQLCriteria::And

void SQLCriteria::OrderBy(ostream&)
{
}  // SQLCriteria::OrderBy

bool SQLCriteria::ArgsOk()
{
	return (crit && critSz > 1);
}  // SQLCriteria::ArgsOk

bool SQLCriteria::Compose()
{
  composedOk = false;
  char methodFullName[128];

  ostrstream(methodFullName, sizeof(methodFullName))
		<< typeid(* this).name() << "::Compose" << ends;
	methodFullName [sizeof(methodFullName) - 1] = '\0';    
  if(ArgsOk())
	{
  	{
      ostrstream oss(crit, critSz);

      What(oss);
      From(oss);
      Where(oss);
      OrderBy(oss);
      oss << " ;" << ends;
    }
    crit[critSz - 1] = '\0';
    composedOk = criteria_finished_ok(crit, methodFullName);
  }
  else
    error("%s\n%s", WRONG_ARGS_PASSED_TO, methodFullName);
	if(composedOk == false)
  	crit[0] = '\0';
  return composedOk;
}  // SQLCriteria::Compose

char* SQLCriteria::Criteria()
{
	if(composedOk == false)
  {
  	crit[0] = '\0';
    error("Badly composed criteria accessed from %s.",
          typeid(* this).name());
  }
 	return crit;
}  // SQLCriteria::Criteria

const char* SQLCriteria::CriteriaC()
{
	return Criteria();
}  // SQLCriteria::CriteriaC


ReportOthActsCriteria::ReportOthActsCriteria(const size_t size,
    	const char* const lawsuitTypes, const long int composit,
      const CDate begD, const CDate endD) :
	SQLCriteria(size),
	lawTypes(lawsuitTypes), composition(composit),
	begDate(begD), endDate(endD)
{
}  // ReportOthActsCriteria::ReportOthActsCriteria

bool ReportOthActsCriteria::ArgsOk()
{
	bool res = SQLCriteria::ArgsOk();

  if(res)
    res = (lawTypes && lawTypes[0] &&
    	begDate.Empty() == false && endDate.Empty() == false);
  return res;
}  // ReportOthActsCriteria::ArgsOk

void ReportOthActsCriteria::What(ostream& os)
{
	os << SQL_SELECT;
	os << "L.F_SUBJECT, "
  	"S.F_TYPE, S.F_YEAR, S.F_NO, S.F_KIND, S.F_DATE, S.F_COMPOSITION";
}  // ReportOthActsCriteria::What

void ReportOthActsCriteria::From(ostream& os)
{
	os << " FROM " << "T_LAWSUIT L, T_SESSION S";
}  // ReportOthActsCriteria::From

void ReportOthActsCriteria::Types(ostream& os)
{
	if(ArgsOk())
  {
  	And(os);
		os << "S.F_TYPE IN (" << SQLCharSet(lawTypes) << ")";
  }
}  // ReportOthActsCriteria::LawTypes

void ReportOthActsCriteria::Period(ostream& os)
{
	if(ArgsOk())
  {
  	And(os);
		os << "(S.F_DATE BETWEEN " << begDate.Value() << " AND "
	 << endDate.Value() << ")";
  }
}  // ReportOthActsCriteria::Period

void ReportOthActsCriteria::Composition(ostream& os)
{
	if(composition)
  {
  	And(os);
    os << "S.F_COMPOSITION = " << composition;
  }
}  // ReportOthActsCriteria::Composition

void ReportOthActsCriteria::Results(ostream&)
{
}  // ReportOthActsCriteria::Results

void ReportOthActsCriteria::SessKinds(ostream&)
{
}  // ReportOthActsCriteria::SessKinds

void ReportOthActsCriteria::OtherSessConditions(ostream& os)
{
	if(ArgsOk())
  {
		And(os);
	  os << "S.F_KIND != \'" << KIND_RESOLUTION_SESSION << "\' ";
# if ! INSTANCE
		And(os);
	  os << "S.F_KIND != \'" << KIND_LISTENING_SESSION << "\' ";
# endif  // of ! INSTANCE
		And(os);
	  os << "S.F_RESULT != \'" << RESULT_UNFIXED << "\' ";
//20050322 ->
		And(os);
	  os << "S.F_RESULT != \'" << RESULT_ABORTED << "\' ";
//20050322 <-
# if JUDGE_CHKUP_EXCLUDE_RES_FIXED  // 25.jan.2001 - begin
		And(os);
	  os << "S.F_RESULT != \'" << RESULT_FIXED << "\' ";
# endif  // of JUDGE_CHKUP_EXCLUDE_RES_FIXED  // 25.jan.2001 - end
	}
}  // ReportOthActsCriteria::OtherSessConditions

void ReportOthActsCriteria::Join(ostream& os)
{
	if(ArgsOk())
  {
  	And(os);
    os << "S.F_TYPE = L.F_TYPE AND "
          "S.F_YEAR = L.F_YEAR AND "
          "S.F_NO = L.F_NO";
  }
}  // ReportOthActsCriteria::Join

void ReportOthActsCriteria::OtherLawConditions(ostream&)
{
}  // ReportOthActsCriteria::OtherLawConditions

void ReportOthActsCriteria::OtherActsCondition(ostream& os)
{
	if(ArgsOk())
  {
  	And(os);
    os << "(L.F_FINISHED IS NULL OR "             // check 20060426 AND
	     "(L.F_FINISHED IS NOT NULL AND "
	     "NOT S.F_FINAL = F_FINISHED"
	     ")"
          ")";
  }
}  // ReportOthActsCriteria::OtherActsCondition

void ReportOthActsCriteria::Where(ostream& os)
{
	if(ArgsOk())
  {
  	os << SQL_WHERE;
		Types(os);
    startWithAnd = true;
    SessKinds(os);
    Period(os);
    Composition(os);
    Results(os);
    OtherSessConditions(os);
    Join(os);
    OtherLawConditions(os);
    OtherActsCondition(os);
  }
}  // ReportOthActsCriteria::Where

ReportOthActsOpenSesCriteria::
	ReportOthActsOpenSesCriteria(const size_t size,
  	const char* const lawsuitTypes, const long int composit,
    const CDate begD, const CDate endD) :
    	ReportOthActsCriteria(size, lawsuitTypes, composit, begD, endD)
{
}  // ReportOthActsOpenSesCriteria::ReportOthActsOpenSesCriteria

bool ReportOthActsOpenSesCriteria::ArgsOk()
{
	return ReportOthActsCriteria::ArgsOk();
}  // ReportOthActsOpenSesCriteria::ArgsOk

void ReportOthActsOpenSesCriteria::Results(ostream& os)
{
	if(ArgsOk())
  {
    CharPtr tmpLeft(strlen(RESULT_LEFTS) + strlen(RESULT_STOPPEDS) +
              strlen(RESULT_UNMOVEABLES) + 1);

    ostrstream (tmpLeft, tmpLeft.Size())
      << RESULT_LEFTS << RESULT_STOPPEDS << RESULT_UNMOVEABLES << ends;
    tmpLeft [tmpLeft.Size() - 1] = '\0';
    And(os);
    os << "S.F_RESULT NOT IN (" << SQLCharSet(tmpLeft) << ")";
  }
}  // ReportOthActsOpenSesCriteria::Results

void ReportOthActsOpenSesCriteria::SessKinds(ostream& os)
{
	if(ArgsOk())
  {
    And(os);
    os << "S.F_KIND NOT IN (" << SQLCharSet(KIND_CLOSED_SESSIONS) << ")";
  }
}  // ReportOthActsOpenSesCriteria::SessKinds

void ReportOthActsOpenSesCriteria::Where(ostream& os)
{
	if(ArgsOk())
  {
  	ReportOthActsCriteria::Where(os);
  }
}  // ReportOthActsOpenSesCriteria::

ReportOthActsClosedSesCriteria::
	ReportOthActsClosedSesCriteria(const size_t size,
  	const char* const lawsuitTypes, const long int composit,
    const CDate begD, const CDate endD) :
    	ReportOthActsCriteria(size, lawsuitTypes, composit, begD, endD)
{
}  // ReportOthActsClosedSesCriteria::ReportOthActsClosedSesCriteria

bool ReportOthActsClosedSesCriteria::ArgsOk()
{
	return ReportOthActsCriteria::ArgsOk();
}  // ReportOthActsClosedSesCriteria::ArgsOk

void ReportOthActsClosedSesCriteria::Results(ostream& os)
{
	if(ArgsOk())
  {
    And(os);
    os << "S.F_RESULT NOT IN (" << SQLCharSet(RESULT_LEFTS) << ")";
  }
}  // ReportOthActsClosedSesCriteria::Results

void ReportOthActsClosedSesCriteria::SessKinds(ostream& os)
{
	if(ArgsOk())
  {
    And(os);
    os << "S.F_KIND IN (" << SQLCharSet(KIND_CLOSED_SESSIONS) << ")";
  }
}  // ReportOthActsClosedSesCriteria::SessKinds

void ReportOthActsClosedSesCriteria::OtherSessConditions(ostream& os)
{
	if(ArgsOk())
  {
  	ReportOthActsCriteria::OtherSessConditions(os);
# if INSTANCE
		if(os)
    {
    	// suppressing the warning
    }
# else  // of INSTANCE
		And(os);
    os << "S.F_RESULT != \'" << RESULT_DECREE << "\' ";
# endif  // of INSTANCE
	}
}  // ReportOthActsClosedSesCriteria::OtherSessConditions

void ReportOthActsClosedSesCriteria::Where(ostream& os)
{
	if(ArgsOk())
  {
  	ReportOthActsCriteria::Where(os);
  }
}  // ReportOthActsClosedSesCriteria::Where

ReportOthActsOpenSesCriteria1::
	ReportOthActsOpenSesCriteria1(const char* const lawsuitTypes,
  	const long int composit, const CDate begD, const CDate endD) :
      ReportOthActsOpenSesCriteria(1024, lawsuitTypes, composit, begD,
      	endD)
{
}  // ReportOthActsOpenSesCriteria1::ReportOthActsOpenSesCriteria1

bool ReportOthActsOpenSesCriteria1::ArgsOk()
{
	return ReportOthActsOpenSesCriteria::ArgsOk();
}  // ReportOthActsOpenSesCriteria1::ArgsOk

#if INSTANCE
#  if APPEAL
void ReportOthActsOpenSesCriteria1::OtherLawConditions(ostream&)
{
}  // ReportOthActsOpenSesCriteria1::OtherLawConditions
#  else  // of APPEAL
void ReportOthActsOpenSesCriteria1::OtherLawConditions(ostream& os)
{
	if(ArgsOk())
  {
  	And(os);
    os << "L.F_KIND IN (" << SQLCharSet(KIND_1ST_LAWSUITS) << ")";
  }
}  // ReportOthActsOpenSesCriteria1::OtherLawConditions
#  endif  // of APPEAL
#else  // of INSTANCE
void ReportOthActsOpenSesCriteria1::OtherLawConditions(ostream&)
{
}  // ReportOthActsOpenSesCriteria1::OtherLawConditions
#endif  // of INSTANCE

void ReportOthActsOpenSesCriteria1::Where(ostream& os)
{
	if(ArgsOk())
  {
  	ReportOthActsOpenSesCriteria::Where(os);
  }
}  // ReportOthActsOpenSesCriteria1::Where

ReportOthActsClosedSesCriteria1::
	ReportOthActsClosedSesCriteria1(const char* const lawsuitTypes,
  	const long int composit, const CDate begD, const CDate endD) :
      ReportOthActsClosedSesCriteria(1024, lawsuitTypes, composit, begD,
      	endD)
{
}  // ReportOthActsClosedSesCriteria1::ReportOthActsClosedSesCriteria1

bool ReportOthActsClosedSesCriteria1::ArgsOk()
{
	return ReportOthActsClosedSesCriteria::ArgsOk();
}  // ReportOthActsClosedSesCriteria1::ArgsOk

#if INSTANCE
#  if APPEAL
void ReportOthActsClosedSesCriteria1::OtherLawConditions(ostream&)
{
}  // ReportOthActsClosedSesCriteria1::OtherLawConditions
#  else  // of APPEAL
void ReportOthActsClosedSesCriteria1::OtherLawConditions(ostream& os)
{
	if(ArgsOk())
  {
  	And(os);
    os << "L.F_KIND IN (" << SQLCharSet(KIND_1ST_LAWSUITS) << ")";
  }
}  // ReportOthActsClosedSesCriteria1::OtherLawConditions
#  endif  // of APPEAL
#else  // of INSTANCE
void ReportOthActsClosedSesCriteria1::OtherLawConditions(ostream&)
{
}  // ReportOthActsClosedSesCriteria1::OtherLawConditions
#endif  // of INSTANCE

void ReportOthActsClosedSesCriteria1::Where(ostream& os)
{
	if(ArgsOk())
  {
  	ReportOthActsClosedSesCriteria::Where(os);
  }
}  // ReportOthActsClosedSesCriteria1::Where

#if INSTANCE
ReportOthActsOpenSesCriteria2::
	ReportOthActsOpenSesCriteria2(const char* const lawsuitTypes,
  	const long int composit, const CDate begD, const CDate endD) :
      ReportOthActsOpenSesCriteria(1024, lawsuitTypes, composit, begD,
      	endD)
{
}  // ReportOthActsOpenSesCriteria2::ReportOthActsOpenSesCriteria2

bool ReportOthActsOpenSesCriteria2::ArgsOk()
{
	return ReportOthActsOpenSesCriteria::ArgsOk();
}  // ReportOthActsOpenSesCriteria2::ArgsOk

#  if APPEAL
void ReportOthActsOpenSesCriteria2::OtherLawConditions(ostream&)
{
}  // ReportOthActsOpenSesCriteria2::OtherLawConditions
#  else  // of APPEAL
void ReportOthActsOpenSesCriteria2::OtherLawConditions(ostream& os)
{
	if(ArgsOk())
  {
  	And(os);
    os << "L.F_KIND IN (" << SQLCharSet(KIND_2ND_LAWSUITS) << ")";
  }
}  // ReportOthActsOpenSesCriteria2::OtherLawConditions
#  endif  // of APPEAL

void ReportOthActsOpenSesCriteria2::Where(ostream& os)
{
	if(ArgsOk())
  {
  	ReportOthActsOpenSesCriteria::Where(os);
  }
}  // ReportOthActsOpenSesCriteria2::Where

ReportOthActsClosedSesCriteria2::
	ReportOthActsClosedSesCriteria2(const char* const lawsuitTypes,
  	const long int composit, const CDate begD, const CDate endD) :
      ReportOthActsClosedSesCriteria(1024, lawsuitTypes, composit, begD,
      	endD)
{
}  // ReportOthActsClosedSesCriteria2::ReportOthActsClosedSesCriteria2

bool ReportOthActsClosedSesCriteria2::ArgsOk()
{
	return ReportOthActsClosedSesCriteria::ArgsOk();
}  // ReportOthActsClosedSesCriteria2::ArgsOk

#  if APPEAL
void ReportOthActsClosedSesCriteria2::OtherLawConditions(ostream&)
{
}  // ReportOthActsClosedSesCriteria2::OtherLawConditions
#  else  // of APPEAL
void ReportOthActsClosedSesCriteria2::OtherLawConditions(ostream& os)
{
	if(ArgsOk())
  {
  	And(os);
    os << "L.F_KIND IN (" << SQLCharSet(KIND_2ND_LAWSUITS) << ")";
  }
}  // ReportOthActsClosedSesCriteria2::OtherLawConditions
#  endif  // of APPEAL

void ReportOthActsClosedSesCriteria2::Where(ostream& os)
{
	if(ArgsOk())
  {
  	ReportOthActsClosedSesCriteria::Where(os);
  }
}  // ReportOthActsClosedSesCriteria2::Where
#endif  // of INSTANCE

//
