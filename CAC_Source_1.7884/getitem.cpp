#include "cac.h"

#include "GetDateDialog.h"
#include "GetDateDialog.cpp"

bool get_date(TWindow *parent, TGroup *group, char *title, bool mandatory, bool future)
{
	return TGetDateDialog(parent, group, title, mandatory, future).Execute() == IDOK;
}

#include "GetYearDialog.h"
#include "GetYearDialog.cpp"

bool get_year(TWindow *parent, TGroup *group)
{
	return TGetYearDialog(parent, group).Execute() == IDOK;
}

#include "GetBarCodeDialog.h"
#include "GetBarCodeDialog.cpp"

bool get_barcode(TWindow *parent, long *court, long *requestId, char *ces)
{
#if 20170097
	// 2017:097 layout switch moved to barcode face
	return TGetBarCodeDialog(parent, court, requestId, ces).Execute() == IDOK;
#else  // 20170097
	long layout = get_layout();

	set_layout_ll(LAYOUT_EN_US, false);
	bool result = TGetBarCodeDialog(parent, court, requestId, ces).Execute() == IDOK;
	set_layout_ll(layout, false);
	return result;
#endif  // 20170097
}

#include "GetNIAProtocolDialog.h"
#include "GetNIAProtocolDialog.cpp"

bool get_nia_protocol(TWindow *parent, TNIAProtocol *protocol, nia_match_func match)
{
	return TGetNIAProtocolDialog(parent, protocol, match).Execute() == IDOK;
}
