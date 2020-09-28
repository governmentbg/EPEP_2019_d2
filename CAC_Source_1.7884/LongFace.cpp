//{{TLongFace Implementation}}

TLongFace::TLongFace(TWindow* parent, int resourceId, const char *name, uint textLimit)
:
	TStringFace(parent, resourceId, name, textLimit)
{
}

TLongFace::~TLongFace()
{
}

void TLongFace::SetupWindow()
{
	TStringFace::SetupWindow();
	// 2008:135 LPR: dynamic positive/negative filter
	SetValidator(new TLongValidator(IsWindowEnabled()));
}

long TLongFace::L() const
{
	S();
	return atol(buffer);
}

void TLongFace::SetL(long l)
{
	if (l)
		ltoa(l, buffer, 10);
	else
		*buffer = '\0';

	SetS(buffer);
}

bool TLongFace::EnableWindow(bool enable)
{
	bool result = TStringFace::EnableWindow(enable);

	if (Validator)
		((TLongValidator *) Validator)->SetPositive(enable);

	return result;
}
