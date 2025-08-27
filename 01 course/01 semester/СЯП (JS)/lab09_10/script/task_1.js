let offsetX,
	offsetY,
	isDragging = false

const draggableElement = document.getElementById('draggable')

draggableElement.addEventListener('dragstart', e => {
	offsetX = e.clientX - draggableElement.getBoundingClientRect().left
	offsetY = e.clientY - draggableElement.getBoundingClientRect().top
	isDragging = true
})

document.addEventListener('mousemove', e => {
	if (isDragging) {
		const x = e.clientX - offsetX
		const y = e.clientY - offsetY

		draggableElement.style.left = `${x}px`
		draggableElement.style.top = `${y}px`
	}
})

document.addEventListener('mouseup', () => {
	isDragging = false
})

// $(document).ready(() => {
// 	let offsetX,
// 		offsetY,
// 		isDragging = false

// 	$('#draggable').on('dragstart', e => {
// 		offsetX = e.clientX - $('#draggable').offset().left
// 		offsetY = e.clientY - $('#draggable').offset().top
// 		isDragging = true
// 	})

// 	$(document).on('mousemove', e => {
// 		if (isDragging) {
// 			const x = e.clientX - offsetX
// 			const y = e.clientY - offsetY
// 			$('#draggable').css({ left: `${x}px`, top: `${y}px` })
// 		}
// 	})

// 	$(document).on('mouseup', () => {
// 		isDragging = false
// 	})
// })
