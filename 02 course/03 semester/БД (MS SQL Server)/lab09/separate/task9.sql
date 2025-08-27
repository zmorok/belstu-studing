begin try
    create table #TT (ID int not null);
    insert into #TT (ID) values (null);
end try
begin catch
    select 
        ERROR_NUMBER() [Номер],
        ERROR_MESSAGE() [Сообщение],
        ERROR_LINE() [Номер строки],
        ERROR_PROCEDURE() [Имя процедуру или null],
        ERROR_SEVERITY() [Уровень серьёзности],
        ERROR_STATE() [Метка ошибки];
end catch

drop table #TT