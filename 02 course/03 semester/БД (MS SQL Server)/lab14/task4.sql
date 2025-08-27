use Univer
go

create function FCTEACHER(@p_code varchar(20)) returns int
as begin
	declare @count_prep int = 0
	set @count_prep = (select count(*) from TEACHER where PULPIT = isnull(@p_code, PULPIT))
	return @count_prep
end
go

select PULPIT, dbo.FCTEACHER(PULPIT) from PULPIT
select dbo.FCTEACHER(null)
go