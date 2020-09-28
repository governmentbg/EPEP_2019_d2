//{{TListBox = TLawKindsRTFace}}
class TLawKindsRTFace : public TCharListFace
{
public:
	TLawKindsRTFace(TWindow* parent, int resourceId, const char *name);

	virtual const char *S();
		
protected:
	virtual void Refresh();
};    //{{TLawKindsRTFace}}
