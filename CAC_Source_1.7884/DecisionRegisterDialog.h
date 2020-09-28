class TDecisionRegisterDialog : public TFloatDialog
{
public:
	TDecisionRegisterDialog(TWindow* parent, TDecisionRegisterGroup *group, int resId = IDD_DECISION_REGISTER_BOOK);

protected:
	char types[SIZE_OF_TYPES];
	TDateFace *minDate;
	TDateFace *maxDate;
	TCheckFace *filterText;
	TLongFace *autoSerialNo;
	 
	virtual bool IsValid();
};
