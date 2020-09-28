//{{TDialog = TQueryUnattachedDialog}}
class TQueryUnattachedDialog : public TGroupDialog
{
public:
	TQueryUnattachedDialog(TWindow* parent, TQueryUnattachedGroup *group, int resId = IDD_QUERY_UNATTACHED);

protected:
	TDateFace *minDate;
	TDateFace *maxDate;
	TUserListBox *judges;

	DEFINE_GROUP_TYPE(TQueryUnattachedGroup, QueryUnattachedGroup);
	virtual bool IsValid();

//{{TQueryUnattachedDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TQueryUnattachedDialogVIRTUAL_END}}

//{{TQueryUnattachedDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TQueryUnattachedDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TQueryUnattachedDialog);
};    //{{TQueryUnattachedDialog}}
