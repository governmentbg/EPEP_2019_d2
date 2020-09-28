//{{TListBox = TEisppLawsListBox}}
class TEisppLawsListBox : public TGroupListBox
{
public:
	TEisppLawsListBox(TWindow* parent, int resourceId, TLawsuit *tLawsuit);

	void Query();

protected:
	TLawsuit *lawsuit;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);	// eispp nmr
};    //{{TEisppLawsListBox}}
