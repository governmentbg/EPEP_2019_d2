#include "cac.h"

DEFINE_RESPONSE_TABLE1(TRadioFace, TRadioButton)
//{{TRadioFaceFaceRSP_TBL_BEGIN}}
	EV_MESSAGE(WM_FACE_NAME, EvFaceName),
//{{TRadioFaceFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TRadioFace Implementation}}

TRadioFace::TRadioFace(TWindow* parent, int resourceId, const char *tName, TGroupBox *group)
:
	TRadioButton(parent, resourceId, group), name(tName)
{
}

long TRadioFace::EvFaceName(TParam1, TParam2 lParam)
{
	*(const char **) lParam = name;
	return MW_NAMED_FACE;
}
