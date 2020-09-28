#include "cac.h"

DEFINE_RESPONSE_TABLE1(TCopySideWindsDialog, TGroupDialog)
//{{TCopySideWindsDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_COPY_SIDE_WINDS_TYPE, TypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_COPY_SIDE_WINDS_KIND, KindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_COPY_SIDE_WINDS_RETYPE, ReTypeCBNSelChange),
	EV_CBN_SELCHANGE(IDC_COPY_SIDE_WINDS_REKIND, ReKindCBNSelChange),
//{{TCopySideWindsDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TCopySideWindsDialog Implementation}}

TCopySideWindsDialog::TCopySideWindsDialog(TWindow* parent, TRCDKeyContainer *source, TRCDKeyContainer *tTarget,
	const char *types)
:
	TGroupDialog(parent, source, IDD_COPY_SIDE_WINDS), target(tTarget)
{
	type = new TCharAliasFace(this, IDC_COPY_SIDE_WINDS_TYPE, "F_TYPE", Type, types);
	new TLongFace(this, IDC_COPY_SIDE_WINDS_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_COPY_SIDE_WINDS_YEAR, "F_YEAR");
	kind = new TCharAliasFace(this, IDC_COPY_SIDE_WINDS_KIND, "F_KIND", Kind, KIND_XFERABLES, ALIAS_EMPTY);
	date = new TDateFace(this, IDC_COPY_SIDE_WINDS_DATE, "F_DATE", false);
	index = new TIndexFace(this, IDC_COPY_SIDE_WINDS_INDEX, "F_INDEX", KIND_CLOSED_SESSIONS, SIZE_OF_SESSION_INDEX);

	reType = new TCharAliasFace(this, IDC_COPY_SIDE_WINDS_RETYPE, "F_RETYPE", Type, types);
	reNo = new TLongFace(this, IDC_COPY_SIDE_WINDS_RENO, "F_RENO", SIZE_OF_NO);
	reYear = new TYearFace(this, IDC_COPY_SIDE_WINDS_REYEAR, "F_REYEAR");
	reKind = new TCharAliasFace(this, IDC_COPY_SIDE_WINDS_REKIND, "F_REKIND", Kind, KIND_XFERABLES, ALIAS_EMPTY);
	reDate = new TDateFace(this, IDC_COPY_SIDE_WINDS_REDATE, "F_REDATE", false);
	reIndex = new TIndexFace(this, IDC_COPY_SIDE_WINDS_REINDEX, "F_REINDEX", KIND_CLOSED_SESSIONS, SIZE_OF_SESSION_INDEX);
}

void TCopySideWindsDialog::OKBNClicked()
{
	if (IsValid())
		CmOk();
}

bool TCopySideWindsDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		isValid = false;

		if (!kind->C() != date->Date().Empty())
			error("Полетата От: вид заседание и дата не си съответстват.");
		else if (!reKind->C() != reDate->Date().Empty())
			error("Полетата Към: вид заседание и дата не си съответстват.");
		else
		{
			*Source() << this;
			adjust_xxx_kind(Source(), index, KIND_CLOSED_SESSIONS);

			TRCDKeyContainer *tSource = kind->C() ?
				(TRCDKeyContainer *) kind2DatedContainer(Source()->kind) :
				type2RCDKeyContainer(Source()->key.type);
			TRCDKeyContainer *tAccess;

			*tSource << Source();
			if (tSource->Get() && (tAccess = can_access_basic(Source(), false, false, NULL)) != NULL)
			{
				if (!kind->C())
				{
					Source()->kind = tAccess->kind;
					Source()->date = tAccess->date;
				}

				delete tAccess;

				target->key.no = reNo->L();
				target->key.year = reYear->L();
				target->key.type = reType->C();
				target->kind = reKind->C();
				target->date = reDate->Date();
				adjust_xxx_kind(target, reIndex, KIND_CLOSED_SESSIONS);

				TRCDKeyContainer *tTarget = reKind->C() ?
					(TRCDKeyContainer *) kind2DatedContainer(target->kind) :
					type2RCDKeyContainer(target->key.type);

				*tTarget << target;
				if (tTarget->Get() && (tAccess = can_access_basic(target, true, true, NULL)) != NULL)
				{
					*target << tTarget;	// 2012:142 FIX: get sourceKind
					//if (!reKind->C())
					//{
					//	target->kind = tAccess->kind;
					//	target->date = tAccess->date;
					//}

					delete tAccess;
					isValid = true;
				}
				delete tTarget;
			}
			delete tSource;
		}
	}

	return isValid;
}

void TCopySideWindsDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	TypeCBNSelChange();
	date->SetS("");

	reKind->SetC(kind->C());
	ReTypeCBNSelChange();
	reNo->SetL(0);
	reYear->SetL(Today.year);
	reDate->SetS("");
}

static const char *type2kinds(char type)
{
	return strchr(TYPE_LAWSUITS, type) ? KIND_XFERABLES : type == TYPE_REQUEST ? KIND_REQUEST_RESOLUTIONS : KIND_INREG_EVENTS;
}

void TCopySideWindsDialog::TypeCBNSelChange()
{
	kind->SetXCrit(type2kinds(type->C()));
	KindCBNSelChange();
}

void TCopySideWindsDialog::KindCBNSelChange()
{
	date->EnableWindow(kind->C());
	adjust_xxx_index(kind->C(), index, KIND_CLOSED_SESSIONS);
}

void TCopySideWindsDialog::ReTypeCBNSelChange()
{
	reKind->SetXCrit(type2kinds(reType->C()));
	ReKindCBNSelChange();
}

void TCopySideWindsDialog::ReKindCBNSelChange()
{
	reDate->EnableWindow(reKind->C());
	adjust_xxx_index(reKind->C(), reIndex, KIND_CLOSED_SESSIONS);
}
