#include "cac.h"

#if COURT_TYPE == COURT_AREAL

class AdjustablePlainJudgeCheckup : public plainJudgeCheckup
{  // 14.aug.2002
	public:
		AdjustablePlainJudgeCheckup(TWindow* parent,
			const begMYEndMYTypeCIU& rBMYEMYTCIU) ;
		virtual ~AdjustablePlainJudgeCheckup();
		void AdjustJudgesSet(TJudgeArray* pOther);
		void AdjustSubjWithLongArray(
			TISArray<subject_With_LongIntArray>* pOther);
		void AdjustDateWithLongArray(
			TISArray<Date_With_LongIntArrayR>* pOther);
		TJudgeArray* JudgeArray();
		TISArray<subject_With_LongIntArray>* SubjWithLongArray();
		subject_With_LongIntArray* GeneralRow();
		TISArray<Date_With_LongIntArrayR>* DateWithLongArray();
	virtual void PreAdjustmentActions(const CDate& bP,
		const CDate& eP, const size_t histLen,
		TCheckupThread* const thread, TWaitThreadDialog *wtdl);
    virtual void AmongAdjustmentActions(
			TCheckupThread* const thread, TWaitThreadDialog *wtdl);
	virtual void PostAdjustmentActions(
			TCheckupThread* const thread, TWaitThreadDialog *wtdl);
	protected:  // Methods
    void LoadSubjectRanges(TGArray<TSubjectRange>& rangesArr);
    long int FakeSubject(const long int subj,
      const TGArray<TSubjectRange>& rangesArr);
		virtual void FillBunch(TQuery& q, const size_t histLen) = 0;
		void MakeJudgesSet();
		void MakeArr();
		virtual void fillArr(const CDate&, const CDate&, const bool first=true, const bool second=false);
    void IncHistCell(const UcnLng& ul);
    void MakeHistory();
	protected:  // Data
		TIArray<UcnLng>* pBunch;
		subject_With_LongIntArray* pGenRow;
    TISArray<Date_With_LongIntArrayR>* pHistArr;
};  // class AdjustablePlainJudgeCheckup

AdjustablePlainJudgeCheckup::AdjustablePlainJudgeCheckup(TWindow* parent,
	const begMYEndMYTypeCIU& rBMYEMYTCIU):
		plainJudgeCheckup(parent, rBMYEMYTCIU),
		pBunch(NULL), pGenRow(NULL), pHistArr(NULL)
{
  pHistArr = new TISArray<Date_With_LongIntArrayR>;
}  // AdjustablePlainJudgeCheckup::AdjustablePlainJudgeCheckup

AdjustablePlainJudgeCheckup::~AdjustablePlainJudgeCheckup()
{
	if(pBunch)
		delete pBunch;
	if(pGenRow)
		delete pGenRow;
  if(pHistArr)
    delete pHistArr;
}  // AdjustablePlainJudgeCheckup::~AdjustablePlainJudgeCheckup

void AdjustablePlainJudgeCheckup::AdjustJudgesSet(TJudgeArray* pOther)
{
	if(pOther)
	{
		for(int i = 0; i < pOther->Count(); i++)
			if(judgesSet.Contains((*pOther)[i]) == false)
			{
				TJudge* pNewJudge = new JudgeWithNames;  // TJudge;  // 25.nov.2004

				*pNewJudge << (*pOther)[i];
				judgesSet.Add(pNewJudge);
			}
	}
	else
		error("%s\n%s::AdjustJudgesSet", WRONG_ARGS_PASSED_TO,
			typeid(*this).name());
}  // AdjustablePlainJudgeCheckup::AdjustJudgesSet

void AdjustablePlainJudgeCheckup::AdjustSubjWithLongArray(
	TISArray<subject_With_LongIntArray>* pOther)
{
	if(pArr)
	{
		if(pOther)
		{
			for(int i = 0; i < pOther->Count(); i++)
				if(pArr->Contains((*pOther)[i]) == false)
				{
					subject_With_LongIntArray* pSrc = (*pOther)[i];

					if(pSrc)
					{
  					subject_With_LongIntArray* pNewSWLIA =
						  new subject_With_LongIntArray(
		    judgesSet.Count(), pSrc->subj);

  					pArr->Add(pNewSWLIA);
					}
				}
		}
		else
			error("%s\n%s::AdjustSubjWithLongArray", WRONG_ARGS_PASSED_TO,
				typeid(*this).name());
	}
}  // AdjustablePlainJudgeCheckup::AdjustSubjWithLongArray

void AdjustablePlainJudgeCheckup::AdjustDateWithLongArray(
	TISArray<Date_With_LongIntArrayR>* pOther)
{
	if(pHistArr)
	{
		if(pOther)
		{
			for(int i = 0; i < pOther->Count(); i++)
				if(pHistArr->Contains((*pOther)[i]) == false)
				{
					Date_With_LongIntArrayR* pSrc = (*pOther)[i];

					if(pSrc)
					{
  					Date_With_LongIntArrayR* pNewSWLIA =
						  new Date_With_LongIntArrayR(judgesSet.Count());

            pNewSWLIA->d = pSrc->d;
  					pHistArr->Add(pNewSWLIA);
					}
				}
		}
		else
			error("%s\n%s::AdjustDateWithLongArray", WRONG_ARGS_PASSED_TO,
				typeid(*this).name());
	}
}  // AdjustablePlainJudgeCheckup::AdjustDateWithLongArray

TJudgeArray* AdjustablePlainJudgeCheckup::JudgeArray()
{
	return &judgesSet;
} // AdjustablePlainJudgeCheckup::JudgeArray

TISArray<subject_With_LongIntArray>*
	AdjustablePlainJudgeCheckup::SubjWithLongArray()
{  
	if(pArr == NULL)
		error("%s::SubjWithLongArray\n%s", typeid(*this).name(),
			IS_RETURNING_NULL);
	return pArr;
}  // AdjustablePlainJudgeCheckup::SubjWithLongArray()

TISArray<Date_With_LongIntArrayR>*
	AdjustablePlainJudgeCheckup::DateWithLongArray()
{
	if(pHistArr == NULL)
		error("%s::DateWithLongArray\n%s", typeid(*this).name(),
			IS_RETURNING_NULL);
	return pHistArr;
}  // AdjustablePlainJudgeCheckup::DateWithLongArray()

subject_With_LongIntArray* AdjustablePlainJudgeCheckup::GeneralRow()
{
	if(pGenRow == NULL)
		error("%s::GeneralRow\n%s", typeid(*this).name(),
			IS_RETURNING_NULL);
	return pGenRow;
}  // AdjustablePlainJudgeCheckup::GeneralRow

void AdjustablePlainJudgeCheckup::IncHistCell(const UcnLng& ul)
{
  if(ul.d.Empty() == false)
  {
    Date_With_LongIntArrayR key(0);

    key.d = ul.d;

    const int sInd = pHistArr->Find(&key);

    if(sInd != INT_MAX && sInd < pHistArr->Count())
    {
      int jInd = INT_MAX;

      for (int i = 0;
	     jInd == INT_MAX && i < judgesSet.Count();
           i ++)
        if (strcmp (ul.ucn, judgesSet[i]->ucn) == 0)
          jInd = i;
	if(jInd != INT_MAX && jInd < judgesSet.Count())
        (*((*pHistArr)[sInd]))[jInd] += 1;
    }
  }
}  // AdjustablePlainJudgeCheckup::IncHistCell

void load_non_nested_subject_ranges(const char* const types,
	TGArray<TSubjectRange>& rangesArr)
{
  const char* const funcName = "load_non_nested_subject_ranges";

  rangesArr.Flush();
  if(types && types[0])
  {
    {
      char crit[512];

      ostrstream(crit, sizeof(crit))
        << "SELECT F_SUBJECT, F_LOW_SUBJECT_RANGE, F_HIGH_SUBJECT_RANGE "
          ", F_TYPE "  // 01.apr.2003 -- let it be
          "FROM T_SUBJECT_RANGE WHERE "
          "F_TYPE IN (" << SQLCharSet(types) << ") "
          "ORDER BY F_SUBJECT "
          ";" << ends ;
      ENDING_ZERO(crit);
      if(criteria_finished_ok(crit, funcName))
      {
        TQuery q(crit);

        while(q.Read())
        {
          TSubjectRange* pNewSR = new TSubjectRange;

          (*pNewSR) << q;
          rangesArr.Add(pNewSR);
        }
      }
    }
    {  // Exclude the ranges that envelop other ranges
      bool intersection;

      do
      {
        intersection = false;
        for(int i = 0;
	    intersection == false && i < rangesArr.Count();
          i++)
        {
          const TSubjectRange& sR = *(rangesArr[i]);

          for(int j = 0;
		intersection == false && j < rangesArr.Count();
            j++)
          {
            if(i != j)
            {
              const TSubjectRange& test = *(rangesArr[j]);

		  if(sR.InsideRange(test.subject))
		    intersection = true;
            }
	    }
          if(intersection)
            rangesArr.Remove(i);
        }
      }
      while(intersection);
    }
  }
  else
    error("%s\n%s", WRONG_ARGS_PASSED_TO, funcName);
}  // load_non_nested_subject_ranges

void AdjustablePlainJudgeCheckup::
  LoadSubjectRanges(TGArray<TSubjectRange>& rangesArr)
{  // An old variant - in VersionArchives\1537\_appname.j --> _zdravk.cpp
  rangesArr.Flush();
  if(bMYEMYTCIU.types[0]
#if INSTANCE  // 11.oct.2002
      && bMYEMYTCIU.forBoss
#else  // of INSTANCE  // 11.oct.2002
#endif  // of INSTANCE  // 11.oct.2002
    )
    load_non_nested_subject_ranges(bMYEMYTCIU.types, rangesArr);
}  // AdjustablePlainJudgeCheckup::LoadSubjectRanges

long int AdjustablePlainJudgeCheckup::FakeSubject(const long int subj,
  const TGArray<TSubjectRange>& rangesArr)
{
  long int s = subj;
  bool hit = false;

  for(int i = 0; hit == false && i < rangesArr.Count(); i++)
  {
    const TSubjectRange& sR = *(rangesArr[i]);

    if(sR.InsideRange(subj))
    {
      s = sR.subject;
      hit = true;
    }
  }
  return s;
}  // AdjustablePlainJudgeCheckup::FakeSubject

#define GOOSIP_ACTIONS {thread->Tick(false);\
if(wtdl->WantBreak())return;}

void AdjustablePlainJudgeCheckup::PreAdjustmentActions(const CDate& bP,
		const CDate& eP, const size_t histLen,
		TCheckupThread* thread, TWaitThreadDialog *wtdl)
{
	// Look at the definition of GOOSIP_ACTIONS and
	// be attentive with the 'new' and 'delete' operator(s) here!
	if(pBunch == NULL)
	{
		CharPtr crit(2 * 1024);

		if (MakeCriteria (crit, crit.Size(), bP, eP))
		{
			GOOSIP_ACTIONS
			{
				TCountRecsQuery q(crit);
				q.getRecsCount();
				GOOSIP_ACTIONS
				pBunch = new TIArray<UcnLng>;
				FillBunch(q, histLen);
				GOOSIP_ACTIONS
			}
			if (pBunch)
			{
				MakeJudgesSet();
				GOOSIP_ACTIONS
			}
		}
	}
	else
		error("%s\n%s::PreAdjustmentActions", WRONG_ARGS_PASSED_TO,
			typeid(*this).name());
}  // AdjustablePlainJudgeCheckup::PreAdjustmentActions

void AdjustablePlainJudgeCheckup::
	AmongAdjustmentActions(TCheckupThread *thread, TWaitThreadDialog *wtdl)
{
	if(pBunch)
	{
    MakeArr();
    GOOSIP_ACTIONS
    MakeHistory();
    GOOSIP_ACTIONS
	}
}  // AdjustablePlainJudgeCheckup::AmongAdjustmentActions

void AdjustablePlainJudgeCheckup::
	PostAdjustmentActions(TCheckupThread *thread, TWaitThreadDialog *wtdl)
{
	if(pBunch)
	{
		for(int i = 0; i < pBunch->Count(); i++)
    {
	const UcnLng* ul = (*pBunch)[i];

			incCell (ul->lng, ul->ucn) ;
	IncHistCell(*ul);
    }
    GOOSIP_ACTIONS
    pGenRow = new
	subject_With_LongIntArray(judgesSet.Count());
    makeGeneralRow(*pGenRow);
    GOOSIP_ACTIONS
	}
}  // AdjustablePlainJudgeCheckup::PostAdjustmentActions

#undef GOOSIP_ACTIONS

void AdjustablePlainJudgeCheckup::MakeJudgesSet()
{
  if(pBunch)
  {
    const TIArray<UcnLng>& bunch = *pBunch;

    for(int i = 0; i < bunch.Count(); i++)
     {
      bool alreadyExist = false;

      for(int j = 0;
	  alreadyExist == false && j < judgesSet.Count();
        j++)
        if(strcmp(bunch[i]->ucn, judgesSet[j]->ucn) == 0)
          alreadyExist = true;
      if(alreadyExist == false)
      {
        //TJudge judg(bunch[i]->ucn, UCN_CITIZEN_UCN);  // 25.nov.2004
        JudgeWithNames judg(bunch[i]->ucn, UCN_CITIZEN_UCN);  // 25.nov.2004

        if(judg.ucnType)  // successfully loaded
        {
          TJudge* pNewJ = new JudgeWithNames; // TJudge;  // 25.nov.2004

          (*pNewJ) << judg;
          judgesSet.Add(pNewJ);
        }
      }
    }
  }
}  // AdjustablePlainJudgeCheckup::MakeJudgesSet

void AdjustablePlainJudgeCheckup::MakeArr()
{
  if(pBunch && pArr)
  {
    const TIArray<UcnLng>& bunch = *pBunch;

    for(int i = 0; i < bunch.Count(); i++)
    {
      subject_With_LongIntArray tmp(0);

      tmp.subj = bunch[i]->lng ;
      if(pArr->Find(&tmp) == INT_MAX)
          pArr->Add(new
		subject_With_LongIntArray(judgesSet.Count(),
          tmp.subj)) ;
    }
  }
}  // AdjustablePlainJudgeCheckup::MakeArr

void AdjustablePlainJudgeCheckup::MakeHistory()
{
  if(pBunch && pHistArr)
  {
    const TIArray<UcnLng>& bunch = *pBunch;

    for(int i = 0;
	  i < bunch.Count() &&
	    bunch[i]->d.Empty() == false;  // Essential to break growing of
						    // the array
	  i++)
    {
      Date_With_LongIntArrayR tmp(0);

      tmp.d = bunch[i]->d ;
      if(pHistArr->Find(&tmp) == INT_MAX)
      {
        Date_With_LongIntArrayR* pNewDWLIAR =
	    new Date_With_LongIntArrayR(judgesSet.Count());

        pNewDWLIAR->d = tmp.d;
        pHistArr->Add(pNewDWLIAR) ;
      }
    }
  }
}  // AdjustablePlainJudgeCheckup::MakeHistory

void AdjustablePlainJudgeCheckup::fillArr(const CDate&, const CDate&, const bool first, const bool second)
{
	error("%s::fillArr does nothing!", typeid(*this).name());
  if (first) {};
  if (second) {};
}  // AdjustablePlainJudgeCheckup::fillArr

class FirmSurroundsPlainJudgeCheckup : public AdjustablePlainJudgeCheckup
{  // 12.apr.2001
	public :
		FirmSurroundsPlainJudgeCheckup(TWindow * parent,
			const begMYEndMYTypeCIU & rBMYEMYTCIU);
	protected :
		virtual bool MakeCriteria(char * const crit, const size_t critSz,
    const CDate & bP, const CDate & eP);
		virtual void FillBunch(TQuery& q, const size_t histLen);
		virtual bool GuessAboutJudge(const TSurround& surr,
			const TGSArray<BriefSessionData>& briefSessions,
			int& index, const bool errMsg = true);
};  // class FirmSurroundsPlainJudgeCheckup

FirmSurroundsPlainJudgeCheckup::
FirmSurroundsPlainJudgeCheckup(TWindow* parent,
	const begMYEndMYTypeCIU& rBMYEMYTCIU) :
		AdjustablePlainJudgeCheckup(parent, rBMYEMYTCIU)
{
}  // FirmSurroundsPlainJudgeCheckup::FirmSurroundsPlainJudgeCheckup

bool FirmSurroundsPlainJudgeCheckup::MakeCriteria(char* const crit,
	const size_t critSz, const CDate& bP, const CDate& eP)
{
	if (! (bMYEMYTCIU.types [0] && crit && critSz))
		return false;

	ostrstream oss(crit, critSz);

  if(bMYEMYTCIU.composition)  // 25.nov.2004 - begin
  {
    oss <<
        "SELECT "
        "S.F_NO, S.F_YEAR, S.F_TYPE, "  // key fields
        "S.F_KIND, S.F_JUDGE, S.F_DATE, "
        "S.F_LAWSUIT_TYPE, S.F_LAWSUIT_YEAR, S.F_LAWSUIT_NO "
      "FROM "
        "T_SURROUND S, T_LAWSUIT L "
      "WHERE "
        "S.F_DATE BETWEEN " << bP.Value() << " AND " << eP.Value() << " AND "
          "S.F_LAWSUIT_TYPE IN (" << SQLCharSet(bMYEMYTCIU.types) << ") AND "
	    "S.F_KIND IN('" << KIND_CHANGE_SURROUND << "') ";
      oss << "AND "
        "L.F_TYPE = S.F_LAWSUIT_TYPE AND "
        "L.F_YEAR = S.F_LAWSUIT_YEAR AND "
        "L.F_NO = S.F_LAWSUIT_NO AND "
        "L.F_COMPOSITION = " << bMYEMYTCIU.composition << " ";
     oss <<
      "ORDER BY "
        "S.F_DATE DESC";  // To detect the last three days' distribution
    oss << " ;" << ends;
  }
  else
  {  // 25.nov.2004 - end
    oss <<
        "SELECT "
        "F_NO, F_YEAR, F_TYPE, "  // key fields
        "F_KIND, F_JUDGE, F_DATE, "
	  "F_LAWSUIT_TYPE, F_LAWSUIT_YEAR, F_LAWSUIT_NO "
      "FROM "
	  "T_SURROUND "
      "WHERE "
        "F_DATE BETWEEN " << bP.Value() << " AND " << eP.Value() << " AND "
          "F_LAWSUIT_TYPE IN (" << SQLCharSet(bMYEMYTCIU.types) << ") AND "
          "F_KIND IN('" << KIND_CHANGE_SURROUND << "') ";
     oss <<
      "ORDER BY "
        "F_DATE DESC";  // To detect the last three days' distribution
    oss << " ;" << ends;
  }  // 25.nov.2004
	crit[critSz - 1] = '\0';
	return criteria_finished_ok(crit, typeid(*this).name(), "MakeCriteria");
}  // FirmSurroundsPlainJudgeCheckup::MakeCriteria

bool FirmSurroundsPlainJudgeCheckup::GuessAboutJudge(const TSurround& surr,
    	const TGSArray<BriefSessionData>& briefSessions, int& index,
    	const bool errMsg)
{
	const int cnt = briefSessions.Count();
	bool res = true;
	
	index = INT_MAX;
	if(cnt > 1)
	{
		index = cnt - 1;
		bool diff = false;

		for(int i = 1; diff == false && i < cnt; i++)
			diff = strcmp(briefSessions[0]->judge, briefSessions[i]->judge);
		if(diff)
		{
			char surrDesc[96];
			char sessDesc[128];
			const BriefSessionData& bSD = *(briefSessions[index]);

			TRCDKeyContainer_(surr.key, surr.kind).Print(surrDesc, 
				sizeof(surrDesc));
			DatedContainer_(bSD.key.type, bSD.key.no, bSD.key.year,
         	bSD.kind, bSD.date).Print(sessDesc, sizeof(sessDesc));
			info("По\n%s\nима прроведени повече от едно заседание и "
				"с различни съдии-докладчици. Ще бъде използуван "
				"съдията от\n%s.", surrDesc, sessDesc);
		}
	}
	else
		if(cnt == 1)
			index = 0;
		else  // cnt == 0
		{     
			if(errMsg)
				error("%s\n%s::%s", WRONG_ARGS_PASSED_TO, 
						typeid(*this).name(), "GuessAboutJudge");
			res = false;
		}
  return res;
}  // FirmSurroundsPlainJudgeCheckup::GuessAboutJudge

void FirmSurroundsPlainJudgeCheckup::FillBunch(TQuery& q,
  const size_t histLen)
{
#if defined(FIRM_SURROUNDS_USE_SESS_JUDGE)
#  error FIRM_SURROUNDS_USE_SESS_JUDGE must not be defined here!
#endif  // of defined(FIRM_SURROUNDS_USE_SESS_JUDGE)
#define FIRM_SURROUNDS_USE_SESS_JUDGE 0
  if(pBunch)
  {
    TIArray<UcnLng>& bunch = *pBunch;
    TSurround surr;
#if FIRM_SURROUNDS_USE_SESS_JUDGE
    TGSArray<BriefSessionData> briefSessions;
#endif  // of FIRM_SURROUNDS_USE_SESS_JUDGE
    LawPtr pLaw;
    const CDate empty;
    CDate currDate = empty;
    size_t currHistLen = 0;
    TGArray<TSubjectRange> rangesArr;

    LoadSubjectRanges(rangesArr);
    while(q.Read())
    {
      surr << q;
      if(currDate != surr.date)
	{
        currDate = surr.date;
        currHistLen += 1;
      }
      pLaw.Ref().key = surr.lawsuit;
      if(pLaw.Ref().Get())
      {
        const CDate d = currHistLen > histLen ? empty : currDate;

#if FIRM_SURROUNDS_USE_SESS_JUDGE
	  if(surround_to_brief_sessions(surr, briefSessions))
	  {
          int index;

          if(GuessAboutJudge(surr, briefSessions, index))
            bunch.Add(new UcnLng(briefSessions[index]->judge,
              FakeSubject(pLaw.Ref().subject, rangesArr), d));
        }
        else
#endif  // of FIRM_SURROUNDS_USE_SESS_JUDGE
	    bunch.Add(new UcnLng(surr.judge,
            FakeSubject(pLaw.Ref().subject, rangesArr), d));
      }
    }
  }
#undef FIRM_SURROUNDS_USE_SESS_JUDGE
}  // FirmSurroundsPlainJudgeCheckup::FillBunch

class FirmLawsPlainJudgeCheckup : public AdjustablePlainJudgeCheckup
{  // 12.apr.2001
	public :
		FirmLawsPlainJudgeCheckup(TWindow * parent,
			const begMYEndMYTypeCIU & rBMYEMYTCIU);
	protected :
		virtual bool MakeCriteria(char * const crit, const size_t critSz,
	const CDate & bP, const CDate & eP);
		virtual void FillBunch(TQuery& q, const size_t histLen);
};  // class FirmLawsPlainJudgeCheckup

FirmLawsPlainJudgeCheckup::
FirmLawsPlainJudgeCheckup(TWindow* parent,
	const begMYEndMYTypeCIU& rBMYEMYTCIU) :
		AdjustablePlainJudgeCheckup(parent, rBMYEMYTCIU)
{
}  // FirmLawsPlainJudgeCheckup::FirmLawsPlainJudgeCheckup

bool FirmLawsPlainJudgeCheckup::MakeCriteria(char* const crit,
	const size_t critSz, const CDate& bP, const CDate& eP)
{
  if(bMYEMYTCIU.types[0] && crit && critSz)
  {
    ostrstream oss(crit, critSz);
    const bool wantRejudge =
      DISTRIB_COUNT_REJUDGE ?
      true : false;  // 11.dec.2003

    oss <<
      "SELECT F_SUBJECT, ";
    // "F_JUDGE"  // 11.dec.2003
    oss << (wantRejudge ? "F_REJUDGE AS F_JUDGE" : "F_JUDGE");  // 11.dec.2003
    oss <<
	", F_DATE "
      "FROM T_LAWSUIT "
      "WHERE "
        "F_DATE BETWEEN " << bP.Value() <<" AND " << eP.Value() << " AND "  // 15.apr.2003
	  "F_TYPE IN (" << SQLCharSet (bMYEMYTCIU.types) << ")" ;
    if(bMYEMYTCIU.composition)
      oss << " AND F_COMPOSITION = " << bMYEMYTCIU.composition;
    oss << " ORDER BY F_DATE DESC";
    oss << " ;" << ends ;
    crit [critSz - 1] = '\0' ;
    return criteria_finished_ok(crit, typeid (*this).name(),
      "MakeCriteria");
  }
  else
    return false;
}  // FirmLawsPlainJudgeCheckup::MakeCriteria

void FirmLawsPlainJudgeCheckup::FillBunch(TQuery& q, const size_t histLen)
{
  if(pBunch)
  {
    TIArray<UcnLng>& bunch = *pBunch;
    LawPtr pLaw;
    const CDate empty;
    CDate currDate = empty;
    size_t currHistLen = 0;
    TGArray<TSubjectRange> rangesArr;

    LoadSubjectRanges(rangesArr);
    while(q.Read())
    {
      pLaw.Ref() << q;
      if(currDate != pLaw.Ref().date)
	{
        currDate = pLaw.Ref().date;
        currHistLen += 1;
      }
      bunch.Add(new UcnLng(pLaw.Ref().judge,
        FakeSubject(pLaw.Ref().subject, rangesArr),
        currHistLen > histLen ? empty : currDate));
    }
	}
}  // FirmLawsPlainJudgeCheckup::FillBunch

class TZdravkovaDistributionThread : public TCheckupThread
{  // 14.aug.2002
	public:
		TZdravkovaDistributionThread(const begMYEndMYTypeCIU& rBMYEMYTCIU, const CDate &endDay,
	const bool wantText, const bool wantHTML);
		virtual ~TZdravkovaDistributionThread();
    void PrintZdravkovaDistribution();
	protected:  // Methods
		virtual bool ArgsOk();
		virtual void Execute();
		virtual void CreateOutDoc();
		void MakeDescription(CheckupActions action);
    void WriteCheckup(ZdravkovaDistributionOutFile& outF,
	const TJudgeArray& jArr,
	const TISArray<subject_With_LongIntArray>& lawsDataArr,
	const TISArray<subject_With_LongIntArray>& surrsDataArr,
	const subject_With_LongIntArray& lawsGenRow,
	const subject_With_LongIntArray& surrsGenRow);
    void WriteHistory(ZdravkovaDistributionOutFile& outF,
	const TJudgeArray& jArr,
	const TISArray<Date_With_LongIntArrayR>& lawsHistArr,
	const TISArray<Date_With_LongIntArrayR>& surrsHistArr);
    void WriteHTMLCheckup(ZdravkovaDistributionHTMLOutFile& outF,
	const TJudgeArray& jArr,
	const TISArray<subject_With_LongIntArray>& lawsDataArr,
	const TISArray<subject_With_LongIntArray>& surrsDataArr,
	const subject_With_LongIntArray& lawsGenRow,
	const subject_With_LongIntArray& surrsGenRow);
    void WriteHTMLHistory(ZdravkovaDistributionHTMLOutFile& outF,
	const TJudgeArray& jArr,
	const TISArray<Date_With_LongIntArrayR>& lawsHistArr,
	const TISArray<Date_With_LongIntArrayR>& surrsHistArr);
    void CreateHTMLOutDoc();
    void MakeHTMLFormatFN();
	protected:  // Data
		FirmSurroundsPlainJudgeCheckup* pSurrsChkup;
		FirmLawsPlainJudgeCheckup* pLawsChkup;
		const begMYEndMYTypeCIU& bMYEMYTCIU;
		char descrBuff[128];
    char formatFN[128];
    char htmlFormatFN[128];
    const CDate &eP;
    ZdravkovaDistributionOutFile *zdofText;
    ZdravkovaDistributionHTMLOutFile *zdofHtml;
    const bool wantTextOutput;
    const bool wantHTMLOutput;
};

TZdravkovaDistributionThread::TZdravkovaDistributionThread(const begMYEndMYTypeCIU& rBMYEMYTCIU,
  const CDate &endDay, const bool wantText, const bool wantHTML) :
		TCheckupThread(0, 10),
		pSurrsChkup(NULL), pLawsChkup(NULL), bMYEMYTCIU(rBMYEMYTCIU),
    eP(endDay), wantTextOutput(wantText), wantHTMLOutput(wantHTML)
{
  zdofText = zdofHtml = NULL;
  plainJudgeCheckup::makeFmtFN(bMYEMYTCIU, formatFN, sizeof(formatFN));
  MakeHTMLFormatFN();
}

TZdravkovaDistributionThread::~TZdravkovaDistributionThread()
{
	delete pSurrsChkup;
	delete pLawsChkup;
}

bool TZdravkovaDistributionThread::ArgsOk()
{
	return TCheckupThread::ArgsOk();
}

void TZdravkovaDistributionThread::MakeDescription(CheckupActions action)
{
	ostrstream oss (descrBuff, sizeof(descrBuff));

	oss << "Разпределение на Дела и ";
  {
    char cS[2];

    cS[0] = KIND_CHANGE_SURROUND;
    cS[1] = '\0';
    kinds_to_plural (oss, cS, true) ;
  }
  oss << " - ";
	switch(action)
	{
		case GatheringData:
			oss << EXTRACTING_DATA;
			break;
		case MakingGeneralRow:
			oss << MAKING_GENERAL_ROW;
			break;
		case CreatingOutDoc:
			oss << CREATING_OUT_DOC;
			break;
	}
	oss << ends;
	descrBuff[sizeof(descrBuff) - 1] = '\0';
}

void
TZdravkovaDistributionThread::WriteCheckup(ZdravkovaDistributionOutFile& outF,
  const TJudgeArray& jArr,
  const TISArray<subject_With_LongIntArray>& lawsDataArr,
  const TISArray<subject_With_LongIntArray>& surrsDataArr,
  const subject_With_LongIntArray& lawsGenRow,
  const subject_With_LongIntArray& surrsGenRow)
{
  outF.WriteHeader(jArr);
  {
    CharPtr subjShortNameHolder(SIZE_OF_SUBJECT_NAME);

    for(int i = 0;
	  i < surrsDataArr.Count() &&
	    i < lawsDataArr.Count();
        i++)
    {
      const subject_With_LongIntArray& primary = *(lawsDataArr[i]);
      const subject_With_LongIntArray& changes = *(surrsDataArr[i]);
	bool atTheTop = false;

      subjShortNameHolder[0] = '\0';
      if(outF.pageLinesLeft() <
          outF.WritePlainItem(primary, changes, bMYEMYTCIU.types, false,
          true, NULL, false, &subjShortNameHolder) + 1)
      {
        outF.footherLine();
        outF.newPage();
        atTheTop = true;
      }
      outF.WritePlainItem(primary, changes, bMYEMYTCIU.types, atTheTop,
	  true, NULL, true, &subjShortNameHolder);
    }
  }
  {
    bool atTheTop = false;

    if(outF.pageLinesLeft() <
        outF.WritePlainItem(lawsGenRow, surrsGenRow,
          bMYEMYTCIU.types , false, true, NULL, false) + 1)
    {
      outF.footherLine();
      outF.newPage();
      atTheTop = true;
    }
    outF.WritePlainItem(lawsGenRow, surrsGenRow, bMYEMYTCIU.types,
      atTheTop);
  }
  outF.footherLine();
}

void TZdravkovaDistributionThread::
WriteHistory(ZdravkovaDistributionOutFile& outF,
  const TJudgeArray& jArr,
  const TISArray<Date_With_LongIntArrayR>& lawsHistArr,
  const TISArray<Date_With_LongIntArrayR>& surrsHistArr)
{
  const int surrsHistIfems = surrsHistArr.Count();
  const int lawsHistIfems = lawsHistArr.Count();

  if(surrsHistIfems > 0 && lawsHistIfems > 0)
  {
    if(section_says_yes(formatFN, "***_HISTORY_AT_NEW_PAGE"))
      outF.newPage();
    if(outF.pageLinesLeft() <
      outF.WriteHeader(jArr, false, false) +
      outF.WritePlainItem(*(lawsHistArr[0]), *(surrsHistArr[0]),
        bMYEMYTCIU.types, false, false, &((lawsHistArr[0])->d), false) +
      1 + 1)
    {
      outF.footherLine();
      outF.newPage();
    }
    outF.newLine();
		outF.WriteHeader(jArr, false);
    for(int i = 0; i < surrsHistIfems && i < lawsHistIfems; i++)
    {
	const Date_With_LongIntArrayR& primary = *(lawsHistArr[i]);
      const Date_With_LongIntArrayR& changes = *(surrsHistArr[i]);

      if(primary.d == changes.d)
      {
	  bool atTheTop = false;

        if(outF.pageLinesLeft() <
            outF.WritePlainItem(primary, changes, bMYEMYTCIU.types, false,
              false, &(primary.d), false) + 1)
        {
          outF.footherLine();
          outF.newPage();
          atTheTop = true;
        }
        outF.WritePlainItem(primary, changes, bMYEMYTCIU.types, atTheTop,
          false, &(primary.d));
      }
      else
        error("%s\n%s::WriteHistory",
          UNDAJUSTED_ARRAYS_PASSED_TO, typeid(*this).name());
    }
    outF.footherLine();
  }
}

void TZdravkovaDistributionThread::
WriteHTMLCheckup(ZdravkovaDistributionHTMLOutFile& outF,
  const TJudgeArray& jArr,
  const TISArray<subject_With_LongIntArray>& lawsDataArr,
  const TISArray<subject_With_LongIntArray>& surrsDataArr,
  const subject_With_LongIntArray& lawsGenRow,
  const subject_With_LongIntArray& surrsGenRow)
{
  char tableBgcolor[16];
  char tableWidth[16];

  get_section_string (htmlFormatFN, "***_CHECK_UP_TABLE_BGCOLOR", tableBgcolor, sizeof(tableBgcolor));
  get_section_string (htmlFormatFN, "***_TABLE_WIDTH", tableWidth, sizeof(tableWidth));

  {
    HTML_table table(outF, true, tableBgcolor, 0, 1, tableWidth);

    outF.WriteHeader(jArr);
    for(int i = 0;
	  i < surrsDataArr.Count() &&
	    i < lawsDataArr.Count();
        i++)
    {
	const subject_With_LongIntArray& primary = *(lawsDataArr[i]);
      const subject_With_LongIntArray& changes = *(surrsDataArr[i]);

      outF.WritePlainItem(jArr, primary, changes, bMYEMYTCIU.types);
    }
    outF.WritePlainItem(jArr, lawsGenRow, surrsGenRow, bMYEMYTCIU.types);
  }
}

void TZdravkovaDistributionThread::
WriteHTMLHistory(ZdravkovaDistributionHTMLOutFile& outF,
  const TJudgeArray& jArr,
  const TISArray<Date_With_LongIntArrayR>& lawsHistArr,
  const TISArray<Date_With_LongIntArrayR>& surrsHistArr)
{
  const int surrsHistIfems = surrsHistArr.Count();
  const int lawsHistIfems = lawsHistArr.Count();

  if(surrsHistIfems > 0 && lawsHistIfems > 0)
  {
    char tableBgcolor[16];
    char tableWidth[16];

    get_section_string (htmlFormatFN, "***_HISTORY_TABLE_BGCOLOR", tableBgcolor, sizeof(tableBgcolor));
    get_section_string (htmlFormatFN, "***_TABLE_WIDTH", tableWidth, sizeof(tableWidth));

    {
      html_br(outF);
      HTML_table table(outF, true, tableBgcolor, 0, 1, tableWidth);

      outF.WriteHeader(jArr, false);
      for(int i = 0; i < surrsHistIfems && i < lawsHistIfems; i++)
      {
        const Date_With_LongIntArrayR& primary = *(lawsHistArr[i]);
        const Date_With_LongIntArrayR& changes = *(surrsHistArr[i]);

        if(primary.d == changes.d)
        {
          outF.WritePlainItem(jArr, primary, changes, bMYEMYTCIU.types,
		false, &(primary.d));
        }
        else
	    error("%s\n%s::WriteHTMLHistory",
            UNDAJUSTED_ARRAYS_PASSED_TO, typeid(*this).name());
	}
    }
  }
}

void TZdravkovaDistributionThread::CreateOutDoc()
{
	if (!(pSurrsChkup && pLawsChkup))
		return;

	const TJudgeArray* const pJArr = pLawsChkup->JudgeArray();
	const TISArray<subject_With_LongIntArray> *const pSurrsDataArr = pSurrsChkup->SubjWithLongArray();
	const subject_With_LongIntArray *const pSurrsGenRow = pSurrsChkup->GeneralRow();
	const TISArray<Date_With_LongIntArrayR> *const pSurrsHistArr = pSurrsChkup->DateWithLongArray();
	const TISArray<subject_With_LongIntArray> *const pLawsDataArr = pLawsChkup->SubjWithLongArray();
	const subject_With_LongIntArray *const pLawsGenRow = pLawsChkup->GeneralRow();
	const TISArray<Date_With_LongIntArrayR> *const pLawsHistArr = pLawsChkup->DateWithLongArray();

	if (pJArr && pLawsDataArr && pLawsGenRow && pLawsHistArr && pSurrsDataArr && pSurrsGenRow && pSurrsHistArr)
	{
		zdofText = new ZdravkovaDistributionOutFile(wtdl->Parent, formatFN, pJArr->Count());
		ZdravkovaDistributionOutFile &outF = *zdofText;

		outF.title();
		outF.formattedHead(&bMYEMYTCIU);
		WriteCheckup(outF, *pJArr, *pLawsDataArr, *pSurrsDataArr, *pLawsGenRow, *pSurrsGenRow);
		Tick(false);

		if (wtdl->WantBreak() == false)
			WriteHistory(outF, *pJArr, *pLawsHistArr, *pSurrsHistArr);

		outF.tail();
    }
}

void TZdravkovaDistributionThread::CreateHTMLOutDoc()
{
	if (!(pSurrsChkup && pLawsChkup))
		return;

	const TJudgeArray* const pJArr = pLawsChkup->JudgeArray();
	const TISArray<subject_With_LongIntArray>* const pSurrsDataArr = pSurrsChkup->SubjWithLongArray();
	const subject_With_LongIntArray* const pSurrsGenRow = pSurrsChkup->GeneralRow();
	const TISArray<Date_With_LongIntArrayR>* const pSurrsHistArr = pSurrsChkup->DateWithLongArray();
	const TISArray<subject_With_LongIntArray>* const pLawsDataArr = pLawsChkup->SubjWithLongArray();
	const subject_With_LongIntArray* const pLawsGenRow = pLawsChkup->GeneralRow();
	const TISArray<Date_With_LongIntArrayR>* const pLawsHistArr = pLawsChkup->DateWithLongArray();

	if (pJArr && pLawsDataArr && pLawsGenRow && pLawsHistArr && pSurrsDataArr && pSurrsGenRow && pSurrsHistArr)
	{
		zdofHtml = new ZdravkovaDistributionHTMLOutFile(wtdl->Parent, htmlFormatFN, pJArr->Count());
		ZdravkovaDistributionHTMLOutFile &outF = *zdofHtml;
		HTML_html html(outF, false);

		{
			HTML_head head(outF);
			outF.head();
			HTML_title title(outF);
			outF.title();
		}

		{
			char bodyBgColor[32];
			char bodyFontFace[64];
			char bodyFontColor[16];
			char bodyFontSize[16];

			get_section_string(htmlFormatFN, "***_BODY_BGCOLOR", bodyBgColor, sizeof bodyBgColor);
			get_section_string(htmlFormatFN, "***_BODY_FONT_FACE", bodyFontFace, sizeof bodyFontFace);
			get_section_string(htmlFormatFN, "***_BODY_FONT_COLOR", bodyFontColor, sizeof bodyFontColor);
			get_section_string(htmlFormatFN, "***_BODY_FONT_SIZE", bodyFontSize, sizeof bodyFontSize);

			HTML_body body(outF, bodyBgColor);
			HTML_font *pHTMLBodyFont = (bodyFontFace[0] || bodyFontColor[0] || bodyFontSize[0]) ?
				new HTML_font(outF, bodyFontFace, bodyFontColor, bodyFontSize) : NULL;

			{
				HTML_center* pHTMLCenter = NULL;

				if(section_says_yes(htmlFormatFN, "***_CENTER"))
					pHTMLCenter = new HTML_center(outF);

				outF << endl;
				outF.formattedHead(&bMYEMYTCIU);
				WriteHTMLCheckup(outF, *pJArr, *pLawsDataArr, *pSurrsDataArr, *pLawsGenRow,
					*pSurrsGenRow);
				Tick(false);

				if (wtdl->WantBreak() == false)
				{
					WriteHTMLHistory(outF, *pJArr, *pLawsHistArr, *pSurrsHistArr);
				}

				if (pHTMLCenter)
					delete pHTMLCenter;
			}

			if (pHTMLBodyFont)
				delete pHTMLBodyFont;
		}
	}
}

void TZdravkovaDistributionThread::MakeHTMLFormatFN()
{
  htmlFormatFN[0] = '\0';
  const int ind = 9;
  const size_t l = strlen(formatFN);

  if(ind < l && l < sizeof(htmlFormatFN))
  {  // text\judgede.fma  -->  text\judgede.fma
    strcpy(htmlFormatFN, formatFN);
    htmlFormatFN[ind] = 'h';
  }
  else
		error("%s\n%s::MakeHTMLFormatFN", WRONG_ARGS_PASSED_TO,
			typeid(*this).name());
}

void TZdravkovaDistributionThread::Execute()
{
  int histLen =
    get_section_int(formatFN, "***_HISTORY_LEN", 4);
	CDate bP;

  if(histLen < 0)
    histLen = 0;
	bP.month = bMYEMYTCIU.begMonth;
	bP.year = bMYEMYTCIU.begYear;
	bP = firstDayOfThisMonth(bP);
	pSurrsChkup = new FirmSurroundsPlainJudgeCheckup(wtdl->Parent, bMYEMYTCIU);
	pLawsChkup = new FirmLawsPlainJudgeCheckup(wtdl->Parent, bMYEMYTCIU);
	try
	{  // -- 0 --
		MakeDescription(GatheringData);
		wtdl->SetHeader(descrBuff);
		wtdl->PleaseWait();
		totalPartSteps = 2 * (3 + 2 + 2);             // Gossip service
		// 3 --> pre-adjustment, 1 --> among-adjustment,
    // 2 --> post-adjustment
		totalPartSteps += (1 + 1);                    // Gossip service
    // 1 --> adjustment - 1st step, 1 --> adjustment - 2nd step
		endingVal = barHiVal - createOutDocSteps;     // Gossip service
    reportFinishedItems = false;                  // Gossip service
    passeds = "Стъпка";                           // Gossip service
		pSurrsChkup->PreAdjustmentActions(bP, eP, histLen, this, wtdl);
		if(wtdl->WantBreak() == false)
			pLawsChkup->PreAdjustmentActions(bP, eP, histLen, this, wtdl);
		if(wtdl->WantBreak() == false)
		{
			pSurrsChkup->AdjustJudgesSet(pLawsChkup->JudgeArray());
			pLawsChkup->AdjustJudgesSet(pSurrsChkup->JudgeArray());
			Tick(false);
		}
		if(wtdl->WantBreak() == false)
			pSurrsChkup->AmongAdjustmentActions(this, wtdl);
		if(wtdl->WantBreak() == false)
			pLawsChkup->AmongAdjustmentActions(this, wtdl);
		if(wtdl->WantBreak() == false)
		{
			pSurrsChkup->AdjustSubjWithLongArray(
				pLawsChkup->SubjWithLongArray());
			pLawsChkup->AdjustSubjWithLongArray(
				pSurrsChkup->SubjWithLongArray());
			pSurrsChkup->AdjustDateWithLongArray(
				pLawsChkup->DateWithLongArray());
			pLawsChkup->AdjustDateWithLongArray(
				pSurrsChkup->DateWithLongArray());
			Tick(false);
		}
		if(wtdl->WantBreak() == false)
			pSurrsChkup->PostAdjustmentActions(this, wtdl);
		if(wtdl->WantBreak() == false)
			pLawsChkup->PostAdjustmentActions(this, wtdl);
		if(wtdl->WantBreak() == false)
			Tick(true);
		if(wtdl->WantBreak() == false)
		{
			if(endingVal < barHiVal)                    // Gossip service
				endingVal = barHiVal;                     // Gossip service
			currentPartStep = 0;                        // Gossip service
		MakeDescription(CreatingOutDoc);            // Gossip service
		totalPartSteps = 0;                         // Gossip service
	if(wantTextOutput)
	  totalPartSteps += 2;
	if(wantHTMLOutput)
	  totalPartSteps += 2;
	if(wantTextOutput)
			  CreateOutDoc();
	if(wtdl->WantBreak() == false)
	  if(wantHTMLOutput)
	    CreateHTMLOutDoc();
		}
		if(wtdl->WantBreak() == false)
			Tick(true);
		wtdl->AlmostDone();
	}  // -- 0 --
	CATCH_ANYTHING
}

void TZdravkovaDistributionThread::PrintZdravkovaDistribution()
{
	Generate();

	if (zdofText)
	{
		show_text_stream_use_settings(NULL, *zdofText, formatFN, FLOAT_ALIGNMENT);
		delete zdofText;
	}

	if (zdofHtml)
	{
		show_html_stream(NULL, *zdofHtml);	// 2015:261 LPR: was wtdl->Parent
		delete zdofHtml;
	}
}

#include "_zdrdsd.h"	// 2010:090 former _Dlgs.h
#include "_zdrdsd.cpp"	// 2010:090 former _Dialogs.cpp

void print_zdravkova_distribution(TWindow* parent, begMYEndMYTypeCIU& rBMYEMYTCIU)
{
  CDate lastDate = Today;
  bool text = true;
  bool html = true;
  bool confirm;
  bool goodData;

  rBMYEMYTCIU.forBoss = true;
  do
  {
    goodData = true;

    confirm = TZdravkovaDistributionDialog(parent, NULL, NULL, NULL, NULL,
	NULL, text, html, lastDate, rBMYEMYTCIU.forBoss).Execute() == IDOK;
    if(confirm)
    {
	if(text == false && html == false)
	  goodData = error(NO_OUTPUT_CHOSEN);
	else
	{
	  if(lastDate < rBMYEMYTCIU.BegDate())
	  {
	    char tmp[32];

	    CDateToString(rBMYEMYTCIU.BegDate(), tmp, sizeof(tmp));
	    goodData = error("\"До дата\" е по-малка от %s.", tmp);
	  }
	}
    }
  }
  while(confirm == true && goodData == false);
  if(confirm && goodData)
  {
    rBMYEMYTCIU.endDay = lastDate.day;
    rBMYEMYTCIU.endMonth = lastDate.month;
    rBMYEMYTCIU.endYear = lastDate.year;
    TWaitThreadDialog wtdl;
    TZdravkovaDistributionThread(rBMYEMYTCIU, lastDate, text, html).PrintZdravkovaDistribution();
  }
}  // print_zdravkova_distribution

#endif  // AREAL
