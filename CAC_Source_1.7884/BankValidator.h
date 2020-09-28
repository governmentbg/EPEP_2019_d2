//{{TFilterValidator = TBankValidator}}
class TBankValidator : public TFilterValidator
{
public:
	TBankValidator();

//{{TBankValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
//{{TBankValidatorVIRTUAL_END}}
};    //{{TBankValidator}}
