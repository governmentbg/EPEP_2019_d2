bool renumber(TWindow *parent, TUCNGroup *source, TUCNGroup *target);
bool join(TWindow *parent, TUCNGroup *source, TUCNGroup *target);
bool recompose(TLawsuit *source, TLawsuit *target);
bool renumber(TStreet *source, TStreet *target);
bool join(TStreet *source, TStreet *target);

// 2010:196 LPR: immediate electric signal
class TRenumberContainer : public TDummyContainer
{
public:
	TRenumberContainer(TRCDKeyContainer *source, TRCDKeyContainer *tTarget);

	TRCDKeyContainer *target;
};

void renumber_eispps(TOriginContainer *origin, TOriginContainer *target);	// 2014:022
void renumber_doc(TRequest *source, TRequest *target);
void renumber_law(TLawsuit *source, TLawsuit *target);
bool renumber_surround(TWindow *parent, TSurround *surround);
bool renumber_inreg(TWindow *parent, TInReg *inReg);
