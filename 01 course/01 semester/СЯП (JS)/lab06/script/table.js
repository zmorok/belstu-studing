document.getElementById('browserVersion').textContent = navigator.appVersion
document.getElementById('browserName').textContent = navigator.appName
document.getElementById('browserCodeName').textContent = navigator.appCodeName
document.getElementById('os').textContent = navigator.platform
document.getElementById('javaSupport').textContent = navigator.javaEnabled()
	? 'Включена'
	: 'Отключена'
document.getElementById('screenWidth').textContent = screen.width
document.getElementById('screenHeight').textContent = screen.height
document.getElementById('colorDepth').textContent = screen.colorDepth

document.getElementById('visitedURLs').textContent = document.referrer

document.getElementById('currentURL').textContent = location.href

document.getElementById('documentPath').textContent = location.pathname
document.getElementById('domainName').textContent = location.hostname
