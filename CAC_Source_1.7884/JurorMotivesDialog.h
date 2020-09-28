//{{TDialog = TJurorMotivesDialog}}
class TJurorMotivesDialog : public TWhateverTextBaseDialog
{
public:
	TJurorMotivesDialog(TWindow* parent, const char *title, char *tMotives, int resId = IDD_JUROR_MOTIVES);

protected:
	char *motives;
	mstr m;
	TText field;

	virtual bool IsValid();

//{{TJurorMotivesDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TJurorMotivesDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TJurorMotivesDialog);
};    //{{TJurorMotivesDialog}}
