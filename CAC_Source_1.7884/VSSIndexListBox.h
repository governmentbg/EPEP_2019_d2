//{{TListBox = TVSSIndexListBox}}
class TVSSIndexListBox : public TCloneListBox
{
public:
	TVSSIndexListBox(TWindow* parent, int resourceId);

	void SetCrit(const CDate &tStart, const CDate &tFinal);

protected:
	char types[SIZE_OF_TYPES];
	CDate start;
	CDate final;
	
	virtual void Refresh();
};    //{{TVSSIndexListBox}}
