//{{TNIATDepInNoFace Implementation}}

TNIATDepInNoFace::TNIATDepInNoFace(TWindow* parent, int resourceId, const char *name)
:
	TStringFace(parent, resourceId, name, SIZE_OF_NIA_TDEP_IN_NO)
{
	SetValidator(new TNIATDepInNoValidator());
}
