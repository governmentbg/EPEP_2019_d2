//{{TDialog = TGetObjectCountDialog}}
class TGetObjectCountDialog : public TGroupDialog
{
public:
	TGetObjectCountDialog(TWindow* parent, TObjectCountGroup *group);

protected:
	TRadioFace *byYears;
	TRadioFace *byDates;
	TYearFace *minYear;
	TYearFace *maxYear;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCheckFace *logging;
	TCheckFace *attached;

	DEFINE_GROUP_TYPE(TObjectCountGroup, ObjectGroup)
	virtual bool IsValid();

//{{TGetObjectCountDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetObjectCountDialogVIRTUAL_END}}

//{{TGetObjectCountDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void ByYearsBNClicked();
//{{TGetObjectCountDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetObjectCountDialog);
};    //{{TGetObjectCountDialog}}
