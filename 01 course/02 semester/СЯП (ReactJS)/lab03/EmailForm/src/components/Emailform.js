import React from 'react'
import './Emailform.css'

class EmailForm extends React.Component {
	constructor(props) {
		super(props)
		this.state = {
			email: '',
			isValid: true,
			message: 'Введите вашу почту',
		}
	}

	validateEmail = email => {
		const re = /^[^\s@]+@[a-z^\s@]+\.com$/
		return re.test(email)
	}

	handleSubmit = e => {
		e.preventDefault()
		const { email } = this.state
		const isValidEmail = this.validateEmail(email)
		this.setState({ isValid: isValidEmail })

		isValidEmail
			? this.setState({ message: 'Email отправлен успешно!' })
			: this.setState({
					message: 'Введите некорректный адрес электронной почты',
			  })
	}

	handleChange = e => {
		this.setState({ email: e.target.value })
	}

	render() {
		const { email, isValid, message } = this.state

		return (
			<div className='emailform'>
				<form className='form' onSubmit={this.handleSubmit}>
					<label>
						Email:
						<input type='email' value={email} onChange={this.handleChange} />
					</label>
					<button type='submit'>Отправить</button>
				</form>
				{isValid ? (
					<p className='message' style={{ color: 'green' }}>
						{message}
					</p>
				) : (
					<p className='message' style={{ color: 'red' }}>
						{message}
					</p>
				)}
			</div>
		)
	}
}

export default EmailForm
