// ----- TReportJudge --------------------------------------------------------
class TReportJudge : public TCitizen
{
public:
	TReportJudge(const char *tUCN);

	int no;
	mstr name;  // 2018-07-13: was [SIZE_OF_USER_NAME]
	long values[SECTIONS_MAX][JUDGE_COLUMNS_MAX];
};

extern const offield repjudge_fields[];

// ----- TReportJudgeArray ---------------------------------------------------
class TReportJudgeArray : public TGroupArray
{
public:
	TReportJudge *operator[](int i) const { return (TReportJudge *) TGroupArray::operator[](i); }
	TReportJudge *JudgeAt(i) { return (*this)[i]; }
	void Increment(const char *judge, int section, int column, bool total = true);
	void Summary(const char *name);
};
