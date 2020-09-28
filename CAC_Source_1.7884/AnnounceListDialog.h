//{{TDialog = TAnnounceListDialog}}
class TAnnounceListDialog : public TGroupListDialog
{
public:
	TAnnounceListDialog(TWindow* parent, TAnnounceGroup *tGroup);

protected:
	TAnnounceGroup *group;
	TStatic *dateLabel;
	TDateFace *date;
	TReceiverAliasFace *sender;
	TDelivererListBox *deliverers;
	TAnnounceListWindow *announces;
	// 2010:004 LPR: +internal
	char types[SIZE_OF_TYPES];
	TCharAliasFace *type;
	TStatic *dropLabel;
	// 2010:005 LPR: default button
	TButton *search;
	TButton *add, *edit;
	TButton *assign;		// 2010:035
	TCheckFace *delivered;
	TStringFace *findText;
	TButton *export;		// 2012:040; other courts: import
	TButton *import;		// 2018-03-19: separate

	void SetSearch();
	void Add(bool copy);	// 2010:340 +related

//{{TAnnounceListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAnnounceListDialogVIRTUAL_END}}

//{{TAnnounceListDialogRSP_TBL_BEGIN}}
	void DropCBNSelChange();
	void PrevBNClicked();
	void NextBNClicked();
	void FindBNEnable(TCommandEnabler &tce);
	void SearchBNClicked();
	void AddBNClicked();
	void CopyBNClicked();
	void EditBNClicked();
	void ServeBNClicked();
	void AssignBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	void AssignBNEnable(TCommandEnabler &tce);
	long EvFocusFace(TParam1 wParam, TParam2);
	void ExportBNEnable(TCommandEnabler &tce);
	void ExportBNClicked();
	void ImportBNEnable(TCommandEnabler &tce);
	void ImportBNClicked();
//{{TAnnounceListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAnnounceListDialog);
};    //{{TAnnounceListDialog}}
