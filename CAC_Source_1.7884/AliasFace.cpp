DEFINE_RESPONSE_TABLE1(TAliasFace, TGroupComboBox)
//{{TAliasFaceRSP_TBL_BEGIN}}
	EV_MESSAGE(WM_FACE_VALID, EvFaceValid),
	EV_MESSAGE(WM_FACE_NAME, EvFaceName),
	EV_NOTIFY_AT_CHILD(CBN_DROPDOWN, CBNDropDown),
//{{TAliasFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAliasFace Implementation}}

TAliasFace::TAliasFace(TWindow* parent, int resourceId, const char *tName, TAliasGroup *tAlias, const char *tXCrit,
	long tFlags, TGroupArray *array)
:
	TGroupComboBox(parent, resourceId, array, tAlias->NameField->Size), name(tName), alias(tAlias), xCrit(tXCrit),
		flags(tFlags), dropDown(false)
{
	if ((flags & ALIAS_DROPDOWN) == ALIAS_DROPDOWN)
	{
		dropDown = true;
		flags &= ~ALIAS_DROPDOWN;
	}

	if (flags & ALIAS_ENABLE)
		autoEnable = true;

	if (flags & ALIAS_ABCSORT)
		sortcol = 0;
}

TAliasFace::~TAliasFace()
{
}

void TAliasFace::SetXCrit(const char *tXCrit)
{
	xCrit = tXCrit;
	Refresh();
}

void TAliasFace::GetText()
{
	TGroupComboBox::GetText(text, sizeof text);
	strmove(text, ABCSkip(text, true));
}

int TAliasFace::FindVisualIndex()
{
	int index = TGroupComboBox::GetVisualIndex();
	mstr m;

	GetText();

	if (index >= 0)
	{
		GetSearchText(m, index);
		if (strcmp(ABCSkip(str(m), true), text))
			index = -1;
	}

	if (index == -1)
	{
		for (int i = 0; i < array->Count(); i++)
		{
			GetSearchText(m, i);
			if (!strcmp(ABCSkip(str(m), true), text))
			{
				if (index == -1)
					index = i;
				else
				{
					index = -2;
					break;
				}
			}
		}

		if (index >= 0)
			SetIndexNotify(index);
	}

	return index;
}

void TAliasFace::SetIndexNotify(int index)
{
	SetVisualIndex(index);
	Parent->PostMessage(WM_COMMAND, MAKEWPARAM(Attr.Id, CBN_SELCHANGE), (LPARAM) Handle);
}

void TAliasFace::SetDefaultIndex(int index)
{
	SetVisualIndex(index < array->Count() ? index : 0 - dropDown);
}

TGroup *TAliasFace::NewGroup() const
{
	return alias->clone();
}

void TAliasFace::LoadData(const char *s)
{
	if (!alias->LoadCache(this, xCrit, flags))
		TGroupList::LoadData(s);

	if (InsertEmptyOption())
		InsertEmpty();
}

void TAliasFace::Refresh()
{
	if (dropDown)
		TGroupList::Refresh();
	else
	{
		TGroupComboBox::Refresh();

		if (!array->Count())
		{
			// 2007:029 LRQ: better message, 2007:029 LPR: try to restart
			fatal("%s: номенклатурата не е попълнена (липсва "
				"стойност за полето %s). %s.",
				alias->Name, name, can_admin(RIGHT_NOM) ?
				"Редактирайте номенклатурата и изпълнете функцията отново" :
				"Обърнете се към администратора на системата");
		}
	}
}

bool TAliasFace::IsValid()
{
	if (dropDown)
	{
		int index = FindVisualIndex();

		if (index == -2)
			return error("Има два или повече елемента \"%s\". Изберете един от тях.", text);

		if (index == -1 && any(text))
			return error("Няма елемент \"%s\". Изберете елемент или въведете точния текст.", text);

		return true;
	}

	return GetVisualIndex() >= 0 ? true : error("Невалидна стойност на списъчното поле %s.", name);
}

void TAliasFace::FlushVisuals()
{
	// reset drop list height
	if (dropDown && array->Count() >= 2)
	{
		ClearList();
		AddString("");
	}

	TGroupComboBox::FlushVisuals();
}

bool TAliasFace::InsertEmptyOption()
{
	// 2010:267 LPR: support for ALIAS_OPTION
	return (flags & ALIAS_EMPTY) || (!array->Count() && (flags & ALIAS_OPTION));
}

void TAliasFace::SetupWindow()
{
	if (sortcol == -1 && !array->Sorted())
	{
		if (alias->OrderField())
		{
			delete array;
			array = new TCharAliasArray;
		}
		else
			sortcol = 0;
	}

#if TYPECHECK || RANGECHECK
	if ((Attr.Style & CBS_TYPE) != (dropDown ? CBS_DROPDOWN : CBS_DROPDOWNLIST))
		fatal("%s: invalid drop down type", name);
#endif  // TYPECHECK || RANGECHECK

	TGroupComboBox::SetupWindow();

	if (dropDown)
		autoEnable = false;
}

long TAliasFace::EvFaceValid(TParam1, TParam2)
{
	return IsValid() ? 0 : MW_INVALID_FACE;
}

long TAliasFace::EvFaceName(TParam1, TParam2 lParam)
{
	*(const char **) lParam = name;
	return MW_NAMED_FACE;
}

void TAliasFace::CBNDropDown()
{
	if (dropDown)
		SetWaitCursor(false);
}
