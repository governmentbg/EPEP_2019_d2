//{{TDialog = TSubjectRealityTypesDialog}}
class TSubjectRealityTypesDialog : public TGroupDialog
{
public:
	TSubjectRealityTypesDialog(TWindow* parent, TSubject *subject, int resId = IDD_SUBJECT_REALITY_TYPES);

protected:
	// 2007:213 LPR: uniclass +related
	TCharRealityType charReasonType;
	TCharRealityType charPenalityType;

	DEFINE_GROUP_TYPE(TSubject, Subject)

//{{TSubjectRealityTypesDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TSubjectRealityTypesDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSubjectRealityTypesDialog);
};    //{{TSubjectRealityTypesDialog}}
