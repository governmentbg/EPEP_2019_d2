//{{TSenderAliasFace Implementation}}

TSenderAliasFace::TSenderAliasFace(TWindow* parent, int resourceId, const char *name, char ucnType, long flags,
	TGArray<TReceiver> *receivers)
:
	TStringAliasFace(parent, resourceId, name, Receiver, C2S[ucnType], flags, receivers)
{
}

// 2016:071 copied from Receiver
char TSenderAliasFace::UCNType()
{
	TReceiver *receiver = (TReceiver *) Group();
	return receiver ? receiver->ucnType : '\0';
}
