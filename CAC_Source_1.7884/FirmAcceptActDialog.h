//{{TDialog = TFirmAcceptActDialog}}
class TFirmAcceptActDialog : public TJudicialActDialog
{
public:
	TFirmAcceptActDialog(TWindow* parent, TDecision *decision, TLawsuit *tLawsuit, const char *kinds, TSession *session);

protected:
	TDateFace *enterDate;
	TLongFace *registry;
	TLongFace *level;
	TCheckFace *contract;		// 2005:060

	virtual bool IsValid();	// JIT checks to reduce partial writes
	void ResultChanged();

//{{TFirmAcceptActDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFirmAcceptActDialogVIRTUAL_END}}
};    //{{TFirmAcceptActDialog}}
