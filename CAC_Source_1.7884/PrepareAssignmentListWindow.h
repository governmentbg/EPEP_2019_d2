//{{TListWindow = TPrepareAssignmentListWindow}}
class TPrepareAssignmentListWindow : public TAssignmentListWindow
{
public:
	TPrepareAssignmentListWindow(TWindow* parent, int resourceId, TGSArray<TAssignment> *array, TDistribution *distribution, char *kinds);

	virtual bool IsValid();

protected:
	virtual int GetImage(TGroup *group) { return 0; }
	virtual void InsertVisual(TGroup *group, int index);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDiscard(int count);
};    //{{TPrepareAssignmentListWindow}}
