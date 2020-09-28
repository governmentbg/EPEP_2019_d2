//{{TDialog = TEditPneDialog}}
class TEditPneDialog : public TEisDialog
{
public:
	TEditPneDialog(TWindow* parent, PNE *pne, bool edit, int tPneMode, const char *nmrnpr, int resId = IDD_EDIT_PNE);

protected:
	int pneMode;
	TEisNmrFace *nprnmr;
	TEisNmrFace *nmr;
	TEisAliasFace *otdtip;
	TStatic *dtaotdLabel;
	TDateFace *dtaotd;
	TDateFace *dtadod;
	TStatic *kcqLabel;
	TStringFace *kcqName;
	TEisCrimeListBox *kcq;
	TEisAliasFace *stpdvs;
	TEisAliasFace *sts;
	TButton *ok;
	TButton *search;

	DEFINE_GROUP_TYPE(PNE, Pne)
	virtual bool IsValid();

//{{TEditPneDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditPneDialogVIRTUAL_END}}

//{{TEditPneDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void SearchBNClicked();
	void NameENSetFocus();
	void NameENKillFocus();
	void AddressBNClicked();
//{{TEditPneDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditPneDialog);
};  //{{TEditPneDialog}}
