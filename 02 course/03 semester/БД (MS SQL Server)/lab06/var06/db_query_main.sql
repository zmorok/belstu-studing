USE master;
CREATE DATABASE lab06_db;
GO
USE lab06_db;
GO

CREATE TABLE СТУДЕНТЫ (
    ID_студента INT IDENTITY(1,1),
    Фамилия NVARCHAR(50),
    Имя NVARCHAR(50),
    Отчество NVARCHAR(50),
    Адрес NVARCHAR(100),
    Телефон NVARCHAR(20),
	CONSTRAINT pk_id_студента PRIMARY KEY (ID_студента) 
);

CREATE TABLE ПРЕДМЕТЫ (
    ID_предмета INT IDENTITY(1,1),
    Название NVARCHAR(100),
    Часы_лекций INT,
    Часы_практических_занятий INT,
    Часы_лабораторных_работ INT,
	CONSTRAINT pk_id_предмета PRIMARY KEY (ID_предмета)
);

CREATE TABLE ОЦЕНКИ (
    ID_оценки INT IDENTITY(1,1),
    ID_студента INT,
    ID_предмета INT,
    Оценка NVARCHAR(2),
	CONSTRAINT pk_id_оценки PRIMARY KEY (ID_оценки),
	CONSTRAINT fk_id_студента_ОЦЕНКИ FOREIGN KEY (ID_студента) REFERENCES СТУДЕНТЫ(ID_студента),
	CONSTRAINT fk_id_предмета_ОЦЕНКИ FOREIGN KEY (ID_предмета) REFERENCES ПРЕДМЕТЫ(ID_предмета)
);

CREATE TABLE ПОСЕЩАЕМОСТЬ (
    ID_посещения INT IDENTITY(1,1),
    ID_студента INT,
    ID_предмета INT,
    Часы_лекций INT,
    Часы_практических_занятий INT,
    Часы_лабораторных_работ INT,
	CONSTRAINT pk_id_посещения	PRIMARY KEY (ID_посещения),
	CONSTRAINT fk_id_студента_ПОСЕЩАЕМОСТЬ FOREIGN KEY (ID_студента) REFERENCES СТУДЕНТЫ(ID_студента),
	CONSTRAINT fk_id_предмета_ПОСЕЩАЕМОСТЬ FOREIGN KEY (ID_предмета) REFERENCES ПРЕДМЕТЫ(ID_предмета)
);

INSERT INTO СТУДЕНТЫ (Фамилия, Имя, Отчество, Адрес, Телефон)
VALUES 
('Иванов', 'Иван', 'Иванович', 'ул. Ленина, д. 10', '89001234567'),
('Петров', 'Петр', 'Петрович', 'ул. Пушкина, д. 20', '89007654321'),
('Сидоров', 'Сидор', 'Сидорович', 'ул. Мира, д. 5', '89009876543');

INSERT INTO ПРЕДМЕТЫ (Название, Часы_лекций, Часы_практических_занятий, Часы_лабораторных_работ)
VALUES
('Математика', 40, 20, 10),
('Физика', 30, 15, 15),
('Информатика', 35, 20, 20);

INSERT INTO ОЦЕНКИ (ID_студента, ID_предмета, Оценка)
VALUES 
(1, 1, '8'),
(2, 2, '4'),
(3, 3, '6'),
(1, 2, '10'),
(2, 3, '5');

INSERT INTO ПОСЕЩАЕМОСТЬ (ID_студента, ID_предмета, Часы_лекций, Часы_практических_занятий, Часы_лабораторных_работ)
VALUES 
(1, 1, 40, 20, 10),
(2, 2, 30, 15, 15),
(3, 3, 35, 20, 20),
(1, 2, 30, 15, 10),
(2, 3, 35, 20, 15);