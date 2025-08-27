use Univer
go

/*
create table #aud (
    auditorium char(20) not null,
    name varchar(50) not null,
    capacity int check (capacity >= 0),
	type char(10) not null
)
go

create table #typ (
	type char(10) not null,
	typename varchar(30) not null
)
go
*/

--drop table #typ
--go

--exec sp_help AUDITORIUM_TYPE
--go

create procedure PAUDITORIUM_INSERTX @a char(20), @n varchar(50), @c int = 0, @t char(10), @tn varchar(50)
as
declare @rc int = 1
begin try
	set tran isolation level serializable
	begin tran
		insert into #typ values (@n, @tn)
		exec @rc = PAUDITORIUM_INSERT @a, @n, @c, @t;
	commit tran
	return @rc
end try
begin catch
	print 'код ошибки:	' + cast(error_number() as varchar)
	print 'серьёзность:	' + cast(error_severity() as varchar)
	print 'сообщение:	' + error_message()
	if error_procedure() is not null
		print 'имя процедуры:	' + error_procedure()
	if @@trancount > 0
		rollback tran
	return -1
end catch
go

declare @e int = 0;
exec @e = PAUDITORIUM_INSERTX '100', 23, 30, 'ауд1', 'перемен tn'
print 'ошибка?: ' + cast(@e as varchar)
go

select * from #aud
select * from #typ