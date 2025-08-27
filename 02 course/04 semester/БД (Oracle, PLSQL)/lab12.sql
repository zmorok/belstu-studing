/*
ALTER TABLE C##DSZ.TEACHER ADD (
  BIRTHDAY DATE,
  SALARY NUMBER(8,2)
);

ALTER TABLE C##DSZ.TEACHER DROP COLUMN BIRTHDAY;
ALTER TABLE C##DSZ.TEACHER DROP COLUMN SALARY;

SELECT * FROM C##DSZ.TEACHER;

DECLARE
   l_seed NUMBER;
BEGIN
   l_seed := TO_NUMBER(TO_CHAR(SYSTIMESTAMP, 'YYYYMMDD'));
   DBMS_RANDOM.SEED(l_seed);
   UPDATE C##DSZ.TEACHER
      SET birthday = TRUNC(SYSDATE - DBMS_RANDOM.VALUE(8000, 20000)),
          salary   = ROUND(DBMS_RANDOM.VALUE(30000, 100000), 2);
   COMMIT;
END;
/

ALTER SESSION SET CONTAINER = DSZ_PDB;

*/  

-- 2. преподаватели, родившиеся в понедельник
DECLARE
  CURSOR c_monday IS
    SELECT SUBSTR(teacher_name, 1, INSTR(teacher_name, ' ') - 1) 
           || ' ' || SUBSTR(teacher_name, INSTR(teacher_name, ' ', 1, 1) + 1, 1) || '. '
           || SUBSTR(teacher_name, INSTR(teacher_name, ' ', 1, 2) + 1, 1) || '.'      
            AS fio
    FROM C##DSZ.TEACHER
    WHERE TRIM(UPPER(TO_CHAR(BIRTHDAY,'DAY'))) = 'ПОНЕДЕЛЬНИК';
  r c_monday%ROWTYPE;
BEGIN
  DBMS_OUTPUT.PUT_LINE('1. Преподаватели, родившиеся в понедельник:');
  FOR r IN c_monday LOOP
    DBMS_OUTPUT.PUT_LINE(' - ' || r.fio);
  END LOOP;
END;
/

-- 3. преподаватели, родившиеся в следующем месяце
CREATE OR REPLACE VIEW v_teach_next_month AS
SELECT TEACHER, TEACHER_NAME, TO_CHAR(BIRTHDAY,'DD/MM/YYYY') AS BIRTHDAY_FMT
FROM C##DSZ.TEACHER
WHERE TO_CHAR(BIRTHDAY,'MM') = TO_CHAR(ADD_MONTHS(SYSDATE,1),'MM');

SELECT * FROM v_teach_next_month;
DROP VIEW v_teach_next_month;


-- 4. число преподавателей, родившихся в каждом месяце
CREATE OR REPLACE VIEW v_cnt_by_month AS
SELECT TO_CHAR(BIRTHDAY,'MONTH') AS month_name, COUNT(*) AS cnt
FROM C##DSZ.TEACHER
GROUP BY TO_CHAR(BIRTHDAY,'MM'), TO_CHAR(BIRTHDAY,'MONTH')
ORDER BY TO_CHAR(BIRTHDAY,'MM');

SELECT month_name, cnt FROM v_cnt_by_month;
DROP VIEW v_cnt_by_month;

-- 5. преподаватели, у которых в следующем году юбилей (кратный 5 лет), и сколько лет исполнится
DECLARE
  v_next_year  NUMBER := EXTRACT(YEAR FROM SYSDATE) + 1;
  CURSOR c_jub IS SELECT TEACHER_NAME,
                         BIRTHDAY,
                         v_next_year - EXTRACT(YEAR FROM BIRTHDAY) AS age_next
                         FROM C##DSZ.TEACHER
                         WHERE MOD(v_next_year - EXTRACT(YEAR FROM BIRTHDAY),5) = 0;
BEGIN
  DBMS_OUTPUT.PUT_LINE('5. Преподаватели, у которых в следующем году юбилей ('||v_next_year||') :');
  FOR r IN c_jub LOOP
    DBMS_OUTPUT.PUT_LINE(' - '||r.teacher_name||' — исполнится '||r.age_next||' лет' || '  ('||r.birthday||');');
  END LOOP;
END;
/

-- 6. средняя зарплата по кафедрам, по факультетам и в целом
DECLARE
  CURSOR c_avg IS
    SELECT 'Средняя зарплата у кафедры ' as n, rtrim(PULPIT) || CHR(9) as g, CEIL(AVG(SALARY)) as v
    FROM C##DSZ.TEACHER GROUP BY PULPIT
    UNION ALL
    SELECT 'Средняя зарплата у факультета ' as n, rtrim(F.FACULTY) || chr(9) as g, CEIL(AVG(T.SALARY)) as v
    FROM C##DSZ.TEACHER T JOIN C##DSZ.PULPIT P ON T.PULPIT = P.PULPIT
                          JOIN C##DSZ.FACULTY F ON P.FACULTY = F.FACULTY
                          GROUP BY F.FACULTY
    UNION ALL
    SELECT 'Средняя зарплата' as n, 'по всем факультетам' as g, CEIL(AVG(SALARY)) as v FROM C##DSZ.TEACHER;
BEGIN
  FOR r IN c_avg LOOP
    DBMS_OUTPUT.PUT_LINE(r.n || ' ' || r.g || CHR(9) || r.v);
  END LOOP;
END;
/

-- 7. деление двух переменных, обработка ZERO_DIVIDE и пользовательская ошибка
DECLARE
  v_num NUMBER := 10;
  v_den NUMBER := 0;
  v_res NUMBER;
BEGIN
  IF v_den = 0 THEN
    RAISE_APPLICATION_ERROR(-20001,'(Кастомная) Делитель не может быть нулем.');
  END IF;

  v_den := 0;
  v_res := v_num / v_den;
  DBMS_OUTPUT.PUT_LINE('Результат: '||v_res);

EXCEPTION
  WHEN ZERO_DIVIDE THEN
    DBMS_OUTPUT.PUT_LINE('Ошибка ZERO_DIVIDE: деление на ноль.');
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('Другая ошибка: '||SQLERRM);
END;
/

-- 8. обработка NO_DATA_FOUND
DECLARE
  v_code C##DSZ.TEACHER.TEACHER%TYPE := '1СМЛВ';
  v_name C##DSZ.TEACHER.TEACHER_NAME%TYPE;
BEGIN
  SELECT teacher_name INTO v_name FROM C##DSZ.TEACHER WHERE teacher = v_code;

  DBMS_OUTPUT.PUT_LINE('Найден: '||v_name);

EXCEPTION
  WHEN NO_DATA_FOUND THEN
    DBMS_OUTPUT.PUT_LINE('Преподаватель ' || v_code || ' не найден');
END;
/

-- 9. связка PRAGMA EXCEPTION_INIT
DECLARE
  ex_main EXCEPTION;
  PRAGMA EXCEPTION_INIT(ex_main, -20001);
BEGIN

  DECLARE
    ex_nested EXCEPTION;
    PRAGMA EXCEPTION_INIT(ex_nested, -20001);
  BEGIN
    RAISE_APPLICATION_ERROR(-20001,'Ошибка во вложенном блоке');
  EXCEPTION
    WHEN OTHERS THEN
      RAISE;
  END;

EXCEPTION
  WHEN ex_main THEN
    DBMS_OUTPUT.PUT_LINE('Внешний блок поймал ex_main: '||SQLERRM);
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('Внешний блок — другое исключение: '||SQLERRM);
END;
/

-- 9.1. без PRAGMA
DECLARE
  ex_dup EXCEPTION;
BEGIN
    DECLARE
      ex_dup EXCEPTION;
    BEGIN
      RAISE ex_dup;
    END;
  EXCEPTION
    WHEN ex_dup THEN
      DBMS_OUTPUT.PUT_LINE('Внешний блок поймал ex_dup');
    WHEN OTHERS THEN
      DBMS_OUTPUT.PUT_LINE('Исключение = ' || sqlerrm);
END;
/

-- 10. генерируется ли NO_DATA_FOUND для агрегатных функций
DECLARE
  v_mx NUMBER;
BEGIN
  SELECT MAX(salary) INTO v_mx FROM C##DSZ.TEACHER WHERE salary < 0;

  IF v_mx IS NULL THEN
    DBMS_OUTPUT.PUT_LINE('MAX вернул NULL, NO_DATA_FOUND не сработал.');
  ELSE
    DBMS_OUTPUT.PUT_LINE('MAX = '||v_mx);
  END IF;

EXCEPTION
  WHEN NO_DATA_FOUND THEN
    DBMS_OUTPUT.PUT_LINE('NO_DATA_FOUND поймано');
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('Другая ошибка: '||SQLERRM);
END;
/