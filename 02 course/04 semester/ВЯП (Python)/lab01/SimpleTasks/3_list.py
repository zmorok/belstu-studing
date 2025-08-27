import random as r

spisok = []
for s in range(1, 10): spisok.append(r.randint(1, 500))
  
print(f'{'*'*20}\nСписок: {spisok}\n{'*'*20}')
print(f'Последний элемент списка: {spisok[-1]}')
i = int(input('Введите ЧИСЛО для добавления: ')); spisok.append(i)
print(f'{'*'*20}\nНовый список: {spisok}\n{'*'*20}')
c = int(input('Введите число для отслеживания: '))
print(f'Число {c} встречается в списке: {spisok.count(c)} раз')
spisok.sort; print(f'{'*'*20}\nОтсортированный список: {spisok}\n{'*'*20}')

sent = input('Ваше предложение: ')
li = sent.split(' ')
print('Список из слов: ', li, '\nКол-во слов: ', len(li))