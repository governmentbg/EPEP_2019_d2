//{{TListBox = TCharArrayListFace}}
class TCharArrayListFace : public TAliasListBox
{
public:
	TCharArrayListFace(TWindow* parent, int resourceId, const char *tName, TCharAliasGroup *alias, const char *xCrit, int tSize, long flags = FLAG_NULL);
	virtual ~TCharArrayListFace();

	virtual const char *S() = 0;
	virtual void SetS(const char *s) = 0;

	void SetXCrit(const char *tXCrit);

protected:
	const char *name;
	char *s;
	int size;

	virtual int MarkedsMax();	
	char CharAt(int index) { return ((TCharAliasGroup *) GroupAt(index))->ValueField()->C(); }

//{{TCharrayFaceRSP_TBL_BEGIN}}
protected:
	long EvFaceName(TParam1, TParam2 lParam);
//{{TCharrayFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TCharArrayListFace);
};    //{{TCharArrayListFace}}
