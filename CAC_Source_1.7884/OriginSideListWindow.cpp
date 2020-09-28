#include "cac.h"

//{{TOriginSideListWindow Implementation}}

TOriginSideListWindow::TOriginSideListWindow(TWindow *parent, long sideMode, int resourceId, TOriginContainer *container,
	TLawsuit *lawsuit)
:
	TSideWindListWindow(parent, sideMode, resourceId, container, lawsuit)
{
}

char TOriginSideListWindow::OriginKind() const
{
	return lawsuit ? lawsuit->sourceKind : container->kind;
}

bool TOriginSideListWindow::CheckInvolvement(const TSideWind *sideWind)
{
	return check_involvement(this, OriginKind(), sideWind);
}

bool TOriginSideListWindow::EditableInvolvement(const TSideWind *sideWind)
{
#if COURT_TYPE == COURT_REGIONAL
	if (OriginKind() == KIND_EUHER_REQUEST && strchr(INVOLVEMENT_EUHER_MANDATORYS, sideWind->involvement))
		return false;
#endif  // REGIONAL
	return TSideWindListWindow::EditableInvolvement(sideWind);
}
