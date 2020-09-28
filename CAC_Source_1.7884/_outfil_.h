class pageManager
 {
  private :
    int pgLines, lines ;
  public :
    pageManager () ;
    pageManager (int pL, int l) ;
    void setParams (int pL, int l) ;
    int getLines () const ;
    int getPgLines () const ;
    int linesLeft () const ;
    void setLines (int n) ;
    void incLines (int n = 1) ;
    bool newPageNeed () const ;
    bool enoughLinesLeft (int n) const ;
 } ;  // class pageManager

const char
  titleMark = '#',
  tailMark = '$', 
  headEnd = '@',
  frmTxtParam = '@',
  frmTxtStmtEnd = '#',
  summDlgTextsMark = '&' ;

typedef void (* bEAct) (ofstream & os) ;

bool check_ifstream (ifstream & ifs, const char * const whatFile,
                     const char * const where) ;  // 17.oct.2001
bool seek_parameters_section(ifstream &ifs);
void load_format_parameters (ifstream & ifs, int & hLL, int & pageLines,
                             int & itemLines, int & itemLines1,
                             unsigned long int & extraVal,
                             unsigned long int & extraVal1,
                             int & alignment,
				     int & justify) ;  // 17.oct.2001

typedef void (* bEActText) (ostrstream & os) ;

class outText : public ostrstream
 {
  protected :
    char * hName ;
    bEActText done ;
    int hLL, pageLines, itemLines, itemLines1, continuation ;
    unsigned long int extraVal, extraVal1 ;
    int alignment, justify ;
    TWindow * pParent ;
    pageManager pM ;
    char horzLn [SIZE_OF_LINE] ;
    int pagesPrinted ;
    virtual void loadHLL () ;
    void toBeContinuedWithoutEndl () ;
    virtual void toBeContinued (const void * const anyData = NULL) ;
  public :
    outText (TWindow * parent, const char * hFName, bEActText initAct = NULL, bEActText doneAct = NULL) ;
    virtual ~outText () ;
    void initFF(const char * hFName);
    void title (ostream * oSPtr = NULL) ;
    int head (bool write = true) ;
    int tail (bool write = true) ;
    void newPage () ;
    void headAtNewPage () ;
    void horzLine () ;
    void topLine () ;
    void footherLine () ;
    void mediumFootherLine () ;
    void mediumTopLine () ;
    void footherAndTail () ;
    void nextLSToPrint (int linesNeed) ;
    void writeString (const char * str) ;
    int pageLinesLeft () const ;
    int pageLinesUsed () const ;
    int getPageLines () const ;
    int linesOfItem (bool first = true) const ;
    void resetPrintedLines () ;
    void newLine () ;
    bool changeFmt (const char * const otherFmt) ;
    int getAlignment () const ;
    int getJustification () const ;
    bool usedFormatFileName (char * const dest,
				     const size_t destSz) const ;
    int getPagesPrinted () const ;
    int sectionOut (const char * const section, bool write = true) ;

    friend class HTMLOutput ;
 } ;  // class outText

class frmHeadOutText : public outText
 {
  public :
    frmHeadOutText (TWindow * parent, const char * hFName) ;
    virtual bool supportsFormatterTail () const ;
    virtual bool supportsAnyFormattedText () const ;

    friend class HTMLOutput ;
    friend class BancruptcyHTMLWriter ;
  protected :
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
    virtual int formHead (const void * p, bool write,
	const char * const section = FORMATTED_HEADER_SECTION,
				  int * pNumberOfCodes = NULL,
				  TWaitThread *pDLThrd = NULL,
				  TWaitThreadDialog *wtdl = NULL
				 ) ;
  protected:  // Data
    ostream* pAltStream;  // 22.jan.2004 - to be used from derived classes as
	// an alternative output
 } ;  // class frmHeadOutText

class timeBookOutFile : public frmHeadOutText
 {
  protected :
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
    virtual void toBeContinued (const void * const anyData) ;
  public :
    timeBookOutFile (TWindow * parent,
			   const char * hFName = NULL) ;
    int formattedHead (const collCompBegDEndDBegN * cCBDEDBNPtr,
			     bool write = true,
                       ostream* altStreamPtr = NULL  // 02.feb.2005
                      ) ;
    void writeItem (timeBookColumns & tBC, bool first) ;
    void nextSToPrint (int inesNeed,
                       const collCompBegDEndDBegN * cCBDEDBNPtr,
                       timeBookColumns * pTBC = NULL,
                       bool * pDateRowWritten = NULL) ;
    size_t minRowsForSess () ;
    void oneSessPrtStartFinalChar (const bool start = true,
                                   const bool newLBefore = false,
                                   const bool newLAfter = false) ;
    void IncItemLines(bool first);  // 02.feb.2005
    void IncLines(int n);  // 02.feb.2005
    void SetPageLines(int n);  // 09.feb.2005
 } ;  // class timeBookOutFile

class timeBookSummaryOutFile : public frmHeadOutText
 {
  protected :
    const CodeValueArray * const pCodeValArr ;
# if INSTANCE
    const TISArray <reportData> * const pRDArr ;
# endif  // of INSTANCE
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
  public :
    timeBookSummaryOutFile (TWindow * parent,
				    const char * hFName = NULL,
    const CodeValueArray * const pCodeValueArr = NULL
# if INSTANCE
									,
				    const TISArray <reportData>
					* const pRpDataArr = NULL
# endif  // of INSTANCE

				   ) ;
    void formattedHead (const collCompBegDEndDBegN * cCBDEDBNPtr,
				const bool write = true,
				int * pCodesNum = NULL,
				TWaitThread * pDLThrd = NULL,
				TWaitThreadDialog *wtdl = NULL
			     ) ;
 } ;  // timeBookSummaryOutFile

class judgeCheckupOutFile : public frmHeadOutText
 {
  protected :
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
  public :
    judgeCheckupOutFile (TWindow * parent,
                       const char * hFName = NULL) ;
    int formattedHead (const begMYEndMYTypeCIU * bMYEMYTCIUPtr,
                       bool write = true) ;
    virtual  // 04.dec.2000                   
    void writeItem (judgeCheckupColumns & rJCC, bool first) ;
    virtual bool supportsFormatterTail () const ;  // 04.apr.2001                 
    int formattedTail (const begMYEndMYTypeCIU * bMYEMYTCIUPtr,
                       bool write = true) ;  // 04.apr.2001
 } ;  // class judgeCheckupOutFile

# if INSTANCE
#   if APPEAL
class IlievaJudgeCheckupOutFile : public judgeCheckupOutFile
 {
  public :
    IlievaJudgeCheckupOutFile (TWindow * parent,
					 const char * hFName = NULL) ;
    virtual void writeItem (judgeCheckupColumns & rJCC, bool first) ;
 } ;  // class IlievaJudgeCheckupOutFile
#   else  // of APPEAL
class KracholovJudgeCheckupOutFile : public judgeCheckupOutFile
 {
  public :
    KracholovJudgeCheckupOutFile (TWindow * parent,
                                  const char * hFName = NULL) ;
    virtual void writeItem (judgeCheckupColumns & rJCC, bool first) ;
 } ;  // class KracholovJudgeCheckupOutFile

class PdCitJudgeCheckupOutFile : public judgeCheckupOutFile
 {
  public :
    PdCitJudgeCheckupOutFile (TWindow * parent,
                              const char * hFName = NULL) ;
    virtual void writeItem (judgeCheckupColumns & rJCC, bool first) ;
 } ;  // class PdCitJudgeCheckupOutFile

class FirmJudgeCheckupOutFile : public judgeCheckupOutFile
 {
  public :
    FirmJudgeCheckupOutFile (TWindow * parent,
                             const char * hFName = NULL) ;
    virtual void writeItem (judgeCheckupColumns & rJCC, bool first) ;
    virtual bool supportsFormatterTail () const ;                 
 } ;  // class FirmJudgeCheckupOutFile

#   endif  // of APPEAL
# endif  // of INSTANCE

class plainJudgeCheckupOutFile : public judgeCheckupOutFile
 {
  protected :
    const int judgesCnt ;
    const int subjNameColumnWidthCoef ;
    size_t vertLines () ;
    size_t numberColumnWidth () ;
    size_t subjectTextColumnWidth () ;
    void makeMediumLine () ;
    void fillColumnsWithNames (const TJudgeArray & const sWJHA,
					 TIArray <numeratedColumn> & columns) ;
    void fillColumnsWithNumbers (const subject_With_LongIntArray &
						    const sWJHA,
					   TIArray <numeratedColumn> & columns,
					   const char * const types,
					   const bool first) ;
    void mkNColumnsArr (TIArray <numeratedColumn> & columns) ;
    int wrtItem (const TIArray <numeratedColumn> & columns,
		     bool write) ;
	// returns number of lines written
    // 20.nov.2002 - begin  (some HTML support)
    virtual bool IsHtml () { return false ; } ;
    virtual const char * CellTxtVAlign () { return NULL ; } ;
    virtual const char * CellTxtAlign () { return NULL ; } ;
    virtual bool CellTxtBold () { return false ; } ;
    virtual const char * CellBgColor () { return NULL ; } ;
    virtual const char * CellFontColor () { return NULL ; } ;//20.nov.2002-end
  public :
    plainJudgeCheckupOutFile (TWindow * parent,
					const char * hFName,
					const int howManyJudges) ;
    int writePlainItem (const TJudgeArray & const jArray,
				const bool write = true) ;
	// returns number of lines written
    int writePlainItem (const subject_With_LongIntArray & const lArray,
				const char * const types,
				const bool first = true, const bool write = true) ;
	// returns number of lines written
 } ;  // class plainJudgeCheckupOutFile

class summaryDataOutFile : public frmHeadOutText
 {
  protected :
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
  public :
    summaryDataOutFile (TWindow * parent,
				const char * hFName = NULL) ;
    int formattedHead (const summaryFmtHeadData & sFHD,
			     bool write = true) ;
    void writeItem (summaryColumns & pSC, bool first) ;
    void nextItemToPrint (int linesNeed,
				  const summaryFmtHeadData & sFHD) ;
 } ;  // class summaryDataOutFile

class inputDiaryOutFile : public frmHeadOutText
 {
  protected :
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
  public :
    inputDiaryOutFile (TWindow * parent,
                       const char * hFName = NULL) ;
    void writeItem (requestColumns & rC, bool first) ;
    int formattedHead (const begNEndNYearTypes * bNENYTPtr,
                       bool write = true) ;
    void nextReqToPrint (int inesNeed,
                         const begNEndNYearTypes * bNENYTPtr) ;
 } ;  // class inputDiaryOutFile

class AppealedLawsInputDiaryOutFile : public inputDiaryOutFile
 {
  public :
    AppealedLawsInputDiaryOutFile (TWindow * parent,
					     const char * hFName = NULL) ;
    void writeItem (AppealedLawsInRegColumns & iRC, bool first) ;
    size_t minRowsForInreg () const ;
    void prtStartFinalChar (const bool start = true,
				    const bool newLBefore = false,
				    const bool newLAfter = false) ;
 } ;  // class AppealedLawsInputDiaryOutFile

class ResultsSetBkOutFile : public frmHeadOutText
 {
  protected :
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
  public :
    ResultsSetBkOutFile (TWindow * parent,
                         const char * hFName = NULL) ;
    int FormattedHead (const collCompBegDEndDBegN * cCBDEDBNPtr,
                       bool write = true) ;
    void WriteItem (ResultsSetBookColumns & rSBC, const bool first) ;
 } ;  // class ResultsSetBkOutFile

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
class TradeLaw634OutFile : public frmHeadOutText
 {
  protected :
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
  public :
    TradeLaw634OutFile (TWindow * parent,
				const char * hFName) ;
    int FormattedHead (const Trade634DlgData * T634DDPtr,
			     bool write = true) ;
    void WriteItem (TradeLaw634BookColumns & tL634BC, const bool first) ;
    size_t MinRowsTL634Chain () const ;
    void NextChainToPrint (const int linesNeed, const bool withHead) ;
    void OneChainPrtStartFinalChar (const bool start = true,
						const bool newLBefore = false,
						const bool newLAfter = false) ;
 } ;  // class TradeLaw634OutFile

#   endif  // of APPEAL
# endif  // of INSTANCE

class SessDistributionOutFile : public frmHeadOutText
{
	protected :
		virtual void fHOutput (const void* p, char* s, size_t len,
			int algn, size_t skip, int dCipher);
	public :
		SessDistributionOutFile (TWindow * parent,
			const char * hFName);
		int FormattedHead (const begMYEndMYTypeCIU& bMYEMYTCIU,
			bool write = true);
		void WriteItem (SessDistributionColumns& aSDC, const bool first);
		void NextItemToPrint (const int linesNeed,
			const bool addHead = true);
};  // class SessDistributionOutFile

class ZdravkovaDistributionOutFile : public judgeCheckupOutFile
{
	public:
		ZdravkovaDistributionOutFile(TWindow* parent,
			const char* hFName, const int howManyJudges);
		int WriteHeader(const TJudgeArray& jArray, const bool first = true,
				const bool write = true);
		// returns the number of lines written
		int WritePlainItem(const subject_With_LongIntArray& primary,
				const subject_With_LongIntArray& changes,
				const char* const types, const bool atTheTop,
        const bool first = true, const CDate* pDate = NULL, 
				const bool write = true, CharPtr* pShortNameHolder = NULL);
		// returns the number of lines written
	protected:  // Methods
		size_t VertLines();
		size_t HeaderJudgeVertLines();
		size_t NumberColumnWidth();
		size_t SubjectTextColumnWidth();
		size_t HeaderJudgeColumnWidth();
		void MakeHeaderAboveJudgeMediumLine();
		void MakeHeaderBelowJudgeMediumLine();
		void MakeMediumLine();
		void FillColumnsWithNames(const TJudgeArray& judgArr,
				TIArray<numeratedColumn>& columns, const bool first);
		void FillHeaderPrimaryChangesColumns(
        TIArray<numeratedColumn>& columns);
		void FillColumnsWithNumbers(const subject_With_LongIntArray& primary,
				const subject_With_LongIntArray& changes,
				TIArray<numeratedColumn>& columns, const char* const types,
        const CDate* pDate, const bool first, const bool write = true,
        CharPtr* pShortNameHolder = NULL);
		void MakeHeaderNamesArray(TIArray<numeratedColumn>& columns);
		void MakeHeaderPrimaryChangesArray(TIArray<numeratedColumn>& columns);
		void MakeDataLinesArray(TIArray<numeratedColumn>& columns);
		int WrtItem(const TIArray<numeratedColumn>& columns,
      const int nonSubjectClmnWidth, const bool write);
	protected:  // Data
		const int judgesCnt;
		const int subjNameColumnWidthCoef;
		const int numberColumnDefaultMaxWidth;
    int pageWidth;
    int numberColumnMinWidth;
};  // class ZdravkovaDistributionOutFile

class ZdravkovaDistributionHTMLOutFile :
  public ZdravkovaDistributionOutFile
{
	public:
		ZdravkovaDistributionHTMLOutFile(TWindow* parent,
			const char* hFName, const int howManyJudges);
		void WriteHeader(const TJudgeArray& jArray, const bool first = true);
		void WritePlainItem(const TJudgeArray& jArr,
	const subject_With_LongIntArray& primary,
			const subject_With_LongIntArray& changes,
			const char* const types, const bool first = true,
			const CDate* pDate = NULL);
	protected:  // Methods
		void FillCell(column& clmn);
};  // class ZdravkovaDistributionHTMLOutFile

// 20051010 -->
# if ! INSTANCE  // Regional court only
class bookZZDNOutFile : public frmHeadOutText
 {
  protected :
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
    virtual void toBeContinued (const void * const anyData) ;
  public :
    bookZZDNOutFile (TWindow * parent,
                     const char * hFName = NULL) ;
    int formattedHead (const ZZDN_Dlg_data * cCBDEDBNPtr_ZZDN,
                       bool write = true,
                       ostream* altStreamPtr = NULL  // 02.feb.2005
                      ) ;
    void writeItem (BookZZDNColumns & tBC, bool first) ;
    void nextSToPrint (int inesNeed,
                       const ZZDN_Dlg_data * cCBDEDBNPtr_ZZDN,
                       BookZZDNColumns * pTBC = NULL,
                       bool * pDateRowWritten = NULL) ;
    size_t minRowsForSess () ;
    void oneSessPrtStartFinalChar (const bool start = true,
					     const bool newLBefore = false,
                                   const bool newLAfter = false) ;
    void IncItemLines(bool first);  // 02.feb.2005
    void IncLines(int n);  // 02.feb.2005
    void SetPageLines(int n);  // 09.feb.2005
 } ;  // class bookZZDNOutFile
# endif
// 20051010 <--

class HTMLOutText : public frmHeadOutText
{  // 20.nov.2002
	public:
		HTMLOutText(TWindow* parent, const char* fmtFName);
		void WriteToTableRow(columnsArr& cArr, const int basicCipher,
			const char* const section);
		static void WriteToTableRow(columnsArr& cArr, const int basicCipher,
			const char* const section, outText& outF, const char* const hName);
		static void DumpColumn(ostream& os, columnsArr& cArr,
	const int cipher);
		static void DumpColumn(ostream& os, column * const pCol);
};  // class HTMLOutText

class HTMLOutput
{  // 20.nov.2002
	public:
		HTMLOutput(outText& outTextRef);
    virtual ~HTMLOutput();
		void WriteToTableRow(columnsArr& cArr, const int basicCipher,
			const char* const section);
    void SectionOut(const char* const section);
  protected:
    outText& outF;
};  // class HTMLOutput
