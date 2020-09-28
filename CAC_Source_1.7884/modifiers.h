bool force_session(TWindow *parent, TSession *session, TLawsuit *lawsuit);
void force_decision_xfer(TWindow *parent, TDecision *decision, TLawsuit *lawsuit);
bool force_decision(TWindow *parent, TDecision *decision, TLawsuit *lawsuit);
bool accuse_sidewind(TWindow *parent, TRCDKeyContainer *container, TSideWind *sideWind);
bool punish_side(TWindow *parent, TDecision *decision, const TJudgement *judgement, TLawsuit *lawsuit);
