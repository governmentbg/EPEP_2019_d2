//{{TDialog = TConfigDataListDialog}}
class TConfigDataListDialog : public TGroupListDialog
{
public:
	TConfigDataListDialog(TWindow* parent, TConfigName *tConfigName, int resId = IDD_CONFIG_DATA_LIST);

protected:
	TConfigName *configName;
	TStringFace *name;
	TCharAliasFace *section;
	TConfigDataListWindow *configs;
	TStatic *text1;
	TStatic *text2;
	TStatic *text3;

	char sections[SIZE_OF_SECTIONS];

//{{TConfigDataListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TConfigDataListDialogVIRTUAL_END}}

//{{TConfigDataListDialogRSP_TBL_BEGIN}}
	void SectionCBNSelChange();
     	void ConfigsLVNItemChanged(TLwNotify &);
//{{TConfigDataListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TConfigDataListDialog);
};    //{{TConfigDataListDialog}}
