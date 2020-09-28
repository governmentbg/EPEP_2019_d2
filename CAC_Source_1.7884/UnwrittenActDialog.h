//{{TDialog = TUnwrittenActDialog}}
class TUnwrittenActDialog : public TUnwrittenDialog
{
public:
	TUnwrittenActDialog(TWindow* parent, TUnwrittenActGroup *group);

protected:
	TCharListFace *kinds;

	DEFINE_GROUP_TYPE(TUnwrittenActGroup, UnwrittenActGroup)
	virtual void Query(const char *criteria, const char *title);
};    //{{TUnwrittenActDialog}}
