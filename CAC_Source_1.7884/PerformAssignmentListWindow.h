//{{TListWindow = TPerformAssignmentListWindow}}
class TPerformAssignmentListWindow : public TAssignmentListWindow
{
public:
	TPerformAssignmentListWindow(TWindow* parent, int resourceId, TGSArray<TAssignment> *array, TDistribution *distribution, const char *kinds, TUserLoadListBox *tLoad);

protected:
	TUserLoadListBox *load;		// 2007:255 LPR: refresh after edit

	virtual void InsertVisual(TGroup *group, int index);
	virtual void DrawSecond(TListWindItem &lwi, TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDiscard(int count);
};    //{{TPerformAssignmentListWindow}}
