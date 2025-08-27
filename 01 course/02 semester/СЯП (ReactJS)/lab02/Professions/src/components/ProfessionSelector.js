import React from 'react'

class ProfessionSelector extends React.Component {
	handleSelect = e => {
		const { onSelect } = this.props
		this.setState = onSelect(e.target.value)
	}

	render() {
		return (
			<div>
				<h2>Выберите профессию:</h2>
				<select onChange={this.handleSelect}>
					<option value=''>Выберите профессию</option>
					<option value='programmer'>Программист</option>
					<option value='designer'>Дизайнер</option>
					<option value='marketer'>Маркетолог</option>
					<option value='engineer'>Инженер</option>
					<option value='teacher'>Учитель</option>
					<option value='reaper'>Музикантъ</option>
				</select>
			</div>
		)
	}
}

export default ProfessionSelector
