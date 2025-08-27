use Univer
go

alter function FSUBJECTS(@p varchar(20)) returns varchar(300)
as begin
	declare @str varchar(300) = 'Дисциплины: ', @name varchar(20)
	declare curs cursor static local for (select s.SUBJECT from SUBJECT s where PULPIT = @p)
	open curs
		fetch curs into @name
		while @@fetch_status = 0
		begin
			set @str += rtrim(@name) + ', '
			fetch curs into @name
		end
	close curs
	return @str
end
go

select distinct PULPIT, dbo.FSUBJECTS(PULPIT) from PULPIT
go