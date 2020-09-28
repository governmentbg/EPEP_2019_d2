//{{TListBox = TSLCReasonListBox}}
class TSLCReasonListBox : public TCloneListBox
{
public:
	TSLCReasonListBox(TWindow* parent, int resourceId, const char *tSLCTypes, long tFlags = 0);

	void SetMark(TGArray<TSLCReason> *slcReasons);

protected:
	const char *slcTypes;
	long flags;

	TSLCReason *SLCReasonAt(int index) { return (TSLCReason *) GroupAt(index); }

	virtual void Criteria(msql &m, TGroup *group);	
	virtual void LoadData(const char *s);
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TSLCReasonListBox}}
