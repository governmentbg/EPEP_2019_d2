class TPosText;

class TPosTerm
{
public:
	static void Initialize();
	static bool Active(int port) { return hcomm[port] != INVALID_HANDLE_VALUE; }
	static void Shutdown();

	static bool Approval(TWindow *parent, int port);	// 2014:092 LPR/URQ: +re-initialize ports if needed
	// 2013:312 LPR/LRQ: +cardId
	static long Payment(TWindow *parent, int port, double value, char *transId, char *authId, char *cardId);
	// 2014:100 LPR: public
	static long initicks;

protected:
	static bool Error(int port, const char *text, const char *format, ...);
	static bool InitPort(int port);
	static void ClosePort(int port);
	// 2013:312 LPR/LRQ: fixed timeout, -1 for user cancel
	static sign Send(TWindow *parent, int port, TPosText &send, TPosText &recv, char message, const char *prompt);

	static HANDLE hcomm[POSTERM_COUNT];
};
