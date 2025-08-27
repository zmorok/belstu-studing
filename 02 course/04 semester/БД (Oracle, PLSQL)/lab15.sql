-- 1.
CREATE TABLE t_range (
    id NUMBER PRIMARY KEY,
    payload VARCHAR2(50)
) PARTITION BY RANGE (id) (
    PARTITION p_low VALUES LESS THAN (1000),
    PARTITION p_mid VALUES LESS THAN (2000),
    PARTITION p_high VALUES LESS THAN (MAXVALUE)
);

-- 2.
CREATE TABLE t_interval (
    id DATE,
    payload  VARCHAR2(50)
)
PARTITION BY RANGE (id)
INTERVAL (NUMTOYMINTERVAL(1,'MONTH')) (
    PARTITION p_start VALUES LESS THAN (DATE '2025-01-01')
);

-- 3.
CREATE TABLE t_hash (
    code     VARCHAR2(10),
    payload  VARCHAR2(50)
)
PARTITION BY HASH (code) (
    PARTITION p_h1,
    PARTITION p_h2,
    PARTITION p_h3
);

-- 4.
CREATE TABLE t_list (
    region   CHAR(5),
    payload  VARCHAR2(50)
)
PARTITION BY LIST (region) (
    PARTITION p_1 VALUES ('1','1.1'),
    PARTITION p_2 VALUES ('2','2.1'),
    PARTITION p_3 VALUES ('3','3.1','3.2')
);

-- 5.
INSERT INTO t_range VALUES (100, 'low 100');
INSERT INTO t_range VALUES (1500, 'mid 1500');
INSERT INTO t_range VALUES (2500, 'high 2500');

INSERT INTO t_interval VALUES (DATE '2025-01-15', 'Январь');
INSERT INTO t_interval VALUES (DATE '2025-03-10', 'Март');
INSERT INTO t_interval VALUES (DATE '2026-08-20', 'Август');

INSERT INTO t_hash VALUES ('A1', 'code A1');
INSERT INTO t_hash VALUES ('B2', 'code B2');
INSERT INTO t_hash VALUES ('C3', 'code C3');

INSERT INTO t_list VALUES ('1', 'строка 1.');
INSERT INTO t_list VALUES ('2', 'строка 2.');
INSERT INTO t_list VALUES ('3', 'строка 3.');

COMMIT;

SELECT * FROM t_range PARTITION(p_low);
SELECT * FROM t_interval PARTITION FOR (DATE '2025-03-10');
SELECT * FROM t_hash PARTITION(p_h2);
SELECT * FROM t_list PARTITION(p_3);

-- 6
ALTER TABLE t_range ENABLE ROW MOVEMENT;
ALTER TABLE t_interval ENABLE ROW MOVEMENT;
ALTER TABLE t_hash ENABLE ROW MOVEMENT;
ALTER TABLE t_list ENABLE ROW MOVEMENT;

UPDATE t_range SET id = 1800 WHERE id = 100;
UPDATE t_interval SET id = DATE '2026-12-01' WHERE id = DATE '2025-01-15';
UPDATE t_hash SET code = 'D4' WHERE code  = 'A1';
UPDATE t_list SET region = '2' WHERE region = '3';
COMMIT;

SELECT id, payload
FROM t_range PARTITION (p_mid);

-- 7.
ALTER TABLE t_range MERGE PARTITIONS p_mid, p_high INTO PARTITION p_merged; 

-- 8.
ALTER TABLE t_list SPLIT PARTITION p_3 VALUES ('3.1')
INTO (PARTITION p_3_1, PARTITION p_3_rest);

-- 9.
CREATE TABLE t_range_stage(
    id NUMBER PRIMARY KEY,
    payload VARCHAR2(50)
);
INSERT INTO t_range_stage VALUES (3500, 'stage 3500');
COMMIT;

ALTER TABLE t_range EXCHANGE PARTITION p_merged WITH TABLE t_range_stage WITHOUT VALIDATION;
SELECT * FROM t_range;

SELECT partition_name, high_value
FROM user_tab_partitions
WHERE table_name = 'T_LIST'
ORDER BY partition_position;


-- 10.
SELECT table_name, partitioning_type FROM user_part_tables;
SELECT * FROM user_tab_partitions WHERE table_name = 'T_RANGE';
SELECT * FROM t_list PARTITION(p_1);
SELECT * FROM t_interval PARTITION FOR (DATE '2026-08-20');

