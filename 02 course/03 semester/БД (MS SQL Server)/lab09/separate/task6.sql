use Univer;
go

select
  S.NAME [ФИО],
  G.PROFESSION [Специальность],
  P.SUBJECT [Дисциплина],
  P.NOTE [Оценка],
  case
    when P.NOTE >= 9 then 'Отлично'
    when P.NOTE >= 7 then 'Хорошо'
    when P.NOTE >= 4 then 'Удовлетворительно'
    else 'Неудовлетворительно'
  end [Категория оценки]
from
  STUDENT S
  join GROUPS G on S.IDGROUP = G.IDGROUP
  join PROGRESS P on S.IDSTUDENT = P.IDSTUDENT
  join FACULTY F on G.FACULTY = F.FACULTY
where F.FACULTY = 'ИТ'