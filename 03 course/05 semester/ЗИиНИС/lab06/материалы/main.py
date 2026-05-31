from sympy import Poly, symbols, GF
import numpy as np
import random

x = symbols('x')
g = Poly(x**5 + x**3 + x**2 + x + 1, x, domain=GF(2))

k = 9
r = 5

n = k + r

def print_bin(bin):
    return ''.join(str(e) for e in bin)

def generate_systematic_G():
    P = np.zeros((k, r), dtype=int)
    for i in range(k):
        power = Poly(x**(n - 1 - i), x, domain=GF(2))
        rem = power % g
        coeffs = rem.all_coeffs()
        padded = [0] * (r - len(coeffs)) + coeffs
        P[i] = padded  # high to low
    I_k = np.eye(k, dtype=int)
    G = np.hstack((I_k, P))
    return G

def generate_H_from_G(G):
    k, n = G.shape
    r = n - k

    P = G[:, k:]              # размер k × r

    M = np.vstack((P, np.eye(r, dtype=int)))  # размер (k+r) × r = n × r

    H = M.T
    return H


def encode_matrix(message, G):
    msg = np.array(message, dtype=int)
    code = (msg @ G) % 2
    return code.tolist()

def normalize_syndrome(synd, r):
    if synd is None:
        return [0] * r
    from sympy import Poly as SymPoly

    if isinstance(synd, SymPoly):
        coeffs = synd.all_coeffs()
    else:
        coeffs = list(synd)

    if len(coeffs) > r:
        coeffs = coeffs[-r:]

    pad = [0] * (r - len(coeffs))
    return pad + coeffs


def encode_cyclic(message):
    if len(message) != k:
        raise ValueError("Message length must be k")
    msg_poly = Poly(message, x, domain=GF(2))
    shifted = msg_poly * Poly(x**r, x, domain=GF(2))
    rem = shifted % g
    code_poly = shifted + rem
    code_coeffs = code_poly.all_coeffs()
    while len(code_coeffs) < n:
        code_coeffs = [0] + code_coeffs
    return code_coeffs

def decode_cyclic(received):
    rec_poly = Poly(received, x, domain=GF(2))
    synd = rec_poly % g
    if synd == Poly(0, x, domain=GF(2)):
        return received, "Ошибок нет", None
    synd_poly = synd
    for m in range(n):
        power = Poly(x**m, x, domain=GF(2)) % g
        if power == synd_poly:
            error_index = n - 1 - m  # 0-based from high degree
            corrected = received[:]
            corrected[error_index] = 1 - corrected[error_index]
            return corrected, f"Исправлена единичная ошибка на позиции {error_index + 1}", synd.all_coeffs()
    return received, "Множественные ошибки обнаружены (не корректируется)", synd.all_coeffs()

# Пример использования
message = [1] +[0]*3 + [1] + [0] + [1]*3
print("Matrix G:")
print("#"*30)
G = generate_systematic_G()
for i in range(len(G)):
    for j in range(len(G[0])):
        if (j == k):
            print(' | ' + str(G[i][j]), end=' ')
        else:
            print(str(G[i][j]), end=' ')
    print()
print("#"*30)

print("\nMatrix H:")
print("#"*30)
H = generate_H_from_G(G)

for i in range(len(H)):
    for j in range(len(H[0])):
        if (j == k):
            print(' | ' + str(H[i][j]), end=' ')
        else:
            print(str(H[i][j]), end=' ')
    print()
print("#"*30)
print()

codeword = encode_cyclic(message)  # или encode_matrix(message, G) - то же самое
print("Информационное слово X_k:\t", print_bin(message))
print("Кодовое слово X_n:\t\t", print_bin(codeword))
print("Проверочные биты X_r:\t\t", print_bin(codeword[-5:]))

# Эксперименты
corrected, status, synd = decode_cyclic(codeword)
print("\n0 ошибок:") 
print("- Исходное сообщение: ", print_bin(codeword))
print("- Исправл. сообщение: ", print_bin(corrected))
print(" | Синдром:", print_bin(normalize_syndrome(synd, r)), "\n | Статус:", status )

received_1err = codeword[:]
# err_pos = random.randint(0, n-1)
err_pos = 0
received_1err[err_pos] = 1 - received_1err[err_pos]
corrected, status, synd = decode_cyclic(received_1err)

print("\n1 ошибка:") 
print("- Исходное сообщение: ", print_bin(received_1err), "( позиция ", err_pos + 1, " )")
print("- Исправл. сообщение: ", print_bin(corrected))
print(" | Синдром:", print_bin(normalize_syndrome(synd, r)), "\n | Статус:", status )

received_2err = codeword[:]
# err_pos1 = random.randint(0, n-1)
# err_pos2 = random.randint(0, n-1)
err_pos1 = 6
err_pos2 = 7
received_2err[err_pos1] = 1 - received_2err[err_pos1]
received_2err[err_pos2] = 1 - received_2err[err_pos2]
corrected, status, synd = decode_cyclic(received_2err)

print("\n2 ошибки:") 
print("- Исходное сообщение: ", print_bin(received_2err), "( позиция ", err_pos1 + 1, "и", err_pos2 + 1, " )")
print("- Исправл. сообщение: ", print_bin(corrected))
print(" | Синдром:", print_bin(normalize_syndrome(synd, r)), "\n | Статус:", status )