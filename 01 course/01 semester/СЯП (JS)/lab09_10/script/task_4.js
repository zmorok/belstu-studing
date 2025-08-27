let drawGraph = function (func) {
	// Получаем контекст canvas
	let canvas = document.getElementById('сanvas')
	let ctx = canvas.getContext('2d')

	let selectedColor = document.getElementById('color').value

	// Очищаем canvas перед каждым рисованием
	ctx.clearRect(0, 0, canvas.width, canvas.height)

	// Устанавливаем цвет линии графика
	ctx.strokeStyle = '#333333'

	// Настройки осей координат
	let xAxis = canvas.width / 2
	let yAxis = canvas.height / 2

	// Масштаб по осям
	let scaleX = 20
	let scaleY = 20

	// Рисуем оси координат
	ctx.beginPath()
	ctx.moveTo(0, yAxis)
	ctx.lineTo(canvas.width, yAxis)
	ctx.moveTo(xAxis, 0)
	ctx.lineTo(xAxis, canvas.height)
	ctx.stroke()

	// Рисуем график функции
	ctx.beginPath()
	ctx.strokeStyle = selectedColor
	for (let x = -500; x <= 500; x += 0.1) {
		let y
		if (func === 'x^2') {
			y = x * x
		} else if (func === 'x^3') {
			y = x * x * x
		} else if (func === 'sin(x)') {
			y = Math.sin(x)
		} else if (func === 'cos(x)') {
			y = Math.cos(x)
		} else if (func === 'tan(x/2)') {
			y = Math.tan(x / 2)
		}

		// Переводим координаты в систему canvas
		let canvasX = xAxis + x * scaleX
		let canvasY = yAxis - y * scaleY

		// Рисуем линию к следующей точке
		ctx.lineTo(canvasX, canvasY)
	}
	ctx.stroke()
}

document.getElementById('func1').addEventListener('click', function () {
	drawGraph('x^2')
})
document.getElementById('func2').addEventListener('click', function () {
	drawGraph('x^3')
})
document.getElementById('func3').addEventListener('click', function () {
	drawGraph('sin(x)')
})
document.getElementById('func4').addEventListener('click', function () {
	drawGraph('cos(x)')
})
document.getElementById('func5').addEventListener('click', function () {
	drawGraph('tan(x/2)')
})

// $(document).ready(() => {
// 	let drawGraph = function (func) {
// 		// Получаем контекст canvas
// 		let canvas = document.getElementById('сanvas')
// 		let ctx = canvas.getContext('2d')

// 		let selectedColor = $('#color').val()

// 		// Очищаем canvas перед каждым рисованием
// 		ctx.clearRect(0, 0, canvas.width, canvas.height)

// 		// Устанавливаем цвет линии графика
// 		ctx.strokeStyle = '#333333'

// 		// Настройки осей координат
// 		let xAxis = canvas.width / 2
// 		let yAxis = canvas.height / 2

// 		// Масштаб по осям
// 		let scaleX = 20
// 		let scaleY = 20

// 		// Рисуем оси координат
// 		ctx.beginPath()
// 		ctx.moveTo(0, yAxis)
// 		ctx.lineTo(canvas.width, yAxis)
// 		ctx.moveTo(xAxis, 0)
// 		ctx.lineTo(xAxis, canvas.height)
// 		ctx.stroke()

// 		// Рисуем график функции
// 		ctx.beginPath()
// 		ctx.strokeStyle = selectedColor
// 		for (let x = -500; x <= 500; x += 0.1) {
// 			let y
// 			if (func === 'x^2') {
// 				y = x * x
// 			} else if (func === 'x^3') {
// 				y = x * x * x
// 			} else if (func === 'sin(x)') {
// 				y = Math.sin(x)
// 			} else if (func === 'cos(x)') {
// 				y = Math.cos(x)
// 			} else if (func === 'tan(x/2)') {
// 				y = Math.tan(x / 2)
// 			}

// 			// Переводим координаты в систему canvas
// 			let canvasX = xAxis + x * scaleX
// 			let canvasY = yAxis - y * scaleY

// 			// Рисуем линию к следующей точке
// 			ctx.lineTo(canvasX, canvasY)
// 		}
// 		ctx.stroke()
// 	}

// 	$('#func1').click(() => {
// 		drawGraph('x^2')
// 	})

// 	$('#func2').click(() => {
// 		drawGraph('x^3')
// 	})

// 	$('#func3').click(() => {
// 		drawGraph('sin(x)')
// 	})

// 	$('#func4').click(() => {
// 		drawGraph('cos(x)')
// 	})

// 	$('#func5').click(() => {
// 		drawGraph('tan(x/2)')
// 	})
// })
