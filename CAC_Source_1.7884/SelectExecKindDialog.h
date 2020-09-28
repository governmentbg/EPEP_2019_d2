//{{TDialog = TSelectExecKindDialog}}
class TSelectExecKindDialog : public TGroupDialog
{
public:
	TSelectExecKindDialog(TWindow* parent, TDecision *decision, int resId = IDD_SELECT_EXEC_KIND);

	virtual bool IsValid();

protected:
	TRadioFace *kind410money;
	TRadioFace *kind410movable;
	TRadioFace *kind417money;
	TRadioFace *kind417movable;
};    //{{TSelectExecKindDialog}}
