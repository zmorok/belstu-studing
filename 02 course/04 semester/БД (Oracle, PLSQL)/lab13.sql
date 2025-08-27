-- C__DSZ_PDB

-- 1. вывод преподов на кафедре
DECLARE
   PROCEDURE get_teachers (pcode teacher.pulpit%TYPE) IS
   BEGIN
      
      FOR t IN (SELECT teacher_name FROM teacher WHERE pulpit = pcode ORDER BY teacher_name)
      LOOP
         DBMS_OUTPUT.put_line(t.teacher_name);
      END LOOP;
   END;
BEGIN
   DBMS_OUTPUT.put_line('Преподаватели кафедры ИСиТ:');
   get_teachers('ИСиТ');
END;
/

-- 2. кол-во преподов на кафедре
DECLARE
   FUNCTION get_num_teachers (pcode teacher.pulpit%TYPE)
   RETURN NUMBER
   IS
      v_cnt NUMBER;
   BEGIN
      SELECT COUNT(*) INTO v_cnt FROM teacher WHERE pulpit = pcode;
      RETURN v_cnt;
   END;
BEGIN
   DBMS_OUTPUT.put_line('Число преподавателей на кафедре ИСиТ = ' || get_num_teachers('ИСиТ'));
END;
/

-- 3
-- 3.1.
CREATE OR REPLACE PROCEDURE get_teachers (fcode faculty.faculty%TYPE)
IS BEGIN
   FOR t IN (SELECT te.teacher_name FROM teacher te JOIN pulpit pu ON te.pulpit = pu.pulpit WHERE pu.faculty = fcode ORDER BY te.teacher_name)
   LOOP
      DBMS_OUTPUT.put_line(t.teacher_name);
   END LOOP;
END;
/

-- DROP PROCEDURE get_teachers;

BEGIN
   DBMS_OUTPUT.put_line('Преподаватели факультета ИДиП:');
   -- ХТиТ ЛХФ ИЭФ ТТЛП ТОВ    
   get_teachers('ИДиП');
END;
/

-- 3.2.
CREATE OR REPLACE PROCEDURE get_subjects
   (pcode subject.pulpit%TYPE)
IS
BEGIN
   FOR s IN (SELECT subject_name FROM subject WHERE pulpit = pcode ORDER BY subject_name)
   LOOP
      DBMS_OUTPUT.put_line(s.subject_name);
   END LOOP;
END;
/

-- DROP PROCEDURE get_subjects;

BEGIN
   DBMS_OUTPUT.put_line('Дисциплины кафедры ИСиТ:');
   get_subjects('ИСиТ');
END;
/

-- 4
-- 4.1.
CREATE OR REPLACE FUNCTION get_num_teachers(fcode faculty.faculty%TYPE)
RETURN NUMBER IS
   v_cnt NUMBER;
BEGIN
   SELECT COUNT(*) INTO v_cnt FROM teacher te JOIN pulpit pu ON te.pulpit = pu.pulpit WHERE pu.faculty = fcode;
   RETURN v_cnt;
END;
/

DECLARE
   v NUMBER;
BEGIN
   v := get_num_teachers('ИДиП');
   DBMS_OUTPUT.put_line('На факультете ИДиП работает ' || v || ' преподавателей');
END;
/

-- 4.2.
CREATE OR REPLACE FUNCTION get_num_subjects(pcode subject.pulpit%TYPE)
RETURN NUMBER IS
   v_cnt NUMBER;
BEGIN
   SELECT COUNT(*) INTO v_cnt FROM subject WHERE pulpit = pcode;
   RETURN v_cnt;
END;
/

BEGIN
   DBMS_OUTPUT.put_line('Количество дисциплин кафедры ИСиТ = ' || get_num_subjects('ИСиТ'));
END;
/

-- 5
CREATE OR REPLACE PACKAGE teachers IS
   PROCEDURE get_teachers (fcode faculty.faculty%TYPE);
   PROCEDURE get_subjects (pcode subject.pulpit%TYPE);
   FUNCTION get_num_teachers (fcode faculty.faculty%TYPE) RETURN NUMBER;
   FUNCTION get_num_subjects (pcode subject.pulpit%TYPE) RETURN NUMBER;
END teachers;
/

CREATE OR REPLACE PACKAGE BODY teachers IS
   PROCEDURE get_teachers (fcode faculty.faculty%TYPE) IS
      i NUMBER := 1;
   BEGIN
      FOR t IN (SELECT te.teacher_name
                FROM teacher te
                JOIN pulpit pu ON te.pulpit = pu.pulpit
                WHERE pu.faculty = fcode
                ORDER BY te.teacher_name)
      LOOP
         DBMS_OUTPUT.put_line(i || '.' || CHR(9) || t.teacher_name);
         i := i + 1;
      END LOOP;
   END;

   PROCEDURE get_subjects (pcode subject.pulpit%TYPE) IS
     i NUMBER := 1;
   BEGIN
      FOR s IN (SELECT subject_name
                FROM subject
                WHERE pulpit = pcode
                ORDER BY subject_name)
      LOOP
         DBMS_OUTPUT.put_line(i || '.' || CHR(9) ||s.subject_name);
         i := i + 1;
      END LOOP;
   END;

   FUNCTION get_num_teachers(fcode faculty.faculty%TYPE)
   RETURN NUMBER IS
      v_cnt NUMBER;
   BEGIN
      SELECT COUNT(*) INTO v_cnt FROM teacher te JOIN pulpit pu ON te.pulpit = pu.pulpit WHERE pu.faculty = fcode;
      RETURN v_cnt;
   END;

   FUNCTION get_num_subjects(pcode subject.pulpit%TYPE)
   RETURN NUMBER IS
      v_cnt NUMBER;
   BEGIN
      SELECT COUNT(*) INTO v_cnt FROM subject WHERE pulpit = pcode;
      RETURN v_cnt;
   END;

END teachers;
/

-- 6
BEGIN
   DBMS_OUTPUT.put_line('=== Факультет ИДиП ===');
   teachers.get_teachers('ИДиП');
   DBMS_OUTPUT.put_line('Всего преподавателей: ' || teachers.get_num_teachers('ИДиП'));

   DBMS_OUTPUT.put_line(CHR(10) || '=== Кафедра ИСиТ ===');
   teachers.get_subjects('ИСиТ');
   DBMS_OUTPUT.put_line('Всего дисциплин: ' || teachers.get_num_subjects('ИСиТ'));
END;
/

-- DROP PACKAGE teachers;