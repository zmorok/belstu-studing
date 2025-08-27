-- 0. Если нужно, сперва удаляем старую БД
IF DB_ID('FreelancePlatform') IS NOT NULL
BEGIN
    ALTER DATABASE FreelancePlatform SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
    DROP DATABASE FreelancePlatform;
END
GO

-- 1. Создаём чистую базу
CREATE DATABASE FreelancePlatform;
GO

-- 2. Переключаемся на неё
USE FreelancePlatform;
GO

-- 3. Freelancers
CREATE TABLE Freelancers (
    FreelancerID    INT             IDENTITY(1,1) PRIMARY KEY,
    FullName        NVARCHAR(100)   NOT NULL,
    Email           NVARCHAR(100)   NOT NULL UNIQUE,
    ProfilePhoto    VARBINARY(MAX)  NULL,
    RegistrationDate DATETIME       NOT NULL DEFAULT GETDATE()
);
GO

-- 4. Clients
CREATE TABLE Clients (
    ClientID     INT             IDENTITY(1,1) PRIMARY KEY,
    FullName     NVARCHAR(100)   NOT NULL,
    Email        NVARCHAR(100)   NOT NULL UNIQUE,
    Company      NVARCHAR(100)   NULL,
    ClientPhoto  VARBINARY(MAX)  NULL
);
GO

-- 5. Projects
CREATE TABLE Projects (
    ProjectID    INT             IDENTITY(1,1) PRIMARY KEY,
    ClientID     INT             NOT NULL
                           CONSTRAINT FK_Projects_Clients REFERENCES Clients(ClientID),
    Title        NVARCHAR(200)   NOT NULL,
    Description  NVARCHAR(MAX)   NULL,
    Budget       DECIMAL(18,2)   NULL,
    CreatedDate  DATETIME        NOT NULL DEFAULT GETDATE(),
    ProjectPhoto VARBINARY(MAX)  NULL
);
GO

-- 6. Orders
CREATE TABLE Orders (
    OrderID      INT             IDENTITY(1,1) PRIMARY KEY,
    ProjectID    INT             NOT NULL
                           CONSTRAINT FK_Orders_Projects REFERENCES Projects(ProjectID),
    FreelancerID INT             NOT NULL
                           CONSTRAINT FK_Orders_Freelancers REFERENCES Freelancers(FreelancerID),
    AssignedDate DATETIME        NOT NULL DEFAULT GETDATE(),
    Status       NVARCHAR(50)    NOT NULL DEFAULT 'Назначен'
);
GO

-- 7. Добавляем хранимую процедуру
CREATE PROCEDURE AddProject
    @ClientID    INT,
    @Title       NVARCHAR(200),
    @Description NVARCHAR(MAX),
    @Budget      DECIMAL(18,2)
AS
BEGIN
    INSERT INTO Projects (ClientID, Title, Description, Budget, CreatedDate)
    VALUES (@ClientID, @Title, @Description, @Budget, GETDATE());
END;
GO

-- 8. Процедура для заказов по фрилансеру
CREATE PROCEDURE GetOrdersByFreelancer
    @FreelancerID INT
AS
BEGIN
    SELECT 
      o.OrderID, 
      p.Title       AS ProjectTitle, 
      o.AssignedDate, 
      o.Status
    FROM Orders o
    JOIN Projects p ON p.ProjectID = o.ProjectID
    WHERE o.FreelancerID = @FreelancerID;
END;
GO

-- 9. Триггер на вставку заказа
CREATE TRIGGER trg_AfterInsertOrder
ON Orders
AFTER INSERT
AS
BEGIN
    PRINT 'Фрилансеру назначен новый заказ.';
END;
GO
