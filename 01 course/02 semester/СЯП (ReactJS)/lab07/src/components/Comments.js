import React, { useEffect, useState } from 'react'

const Comments = () => {
	const [comments, setComments] = useState([])
	const [newComment, setNewComment] = useState({
		username: '',
		email: '',
		message: '',
		secretWord: '',
	})

	// генератор 'секретного кода'
	const generateSecretCode = username => {
		return username.toLowerCase().replace(/\s/g, '') + 'scrcode'
	}

	// обработчик input.value для занесения в элементы состояния
	const handleInputChange = e => {
		const { name, value } = e.target
		setNewComment({ ...newComment, [name]: value })
	}

	// обработчик добавления комментария
	const handleAddComment = () => {
		const currentDate = new Date()
		const formattedDate = `${
			currentDate.getDate() > 9
				? currentDate.getDate()
				: '0' + currentDate.getDate()
		}–${
			currentDate.getMonth() + 1 > 9
				? currentDate.getMonth() + 1
				: '0' + (currentDate.getMonth() + 1)
		}–${currentDate.getFullYear()} ${
			currentDate.getHours() > 9
				? currentDate.getHours()
				: '0' + currentDate.getHours()
		}:${
			currentDate.getMinutes() > 9
				? currentDate.getMinutes()
				: '0' + currentDate.getMinutes()
		}:${
			currentDate.getSeconds() > 9
				? currentDate.getSeconds()
				: '0' + currentDate.getSeconds()
		} `

		// скрипт, выявляющий пустые окна ввода
		const inputs = document.querySelectorAll('input')
		const emptyFields = []
		const spaces = '\n           '
		inputs.forEach(input => {
			if (input.value.trim() === '') {
				emptyFields.push(input.placeholder)
			}
		})
		if (emptyFields.length > 0) {
			alert(
				`Пожалуйста, заполните следующие поля:${spaces}${emptyFields.join(
					spaces
				)}`
			)
			return
		}

		newComment.secretWord = generateSecretCode(newComment.username)
		const updatedComments = [
			...comments,
			{ ...newComment, date: formattedDate },
		]

		setComments(updatedComments)
		setNewComment({ username: '', email: '', message: '', secretWord: '' })
	}

	// обработчик изменения комментария
	const handleEditComment = (index, updatedComment) => {
		const updatedComments = [...comments]
		if (updatedComment.message === '') {
			alert('Нельзя изменить на пустой комментарий')
		} else {
			updatedComments[index] = updatedComment
			setComments(updatedComments)
		}
	}

	// обработчик удаления комментария
	const handleDeleteComment = (index, secretWord) => {
		const confirmation = prompt(
			'Введите секретное слово для удаления комментария:'
		)
		if (confirmation === secretWord) {
			const updatedComments = [...comments]
			updatedComments.splice(index, 1)
			setComments(updatedComments)
		} else {
			alert('Неверное секретное слово. Комментарий не удален.')
		}
	}

	// обработчик просмотра информации о комментарии
	const handleViewCommentInfo = comment => {
		alert(
			`Имя пользователя: ${comment.username}\nДата изменения: ${comment.date}\nЭлектронная почта: ${comment.email}`
		)
	}

	// два useEffect() для хранения комментариев в локальном хранилище браузера
	useEffect(() => {
		const savedCommentsFromLocalStorage = JSON.parse(
			localStorage.getItem('savedComments')
		)
		if (savedCommentsFromLocalStorage) {
			setComments(savedCommentsFromLocalStorage)
		}
	}, [])
	useEffect(() => {
		localStorage.setItem('savedComments', JSON.stringify(comments))
	}, [comments])

	return (
		<div className='comments-container'>
			<h3>Добавить комментарий</h3>
			<div className='add-comment'>
				<input
					type='text'
					name='username'
					placeholder='Имя пользователя'
					value={newComment.username}
					onChange={handleInputChange}
				/>
				<input
					type='email'
					name='email'
					placeholder='Электронная почта'
					value={newComment.email}
					onChange={handleInputChange}
				/>
				<input
					name='message'
					placeholder='Текст сообщения'
					value={newComment.message}
					onChange={handleInputChange}
				/>
				<button onClick={handleAddComment}>Добавить</button>
			</div>
			<h2>Комментарии</h2>
			{comments.map((comment, index) => (
				<div className='comment' key={index}>
					<div className='comment-header'>
						<strong>{comment.username}</strong> | {comment.date}
						<button onClick={() => handleViewCommentInfo(comment)}>
							Информация
						</button>
					</div>
					<div className='comment-body'>{comment.message}</div>
					<div className='comment-buttons'>
						<button
							onClick={() =>
								handleEditComment(index, {
									...comment,
									message: prompt('Введите новый текст комментария:'),
								})
							}
						>
							Редактировать
						</button>
						<button
							onClick={() => handleDeleteComment(index, comment.secretWord)}
						>
							Удалить
						</button>
					</div>
				</div>
			))}
		</div>
	)
}

export default Comments
