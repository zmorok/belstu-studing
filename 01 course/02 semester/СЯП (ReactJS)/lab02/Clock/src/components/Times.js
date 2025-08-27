import React from 'react'
import Clock from './Clock'

class Times extends React.Component {
	render() {
		return (
			<div className='content'>
				<h1>Различные часы:</h1>
				<div className='times'>
					<div className='time minsk'>
						<h1>Время в Минске</h1>
						<Clock />
						<Clock format='12' timezone='+3' />
					</div>
					<div className='time new-york'>
						<h1>Время в Нью-Йорке</h1>
						<Clock timezone='-5' />
						<Clock format='12' timezone='-5' />
					</div>
					<div className='time pekin'>
						<h1>Время в Пекине</h1>
						<Clock timezone='+8' />
						<Clock format='12' timezone='+8' />
					</div>
					<div className='time new-deli'>
						<h1>Время в Нью-Дели</h1>
						<Clock timezone='+5:30' />
						<Clock format='12' timezone='+5:30' />
					</div>
				</div>
			</div>
		)
	}
}

export default Times
