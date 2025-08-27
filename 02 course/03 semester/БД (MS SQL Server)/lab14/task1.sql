use Univer
go

alter function COUNT_STUDENTS(@faculty varchar(20)) returns int
as begin
	declare @count int = 0
	set @count = (
		select count(*) from FACULTY f join
		GROUPS g on g.FACULTY = f.FACULTY join
		STUDENT s on s.IDGROUP = g.IDGROUP
		where f.FACULTY = @faculty
	)
	return @count
end
go

select FACULTY, dbo.COUNT_STUDENTS(FACULTY) from FACULTY
go

-- drop function COUNT_STUDENTS

alter function COUNT_STUDENTS(@faculty varchar(20) = null, @prof varchar(20) = null) returns int
as begin
	declare @count int = 0
	set @count = (
		select count(*) from FACULTY f join
		GROUPS g on g.FACULTY = f.FACULTY join
		STUDENT s on s.IDGROUP = g.IDGROUP	
		where f.FACULTY = @faculty and g.PROFESSION = isnull(@prof, PROFESSION)
	)
	return @count
end
go

select f.FACULTY, p.PROFESSION, dbo.COUNT_STUDENTS(f.FACULTY, p.PROFESSION) [Количество студентов]
	from FACULTY f join PROFESSION p on p.FACULTY = f.FACULTY
	order by f.FACULTY, p.PROFESSION
go