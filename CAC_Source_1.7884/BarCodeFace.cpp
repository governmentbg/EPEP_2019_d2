DEFINE_RESPONSE_TABLE1(TBarCodeFace, TStringFace)
//{{TBarCodeFaceRSP_TBL_BEGIN}}
	EV_WM_SETFOCUS,
	EV_WM_KILLFOCUS,
//{{TBarCodeFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TBarCodeFace Implementation}}

TBarCodeFace::TBarCodeFace(TWindow* parent, int resourceId, const char *name)
:
	TStringFace(parent, resourceId, name, SIZE_OF_BARCODE_TEXT)
{
	SetValidator(new TBarCodeValidator());
}

void TBarCodeFace::EvSetFocus(THandle hWndLostFocus)
{
	TStringFace::EvSetFocus(hWndLostFocus);

	savedLayout = get_layout();
	set_layout_ll(LAYOUT_EN_US, false);
}

void TBarCodeFace::EvKillFocus(THandle hWndGetFocus)
{
	set_layout_ll(savedLayout, false);

	TStringFace::EvKillFocus(hWndGetFocus);
}
