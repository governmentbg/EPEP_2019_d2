//{{TEdit = TIndexFace}}
class TIndexFace : public TLongFace
{
public:
	TIndexFace(TWindow* parent, int resourceId, const char *name, const char *tKinds, uint textLimit);
	virtual ~TIndexFace();

protected:
	const char *kinds;	

//{{TIndexFaceVIRTUAL_BEGIN}}
public:
	virtual void SetupWindow();
//{{TIndexFaceVIRTUAL_END}}
};    //{{TIndexFace}}
