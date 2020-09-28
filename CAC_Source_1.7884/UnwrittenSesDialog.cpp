//{{TUnwrittenSesDialog Implementation}}

TUnwrittenSesDialog::TUnwrittenSesDialog(TWindow* parent, TUnwrittenSesGroup *group)
:
	TUnwrittenDialog(parent, group, "Заседания", IDD_UNWRITTEN_SES)
{
	types = new TCharListFace(this, IDC_UNWRITTEN_SES_TYPES, "F_TYPES", Type, TYPE_LAWSUITS, SIZE_OF_TYPES);
	kinds = new TCharListFace(this, IDC_UNWRITTEN_SES_KINDS, "F_KINDS", Kind, KIND_VISIBLE_SESSIONS, SIZE_OF_KINDS);
	new TRadioFace(this, IDC_UNWRITTEN_SES_BY_SES_DATE, "F_BY_SES_DATE");
	new TRadioFace(this, IDC_UNWRITTEN_SES_BY_ACT_DATE, "F_BY_ACT_DATE");
}

void TUnwrittenSesDialog::Query(const char *criteria, const char *title)
{
	const CDate &maxActDate = UnwrittenSesGroup()->byActDate ? UnwrittenSesGroup()->date : DATE_MAX;
	TUnwrittenSesQuery(this, criteria, title, maxActDate).Execute();
}
