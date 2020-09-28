//{{TDialog = TWhateverTextViewDialog}}
class TWhateverTextViewDialog : public TWhateverTextBaseDialog
{
public:
	TWhateverTextViewDialog(TWindow* parent, TGroup *group, const char *title, const char *what, int size, TDatedContainer *tElectric, int resId = IDD_WHATEVER_TEXT_VIEW);
	TWhateverTextViewDialog(TWindow* parent, TText *field, const char *title, const char *what, int size, TDatedContainer *tElectric, int resId = IDD_WHATEVER_TEXT_VIEW);

protected:
	TDatedContainer *electric;

//{{TWhateverTextViewDialogRSP_TBL_BEGIN}}
	void ViewBNClicked();
//{{TWhateverTextViewDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TWhateverTextViewDialog);
};    //{{TWhateverTextViewDialog}}
