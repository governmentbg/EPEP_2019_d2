DEFINE_RESPONSE_TABLE1(TCharArrayListFace, TAliasListBox)
//{{TCharArrayListFaceRSP_TBL_BEGIN}}
	EV_MESSAGE(WM_FACE_NAME, EvFaceName),
//{{TCharArrayListFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TCharArrayListFace Implementation}}

TCharArrayListFace::TCharArrayListFace(TWindow* parent, int resourceId, const char *tName, TCharAliasGroup *alias, const char *xCrit, int tSize, long flags)
:
	TAliasListBox(parent, resourceId, alias, xCrit, flags), name(tName), size(tSize), s(NULL)
{
	s = new char[size];
	*s = '\0';
}

TCharArrayListFace::~TCharArrayListFace()
{
	delete[] s;
}

void TCharArrayListFace::SetXCrit(const char *tXCrit)
{
	xCrit = tXCrit;
	Refresh();
}

int TCharArrayListFace::MarkedsMax()
{
	return size - 1;
}

long TCharArrayListFace::EvFaceName(TParam1, TParam2 lParam)
{
	*(const char **) lParam = name;
	return MW_NAMED_FACE;
}
