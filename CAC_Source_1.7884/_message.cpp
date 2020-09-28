#include "cac.h"

bool unknown_finished_law_status (const TRCDKey &key)
 {
  char lawKeyStr [64] ;

  ostrstream (lawKeyStr, sizeof (lawKeyStr))
    << TRCDKey_ (key) << ends ;
  lawKeyStr [sizeof (lawKeyStr) - 1] = '\0' ;
  error ("%s:\n������ � ��������, �� �� ���� �� �������� ���� � "
         "������ ��� ����������.", lawKeyStr) ;
  return false ;
 }  // unknown_finished_law_status

void warn_text_too_wide_A4_portrait_doc (char *tText)
 {
  const int symbols = longest_text_line_symbols (tText) ;
  const char * const tooWide = "���������� � ����� ����� � " ;

  if (symbols <= A4_PORTRAIT_CONDENSED_SYMBOLS)
    return ;

  if (WINDOWS_PRINT)
    info ("%s���� ���� �� ������ �� ������������� �� "
	    "� ��-������ �����.", tooWide);
  else
    info ("%s%s���� �� �� ������ �� ������ A4 (�������).", tooWide,
	    symbols < A4_MAX_PORTRAIT_CONDENS_SYMBOLS ? "���� �� " : "") ;
 }  // warn_too_wide_A4_portrait_doc

bool confirm_unwritten_acts_search ()
 {
  return ask ("�� ������ �� ���������� �� ������������ ������� ������?"
//              " %s.", RESULT_WILL_BE_DELAYED  // 31.may.2001
		 ) ;
 }  // confirm_unwritten_acts_search ()

bool incorrect_period_given (TWindow * parent, int childId)
 {
  bool argsOK = parent && childId ;
  char tmp [128] ;
  const size_t maxL = sizeof (tmp) - 1 ;

  tmp [0] = tmp [maxL] = '\0' ;
  if (argsOK)
    if (parent -> GetDlgItemText (childId, tmp, maxL) <= 0)
     {
	tmp [0] = '\0' ;
	argsOK = false ;
     }
  return error ("%s%s\"%s\".",
		    INCORRECT_PERIOD_GIVEN, argsOK ? " �� " : "",
		    difference (tmp, "&")) ;
 }  // incorrect_period_given


