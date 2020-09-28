//{{TListWindow = TConfigNameListWindow}}
class TConfigNameListWindow : public TGroupListWindow
{
public:
	TConfigNameListWindow(TWindow* parent, int resourceId);

	TConfigName *ConfigName() { return (TConfigName *) Group(); }

	void Open();	// Edit
	void Copy();

protected:
	virtual TGroup *NewGroup() const;

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);	// Rename
	virtual bool AskDelete(TGroup *group);

//{{TConfigNameListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TConfigNameListWindowVIRTUAL_END}}
};    //{{TConfigNameListWindow}}
