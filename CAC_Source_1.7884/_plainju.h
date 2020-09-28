class TJudge : public TFoundGroup
 {
  private :
    void addFields () ;
  public :
    char post ;
    char name1stSymb ;
    char family1stSymb ;
    TJudge () ;
    TJudge (const char * const anUCN, const char anUCNType = UCN_CITIZEN_UCN,
	     const bool loadInitials = true  // 06.feb.2003
	    ) ;
    bool load (const char * const anUCN,
		   const char anUCNType = UCN_CITIZEN_UCN,
		   const bool loadInitials = true  // 06.feb.2003
		  ) ;
    void clear () ;
    bool nameFamily (char * pN, const size_t nSz,
			   char * pF, const size_t fSz) ;
    bool operator<=(const TJudge &that) const { return operator<(that) || operator==(that); }
protected:
    virtual bool operator==(const TJudge &other) const;      // 25.nov.2004
    virtual bool operator<(const TJudge &other) const;       // 25.nov.2004
 } ;  // class TJudge

typedef TGSArray<TJudge> TJudgeArray;

class JudgeWithNames : public TJudge
{
	public:  // Construction / destruction
		JudgeWithNames();
		JudgeWithNames(const char* const anUCN,
			const char anUCNType = UCN_CITIZEN_UCN);
		JudgeWithNames(const bool fillNames,
	const char* const anUCN,
			const char anUCNType = UCN_CITIZEN_UCN);      // 25.nov.2004
	public:  // Methods
		bool FillNames();
    bool operator<=(const TJudge &that) const { return operator<(that) || operator==(that); }
protected:
    virtual bool operator==(const TJudge &other) const;         // 25.nov.2004
    virtual bool operator<(const TJudge &other) const;          // 25.nov.2004

	public:  // Data
		char names[SIZE_OF_NAME_FOR_BOOK];
    char name[SIZE_OF_CITIZEN_NAME];      // 25.nov.2004
    char family[SIZE_OF_CITIZEN_FAMILY];  // 25.nov.2004
	protected:  // Methods
    void AddFields();                     // 25.nov.2004
	protected:  // Data
    bool namesFilled;                     // 25.nov.2004
};  // class JudgeWithNames

class subject_With_LongIntArray
 {
  protected :
    long * pLongs ;
  public :
    const size_t arrItems ;
    long subj ;
    subject_With_LongIntArray (const size_t lonsgCnt,
					 const long subject = 0) ;
    ~subject_With_LongIntArray () { delete [] pLongs ; }
    long & operator [] (size_t index) const ;
    const long & itemAtIndex (size_t index) const ;
    bool smallestVal (const size_t index,  // 14.aug.2002 - begin
			    const size_t indToExclude) const ;
    bool biggestVal (const size_t index,
			   const size_t indToExclude) const ;  // 14.aug.2002-end
    bool operator<=(const subject_With_LongIntArray &that) const { return subj <= that.subj; }
 } ;

class Date_With_LongIntArray : public subject_With_LongIntArray
{  // 14.aug.2002
  public:
    Date_With_LongIntArray(const size_t lonsgCnt);
    CDate d;
    bool operator<=(const Date_With_LongIntArray &that) const { return d <= that.d; };
};  // class Date_With_LongIntArray

class Date_With_LongIntArrayR : public Date_With_LongIntArray
{  // 14.aug.2002
  public:
    Date_With_LongIntArrayR(const size_t lonsgCnt);
};  // class Date_With_LongIntArrayR

bool operator<(const Date_With_LongIntArrayR& j1,
  const Date_With_LongIntArrayR& j2);
