//{{TEdit = TLongFace}}
class TLongFace : public TStringFace
{
public:
	TLongFace(TWindow* parent, int resourceId, const char *name, uint textLimit);
	virtual ~TLongFace();

	virtual bool EnableWindow(bool enable);

	long L() const;
	void SetL(long l);

protected:
//{{TLongFaceVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TLongFaceVIRTUAL_END}}
};    //{{TLongFace}}
