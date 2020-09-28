//{{TDialog = TFloatJurorDialog}}
class TFloatJurorDialog : public TFloatDialog
{
public:
	TFloatJurorDialog(TWindow* parent, TFloatJurorGroup *group, int resId = IDD_FLOAT_JUROR);

protected:
	TRadioFace *load;
	TRadioFace *redistrib;
	TCheckFace *manualRed;
	TCharListFace *lawKinds;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TStatic *compositionsLabel;
	TCharListFace *compositions;
	TCharListFace *sesKinds;
	TStatic *dateLabel;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCheckFace *important;
	TCheckFace *limited;
	TUserListBox *sesJudges;
	TKnownPersonListBox *jurors;
	TRadioFace *loadFlat;
	TCheckFace *external;	// 2017:072
	TCharListFace *types;	// 2017:073

	bool Load();
	bool External();

	DEFINE_GROUP_TYPE(TFloatJurorGroup, FloatJurorGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

//{{TFloatJurorDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFloatJurorDialogVIRTUAL_END}}

//{{TFloatJurorDialogRSP_TBL_BEGIN}}
	void LoadBNClicked();
	void ExternalBNClicked();
//{{TFloatJurorDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFloatJurorDialog);
};    //{{TFloatJurorDialog}}
