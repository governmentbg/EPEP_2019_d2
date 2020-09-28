//{{TDialog = TArticleListDialog}}
class TArticleListDialog : public TGroupListDialog
{
public:
	TArticleListDialog(TWindow* parent);

protected:
	TArticleListWindow *articles;

//{{TArticleListDialogRSP_TBL_BEGIN}}
	void SubjectBNClicked();
	void PrintBNClicked();
//{{TArticleListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TArticleListDialog);
};    //{{TArticleListDialog}}
