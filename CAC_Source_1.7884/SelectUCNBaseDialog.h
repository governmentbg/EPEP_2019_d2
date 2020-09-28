//{{TDialog = TSelectUCNBaseDialog}}
class TSelectUCNBaseDialog : public TGroupDialog
{
public:
	TSelectUCNBaseDialog(TWindow* parent, TFoundGroup *ucnGroup, const char *tUCNTypes, const char *tInvolvements,
		bool active, bool stat, int resId);

protected:
	TCharAliasFace *uxnType;
	TSelectUCNListBox *groups;
	TUCNFace *ucn;
	TCitizenNameFace *citizenName;
	TCitizenNameFace *reName;
	TCitizenNameFace *family;
	// 2004:160 LPR: can't do it in one name
	TFirmNameFace *firmName;
	TDamnNameFace *damnName;
	TSenderNameFace *senderName;
	// 2004:162+ LPR: universal
	const char *ucnTypes;
	char uxnTypes[SIZE_OF_UXN_TYPES];
	TNameFace *name;
	const char *involvements;
	TCharAliasFace *involvement;	// 2004:166 From select uxn ti
	TCharAliasFace *fromSide;
	TButton *ok;			// 2004:168 LRQ: for en/disable
	TStatic *reNameLabel;
	TStatic *familyLabel;
	TButton *search;			// 2004:173 LPR: to set as default
	// 2004:253 VRQ: for search/ident
	TBulStatFace *bulstat;
	TStatic *bulstatLabel;
	TButton *research;
	// 2004:323 LPR: for known persons
	TStatic *fromSideLabel;
	char lastUXNType;			// 2008:056 LPR: new search
	TLNCFace *lnc;
	TStatic *lncLabel;

	DEFINE_GROUP_TYPE(TFoundGroup, UCNGroup)
	bool Search(bool extra);
	void UpdateButtons(bool forceSearch);	// 2008:056 LPR: new search
	virtual bool UCNAvailable();			// 2016:314 for multi select

//{{TSelectUCNBaseDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSelectUCNBaseDialogVIRTUAL_END}}

//{{TSelectUCNBaseDialogRSP_TBL_BEGIN}}
	void PrintBNClicked();
	void UXNTypeCBNSelChange();
	void InvolvementCBNSelChange();
	void UCNENUpdate();
//{{TSelectUCNBaseDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectUCNBaseDialog);
};    //{{TSelectUCNBaseDialog}}
