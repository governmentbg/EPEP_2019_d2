#include "cac.h"

StringObjectRoll::StringObjectRoll(const char* const startingItem_):
  startingItem(startingItem_)
{
	pDummy = strnew ("");
}  // StringObjectRoll::StringObjectRoll

StringObjectRoll::~StringObjectRoll()
{
	delete[] pDummy;
}  // StringObjectRoll::StringObjectRoll

int StringObjectRoll::ItemsCount() const
{
	return data.Count();
}  // StringObjectRoll::ItemsCount

const char* StringObjectRoll::operator[](const int index) const
{
	char* pSO = NULL;

	if(0 <= index && index <= ItemsCount())
		pSO = data[index];
	if(pSO == NULL)
	{
		error("%s::operator[](%d) was about to return a NULL pointer. %s",
			typeid(*this).name(), index, RETURNING_EMPTY_OBJ_INSTEAD);
		pSO = pDummy;
	}
	return pSO;
}  // StringObjectRoll::operator[]

size_t StringObjectRoll::CritSz() const
{
	return 1024;
}  // StringObjectRoll::CritSz

void StringObjectRoll::FillRoll()
{
	CharPtr criteria(CritSz());

  if(startingItem)
    data.Add(strnew (startingItem));
	if(MakeCriteria(criteria))
	{
		TCountRecsQuery q(criteria);

		while(q.Read())
			AddNextItem(q);
	}
}  // StringObjectRoll::FillRoll

CharMatchStringObjectRoll::CharMatchStringObjectRoll(
  const char* const startingItem_, const bool useMatch):
  StringObjectRoll(startingItem_), canMatch(useMatch),
  delimiter(' ')  // It looks that it is important for the delimiter
    // to be an invisible symbol (see the ActualData method)
{
}  // CharMatchStringObjectRoll::CharMatchStringObjectRoll

bool CharMatchStringObjectRoll::Match(char test, int ind) const
{
  bool match = false;

  if(ind < ItemsCount())
  {
    if(test && canMatch)  // test == '\0' always matches
    {
      if(ind == 0 && startingItem)
        match = true;  // I want startingItem always to match
      else
      {
        const char* const str = (*this)[ind];

        for(size_t i = 0; match == false && str[i] && str[i] != delimiter; i++)
          match = test == str[i];
      }
    }
    else
      match = true;
  }
  else
    error("%s\n%s::Match", WRONG_ARGS_PASSED_TO, typeid(*this).name());
  return match;
}  // CharMatchStringObjectRoll::Match

const char* CharMatchStringObjectRoll::ActualData(int ind) const
{
  const char* res = pDummy;

  if(ind < ItemsCount())
  {
    res = (*this)[ind];

    if(canMatch)
    {
	if((ind == 0 && startingItem) == false)
	{
	  {
	    size_t i = 0;

	    while(res[i] && res[i] != delimiter)
		i++;
	    // -- Commented, because causes an error in
	    // -- string_to_html_ostream_allowCode,
	    // -- I don't know why.
//          if(delimiter && res[i] == delimiter)  // Therefore isn't '\0'
//            i++;
	    // -----------------------------------------
	    res = &(res[i]);
	  }
	    // -- This way doesn't work, too (see above)
//        if(res[0] && res[0] == delimiter && res[1])
//          res = &(res[1]);
	    // -----------------------------------------
	}
    }
  }
  else
    error("%s\n%s::ActualData", WRONG_ARGS_PASSED_TO, typeid(*this).name());
  return res;
}  // CharMatchStringObjectRoll::ActualData

SubjectNameRoll::SubjectNameRoll(const char* const types_,
  const char* const startingItem_, const bool startNameWithType):
	CharMatchStringObjectRoll(startingItem_, startNameWithType),
  types(types_), includeTypeInName(startNameWithType)
{
}  // SubjectNameRoll::SubjectNameRoll

bool SubjectNameRoll::MakeCriteria(CharPtr& criteria)
{
	bool res = false;

	if(types && types[0])
	{
		{
			ostrstream oss(criteria, criteria.Size());

			oss << "SELECT F_TYPE, F_SUBJECT, F_NAME "
					"FROM T_SUBJECT WHERE F_TYPE IN ("
				<< SQLCharSet(types) << ") ORDER BY F_TYPE, F_SUBJECT"
					" ;" << ends;
		}
		res = criteria_finished_ok(criteria, typeid(*this).name(),
			"MakeCriteria");
	}
	return res;
}  // SubjectNameRoll::MakeCriteria

void SubjectNameRoll::AddNextItem(TQuery& q)
{
	subj << q;
  {
    char tmp[sizeof(subj.name) + 16];

    {
      ostrstream oss(tmp, sizeof(tmp));

      if(includeTypeInName)
        oss << subj.type << delimiter; // oss << subj.type << " "; //10.feb.2003
      oss << subj.name << ends;
    }
    ENDING_ZERO(tmp);
    data.Add(strnew (tmp));
  }
}  // SubjectNameRoll::AddNextItem

SubjectCodeRoll::SubjectCodeRoll(const char* const types_,
  const char* const startingItem_, const bool startCodeWithType):
  SubjectNameRoll(types_, startingItem_, startCodeWithType)
{
}  // SubjectCodeRoll::SubjectCodeRoll

void SubjectCodeRoll::AddNextItem(TQuery& q)
{
	subj << q;
  {
    char tmp[32 + 16];

    {
      ostrstream oss(tmp, sizeof(tmp));

      if(includeTypeInName )
        oss << subj.type << delimiter; // oss << subj.type << " "; //10.feb.2003
      oss << subj.Code(0, false) << ends;
    }
    ENDING_ZERO(tmp);
    data.Add(strnew (tmp));
  }
}  // SubjectCodeRoll::AddNextItem

JudgeNameRoll::JudgeNameRoll(const char* const types_,
  const char* const startingItem_, const bool startNamesWithColleges):
	CharMatchStringObjectRoll(startingItem_, startNamesWithColleges),
	types(types_), includeCollegesInNames(startNamesWithColleges)
{
}  // JudgeNameRoll::JudgeNameRoll

bool JudgeNameRoll::MakeCriteria(CharPtr& criteria)
{
	bool res = false;

	if(types && types[0])
	{
		{
			ostrstream oss(criteria, criteria.Size());

			oss << "SELECT F_UCN, F_COLLEGES FROM T_USER "
				"WHERE F_POST IN (" << SQLCharSet(POST_JUDGES) << ") AND "
					"F_FLAGS IN (";
			{
				arrayOfLong flagsArr;

				add_set_bit_values(flagsArr, FLAG_ACTIVE, MAX_USER_FLAGS);
				oss << flagsArr;
			}
			oss << ") ORDER BY F_POST, F_UCN"
					" ;" << ends;
		}
		res = criteria_finished_ok(criteria, typeid(*this).name(),
			"MakeCriteria");
	}
	return res;
}  // JudgeNameRoll::MakeCriteria

void JudgeNameRoll::AddNextItem(TQuery& q)
{
	bool hasRights = false;

	usr << q;
	for(size_t i = 0; hasRights == false && i < strlen(types); i++)
		if(strchr(usr.colleges, types[i]))
			hasRights = true;
	if(hasRights)
	{
		char tmp[SIZE_OF_NAME_FOR_BOOK + SIZE_OF_COLLEGES];

    tmp[0] = '\0';
    if(includeCollegesInNames)
    {
      ostrstream(tmp, sizeof(tmp)) << usr.colleges
        << delimiter  // << " "  // 10.feb.2003
        << ends;
      ENDING_ZERO(tmp);
    }
    {
      const size_t l = strlen(tmp);

      if(l + 1 < sizeof(tmp))
		    citizenUCNAndUCNTypeToName (usr.ucn, UCN_CITIZEN_UCN, tmp + l,
          sizeof(tmp) - l, false, true, false);
    }
		data.Add(strnew (tmp));
	}
}  // JudgeNameRoll::AddNextItem

JudgeUcnRoll::JudgeUcnRoll(const char* const types_,
  const char* const startingItem_, const bool startNamesWithColleges):
  JudgeNameRoll(types_, startingItem_, startNamesWithColleges)
{
}  // JudgeUcnRoll::JudgeUcnRoll

void JudgeUcnRoll::AddNextItem(TQuery& q)
{
	bool hasRights = false;

	usr << q;
	for(size_t i = 0; hasRights == false && i < strlen(types); i++)
		if(strchr(usr.colleges, types[i]))
			hasRights = true;
	if(hasRights)
	{
		char tmp[SIZE_OF_UCN + SIZE_OF_COLLEGES];
    
    {
      ostrstream oss(tmp, sizeof(tmp));

      if(includeCollegesInNames)
        oss << usr.colleges << delimiter;
      oss << usr.ucn << ends;
    }
		data.Add(strnew (tmp));
	}
}  // JudgeUcnRoll::AddNextItem
