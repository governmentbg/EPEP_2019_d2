//{{TComboBox = TNprFzlPneComboBox}}
class TNprFzlPneComboBox : public TGroupComboBox
{
public:
	TNprFzlPneComboBox(TWindow* parent, int resourceId, FZL *tFZL, TXArray<NPRFZLPNE> *tAllLinks, NPR *tNpr);

	NPRFZLPNE *NprFzlPne() { return (NPRFZLPNE *) Group(); }
	virtual void Refresh();

protected:
	FZL *fzl;
	NPR *npr;
	TXArray<NPRFZLPNE> *allLinks;
	TXArray<NPRFZLPNE> *exclude;

	virtual void GetGroupText(mstr &m, TGroup *group, int column);

	virtual TGroup *NewGroup() const;
	virtual void LoadData(const char *);
};    //{{TNprFzlPneComboBox}}
