import React from 'react'

class Todolist extends React.Component {
	constructor(props) {
		super(props)
		this.state = {
			noteInput: '',
			notes: [],
		}
	}

	addNote = () => {
		const { noteInput } = this.state
		if (noteInput.trim() !== '') {
			this.setState(prevState => ({
				notes: [...prevState.notes, noteInput],
				noteInput: '',
			}))
		}
	}

	deleteNote = index => {
		this.setState(prevState => ({
			notes: prevState.notes.filter((_, i) => i !== index),
		}))
	}

	editNote = (index, editedNote) => {
		if (editedNote.trim() === '') {
			if (window.confirm('Вы убрали текст заметки, хотите удалить заметку?')) {
				this.deleteNote(index)
			}
		} else {
			this.setState(prevState => ({
				notes: prevState.notes.map((note, i) =>
					i === index ? editedNote : note
				),
			}))
		}
	}

	render() {
		const { noteInput, notes } = this.state

		return (
			<div>
				<div className='header'>
					<input
						id='noteInput'
						type='text'
						value={noteInput}
						onChange={e => this.setState({ noteInput: e.target.value })}
					/>
					<button id='addNoteBtn' onClick={this.addNote}>
						Добавить заметку
					</button>
				</div>
				<ol id='notesList'>
					{notes.map((note, index) => (
						<li className='note'>
							<p>{note}</p>
							<span
								className='editNoteBtn'
								onClick={() => {
									const editedNote = prompt('Редактировать заметку:', note)
									if (editedNote !== null) {
										this.editNote(index, editedNote)
									}
								}}
							>
								Редактировать
							</span>
							<span
								className='deleteNoteBtn'
								onClick={() => this.deleteNote(index)}
							>
								Удалить
							</span>
						</li>
					))}
				</ol>
			</div>
		)
	}
}

export default Todolist
