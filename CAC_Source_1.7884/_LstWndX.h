//{{TListWindow = TListWindowEx}}
class TListWindowEx : public TListWindow
{
  public:  // Construction / Destruction
    TListWindowEx(TWindow* parent, int resourceId);
    virtual ~TListWindowEx();
  public:  // Methods
    bool SetFullRowSel(BOOL bFillRowSel);
    bool GetFullRowSel();
  public:  // Data
    BOOL m_bClientWidthSel;
    bool m_bHorzLines;  // 01.oct.2004
    bool m_bVertLines;  // 01.oct.2004
	protected:  // Methods
    void InitializeData();
  	virtual void DrawItem(DRAWITEMSTRUCT far& drawInfo);
  	LRESULT EvSetBkColor(WPARAM wParam, LPARAM lParam);
  	LRESULT EvSetTextBkColor(WPARAM wParam, LPARAM lParam);
  	LRESULT EvSetTextColor(WPARAM wParam, LPARAM lParam);
  	LRESULT EvSetImageList(WPARAM wParam, LPARAM lParam);
  	static LPCTSTR MakeShortString(TDC& dc, LPCTSTR lpszLong, int nColumnLen,
      int nOffset);
  	void RepaintSelectedItems();
  	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);  // I could not find the
      // corresponding method in OWL. So I put some of it's code in
      // InitializeData. I hope this would be acceptable especially if the
      // second constructor (..., int resourceId...) is used.
	protected:  // Data
    // Attributes
  	bool m_bFullRowSel;
    // Implementation - client area width
    int m_cxClient;
    // Implementation - state icon width
    int m_cxStateImageOffset;
    // Implementation - list view colors
    COLORREF m_clrText;
    COLORREF m_clrTextBk;
    COLORREF m_clrBkgnd;

//{{TListWindowExRSP_TBL_BEGIN}}
  protected:
    void EvKillFocus(THandle hWndGetFocus);
    void EvPaint();
    void EvSetFocus(THandle hWndLostFocus );
    void EvSize(uint sizeType, TSize& size);
//{{TListWindowExRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TListWindowEx);
};    //{{TListWindowEx}}
