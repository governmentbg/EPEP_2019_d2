//{{TListBox = TCharAutoListFace}}
class TCharAutoListFace : public TCharListFace
{
public:
	TCharAutoListFace(TWindow* parent, int resourceId, const char *name, TCharAliasGroup *group, const char *xCrit, int size);

	virtual const char *S();
};  //{{TCharAutoListFace}}
