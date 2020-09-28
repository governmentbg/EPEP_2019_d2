//{{TListBox = TXferSideListBox}}
class TXferSideListBox : public TGroupListBox
{
public:
	TXferSideListBox(TWindow* parent, int resourceId, TXferDocument *tDocument, TGSArray<TXferSide> *tArray, const char *tInvolvements);

	void ApplyInvolvement(char involvement);

protected:
	TXferDocument *document;
	const char *involvements;

	void LoadSides(TGroup *(*tClone)());

	virtual TGroup *NewGroup() const;
	virtual void Refresh();
};  //{{TXferSideListBox}}
