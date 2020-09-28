//{{TLocalSurroundmentDialog Implementation}}

TLocalSurroundmentDialog::TLocalSurroundmentDialog(TWindow* parent, TSurroundment *surroundment, TSession *tSession,
	TDecision *tDecision)
:
	TEditDialog(parent, surroundment, true, IDD_LOCAL_SURROUNDMENT), session(tSession), decision(tDecision)
{
	new TCharAliasFace(this, IDC_LOCAL_SURROUNDMENT_KIND, "F_KIND", Kind, NULL);
	new TLongFace(this, IDC_LOCAL_SURROUNDMENT_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_LOCAL_SURROUNDMENT_YEAR, "F_YEAR");
	new TDateFace(this, IDC_LOCAL_SURROUNDMENT_DATE, "F_DATE");
	new TCharAliasFace(this, IDC_LOCAL_SURROUNDMENT_RESULT, "F_RESULT", Result, kind2SurroundmentResults(decision->kind));
	new TStringFace(this, IDC_LOCAL_SURROUNDMENT_TEXT, "F_TEXT", SIZE_OF_SURROUNDMENT_TEXT);
}
