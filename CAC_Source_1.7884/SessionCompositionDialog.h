//{{TDialog = TSessionCompositionDialog}}
class TSessionCompositionDialog : public TTroubleDialog
{
public:
	TSessionCompositionDialog(TWindow* parent, TBaseSession *session, TLawsuit *tLawsuit, long edit, int resId);

protected:
	TLawsuit *lawsuit;
	mstr posts;

	TPersonAliasFace *president;
	TPersonAliasFace *judgeX;
	TPersonAliasFace *reJudgeX;
	TPersonAliasFace *judge;
	TPersonAliasFace *prosecutor;
	TPersonAliasFace *secretar;
	TLongAliasFace *composition;	// 2017:075 from all derived

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	TJurorDistrib distrib;		// 2009:220
#endif  // AREAL || MILITARY || REGIONAL
	// 2010:167 jurors moved here from open result
	TPersonAliasFace *juror;
	TPersonAliasFace *reJuror;
	TPersonAliasFace *jurorX;
	TPersonAliasFace *reserve;	// 2017:172 +related
	bool jurors;

	TPersonAliasFace *JurorFace(int resourceId, const char *name, const char *ucn0, const char *ucn1, const char *include);
	TPersonAliasFace *PostFace(int resourceId, const char *name, char post);
	void AdjustPresident();

	DEFINE_GROUP_TYPE(TBaseSession, Session)
	virtual bool IsValid();

//{{TSessionCompositionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSessionCompositionDialogVIRTUAL_END}}
};    //{{TSessionCompositionDialog}}
