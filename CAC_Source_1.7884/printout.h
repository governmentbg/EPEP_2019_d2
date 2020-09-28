void print_inreg_appealers(mstr &m, TInReg *inReg);
char outreg_subpoena_kind(const TOutReg *outReg);
bool print_outreg(TWindow *parent, const TOutReg *outReg, TInReg *inReg, TRCDKeyContainer *event, bool save,
	TGArray<TInRegEvent> *inRegEvents, const TConnect *lower);
void outreg_send_appeal(TWindow *parent, const TOutReg *outReg, const TGArray<TInRegEvent> *inRegEvents, bool save);
