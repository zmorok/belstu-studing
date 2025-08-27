use Univer;
go

select F.FACULTY, G.PROFESSION, P.SUBJECT, avg(P.NOTE) [Средняя за экзамен] from FACULTY F
	join GROUPS G on F.FACULTY = G.FACULTY
	join STUDENT S on G.IDGROUP = S.IDGROUP
	join PROGRESS P on S.IDSTUDENT = P.IDSTUDENT
	where F.FACULTY = 'ИТ'
	group by F.FACULTY,G.PROFESSION,P.SUBJECT

-- task01 'rollup'
select F.FACULTY, G.PROFESSION, P.SUBJECT, avg(P.NOTE) [Средняя за экзамен] from FACULTY F
	join GROUPS G on F.FACULTY = G.FACULTY
	join STUDENT S on G.IDGROUP = S.IDGROUP
	join PROGRESS P on S.IDSTUDENT = P.IDSTUDENT
	where F.FACULTY = 'ИТ'
	group by rollup (F.FACULTY,G.PROFESSION,P.SUBJECT)

-- task02 'cube'
select F.FACULTY, G.PROFESSION, P.SUBJECT, avg(P.NOTE) [Средняя за экзамен] from FACULTY F
	join GROUPS G on F.FACULTY = G.FACULTY
	join STUDENT S on G.IDGROUP = S.IDGROUP
	join PROGRESS P on S.IDSTUDENT = P.IDSTUDENT
	where F.FACULTY = 'ИТ'
	group by cube (F.FACULTY, G.PROFESSION, P.SUBJECT)

-- task03 'union'
select F.FACULTY, G.PROFESSION, P.SUBJECT, avg(P.NOTE) [Средняя за экзамен] from FACULTY F
	join GROUPS G on F.FACULTY = G.FACULTY
	join STUDENT S on G.IDGROUP = S.IDGROUP
	join PROGRESS P on S.IDSTUDENT = P.IDSTUDENT
	where F.FACULTY = 'ИТ'
	group by F.FACULTY, G.PROFESSION, P.SUBJECT
	except
select F.FACULTY, G.PROFESSION, P.SUBJECT, avg(P.NOTE) [Средняя за экзамен] from FACULTY F
	join GROUPS G on F.FACULTY = G.FACULTY
	join STUDENT S on G.IDGROUP = S.IDGROUP
	join PROGRESS P on S.IDSTUDENT = P.IDSTUDENT
	where F.FACULTY = 'ИТ'
	group by F.FACULTY, G.PROFESSION, P.SUBJECT