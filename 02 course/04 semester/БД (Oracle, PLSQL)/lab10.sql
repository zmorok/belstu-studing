show con_name;
-- DB_ORACLE_FREE

-- 1
BEGIN NULL; END;

-- 2
BEGIN DBMS_OUTPUT.PUT_LINE('Hello World!'); END;

-- 3
select keyword from v_$reserved_words
where length = 1 and keyword != 'A'; -- E H U D T K M P G A 

-- 4
select keyword from v_$reserved_words
where length > 1 and keyword != 'A';

-- 5
DECLARE
  v_int1 NUMBER(2) := 10;
  v_int2 NUMBER(1) := 3;
  v_sum NUMBER;
  v_diff NUMBER;
  v_prod NUMBER;
  v_div NUMBER;
  v_mod NUMBER;

  v_fix1 NUMBER(7,2)  := 123.45;
  v_fix2 NUMBER(7,-2) := 12345;

  v_exp1 NUMBER := 1.23E4;
  v_exp2 NUMBER  := 4.56E-2;

  v_date DATE := TO_DATE('04-05-2025','DD-MM-YYYY');

  v_char CHAR(3) := 'X';
  v_vchar VARCHAR2(6 CHAR) := 'оракле';

  v_bool BOOLEAN := TRUE;
BEGIN
  v_sum  := v_int1 + v_int2;
  v_diff := v_int1 - v_int2;
  v_prod := v_int1 * v_int2;
  v_div  := TRUNC(v_int1 / v_int2);
  v_mod  := MOD(v_int1, v_int2);

  DBMS_OUTPUT.PUT_LINE('v_int1 = '||v_int1);
  DBMS_OUTPUT.PUT_LINE('v_int2 = '||v_int2);
  DBMS_OUTPUT.PUT_LINE('Сумма = '||v_sum);
  DBMS_OUTPUT.PUT_LINE('Разность = '||v_diff);
  DBMS_OUTPUT.PUT_LINE('Произведение = '||v_prod);
  DBMS_OUTPUT.PUT_LINE('Деление = '||v_div);
  DBMS_OUTPUT.PUT_LINE('Остаток = '||v_mod);

  DBMS_OUTPUT.PUT_LINE('fix1 = '||v_fix1);
  DBMS_OUTPUT.PUT_LINE('fix2 = '||v_fix2);

  DBMS_OUTPUT.PUT_LINE('exp1 = '||v_exp1);
  DBMS_OUTPUT.PUT_LINE('exp2 = '||v_exp2);

  DBMS_OUTPUT.PUT_LINE('date = '||TO_CHAR(v_date));
  DBMS_OUTPUT.PUT_LINE('char = '||v_char);
  DBMS_OUTPUT.PUT_LINE('varchar = '||v_vchar);
  if v_bool then DBMS_OUTPUT.PUT_LINE('v_bool = true'); 
  else DBMS_OUTPUT.PUT_LINE('v_bool = false'); end if;
END;

-- 6
DECLARE
  c_str CONSTANT VARCHAR2(6 CHAR) := 'оракле';
  c_char CONSTANT CHAR := 'Z';
  c_num CONSTANT NUMBER := 100;
  v_tmp VARCHAR2(50 CHAR);
  v_num2 NUMBER := 25;
BEGIN
  v_tmp := c_str || c_char;
  DBMS_OUTPUT.PUT_LINE('Конкатенация: '||v_tmp);
  DBMS_OUTPUT.PUT_LINE('Сумма: '||(c_num + v_num2));
  DBMS_OUTPUT.PUT_LINE('Деление: '||(c_num / v_num2));
END;

-- от sys переключаюсь в свою PDB с CDB
-- там таблицы (CUSTOMERS, OFFICES, ORDERS, PRODUCTS, SALESREPS где OWNER = C##DSZ)
alter session set container = DSZ_PDB;
show con_name;

-- 7
DECLARE
  v_ord C##DSZ.ORDERS.ORDER_NUM%TYPE;
BEGIN
  SELECT ORDER_NUM INTO v_ord FROM C##DSZ.ORDERS WHERE ORDER_NUM = 112961;
  DBMS_OUTPUT.PUT_LINE('v_ord = '||v_ord);
END;

-- 8
DECLARE
  v_str C##DSZ.ORDERS%ROWTYPE;
BEGIN
  SELECT * INTO v_str FROM C##DSZ.ORDERS WHERE ORDER_NUM = 112961;
  DBMS_OUTPUT.PUT_LINE('v_str = '|| v_str.ORDER_NUM || ' | ' || v_str.CUST);
END;

-- 9
DECLARE
  v_str C##DSZ.ORDERS%ROWTYPE;
BEGIN
    SELECT * INTO v_str FROM C##DSZ.ORDERS WHERE ORDER_NUM = 112961;
  
  IF v_str.ORDER_NUM = 112961 THEN
    DBMS_OUTPUT.PUT_LINE('ORDER_NUM корректен');
  END IF;

  IF v_str.MFR = 'REI' THEN
    DBMS_OUTPUT.PUT_LINE('MFR это REI');
  ELSE
    DBMS_OUTPUT.PUT_LINE('другой MFR');
  END IF;

  IF v_str.QTY <= 0 THEN
    DBMS_OUTPUT.PUT_LINE('количество <= 0');
  ELSIF v_str.QTY BETWEEN 1 AND 9 THEN
    DBMS_OUTPUT.PUT_LINE('количество от 1 до 9');
  ELSE
    DBMS_OUTPUT.PUT_LINE('количество >= 10');
  END IF;
END;

-- 10
DECLARE
  v_qt C##DSZ.ORDERS.QTY%TYPE;
  str VARCHAR2(20 CHAR) := 'количество= ';
  str_qt VARCHAR2(10 CHAR);
BEGIN
  SELECT QTY INTO v_qt FROM C##DSZ.ORDERS WHERE ORDER_NUM = 112961;
  CASE v_qt
    WHEN 1 THEN str_qt := 'один';
    WHEN 2 THEN str_qt := 'два';
    WHEN 3 THEN str_qt := 'три';
    WHEN 4 THEN str_qt := 'четыре';
    WHEN 5 THEN str_qt := 'пять';
    WHEN 6 THEN str_qt := 'шесть';
    WHEN 7 THEN str_qt := 'семь';
    WHEN 8 THEN str_qt := 'восемь';
    WHEN 9 THEN str_qt := 'девять';
    ELSE str_qt := 'неизвсетно';
  END CASE;
  DBMS_OUTPUT.PUT_LINE(str||str_qt);
END;   

-- 11
DECLARE
    chislo number := 1;
BEGIN
  LOOP
    EXIT WHEN chislo > 10;
      DBMS_OUTPUT.PUT_LINE(chislo ** 2);
      chislo := chislo + 1;
  END LOOP;
END;

-- 12
DECLARE
  chislo number := 1;
BEGIN
  WHILE chislo < 11 LOOP
    DBMS_OUTPUT.PUT_LINE('chislo = '||chislo);
    chislo := chislo + 1;
  END LOOP;
END;

-- 13
BEGIN
  FOR chislo IN 1..10 
  LOOP
    DBMS_OUTPUT.PUT_LINE(chislo ** 4);
  END LOOP;
END;


  