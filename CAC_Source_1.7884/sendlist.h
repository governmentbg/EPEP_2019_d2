void init_event(TOutReg *outReg, TInReg *inReg, TInRegEvent *inRegEvent);
bool send_inreg(TOutReg *outReg, TInReg *inReg, TInRegEvent *inRegEvent = NULL);
void load_list(TInReg *inReg, TGArray<TInReg> *inRegs);
void send_list(TOutReg *outReg, TInReg *inReg);
void receive_list(TOutReg *outReg);
void delete_send_list(TOutReg *outReg);
void delete_receive_list(TOutReg *outReg);
void revert_receive_list(TOutReg *outReg);
