// ----- TNotifying --------------------------------------------------------
class TSubpoenaVariantGroup;

class TNotifying
{
public:
	bool Notify(TSubpoenaVariantGroup *variant);

	// 2015:222 LPR: public for print_subpoena()
	TWindow* const pParent;
	TSubpoena &subpoena;
	TRCDKeyContainer *const pKeyContainer;
	TSubpoena *rSentSubpoena;

	static bool AlternateInsert(const TSubpoenaVariantGroup *variant, char subpoenaKind);
	bool Insert(TSubpoenaVariantGroup *variant, bool edit);

protected:
	TNotifying(TWindow *parent, int tSideIndex, char evStatus, TSubpoena &tSubpoena, TRCDKeyContainer *keyContainer);

	virtual bool PrepareForNotify(TLinkWind &wLink);
	virtual bool SpecificPreparations();
	bool InsertStandard(TSubpoenaVariantGroup *variant);
	bool InsertAlternate(TSubpoenaVariantGroup *variant, bool edit);
	virtual const char *Prefix();
	bool InsertRegisteredMail();
	bool PreSetDeliverer();

	bool FillRegMailReceiver(TRegisteredMail &mail);

	const char result;
	const int sideIndex;

	// 2011:306 LPR: get into subpoena
	bool GetAddress(TUCNGroup *ucnGroup, bool foreign);
};

class LinkWriter
 {
  protected :
    const size_t buffSz ;
    char * const buff ;
    stringArray & textReceiver ;
    const TRCDKey lawKey ;
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
    TDamn firmD ;
    bool firmDFound ;
    bool firmDSearched ;

    bool FirmDLoaded (const TRCDKey * const pDifferentFirmLawKey = NULL) ;
    bool DifferFLKeyOK (const TRCDKey * const pDifferentFirmLawKey) const ;
    void FakeReceiver (
	const TLinkWind & link,  // 09.oct.2003 from TSideLink
	const bool firmInfoAtIndex2IfPossible) ;
#   endif  // APPEAL
# endif  // INSTANCE
    // 09.oct.2003 from TSideLink; 2008:176 LPR: +links for multi-subpoenas
    void LinkTextItems (const TLinkWind & link, TGArray<TLinkWind> *links = NULL) ;
  public :
    LinkWriter (stringArray & txtReceiver, char * const buf,
		const size_t bufSz, const TRCDKey aLawKey) ;
    ~LinkWriter () ;
    // 09.oct.2003 from TSideLink; 2008:176 LPR: +links for multi-subpoenas
    void MakeLinkTextItems (
	const TLinkWind & link,
	const bool fake, const bool firmInfoAtIndex2IfPossible = false,
	TGArray<TLinkWind> *links = NULL);
    const char * FirmDName () ;
    bool useCommaChar;	    // 2010:077 LPR: +related
 } ;  // class LinkWriter

// ----- TNotifySubpoena ---------------------------------------------------
class TNotifySubpoena : public TNotifying
{
public:
	TNotifySubpoena(TWindow *parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer);

protected:
	virtual const char *Prefix();
};

// ----- TDecisionAnnounce -------------------------------------------------
class TDecisionAnnounce : public TNotifying
{
public:
	TDecisionAnnounce(TWindow* parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer);

protected:
	virtual const char *Prefix();
};


// ----- TInRegEventAnnounce -----------------------------------------------
class TInRegEventAnnounce : public TNotifying
{
public:
	TInRegEventAnnounce(TWindow *parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer);

protected :
	virtual const char *Prefix();
};

// ----- TResolutionAnnounce -----------------------------------------------
class TResolutionAnnounce : public TNotifying
{
public:
    TResolutionAnnounce(TWindow* parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer);

protected:
    virtual const char *Prefix();
};

// ----- TLetterTo ---------------------------------------------------------
class TLetterTo : public TNotifying
{
public:
	TLetterTo(TWindow* parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer,
		TSubpoena *tSentSubpoena);

protected:
	virtual bool PrepareForNotify(TLinkWind &wLink);
	virtual const char *Prefix();
	virtual bool FillRegMailReceiver(TRegisteredMail &mail);
	virtual bool SpecificPreparations();
	bool GetReceiver(TTransGroup &tUCNGroup);
};

// ----- TTelegram ---------------------------------------------------------
class TTelegram : public TNotifying
{
public:
	TTelegram(TWindow *parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer);

protected:
	virtual const char *Prefix();
};

// ----- TStatePaper -------------------------------------------------------
class TStatePaper : public TNotifying
{
public:
	TStatePaper(TWindow *parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer);

protected:
	virtual const char *Prefix();
	virtual bool FillRegMailReceiver(TRegisteredMail &mail);
};

// ----- TSWNotify ---------------------------------------------------------
class TSWNotify : public TNotifying
{
public:
	TSWNotify(TWindow* parent, int sideIndex, char evStatus, TSubpoena &subpoena, TRCDKeyContainer *keyContainer,
		TSubpoena *tSentSubpoena);

protected:
	virtual bool SpecificPreparations();
};
