//{{TListBox = TFindReplaceListBox}}
class TFindReplaceListBox : public TStringListBox
{
public:
	TFindReplaceListBox(TWindow* parent, int resourceId, TIArray<TReplacePair> *tPairs);

	TReplacePair *Pair();

	bool AddPair(TReplacePair *pair);
	void DeletePair();


protected:
	TIArray<TReplacePair> *pairs;

	virtual void Refresh();
};    //{{TFindReplaceListBox}}
