//{{TDialog = TFloatDialog}}
class TFloatDialog : public TGroupDialog
{
public:
	TFloatDialog(TWindow* parent, TFloatGroup *group, int resId);

protected:
	DEFINE_GROUP_TYPE(TFloatGroup, FloatGroup)

#if TESTVER
	void Message(mstr &m);
	void MessageEx(mstr &m);
#else  // TESTVER
	void Message(mstr &) { }
	void MessageEx(mstr &) { }
#endif  // TESTVER
	virtual void Action(bool execute);
	void Clicked(bool execute);

//{{TFloatDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void FilterBNClicked();
//{{TFloatDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFloatDialog);
};  //{{TFloatDialog}}
