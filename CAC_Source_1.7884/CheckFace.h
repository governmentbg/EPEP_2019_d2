//{{TCheckBox = TCheckFace}}
class TCheckFace : public TCheckBox
{
public:
	TCheckFace(TWindow* parent, int resourceId, const char *tName, TGroupBox* group = 0);

protected:
	const char *name;

//{{TCheckFaceRSP_TBL_BEGIN}}
	long EvFaceName(TParam1, TParam2 lParam);
//{{TCheckFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TCheckFace);
};    //{{TCheckFace}}
