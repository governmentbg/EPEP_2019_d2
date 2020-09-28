//{{TDialog = TMovementDialog}}
class TMovementDialog : public TGroupDialog
{
public:
	TMovementDialog(TWindow* parent, TMovement *movement, TRCDKeyGroup *group, int resId = IDD_IN_OUT_MOVEMENT);

protected:
	// 2006:016 LPR: Unify in/out
	TRCDKeyGroup *group;
	TCharAliasFace *kind;

	TDateFace *date;
	TStringFace *text;
	TTimeFace *time;

	DEFINE_GROUP_TYPE(TMovement, Movement)
	virtual bool IsValid();

//{{TMovementDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TMovementDialogVIRTUAL_END}}

//{{TMovementDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TMovementDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TMovementDialog);
};    //{{TMovementDialog}}
