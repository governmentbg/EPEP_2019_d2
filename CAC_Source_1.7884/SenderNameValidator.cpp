//{{TSenderNameValidator Implementation}}

TSenderNameValidator::TSenderNameValidator()
:
	//TNameValidator("�-�A-Z0-9 .\"\\-")
	// 2007:036 LRQ: copied from firm name
	// 2016:348 allow lower case
	TNameValidator("�-�A-Z0-9�-�a-z .\"\\-:,&!/")
{
}
