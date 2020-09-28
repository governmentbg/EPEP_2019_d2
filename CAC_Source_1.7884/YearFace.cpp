//{{TYearFace Implementation}}

TYearFace::TYearFace(TWindow* parent, int resourceId, const char *name)
:
	TLongFace(parent, resourceId, name, SIZE_OF_YEAR)
{
}

void TYearFace::SetupWindow()
{
	TLongFace::SetupWindow();

	// 2010:120 WTF? comout
	//if (IsWindowEnabled())
		SetValidator(new TYearValidator());
}
