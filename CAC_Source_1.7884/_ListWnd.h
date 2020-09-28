class TColumnRowListWindow: public TListWindowEx  // TListWindow  // 11.nov.2003
{
  public:  // Construction / Destruction
    TColumnRowListWindow(TWindow* parent, int resourceId);
    virtual ~TColumnRowListWindow();
  protected:  // Methods
  	int GetColumnCount();
	int GetColumnAtX(const int x, int* const pX1 = NULL,
      int* const pX2 = NULL);
	int GetRowAtY(const int y, uint32* const pItData = NULL,
      int* const pY1 = NULL, int* const pY2 = NULL);
    int GetFirstRowWithMatchingData(const uint32 data, bool focusIt = false,
      bool selectIt = false);
};  // class TColumnRowListWindow

class TItemDataComparer: public TLwComparator
{
  public:  // Methods
    virtual int Compare(uint32 item1, uint32 item2, uint32 lParam) const;
};  // class TItemDataComparer

class TLawDistribListWindow: public TColumnRowListWindow
{
	public:  // Construction / Destruction
		TLawDistribListWindow(TWindow* parent, int resourceId,
			const TJudgeArray& jSet, ArrayOfJudgeLawStatsArrays& data,
			const SubjectRoll& sRoll, TStatic& ln_0, TStatic& ln_1);
		virtual ~TLawDistribListWindow();
  public:  // Methods
    virtual void SetupWindow();
    virtual void WriteOneCell(const JudgeLawStats& s, char* const dest,
      const size_t destSz);
    virtual bool InsertOneRow(const JudgeLawStatsArray& jLSA,
      const bool shortNames = false, const bool withSubjCode = true);
    virtual bool InsertEmptyRow(const JLawStatsArrInfo& info);
    bool DeleteOneRow(const JLawStatsArrInfo& info);
    bool Refresh();
    void HideLines();
  protected:  // Methods
    void EvLButtonDown(uint modKeys, TPoint& point);
    void EvPaint();
    void EvVScroll(uint scrollCode, uint thumbPos, THandle hWndCtl);
    void EvSize(uint sizeType, TSize& size);
    virtual bool InsertColumns();
    virtual bool InsertRows();
    int PlaceForIdentification(const uint32 identification) const;
    void ShowLines(const TPoint& point);
  protected:  // Data
    const TJudgeArray& judgSet;
    ArrayOfJudgeLawStatsArrays& dataArr;
    TStatic& ln0;
    TStatic& ln1;
    const SubjectRoll& subjRoll;
DECLARE_RESPONSE_TABLE(TLawDistribListWindow);
};  // class TLawDistribListWindow

class TLawDistribListWindow2: public TLawDistribListWindow
{
	public:  // Construction / Destruction
		TLawDistribListWindow2(TWindow* parent, int resourceId,
			const TJudgeArray& jSet, ArrayOfJudgeLawStatsArrays& data,
			const SubjectRoll& sRoll, TStatic& ln_0, TStatic& ln_1);
  public:  // Methods
    virtual void WriteOneCell(const JudgeLawStats& s, char* const dest,
      const size_t destSz);
    virtual bool InsertOneRow(const JudgeLawStatsArray& jLSA,
      const bool shortNames = false, const bool withSubjCode = true);
  protected:  // Methods
    static void ClosestWidth(char* const txt, const size_t txtSz,
      char* sample, const int align, TListWindow& lw);
    virtual void AdjustWidth(char* const txt, const size_t txtSz,
	const char* const sample);
    virtual bool InsertColumns();
    virtual bool InsertRows();
};  // class TLawDistribListWindow2
