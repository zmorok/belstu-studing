use Univer
go

create function FFACPUL(@f_code varchar(20), @p_code varchar(20)) returns table
as return
	select f.FACULTY, p.PULPIT from FACULTY f left join
		PULPIT p on f.FACULTY = p.FACULTY
		where f.FACULTY = isnull(@f_code, f.FACULTY) 
			and p.PULPIT = isnull(@p_code, p.PULPIT)
go

select * from dbo.FFACPUL(null, null) order by FACULTY
select * from dbo.FFACPUL('ТТЛП', null) order by FACULTY
select * from dbo.FFACPUL(null, 'ИСиТ') order by FACULTY
select * from dbo.FFACPUL('ТТЛП', 'ИСиТ') order by FACULTY