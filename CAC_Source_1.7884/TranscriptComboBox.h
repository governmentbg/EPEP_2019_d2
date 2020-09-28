//{{TComboBox = TTranscriptComboBox}}
class TTranscriptComboBox : public TGroupComboBox
{
public:
	TTranscriptComboBox(TWindow* parent, int resourceId, TPrintSubpoena *tPrintSub);

	TRCDKeyContainer *GetContainer();	// newly allocated copy
	TRCDKeyContainer *ContainerAt(int index) const { return (TRCDKeyContainer *) GroupAt(index); }

protected:
	TPrintSubpoena *printsub;
	char *plus_req;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);
	virtual void LoadData(const char *s);		// add request if needed
	virtual void Refresh();					// seleft first if any
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TTranscriptComboBox}}
