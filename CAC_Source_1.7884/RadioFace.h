//{{TRadioBox = TRadioFace}}
class TRadioFace : public TRadioButton
{
public:
	TRadioFace(TWindow* parent, int resourceId, const char *tName, TGroupBox* group = 0);

protected:
	const char *name;

//{{TRadioFaceRSP_TBL_BEGIN}}
	long EvFaceName(TParam1, TParam2 lParam);
//{{TRadioFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TRadioFace);
};    //{{TRadioFace}}
