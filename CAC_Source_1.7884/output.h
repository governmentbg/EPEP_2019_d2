class TOutput : public TProcess
{
public:
	TOutput(TWindow *parent, const char *what, const char *tWait);

	bool FindFinalizer(const TLawsuit &lawsuit, TSession &session, const CDate &start, const CDate &final);

	void SetLawKey(const TLawsuit &lawsuit);
	char sLawKey[0x40];

protected:
	// 2009:222
	void TraceRelated(const TRCDKey &start, char connectType);
	virtual void HandleRelated(const TConnect &connect);
};
