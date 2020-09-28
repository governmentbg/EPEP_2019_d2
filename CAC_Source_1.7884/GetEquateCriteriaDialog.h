//{{TDialog = TGetEquateCriteriaDialog}}
class TGetEquateCriteriaDialog : public TGroupDialog
{
public:
	TGetEquateCriteriaDialog(TWindow* parent, TEquateGroup *group, int resId = IDD_GET_EQUATE_CRITERIA);

protected:
	TCharListFace *kinds;

//{{TGetEquateCriteriaDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TGetEquateCriteriaDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetEquateCriteriaDialog);
};    //{{TGetEquateCriteriaDialog}}
