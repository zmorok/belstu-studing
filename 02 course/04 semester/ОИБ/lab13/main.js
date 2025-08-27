const crypto = require('crypto').webcrypto
const { TextEncoder, TextDecoder } = require('util')

const text = 'Жук'

async function main() {
	// 2. Генерация случайных чисел
	const randomArray = new Uint16Array(5)
	crypto.getRandomValues(randomArray)
	console.log('Случайные числа:', Array.from(randomArray))

	// 3. AES-GCM шифрование / дешифрование / хеширование
	const aesKey = await crypto.subtle.generateKey(
		{ name: 'AES-GCM', length: 256 },
		true,
		['encrypt', 'decrypt']
	)

	const iv = crypto.getRandomValues(new Uint8Array(12))
	const encodedText = new TextEncoder().encode(text)

	const ciphertext = await crypto.subtle.encrypt(
		{ name: 'AES-GCM', iv },
		aesKey,
		encodedText
	)
	console.log('Зашифрованный (hex):', bufferToHex(ciphertext))

	const decrypted = await crypto.subtle.decrypt(
		{ name: 'AES-GCM', iv },
		aesKey,
		ciphertext
	)
	console.log('Расшифрованный текст:', new TextDecoder().decode(decrypted))

	const hash = await crypto.subtle.digest(
		'SHA-256',
		new TextEncoder().encode(text)
	)
	console.log('SHA-256 хеш:', bufferToHex(hash))

	console.log(
		'Ключ AES:',
		bufferToHex(await crypto.subtle.exportKey('raw', aesKey))
	)

	// 4. Упаковка и распаковка ключа с AES-KW
	const wrappingKey = await crypto.subtle.generateKey(
		{ name: 'AES-KW', length: 256 },
		true,
		['wrapKey', 'unwrapKey']
	)

	const wrappedKey = await crypto.subtle.wrapKey(
		'raw',
		aesKey,
		wrappingKey,
		'AES-KW'
	)
	console.log('Упакованный AES-ключ:', bufferToHex(wrappedKey))

	const unwrappedKey = await crypto.subtle.unwrapKey(
		'raw',
		wrappedKey,
		wrappingKey,
		'AES-KW',
		{ name: 'AES-GCM', length: 256 },
		true,
		['encrypt', 'decrypt']
	)

	const decryptedUnwrapped = await crypto.subtle.decrypt(
		{ name: 'AES-GCM', iv },
		unwrappedKey,
		ciphertext
	)
	console.log(
		'Расшифровка с распакованным ключом:',
		new TextDecoder().decode(decryptedUnwrapped)
	)

	// 5. Подпись и проверка подлинности (ECDSA)
	const ecKeyPair = await crypto.subtle.generateKey(
		{
			name: 'ECDSA',
			namedCurve: 'P-256',
		},
		true,
		['sign', 'verify']
	)

	const signature = await crypto.subtle.sign(
		{ name: 'ECDSA', hash: { name: 'SHA-256' } },
		ecKeyPair.privateKey,
		encodedText
	)
	console.log('Подпись (hex):', bufferToHex(signature))

	const isValid = await crypto.subtle.verify(
		{ name: 'ECDSA', hash: { name: 'SHA-256' } },
		ecKeyPair.publicKey,
		signature,
		encodedText
	)
	console.log('Подпись действительна?', isValid)
}

main().catch(console.error)

function bufferToHex(buffer) {
	return [...new Uint8Array(buffer)]
		.map(b => b.toString(16).padStart(2, '0'))
		.join('')
}
