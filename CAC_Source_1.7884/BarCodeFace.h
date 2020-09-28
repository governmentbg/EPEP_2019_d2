//{{TEdit = TBarCodeFace}}
class TBarCodeFace : public TStringFace
{
public:
	TBarCodeFace(TWindow* parent, int resourceId, const char *name);

protected:
	long savedLayout;

//{{TBarCodeFaceRSP_TBL_BEGIN}}
	void EvSetFocus(THandle hWndLostFocus);
	void EvKillFocus(THandle hWndGetFocus);
//{{TBarCodeFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TBarCodeFace);
};  //{{TBarCodeFace}}
