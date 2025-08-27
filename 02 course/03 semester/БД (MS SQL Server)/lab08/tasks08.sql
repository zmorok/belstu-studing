use Univer;
go

-- task01
create view [08_Преподаватель] as
select TEACHER [Код], TEACHER_NAME [Имя преподавателя], GENDER [Пол], PULPIT [Код кафедры] from TEACHER;
go

-- task02
create view [08_Количество_кафедр] as
select F.FACULTY [Факультет], count(P.PULPIT) [Кол-во кафедр] from FACULTY F
join PULPIT P on F.FACULTY = P.FACULTY group by F.FACULTY;
go

-- task03
create view [08_Аудитории] ([Код], [Имя аудитории]) as
select AUDITORIUM, AUDITORIUM_NAME from AUDITORIUM
where AUDITORIUM_TYPE like 'ЛК%';
go

-- task04
create view [08_Лекционные_аудитории] ([Код], [Имя лекционной аудитории]) as
select AUDITORIUM [Код], AUDITORIUM_NAME [Наименование аудитории] from AUDITORIUM
where AUDITORIUM_TYPE like 'ЛК%' with check option;
go

-- task05
create view [08_Дисциплины] as
select top 100 percent SUBJECT [Код], SUBJECT_NAME [Наименование дисциплины], PULPIT [Код кафедры]
from SUBJECT order by SUBJECT_NAME;
go

-- task06
alter view [08_Количество_кафедр] with schemabinding as
select dbo.FACULTY.FACULTY [Факультет], count(dbo.PULPIT.PULPIT) [Кол-во кафедр] from dbo.FACULTY
join dbo.PULPIT on dbo.FACULTY.FACULTY = dbo.PULPIT.FACULTY
group by dbo.FACULTY.FACULTY;
go

select * from [08_Преподаватель]
select * from [08_Количество_кафедр]
select * from [08_Аудитории]
select * from [08_Лекционные_аудитории]
select * from [08_Дисциплины]

/*
drop view [08_Преподаватель]
drop view [08_Количество_кафедр]
drop view [08_Аудитории]
drop view [08_Лекционные_аудитории]
drop view [08_Дисциплины]
*/