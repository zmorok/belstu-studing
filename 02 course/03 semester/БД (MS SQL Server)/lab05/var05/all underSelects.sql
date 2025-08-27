use lab05_db;
go

/*1*/
select Название from ПРЕДМЕТЫ
where ID_предмета in (
    select ID_предмета from ОЦЕНКИ where Оценка like 8
);

/*2*/
select Название from ПРЕДМЕТЫ P 
inner join ОЦЕНКИ O on O.ID_предмета = P.ID_предмета 
where O.ID_студента in (
    select ID_студента from СТУДЕНТЫ where Фамилия like 'Ива%'
);

/*3*/
select Название from ПРЕДМЕТЫ P
inner join ОЦЕНКИ O on P.ID_предмета = O.ID_предмета
where Оценка = 4;

/*4*/
select Часы_лекций, Часы_практических_занятий from ПРЕДМЕТЫ P1
where Часы_лекций = (
    select top 1 Часы_лекций 
    from ПРЕДМЕТЫ P2 
    where P1.Название = P2.Название 
    order by Часы_лекций desc
) order by Часы_лекций desc;

/*5*/
select Фамилия from СТУДЕНТЫ
where not exists (
    select * from ОЦЕНКИ
    where ОЦЕНКИ.ID_студента = СТУДЕНТЫ.ID_студента
);

/*6*/
select top 1
    (select avg(cast(Оценка as float)) from ОЦЕНКИ where ID_предмета = 1) as [Средняя по Математике],
    (select avg(cast(Оценка as float)) from ОЦЕНКИ where ID_предмета = 2) as [Средняя по Физике],
    (select avg(cast(Оценка as float)) from ОЦЕНКИ where ID_предмета = 3) as [Средняя по Информатике];

/*7*/
select Фамилия from СТУДЕНТЫ
where Фамилия like 'П%' 
and ID_студента >= all (select ID_студента from СТУДЕНТЫ where Фамилия like 'И%');

/*8*/
select Фамилия, Имя from СТУДЕНТЫ
where ID_студента = any (select ID_студента from ОЦЕНКИ where Оценка < 6);


select Адрес, count(*) as cnt from СТУДЕНТЫ
group by Адрес having count(*) = 1