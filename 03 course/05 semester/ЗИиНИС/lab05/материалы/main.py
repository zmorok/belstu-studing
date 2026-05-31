import random

def xor(bits):
    x = 0
    for b in bits:
        x ^= (b & 1)
    return x

def encode(Xk, k1=3, k2=8, parity_groups=2):
    k = len(Xk)
    matrix = [Xk[i*k2:(i+1)*k2] for i in range(k1)]

    h_parities = [xor(row) for row in matrix]
    v_parities = [xor([matrix[i][j] for i in range(k1)]) for j in range(k2)]

    Xn = Xk + h_parities + v_parities
    parity_count = k1 + k2

    if parity_groups >= 3:
        Xhv = xor(Xk + h_parities + v_parities)
        Xn.append(Xhv)
        parity_count += 1

    return Xn, parity_count

def generate_errors(Xn, error_count):
    Yn = Xn.copy()
    error_positions = random.sample(range(len(Xn)), error_count)
    for pos in error_positions:
        Yn[pos] ^= 1
    return Yn, error_positions

def decode(Yn, k1=3, k2=8, parity_groups=2):
    k = k1 * k2
    Yk = Yn[:k]
    matrix = [Yk[i*k2:(i+1)*k2] for i in range(k1)]

    computed_h = [xor(row) for row in matrix]
    computed_v = [xor([matrix[i][j] for i in range(k1)]) for j in range(k2)]

    parity_start = k
    Yh = Yn[parity_start:parity_start + k1]
    Yv = Yn[parity_start + k1:parity_start + k1 + k2]

    error_h = [Yh[i] ^ computed_h[i] for i in range(k1)]
    error_v = [Yv[j] ^ computed_v[j] for j in range(k2)]
    errors_detected = sum(error_h) + sum(error_v)

    additional_parities = Yn[parity_start + k1 + k2:]
    error_additional_sum = 0
    if parity_groups >= 3:
        computed_Xhv = xor(Yk + computed_h + computed_v)
        if additional_parities:
            error_additional_sum = additional_parities[0] ^ computed_Xhv

    if errors_detected == 0 and error_additional_sum == 0:
        return Yk, "Ошибок не обнаружено"

    error_positions = []
    for i in range(k1):
        if error_h[i]:
            for j in range(k2):
                if error_v[j]:
                    error_positions.append((i, j))

    if len(error_positions) == 1 and error_additional_sum <= 1:
        i, j = error_positions[0]
        matrix[i][j] ^= 1
        Yk_corrected = [bit for row in matrix for bit in row]
        return Yk_corrected, "Ошибка исправлена"
    else:
        return Yk, "Обнаружены множественные ошибки"

def analyze_correction(k1, k2, parity_groups, error_count, N1):
    k = k1 * k2
    N2 = 0
    N3 = 0

    for _ in range(N1):
        Xk = [random.randint(0, 1) for _ in range(k)]
        Xn, _ = encode(Xk, k1, k2, parity_groups)
        Yn, _ = generate_errors(Xn, error_count)
        Yk_corrected, message = decode(Yn, k1, k2, parity_groups)

        if message != "Обнаружены множественные ошибки":
            N2 += 1
            if Yk_corrected == Xk:
                N3 += 1

    return N2 / N1, N3 / N1

if __name__ == "__main__":
    k1, k2 = 4, 8
    k = k1 * k2
    Xk = [random.randint(0, 1) for _ in range(k)]

    for pg in [2, 3]:
        print(f"Проверка с {pg} группами паритетов")

        Xn, parity_count = encode(Xk, k1, k2, pg)
        print(f"Исходное сообщение Xk: (длина= {len(Xk)})        {Xk}")
        print(f"Закодированное слово Xn: (длина= {len(Xk)})      {Xn}")

        Yn, error_pos = generate_errors(Xn, 1)
        print(f"Позиция ошибки:                           {error_pos[0] + 1}")
        print(f"Принятое слово (с ошибкой) Yn:            {Yn}")

        Yk_corrected, message = decode(Yn, k1, k2, pg)
        print(f"Исправленное сообщение Yk:                {Yk_corrected}")
        print(f"Результат декодирования: {message}")

        N1 = 1000
        N2_N1, N3_N1 = analyze_correction(k1, k2, pg, 1, N1)
        print(f"Анализ (1 ошибка, {N1} испытаний): N2/N1={N2_N1:.3f}, N3/N1={N3_N1:.3f}\n")
