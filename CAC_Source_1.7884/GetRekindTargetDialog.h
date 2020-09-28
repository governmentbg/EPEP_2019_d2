//{{TDialog = TGetRekindTargetDialog}}
class TGetRekindTargetDialog : public TScaleDialog
{
public:
	TGetRekindTargetDialog(TWindow* parent, TRequest *tRequest, TLawsuit *tLawsuit);

protected:
	TRequest *request;
	TLawsuit *lawsuit;
	mstr requestKinds;	// 2017:087 char[] -> mstr
	// 2017:087
	mstr lawKinds;
	const char *types;

	TStatic *header;
	TCharAliasFace *requestKind;
	TLongFace *requestNo;
	TYearFace *requestYear;
	TCharAliasFace *lawsuitKind;
	TSubjectAliasFace *subject;
	TLongFace *lawsuitNo;
	TYearFace *lawsuitYear;
	TGetContainerAliasFace *eventKind;
	TDateFace *eventDate;
	TIndexFace *eventIndex;

	virtual bool IsValid();

//{{TGetRekindTargetDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetRekindTargetDialogVIRTUAL_END}}

//{{TGetRekindTargetDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void OKBNEnable(TCommandEnabler &tce);
	void RequestKindCBNSelChange();
	void LawsuitKindCBNSelChange();
	void EventKindCBNSelChange();
//{{TGetRekindTargetDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetRekindTargetDialog);
};    //{{TGetRekindTargetDialog}}
