class TypeErrorStrings
 {
  protected :
    const char * const wrongArgument ;
    const char * const noAccessibleTypes ;
    const char * const mayBeNoRights ;
  public :
    TypeErrorStrings () ;
 } ;  // class TypeErrorStrings

class TTypeAliasFace : public TCharAliasFace
 {
  protected :
    char types[SIZE_OF_TYPES];
  public :
    TTypeAliasFace (TWindow * parent, int resourceId, const char * name,
		    const bool readOnly = true,
		    const char * const typesAllowed = NULL) ;  // Empty
			 // string is not allowed
 } ;  // class TTypeAliasFace

class TMonthAliasFace : public TLongAliasFace
 {
  public :
    TMonthAliasFace (TWindow * parent, int resourceId, const char * name) ;
    virtual void SetupWindow () ;
 } ;  // class TMonthAliasFace

class RadioCluster
 {
  protected :
    struct ButtonData
     {
	TRadioButton * pButton ;
	bool enabled ;
      int idc ;
      bool operator == (const ButtonData & other)
       {return pButton == other.pButton ;}
     } ;
    TGroupBox * pGrpBox ;
    TIArray <ButtonData> cluster ;
    int oldSelection ;
    bool enabled ;
  public :
    RadioCluster () ;
    ~RadioCluster () ;
    void SetGrpBox (TGroupBox * pGB) ;
    TGroupBox * GetGrpBox () ;  // 05.aug.2003
    void Add (TRadioButton * pB, int idc = -1) ;
    int GetSelection () ;
    TRadioButton * GetSelectionP () ;
    int GetSelectionIdc () ;
    void SetSelection (int sel) ;
    void SetSelectionP (TRadioButton * pB) ;
    void SetSelectionIdc (int idc) ;
    void DisableCluster (int sel = 0) ;
    void DisableClusterP (TRadioButton * pB) ;
    void DisableClusterIdc (int idc) ;
    void EnableCluster () ;
    void EnableItem (int item, bool enable = true) ;
    void EnableItemP (TRadioButton * pB, bool enable = true) ;
    void EnableItemIdc (int idc, bool enable = true) ;
    bool GrpBoxIsEnabled () ;
 } ;  // class RadioCluster

class KindPtr
 {
  private :
    TKind k ;
  protected :
    TKind * KPtr () ;
 } ;  // class KindPtr

class DefaultLawKinds
 {
  protected :
    char lawKinds [SIZE_OF_KINDS] ;
  public :
    DefaultLawKinds (const char * const lKinds = NULL) ;  // The lKinds
      // parameter added on 29.jan.2002. NULL means "All law kinds"
    char * LawKinds () ;
 } ;  // class DefaultLawKinds

class TLawKindsListFace : public TCharListFace, public KindPtr,
				  public DefaultLawKinds, public TypeErrorStrings
 {
  protected :
    const bool accordingToRights ;
  public :
    TLawKindsListFace (TWindow * parent, int resourceId, const char * name,
                       const bool followRights = true,
			     const char * const lKinds = NULL) ;  // 29.jan.2002,
				 // Empty string is not allowed here!
    virtual ~TLawKindsListFace () ;
    virtual void SetupWindow () ;
 } ;  // class TLawKindsListFace

class TDateFaceOldVal : public TDateFace
 {
  protected :
    CDate oldVal ;
  public :
    TDateFaceOldVal (TWindow * parent, int resourceId, const char * name) ;
							    // mandatory will be always
							    // falce for this class
    virtual ~TDateFaceOldVal () ;
    virtual void DisableFace () ;
    virtual void EnableFace () ;
 } ;  // class TDateFaceOldVal

class TTimeFaceOldVal : public TTimeFace
 {  // 06.jun.2002
	protected :
		CTime oldVal ;
	public :
		TTimeFaceOldVal (TWindow * parent, int resourceId, const char * name) ;
										   // mandatory will be always
																							// falce for this class
		virtual ~TTimeFaceOldVal () ;
		virtual void DisableFace () ;
		virtual void EnableFace () ;
 } ;  // class TTimeFaceOldVal

class TCatchPasteEdit : public TEdit
 {
  protected :
    void PasteRequested () ;
  public :
    TCatchPasteEdit (TWindow * parent, int resourceId,
			   uint textLimit = 0) ;
    virtual ~TCatchPasteEdit () ;
  DECLARE_RESPONSE_TABLE (TCatchPasteEdit) ;
 } ;

class TYearFaceOldVal : public TYearFace
 {
  protected :
    long int oldVal ;
  public :
    TYearFaceOldVal (TWindow* parent, int resourceId, const char *name) ;
    virtual ~TYearFaceOldVal () ;
		virtual void DisableFace () ;
    virtual void EnableFace () ;
 } ;  // class TYearFace

class TStringFaceOldVal : public TStringFace
{
	protected:
    const uint oldValSz;
  	char* const oldVal;
    static uint TxtLimit(uint textLimit);
  public :
    TStringFaceOldVal(TWindow* parent, int resourceId, const char *name,
                      const uint textLimit) ;
    virtual ~TStringFaceOldVal();
    virtual void DisableFace();
    virtual void EnableFace();
} ;  // class TStringFaceOldVal
