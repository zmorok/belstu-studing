alter procedure AfterBDAY @bday varchar(4), @count int output
as begin try
	if (@bday is null or len(@bday) = 0) raiserror('',11,1)
	set @count = (select count(*) from STUDENT where year(BDAY) >= @bday)

	select IDSTUDENT id, 
			IDGROUP idg, 
			NAME name, 
			BDAY birthday, 
			INFO info, 
			FOTO photo into #tb from STUDENT where cast(year(BDAY) as varchar) >= @bday

	select * from #tb order by birthday
	drop table #tb
end try
begin catch
	print 'параметр @bday не может быть пустым или содержать буквы.'
end catch
go

declare @c int, @year varchar(4) = ''
exec AfterBDAY @year, @c output
print 'количество людей, родившихся позднее ' + @year + ' : ' + cast(@c as varchar) 