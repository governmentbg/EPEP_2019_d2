//{{TListWindow = TAssignmentListWindow}}
class TAssignmentListWindow : public TDoubleListWindow
{
public:
	TAssignmentListWindow(TWindow* parent, int resourceId, TGSArray<TAssignment> *array, TDistribution *tDistribution,
		const char *tKinds, int long_width);
	virtual ~TAssignmentListWindow();

	void Add();
	virtual bool IsValid();

	TDistribution *distribution;
	const char *kinds;

protected:
	TImageList *images;
	const char *resolutions;

	TAssignment *AssignmentAt(int index) { return (TAssignment *) GroupAt(index); }

	virtual bool AskDiscard(int count) = 0;
	virtual bool EditGroup(TGroup *) = 0;
	virtual void DrawSecond(TListWindItem &lwi, TGroup *group);
	virtual TGroup *NewGroup() const { return new TAssignment; }

//{{TAssignmentListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAssignmentListWindowVIRTUAL_END}}

//{{TAssignmentListWindowRSP_TBL_BEGIN}}
	void EvLButtonDown(uint modKeys, TPoint& point);
	void EvRButtonDown(uint modKeys, TPoint& point);
//{{TAssignmentListWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAssignmentListWindow);
};    //{{TAssignmentListWindow}}
