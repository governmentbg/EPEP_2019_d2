//{{TDialog = TEditDangerDialog}}
class TEditDangerDialog : public TGroupDialog
{
public:
	TEditDangerDialog(TWindow* parent, TDanger *tDanger);

protected:
	TLongFace *danger;
	TCheckFace *active;

//{{TEditDangerDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditDangerDialogVIRTUAL_END}}

//{{TEditDangerDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditDangerDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditDangerDialog);
};    //{{TEditDangerDialog}}