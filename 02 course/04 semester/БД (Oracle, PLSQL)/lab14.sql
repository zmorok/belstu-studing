-- С__DSZ_PDB
-- 1
CREATE TABLE tempt (
  product_id NUMBER CONSTRAINT products_pk PRIMARY KEY,
  product_name VARCHAR2(100),
  price NUMBER(10,2),
  active CHAR(1) DEFAULT 'Y'
);

-- 2
INSERT INTO tempt VALUES (1,  'один', 199.99, 'Y');
INSERT INTO tempt VALUES (2,  'два', 49.99, 'Y');
INSERT INTO tempt VALUES (3,  'три', 24.90, 'Y');
INSERT INTO tempt VALUES (4,  'четыре', 299.00, 'Y');
INSERT INTO tempt VALUES (5,  'пять', 149.00, 'Y');
INSERT INTO tempt VALUES (6,  'шесть', 89.50, 'Y');
INSERT INTO tempt VALUES (7,  'семь', 69.00, 'Y');
INSERT INTO tempt VALUES (8,  'восемь', 39.99, 'Y');
INSERT INTO tempt VALUES (9,  'девять', 29.90, 'Y');
INSERT INTO tempt VALUES (10, 'десять', 9.90, 'Y');
COMMIT;

delete from tempt where product_id = 10;
update tempt set price = price * 1.1;
delete tempt;
select * from tempt;

-- 3
CREATE OR REPLACE TRIGGER tempt_before_statement_tr
BEFORE INSERT OR UPDATE OR DELETE ON tempt
DECLARE
BEGIN
    DBMS_OUTPUT.put_line('3. TRIGGER tempt_before_statement_tr (BEFORE STATEMENT)');
END;
/

-- 4 + 5
CREATE OR REPLACE TRIGGER tempt_before_row_tr
BEFORE INSERT OR UPDATE OR DELETE ON tempt
FOR EACH ROW
DECLARE
BEGIN
    IF INSERTING THEN DBMS_OUTPUT.put_line('4_5. TRIGGER tempt_before_row_tr (BEFORE ROW) - INSERTING');
    ELSIF UPDATING THEN DBMS_OUTPUT.put_line('4_5. TRIGGER tempt_before_row_tr (BEFORE ROW) - UPDATING');
    ELSIF DELETING THEN DBMS_OUTPUT.put_line('4_5. TRIGGER tempt_before_row_tr (BEFORE ROW) - DELETING');
    END IF;
END;
/

-- 6
CREATE OR REPLACE TRIGGER tempt_after_statement_tr
AFTER INSERT OR UPDATE OR DELETE ON tempt
DECLARE
BEGIN
    DBMS_OUTPUT.put_line('6. TRIGGER tempt_after_statement_tr (AFTER STATEMENT)');
END;
/

-- 7
CREATE OR REPLACE TRIGGER tempt_after_row_tr
AFTER INSERT OR UPDATE OR DELETE ON tempt
FOR EACH ROW
DECLARE
BEGIN
    IF INSERTING THEN DBMS_OUTPUT.put_line('7. TRIGGER tempt_after_row_tr (AFTER ROW) - INSERTING');
    ELSIF UPDATING THEN DBMS_OUTPUT.put_line('7. TRIGGER tempt_after_row_tr (AFTER ROW) - UPDATING');
    ELSIF DELETING THEN DBMS_OUTPUT.put_line('7. TRIGGER tempt_after_row_tr (AFTER ROW) - DELETING');
    END IF;
END;
/

-- 8
CREATE TABLE audit_log (
    operation_date TIMESTAMP DEFAULT SYSTIMESTAMP,
    operation_type VARCHAR2(10),
    trigger_name VARCHAR2(30),
    data CLOB
);

-- 9
-- ===== START =====
CREATE OR REPLACE TRIGGER tempt_before_statement_tr
BEFORE INSERT OR UPDATE OR DELETE ON tempt
DECLARE
    operation VARCHAR2(10);
    message CLOB := '3. TRIGGER tempt_before_statement_tr (BEFORE STATEMENT)';
BEGIN
    IF INSERTING THEN operation := 'INSERT';
    ELSIF UPDATING THEN operation := 'UPDATE';
    ELSIF DELETING THEN operation := 'DELETE';
    END IF;
    DBMS_OUTPUT.put_line(message || ' - ' || operation);
    INSERT INTO audit_log(operation_type, trigger_name, data)
        VALUES (operation, 'tempt_before_statement_tr', TO_CLOB(message || ' - ' ||operation));
END;
/

CREATE OR REPLACE TRIGGER tempt_before_row_tr
BEFORE INSERT OR UPDATE OR DELETE ON tempt
FOR EACH ROW
DECLARE
    operation VARCHAR2(10);
    message CLOB := '4_5. TRIGGER tempt_before_row_tr (BEFORE ROW) (';
BEGIN
    IF INSERTING THEN
        operation := 'INSERT';
        message := message || 'INSERTING)';
    ELSIF UPDATING THEN
        operation := 'UPDATE';
        message := message || 'UPDATING)';
    ELSIF DELETING THEN
        operation := 'DELETE';
        message := message || 'DELETING)';
    END IF;
    
    DBMS_OUTPUT.put_line(message || ' - ' || operation);
    INSERT INTO audit_log(operation_type, trigger_name, data)
        VALUES (operation, 'tempt_before_row_tr', TO_CLOB(message || ' - ' ||operation));
END;
/

CREATE OR REPLACE TRIGGER tempt_after_statement_tr
AFTER INSERT OR UPDATE OR DELETE ON tempt
DECLARE
    operation VARCHAR2(10);
    message CLOB := '3. TRIGGER tempt_after_statement_tr (AFTER STATEMENT)';
BEGIN
    IF INSERTING THEN operation := 'INSERT';
    ELSIF UPDATING THEN operation := 'UPDATE';
    ELSIF DELETING THEN operation := 'DELETE';
    END IF;
    DBMS_OUTPUT.put_line(message || ' - ' || operation);
    INSERT INTO audit_log(operation_type, trigger_name, data)
        VALUES (operation, 'tempt_after_statement_tr', TO_CLOB(message || ' - ' ||operation));
END;
/

CREATE OR REPLACE TRIGGER tempt_after_row_tr
AFTER INSERT OR UPDATE OR DELETE ON tempt
FOR EACH ROW
DECLARE
    operation VARCHAR2(10);
    message CLOB := '4_5. TRIGGER tempt_after_row_tr (AFTER ROW) (';
BEGIN
    IF INSERTING THEN
        operation := 'INSERT';
        message := message || 'INSERTING)';
    ELSIF UPDATING THEN
        operation := 'UPDATE';
        message := message || 'UPDATING)';
    ELSIF DELETING THEN
        operation := 'DELETE';
        message := message || 'DELETING)';
    END IF;
    
    DBMS_OUTPUT.put_line(message || ' - ' || operation);
    INSERT INTO audit_log(operation_type, trigger_name, data)
        VALUES (operation, 'tempt_after_row_tr', TO_CLOB(message || ' - ' ||operation));
END;
/
-- ===== END =====

-- 10
SELECT * FROM audit_log;
SELECT * FROM tempt;
INSERT INTO tempt VALUES (1,  'один', 199.99, 'Y');
COMMIT;

-- ====================================
DROP TABLE tempt;
DROP TABLE audit_log;
DROP TRIGGER tempt_before_statement_tr;
DROP TRIGGER tempt_before_row_tr;
DROP TRIGGER tempt_after_statement_tr;
DROP TRIGGER tempt_after_row_tr;
DROP TRIGGER prevent_tempt_drop;
-- ====================================

-- 11
CREATE OR REPLACE TRIGGER prevent_tempt_drop
BEFORE DROP ON SCHEMA
DECLARE
    DROP_EXC EXCEPTION;
    PRAGMA EXCEPTION_INIT(DROP_EXC, -20001);
BEGIN
    IF ORA_DICT_OBJ_NAME = 'TEMPT' THEN
        RAISE_APPLICATION_ERROR(-20001, 'Удаление таблицы TEMPT запрещено.');
    END IF;
    
    EXCEPTION
        WHEN DROP_EXC THEN
            DBMS_OUTPUT.put_line(''||SQLERRM);
            RAISE;
        WHEN OTHERS THEN
            DBMS_OUTPUT.put_line(SQLERRM);
END;
/

-- 12
DROP TABLE audit_log;

-- 13
CREATE OR REPLACE VIEW tempt_active_v AS
SELECT product_id, product_name, price FROM tempt WHERE active = 'Y';

-- 14
CREATE OR REPLACE TRIGGER tempt_active_v_update_tr
INSTEAD OF UPDATE ON tempt_active_v
FOR EACH ROW
DECLARE
   new_id NUMBER;
BEGIN
    UPDATE tempt SET active = 'N' WHERE product_id = :OLD.product_id AND active = 'Y';
    SELECT MAX(product_id) INTO new_id FROM tempt_active_v;
    INSERT INTO tempt(product_id, product_name, price, active) VALUES (new_id+1, :NEW.product_name, :NEW.price, 'Y');
    INSERT INTO audit_log(operation_type, trigger_name, data) VALUES ('UPDATE','tempt_active_v_upd_tr','8. INSTEAD‑OF (old id='||:OLD.product_id||', new id='||new_id||')');
END;
/

SELECT * FROM tempt;
SELECT * FROM audit_log;

UPDATE tempt_active_v SET price = price * 1.05 WHERE product_id = 1;

-- 15
CREATE OR REPLACE TRIGGER tempt_before_statement_A
BEFORE INSERT ON tempt
BEGIN
    INSERT INTO audit_log(operation_type, trigger_name, data) VALUES ('INSERT','tempt_before_statement_A','A. default order');
END;
/

CREATE OR REPLACE TRIGGER tempt_before_statement_B
BEFORE INSERT ON tempt
BEGIN
    INSERT INTO audit_log(operation_type, trigger_name, data) VALUES ('INSERT','tempt_before_statement_B','B. default order');
END;
/


DECLARE
    max_id NUMBER;
BEGIN
    SELECT MAX(product_id) INTO max_id FROM tempt;
    INSERT INTO tempt VALUES (max_id+1,'demo‑1',1,'Y');
END;
/

SELECT * FROM audit_log;

CREATE OR REPLACE TRIGGER tempt_before_statement_B
BEFORE INSERT ON tempt
FOLLOWS tempt_before_statement_A 
BEGIN
    INSERT INTO audit_log(operation_type, trigger_name, data) VALUES ('INSERT','tempt_before_statement_B','B. follows A.');
END;
/