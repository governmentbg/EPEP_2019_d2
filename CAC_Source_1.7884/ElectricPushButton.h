//{{TButton = TElectricPushButton}}
class TElectricPushButton : public TPushButton
{
public:
	TElectricPushButton(TScaleDialog *parent, int resourceId);

protected:
//{{TElectricPushButtonRSP_TBL_BEGIN}}
	void EvKeyUp(uint key, uint repeatCount, uint flags);
	void EvKeyDown(uint key, uint repeatCount, uint flags);
	void EvSysKeyUp(uint key, uint repeatCount, uint flags);
	void EvSysKeyDown(uint key, uint repeatCount, uint flags);
//{{TElectricPushButtonRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TElectricPushButton);
};    //{{TElectricPushButton}}
