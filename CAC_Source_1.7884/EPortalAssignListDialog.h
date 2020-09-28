//{{TDialog = TEPortalAssignListDialog}}
class TEPortalAssignListDialog : public TGroupListDialog
{
public:
	TEPortalAssignListDialog(TWindow* parent, TInReg *tInReg, const TSideWind *tSideWind, bool tEdit);

protected:
	TInReg *inReg;
	TEPortalAssignListWindow *assigns;
	TPushButton *eportalSubpoena;
	bool edit;

	TStatic *header1;
	TStatic *header2;

//{{TEPortalAssignListDialogVIRTUAL_BEGIN}}
	virtual bool CanClose();
	virtual void SetupWindow();
//{{TEPortalAssignListDialogVIRTUAL_END}}

//{{TEPortalAssignListDialogRSP_TBL_BEGIN}}
	void DeleteBNEnable(TCommandEnabler &tce);
	void CancelBNClicked();
	void EvClose();
	void EPortalSubpoenaBNEnable(TCommandEnabler &tce);
	void EPortalSubpoenaBNClicked();
//{{TEPortalAssignListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEPortalAssignListDialog);
};    //{{TEPortalAssignListDialog}}
