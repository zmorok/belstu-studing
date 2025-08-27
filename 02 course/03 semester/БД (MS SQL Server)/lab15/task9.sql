use Univer
go

create trigger DDL_UNIVER on database for DDL_DATABASE_LEVEL_EVENTS
as begin
    declare @event_type varchar(50) = eventdata().value('(/EVENT_INSTANCE/EventType)[1]', 'varchar(50)')
    declare @object_name varchar(128) = parsename(eventdata().value('(/EVENT_INSTANCE/ObjectName)[1]', 'varchar(128)'), 1)
    declare @object_type varchar(50) = eventdata().value('(/EVENT_INSTANCE/ObjectType)[1]', 'varchar(50)')
    
    if @object_name in (
        select name 
        from sys.tables 
        where schema_name() = 'dbo' 
          and name not in ('sysdiagrams', 'TR_AUDIT')
    )
    begin
        declare @error_message varchar(300) = 'Операции с таблицей ' + rtrim(@object_name) + ' запрещены'
        print 'Тип события: ' + @event_type
        print 'Имя объекта: ' + @object_name
        print 'Тип объекта: ' + @object_type
        raiserror(@error_message, 16, 1)
        rollback
    end
end
go

drop trigger DDL_UNIVER

   select * from PULPIT
   alter table PULPIT add FACCCCC char