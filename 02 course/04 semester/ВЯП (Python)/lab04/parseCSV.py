import requests
from bs4 import BeautifulSoup

import pandas as pd

url = 'https://books.toscrape.com/'

data = requests.get(url)

soup = BeautifulSoup(data.text, 'html.parser')
books = []

for book in soup.select('.product_pod'):
  title = book.h3.a['title']
  price = book.select_one('.price_color').text.replace('Â£', '')
  availability = book.select_one('.instock.availability').text.strip()
  books.append({'title': title, 'price': price, 'availability': availability})

for book in books:
  print(f'{book['title']} - {book['price']} - {book['availability']}')

df = pd.DataFrame(books)
df.to_csv('books.csv', index=False, encoding='utf-8')
print('Данные сохранены в books.csv')