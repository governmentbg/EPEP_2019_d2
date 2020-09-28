//{{TListBox = TMigrateEisppSendersListBox}}
class TMigrateEisppSendersListBox : public TMigrateSendersListBox
{
public:
	TMigrateEisppSendersListBox(TWindow* parent, int resourceId, char tUCNType);

	void SetUCNType(char tUCNType);

protected:
	char ucnType;

	virtual void Criteria(msql &m, TGroup *group);
};    //{{TMigrateEisppSendersListBox}}
