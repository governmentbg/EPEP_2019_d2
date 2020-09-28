//{{TArticleListWindow Implementation}}

TArticleListWindow::TArticleListWindow(TWindow* parent, int resourceId)
:
	TGroupListWindow(parent, resourceId, new TLongAliasArray, 172)
{
	Add(
		new TGroupListColumn("", group_inactive, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("���", "F_NAME", 148),
		NULL
	);
}

void TArticleListWindow::Subject()
{
	subject_article(this, (TArticle *) Group());
}

void TArticleListWindow::Print()
{
	print_realticles((TLongAliasArray *) array, "�������� �� ���������");
}

TGroup *TArticleListWindow::NewGroup() const
{
	return new TArticle;
}

bool TArticleListWindow::AddGroup(TGroup *group)
{
	return edit_article(this, (TArticle *) group, false);
}

bool TArticleListWindow::EditGroup(TGroup *group)
{
	return edit_article(this, (TArticle *) group, true);
}
