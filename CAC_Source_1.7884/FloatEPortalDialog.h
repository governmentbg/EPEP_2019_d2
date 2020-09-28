//{{TDialog = TFloatEPortalDialog}}
class TFloatEPortalDialog : public TFloatDialog
{
public:
	TFloatEPortalDialog(TWindow* parent, TFloatEPortalGroup *group);

protected:
	TRadioFace *log;
	TRadioFace *guid;
	TStatic *logDateLabel;
	TDateFace *minLogDate;
	TDateFace *maxLogDate;
	TDateFace *minGuidDate;
	TDateFace *maxGuidDate;
	TWhatListBox *whats;
	TEPorTypeListFace *types;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;

	DEFINE_GROUP_TYPE(TFloatEPortalGroup, FloatEPortalGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

//{{TFloatEPortalDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFloatEPortalDialogVIRTUAL_END}}

//{{TFloatEPortalDialogRSP_TBL_BEGIN}}
	void InputBNClicked();
//{{TFloatEPortalDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFloatEPortalDialog);
};    //{{TFloatEPortalDialog}}
