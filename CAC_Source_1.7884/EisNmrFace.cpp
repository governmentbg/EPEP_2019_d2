//{{TEisNmrFace Implementation}}

TEisNmrFace::TEisNmrFace(TWindow* parent, int resourceId, const char *name, char letter)
:
	TStringFace(parent, resourceId, name, SIZE_OF_EISPP_NMR), startHere(false)
{
	SetValidator(new TEisNmrValidator(letter));
}

void TEisNmrFace::SetStartHere(bool tStartHere)
{
	startHere = tStartHere;
}

void TEisNmrFace::SetKinds(char requestKind, char lawsuitKind, bool has_eispp_nmr)
{
	startHere = strchr(KIND_START_HERE_INDOCS, requestKind);

	if (has_eispp_nmr)
		EnableWindow(false);
	else
	{
		Kind->Seek(lawsuitKind);

		if (EISPP_ACTIVE && Kind->eispp)
			EnableWindow(true);
		else
		{
			EnableWindow(false);
			SetS("");
		}
	}
}

bool TEisNmrFace::IsValid()
{
	bool isValid = TStringFace::IsValid();

	if (isValid)
	{
		const char *nmr = S();

		if (any(nmr) && startHere && strncmp(nmr, COURT_TLACR, 3))
			isValid = error("ЕИСПП номера не започва с кода на текущия съд.");
	}

	return isValid;
}
