//{{TDialog = TGetYearDialog}}
class TGetYearDialog : public TGroupDialog
{
public:
	TGetYearDialog(TWindow* parent, TGroup *group, int resId = IDD_GET_YEAR);

protected:
	TYearFace *year;

	virtual bool IsValid();

//{{TGetYearDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TGetYearDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetYearDialog);
};    //{{TGetYearDialog}}
