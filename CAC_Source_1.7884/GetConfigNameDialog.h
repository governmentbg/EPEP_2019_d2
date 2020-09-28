class TGetConfigNameDialog : public TGroupDialog
{
public:
	TGetConfigNameDialog(TWindow* parent, TConfigName *configName, const char *tTitle, const char *tInvalid, int resId = IDD_GET_CONFIG_NAME);

protected:
	const char *title;
	const char *invalid;
	char backup[SIZE_OF_ALIAS];
	THostNameFace *config;

	virtual bool IsValid();

//{{TGetConfigNameDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetConfigNameDialogVIRTUAL_END}}
};
