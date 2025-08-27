employees = { 'Иванов': 50000, 'Петров': 60000, 'Сидоров': 55000, 'Кузнецов': 70000 }

key = 'Петров'
salary = employees.get(key, 'Сотрудник не найден')
print(f'Зарплата {key}: {salary}')

key = 'Смирнов'
salary = employees.get(key, 'Сотрудник не найден')
print(f'Зарплата {key}: {salary}')

employees['Смирнов'] = 65000
print('Добавлен новый сотрудник:', employees)

print(f'Количество сотрудников: {len(employees)}')

print('Список сотрудников:', list(employees.keys()))

average_salary = sum(employees.values()) / len(employees)
print(f'Средняя зарплата: {average_salary}')
