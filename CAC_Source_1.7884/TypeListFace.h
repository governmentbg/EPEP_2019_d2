//{{TListBox = TTypeListFace}}
class TTypeListFace : public TCharAutoListFace
{
public:
	TTypeListFace(TWindow *parent, int resourceId, const char *name, const char *tTypes, bool tWrite);

	virtual const char *S();		// 2009:337 LPR: auto select if !write

protected:
	char types[SIZE_OF_TYPES];
	bool write;
};  //{{TTypeListFace}}
