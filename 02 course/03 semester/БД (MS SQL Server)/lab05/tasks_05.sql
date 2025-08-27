use Univer;
go

/*1*/
select PULPIT_NAME from PULPIT
where FACULTY in (
	select FACULTY from PROFESSION where (PROFESSION_NAME like '%технологи%')
);

/*2*/
select PULPIT_NAME 
from PULPIT PU 
inner join FACULTY F on F.FACULTY = PU.FACULTY 
and PU.FACULTY in (
	select FACULTY from PROFESSION where (PROFESSION_NAME like '%технологи%')
);

/*3*/
select distinct PULPIT_NAME
from PULPIT inner join PROFESSION on PULPIT.FACULTY = PROFESSION.FACULTY
			inner join FACULTY on FACULTY.FACULTY = PULPIT.FACULTY
			where (PROFESSION_NAME like '%технологи%');

/*4*/
select AUDITORIUM_TYPE, AUDITORIUM_CAPACITY from AUDITORIUM A1
where AUDITORIUM_CAPACITY = (
	select top 1 AUDITORIUM_CAPACITY
	from AUDITORIUM A2
	where A1.AUDITORIUM_TYPE = A2.AUDITORIUM_TYPE
	order by AUDITORIUM_CAPACITY desc
) order by AUDITORIUM_CAPACITY desc;

/*5*/
select FACULTY_NAME from FACULTY
where not exists (
	select * from PULPIT
	where PULPIT.FACULTY = FACULTY.FACULTY
);

/*6*/
select top 1
	(select avg(cast(NOTE as float)) from PROGRESS where SUBJECT = 'ОАиП') [Средняя по ОАиП],
	(select avg(cast(NOTE as float)) from PROGRESS where SUBJECT = 'БД') [Средняя по БД],
	(select avg(cast(NOTE as float)) from PROGRESS where SUBJECT = 'СУБД') [Средняя по СУБД];

/*7*/
select NAME, BDAY from STUDENT
where NAME like 'Ш%' 
and BDAY >= all (select BDAY from STUDENT where NAME like 'Ь%');

/*8*/
select NAME, BDAY
from STUDENT
where IDGROUP = any (select IDGROUP from STUDENT where BDAY < '1993-01-01');

/*10*/
select BDAY, count(*) [cnt] from STUDENT
group by BDAY having count(*) > 1