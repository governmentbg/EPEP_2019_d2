//{{TAddSubjectDialog Implementation}}

TAddSubjectDialog::TAddSubjectDialog(TWindow* parent, TSubject *tSubject, const char *kinds)
:
	TEditDialog(parent, tSubject, false, IDD_ADD_SUBJECT)
{
	new TLongFace(this, IDC_ADD_SUBJECT_SUBJECT, "F_SUBJECT_LONG", SIZE_OF_SUBJECT_SUBJECT);
	new TCharAliasFace(this, IDC_ADD_SUBJECT_KIND_I, "F_KIND_I", Kind,
		tSubject->saas ? kinds : C2S[tSubject->sd[0].kind], tSubject->saas ? ALIAS_EMPTY : FLAG_NULL);
	new TCharAliasFace(this, IDC_ADD_SUBJECT_KIND_II, "F_KIND_II", Kind,
		tSubject->saas ? C2S[tSubject->sd[1].kind] : kinds, tSubject->saas ? FLAG_NULL : ALIAS_EMPTY);
	new TStringFace(this, IDC_ADD_SUBJECT_NAME, "F_NAME", SIZE_OF_SUBJECT_NAME);
}
