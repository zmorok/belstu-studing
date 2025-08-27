import React from 'react'
import JobMenu from './JobMenu'
import ProfessionSelector from './ProfessionSelector'
import './style.css'

class App extends React.Component {
	constructor(props) {
		super(props)
		this.state = {
			selectedProfession: null,
		}
	}

	render() {
		const { selectedProfession } = this.state

		return (
			<div>
				<ProfessionSelector
					onSelect={selectedProfession => this.setState({ selectedProfession })}
				/>
				{selectedProfession && <JobMenu profession={selectedProfession} />}
			</div>
		)
	}
}

export default App
