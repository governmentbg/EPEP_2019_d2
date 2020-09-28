DEFINE_RESPONSE_TABLE1(TArticleListDialog, TGroupListDialog)
//{{TArticleListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDSUBJECT, SubjectBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_COMMAND_ENABLE(IDSUBJECT, SelectEnable),
//{{TArticleListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TArticleListDialog Implementation}}

TArticleListDialog::TArticleListDialog(TWindow* parent)
:
	TGroupListDialog(parent, IDD_ARTICLE_LIST)
{
	articles = new TArticleListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TArticleListDialog::SubjectBNClicked()
{
	articles->Subject();
}

void TArticleListDialog::PrintBNClicked()
{
	articles->Print();
}
