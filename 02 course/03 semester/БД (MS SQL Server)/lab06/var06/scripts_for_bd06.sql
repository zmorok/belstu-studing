use lab06_db;
go

/*task 1*/
select distinct P.Название [Название_предмета],
	max(PP.Часы_лекций + PP.Часы_практических_занятий + PP.Часы_лабораторных_работ) [Макс_часы],
	min(PP.Часы_лекций + PP.Часы_практических_занятий + PP.Часы_лабораторных_работ) [Мин_часы],
	avg(PP.Часы_лекций + PP.Часы_практических_занятий + PP.Часы_лабораторных_работ) [Среднее_часы],
	sum(PP.Часы_лекций + PP.Часы_практических_занятий + PP.Часы_лабораторных_работ) [Общие_часы],
	count(PP.ID_студента) [Количество_студентов]
from ПОСЕЩАЕМОСТЬ PP
inner join ПРЕДМЕТЫ P on P.ID_предмета = PP.ID_предмета
group by P.Название;

/*task 2*/
select *
from (select case 
		when cast(Оценка as int) >= 8 then 'оценка больше 8'
		when cast(Оценка as int) between 5 and 7 then 'оценка от 5 до 7'
		else 'оценка 4'
		end [Интервалы_оценок], count(*) [Количество_студентов]
	from ОЦЕНКИ
	group by case
			when cast(Оценка as int) >= 8 then 'оценка больше 8'
			when cast(Оценка as int) between 5 and 7 then 'оценка от 5 до 7'
			else 'оценка 4'
			end) as T
order by case [Интервалы_оценок]
		when 'оценка больше 8' then 1
		when 'оценка от 5 до 7' then 2
		when 'оценка 4' then 3
		else 0
		end;

/*task 3*/
select P.Название [Название_предмета], round(avg(cast(O.Оценка as float(4))), 2) [Средняя_оценка] from ОЦЕНКИ O
inner join ПРЕДМЕТЫ P on O.ID_предмета = P.ID_предмета
group by P.Название order by [Средняя_оценка] desc;

/*task 4*/
select P.Название [Название_предмета], round(avg(cast(O.Оценка as float(4))), 2) [Средняя_оценка] from ОЦЕНКИ O
inner join ПРЕДМЕТЫ P on O.ID_предмета = P.ID_предмета
where P.Название = 'Математика' or P.Название = 'Физика'
group by P.Название order by [Средняя_оценка] desc;

/*task 5*/
select P.Название as [Название_предмета], round(avg(cast(O.Оценка as float(4))), 2) as [Средняя_оценка] from ОЦЕНКИ O
inner join ПРЕДМЕТЫ P on O.ID_предмета = P.ID_предмета
where P.Название != 'Информатика' group by P.Название
order by [Средняя_оценка] desc;

/*task 6*/
select P.Название as [Название_предмета], count(O.ID_студента) as [Количество_студентов] from ОЦЕНКИ O
inner join ПРЕДМЕТЫ P on O.ID_предмета = P.ID_предмета
where O.Оценка = '8' or O.Оценка = '9'
group by P.Название having count(O.ID_студента) > 0 order by P.Название desc;

