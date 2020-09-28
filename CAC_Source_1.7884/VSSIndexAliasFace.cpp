//{{TVSSIndexAliasFace Implementation}}

TVSSIndexAliasFace::TVSSIndexAliasFace(TWindow* parent, int resourceId, const char *name)
:
	TStringAliasFace(parent, resourceId, name, VSSIndex, NULL, FLAG_NULL), courtType('\0')
{
	loadData = false;
}

void TVSSIndexAliasFace::SetCrit(char tCourtType, const char *tTypes, char tKind, const CDate &tDate)
{
	courtType = tCourtType;
	types = tTypes;
	kind = tKind;
	date = tDate;
	Refresh();
}

void TVSSIndexAliasFace::Refresh()
{
	mstr m(S());

	array->Flush();
	array->Add(new TVSSIndex());
	TVSSIndexSet::AddMatching(*array, courtType, types, kind, date, date, false);
	TAliasFace::Refresh();
	SetS(str(m));
}
