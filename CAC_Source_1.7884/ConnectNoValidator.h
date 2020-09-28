//{{TFilterValidator = TConnectNoValidator}}
class TConnectNoValidator : public TFilterValidator
{
public:
	TConnectNoValidator(TCharAliasFace *tFace = NULL);

protected:
	TCharAliasFace *face;

//{{TConnectNoValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TConnectNoValidatorVIRTUAL_END}}
};    //{{TConnectNoValidator}}
