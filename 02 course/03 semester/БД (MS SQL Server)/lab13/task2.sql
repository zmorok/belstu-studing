use Univer
go

create procedure PSUBJECT 
	@p varchar(20) = null, @c int output
as begin
	declare @r int = (select count(*) from SUBJECT)
	set @c = (select count(*) from SUBJECT where PULPIT = @p) 
	select * from SUBJECT where PULPIT = @p
	return @r
end
go

drop procedure PSUBJECT
go

declare @r int, @c int, @name varchar(20) = 'ИСиТ', @s varchar

exec @r = PSUBJECT @name, @c output

select @r [общее кол-во], @c [кол-во с кафед]
