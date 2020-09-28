/* 20.Jul.2004; 2010:098 -> common_.sql; 2012:167 -> appeal_.sql */
CREATE FUNCTION session_keeping (law_type CHAR, law_year INTEGER, law_no INTEGER, sess_kind CHAR, sess_date INTEGER)
RETURNS INTEGER AS $$
DECLARE
	law_subject INTEGER;
	surr RECORD;
	surr_found INTEGER;
BEGIN
	law_subject = 0;
	surr_found = 0;

	IF (law_type IS NULL OR law_no IS NULL OR law_year IS NULL OR sess_kind IS NULL OR sess_date IS NULL)
	THEN
		RETURN 0;
	END IF;

	SELECT f_subject FROM t_lawsuit WHERE f_type = law_type AND f_year = law_year AND f_no = law_no INTO law_subject;

	IF (law_subject IN (9002, 9003))
	THEN
		RETURN 1;
	END IF;

	FOR surr IN SELECT f_type, f_year, f_no, f_kind FROM t_surroundment
		WHERE
		f_lawsuit_type = law_type AND f_lawsuit_year = law_year AND
		f_lawsuit_no = law_no AND
		f_session_kind = sess_kind AND f_session_date = sess_date
		ORDER BY f_type, f_year, f_no
	LOOP
		IF (surr.f_kind = 'ù' AND surr.f_type IS NOT NULL AND surr.f_year IS NOT NULL AND surr.f_no IS NOT NULL)
		THEN
			SELECT COUNT(f_no) FROM t_surround WHERE f_type = surr.f_type AND f_year = surr.f_year
			AND f_no = surr.f_no INTO surr_found;

			IF (surr_found = 1)
			THEN
				RETURN 1;
			END IF;
		END IF;
	END LOOP;

	RETURN 0;
END;
$$ LANGUAGE PLPGSQL;
