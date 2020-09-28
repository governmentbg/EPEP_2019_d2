//{{TUnwrittenActDialog Implementation}}

TUnwrittenActDialog::TUnwrittenActDialog(TWindow* parent, TUnwrittenActGroup *group)
:
	TUnwrittenDialog(parent, group, "Дела", IDD_UNWRITTEN_ACT)
{
	kinds = new TCharListFace(this, IDC_UNWRITTEN_ACT_KINDS, "F_KINDS", Kind, KIND_LAWSUITS, SIZE_OF_KINDS);

	new TRadioFace(this, IDC_UNWRITTEN_ACT_BY_JTYN, "F_BY_JTYN");
	new TRadioFace(this, IDC_UNWRITTEN_ACT_BY_TYN, "F_BY_TYN");
	new TRadioFace(this, IDC_UNWRITTEN_ACT_BY_TSYN, "F_BY_TSYN");
	new TRadioFace(this, IDC_UNWRITTEN_ACT_BY_TKYN, "F_BY_TKYN");
}

void TUnwrittenActDialog::Query(const char *criteria, const char *title)
{
	TUnwrittenActQuery(this, criteria, title, &UnwrittenActGroup()->date).Execute();
}
