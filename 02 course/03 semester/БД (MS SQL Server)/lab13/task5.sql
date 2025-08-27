use Univer
go

create procedure SUBJECT_REPORT @p char(10)
as
declare @rc int = 0
begin try
	declare @sc char(10), @res varchar(300) = ''
	declare SUBJECT_C cursor local for (select SUBJECT from SUBJECT where PULPIT = @p)

	if not exists (select SUBJECT from SUBJECT where PULPIT = @p)
		raiserror('ошибка',11 ,1)
	else
		open SUBJECT_C
		fetch SUBJECT_C into @sc
		print 'Коды предметов:'
		while @@FETCH_STATUS = 0
		begin
			set @res += rtrim(@sc) + ', '
			set @rc += 1
			fetch SUBJECT_C into @sc
		end
		print @res
		close SUBJECT_C
		return @rc
end try
begin catch
	print 'ошибка в параметрах'
	if error_procedure() is not null
		print 'имя процедуры:	' + error_procedure()
	return @rc
end catch
go

declare @rc int = 0
exec @rc = SUBJECT_REPORT 'ИСиТ'
print '@rc:	' + cast(@rc as varchar)

--drop procedure SUBJECT_REPORT