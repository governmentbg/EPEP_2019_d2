//{{TDialog = TSelectSurroundsDialog}}
class TSelectSurroundsDialog : public TQuickDialog
{
public:
	TSelectSurroundsDialog(TWindow* parent, TGArray<TSurround> *tPossibles, TGArray<TSurround> *tSelecteds,
		bool tShowReloaded);

protected:
	TGArray<TSurround> *possibles;
	TGArray<TSurround> *selecteds;
	bool showReloaded;

	TSelectSurroundsListBox *surrounds;
	TStatic *reloaded;

//{{TSelectSurroundsDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSelectSurroundsDialogVIRTUAL_END}}

//{{TSelectSurroundsDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TSelectSurroundsDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectSurroundsDialog);
};    //{{TSelectSurroundsDialog}}
