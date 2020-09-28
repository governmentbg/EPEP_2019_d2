//{{TDialog = TStandardActDialog}}
class TStandardActDialog : public TJudicialActDialog
{
public:
	TStandardActDialog(TWindow* parent, TDecision *decision, TLawsuit *lawsuit, const char *kinds, bool grant, bool flags,
		TSession *session);

protected:
	TButton *returnedType;
	TButton *motives;
	TCheckFace *confess;
	TCheckFace *absence;
	TButton *opinion;		// 2009:149 +related
	TStatic *motivesLabel;	// 2009:349 motiving
	TKeyWordsFace *keyWords;

	virtual void SetMotiving(bool tMotiving);
	virtual void KindChanged();

//{{TStandardActDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TStandardActDialogVIRTUAL_END}}

//{{TStandardActDialogRSP_TBL_BEGIN}}
	void MotivesBNClicked();
	void ConfessBNClicked();
	void AbsenceBNClicked();
	void OpinionBNClicked();
	void ReturnedTypeBNClicked();
//{{TStandardActDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TStandardActDialog);
};    //{{TStandardActDialog}}
