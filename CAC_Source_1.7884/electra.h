// ----- TElectricCollect ----------------------------------------------------
class TElectricCollect
{
public:
	TElectricCollect(TRCDKeyContainer *container);

	void Collect();

protected:
	virtual void AddItem(TRCDKeyContainer *container) = 0;

	TRequest *request;		// 2002:298 Dual-mode
	TLawsuit *lawsuit;
	TInReg *inReg;			// 2002:329 Tripple-mode
	TOutReg *outReg;			// 2010:004 Quad-mode
	TProve *prove;			// 2010:194 Pentium

	char kinds[SIZE_OF_KINDS];	// 2010:162 Signals

	void AddProc(TRCDKeyContainer *container);
	void AddCopy(const TRCDKeyContainer *container);
};

// 2016:081 LPR: split for electric save
void electric_items_collect(TRCDKeyContainer *container, TGSArray<TElectric> &items);
// 2016:007 PRQ: public, for extra documents
void electric_items_interact(TWindow *parent, TRCDKeyContainer *container, bool attach, const TLawsuit *lawsuit);
// 2009:176 LPR: entry point, moved here from action
void electric_blue(TWindow *parent, TRCDKeyContainer *container, TPoint *where = NULL);
// 2011:314 LPR: _ only: toplevel modal electric without actions
// 2016:117 LPR: commented out, used only in firm distribution
//void electric_firm(TWindow *parent, TRCDKeyContainer *container);
// 2010:162 LPR: real-time TRCDKeyContainer signals; 2017:04 +data
void electric_signal(int sigtype, const TRCDKeyContainer *container, const void *data = NULL);
// 2010:172 LPR: generic electric-dialogs cycle
int electric_foreach(int command, void *argument);
