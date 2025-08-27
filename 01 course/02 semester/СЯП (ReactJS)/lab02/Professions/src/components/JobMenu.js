import React from 'react'

class JobMenu extends React.Component {
	render() {
		const { profession } = this.props

		const menu = {
			programmer: [
				'GitHub',
				'Stack Overflow',
				'MDN Web Docs',
				'Codecademy',
				'FreeCodeCamp',
				'LeetCode',
				'HackerRank',
			],
			designer: [
				'Behance',
				'Dribbble',
				'Adobe Creative Cloud',
				'Sketch',
				'Figma',
				'InVision',
				'Canva',
			],
			marketer: [
				'HubSpot',
				'Moz',
				'Google Analytics',
				'Buffer',
				'Mailchimp',
				'Hootsuite',
				'Sprout Social',
			],
			engineer: [
				'IEEE Xplore',
				'ResearchGate',
				'Google Scholar',
				'Engineering Village',
				'SpringerLink',
				'arXiv',
				'ScienceDirect',
			],
			teacher: [
				'Edutopia',
				'Scholastic',
				'TeacherTube',
				'TeachThought',
				'PBS LearningMedia',
				'Khan Academy',
				'EdX',
			],
			reaper: ['репер'],
		}

		return (
			<div>
				<h2>{profession}</h2>
				<ul>
					{menu[profession].map(item => (
						<li>{item}</li>
					))}
				</ul>
			</div>
		)
	}
}

export default JobMenu
