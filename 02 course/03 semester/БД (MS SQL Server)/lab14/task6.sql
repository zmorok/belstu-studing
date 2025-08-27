use Univer
go

create function c_pulpits(@faculty varchar(20)) returns int
as begin
    return (select count(*) from PULPIT where FACULTY = @faculty)
end
go

create function c_groups(@faculty varchar(20)) returns int
as begin
    return (select count(*) from GROUPS where FACULTY = @faculty)
end
go


create function c_professions(@faculty varchar(20)) returns int
as begin
    return (select count(*) from PROFESSION where FACULTY = @faculty)
end
go

------------------------------------------------------------------------------
alter function FACULTY_REPORT(@c int) returns @fr table
(
    [Факультет] varchar(50), [Количество кафедр] int, [Количество групп] int, [Количество студентов] int, [Количество специальностей] int
)
as begin
    declare cc cursor static local for (select FACULTY from FACULTY where dbo.COUNT_STUDENTS(FACULTY, default) > @c)

    declare @f varchar(30)
    open cc
		fetch cc into @f
		while @@fetch_status = 0
		begin
			insert @fr values (@f, dbo.c_pulpits(@f), dbo.c_groups(@f), dbo.COUNT_STUDENTS(@f, default), dbo.c_professions(@f))
			fetch cc into @f
		end
	close cc

    return
end
go

select * from FACULTY_REPORT(-1)
go
