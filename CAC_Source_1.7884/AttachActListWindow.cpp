//{{TAttachActListWindow Implementation}}

TAttachActListWindow::TAttachActListWindow(TWindow* parent, int resourceId, TGArray<TIndexContainer> *array)
:
	TGroupListWindow(parent, resourceId, array, 332)
{
	Add(
		new TGroupListColumn("����", container_event_date, 52, LVCFMT_CENTER),
		new TGroupListColumn("�������", container_event_text, 168),
		new TGroupListColumn("������������ ���������", container_attached, 100),
		NULL
	);
}

void TAttachActListWindow::Attach()
{
	TIndexContainer *container = (TIndexContainer *) Group();

	if (attach_act(this, container))
		RedrawVisual(GetVisualIndex());
}
