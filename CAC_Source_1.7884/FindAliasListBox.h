//{{TListBox = TFindAliasListBox}}
class TFindAliasListBox : public TAliasListBox
{
public:
	TFindAliasListBox(TWindow* parent, int resourceId, TAliasGroup *alias, const char *xCrit, const char *tInitialName);

	void Search(const char *name, bool extra);

//{{TFindAliasDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFindAliasDialogVIRTUAL_END}}

protected:
	const char *initialName;
};    //{{TFindAliasListBox}}
