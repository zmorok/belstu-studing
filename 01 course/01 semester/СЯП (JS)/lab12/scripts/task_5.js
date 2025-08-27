let clicker = function () {
	let counter = 0

	return function () {
		counter++
		document.getElementById('task_5').innerHTML = `Кликов: ${counter}`
	}
}

document.getElementById('task_5').onclick = clicker()
