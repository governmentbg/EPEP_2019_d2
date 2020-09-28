class TSubjectRange : public TGroup
 {
  public :
    long subject ;
    char type ;
    long lowSubjectRange ;
    long highSubjectRange ;
	  char name [SIZE_OF_SUBJECT_RANGE_NAME] ;
	  char attrib [SIZE_OF_SUBJECT_ATTRIB] ;
    char letters[SIZE_OF_SUBJECT_RANGE_LETTERS];

  public :
    TSubjectRange () ;

	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	// 2016:098
	static bool InsideRange(long lowSubjectRange, long highSubjectRange, long tSubject);
	virtual bool InsideRange(long tSubject) const;

private:
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	bool Get(const char *fields = NULL) { return TGroup::Get(fields); }
	virtual bool Try(const char *fields = NULL) { return TGroup::Try(fields); }
 } ;  // class TSubjectRange
