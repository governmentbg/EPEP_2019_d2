//{{TDialog = TSubpoenaDialog}}
class TSubpoenaDialog : public TResizeDialog
{
public:
	TSubpoenaDialog(TWindow* parent, TRCDKeyContainer *tContainer, char tStatus, TSideWind *tSideWind, int tSideIndex,
		TGArray<TLinkWind> *tLinks, TGArray<TSideWind> *allSides, TGArray<TUCNGroup> *affecteds,
		const TLawsuit *tLawsuit);

protected:
	TRCDKeyContainer *container;
	TSideWind *sideWind;
	TGArray<TLinkWind> *links;
	int sideIndex;
	char status;		// container status cache
	char originKind;
	char secondKind;		// 2010:281
	bool apcTemplate;		// 2014:035 regional only

	TStatic *name;
	TEdit *text;
	TStatic *count;
	TSubpoenaListWindow *subpoenas;

	TButton *print;
	TButton *notify;
	TButton *remand;
	TButton *erase;
	TButton *letter;
	TButton *outreg;
	TButton *number;	// 2012:039 LRQ: +related
	TButton *cancel;
	TButton *view;
	TButton *edit;	// 2015:033 +related

	virtual void Resize();

	mstr printKinds, letterKinds;
	mstr aloneKinds, notifyKinds;
	mstr stickerKinds;
	TSubpoenaVariantGroup variant;

	void ComputeKinds();
	void AddNotifyKinds(mstr &tNotifyKinds, const TSubpoena *sentSubpoena);
	void ItemChanged();
	bool ChooseSubpoenaVariant(TSubpoena *subpoena, const char *available, bool allowLinks);
	void PrintSubpoena(TSubpoena &subpoena);

//{{TSubpoenaDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
	virtual TResult EvNotify(uint id, TNotify far& notifyInfo);
//{{TSubpoenaDialogVIRTUAL_END}}

//{{TSubpoenaDialogRSP_TBL_BEGIN}}
	void SubpoenasLVNItemChanged(TLwNotify &);
	bool SubpoenasLVNItemChanging(TLwNotify &lwn);
	void NotifyBNClicked();
	void DeleteBNClicked();
	void LetterBNClicked();
	void PrintBNClicked();
	void ReturnBNClicked();
	void OutRegBNClicked();
	void NumberBNClicked();
	void ViewBNClicked();
	void EditBNClicked();
//{{TSubpoenaDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSubpoenaDialog);
};    //{{TSubpoenaDialog}}
