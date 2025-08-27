let helloUser = function (name) {
	if (!name) {
		name = 'Аноним'
	}
	alert(`Привет, ${name}`)
}

$(document).ready(() => {
	$('#task_4').on('click', function () {
		let userName = prompt('Здравствуйте, введите ваше имя!')
		helloUser(userName)
	})
})
