tuple_1 = (11, 12, 13, [10, 20, 30], 13, [12, 232, 323], '231sdada')

for el in tuple_1:
  if not isinstance(el, int) and isinstance(el, list):
    print(f'** Найден список: {el}')
    print(f'Первый элемент вложенного списка: {el[0]}')
    print(f'Последний элемент вложенного списка: {el[-1]}')
    print('*'*30)