class column
 {
  private :
    size_t position, calls ;
    TStringArray strData ;
    bool columnErr ;
    const size_t cWidth ;
    TStringArray * pItems ;  // 21.jan.2005
    int oldItemsNumber ;  // 21.jan.2005
    void strToRows (const char * str, int leadingBlanks) ;
    void strToRowsL (const char * const str, size_t len,
			   int leadingBlanks) ;
    void textToRows (const char * const str, int leadingBlanks) ;
    void encloseStr (const char * const str, size_t len) ;  // 21.jan.2005
  public :
    column (size_t width, const char * str = NULL,
		const int leadingBlanks = 0) ;
    //virtual ~column () ;
    size_t getRowsCount () const ;
    const char * nextRowOrEmptyStr () ;
    void reWind () ;
    const char * operator [] (size_t index) const ;
    bool error () const ;  // Not implemented yet
    size_t currPosition () const ;
    size_t nextRowReqs () const ;
    size_t addText (const char * text, const int leadingBlanks = 0) ;
    size_t addText (const mstr &m, const int leadingBlanks = 0) { return addText(str(m), leadingBlanks); }
    size_t addEmptyRow () ;
    size_t addVerticalSeparator ();
    size_t width () const ;
    void flush () ;
    void setItems (TStringArray * itemsPtr) ;
 } ;  // class column

class columnArray : public TIArray<column>
{
public:
	columnArray() { }
	~columnArray() { mbk_clear(this); }
};

class numeratedColumn : public column
 {
  public :
    const int clmnNo ;
    numeratedColumn (size_t width, const int no,
			   const char * str = NULL,
			   const int leadingBlanks = 0) ;
    bool operator == (const numeratedColumn & other) ;
 } ;  // class numeratedColumn

class ColumnXmlEncloser
{  // 21.jan.2005
public:  // Construction / destruction
  ColumnXmlEncloser(column* columnPtr, int cipher,
    TStringArray* itemsPtr);
  ~ColumnXmlEncloser();
protected:  // Data
  column* const pColumn;
  const int code;
  TStringArray* const pItems;
};  // class ColumnXmlEncloser
