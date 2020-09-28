//{{TListBox = TStreetMapListBox}}
class TStreetMapListBox : public TGroupListBox
{
public:
	TStreetMapListBox(TWindow* parent, int resourceId);

	void SetUser(const char *tUser);

protected:
	const char *user;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);
};    //{{TStreetMapListBox}}
