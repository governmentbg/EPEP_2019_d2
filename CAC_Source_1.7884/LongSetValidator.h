//{{TFilterValidator = TLongSetValidator}}
class TLongSetValidator : public TFilterValidator
{
public:
	TLongSetValidator();

protected:
//{{TLongSetValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
	virtual bool IsValid(const char far* input);
//{{TLongSetValidatorVIRTUAL_END}}
};    //{{TLongSetValidator}}
