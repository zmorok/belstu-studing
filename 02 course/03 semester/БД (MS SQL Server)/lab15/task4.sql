use Univer;
go

-- drop trigger TR_TEACHER
create trigger TR_TEACHER on TEACHER after INSERT, DELETE, UPDATE
as declare @a1 varchar(5), @a2 varchar(50), @a3 varchar(1), @a4 varchar(10), @in varchar(700)
declare @ins int = (select count(*) from inserted), @del int = (select count(*) from deleted)
-- INS
if @ins > 0 and @del = 0
begin
    set @a1 = (select TEACHER from inserted) 
    set @a2 = (select TEACHER_NAME from inserted)
	set @a3 = (select GENDER from inserted)
    set @a4 = (select PULPIT from inserted)

    set @in = @a1 + ' ' + @a2 + ' ' + @a3 + ' ' + @a4
    insert into TR_AUDIT (STMT, TRNAME, CC) values ('INS', 'TR_TEACHER', @in)
end
-- DEL
else if @ins = 0 and @del > 0 
begin
    set @a1 = (select TEACHER from deleted)

    set @in = @a1
    insert into TR_AUDIT (STMT, TRNAME, CC) values ('DEL', 'TR_TEACHER', @in)
end
-- UPD
else if @ins > 0 and @del > 0
begin
    declare @a5 varchar(5), @a6 varchar(50), @a7 varchar(1), @a8 varchar(10)

	set @a1 = (select TEACHER from deleted)
	set @a2 = rtrim((select TEACHER_NAME from deleted))
	set @a3 = rtrim((select GENDER from deleted))
	set @a4 = rtrim((select PULPIT from deleted))
	set @a5 = (select TEACHER from inserted)
	set @a6 = rtrim((select TEACHER_NAME from inserted))
	set @a7 = rtrim((select GENDER from inserted))
	set @a8 = rtrim((select PULPIT from inserted))

    set @in = 'BEFORE: ' + @a1 + ', ' + @a2 + ', ' + @a3 + ', ' + @a4 + '.|||AFTER: ' + @a5 + ', ' + @a6 + ', ' + @a7 + ', ' + @a8 + '.'
    insert into TR_AUDIT (STMT, TRNAME, CC) values ('UPD', 'TR_TEACHER', @in)
end
return
go

select * from TEACHER
select * from TR_AUDIT 
-- delete from TR_AUDIT where ID is not null
--insert into  TEACHER values  ('ФБВГ', 'Фереев Бавил Витальевич', 'м', 'ИСиТ')
--delete from TEACHER where TEACHER = 'ФБВГ'
update TEACHER set TEACHER_NAME = 'dadasdasdasdas' where TEACHER = 'ФБВГ'
go