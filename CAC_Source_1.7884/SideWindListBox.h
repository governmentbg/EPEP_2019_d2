//{{TListBox = TSideWindListBox}}
class TSideWindListBox : public TCloneListBox
{
public:
	TSideWindListBox(TWindow* parent, int resourceId, TGArray<TSideWind> *sideWinds);

	int Count() { return array->Count(); }
	TSideWind *SideWind() { return (TSideWind *) Group(); }
	TSideWind *SideWindAt(int index) { return (TSideWind *) GroupAt(index); }
};    //{{TSideWindListBox}}
