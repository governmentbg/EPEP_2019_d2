//{{TSenderNameValidator Implementation}}

TSenderNameValidator::TSenderNameValidator()
:
	//TNameValidator("À-ßA-Z0-9 .\"\\-")
	// 2007:036 LRQ: copied from firm name
	// 2016:348 allow lower case
	TNameValidator("À-ßA-Z0-9à-ÿa-z .\"\\-:,&!/")
{
}
