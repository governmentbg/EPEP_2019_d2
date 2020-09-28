//{{TComboBox = TArticleAliasFace}}
class TArticleAliasFace : public TLongAliasFace
{
public:
	TArticleAliasFace(TWindow* parent, int resourceId, const char *name, long tSubject, long flags);

protected:
	long subject;

	virtual TGroup *NewGroup() const;
};    //{{TArticleAliasFace}}
