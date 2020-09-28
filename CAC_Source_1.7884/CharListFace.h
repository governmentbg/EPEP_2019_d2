//{{TListBox = TCharListFace}}
class TCharListFace : public TCharArrayListFace
{
public:
	TCharListFace(TWindow* parent, int resourceId, const char *name, TCharAliasGroup *group, const char *xCrit, int size, long flags = FLAG_NULL);

	virtual const char *S();
	virtual void SetS(const char *s);
	bool Empty();

	const char C() { return *S(); }
	void SetC(char c);	// { c, '\0' }

protected:
	char CharAt(int index) { return ((TCharAliasGroup *) GroupAt(index))->ValueField()->C(); }
	virtual void Refresh();

//{{TCharListFaceVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TCharListFaceVIRTUAL_END}}
};    //{{TCharListFace}}
