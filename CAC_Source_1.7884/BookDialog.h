//{{TDialog = TBookDialog}}
class TBookDialog : public TFloatDialog
{
public:
	TBookDialog(TWindow* parent, TBookGroup *group, TCharAliasGroup *tType, int resId, bool tAliasFace = false);
	virtual ~TBookDialog();

protected:
	bool aliasFace;
	bool aliasType;
	TCharListFace *types;
	TCharAliasFace *type; 	// aliasType
	TRadioFace *byNoYear;
	TRadioFace *byPeriod;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *year;
	TDateFace *minDate;
	TDateFaceUpDown *minDateUpDn;
	TDateFace *maxDate;
	TDateFaceUpDown *maxDateUpDn;

	DEFINE_GROUP_TYPE(TBookGroup, BookGroup)
	virtual bool IsValid();

//{{TBookDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TBookDialogVIRTUAL_END}}

//{{TBookDialogRSP_TBL_BEGIN}}
	void ByBNClicked();
//{{TBookDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TBookDialog);
};    //{{TBookDialog}}
