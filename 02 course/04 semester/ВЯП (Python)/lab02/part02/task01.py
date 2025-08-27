def word_frequency(text):
  words = text.lower().split()
  word_count = {}

  for word in words:
    if word in word_count:
      word_count[word] += 1
    else:
      word_count[word] = 1

  return word_count

text = 'один пять дом дом дом пять один три солнце'
print(word_frequency(text))

