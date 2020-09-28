//{{TDialog = TSetSessionBaseDialog}}
class TSetSessionBaseDialog : public TSessionCompositionDialog
{
public:
	TSetSessionBaseDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, const char *kinds, bool edit,
		int resId);

protected:
	// 2007:220 LPR/LRQ: +related: 1 session only
	TDateFace *date;
	TTimeFace *time;
	TLongAliasFace *hall;
	TCharAliasFace *kind;
	// 2005:108 end date/time support
	TDateFace *end;
	TTimeFace *entime;

	DEFINE_GROUP_TYPE(TSession, Session)
	virtual bool IsValid();

//{{TSetSessionBaseDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSetSessionBaseDialogVIRTUAL_END}}

//{{TSetSessionBaseDialogRSP_TBL_BEGIN}}
	void JudgeCBNSelChange();
//{{TSetSessionBaseDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSetSessionBaseDialog);
};    //{{TSetSessionBaseDialog}}
