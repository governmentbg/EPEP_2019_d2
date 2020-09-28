//{{TDialog = TPrepareNIAProtocolDialog}}
class TPrepareNIAProtocolDialog : public TFloatDialog
{
public:
	TPrepareNIAProtocolDialog(TWindow* parent, TPrepareNIAProtocolGroup *group, int resId = IDD_PREPARE_NIA_PROTOCOL);

protected:
	TNIATDepAliasFace *niaTDep;
	TStatic *typesLabel;
	TCharListFace *types;
	TRadioFace *byExecDate;
	TRadioFace *byOutRegDate;
	TDateFace *minExecDate;
	TDateFaceUpDown *minExecDateUpDn;
	TDateFace *maxExecDate;
	TDateFaceUpDown *maxExecDateUpDn;
	TDateFace *minOutRegDate;
	TDateFaceUpDown *minOutRegDateUpDn;
	TDateFace *maxOutRegDate;
	TDateFaceUpDown *maxOutRegDateUpDn;

	DEFINE_GROUP_TYPE(TPrepareNIAProtocolGroup, PrepareGroup)
	virtual bool IsValid();

//{{TPrepareNIAProtocolDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TPrepareNIAProtocolDialogVIRTUAL_END}}

//{{TPrepareNIAProtocolDialogRSP_TBL_BEGIN}}
	void ByBNClicked();
	void OKBNClicked();
//{{TPrepareNIAProtocolDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPrepareNIAProtocolDialog);
};    //{{TPrepareNIAProtocolDialog}}
