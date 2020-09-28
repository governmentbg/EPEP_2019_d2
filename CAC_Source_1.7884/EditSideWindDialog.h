//{{TDialog = TEditSideWindDialog}}
class TEditSideWindDialog : public TGroupDialog
{
public:
	TEditSideWindDialog(TWindow* parent, TSideWind *sideWind, const char *involvements, bool tUnwanted, int resId = IDD_EDIT_SIDE_WIND);

protected:
	const char *involvements;
	TCharAliasFace *involvement;
	TCharAliasFace *fromSide;
	TStringFace *unwanted;

//{{TEditSideWindDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditSideWindDialogVIRTUAL_END}}

//{{TEditSideWindDialogRSP_TBL_BEGIN}}
	void InvolvementCBNSelChange();
//{{TEditSideWindDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditSideWindDialog);
};    //{{TEditSideWindDialog}}
