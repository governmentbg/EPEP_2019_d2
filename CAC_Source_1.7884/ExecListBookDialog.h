class TExecListBookDialog : public TFloatDialog
{
public:
	TExecListBookDialog(TWindow* parent, TFloatExecListGroup *group, int resId = IDD_EXEC_LIST_BOOK);

protected:
	TCharListFace *types;
	TDateFace *minExecListDate;
	TDateFace *maxExecListDate;
	TRadioFace *execListAlls;
	TRadioFace *execListJudicials;
	TCheckFace *voluntary; 

	virtual bool IsValid();
	DEFINE_GROUP_TYPE(TFloatExecListGroup, FloatExecListGroup)

	virtual void SetupWindow();
};
