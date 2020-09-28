TAnnounceNumberDialog::TAnnounceNumberDialog(TWindow* parent)
:
	TEditDialog(parent, AnnounceIndex, false, IDD_ANNOUNCE_NUMBER)
{
	new TLongFace(this, IDC_ANNOUNCE_NUMBER_NO, "F_NO", SIZE_OF_NO);
}
