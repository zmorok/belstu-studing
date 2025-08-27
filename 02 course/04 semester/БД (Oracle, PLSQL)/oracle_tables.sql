ALTER SESSION SET CONTAINER = DSZ_PDB;

CREATE TABLE FACULTY
(
  FACULTY                 CHAR(10)        NOT NULL,
  FACULTY_NAME            VARCHAR2(100),

  CONSTRAINT PK_FACULTY PRIMARY KEY(FACULTY) 
);
  
CREATE TABLE PULPIT 
(
  PULPIT                  CHAR(20)        NOT NULL,
  PULPIT_NAME             VARCHAR2(200), 
  FACULTY                 CHAR(10)        NOT NULL, 
 
  CONSTRAINT PK_PULPIT PRIMARY KEY(PULPIT),
  CONSTRAINT FK_PULPIT_FACULTY FOREIGN KEY(FACULTY) REFERENCES FACULTY(FACULTY)
); 
    
CREATE TABLE TEACHER
( 
  TEACHER                 CHAR(20)        NOT NULL,
  TEACHER_NAME            VARCHAR2(100), 
  PULPIT                  CHAR(20)        NOT NULL,

  CONSTRAINT PK_TEACHER  PRIMARY KEY(TEACHER), 
  CONSTRAINT FK_TEACHER_PULPIT FOREIGN KEY(PULPIT) REFERENCES PULPIT(PULPIT)
);
 
CREATE TABLE SUBJECT
(
  SUBJECT                 CHAR(20)        NOT NULL, 
  SUBJECT_NAME            VARCHAR2(100)    NOT NULL,
  PULPIT                  CHAR(20)        NOT NULL,  
  
  CONSTRAINT PK_SUBJECT PRIMARY KEY(SUBJECT),
  CONSTRAINT FK_SUBJECT_PULPIT FOREIGN KEY(PULPIT) REFERENCES PULPIT(PULPIT)
);
 
CREATE TABLE AUDITORIUM_TYPE 
(
  AUDITORIUM_TYPE         CHAR(10),  
  AUDITORIUM_TYPENAME     VARCHAR2(60)    NOT NULL,

  CONSTRAINT PK_AUDITORIUM_TYPE  PRIMARY KEY(AUDITORIUM_TYPE)
);

CREATE TABLE AUDITORIUM 
(
  AUDITORIUM              CHAR(10),
  AUDITORIUM_NAME         VARCHAR2(200),         
  AUDITORIUM_CAPACITY     NUMBER(4),
  AUDITORIUM_TYPE         CHAR(10)        NOT NULL,

  CONSTRAINT PK_AUDITORIUM PRIMARY KEY(AUDITORIUM),
  CONSTRAINT FK_AUDITORIUM_AUDITORIUM_TYPE FOREIGN KEY(AUDITORIUM_TYPE) REFERENCES AUDITORIUM_TYPE(AUDITORIUM_TYPE)
);

/*
  DROP TABLE AUDITORIUM PURGE;
  DROP TABLE AUDITORIUM_TYPE PURGE;
  DROP TABLE SUBJECT PURGE;
  DROP TABLE TEACHER PURGE;
  DROP TABLE PULPIT PURGE;
  DROP TABLE FACULTY PURGE;
*/

/*
  TRUNCATE TABLE AUDITORIUM;
  TRUNCATE TABLE AUDITORIUM_TYPE;
  TRUNCATE TABLE SUBJECT;
  TRUNCATE TABLE TEACHER;
  TRUNCATE TABLE PULPIT;
  TRUNCATE TABLE FACULTY;
*/

--------------------------------------------------------------------------------
INSERT INTO AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME) VALUES ('ЛК', 'Лекционная');
INSERT INTO AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME) VALUES ('ЛБ-К', 'Компьютерный класс');
INSERT INTO AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME) VALUES ('ЛК-К', 'Лекционная с уст. компьютерами');
INSERT INTO AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME) VALUES ('ЛБ-X', 'Химическая лаборатория');
INSERT INTO AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME) VALUES ('ЛБ-СК', 'Спец. компьютерный класс');
--------------------------------------------------------------------------------
INSERT INTO FACULTY (FACULTY, FACULTY_NAME) VALUES ('ИДиП', 'Издателькое дело и полиграфия');
INSERT INTO FACULTY (FACULTY, FACULTY_NAME) VALUES ('ХТиТ', 'Химическая технология и техника');
INSERT INTO FACULTY (FACULTY, FACULTY_NAME) VALUES ('ЛХФ', 'Лесохозяйственный факультет');
INSERT INTO FACULTY (FACULTY, FACULTY_NAME) VALUES ('ИЭФ', 'Инженерно-экономический факультет');
INSERT INTO FACULTY (FACULTY, FACULTY_NAME) VALUES ('ТТЛП', 'Технология и техника лесной промышленности');
INSERT INTO FACULTY (FACULTY, FACULTY_NAME) VALUES ('ТОВ', 'Технология органических веществ');
--------------------------------------------------------------------------------
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ИСиТ', 'Иформационный систем и технологий ', 'ИДиП');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ПОиСОИ', 'Полиграфического оборудования и систем обработки информации ', 'ИДиП');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ЛВ', 'Лесоводства', 'ЛХФ');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ОВ', 'Охотоведения', 'ЛХФ');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ЛУ', 'Лесоустройства', 'ЛХФ');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ЛЗиДВ', 'Лесозащиты и древесиноведения', 'ЛХФ');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ЛПиСПС', 'Ландшафтного проектирования и садово-паркового строительства', 'ЛХФ');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ТЛ', 'Транспорта леса', 'ТТЛП');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ЛМиЛЗ', 'Лесных машин и технологии лесозаготовок', 'ТТЛП');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ОХ', 'Органической химии', 'ТОВ');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ТНХСиППМ', 'Технологии нефтехимического синтеза и переработки полимерных материалов', 'ТОВ');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ТНВиОХТ', 'Технологии неорганических веществ и общей химической технологии ', 'ХТиТ');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ХТЭПиМЭЕ', 'Химии, технологии электрохимических производств и материалов электронной техники', 'ХТиТ');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('ЭТиМ', 'экономической теории и маркетинга', 'ИЭФ');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES ('МиЭП', 'Менеджмента и экономики природопользования', 'ИЭФ');
--------------------------------------------------------------------------------
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('СМЛВ', 'Смелов Владимир Владиславович', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('АКНВЧ', 'Акунович Станислав Иванович', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('КЛСНВ', 'Колесников Леонид Валерьевич', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ГРМН', 'Герман Олег Витольдович', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ЛЩНК', 'Лащенко Анатолий Пвалович', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('БРКВЧ', 'Бракович Андрей Игорьевич', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ДДК', 'Дедко Александр Аркадьевич', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('КБЛ', 'Кабайло Александр Серафимович', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('УРБ', 'Урбанович Павел Павлович', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('РМНК', 'Романенко Дмитрий Михайлович', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ПСТВЛВ', 'Пустовалова Наталия Николаевна', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('?', 'Неизвестный', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ГРН', 'Гурин Николай Иванович', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ЖЛК', 'Жиляк Надежда Александровна', 'ИСиТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('БРТШВЧ', 'Барташевич Святослав Александрович', 'ПОиСОИ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ЮДНКВ', 'Юденков Виктор Степанович', 'ПОиСОИ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('БРНВСК', 'Барановский Станислав Иванович', 'ЭТиМ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('НВРВ', 'Неверов Александр Васильевич', 'МиЭП');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('РВКЧ', 'Ровкач Андрей Иванович', 'ОВ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ДМДК', 'Демидко Марина Николаевна', 'ЛПиСПС');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('МШКВСК', 'Машковский Владимир Петрович', 'ЛУ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ЛБХ', 'Лабоха Константин Валентинович', 'ЛВ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ЗВГЦВ', 'Звягинцев Вячеслав Борисович', 'ЛЗиДВ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('БЗБРДВ', 'Безбородов Владимир Степанович', 'ОХ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ПРКПЧК', 'Прокопчук Николай Романович', 'ТНХСиППМ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('НСКВЦ', 'Насковец Михаил Трофимович', 'ТЛ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('МХВ', 'Мохов Сергей Петрович', 'ЛМиЛЗ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ЕЩНК', 'Ещенко Людмила Семеновна', 'ТНВиОХТ');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES ('ЖРСК', 'Жарский Иван Михайлович', 'ХТЭПиМЭЕ');
--------------------------------------------------------------------------------
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('СУБД', 'Системы управления базами данных', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('БД', 'Базы данных', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ИНФ', 'Информацтонные технологии', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ОАиП', 'Основы алгоритмизации и программирования', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ПЗ', 'Представление знаний в компьютерных системах', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ПСП', 'Пограммирование сетевых приложений', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('МСОИ', 'Моделирование систем обработки информации', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ПИС', 'Проектирование информационных систем', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('КГ', 'Компьютерная геометрия ', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ПМАПЛ', 'Полиграфические машины, автоматы и поточные линии', 'ПОиСОИ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('КМС', 'Компьютерные мультимедийные системы', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ОПП', 'Организация полиграфического производства', 'ПОиСОИ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ДМ', 'Дискретная матеатика', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('МП', 'Математисеское программирование', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ЛЭВМ', 'Логические основы ЭВМ', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ООП', 'Объектно-ориентированное программирование', 'ИСиТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ЭП', 'Экономика природопользования', 'МиЭП');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ЭТ', 'Экономическая теория', 'ЭТиМ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('БЛЗиПсOO', 'Биология лесных зверей и птиц с осн. охотов.', 'ОВ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ОСПиЛПХ', 'Основы садовопаркового и лесопаркового хозяйства', 'ЛПиСПС');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ИГ', 'Инженерная геодезия ', 'ЛУ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ЛВ', 'Лесоводство', 'ЛЗиДВ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ОХ', 'Органическая химия', 'ОХ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ТРИ', 'Технология резиновых изделий', 'ТНХСиППМ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ВТЛ', 'Водный транспорт леса', 'ТЛ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ТиОЛ', 'Технология и оборудование лесозаготовок', 'ЛМиЛЗ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ТОПИ', 'Технология обогащения полезных ископаемых ', 'ТНВиОХТ');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES ('ПЭХ', 'Прикладная электрохимия', 'ХТЭПиМЭЕ');
--------------------------------------------------------------------------------
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('206-1', '206-1', 'ЛБ-К', 15);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('301-1', '301-1', 'ЛБ-К', 15);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('236-1', '236-1', 'ЛК', 60);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('313-1', '313-1', 'ЛК', 60);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('324-1', '324-1', 'ЛК', 50);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('413-1', '413-1', 'ЛБ-К', 15);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('423-1', '423-1', 'ЛБ-К', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('408-2', '408-2', 'ЛК', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('103-4', '103-4', 'ЛК', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('105-4', '105-4', 'ЛК', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('107-4', '107-4', 'ЛК', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('110-4', '110-4', 'ЛК', 30);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('111-4', '111-4', 'ЛК', 30);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('114-4', '114-4', 'ЛК-К', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('132-4', '132-4', 'ЛК', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('02Б-4', '02Б-4', 'ЛК', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('229-4', '229-4', 'ЛК', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('304-4', '304-4', 'ЛБ-К', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('314-4', '314-4', 'ЛК', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('320-4', '320-4', 'ЛК', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('429-4', '429-4', 'ЛК', 90);
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY) VALUES ('?', '???', 'ЛК', 90);
COMMIT;
