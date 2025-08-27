-- drop database freelanceplatform;
-- create database freelanceplatform;

use freelanceplatform;
go

-- таблица "roles"
create table roles (
    id_role int primary key identity(1,1),
    role_name nvarchar(15) not null,
    role_privileges text not null,
);

-- таблица "users"
create table users (
    id_user int primary key identity(1,1),
    role int not null,
    last_name nvarchar(50) not null,
    first_name nvarchar(50) not null,
    middle_name nvarchar(50),
    gender nchar(1) not null,
    phone_number nvarchar(15) not null,
	email nvarchar(30) not null,
	registration_date datetime not null,
    last_online_time datetime not null,
    constraint fk_users_role foreign key (role) references roles(id_role),
);

-- таблица "projects"
create table projects (
    id_project int primary key identity(1,1),
    id_customer int not null,
    title nvarchar(100) not null,
    status nvarchar(15) not null,
    description text not null,
    constraint fk_projects_customer foreign key (id_customer) references users(id_user),
);

-- таблица "reviews"
create table reviews (
    id_review int primary key identity(1,1),
    id_author int not null,
    id_recipient int not null,
    comment text,
    rating tinyint not null check (rating between 1 and 5),
    constraint fk_reviews_author foreign key (id_author) references users(id_user),
    constraint fk_reviews_recipient foreign key (id_recipient) references users(id_user),
);

-- таблица "payments"
create table payments (
    id_payment int primary key identity(1,1),
    payment_date datetime not null,
    amount decimal(10, 2) not null,
);

-- таблица "orders"
create table orders (
    id_order int primary key identity(1,1),
    id_project int not null,
    id_freelancer int not null,
    id_payment int not null unique,
    status nvarchar(15) not null,
    constraint fk_orders_project foreign key (id_project) references projects(id_project),
    constraint fk_orders_freelancer foreign key (id_freelancer) references users(id_user),
    constraint fk_orders_payment foreign key (id_payment) references payments(id_payment),
);

-- таблица "conversations"
create table conversations (
    id_conversation int primary key identity(1,1),
    id_sender int not null,
    id_recipient int not null,
    constraint fk_conversations_sender foreign key (id_sender) references users(id_user),
    constraint fk_conversations_receiver foreign key (id_recipient) references users(id_user),
);

-- таблица "messages"
create table messages (
    id_message int primary key identity(1,1),
    id_conversation int not null,
    text_content text not null,
    constraint fk_messages_conversation foreign key (id_conversation) references conversations(id_conversation),
);

-- таблица "complaints"
create table complaints (
    id_complaint int primary key identity(1,1),
    id_user int not null,
    id_moderator int not null,
	status nvarchar(15) not null,
    description text not null,
    constraint fk_complaints_user foreign key (id_user) references users(id_user),
    constraint fk_complaints_moderator foreign key (id_moderator) references users(id_user)
);


