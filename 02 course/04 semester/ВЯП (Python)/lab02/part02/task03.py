service_a = {'Анна', 'Иван', 'Мария', 'Сергей', 'Алексей'}
service_b = {'Мария', 'Иван', 'Дмитрий', 'Ольга', 'Светлана'}
service_c = {'Сергей', 'Ольга', 'Александр', 'Иван', 'Анна'}

all_services_users = service_a & service_b & service_c
only_in_a = service_a - (service_b | service_c)
only_in_b = service_b - (service_a | service_c)
only_in_c = service_c - (service_a | service_b)
unique_users = only_in_a | only_in_b | only_in_c

unique_counts = {
  'сервис A': len(only_in_a),
  'сервис B': len(only_in_b),
  'сервис C': len(only_in_c),
}

largest_unique_service = max(unique_counts, key=unique_counts.get)

print('Пользователи во всех сервисах:', all_services_users)
print('Уникальные пользователи каждого сервиса:', unique_users)
print('Сервис с наибольшим количеством уникальных пользователей:', largest_unique_service)
