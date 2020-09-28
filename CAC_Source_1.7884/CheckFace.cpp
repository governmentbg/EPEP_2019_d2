#include "cac.h"

DEFINE_RESPONSE_TABLE1(TCheckFace, TCheckBox)
//{{TCheckFaceFaceRSP_TBL_BEGIN}}
	EV_MESSAGE(WM_FACE_NAME, EvFaceName),
//{{TCheckFaceFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TCheckFace Implementation}}

TCheckFace::TCheckFace(TWindow* parent, int resourceId, const char *tName, TGroupBox *group)
:
	TCheckBox(parent, resourceId, group), name(tName)
{
}

long TCheckFace::EvFaceName(TParam1, TParam2 lParam)
{
	*(const char **) lParam = name;
	return MW_NAMED_FACE;
}
