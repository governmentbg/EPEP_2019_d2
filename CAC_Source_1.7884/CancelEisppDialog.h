//{{TDialog = TCancelEisppDialog}}
class TCancelEisppDialog : public TScaleDialog
{
public:
	TCancelEisppDialog(TWindow* parent, const char *tNprnmr, const char *tXmlName, const char *tErrName,
		const char *tTmpName, long tTimeout, int resId = IDD_CANCEL_EISPP);

protected:
	const char *nprnmr;
	const char *xmlName;
	const char *errName;
	const char *tmpName;
	long timeout;
	long counter;
	TStatic *text;
	TPushButton *cancel;

//{{TCancelEisppDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TCancelEisppDialogVIRTUAL_END}}

//{{TCancelEisppDialogRSP_TBL_BEGIN}}
	void EvTimer(uint timerId);
//{{TCancelEisppDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TCancelEisppDialog);
};  //{{TCancelEisppDialog}}
