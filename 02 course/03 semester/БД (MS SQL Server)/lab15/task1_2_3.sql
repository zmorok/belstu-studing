use Univer;
go

-- drop table TR_AUDIT
----------TR_AUDIT----------
create table TR_AUDIT(
	ID int identity,
	STMT varchar(20) check (STMT in ('INS', 'DEL', 'UPD')),
	TRNAME varchar(50),
	CC varchar(300)
)
go
----------------------------


----------task01------------
create trigger TR_TEACHER_INS on TEACHER after INSERT 
as declare @a1 varchar(5), @a2 varchar(50), @a3 varchar(1), @a4 varchar(10), @in varchar(500)
set @a1 = (select TEACHER from inserted)
set @a2 = rtrim((select TEACHER_NAME from inserted))
set @a3 = rtrim((select GENDER from inserted))
set @a4 = rtrim((select PULPIT from inserted))
set @in = @a1 + ' ' + @a2 + ' ' + @a3 + ' '+ @a4
insert into TR_AUDIT (STMT,TRNAME,CC)
	values ('INS','TR_TEACHER_INS', @in)
return
go

/*
drop trigger TR_TEACHER_INS
go
*/

select * from TEACHER
insert into  TEACHER values  ('ФБВГ', 'Фереев Бавил Витальевич', 'м', 'ИСиТ')
--delete from TEACHER where TEACHER = 'ФБВГ'
go

select * from TR_AUDIT
go
----------------------------

----------task02------------
create trigger TR_TEACHER_DEL on TEACHER after DELETE 
as declare @a1 varchar(5)
set @a1 = (select TEACHER from deleted)
insert into TR_AUDIT (STMT,TRNAME,CC)
	values ('DEL','TR_TEACHER_DEL', @a1)
return
go

/*
drop trigger TR_TEACHER_DEL
go
*/

--insert into  TEACHER values  ('ФБВГ', 'Фереев Бавил Витальевич', 'м', 'ИСиТ')
delete from TEACHER where TEACHER = 'ФБВГ'
select * from TR_AUDIT
go
----------------------------

----------task03------------
create trigger TR_TEACHER_UPD on TEACHER after UPDATE
as declare @a1 varchar(5),	@a2 varchar(60),
	@a3 varchar(1),			@a4 varchar(10),
	@in varchar(700),		@a5 varchar(5),
	@a6 varchar(60),		@a7 varchar(1),
	@a8 varchar(10)

set @a1 = (select TEACHER from deleted)
set @a2 = rtrim((select TEACHER_NAME from deleted))
set @a3 = rtrim((select GENDER from deleted))
set @a4 = rtrim((select PULPIT from deleted))
set @a5 = (select TEACHER from inserted)
set @a6 = rtrim((select TEACHER_NAME from inserted))
set @a7 = rtrim((select GENDER from inserted))
set @a8 = rtrim((select PULPIT from inserted))
set @in = 'BEFORE: ' + @a1 + ', ' + @a2 + ', ' + @a3 + ', '+ @a4 + '.|||AFTER: ' + @a5 + ', ' + @a6 + ', ' + @a7 + ', '+ @a8 + '.'
insert into TR_AUDIT(STMT, TRNAME, CC) values ('UPD','TR_TEACHER_UPD', @in)
return

/*
drop trigger TR_TEACHER_UPD
go
*/

select * from TEACHER
select * from TR_AUDIT 
--insert into  TEACHER values  ('ФБВГ', 'Фереев Бавил Витальевич', 'м', 'ИСиТ')
--delete from TEACHER where TEACHER = 'ФБВГ'
update TEACHER set TEACHER_NAME = 'dadasdasdasdas' where TEACHER = 'ФБВГ'
go
----------------------------
