use Univer
go

create trigger TR_TEACHER_DEL1 on TEACHER after DELETE  
as print 'TR_TEACHER_DELETE_1'
return
go
-- drop trigger TR_TEACHER_DEL1

create trigger TR_TEACHER_DEL2 on TEACHER after DELETE  
as print 'TR_TEACHER_DELETE_2'
return
go
-- drop trigger TR_TEACHER_DEL2

create trigger TR_TEACHER_DEL3 on TEACHER after DELETE  
as print 'TR_TEACHER_DELETE_3'
return
go
-- drop trigger TR_TEACHER_DEL3

select t.name, e.type_desc from sys.triggers t 
join sys.trigger_events e on t.object_id = e.object_id  
where object_name(t.parent_id) = 'TEACHER' 
and e.type_desc = 'DELETE'

exec sp_settriggerorder @triggername = 'TR_TEACHER_DEL3', @order = 'First', @stmttype = 'DELETE'

exec sp_settriggerorder @triggername = 'TR_TEACHER_DEL2', @order = 'Last', @stmttype = 'DELETE'