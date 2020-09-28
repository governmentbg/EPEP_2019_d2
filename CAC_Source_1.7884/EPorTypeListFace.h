//{{TListBox = TEPorTypeListFace}}
class TEPorTypeListFace : public TTypeListFace
{
public:
	TEPorTypeListFace(TWindow *parent, int resourceId, const char *name);

protected:
	virtual void GetGroupText(mstr &m, TGroup *group, int column);
};  //{{TEPorTypeListFace}}
