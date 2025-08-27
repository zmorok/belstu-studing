import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

file_path = 'books.csv'
df = pd.read_csv(file_path)

print('Первые данные:')
print(df.head())

df_sorted = df.sort_values(by='price', ascending=True)

print('\nПервые 5 строк отсортированных данных:')
print(df_sorted.head())

print('\nСтатистическое описание данных:')
print(df_sorted.describe())

if df_sorted.duplicated(subset=['title']).any():
  grouped = df_sorted.groupby('title')['price'].mean()
  print('\nСредняя цена повторяющихся книг:')
  print(grouped)

plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
sns.histplot(df_sorted['price'])
plt.title('Распределение цен книг')
plt.xlabel('Цена (£)')
plt.ylabel('Количество')

plt.subplot(1, 2, 2)
sns.boxplot(x=df_sorted['price'])
plt.title('Box-Plot цен книг')

plt.tight_layout()
plt.show()

plt.figure(figsize=(8, 5))
sns.scatterplot(x=range(len(df_sorted)), y=df_sorted['price'])
plt.title('Диаграмма рассеяния цен книг')
plt.xlabel('Индекс книги')
plt.ylabel('Цена (£)')
plt.show()