//{{TListWindow = TJudgementListWindow}}
class TJudgementListWindow : public TSideWindListWindow
{
public:
	TJudgementListWindow(TWindow* parent, long sideMode, int resourceId, TDecision *decision, TLawsuit *lawsuit);

	void Enter();
	void Exec();
	void Extra();
	void P64Check(mstr &m) { Decision()->P64Check(m, array, lawsuit->kind); }

protected:
	virtual TGroup *NewGroup() const;

	TDecision *Decision() { return (TDecision *) container; }
	TJudgement *Judgement() { return (TJudgement *) Group(); }
};    //{{TJudgementListWindow}}
