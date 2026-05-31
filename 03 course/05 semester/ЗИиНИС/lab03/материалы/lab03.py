import math
from pathlib import Path
from collections import Counter

LATIN_ALPHABET = set("abcdefghijklmnopqrstuvwxyz")
BASE64_ALPHABET = set("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=")

def read_text(path: str) -> str:
    return Path(path).read_text(encoding="utf-8", errors="ignore")

def filter_symbols(text: str, alphabet: set, *, casefold: bool) -> list[str]:
    t = text.casefold() if casefold else text
    return [ch for ch in t if ch in alphabet]

def shannon_entropy(freqs: Counter) -> float:
    total = sum(freqs.values())
    if total == 0:
        return 0.0
    H = 0.0
    for cnt in freqs.values():
        if cnt > 0:
            p = cnt / total
            H -= p * math.log2(p)
    return H

def hartley_entropy(M: int) -> float:
    return math.log2(M) if M > 0 else 0.0

def redundancy_percent(H_ch: float, H_s: float) -> float:
    return 0.0 if H_ch == 0 else (1 - (H_s / H_ch)) * 100.0

def analyze(label: str, text: str, alphabet: set, *, casefold: bool) -> None:
    symbols = filter_symbols(text, alphabet, casefold=casefold)
    freqs = Counter(symbols)

    H_ch = hartley_entropy(len(alphabet))
    H_s  = shannon_entropy(freqs)
    R    = redundancy_percent(H_ch, H_s)

    print(f"\n{label}")
    print(f"Энтропия Хартли:        {H_ch:.6f} бит/символ")
    print(f"Энтропия Шеннона:       {H_s:.6f} бит/символ")
    print(f"Избыточность алфавита:  {R:.4f}%")

def main():
    text_a = read_text("text.txt")
    text_b = read_text("base.txt")

    analyze("# Расчёты для текста в латинице:", text_a, LATIN_ALPHABET, casefold = True)
    analyze("# Расчёты для текста в Base64:",  text_b, BASE64_ALPHABET, casefold = False)

if __name__ == "__main__":
    main()
