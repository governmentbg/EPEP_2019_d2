/* 20.Jul.2004; 2010:098 -> common_.sql; 2012:167 -> appeal_.sql */
set term ^;

create procedure session_keeping
  (law_type char, law_year integer, law_no integer, sess_kind char, sess_date integer)
  returns (keeping integer)
as
  declare variable law_subject integer;
  declare variable surr_type char;
  declare variable surr_year integer;
  declare variable surr_no integer;
  declare variable surr_kind char;
  declare variable surr_found integer;
begin
  keeping = 0;
  law_subject = 0;
  surr_type = null;
  surr_year = null;
  surr_no = null;
  surr_kind = null;
  surr_found = 0;
  if(law_type is null or law_no is null or law_year is null or
    sess_kind is null or sess_date is null) then
  begin
    suspend;
    exit;
  end
  select f_subject from t_lawsuit where
    f_type = :law_type and f_year = :law_year and f_no = :law_no
    into :law_subject;
  if(law_subject in(9002, 9003)) then
  begin
    keeping = 1;
  end
  else
  begin
    for
      select f_type, f_year, f_no, f_kind from t_surroundment where
      f_lawsuit_type = :law_type and f_lawsuit_year = :law_year and
      f_lawsuit_no = :law_no and
      f_session_kind = :sess_kind and f_date = :sess_date
      order by f_type, f_year, f_no
      into :surr_type, :surr_year, :surr_no, :surr_kind
    do
    begin
      if(surr_kind = '' and surr_type is not null and
        surr_year is not null and surr_no is not null) then
      begin
        select count(f_no) from t_surround where
        f_type = :surr_type and f_year = :surr_year and f_no = :surr_no
        into :surr_found;
        if(surr_found = 1) then
        begin
          keeping = 1;
        end
      end
    end
  end
  suspend;
  exit;
end ^

set term ;^
