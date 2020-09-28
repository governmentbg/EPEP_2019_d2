UPDATE T_SURROUND SET F_JUDGE = NULL WHERE F_JUDGE = '0000000206' AND (NOT F_LAWSUIT_TYPE = 'd' OR NOT F_KIND = 'o');
UPDATE T_SURROUND SET F_JUDGE = NULL WHERE F_JUDGE = '0000000206';
CREATE TABLE T_WHAT
(
 F_WHAT INTEGER NOT NULL,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_WHAT PRIMARY KEY (F_WHAT),
 CONSTRAINT C_WHAT_WHAT CHECK ((F_WHAT BETWEEN 1 AND 99999) OR (F_WHAT IS NULL))
);
UPDATE T_SUBJECT SET F_NAME = 'Регистрация на сдружение, фондация, читалище, синдикална и работодателска организация'
WHERE F_SUBJECT = 2220000 AND F_TYPE = 'd';
UPDATE T_SUBJECT SET F_NAME = 'Регистрация на сдружение, фондация, читалище, синдикална и работодателска организация'
WHERE F_SUBJECT = 2220000 AND F_TYPE = 't';
UPDATE T_SUBJECT SET F_NAME = 'Отказ за вписване на промени на сдружение, фондация, читалище, синдикална и работодателска организация'
WHERE F_SUBJECT = 2221000 AND F_TYPE = 't';
ALTER TABLE T_EPORT_GUID ADD F_STAMP TIMESTAMP;
