import json, html, re
from collections import Counter
from pathlib import Path

import numpy as np
from PIL import Image
import nltk
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize

import pymorphy3
from wordcloud import WordCloud
import matplotlib.pyplot as plt


def load_telegram_json(json_path, skip_words = None) -> str:
    if skip_words is None:
        skip_words = {
            "tiktok",
            "https", 
            "com", "vm", "system", "daily", "bet", "free", "balance", "фишка", "баланс", "to", "the", "it", "i", "t", "s", "a", "а", "пиздец", "бля", "говно", "хуй"}

    data = json.loads(Path(json_path).read_text(encoding="utf-8"))
    parts: list[str] = []

    for msg in data.get("messages", []):
        # пропуск service-messages
        if msg.get("type") != "message":
            continue  

        # получение текста сообщения
        txt = msg.get("text", "")
        if isinstance(txt, list):
            txt = " ".join(
                piece if isinstance(piece, str) else piece.get("text", "")
                for piece in txt
            )
        txt = html.unescape(txt)

        # фильтр по ключевым словам, пропускаем сообщения с ними
        lower_txt = txt.lower()
        if any(word in lower_txt for word in skip_words):
            continue
        parts.append(txt)
    return "\n".join(parts)

# параметры
# имя пользователя/чата с экспорта тг
USER = "xtb" 
TELEGRAM_JSON = f"exports\\{USER}\\result.json"

 # имя файла с маской
MASK = "mask.png"
MASK_PATH = f"masks\\{MASK}"

text = load_telegram_json(TELEGRAM_JSON)
text = re.sub(r"[^\w\s]", " ", text, flags=re.UNICODE)
nltk.download("punkt", quiet=True)
tokens = word_tokenize(text.lower(), language="russian")

nltk.download("stopwords", quiet=True)
stops = set(stopwords.words("russian"))
tokens = [t for t in tokens if t.isalpha() and t not in stops]

morph = pymorphy3.MorphAnalyzer()
lemmas = [morph.parse(t)[0].normal_form for t in tokens]

# частота лемм
freq = Counter(lemmas)

# маска и облако
mask = np.array(Image.open(MASK_PATH))
wc = WordCloud(
    width=800, height=800,
    background_color="white",
    max_words=250,
    mask=mask,
    font_path="arial.ttf",
    contour_width=1,
    contour_color="steelblue"
).generate_from_frequencies(freq)

# визуализация
plt.figure(figsize=(8, 8))
plt.imshow(wc, interpolation="bilinear")
plt.axis("off")
plt.tight_layout()
plt.show()

wc.to_file(f"photos/cloud_from_{USER}.png")
