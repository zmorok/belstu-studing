select Фамилия, Имя, Отчество
from СТУДЕНТЫ S
where exists (
	select 1
	from ПОСЕЩАЕМОСТЬ P, ПРЕДМЕТЫ PR
	where P.ID_предмета = PR.ID_предмета
    and P.ID_студента = S.ID_студента
    and P.Часы_лекций < PR.Часы_лекций
)

update ПОСЕЩАЕМОСТЬ
	set Часы_лекций = 100
	where ID_студента = 2