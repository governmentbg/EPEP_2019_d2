#include "cac.h"

#include "EditAreaDialog.h"
#include "EditAreaDialog.cpp"

#include "EditRegionDialog.h"
#include "EditRegionDialog.cpp"

#include "EditUCLPDialog.h"
#include "EditUCLPDialog.cpp"

bool add_uclp(TWindow *parent, TUCLP *uclp, bool full)
{
	return TEditUCLPDialog(parent, uclp, full, false).Execute() == IDOK;
}

bool edit_uclp(TWindow *parent, TUCLP *uclp, bool full)
{
	return TEditUCLPDialog(parent, uclp, full, true).Execute() == IDOK;
}

#include "EditStreetDialog.h"
#include "EditStreetDialog.cpp"

bool add_street(TWindow *parent, TStreet *street)
{
	return TEditStreetDialog(parent, street, false).Execute() == IDOK;
}

bool edit_street(TWindow *parent, TStreet *street)
{
	return TEditStreetDialog(parent, street, true).Execute() == IDOK;
}

static void cadr_added(mstr &m, TGroup *group)
{
	long l = ((TLongAliasGroup *) group)->ValueField()->L();
	m.cpy(l < 0 ? "д" : "");
}

static long ZERO_LONG = 0;

#include "CadrListWindow.h"
#include "CadrListWindow.cpp"

#include "EmptyListWindow.h"
#include "EmptyListWindow.cpp"

#include "AreaListWindow.h"
#include "AreaListWindow.cpp"

#include "RegionListWindow.h"
#include "RegionListWindow.cpp"

#include "UCLPListWindow.h"
#include "UCLPListWindow.cpp"

#include "StreetListWindow.h"
#include "StreetListWindow.cpp"

const
	TAB_AREAS	= 0,
	TAB_REGIONS	= 1,
	TAB_UCLPS	= 2,
	TAB_STREETS	= 3,
	TAB_COUNT	= 4;

#include "CadrListDialog.h"
#include "CadrListDialog.cpp"

void edit_cadrs(TWindow *parent, int startTab)
{
	TCadrListDialog(parent, startTab).Execute();
	Area->ReloadCache();
	Region->ReloadCache();
}

void edit_areas(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_AREAS, "области");
	edit_cadrs(parent, TAB_AREAS);
}

void edit_regions(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_REGIONS, "общини");
	edit_cadrs(parent, TAB_REGIONS);
}

void edit_uclps(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_UCLPS, "населени места");
	edit_cadrs(parent, TAB_UCLPS);
}

void edit_streets(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_STREETS, "улици");
	edit_cadrs(parent, TAB_STREETS);
}
