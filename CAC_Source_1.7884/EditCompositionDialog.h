//{{TDialog = TEditCompositionDialog}}
class TEditCompositionDialog : public TGroupDialog
{
public:
	TEditCompositionDialog(TWindow* parent, TFineComposition *tComposition, int resId = IDD_EDIT_COMPOSITION);

protected:
	TPersonAliasFace *judge;
	TPersonAliasFace *secretar;

	TPersonAliasFace *judgeX;
	TPersonAliasFace *reJudgeX;

	TPersonAliasFace *president;
	TStringFace *name;		// 2008:254

	DEFINE_GROUP_TYPE(TFineComposition, Composition)
	virtual bool IsValid();

//{{TEditCompositionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditCompositionDialogVIRTUAL_END}}

//{{TEditCompositionDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditCompositionDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditCompositionDialog);
};    //{{TEditCompositionDialog}}