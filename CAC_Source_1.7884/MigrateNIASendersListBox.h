//{{TListBox = TMigrateNIASendersListBox}}
class TMigrateNIASendersListBox : public TMigrateSendersListBox
{
public:
	TMigrateNIASendersListBox(TWindow* parent, int resourceId, bool tSource);

protected:
	bool source;

	virtual void Criteria(msql &m, TGroup *group);
};    //{{TMigrateNIASendersListBox}}
