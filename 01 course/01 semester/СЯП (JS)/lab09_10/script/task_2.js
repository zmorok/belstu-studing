document.getElementById('movingImg').style.position = 'relative'

let move = function (e) {
	let start = Date.now()

	let timer = setInterval(function () {
		let timePassed = Date.now() - start
		e.target.style.left = timePassed / 5 + 'px'
		if (timePassed > 3000) {
			clearInterval(timer)
			e.target.style.left = 0 + 'px'
		}
	})
}

document.getElementById('movingImg').addEventListener('click', move)

// $(document).ready(() => {
// 	$('#movingImg').css({ position: 'relative' })

// 	$('#movingImg').on('click', e => {
// 		let start = Date.now()
// 		let timer = setInterval(function () {
// 			let timePassed = Date.now() - start
// 			$(e.target).css('left', timePassed / 5 + 'px')
// 			if (timePassed > 3000) {
// 				clearInterval(timer)
// 				$(e.target).css('left', 0 + 'px')
// 			}
// 		}, 1)
// 	})
// })
