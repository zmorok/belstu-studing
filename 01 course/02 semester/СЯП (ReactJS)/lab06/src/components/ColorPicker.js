import React, { useState, useEffect } from 'react'

const ColorPicker = () => {
	const [selectedColor, setSelectedColor] = useState()
	const [savedColors, setSavedColors] = useState([])

	useEffect(() => {
		// есть ли сохраненные цвета в локальном хранилище при загрузке страницы
		const savedColorsFromLocalStorage = JSON.parse(
			localStorage.getItem('savedColors')
		)
		if (savedColorsFromLocalStorage) {
			setSavedColors(savedColorsFromLocalStorage)
		}
	}, [])

	useEffect(() => {
		// сохранение список сохраненных цветов в локальном хранилище при изменении
		localStorage.setItem('savedColors', JSON.stringify(savedColors))
	}, [savedColors])

	const handleColorChange = color => {
		setSelectedColor(color)
	}

	const handleSaveColor = () => {
		if (selectedColor) {
			setSavedColors([...savedColors, selectedColor])
		}
	}

	const handleResetColor = () => {
		setSelectedColor(null)
	}

	const handleClearSavedColors = () => {
		setSavedColors([])
	}

	const colors = [
		'#CD5C5C',
		'#F08080',
		'#FA8072',
		'#E9967A',
		'#FFA07A',
		'#DC143C',
		'#FF0000',
		'#B22222',
		'#8B0000',
		'#FFC0CB',
		'#FFB6C1',
		'#FF69B4',
		'#FF1493',
		'#C71585',
		'#DB7093',
		'#FFA07A',
		'#FF7F50',
		'#FF6347',
		'#FF4500',
		'#FF8C00',
		'#FFA500',
		'#ADFF2F',
		'#7FFF00',
		'#00FF00',
		'#32CD32',
		'#98FB98',
		'#90EE90',
		'#00FA9A',
		'#00FF7F',
		'#3CB371',
		'#2E8B57',
		'#228B22',
		'#008000',
		'#006400',
		'#9ACD32',
		'#6B8E23',
		'#808000',
		'#556B2F',
		'#66CDAA',
		'#8FBC8F',
		'#20B2AA',
		'#008B8B',
		'#008080',
		'#00FFFF',
		'#5F9EA0',
		'#4682B4',
		'#7B68EE',
		'#4169E1',
		'#00008B',
		'#FFFFFF',
		'#000000',
	]

	return (
		<div className='color-picker'>
			<h2>Color Picker</h2>
			<div className='selected-color'>
				<h3>Selected Color: {selectedColor}</h3>
				<div
					className='color-display'
					style={{ backgroundColor: selectedColor }}
				></div>
				<button onClick={handleResetColor}>Reset</button>
				<button onClick={handleSaveColor}>Save</button>
			</div>
			<div className='saved-colors'>
				<h3>Saved Colors:</h3>
				<div className='color-list'>
					{savedColors.map((color, index) => (
						<div
							key={index}
							className='color-item'
							style={{ backgroundColor: color }}
							onClick={() => handleColorChange(color)}
						></div>
					))}
				</div>
				<button onClick={handleClearSavedColors}>Clear Saved Colors</button>
			</div>
			<div className='color-palette'>
				<h3>Color Palette:</h3>
				<div className='color-list'>
					{colors.map((color, index) => (
						<div
							key={index}
							className='color-item'
							style={{ backgroundColor: color, cursor: 'pointer' }}
							onClick={() => handleColorChange(color)}
						></div>
					))}
				</div>
			</div>
		</div>
	)
}

export default ColorPicker
