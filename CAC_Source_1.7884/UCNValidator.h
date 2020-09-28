//{{TPXPictureValidator = TUCNValidator}}
class TUCNValidator : public TPXPictureValidator
{
public:
	TUCNValidator(char tType);
	TUCNValidator(TCharAliasFace *tFace);

protected:
	char type;
	TCharAliasFace *face;

//{{TUCNValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TUCNValidatorVIRTUAL_END}}
};  //{{TUCNValidator}}
