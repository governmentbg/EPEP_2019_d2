//{{TEditArticleDialog Implementation}}

TEditArticleDialog::TEditArticleDialog(TWindow* parent, TArticle *tArticle, bool edit, int resId)
:
	TEditDialog(parent, tArticle, edit, resId)
{
	article = new TLongFace(this, IDC_EDIT_ARTICLE_ARTICLE, "F_ARTICLE_LONG", SIZE_OF_ARTICLE);
	new TCheckFace(this, IDC_EDIT_ARTICLE_ACTIVE, "F_FLAGS_ACTIVE");
	new TStringFace(this, IDC_EDIT_ARTICLE_NAME, "F_NAME", SIZE_OF_NAME);
}

void TEditArticleDialog::SetupWindow()
{
	TEditDialog::SetupWindow();
	article->EnableWindow(!edit);
}
