//{{TDialog = TFloatSurroundDialog}}
class TFloatSurroundDialog : public TFloatSubjectDialog
{
public:
	TFloatSurroundDialog(TWindow* parent, TFloatSurroundGroup *group);

protected:
	TTypeListFace *lawTypes;
	TLongFace *minLawNo;
	TLongFace *maxLawNo;
	TYearFace *minLawYear;
	TYearFace *maxLawYear;
	TUserListBox *judges;
	TCharListFace *kinds;
	TDateFace *minDate;
	TDateFace *maxDate;
	TDateFace *minSessionDate;
	TDateFace *maxSessionDate;
	TCheckFace *firmData;
	TStringFace *text;

	DEFINE_GROUP_TYPE(TFloatSurroundGroup, FloatSurroundGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

//{{TFloatSurroundDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFloatSurroundDialogVIRTUAL_END}}
};    //{{TFloatSurroundDialog}}
