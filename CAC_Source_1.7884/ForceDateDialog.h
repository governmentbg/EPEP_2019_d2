//{{TDialog = TForceDateDialog}}
class TForceDateDialog : public TEditDialog
{
public:
	TForceDateDialog(TWindow* parent, TIndexContainer *container, TLawsuit *tLawsuit, const char *tWhat);

protected:
	TLawsuit *lawsuit;
	constant what;		// 2012:083
	TCharAliasFace *kind;
	TDateFace *forceDate;
	TDateFace *lawsuitForceDate;	// 2008:039 former forceDate
	TDateFace *date;			// 2007:241 For forceDate checks

	virtual bool IsValid();

//{{TForceDateDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TForceDateDialogVIRTUAL_END}}
};    //{{TForceDateDialog}}
