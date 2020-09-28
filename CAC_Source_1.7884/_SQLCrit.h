class SQLCriteria
{
	private:
    bool AbstractMethodCalled(const char* methodName);
	protected:
  	const size_t critSz;
    char* const crit;
    bool startWithAnd;
    bool composedOk;
    virtual bool ArgsOk();
    virtual void What(ostream& os);
    virtual void From(ostream& os);
    virtual void Where(ostream& os);
    virtual void And(ostream& os);
    virtual void OrderBy(ostream& os);
  public:
    SQLCriteria(const size_t size);
    ~SQLCriteria() { delete[] crit; }
    virtual bool Compose();
    char* Criteria();
    const char* CriteriaC();
};  // class SQLCriteria

class ReportOthActsCriteria : public SQLCriteria
{
	protected:
    const char* const lawTypes;
    const long int composition;
    const CDate begDate;
    const CDate endDate;
    virtual bool ArgsOk();
    virtual void From(ostream& os);
    virtual void What(ostream& os);
    virtual void Types(ostream& os);
    virtual void Period(ostream& os);
    virtual void Composition(ostream& os);
    virtual void Results(ostream& os);
    virtual void SessKinds(ostream& os);
    virtual void OtherSessConditions(ostream& os);
    virtual void Join(ostream& os);
    virtual void OtherLawConditions(ostream& os);
    virtual void OtherActsCondition(ostream& os);
    virtual void Where(ostream& os);
  public:
  	ReportOthActsCriteria(const size_t size,
    	const char* const lawsuitTypes, const long int composit,
      const CDate begD, const CDate endD);
};  // class ReportOthActsCriteria

class ReportOthActsOpenSesCriteria : public ReportOthActsCriteria
{
	protected:
    virtual bool ArgsOk();
    virtual void Results(ostream& os);
    virtual void SessKinds(ostream& os);
    virtual void Where(ostream& os);
  public:
  	ReportOthActsOpenSesCriteria(const size_t size,
    	const char* const lawsuitTypes, const long int composit,
      const CDate begD, const CDate endD);
};  // class ReportOthActsOpenSesCriteria

class ReportOthActsClosedSesCriteria : public ReportOthActsCriteria
{
	protected:
    virtual bool ArgsOk();
    virtual void Results(ostream& os);
    virtual void SessKinds(ostream& os);
    virtual void OtherSessConditions(ostream& os);
    virtual void Where(ostream& os);
  public:
  	ReportOthActsClosedSesCriteria(const size_t size,
    	const char* const lawsuitTypes, const long int composit,
      const CDate begD, const CDate endD);
};  // class ReportOthActsClosedSesCriteria

class ReportOthActsOpenSesCriteria1 : public ReportOthActsOpenSesCriteria
{
	protected:
    virtual bool ArgsOk();
    virtual void OtherLawConditions(ostream& os);
    virtual void Where(ostream& os);
  public:
  	ReportOthActsOpenSesCriteria1(const char* const lawsuitTypes,
    	const long int composit, const CDate begD, const CDate endD);
};  // class ReportOthActsOpenSesCriteria1

class ReportOthActsClosedSesCriteria1 :
	public ReportOthActsClosedSesCriteria
{
	protected:
    virtual bool ArgsOk();
    virtual void OtherLawConditions(ostream& os);
    virtual void Where(ostream& os);
  public:
  	ReportOthActsClosedSesCriteria1(const char* const lawsuitTypes,
    	const long int composit, const CDate begD, const CDate endD);
};  // class ReportOthActsClosedSesCriteria1

#if INSTANCE
class ReportOthActsOpenSesCriteria2 : public ReportOthActsOpenSesCriteria
{
	protected:
    virtual bool ArgsOk();
    virtual void OtherLawConditions(ostream& os);
    virtual void Where(ostream& os);
  public:
  	ReportOthActsOpenSesCriteria2(const char* const lawsuitTypes,
    	const long int composit, const CDate begD, const CDate endD);
};  // class ReportOthActsOpenSesCriteria2

class ReportOthActsClosedSesCriteria2 :
	public ReportOthActsClosedSesCriteria
{
	protected:
    virtual bool ArgsOk();
    virtual void OtherLawConditions(ostream& os);
    virtual void Where(ostream& os);
  public:
  	ReportOthActsClosedSesCriteria2(const char* const lawsuitTypes,
    	const long int composit, const CDate begD, const CDate endD);
};  // class ReportOthActsClosedSesCriteria2
#endif  // of INSTANCE

class SessDistributionCriteria : public SQLCriteria
{  // 03.jun.2002
	protected:
		const char* const lawTypes;
		const CDate begDate;
		const CDate endDate;
		const long int composition;
		const char* const formatFN;
		TWindow* const pParent;
		virtual bool ArgsOk();
		virtual void What(ostream& os);
		virtual void From(ostream& os);
		virtual void Types(ostream& os);
		virtual void Period(ostream& os);
		virtual void Results(ostream& os);
		virtual void SessKinds(ostream& os);
    virtual void OtherSessConditions(ostream& os);
		virtual void Join(ostream& os);
		virtual void Where(ostream& os);
	public:
		SessDistributionCriteria(const size_t size,
			const char* const lawsuitTypes, const CDate begD,
			const CDate endD, const long int composit,
			const char* const fmtFN = NULL, TWindow* parent = NULL);
};  // class SessDistributionCriteria
