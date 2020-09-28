//{{TDialog = TUnwrittenSesDialog}}
class TUnwrittenSesDialog : public TUnwrittenDialog
{
public:
	TUnwrittenSesDialog(TWindow* parent, TUnwrittenSesGroup *group);

protected:
	TCharListFace *types;
	TCharListFace *kinds;
	
	DEFINE_GROUP_TYPE(TUnwrittenSesGroup, UnwrittenSesGroup)
	virtual void Query(const char *criteria, const char *title);
};    //{{TUnwrittenSesDialog}}
