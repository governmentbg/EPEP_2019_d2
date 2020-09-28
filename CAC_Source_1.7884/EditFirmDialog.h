//{{TDialog = TEditFirmDialog}}
class TEditFirmDialog : public TEditJudicialDialog
{
public:
	TEditFirmDialog(TWindow* parent, TFirm *firm, bool edit, bool full, bool stat);

protected:
	TCheckFace *backup;
	TOldFirm oldFirm;		// 2013:170 LPR: +related: copied from citizen

	bool CommitBackup();

//{{TEditFirmDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditFirmDialogVIRTUAL_END}}

//{{TEditFirmDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditFirmDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditFirmDialog);
};    //{{TEditFirmDialog}}
