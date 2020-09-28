#include "cac.h"

DEFINE_RESPONSE_TABLE1(TEditAddressDialog, TEditDialog)
//{{TEditAddressDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_ADDRESS_AREA, AreaCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_ADDRESS_REGION, RegionCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_ADDRESS_UCLP, UCLPCBNSelChange),
	EV_BN_CLICKED(IDC_EDIT_ADDRESS_ADD_UCLP, AddUCLPBNClicked),
	EV_BN_CLICKED(IDC_EDIT_ADDRESS_EDIT_UCLP, EditUCLPBNClicked),
	EV_BN_CLICKED(IDC_EDIT_ADDRESS_ADD_STREET, AddStreetBNClicked),
	EV_BN_CLICKED(IDC_EDIT_ADDRESS_EDIT_STREET, EditStreetBNClicked),
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
//{{TEditAddressDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditAddressDialog Implementation}}

TEditAddressDialog::TEditAddressDialog(TWindow *parent, TAddress *address, bool edit, int tAddrMode)
	: TEditDialog(parent, address, edit, IDD_EDIT_ADDRESS), addrMode(tAddrMode)
{
	new TCharAliasFace(this, IDC_EDIT_ADDRESS_UCN_TYPE, "F_UCN_TYPE", UCNType, NULL);
	new TStringFace(this, IDC_EDIT_ADDRESS_UCN, "F_UCN", SIZE_OF_UCN);
	area = new TAreaAliasFace(this, IDC_EDIT_ADDRESS_AREA, "F_AREA");
	region = new TRegionAliasFace(this, IDC_EDIT_ADDRESS_REGION, "F_REGION", area);
	uclp = new TFindUCLPAliasFace(this, IDC_EDIT_ADDRESS_UCLP, "F_UCLP", area, region);
	addUCLP = new TPushButton(this, IDC_EDIT_ADDRESS_ADD_UCLP);
	editUCLP = new TPushButton(this, IDC_EDIT_ADDRESS_EDIT_UCLP);
	street = new TFindStreetAliasFace(this, IDC_EDIT_ADDRESS_STREET, "F_STREET", uclp);
	addStreet = new TPushButton(this, IDC_EDIT_ADDRESS_ADD_STREET);
	editStreet = new TPushButton(this, IDC_EDIT_ADDRESS_EDIT_STREET);

	new TStringFace(this, IDC_EDIT_ADDRESS_ADR_NO, "F_ADR_NO", SIZE_OF_ADR_NO);
	new TStringFace(this, IDC_EDIT_ADDRESS_SUB_NO, "F_SUB_NO", SIZE_OF_SUB_NO);
	new TStringFace(this, IDC_EDIT_ADDRESS_ENTRANCE, "F_ENTRANCE", SIZE_OF_ENTRANCE);
	new TStringFace(this, IDC_EDIT_ADDRESS_FLOOR, "F_FLOOR", SIZE_OF_FLOOR);
	new TStringFace(this, IDC_EDIT_ADDRESS_APARTMENT, "F_APARTMENT", SIZE_OF_APARTMENT);
	new TStringFace(this, IDC_EDIT_ADDRESS_BLOCK_NO, "F_BLOCK_NO", SIZE_OF_BLOCK_NO);
	new TStringFace(this, IDC_EDIT_ADDRESS_PHONE, "F_PHONE", SIZE_OF_PHONE);
	new TStringFace(this, IDC_EDIT_ADDRESS_TELEX, "F_TELEX", SIZE_OF_TELEX);
	new TStringFace(this, IDC_EDIT_ADDRESS_FAX, "F_FAX", SIZE_OF_FAX);
	email = new TStringFace(this, IDC_EDIT_ADDRESS_EMAIL, "F_EMAIL", SIZE_OF_EMAIL);
	new TStringFace(this, IDC_EDIT_ADDRESS_REMARK, "F_REMARK", SIZE_OF_ADDRESS_REMARK);
}

bool TEditAddressDialog::CanDirect(long lock)
{
	return can_direct(area->L(), region->L(), lock);
}

void TEditAddressDialog::SetLock()
{
	addUCLP->EnableWindow(can_func(RIGHT_UCLP) && CanDirect(LOCK_UCLP));
	editUCLP->EnableWindow(can_func(RIGHT_UCLP) && CanDirect(LOCK_UCLP));
	addStreet->EnableWindow(can_func(RIGHT_STREET) && CanDirect(LOCK_STREET));
	editStreet->EnableWindow(can_func(RIGHT_STREET) && CanDirect(LOCK_STREET));
}

void TEditAddressDialog::SetData()
{
	area->SetL(UCLP->area);
	region->Reset(UCLP->region);
	uclp->Reset(Address()->uclp);
	street->Reset(Address()->street);
	SetLock();
}

bool TEditAddressDialog::IsValid()
{
	bool isValid = TEditDialog::IsValid();

	if (isValid)
	{
		*Group() << this;		// 2017:010 for match

		const char *status = Address()->Match(addrMode);

		if (status)
			isValid = error("%s.", status);
	}

	return isValid;
}

void TEditAddressDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	Address()->GetUCLP();
	SetData();
}

void TEditAddressDialog::AreaCBNSelChange()
{
	region->Reset(0);
	RegionCBNSelChange();
}

void TEditAddressDialog::RegionCBNSelChange()
{
	SetLock();
	uclp->Reset(0);
	UCLPCBNSelChange();
}

void TEditAddressDialog::UCLPCBNSelChange()
{
	street->Reset(0);
}

void TEditAddressDialog::AddUCLPBNClicked()
{
	uclp->Add();
}

void TEditAddressDialog::EditUCLPBNClicked()
{
	uclp->Edit();
}

void TEditAddressDialog::AddStreetBNClicked()
{
	street->Add();
}

void TEditAddressDialog::EditStreetBNClicked()
{
	street->Edit();
}

void TEditAddressDialog::SearchBNClicked()
{
	if (uclp->Search())
	{
		Address()->uclp = UCLP->uclp;
		Address()->street = 0;
		SetData();
	}
}
