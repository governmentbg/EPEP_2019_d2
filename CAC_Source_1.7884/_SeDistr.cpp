#include "cac.h"

// ---------------- SessDistributionColumns ------------------

SessDistributionColumns::SessDistributionColumns(const char * formatFN,
	TWindow * parent, const SessJudgeDistrData& data, const int serialNo,
	TGSArray<JudgeWithNames>& jNames, const int minRows) :
		columnsArr(formatFN, parent, i_columns,
			1 + OFFSET_SESS_DISTRIBUTION_CIPHER)
{
	MakeColumns(data, serialNo, jNames);
	defineMaxRows();
	if(maxRows < minRows)
		maxRows = minRows;
}  // SessDistributionColumns::SessDistributionColumns

void SessDistributionColumns::MakeColumns(const SessJudgeDistrData& data,
	const int serialNo, TGSArray<JudgeWithNames>& jNames)
{
	column * pColumn;

	if((pColumn = columns[iC_SerialNo]) != NULL)
	{  // -- iC_SerialNo --
		addLongToColumn(pColumn, serialNo);
	}  // -- iC_SerialNo --
	if((pColumn = columns[iC_Date]) != NULL)
	{  // -- iC_Date --
		add_date_to_column (pColumn, data.date) ;
	}  // -- iC_Date --
	if((pColumn = columns[iC_Instance]) != NULL)
	{  // -- iC_Instance --
		const char* txt = "";

		switch(data.instance)
		{
			case 1:
				txt = "I";
				break;
			case 2:
				txt = "II";
				break;
			case 3:
				txt = "К";
				break;
		}
		pColumn->addText(txt);
	}  // -- iC_Instance --
	if((pColumn = columns[iC_PartOfDay]) != NULL)
	{  // -- iC_PartOfDay --
		const char* txt = "";


		switch(data.partOfDay)
		{
			case Morning:
				txt = "Преди обяд";
				break;
			case Afternoon:
				txt = "След обяд";
				break;
		}
		pColumn->addText(txt);
	}  // -- iC_PartOfDay --
	if((pColumn = columns[iC_Judge]) != NULL)
	{  // -- iC_Judge --
		if(ucnPresent(data.judgeUcn))
		{
			char txt[SIZE_OF_NAME_FOR_BOOK];
			JudgeWithNames jn;  // I'm using specially this constructor,
			// because I don't want to load data right now.

			txt[0] = '\0';
			jn.ucnType = UCN_CITIZEN_UCN;
			ostrstream(jn.ucn, sizeof(jn.ucn)) << data.judgeUcn << ends;
			ENDING_ZERO(jn.ucn);

			const int index = jNames.Find(&jn);  // With only 2 fields filled !

			if(0 <= index && index < jNames.Count())
			{  // Get data from the array.
				ostrstream(txt, sizeof(txt)) << jNames[index]->names << ends;
				ENDING_ZERO(txt);
			}
			else
			{  // Get data from the database.
				if(jn.FillNames())
				{
					ostrstream(txt, sizeof(txt)) << jn.names << ends;
					ENDING_ZERO(txt);
					{  // Add data to the array.
						JudgeWithNames* pNewItem = new JudgeWithNames;

						pNewItem->ucnType = jn.ucnType;
						ostrstream(pNewItem->ucn, sizeof(pNewItem->ucn))
							<< jn.ucn << ends;
						ENDING_ZERO(pNewItem->ucn);
						ostrstream(pNewItem->names, sizeof(pNewItem->names))
							<< txt << ends;  // Doesn't participate in comparison
						ENDING_ZERO(pNewItem->names);
						jNames.Add(pNewItem);
					}
				}
			}
			if(txt[0])
				pColumn->addText(txt);
		}
	}  // -- iC_Judge --
	if((pColumn = columns[iC_NumberOfSessions]) != NULL)
	{  // -- iC_NumberOfSessions --
		addLongToColumn(pColumn, data.numberOfSessions);
	}  // -- iC_NumberOfSessions --
	if((pColumn = columns[iC_Secretary]) != NULL)
	{  // -- iC_Secretary --
		if(ucnPresent(data.secretaryUcn))
		{
			char txt[SIZE_OF_NAME_FOR_BOOK];

			if(citizenUCNAndUCNTypeToName(data.secretaryUcn, UCN_CITIZEN_UCN,
					txt, sizeof(txt), false, true, false))
				pColumn->addText(txt);
		}
	}  // -- iC_Secretary --
}  //  SessDistributionColumns::MakeColumns

// ---------------- SessJudgeDistrData ------------------

SessJudgeDistrData::SessJudgeDistrData() : HeapingData()
{
	Clear();
}  // SessJudgeDistrData

void SessJudgeDistrData::Clear()
{
	HeapingData::Clear();
  clear_CDate(date);
  instance = 0;
  partOfDay = WholeDay;
  judgeUcn[0] = '\0';
	judgePost = '\0';
	judgeInitials[0] = '\0';
	numberOfSessions = 0;
  secretaryUcn[0] = '\0';
}  // SessJudgeDistrData::Clear

void SessJudgeDistrData::Copy(const HeapingData& other)
{
	{
		const SessJudgeDistrData* pO =
    	dynamic_cast<const SessJudgeDistrData*>(&other);
    
    if(pO)
    {
      date = pO->date;
      instance = pO->instance;
      partOfDay = pO->partOfDay;
      memmove(judgeUcn, pO->judgeUcn, sizeof(judgeUcn));
      judgeUcn[sizeof(judgeUcn) - 1] = '\0';
      judgePost = pO->judgePost;
      memmove(judgeInitials, pO->judgeInitials, sizeof(judgeInitials));
      judgeInitials[sizeof(judgeInitials) - 1] = '\0';
      numberOfSessions = pO->numberOfSessions;
      memmove(secretaryUcn, pO->secretaryUcn, sizeof(secretaryUcn));
      secretaryUcn[sizeof(secretaryUcn) - 1] = '\0';
		}
    else
			error("%s %s::Copy", IMPROPER_USE_OF, typeid(*this).name());
  }
}  // SessJudgeDistrData::Copy

bool SessJudgeDistrData::LessThan(const HeapingData& other) const
{
	bool res = false;
  const SessJudgeDistrData* pO =
  	dynamic_cast<const SessJudgeDistrData*>(&other);

  if(pO)
  {
    if(date < pO->date)
      res = true;
    else
      if(date > pO->date)
        res = false;
      else
        if(instance < pO->instance)
          res = true;
        else
          if(instance > pO->instance)
            res = false;
          else
            if(partOfDay < pO->partOfDay)
              res = true;
            else
              if(partOfDay > pO->partOfDay)
                res = false;
              else
                if(judgePost < pO->judgePost)
                  res = true;
                else
                  if(judgePost > pO->judgePost)
                    res = false;
                  else
										if(strcmp(judgeInitials, pO->judgeInitials) < 0)
											res = true;
										else
											if(strcmp(judgeInitials, pO->judgeInitials) > 0)
												res = false;
											else
												if(strcmp(judgeUcn, pO->judgeUcn) < 0)
													res = true;
												else
													res = false;
	}
	else
		error("%s %s::LessThan", IMPROPER_USE_OF, typeid(*this).name());
	return res;
}  // SessJudgeDistrData::LessThan

bool SessJudgeDistrData::Equal(const HeapingData& other) const
{
	bool res = false;
	const SessJudgeDistrData* pO =
		dynamic_cast<const SessJudgeDistrData*>(&other);

	if(pO)
	{
		res = (date == pO->date && instance == pO->instance &&
			partOfDay == pO->partOfDay && judgePost == pO->judgePost &&
			strcmp(judgeInitials, pO->judgeInitials) == 0 &&
			strcmp(judgeUcn, pO->judgeUcn) == 0);
	}
	else
		error("%s %s::Equal", IMPROPER_USE_OF, typeid(*this).name());
	return res;
}  // SessJudgeDistrData::Equal

HeapingData& SessJudgeDistrData::Accumulate(const HeapingData& other)
{
  const SessJudgeDistrData* pO =
  	dynamic_cast<const SessJudgeDistrData*>(&other);
  bool proper = false;

  if(pO)
  {
    if(Equal(other))
    {
      numberOfSessions += pO->numberOfSessions;
      proper = true;
    }
  }
  if(proper == false)
		error("%s %s::Accumulate", IMPROPER_USE_OF, typeid(*this).name());
	return (*this);
}  // SessJudgeDistrData::Accumulate

// ---------------- SessJudgeDistrArray ------------------

HeapingData* SessJudgeDistrArray::NewItem()
{
	return new SessJudgeDistrData ;
}  // SessJudgeDistrArray::NewItem

SessJudgeDistrData* SessJudgeDistrArray::operator [] (int index)
{
	SessJudgeDistrData* result = NULL;

  if(index >= 0 && index < Count())
		result = dynamic_cast<SessJudgeDistrData*>
			(HeapingDataArray::operator[](index));
  if(result == NULL)
  	error("%s::operator[] is returning NULL", typeid(*this).name());
	return result;
}  // SessJudgeDistrArray::operator []

// ---------------- JudgeWithNames ------------------

//
// Attention! There are three constructors!
//

JudgeWithNames::JudgeWithNames() : TJudge()
{
  AddFields();
	names[0] = name[0] = family[0] = '\0';
  namesFilled = false;                                      // 25.nov.2004
}  // JudgeWithNames::JudgeWithNames

JudgeWithNames::JudgeWithNames(const char* const anUCN,
	const char anUCNType) :
		TJudge(anUCN, anUCNType)
{
  AddFields();
	names[0] = name[0] = family[0] = '\0';
  namesFilled = false;                                      // 25.nov.2004
  FillNames();                                              // 25.nov.2004
}  // JudgeWithNames::JudgeWithNames

JudgeWithNames::JudgeWithNames(const bool fillNames,
  const char* const anUCN, const char anUCNType):  // 25.nov.2004
	TJudge(anUCN, anUCNType)
{
  AddFields();
	names[0] = '\0';
  name[0] = family[0] = '\0';
  namesFilled = false;
  if(fillNames)
  {
    FillNames();
  }
}

void JudgeWithNames::AddFields()                     // 25.nov.2004
{
	Add(
		new TString("NAMES", names, sizeof(names)),
		new TString("NAME", name, sizeof(name)),
		new TString("FAMILY", family, sizeof(family)),
		NULL
	);
}

bool JudgeWithNames::FillNames()
{
// An old varianth - in VersionArchives\2125\Official\_appname.j-->_SeDistr.cpp
  bool res = false;

  SUPPRESS_UNUSED(res);
  if(namesFilled)
  {
    res = (names[0] != '\0' || family[0] != '\0');
  }
  else
  {
    namesFilled = true;
    res = nameFamily(name, sizeof(name), family, sizeof(family));
    if(res)
    {
      ostrstream oss(names, sizeof(names));

      if(name[0])
        oss << name[0] << ". ";
      if(family[0])
        oss << family;
      oss << ends;
    }
    else
    {
      names[0] = name[0] = family[0] = '\0';
    }
    ENDING_ZERO(names);
  }
	return res;
}  // JudgeWithNames::FillNames

bool JudgeWithNames::operator==(const TJudge& other) const           // 25.nov.2004
{
  const JudgeWithNames* pOth = dynamic_cast<const JudgeWithNames*>(&other);

  if(pOth)
  {
    //FillNames();
    //pOth->FillNames();
    return (strcmp(name, pOth->name) == 0 &&
	strcmp(family, pOth->family) == 0 &&
	TJudge::operator==(other));
  }
  else
  {
    return TJudge::operator==(other);
  }
}

bool JudgeWithNames::operator<(const TJudge& other) const            // 25.nov.2004
{
  const bool thisInter = strcmp(ucn, INTER) == 0 ;
  const bool otherInter = strcmp(other.ucn, INTER) == 0 ;

  if(thisInter == otherInter)
  {
    const JudgeWithNames* pOth = dynamic_cast<const JudgeWithNames*>(&other);

    if(pOth)
    {
	//FillNames();
	//pOth->FillNames();
	{
	  const int nameCmpRes = strcmp(name, pOth->name);

	  if(nameCmpRes < 0)
	    return true;
	  else
	    if(nameCmpRes > 0)
		return false;
	    else
	    {
		const int familyCmpRes = strcmp(family, pOth->family);

		if(familyCmpRes < 0)
		  return true;
		else
		  if(familyCmpRes > 0)
		    return false;
		  else
		    return TJudge::operator<(other);
	    }
	}
    }
    else
    {
	return TJudge::operator==(other);
    }
  }
  else
    return (thisInter == false && otherInter == true);  // 14.aug.2002 - end
}

// ---------------- SessDistributionCriteria ------------------

SessDistributionCriteria::SessDistributionCriteria(const size_t size,
	const char* const lawsuitTypes, const CDate begD, const CDate endD,
	const long int composit,
	const char* const fmtFN, TWindow* parent) :
		SQLCriteria(size),
		lawTypes(lawsuitTypes), begDate(begD), endDate(endD),
		composition(composit), formatFN(fmtFN), pParent(parent)
{
}  // SessDistributionCriteria::SessDistributionCriteria

void SessDistributionCriteria::What(ostream& os)
{
	os << SQL_SELECT;
	os << "S.F_TYPE, S.F_YEAR, S.F_NO, S.F_KIND, S.F_DATE, "
		"S.F_COMPOSITION, S.F_JUDGE, S.F_TIME, L.F_KIND AS LAW_KIND";
}  // SessDistributionCriteria::What

void SessDistributionCriteria::From(ostream& os)
{
	os << " FROM " << "T_SESSION S, T_LAWSUIT L";
}  // SessDistributionCriteria::From

bool SessDistributionCriteria::ArgsOk()
{
	bool res = SQLCriteria::ArgsOk();

  if(res)
    res = (lawTypes && lawTypes[0] &&
    	begDate.Empty() == false && endDate.Empty() == false);
  return res;
}  // SessDistributionCriteria::ArgsOk

void SessDistributionCriteria::Types(ostream& os)
{
	if(ArgsOk())
	{
		And(os);
		os << "S.F_TYPE IN (" << SQLCharSet(lawTypes) << ")";
	}
}  // SessDistributionCriteria::LawTypes

void SessDistributionCriteria::Period(ostream& os)
{
	if(ArgsOk())
	{
		And(os);
		os << "(S.F_DATE BETWEEN " << begDate.Value() << " AND "
			<< endDate.Value() << ")";
	}
}  // SessDistributionCriteria::Period

void SessDistributionCriteria::Results(ostream& os)
{
	if(ArgsOk())
	{
		if(formatFN && formatFN[0])
		{
			if(section_says_yes(formatFN, "***_RESULT_FIXED_ONLY"))
			{
				And(os);
				os << "S.F_RESULT = '" << RESULT_FIXED << "'";
			}
		}
	}
}  // SessDistributionCriteria::Results

void SessDistributionCriteria::SessKinds(ostream& os)
{
	if(ArgsOk())
	{
		if(formatFN && formatFN[0])
		{
			if(section_says_yes(formatFN, "***_OPEN_SESSIONS_ONLY"))
				if(KIND_OPEN_SESSIONS[0])
				{
					And(os);
					os << "S.F_KIND IN (" << SQLCharSet(KIND_OPEN_SESSIONS) << ")";
				}
		}
	}
}  // SessDistributionCriteria::SessKinds

void SessDistributionCriteria::OtherSessConditions(ostream& os)
{
	if(ArgsOk())
	{
		if(composition)
		{
			And(os);
			os << "S.F_COMPOSITION = " << composition << " ";
		}
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
	}
}  // SessDistributionCriteria::OtherSessConditions

void SessDistributionCriteria::Join(ostream& os)
{
	if(ArgsOk())
	{
		And(os);
		os << "L.F_TYPE = S.F_TYPE";
		And(os);
		os << "L.F_YEAR = S.F_YEAR";
		And(os);
		os << "L.F_NO = S.F_NO";
	}
}  // SessDistributionCriteria::Join

void SessDistributionCriteria::Where(ostream& os)
{
	if(ArgsOk())
	{
		os << SQL_WHERE;
		Types(os);
		startWithAnd = true;
		SessKinds(os);
		Period(os);
		Results(os);
    OtherSessConditions(os);
		Join(os);
  }
}  // SessDistributionCriteria::Where

// ---------------- SessionsDistribution ------------------

class TSessionsDistributionThread : public TCheckupThread
{
	public:
		TSessionsDistributionThread(const begMYEndMYTypeCIU& bMYEMYTCIU);
		void PrintSessionsDistribution();
	protected:
		void GatherData();
    virtual void CreateOutDoc();
		virtual void Execute();
		virtual bool ArgsOk();
		void MakeFormatFN();
		void MakeDescription(CheckupActions action);
		void JudgeData(SessJudgeDistrData& data);
		void OneSessionData(const TSession& sess, const char lawKind,
			SessJudgeDistrData& data);
		void OneGatheringStep(const TSession& sess, const char lawKind);
		char GuessAboutLawKind(const SessJudgeDistrData* const pCurrentIten);

		const begMYEndMYTypeCIU& rBMYEMYTCIU;
		SessJudgeDistrArray* pArr;
		char* crit;
		SessDistributionOutFile* pOutF;
		char formatFN[128];
		char descrBuff[128];
};

TSessionsDistributionThread::
TSessionsDistributionThread(const begMYEndMYTypeCIU& bMYEMYTCIU) :
		TCheckupThread(0, 20),
		rBMYEMYTCIU(bMYEMYTCIU), pArr(NULL), crit(NULL), pOutF(NULL)

{
	formatFN[0] = descrBuff[0] = '\0';
}

void TSessionsDistributionThread::MakeFormatFN()
{
	ostrstream(formatFN, sizeof(formatFN))
		<< "text\\sesdistr" << DEFAULT_FORMAT_FILE_EXTENSION << ends;
	formatFN[sizeof(formatFN) - 1] = '\0' ;
}

bool TSessionsDistributionThread::ArgsOk()
{
	return (TCheckupThread::ArgsOk() &&
		rBMYEMYTCIU.BegDate().Empty() == false &&
		rBMYEMYTCIU.EndDate().Empty() == false &&
		rBMYEMYTCIU.types && rBMYEMYTCIU.types[0] && pArr &&
		crit && pOutF && formatFN[0]);
}

void TSessionsDistributionThread::MakeDescription(CheckupActions action)
{
	ostrstream oss (descrBuff, sizeof(descrBuff));

	oss << "Разпределение на заседанията - ";
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

void TSessionsDistributionThread::JudgeData(SessJudgeDistrData& data)
{
	if(ArgsOk())
	{
		const SessJudgeDistrData* theSame = NULL;

		for(int i = 0;
			theSame == NULL && i < pArr->Count();
			i ++)
		{
			const SessJudgeDistrData* const current = (*pArr)[i];

			if(strcmp(data.judgeUcn, current->judgeUcn) == 0)
				theSame = current;
		}
		if(theSame != NULL)
		{
			data.judgePost = theSame->judgePost;
			ostrstream(data.judgeInitials, sizeof(data.judgeInitials))
				<< theSame->judgeInitials << ends;
			ENDING_ZERO(data.judgeInitials);
		}
		else
		{
			data.judgeInitials[0] = '\0';  // Because of speed reasons
			ostrstream(User->ucn, sizeof(User->ucn)) << data.judgeUcn << ends;
			ENDING_ZERO(User->ucn);
			if(User->Try())
				data.judgePost = User->post;
			else
				data.judgePost = (char) 255 ;

		}
	}
}

void TSessionsDistributionThread::OneSessionData(const TSession& sess,
	const char lawKind,	SessJudgeDistrData& data)
{
	data.date = sess.date;
#if INSTANCE
	data.instance = 2;
	if(lawIsCocern3rdInst (lawKind))
		data.instance = 3;
#  if APPEAL
#  else  // of APPEAL
	if(strchr(KIND_1ST_LAWSUITS, lawKind))
		data.instance = 1;
#  endif  // of APPEAL
#else  // of INSTANCE
	data.instance = 1;
  if(lawKind)  // 11.oct.2002 - begin
  {
    // Nofting - suppress the warnig
  }  // 11.oct.2002 - end
#endif  // of INSTANCE
#if INSTANCE  // 11.oct.2002
	data.partOfDay = (sess.time < rBMYEMYTCIU.noon) ? Morning : Afternoon;
#else  // of INSTANCE  // 11.oct.2002 - begin
	data.partOfDay = Morning;
#endif  // of INSTANCE  // 11.oct.2002 - end
	ostrstream(data.judgeUcn, sizeof(data.judgeUcn))
		<< sess.judge << ends;
	ENDING_ZERO(data.judgeUcn);
	JudgeData(data);
	data.numberOfSessions = 1;
}

void TSessionsDistributionThread::OneGatheringStep(const TSession& sess,
	const char lawKind)
{
	if(ArgsOk())
	{
		SessJudgeDistrData data;

		OneSessionData(sess, lawKind, data);
		pArr->StoreUp(data);
	}
}

char TSessionsDistributionThread::GuessAboutLawKind(const SessJudgeDistrData* const pCurrentIten)
{
	char lawKind = '\0';

	if(pCurrentIten)
	{
		switch(rBMYEMYTCIU.types[0])
		{
#if INSTANCE
#  if APPEAL
			case TYPE_PUNISHMENT_LAW:
				lawKind = KIND_PUNISHMENT_LAWS[0];
				break;
			case TYPE_CITIZEN_LAW:
				lawKind = KIND_CITIZEN_LAWS[0];
				break;
#  else  // of APPEAL
			case TYPE_PUNISHMENT_LAW:
				if(pCurrentIten->instance == 1)
					lawKind = KIND_1ST_PUNISHMENT_LAWS[0];
				else
					if(pCurrentIten->instance == 3)
						lawKind = KIND_2ND_ADMIN_PUNISHMENT_LAW;
					else
						lawKind = KIND_2ND_COMMON_PUNISHMENT_LAW;
				break;
			case TYPE_CITIZEN_LAW:
				lawKind = pCurrentIten->instance == 1 ?
					KIND_1ST_CITIZEN_LAWS[0] : KIND_2ND_CITIZEN_LAWS[0];
				break;
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
			case TYPE_ADMIN_LAW:
				lawKind = pCurrentIten->instance == 1 ?
					KIND_1ST_ADMIN_LAWS[0] : KIND_2ND_ADMIN_LAWS[0];
				break;
#endif  // ADMIN || AREAL				
#if COURT_TYPE == COURT_AREAL   // 20070614
			case TYPE_TRADE_LAW:
				lawKind = pCurrentIten->instance == 1 ?
					KIND_1ST_TRADE_LAWS[0] : KIND_2ND_TRADE_LAWS[0];
				break;
			case TYPE_FIRM_LAW:
				lawKind = KIND_FIRM_LAW;
				break;
#endif  // AREAL
#  endif  // of APPEAL
#else  // of INSTANCE
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
			case TYPE_PUNISHMENT_LAW:
				lawKind = KIND_PUNISHMENT_LAWS[0];
				break;
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
			case TYPE_CITIZEN_LAW:
				lawKind = KIND_CITIZEN_LAWS[0];
				break;
			case TYPE_MARRIAGE_LAW:
				lawKind = KIND_MARRIAGE_LAWS[0];
				break;
			case TYPE_ADMIN_LAW:
				lawKind = KIND_ADMIN_LAWS[0];
				break;
#endif  // of INSTANCE
		}
	}
	return lawKind;
}

void TSessionsDistributionThread::CreateOutDoc()
{
	if(ArgsOk())
	{
		TGSArray<JudgeWithNames> jNames;

		pOutF->title();
		pOutF->FormattedHead(rBMYEMYTCIU);
		pOutF->head();
		for(int i = 0 ; i < pArr->Count() ; i ++)
		{
			Tick(false);
			if(wtdl->WantBreak())
				break;

			int minRows = 0;
			const SessJudgeDistrData* const pCurrentIten = (*pArr)[i];
			char lawKind = GuessAboutLawKind(pCurrentIten);

			if(lawKind && KIND_OPEN_SESSIONS[0])
				minRows = judges_count_expecting (lawKind, KIND_OPEN_SESSIONS[0],
					true);
			{
				SessDistributionColumns sDC(formatFN, wtdl->Parent, *pCurrentIten,
					i + 1, jNames, minRows);
				const int rows = sDC.getMaxRows() ;

				pOutF->NextItemToPrint(rows + 1) ;
				pOutF->horzLine() ;
				for(int i = 0 ; i < rows ; i ++)
					pOutF->WriteItem(sDC, true) ;
			}
		}
		pOutF->footherLine();
		pOutF->tail();
	}
}

void TSessionsDistributionThread::Execute()
{
	try
	{  // -- 0 --
		MakeDescription(GatheringData);
		wtdl->SetHeader(descrBuff);
		wtdl->PleaseWait();
		{  // -- 1 --
			TCountRecsQuery q(crit);
			SessPtr pSess;
			char lawKind;
			TDummyGroup lawKindGroup;

			totalPartSteps = q.getRecsCount();            // Gossip service
			endingVal = barHiVal - createOutDocSteps;     // Gossip service
			if(endingVal < barLowVal)                     // Gossip service
				endingVal = barLowVal;                      // Gossip service
			wtdl->SetProgressRange(barLowVal, barHiVal);
			passeds = "Обработени заседания:";
			lawKindGroup.Add(new TChar("LAW_KIND", &lawKind));
			while(q.Read())
			{  // -- 2 --
				Tick(false);
				if(wtdl->WantBreak())
					break;
				pSess.Ref() << q;
				lawKindGroup << q;
				OneGatheringStep(pSess.Ref(), lawKind);
			}  // -- 2 --
			Tick(true);
		}  // -- 1 --
		if(endingVal < barHiVal)                           // Gossip service
			endingVal = barHiVal;                            // Gossip service
		currentPartStep = 0;                               // Gossip service
		if(wtdl->WantBreak() == false)
		{
			MakeDescription(CreatingOutDoc);
			wtdl->SetHeader(descrBuff);
			wtdl->PleaseWait();
			totalPartSteps = pArr->Count();    // Gossip service
			passeds = "Генерирани редове:";                  // Gossip service
			CreateOutDoc();
		}
		Tick(true);                                        // Gossip service
    wtdl->AlmostDone();                                      // Gossip service
	}  // -- 0 --
	CATCH_ANYTHING
}

void TSessionsDistributionThread::PrintSessionsDistribution()
{
	MakeFormatFN();

	SessJudgeDistrArray arr;
	SessDistributionCriteria criteria(512, rBMYEMYTCIU.types, rBMYEMYTCIU.BegDate(), rBMYEMYTCIU.EndDate(),
		rBMYEMYTCIU.composition, formatFN, wtdl->Parent);

	SessDistributionOutFile outF(wtdl->Parent, formatFN);

	if (criteria.Compose())
	{
		pArr = &arr;
		pOutF = &outF;
		crit = criteria.Criteria();
		Generate();
		crit = NULL;
		pOutF = NULL;
		pArr = NULL;
	}

	show_text_stream_use_settings(NULL, outF, formatFN, FLOAT_ALIGNMENT);
}
