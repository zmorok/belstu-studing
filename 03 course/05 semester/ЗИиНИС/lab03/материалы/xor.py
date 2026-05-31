import base64

def ascii_to_bits(s: str) -> str:
    b = s.encode('ascii', errors='strict')
    return ''.join(f'{byte:08b}' for byte in b)

def left_pad_equal(a_bits: str, b_bits: str) -> tuple[str, str]:
    L = max(len(a_bits), len(b_bits))
    return a_bits.rjust(L, '0'), b_bits.rjust(L, '0')

def xor_bits(a_bits: str, b_bits: str) -> str:
    assert len(a_bits) == len(b_bits)
    return ''.join('1' if x != y else '0' for x, y in zip(a_bits, b_bits))

def b64_of_bits(bits: str) -> str:
    if len(bits) % 8 != 0:
        bits = ('0' * (8 - (len(bits) % 8))) + bits
    data = bytes(int(bits[i:i+8], 2) for i in range(0, len(bits), 8))
    return base64.b64encode(data).decode('ascii')

def bits_of_b64(s_b64: str) -> str:
    b = s_b64.encode('ascii')
    return ''.join(f'{byte:08b}' for byte in b)

def run(surname: str, name: str):
    a_bits = ascii_to_bits(surname)
    b_bits = ascii_to_bits(name)
    a_bits_p, b_bits_p = left_pad_equal(a_bits, b_bits)
    ab_bits = xor_bits(a_bits_p, b_bits_p)

    print("Исходные данные (ASCII):")
    print(f"Фамилия :   {a_bits_p}")
    print(f"Имя     :   {b_bits_p}")
    print(f"a XOR b :   {ab_bits}")

    ab_xor_b = xor_bits(ab_bits, b_bits_p)
    print(f"\na^b^b   :   {ab_xor_b}\n")


    a_b64 = b64_of_bits(a_bits)
    b_b64 = b64_of_bits(b_bits)

    a_b64_bits = bits_of_b64(a_b64)
    b_b64_bits = bits_of_b64(b_b64)
    a_b64_bits_p, b_b64_bits_p = left_pad_equal(a_b64_bits, b_b64_bits)
    ab_b64_bits = xor_bits(a_b64_bits_p, b_b64_bits_p)

    print("\nBase64:")
    print(f"Фамилия :   {a_b64_bits_p}")
    print(f"Имя     :   {b_b64_bits_p}")
    print(f"a XOR b :   {ab_b64_bits}\n")
    
    
    

if __name__ == "__main__":
    run(surname="Zhuk", name="Dmitry")
