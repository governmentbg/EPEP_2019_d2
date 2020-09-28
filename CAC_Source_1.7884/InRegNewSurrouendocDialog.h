//{{TDialog = TInRegNewSurrouendocDialog}}
class TInRegNewSurrouendocDialog : public TInRegNewBaseDialog
{
public:
	TInRegNewSurrouendocDialog(TWindow* parent, TInReg *inReg, const char *kinds, const TLawsuit *lawsuit,
		const TSession *tSession, const TSurroundment *tSurrouendoc);

protected:
	const TSession *session;
	const TSurroundment *surrouendoc;

//{{TInRegNewSurrouendocDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TInRegNewSurrouendocDialogVIRTUAL_END}}

//{{TInRegNewSurrouendocDialogRSP_TBL_BEGIN}}
	void KindCBNSelChange();
	void OKBNClicked();
//{{TInRegNewSurrouendocDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TInRegNewSurrouendocDialog);
};    //{{TInRegNewSurrouendocDialog}}