use Univer
go

drop trigger TR_STUDENT 
go

create trigger TR_STUDENT on STUDENT after INSERT, UPDATE
as declare @date date = (select BDAY from inserted)
if (year(@date) < '1800')
begin
	raiserror('Такой даты рождения не может быть',10,1)
	rollback
end
return
go

-- Силюк Валерия Ивановна 1994-07-12
update STUDENT set BDAY = '1700-02-04' where IDSTUDENT = 1000

select * from STUDENT where IDSTUDENT = 1000