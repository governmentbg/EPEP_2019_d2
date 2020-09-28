//{{TDialog = TFloatElectricDialog}}
class TFloatElectricDialog : public TFloatDialog
{
public:
	TFloatElectricDialog(TWindow* parent, TFloatElectricGroup *group, int resId = IDD_FLOAT_ELECTRIC);

protected:
	char types[SIZE_OF_TYPES];
	TCharAliasFace *type;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TStatic *minDateLabel;
	TStatic *finishedLabel;
	TDateFace *minDate;
	TStatic *maxDateLabel;
	TDateFace *maxDate;
	TRadioFace *document;
	TRadioFace *sheetable;
	TRadioFace *missing;

	DEFINE_GROUP_TYPE(TFloatElectricGroup, FloatElectricGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

//{{TFloatElectricDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFloatElectricDialogVIRTUAL_END}}

//{{TFloatElectricDialogRSP_TBL_BEGIN}}
	void TypeCBNSelChange();
	void VariantBNClicked();		
//{{TFloatElectricDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFloatElectricDialog);
};    //{{TFloatElectricDialog}}
