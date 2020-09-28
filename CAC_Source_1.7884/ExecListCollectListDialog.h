//{{TDialog = TExecListCollectListDialog}}
class TExecListCollectListDialog : public TGroupListDialog
{
public:
	TExecListCollectListDialog(TWindow* parent, TPrintExecList *tExecList, int resId = IDD_EXEC_LIST_COLLECT_LIST);

protected:
	TPrintExecList *execList;
	TStatic *header1;
	TStatic *header2;
	TStatic *header3;
	TExecListCollectListWindow *collects;

//{{TExecListCollectListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TExecListCollectListDialogVIRTUAL_END}}
};    //{{TExecListCollectListDialog}}
