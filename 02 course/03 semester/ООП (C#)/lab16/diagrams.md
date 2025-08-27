[plantuml.com](https://plantuml.com/ru/)

<details>
<summary>1. Диаграмма вариантов использования (Use Case Diagram)</summary>

```plantuml
@startuml
actor Врач
actor Пациент
actor Медсестра

usecase "Sign In" as SignIn
usecase "Sign Out" as SignOut
usecase "Регистрация" as Registration
usecase "Просмотр информации" as ViewInfo
usecase "Добавление информации" as AddInfo
usecase "Модификация информации" as ModifyInfo
usecase "Удаление информации" as DeleteInfo
usecase "Определение диагноза" as Diagnose
usecase "Создание назначения" as Prescription
usecase "Выполнение назначения" as ExecutePrescription
usecase "Выписка пациента" as Discharge

Врач --> SignIn
Врач --> SignOut
Врач --> ViewInfo
Врач --> AddInfo
Врач --> Diagnose
Врач --> Prescription
Врач --> ModifyInfo
Врач --> DeleteInfo
Врач --> Discharge

Медсестра --> SignIn
Медсестра --> SignOut
Медсестра --> ViewInfo
Медсестра --> ExecutePrescription

Пациент --> SignIn
Пациент --> SignOut
Пациент --> Registration
Пациент --> ViewInfo

@enduml
```

</details>

<details>

<summary>2. Диаграмма классов (Class Diagram)</summary>

```plantuml
@startuml
class Пользователь {
    +int id
    +string name
    +string role
    +signIn()
    +signOut()
}

class Врач {
    +определитьДиагноз(диагноз: string)
    +сделатьНазначение(назначение: Назначение)
}

class Медсестра {
    +выполнитьНазначение(назначение: Назначение)
}

class Пациент {
    +просмотретьНазначения()
    +историяБолезни: string
    +назначения: Назначение[]
}

class Назначение {
    +int id
    +string тип
    +string описание
    +создать()
    +изменить()
}

class Диагноз {
    +int id
    +string описание
    +зафиксироватьОкончательныйДиагноз()
}

Пользователь <|-- Врач
Пользователь <|-- Медсестра
Пользователь <|-- Пациент
Врач --> Диагноз
Врач --> Назначение
Пациент --> Назначение
Медсестра --> Назначение

@enduml
```

</details>

<details>
<summary>3. Диаграмма последовательности (Sequence Diagram)
   Для примера возьмем процесс "Сделать назначение" врачом.</summary>

```plantuml
@startuml
actor Врач
participant Пациент
participant Назначение
participant Система

Врач -> Система : signIn()
Врач -> Пациент : определитьДиагноз()
Врач -> Назначение : создатьНазначение(тип, описание)
Назначение -> Пациент : добавитьНазначение()
Врач -> Система : signOut()

@enduml
```

</details>

<details>
<summary>4. Диаграмма деятельности (Activity Diagram)
   Диаграмма для процесса "Выписка пациента".</summary>

```plantuml
@startuml
start
:Проверка состояния пациента;
if (Пациент готов к выписке?) then (да)
    :Фиксация окончательного диагноза;
    :Подготовка выписного эпикриза;
    :Выдача инструкций по лечению;
else (нет)
    :Продолжение лечения;
endif
:Завершение работы с пациентом;
stop
@enduml
```

</details>

<details>
<summary>5. Диаграмма компонентов (Component Diagram)</summary>

```plantuml
@startuml
package "Система Больницы" {
    [Сервер БД] - [Приложение Врача]
    [Сервер БД] - [Приложение Медсестры]
    [Сервер БД] - [Приложение Пациента]

    [Приложение Врача] --> [Сервер БД]
    [Приложение Медсестры] --> [Сервер БД]
    [Приложение Пациента] --> [Сервер БД]
}
@enduml
```

</details>
