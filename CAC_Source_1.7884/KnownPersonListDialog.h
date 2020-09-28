//{{TDialog = TKnownPersonListDialog}}
class TKnownPersonListDialog : public TGroupListDialog
{
public:
	TKnownPersonListDialog(TWindow* parent, int resId = IDD_KNOWN_PERSON_LIST);

protected:
	TCharAliasFace *involvement;
	TKnownPersonListWindow *persons;

	// 2005:103 LPR: prosecutor and juror support
	TButton *active;
	TButton *registry;	// 2010:267 URQ: registry or pedagog
	TButton *import;
	TButton *export;
	TButton *load;
	
	void ActiveChanged();

//{{TKnownPersonListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TKnownPersonListDialogVIRTUAL_END}}

//{{TKnownPersonListDialogRSP_TBL_BEGIN}}
	void InvolvementCBNSelChange();
	void PersonsLVNItemChanged(TLwNotify &lwn);
	void RegistryBNClicked();
	void ListBNClicked();
	void RightBNClicked();
	void ActiveBNClicked();
	void ImportBNClicked();
	void ExportBNClicked();
	void RegistryBNEnable(TCommandEnabler &tce);
	void LoadBNClicked();
	void AddressBNClicked();
//{{TKnownPersonListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TKnownPersonListDialog);
};    //{{TKnownPersonListDialog}}
