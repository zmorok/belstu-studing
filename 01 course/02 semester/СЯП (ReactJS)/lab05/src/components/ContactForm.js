import React from 'react'

class ContactForm extends React.Component {
	constructor(props) {
		super(props)
		this.state = {
			name: '',
			email: '',
			message: '',
			gender: '',
			error: '',
			submittedData: [],
		}
	}

	handleSubmit = e => {
		e.preventDefault()
		const { name, email, message, gender } = this.state
		// Валидация полей
		if (!name || !email || !message) {
			this.setState({ error: 'Пожалуйста, заполните все поля.' })
			return
		}
		// Добавление данных в массив отправленных данных + очистка данных
		const newData = { name, email, message, gender }
		this.setState(prevState => ({
			submittedData: [...prevState.submittedData, newData],
			name: '',
			email: '',
			message: '',
			gender: '',
			error: '',
		}))
	}

	handleChange = e => {
		const { name, value } = e.target
		this.setState({ [name]: value })
	}

	handleGenderChange = e => {
		this.setState({ gender: e.target.value })
	}

	render() {
		const { name, email, message, gender, error, submittedData } = this.state
		return (
			<div>
				<div className='form'>
					<form onSubmit={this.handleSubmit}>
						{error && <div style={{ color: 'red' }}>{error}</div>}
						<div>
							<label>Name:</label>
							<input
								type='text'
								name='name'
								value={name}
								onChange={this.handleChange}
							/>
						</div>
						<div>
							<label>Email:</label>
							<input
								type='email'
								name='email'
								value={email}
								onChange={this.handleChange}
							/>
						</div>
						<div>
							<label className='msg'>Message:</label>
							<textarea
								name='message'
								value={message}
								onChange={this.handleChange}
							/>
						</div>
						<div className='gender'>
							<label>Gender:</label>
							<label>
								<input
									type='radio'
									name='gender'
									value='male'
									checked={gender === 'male'}
									onChange={this.handleGenderChange}
								/>
								Male
							</label>
							<label>
								<input
									type='radio'
									name='gender'
									value='female'
									checked={gender === 'female'}
									onChange={this.handleGenderChange}
								/>
								Female
							</label>
						</div>
						<button type='submit'>Submit</button>
					</form>
				</div>
				<hr />
				<table>
					<thead>
						<tr>
							<th>Name</th>
							<th>Email</th>
							<th>Message</th>
							<th>Gender</th>
						</tr>
					</thead>
					<tbody>
						{submittedData.map((data, index) => (
							<tr key={index}>
								<td>{data.name}</td>
								<td>{data.email}</td>
								<td>{data.message}</td>
								<td>{data.gender}</td>
							</tr>
						))}
					</tbody>
				</table>
			</div>
		)
	}
}

export default ContactForm
