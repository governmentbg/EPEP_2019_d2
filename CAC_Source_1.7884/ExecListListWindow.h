//{{TListWindow = TExecListListWindow}}
class TExecListListWindow : public TDoubleListWindow
{
public:
	TExecListListWindow(TWindow* parent, int resourceId);
	virtual ~TExecListListWindow() { delete array; }

	void Search(TExecList *execList);
	TPrintExecList *ExecList() { return TYPECHECK_PTR_CAST(TPrintExecList, Group()); }

protected:
	enum { MAX_UNSENT_DISPLAY = 15 };

	virtual void DrawSecond(TListWindItem &lwi, TGroup *group);

//{{TExecListListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TExecListListWindowVIRTUAL_END}}
};    //{{TExecListListWindow}}
