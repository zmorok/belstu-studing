-- oracle_tables_v2
alter session set container = DSZ_PDB;
show con_name;

-- 1
DECLARE
  s C##DSZ.AUDITORIUM_TYPE%ROWTYPE;
BEGIN
  SELECT * INTO s FROM C##DSZ.AUDITORIUM_TYPE WHERE AUDITORIUM_TYPE = 'ЛК';
  dbms_output.put_line('s= ' ||  rtrim(s.AUDITORIUM_TYPE) || ' -- ' ||  rtrim(s.AUDITORIUM_TYPENAME));
END;

-- 2
DECLARE
  s C##DSZ.AUDITORIUM_TYPE%ROWTYPE;
BEGIN
  SELECT * INTO s FROM C##DSZ.AUDITORIUM_TYPE WHERE AUDITORIUM_TYPE like 'ЛК%';
  dbms_output.put_line('s= ' ||  rtrim(s.AUDITORIUM_TYPE) || ' -- ' ||  rtrim(s.AUDITORIUM_TYPENAME));
  EXCEPTION
    WHEN OTHERS
        then dbms_output.put_line('ошибка: ' || sqlerrm);
        dbms_output.put_line('код: ' || sqlcode);
END;

-- 3
DECLARE
  s C##DSZ.AUDITORIUM_TYPE%ROWTYPE;
BEGIN
  SELECT * INTO s FROM C##DSZ.AUDITORIUM_TYPE WHERE AUDITORIUM_TYPE like 'ЛК%';
  dbms_output.put_line('s= ' ||  rtrim(s.AUDITORIUM_TYPE) || ' -- ' ||  rtrim(s.AUDITORIUM_TYPENAME));
  EXCEPTION
    WHEN TOO_MANY_ROWS
        then dbms_output.put_line('слишком много строк для выборки');
  
    WHEN OTHERS
        then dbms_output.put_line('ошибка: ' || sqlerrm);
        dbms_output.put_line('код: ' || sqlcode);
END;

-- 4
DECLARE
  s C##DSZ.AUDITORIUM_TYPE%ROWTYPE;
  p char(10);
BEGIN
  p := 'ЛКv';
  SELECT * INTO s FROM C##DSZ.AUDITORIUM_TYPE WHERE rtrim(AUDITORIUM_TYPE) like rtrim(p);
  dbms_output.put_line('s= ' ||  rtrim(s.AUDITORIUM_TYPE) || ' -- ' ||  rtrim(s.AUDITORIUM_TYPENAME));
  EXCEPTION
    WHEN TOO_MANY_ROWS
        then dbms_output.put_line('слишком много строк для выборки');
        
    WHEN NO_DATA_FOUND
        then dbms_output.put_line('аудиторий с типом ' || p || ' нет');
  
    WHEN OTHERS
        then dbms_output.put_line('ошибка: ' || sqlerrm);
        dbms_output.put_line('код: ' || sqlcode);
END;

-- 5
DECLARE
  s C##DSZ.AUDITORIUM_TYPE%ROWTYPE;
  p char(10);
BEGIN
  p := 'ЛК';
  SELECT * INTO s FROM C##DSZ.AUDITORIUM_TYPE WHERE rtrim(AUDITORIUM_TYPE) like rtrim(p);
  dbms_output.put_line('s= ' ||  rtrim(s.AUDITORIUM_TYPE) || ' -- ' ||  rtrim(s.AUDITORIUM_TYPENAME));
  
  insert into C##DSZ.AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME) values (rtrim(s.AUDITORIUM_TYPE) || '1', rtrim(s.AUDITORIUM_TYPENAME) || '1');
  dbms_output.put_line('вставлено');
  
  delete from C##DSZ.AUDITORIUM_TYPE WHERE rtrim(AUDITORIUM_TYPE) like rtrim(p) || '1';
  dbms_output.put_line('удалено');
  
  update C##DSZ.AUDITORIUM_TYPE set AUDITORIUM_TYPE = 'ЛКК' where rtrim(AUDITORIUM_TYPE) like rtrim(p) || '1';
  dbms_output.put_line('обновлено');
  
  insert into C##DSZ.AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME) values ('', '');
  dbms_output.put_line('вставлено');
  EXCEPTION
    WHEN TOO_MANY_ROWS
        then dbms_output.put_line('слишком много строк для выборки');
        rollback;
        
    WHEN NO_DATA_FOUND
        then dbms_output.put_line('аудиторий с типом ' || p || ' нет');
        rollback;
        
    WHEN OTHERS
        then dbms_output.put_line('ошибка: ' || sqlerrm);
        dbms_output.put_line('код: ' || sqlcode);
        rollback;
END;

-- 6
DECLARE
    cursor cur is select TEACHER, TEACHER_NAME, PULPIT from C##DSZ.TEACHER;
    t_id C##DSZ.TEACHER.TEACHER%TYPE;
    t_name C##DSZ.TEACHER.TEACHER_NAME%TYPE;
    t_p C##DSZ.TEACHER.PULPIT%TYPE;
BEGIN
    OPEN cur;
    dbms_output.put_line('TEACHER   ' ||'TEACHER_NAME   '||'PULPIT');
    LOOP
        FETCH cur INTO t_id, t_name, t_p;
        EXIT WHEN cur%notfound;
        dbms_output.put_line(t_id|| ' ' || t_name || ' ' || rtrim(t_p));
    END LOOP;
    CLOSE cur;
END;

-- 7
DECLARE
    cursor cur is select SUBJECT, SUBJECT_NAME, PULPIT from C##DSZ.SUBJECT;
    s C##DSZ.SUBJECT%rowtype;
BEGIN
    OPEN cur;
    dbms_output.put_line('SUBJECT   ' ||'SUBJECT_NAME   '||'PULPIT');
    FETCH cur INTO s;
    WHILE cur%found
    LOOP
        dbms_output.put_line(s.SUBJECT|| ' ' || s.SUBJECT_NAME || ' ' || rtrim(s.PULPIT));
        FETCH cur INTO s;
    END LOOP;
    CLOSE cur;
END;

-- 8
DECLARE
  CURSOR c_aud(p_min NUMBER, p_max NUMBER) IS
    SELECT auditorium, auditorium_name, auditorium_capacity, auditorium_type
    FROM C##DSZ.auditorium
    WHERE (p_min IS NULL OR auditorium_capacity >= p_min)
    AND (p_max IS NULL OR auditorium_capacity <= p_max)
    ORDER BY auditorium_capacity;

  l_rec   c_aud%ROWTYPE;
BEGIN
  -- 1
  DBMS_OUTPUT.PUT_LINE(CHR(10) || 'Аудитории с вместимостью < 20:');
  OPEN c_aud(NULL, 19);
  LOOP
    FETCH c_aud INTO l_rec;
    EXIT WHEN c_aud%NOTFOUND;
    DBMS_OUTPUT.PUT_LINE(l_rec.auditorium || ' - ' || l_rec.auditorium_name || ' (' || l_rec.auditorium_capacity || ')');
  END LOOP;
  CLOSE c_aud;

  -- 2
  DBMS_OUTPUT.PUT_LINE(CHR(10) || 'Аудитории с вместимостью 21–30:');
  FOR r IN c_aud(21, 30) LOOP
    DBMS_OUTPUT.PUT_LINE(r.auditorium || ' - ' || r.auditorium_name || ' (' || r.auditorium_capacity || ')');
  END LOOP;

  -- 3
  DBMS_OUTPUT.PUT_LINE(CHR(10) || 'Аудитории с вместимостью 31–60:');
  OPEN c_aud(31, 60);
  FETCH c_aud INTO l_rec;
  WHILE c_aud%FOUND LOOP
    DBMS_OUTPUT.PUT_LINE(l_rec.auditorium || ' - ' || l_rec.auditorium_name || ' (' || l_rec.auditorium_capacity || ')');
    FETCH c_aud INTO l_rec;
  END LOOP;
  CLOSE c_aud;

  -- 1.2
  DBMS_OUTPUT.PUT_LINE(CHR(10) || 'Аудитории с вместимостью 61–80:');
  OPEN c_aud(61, 80);
  LOOP
    FETCH c_aud INTO l_rec;
    EXIT WHEN c_aud%NOTFOUND;
    DBMS_OUTPUT.PUT_LINE(l_rec.auditorium || ' - ' || l_rec.auditorium_name || ' (' || l_rec.auditorium_capacity || ')');
  END LOOP;
  CLOSE c_aud;

  -- 2.2 
  DBMS_OUTPUT.PUT_LINE(CHR(10) || 'Аудитории с вместимостью ≥ 81:');
  FOR r IN c_aud(81, NULL) LOOP
    DBMS_OUTPUT.PUT_LINE(r.auditorium || ' - ' || r.auditorium_name || ' (' || r.auditorium_capacity || ')');
  END LOOP;
END;

-- 9
DECLARE
  v_min_cap  NUMBER := 21;
  v_max_cap  NUMBER := 60;
  cur SYS_REFCURSOR;
  rec C##DSZ.auditorium%ROWTYPE;
BEGIN

  OPEN cur FOR SELECT AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY, AUDITORIUM_TYPE
               FROM C##DSZ.AUDITORIUM
               WHERE AUDITORIUM_CAPACITY BETWEEN v_min_cap AND v_max_cap
               ORDER BY AUDITORIUM_CAPACITY;

  DBMS_OUTPUT.PUT_LINE('Аудитории с вместимостью от '|| v_min_cap || ' до ' || v_max_cap || ':');
  LOOP
    FETCH cur INTO rec;
    EXIT WHEN cur%NOTFOUND;
    DBMS_OUTPUT.PUT_LINE(rec.auditorium || ' - '|| rec.auditorium_name || ' ('|| rec.auditorium_capacity || ')');
  END LOOP;

  CLOSE cur;
END;

-- 10
DECLARE
  CURSOR cur(p_min NUMBER, p_max NUMBER) IS SELECT AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY
                                            FROM C##DSZ.AUDITORIUM
                                            WHERE AUDITORIUM_CAPACITY BETWEEN p_min AND p_max
                                            FOR UPDATE OF AUDITORIUM_CAPACITY;
BEGIN
  DBMS_OUTPUT.PUT_LINE('с 40 до 80 на 10%:');
  FOR rec IN cur(40, 80)
  LOOP
    UPDATE C##DSZ.AUDITORIUM SET AUDITORIUM_CAPACITY = TRUNC(rec.AUDITORIUM_CAPACITY * 0.9) WHERE CURRENT OF cur;
    DBMS_OUTPUT.PUT_LINE('Аудитория ' || rec.AUDITORIUM ||': ' || rec.AUDITORIUM_CAPACITY ||' -> ' || TRUNC(rec.AUDITORIUM_CAPACITY * 0.9));
  END LOOP;
  ROLLBACK;
END;
