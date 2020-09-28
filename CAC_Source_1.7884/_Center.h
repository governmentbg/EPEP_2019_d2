struct LeftRight
 {
  CharPtrArray left ;
  CharPtrArray right ;
  int leftWidth ;
  int rightWidth ;
  const char * const delimiter ;
  LeftRight (const char * const delim = NULL) ; // NULL == "|"
  bool operator == (const LeftRight & other) const ;
 } ;  // struct LeftRight

class SummonsReducer
 {
  protected :
    char **iText;
    bool Load (stringArray & lines) ;
    bool Write (const stringArray & lines) ;
  public :
    SummonsReducer (char **tText) ;
    bool Reduce () ;
 } ;  // class SummonsReducer

class SummonsCenterer
 {
  protected :
    char **iText ;
    bool TabWarningShowed ;
    const int tabSize ;
    bool noDelimiterWarningShowed ;
    const char * const action ;

    int EffectiveLen (char * const str) ;
    int PageWidth (const CharPtrArray & page) ;
    void RefreshWidths (TIArray <LeftRight> & pages) ;
    void IncreasePageWidth (const CharPtrArray & page,
				    const int additionalBlanks) ;
    void BalanceLeftRight (LeftRight & lr) ;
    void BalanceAll (TIArray <LeftRight> & pages) ;
    int MaxWidth (const TIArray <LeftRight> & pages) ;
    void ExpandTo (LeftRight & lr, const int newWidth) ;
    void ExpandAll (TIArray <LeftRight> & pages, const int newWidth) ;
    bool SplitRowAndAddToLR (LeftRight & lr, char * const row) ;
    bool LoadLR (LeftRight & lr, istrstream & is) ;
    bool Load (TIArray <LeftRight> & pages) ;
    bool Write (const TIArray <LeftRight> & pages) ;

  public :
    SummonsCenterer (char **tText) ;
    bool Center () ;
 } ;  // class SummonsCenterer
