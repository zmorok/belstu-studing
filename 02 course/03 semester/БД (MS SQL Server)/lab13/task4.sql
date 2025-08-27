use Univer
go

/*
1. для процедурки табличку aud надо
create table #aud (
    auditorium char(20) not null,
    name varchar(50) not null,
    capacity int check (capacity >= 0),
	type char(10) not null
)
*/
--drop table #aud
--select * from #aud
--drop procedure PAUDITORIUM_INSERT

create procedure PAUDITORIUM_INSERT
	@a char(20), @n varchar(50), @c int = 0, @t char(10)
as begin try
	insert into #aud values (@a, @n, @c, @t)
	return 1
end try
begin catch
	print 'код ошибки:	' + cast(error_number() as varchar)
	print 'серьёзность:	' + cast(error_severity() as varchar)
	print 'сообщение:	' + error_message()
	return -1
end catch
go

declare @e int;
exec @e = PAUDITORIUM_INSERT '100', 23, -130, 'ауд1'
print 'ошибка?: ' + cast(@e as varchar)
go