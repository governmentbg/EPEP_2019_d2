class CharPtr;

class StringObjectRoll
{
	public:  // Construction / destruction
		StringObjectRoll(const char* const startingItem_ = NULL);
		~StringObjectRoll();
	public:  // Methods
		int ItemsCount() const;
		const char* operator[](const int index) const;
		void FillRoll();
	protected:  // Methods
		virtual size_t CritSz() const;
		virtual bool MakeCriteria(CharPtr& criteria) = 0;
		virtual void AddNextItem(TQuery& q) = 0;
	protected:  // Data
		TStringArray data;
		char* pDummy;
    const char* const startingItem;
};  // class StringObjectRoll

class CharMatchStringObjectRoll : public StringObjectRoll
{
	public:  // Construction / destruction
		CharMatchStringObjectRoll(const char* const startingItem_ = NULL,
	const bool useMatch = false);
	public:  // Methods
    virtual bool Match(char test, int ind) const;  // test == '\0' always
	// matches, but ind must be valid
    virtual const char* ActualData(int ind) const;
	protected:  // Data
    const bool canMatch;
    const char delimiter;
};  // class CharMatchStringObjectRoll

class SubjectNameRoll : public CharMatchStringObjectRoll
{
	public:
		SubjectNameRoll(const char* const types_,
	const char* const startingItem_ = NULL,
	const bool startNameWithType = false);
	protected:  // Methods
		virtual bool MakeCriteria(CharPtr& criteria);
		virtual void AddNextItem(TQuery& q);
	protected:  // Data
		const char* const types;
    const bool includeTypeInName;
		TSubject subj;
};  // class SubjectNameRoll

class SubjectCodeRoll : public SubjectNameRoll
{
	public:
		SubjectCodeRoll(const char* const types_,
      const char* const startingItem_ = NULL,
      const bool startCodeWithType = false);
	protected:  // Methods
		virtual void AddNextItem(TQuery& q);
};  // class SubjectCodeRoll

class JudgeNameRoll : public CharMatchStringObjectRoll
{
	public:
		JudgeNameRoll(const char* const types_,
      const char* const startingItem_ = NULL,
      const bool startNamesWithColleges = false);
	protected:  // Methods
		virtual bool MakeCriteria(CharPtr& criteria);
		virtual void AddNextItem(TQuery& q);
	protected:  // Data
		const char* const types;
    const bool includeCollegesInNames;
		TUser usr;
};  // class JudgeNameRoll

class JudgeUcnRoll : public JudgeNameRoll
{  // 10.feb.2003
	public:
		JudgeUcnRoll(const char* const types_,
      const char* const startingItem_ = NULL,
      const bool startNamesWithColleges = false);
	protected:  // Methods
		virtual void AddNextItem(TQuery& q);
};  // class JudgeUcnRoll
