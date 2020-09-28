//{{TFilterValidator = TFirmNameValidator}}
class TFirmNameValidator : public TNameValidator
{
public:
	TFirmNameValidator();

//{{TFirmNameValidatorVIRTUAL_BEGIN}}
	virtual bool IsValid(const char far* input);
//{{TFirmNameValidatorVIRTUAL_END}}
};    //{{TFirmNameValidator}}
