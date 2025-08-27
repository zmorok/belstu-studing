use Univer
go

create trigger FACULTY_ISTEAD_OF on FACULTY instead of DELETE as raiserror('Удаление из FACULTY запрещено', 10, 1) return
go

select * from FACULTY
go

delete from FACULTY where FACULTY = 'ХТиТ'
go

drop trigger FACULTY_INSTEAD_OF
go