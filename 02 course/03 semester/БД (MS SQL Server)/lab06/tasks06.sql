use Univer;
go

/*task 1*/
select A.AUDITORIUM_TYPE,
	max(A.AUDITORIUM_CAPACITY) [Макс. вместительность],
	min(A.AUDITORIUM_CAPACITY) [Мин. вместительность],
	avg(A.AUDITORIUM_CAPACITY) [Средняя вместительность],
	sum(A.AUDITORIUM_CAPACITY) [Все места],
	count(A.AUDITORIUM_CAPACITY) [Кол-во аудиторий]
from AUDITORIUM A
	inner join AUDITORIUM_TYPE A1
	on A1.AUDITORIUM_TYPE = A.AUDITORIUM_TYPE
	group by A.AUDITORIUM_TYPE;

/*task 2*/
select *
from (select case 
		when NOTE between 8 and 10 then 'оценка больше 8'
		when NOTE between 5 and 7 then 'оценка от 5 до 7'
		else 'оценка 4'
		end [Интервалы оценки], count(*) [кол-во]
	from PROGRESS group by case
			when NOTE between 8 and 10 then 'оценка больше 8'
			when NOTE between 5 and 7 then 'оценка от 5 до 7'
			else 'оценка 4'
			end) T order by case [Интервалы оценки]
					when 'оценка больше 8' then 1
					when 'оценка от 5 до 7' then 2
					when 'оценка 4' then 3
					else 0
					end;

/*task 3*/
select F.FACULTY_NAME [Факультет], G.PROFESSION [Специальность], round(avg(cast( P.NOTE as float(4))),2) [Оценка]
	from PROGRESS P 
		inner join STUDENT S on P.IDSTUDENT = S.IDSTUDENT
		inner join GROUPS G on S.IDGROUP = G.IDGROUP
		inner join FACULTY F on G.FACULTY = F.FACULTY
		group by F.FACULTY_NAME, G.PROFESSION, P.NOTE order by P.NOTE desc;

/*task 4*/
select F.FACULTY_NAME [Факультет], G.PROFESSION [Специальность], round(avg(cast( P.NOTE as float(4))),2) [Оценка]
	from PROGRESS P 
		inner join STUDENT S on P.IDSTUDENT = S.IDSTUDENT
		inner join GROUPS G on S.IDGROUP = G.IDGROUP
		inner join FACULTY F on G.FACULTY = F.FACULTY
		where P.SUBJECT = 'БД' or P.SUBJECT = 'ОАиП'
		group by F.FACULTY_NAME, G.PROFESSION, P.NOTE order by P.NOTE desc;

/*task 5*/
select F.FACULTY_NAME [Факультет], G.PROFESSION [Специальность], round(avg(cast( P.NOTE as float(4))),2) [Оценка]
	from PROGRESS P 
		inner join STUDENT S on P.IDSTUDENT = S.IDSTUDENT
		inner join GROUPS G on S.IDGROUP = G.IDGROUP
		inner join FACULTY F on G.FACULTY = F.FACULTY
		where F.FACULTY = 'ТОВ'
		group by F.FACULTY_NAME, G.PROFESSION, P.NOTE order by P.NOTE desc;

/*task 6*/
select P.SUBJECT [Дисциплина], count(IDSTUDENT) [кол-во] from PROGRESS P
where P.NOTE = 8 or P.NOTE = 9
group by P.SUBJECT having count(IDSTUDENT) > 0
order by P.SUBJECT desc;