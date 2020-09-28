//{{TComboBox = TFzlComboBox}}
class TFzlComboBox : public TGroupComboBox
{
public:
	TFzlComboBox(TWindow* parent, int resourceId, SBE *tSbe, TXArray<FZL> *tInclude, TGArray<SBE> *tExcludeSbe,
		TXArray<FZL> *tExcludeFzl);

	FZL *Fzl() { return (FZL *) Group(); }
	virtual void Refresh();

protected:
	SBE *sbe;
	TXArray<FZL> *include;
	TGArray<SBE> *excludeSbe;
	TXArray<FZL> *excludeFzl;

	virtual TGroup *NewGroup() const;
	virtual void LoadData(const char *);

//{{FzlComboBoxRSP_TBL_BEGIN}}
	long EvFaceValid(TParam1, TParam2);
//{{FzlComboBoxRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFzlComboBox);
};    //{{TFzlComboBox}}
