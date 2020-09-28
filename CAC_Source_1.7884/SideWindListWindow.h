//{{TListWindow = TSideWindListWindow}}
class TSideWindListWindow : public TGroupListWindow
{
public:
	TSideWindListWindow(TWindow* parent, long tSideMode, int resourceId, TRCDKeyContainer *tContainer, TLawsuit *tLawsuit);
	virtual ~TSideWindListWindow();

	void Links();		// 2003:333 BRW: process_links()
	void Messages();		// 2003:333 BRW: process_subpoenas()
	void List(bool right);	// 2004:009 BRW: print_side_list()
	void Roll();		// 2004:015 BRW: print_subpoena_list()
	void Child(int command);// 2016:349 LPR: was Reage()
	// 2011:157 LPR: common, redraw group [status]
	void Expend(TSession *session, TLawsuit *lawsuit);
	void Attach();		// 2016:006 PRQ: extra documents
	TLawsuit *Lawsuit() { return lawsuit; }

	TSideWind *SideWind() { return (TSideWind *) Group(); }
	void RedrawGroup() { TGroupListWindow::RedrawGroup(); }
	void LoadSideWinds(TGArray<TSideWind> *sideWinds);	// 2010:077 for xfer load

	TSideWind *SideWindAt(int index) const { return (TSideWind *) GroupAt(index); }
	int Count() const { return array->Count(); }
	bool stat;	// 2011:077 LPR: bulstat required
	// 2016:243; 2018-10-30: was GetEPortalLawsuit(inReg)
	void SetLawsuit(TLawsuit *tLawsuit) { lawsuit = tLawsuit; }
#if COURT_TYPE == COURT_REGIONAL
	void Heritage(int command);	// 2016:351
#endif  // REGIONAL

protected:
	TRCDKeyContainer *container;	// 2009:183 LPR: from TDocSideWindListWindow
	const bool edit;			// 2010:305 LPR: edit and mode
	mstr involvements;
	char status;
	bool unwanted;			// 2008:169 LPR: unwanted flag cache

	// 2008:170 LPR: load links 1st time they are needed then keep/cache
	TLinkWindArray *links;
	void LoadLinkWinds();

	// 2016:245 LPR: send to e-portal if container == lawsuit
	void EPortalSend(TSideWind *sideWind, char oper);
	// 2018-10-29: LRQ: show EPortalAccess column
#if TESTVER
	enum { COLUMN_EPORTAL = 3 };
#else  // TESTVER
	enum { COLUMN_EPORTAL = -1 };
#endif  // TESTVER
	virtual void GetGroupText(mstr &m, TGroup *group, int column);

	TLawsuit *lawsuit;						// 2016:238
	bool GetUCNGroup(TTransGroup &ucnGroup);
	virtual bool CheckInvolvement(const TSideWind *);	// == true
	virtual bool EditableInvolvement(const TSideWind *);	// == involvements

	TSideWindArray *Sides() { return (TSideWindArray *) Array(); }
	virtual void GetSearchText(mstr &m, int index);
	void InitGroup(TSideWind *sideWind) const;
	virtual TGroup *NewGroup() const;
	virtual void Refresh();

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TSideWindListWindow}}
