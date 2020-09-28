//{{TExecListCollectListDialog Implementation}}

TExecListCollectListDialog::TExecListCollectListDialog(TWindow* parent, TPrintExecList *tExecList, int resId)
:
	TGroupListDialog(parent, resId), execList(tExecList)
{
	header1 = new TStatic(this, IDC_EXEC_LIST_COLLECT_LIST_HEADER_1);
	header2 = new TStatic(this, IDC_EXEC_LIST_COLLECT_LIST_HEADER_2);
	header3 = new TStatic(this, IDC_EXEC_LIST_COLLECT_LIST_HEADER_3);
	collects = new TExecListCollectListWindow(this, IDC_GROUP_LIST_WINDOW, execList);
}

void TExecListCollectListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	print_exec_list_set_headers(execList, header1, header2, header3);
}
