import math
from collections import Counter

def calculate_text_entropy(file_path, alphabet):
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            text = f.read().lower()
    except FileNotFoundError:
        print(f"Ошибка: Файл не найден по пути {file_path}")
        return None, None

    filtered_text = [char for char in text if char in alphabet]
    
    if not filtered_text:
        print("Ошибка: В файле не найдено символов указанного алфавита.")
        return 0, {}

    total_chars = len(filtered_text)
    frequencies = Counter(filtered_text)
    
    entropy = 0.0
    for char in frequencies:
        probability = frequencies[char] / total_chars
        entropy -= probability * math.log2(probability)
        
    return entropy, frequencies

def calculate_conditional_entropy(p):
    if p == 0 or p == 1:
        return 0.0
    return -p * math.log2(p) - (1 - p) * math.log2(1 - p)

# испоьзование
CYRILLIC_ALPHABET = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"
LATIN_ALPHABET = "abcdefghijklmnopqrstuvwxyz"


cyrillic_file = 'cyrillic_text.txt'
latin_file = 'latin_text.txt'

source_file = cyrillic_file
bin_file = 'binary_data.bin'

# для бинарника
try:
    with open(source_file, 'rt', encoding="utf-8") as f_in:
        text_content = f_in.read()
        print(f"Прочитано {len(text_content)} символов из '{source_file}'.")

    binary_content = ''.join(format(byte, '08b') for byte in text_content.encode('utf-8'))
    
    print(f"Содержимое кодировано в {len(binary_content)} байт.")

    with open(bin_file, 'w') as f_out:
        f_out.write(binary_content)
    
    print(f"Бинарник записан в '{bin_file}'.\n\n")

except FileNotFoundError:
    print(f"ОШИБКА: Файл '{source_file}' не найден.")
except Exception as e:
    print(f"ОШИБКА: {e}")


h_cyrillic, freq_cyrillic = calculate_text_entropy(cyrillic_file, CYRILLIC_ALPHABET)
h_latin, freq_latin = calculate_text_entropy(latin_file, LATIN_ALPHABET)
h_binary, freq_binary = calculate_text_entropy(bin_file, "01")

print("\nРезультаты анализа энтропии:")

if h_cyrillic:
    print(f"\nЭнтропия кириллического алфавита: {h_cyrillic:.4f} бит/символ")
    for char, count in sorted(freq_cyrillic.items()):
        print(f"'{char}': {count}")

if h_latin:
    print(f"\nЭнтропия латинского алфавита: {h_latin:.4f} бит/символ")
    for char, count in sorted(freq_latin.items()):
        print(f"'{char}': {count}")

if h_binary:
    print(f"\nЭнтропия бинарного алфавита: {h_binary:.4f} бит/символ")
    for char, count in sorted(freq_binary.items()):
        print(f"'{char}': {count}")


print("\n\n\nРасчет количества информации в сообщении")
full_name = "ЖукДмитрийСергеевич"

if h_cyrillic:
    name_only_letters = "".join(c for c in full_name.lower() if c in CYRILLIC_ALPHABET)
    information_shannon = h_cyrillic * len(name_only_letters)
    print(f"- Количество букв в ФИО: {len(name_only_letters)}")
    print(f"RUS: Количество информации: {h_cyrillic:.4f} * {len(name_only_letters)} = {information_shannon:.4f} бит")

bits_per_char_ascii = 8
information_ascii = len(full_name) * bits_per_char_ascii

print(f"BIN: Количество информации: {len(full_name)} * {bits_per_char_ascii} = {information_ascii:.4f} бит")

print("\n\n\nРасчет количества информации при наличии вероятности ошибки")
error_probabilities = [0.1, 0.5, 1.0]

for p in error_probabilities:
    conditional_entropy = calculate_conditional_entropy(p)

    # Расчет для ASCII
    total_ascii_bits = len(full_name) * bits_per_char_ascii
    information_in_channel_ascii = total_ascii_bits * (1 - conditional_entropy)

    print(f"\nВероятность ошибки p = {p}")
    print(f"Условная энтропия = {conditional_entropy:.4f} бит/бит")
    print(f"Количество информации (в кодах ASCII): {total_ascii_bits} * (1 - {conditional_entropy:.4f}) = {information_in_channel_ascii:.4f} бит")

    # Расчет для кириллицы (на основе энтропии)
    # if h_cyrillic:
    #     total_cyrillic_bits = h_cyrillic * len(name_only_letters)
    #     information_in_channel_shannon = total_cyrillic_bits * (1 - conditional_entropy)
    #     print(f"Количество информации (на основе энтропии алфавита): {total_cyrillic_bits:.4f} * (1 - {conditional_entropy:.4f}) = {information_in_channel_shannon:.4f} бит")
