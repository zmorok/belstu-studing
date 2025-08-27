import React from 'react'

class Clock extends React.Component {
	constructor(props) {
		super(props)
		this.state = {
			time: this.getCurrentTime(),
		}
	}

	componentDidMount() {
		this.timerID = setInterval(() => this.tick(), 1000)
	}

	componentWillUnmount() {
		clearInterval(this.timerID)
	}

	tick() {
		this.setState({
			time: this.getCurrentTime(),
		})
	}

	getCurrentTime() {
		const { format, timezone } = this.props
		const date = new Date()
		let hours = date.getHours()
		let minutes = date.getMinutes()
		let seconds = date.getSeconds()

		if (timezone !== undefined) {
			const [tzH, tzM] = timezone.split(':')
			hours = parseInt(tzH) + date.getUTCHours()

			minutes =
				tzM !== '00' && tzM !== undefined
					? parseInt(tzM) + date.getUTCMinutes()
					: date.getUTCMinutes()
			if (minutes >= 60) {
				minutes -= 60
				hours += 1
			}
			if (hours < 0) {
				hours += 24
			} else if (hours >= 24) {
				hours -= 24
			}
		}
		let ampm = hours <= 12 ? 'AM' : 'PM'
		if (format === '12') {
			hours %= 12
			hours = hours || 12
		}

		hours = hours < 10 ? '0' + hours : hours
		minutes = minutes < 10 ? '0' + minutes : minutes
		seconds = seconds < 10 ? '0' + seconds : seconds

		return format === '12'
			? `${hours}:${minutes}:${seconds} ${ampm}`
			: `${hours}:${minutes}:${seconds}`
	}

	render() {
		return (
			<div>
				<h2>
					Текущее время: <span>{this.state.time}</span>
				</h2>
			</div>
		)
	}
}

export default Clock
