// Ждем, пока весь документ полностью загрузится
document.addEventListener('DOMContentLoaded', function () {
	// Получаем элементы слайдера, кнопок "Назад" и "Вперед"
	const slider = document.querySelector('.slider')
	const prevButton = document.querySelector('.btn-prev')
	const nextButton = document.querySelector('.btn-next')

	// Счетчик для отслеживания текущего слайда
	let counter = 0

	// Получаем все слайды
	const slides = document.querySelectorAll('.slide')

	// Добавляем обработчик события на кнопку "Вперед"
	nextButton.addEventListener('click', function () {
		// Проверяем, не находимся ли мы на последнем слайде
		if (counter < slides.length - 1) {
			// Увеличиваем счетчик, если не последний слайд
			counter++
		} else {
			// Возвращаемся к первому слайду, если это последний слайд
			counter = 0
		}
		// Обновляем слайдер с новым счетчиком
		updateSlider()
	})

	// Добавляем обработчик события на кнопку "Назад"
	prevButton.addEventListener('click', function () {
		// Проверяем, не находимся ли мы на первом слайде
		if (counter > 0) {
			// Уменьшаем счетчик, если не первый слайд
			counter--
		} else {
			// Переходим к последнему слайду, если это первый слайд
			counter = slides.length - 1
		}
		// Обновляем слайдер с новым счетчиком
		updateSlider()
	})

	// Функция для обновления стилей слайдера в зависимости от текущего счетчика
	function updateSlider() {
		// Рассчитываем значение сдвига слайдера
		const translateValue = -counter * 100 + '%'
		// Применяем новое значение сдвига
		slider.style.transform = 'translateX(' + translateValue + ')'
	}

	// Функция для запуска автоматического переключения слайдов
	function startAutoSlide() {
		// Устанавливаем интервал для автоматического переключения слайдов
		intervalId = setInterval(function () {
			// Проверяем, не находимся ли мы на последнем слайде
			if (counter < slides.length - 1) {
				// Увеличиваем счетчик, если не последний слайд
				counter++
			} else {
				// Возвращаемся к первому слайду, если это последний слайд
				counter = 0
			}
			// Обновляем слайдер с новым счетчиком
			updateSlider()
		}, 3000) // Интервал в миллисекундах (в данном случае, 3 секунды)
	}

	// Запускаем автоматическое переключение слайдов
	startAutoSlide()

	nextButton.addEventListener('click', function () {
		clearInterval(intervalId) // Остановка текущего интервала
		startAutoSlide() // Запуск нового интервала
		nextSlide() // Переключение на следующий слайд
	})

	prevButton.addEventListener('click', function () {
		clearInterval(intervalId) // Остановка текущего интервала
		startAutoSlide() // Запуск нового интервала
		prevSlide() // Переключение на предыдущий слайд
	})
})
