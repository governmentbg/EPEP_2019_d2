//{{TDialog = TLocalSurroundmentDialog}}
class TLocalSurroundmentDialog : public TEditDialog
{
public:
	TLocalSurroundmentDialog(TWindow* parent, TSurroundment *surroundment, TSession *tSession, TDecision *tDecision);

protected:
	TSession *session;
	TDecision *decision;
};    //{{TLocalSurroundmentDialog}}
