//{{THostNameValidator Implementation}}

THostNameValidator::THostNameValidator()
:
	TFilterValidator("%\\-.0-9A-Z_-z\x81-\xFF")
{
}
