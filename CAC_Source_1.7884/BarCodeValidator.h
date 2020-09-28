//{{TFilterValidator = TBarCodeValidator}}
class TBarCodeValidator : public TFilterValidator
{
public:
	TBarCodeValidator();

//{{TBarCodeValidatorVIRTUAL_BEGIN}}
	virtual void Error(TWindow* owner);
//{{TBarCodeValidatorVIRTUAL_END}}
};  //{{TBarCodeValidator}}
