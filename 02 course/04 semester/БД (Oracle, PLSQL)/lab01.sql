create table DSZ_t 
(
    digits number(3) primary key,
    charrs varchar2(50)
)

insert into DSZ_t (digits, charrs) values (1, 'sdsd');
insert into DSZ_t (digits, charrs) values (2, 'sdwqeqd');
insert into DSZ_t (digits, charrs) values (3, 'e12eqws');
insert into DSZ_t (digits, charrs) values (4, 'pfgbcm 4');
insert into DSZ_t (digits, charrs) values (5, 'pfgbcm 5');
commit


update DSZ_t set charrs = 'строка 2' where digits = 2;
update DSZ_t set charrs = 'строка 3' where digits = 3;
commit

select * from dsz_t

select sum(digits) as "Сумма= " from dsz_t

delete from DSZ_t where digits = 1
rollback

create table DSZ_t_child
(
    id int primary key,
    digits number(3),
    charrs varchar2(50),
    constraint fk_digits foreign key (digits) references DSZ_t(digits)
)

insert into DSZ_t_child (id, digits, charrs) values (1, 1, 'text 1');
insert into DSZ_t_child (id, digits, charrs) values (2, 2, 'text 2');
insert into DSZ_t_child (id, digits, charrs) values (3, 3, 'text 3');
insert into DSZ_t_child (id, digits, charrs) values (4, 4, 'text 4');
insert into DSZ_t_child (id, digits, charrs) values (5, 4, 'text 4_5');
commit;
select * from DSZ_t_child

select p.charrs, c.charrs from DSZ_t p inner join DSZ_t_child c on p.digits = c.digits;
select p.charrs, c.charrs from DSZ_t p left join DSZ_t_child c on p.digits = c.digits;

drop table DSZ_t_child;
drop table DSZ_t;
