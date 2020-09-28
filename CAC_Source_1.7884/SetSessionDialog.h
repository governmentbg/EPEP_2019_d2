//{{TDialog = TSetSessionDialog}}
class TSetSessionDialog : public TSetSessionBaseDialog
{
public:
	TSetSessionDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, const char *kinds, TLoadGroup *tLoadGroup,
		TLoadGroup *tInvisible);

protected:
	bool finished;
	// 2005:108 end date/time support
	TLoadGroup *loadGroup;		// 2006:338 load hall support
	TCheckBox *setReserve;		// 2007:220 2nd date option
	TCheckBox *jurors;		// 2009:217
	TLoadGroup *invisible;
	bool jurorEnabled[3];
	char jurorBackup[3][SIZE_OF_UCN];

	DEFINE_GROUP_TYPE(TSession, Session)
	virtual bool IsValid();

//{{TSetSessionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSetSessionDialogVIRTUAL_END}}

//{{TSetSessionDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void JurorsBNClicked();
//{{TSetSessionDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSetSessionDialog);
};    //{{TSetSessionDialog}}
