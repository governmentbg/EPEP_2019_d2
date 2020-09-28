//{{TEdit = TStringFace}}
class TStringFace : public TEdit
{
public:
	TStringFace(TWindow* parent, int resourceId, const char *tName, uint textLimit);
	virtual ~TStringFace();

	const char *S() const;
	void SetS(const char *s);

protected:
	const char *name;
	char *buffer;

//{{TStringFaceVIRTUAL_BEGIN}}
	virtual void CleanupWindow();
	virtual TResult WindowProc(uint msg, TParam1 p1, TParam2 p2);
//{{TStringFaceVIRTUAL_END}}

//{{TStringFaceRSP_TBL_BEGIN}}
	long EvFaceValid(TParam1, TParam2);
	long EvFaceName(TParam1, TParam2 lParam);
//{{TStringFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TStringFace);
};    //{{TStringFace}}
