enum
{
	XFER_WEBACTS	= 0x01,
	XFER_REQLAWS	= 0x02,
	XFER_EXPERTS	= 0x04,
	XFER_WEBANKS	= 0x08
};

typedef enum
{
	XFER_ADD	= 0x32C9,
	XFER_EDIT	= 0x32CA,
	XFER_DELETE	= 0x32CB,
	XFER_IGNORE	= 0x32CC	// for internal use only
} XFER_ACTION;

extern constant XFER_CCEXP_DEFAULT;

void xfer_delete(const TOutReg *outReg);
void xfer_send(const TOutReg *outReg, const TInReg *inReg);
bool xfer_resendable(const TOutReg *outReg, const TInReg *inReg);
void xfer_resend(const TOutReg *outReg, const TInReg *inReg);
bool xfer_load(TWindow *parent, TRequest *request, TGArray<TConnect> *connects, TElectricTrouble *trouble);
void xfer_load(TWindow *parent, TRequest *request, TGArray<TSideWind> *sideWinds);
void xfer_reject(TRequest *request);
void xfer_accept(TRequest *request);
void xfer_delete(TWindow *parent);
bool xfer_receive(TWindow *parent, TOutReg *outReg, TReceiveInstance *rinst, TElectricTrouble *trouble);
void xfer_accept(TOutReg *outReg);
void xfer_reject(TOutReg *outReg);

void xfer_web(const TDecision *decision, const TSession *session, const TLawsuit *lawsuit);
void xfer_web(const TOutReg *outReg, bool tDelete = false);
void xfer_web_query(TWindow *parent);
void xfer_web_delete(const TDecision *decision, const TLawsuit *lawsuit);

void xfer_send(TSubpoena *subpoena, TRCDKeyContainer *container, TSideWind *sideWind, char lawKind);
void xfer_request(TSession *session, char lawKind);
void xfer_assign(TSession *session, char lawKind);
void xfer_resign(TSession *session, char lawKind);
void xfer_launch(TSession *session, char lawKind);

void xfer_email(TRCDKeyContainer *container, char gop = LOG_ENTER);

// ----- TVirtualDecision ----------------------------------------------------
struct TVirtualDecision
{
	static char DecisionKind(char sessionResult);
	static char DecisionKind(const TSession *session) { return DecisionKind(session->result); }
	static bool AutoCreate(const TSession *session, TDecision *tDecision);

protected:
	char sessionResult;
	char decisionKind;
};

bool xfer_bank_case(TLawsuit *lawsuit, TGroupArray *sideWinds, TRequest *request, XFER_ACTION action);
void xfer_bank_surround(TLawsuit *lawsuit, TSurround *surround, TGroupArray *sideWinds, XFER_ACTION action);
void xfer_bank_event(TLawsuit *lawsuit, TSession *session, TGArray<TSurround> *surrounds, TDecision *decision, TInReg *inReg,
	TGroupArray *sideWinds, TOutReg *outReg, XFER_ACTION action);
void xfer_bank_session_reset(TLawsuit *lawsuit, TSession *session, TSession *previous);
void xfer_bank_session_result_change(TLawsuit *lawsuit, TSession *session, TGArray<TSurround> *surrounds,
	TGArray<TDecision> *decisions, char tResult);	// not for
void xfer_bank_delete_sessions(TLawsuit *lawsuit, TGArray<TSession> *sessions);
void xfer_bank_filter(TGroupArray *array);
void xfer_bank_export(TLawsuit *lawsuit, TGroupArray *containers);

// ----- XFER ----------------------------------------------------------------
enum
{
	XFER_LAWSUIT_FIRST	= 2001,
	XFER_LAWSUIT_LAST		= 2999,
	XFER_CONNECT_FIRST	= 2001,
	XFER_CONNECT_LAST		= 4999,
	XFER_REQUEST_FIRST	= 8001,
	XFER_REQUEST_LAST		= 8999,
	XFER_EIS_LAW_FIRST	= 90000,
	XFER_EIS_LAW_LAST		= 91000
};

// ----- TXferKind -----------------------------------------------------------
class TXferKind : public TLongAliasGroup
{
public:
	TXferKind();
	static TGroup *Clone();

	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	long XferKindRemap();

	long xferKind;
	long types;
	char name[SIZE_OF_NAME];
	char connectKind;
	long eispp;
};

extern TXferKind *XferKind;
