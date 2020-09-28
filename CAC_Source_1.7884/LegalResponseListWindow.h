//{{TListWindow = TLegalResponseListWindow}}
class TLegalResponseListWindow : public TGroupListWindow
{
public:
	TLegalResponseListWindow(TWindow* parent, int resourceId, TGArray<TLegalResponse> *responses);

	bool LoadGroup();
	void ViewGroup() { legalaid_view(this, (TLegalResponse *) Group()); }

protected:
	virtual TGroup *NewGroup() const { return new TLegalResponse; }
};    //{{TLegalResponseListWindow}}
