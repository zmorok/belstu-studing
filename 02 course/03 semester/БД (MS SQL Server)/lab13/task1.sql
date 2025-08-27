use Univer
go

create procedure PSUBJECT
as
begin
	declare @row_count int = (select count(*) from SUBJECT)
	select SUBJECT [код], SUBJECT_NAME [дисциплина], PULPIT [кафедра] from SUBJECT
	return @row_count
end
go

declare @r int = 0
exec @r = PSUBJECT

print 'количество строк: ' + cast(@r as varchar)
go

drop procedure PSUBJECT
go