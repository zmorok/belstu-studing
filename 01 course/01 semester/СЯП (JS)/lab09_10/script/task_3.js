const image = document.getElementById('movingImg')
image.style.position = 'relative'
image.style.marginTop = '18%'

let x = 0
let y = 0
const step = 5
let moveInterval = 0

let move = function () {
	x += step
	y = Math.sin(x / 85) * 300

	image.style.left = x + 'px'
	image.style.top = y + 'px'

	if (x > 1300) {
		clearInterval(moveInterval)
		x = 0
		y = 0
		image.style.left = x + 'px'
		image.style.top = y + 'px'

		moveInterval = setInterval(move, 20)
	}
}

image.addEventListener('click', function () {
	if (moveInterval) {
		clearInterval(moveInterval)
		moveInterval = null
	} else {
		moveInterval = setInterval(move, 20)
	}
})

// $(document).ready(() => {
// 	$('#movingImg').css({
// 		position: 'relative',
// 		marginTop: '18%',
// 	})

// 	let x = 0,
// 		y = 0,
// 		moveInterval = 0
// 	const step = 5

// 	let move = function () {
// 		x += step
// 		y = Math.sin(x / 85) * 300

// 		$('#movingImg').css({
// 			left: x + 'px',
// 			top: y + 'px',
// 		})

// 		if (x > 1300) {
// 			clearInterval(moveInterval)
// 			x = 0
// 			y = 0
// 			$('#movingImg').css({
// 				left: x + 'px',
// 				top: y + 'px',
// 			})

// 			moveInterval = setInterval(move, 20)
// 		}
// 	}

// 	$('#movingImg').on('click', () => {
// 		if (moveInterval) {
// 			clearInterval(moveInterval)
// 			moveInterval = null
// 		} else {
// 			moveInterval = setInterval(move, 20)
// 		}
// 	})
// })
