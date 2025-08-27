SELECT Фамилия, Имя, Отчество, Оценка, Название
FROM СТУДЕНТЫ S
JOIN ОЦЕНКИ O ON S.ID_студента = O.ID_студента
JOIN ПРЕДМЕТЫ P ON O.ID_предмета = P.ID_предмета
WHERE Оценка > (
    SELECT AVG(CAST(Оценка AS FLOAT))
    FROM ОЦЕНКИ
    WHERE ID_предмета = O.ID_предмета
);

select Фамилия, Имя, Отчество, Оценка, Название from СТУДЕНТЫ S, ОЦЕНКИ O, ПРЕДМЕТЫ P
where S.ID_студента = O.ID_студента
	and O.ID_предмета = P.ID_предмета
	and S.ID_студента in (
		select ID_студента from ОЦЕНКИ
		where Оценка > (select avg(cast(Оценка as int)) from ОЦЕНКИ where ID_предмета = O.ID_предмета)
	)