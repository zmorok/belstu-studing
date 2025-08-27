// Получаем все изображения
let images = document.querySelectorAll('.music img')

// Функция для изменения размера изображения
function photoScale(e) {
	// Проходим по каждому изображению
	// Получаем текущий масштаб
	let currentTransform = window.getComputedStyle(e.target).transform

	// Увеличиваем или уменьшаем масштаб (можете настроить значение изменения)
	let newTransform =
		currentTransform === 'none' ? 'scale(2.6) translateX(-15px)' : 'none'

	// Применяем плавное изменение размера
	e.target.style.transition = 'transform 0.5s' // Измените время по желанию
	e.target.style.transform = newTransform

	// Убираем анимацию после завершения
	setTimeout(function () {
		images.forEach(function (img) {
			img.style.transition = 'none'
		})
	}, 500) // Укажите тот же период, что и в transition
}

// Привязываем функцию к событию onclick
images.forEach(function (img) {
	img.addEventListener('click', photoScale)
})
