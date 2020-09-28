//{{TDialog = TGetLoadCriteriaDialog}}
class TGetLoadCriteriaDialog : public TPeriodDialog
{
public:
	TGetLoadCriteriaDialog(TWindow* parent, TLoadGroup *loadGroup, TLoadGroup *tInvisible,
		int resId = IDD_GET_LOAD_CRITERIA);

protected:
	THallListBox *halls;
	TLoadGroup *invisible;

	DEFINE_GROUP_TYPE(TLoadGroup, LoadGroup)
	virtual bool IsValid();

//{{TGetLoadCriteriaDialogRSP_TBL_BEGIN}}
	void OKBNEnable(TCommandEnabler &tce);
	void OKBNClicked();
//{{TGetLoadCriteriaDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetLoadCriteriaDialog);
};    //{{TGetLoadCriteriaDialog}}
