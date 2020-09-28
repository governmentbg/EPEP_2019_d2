//{{TDialog = TUnwrittenDialog}}
class TUnwrittenDialog : public TFloatSubjectDialog
{
public:
	TUnwrittenDialog(TWindow* parent, TUnwrittenGroup *group, const char *tWhat, int resId);

protected:
	const char *what;

	TUserListBox *judges;
	TDateFace *date;

	DEFINE_GROUP_TYPE(TUnwrittenGroup, UnwrittenGroup)
	virtual void Action(bool execute);
	virtual void Query(const char *criteria, const char *title) = 0;
};    //{{TUnwrittenDialog}}
