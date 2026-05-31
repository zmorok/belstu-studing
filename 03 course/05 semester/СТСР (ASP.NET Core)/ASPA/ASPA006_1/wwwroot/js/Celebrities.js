document.addEventListener('DOMContentLoaded', async () => {
    const photosContainer = document.getElementById('photos-container');
    const eventsContainer = document.getElementById('events-container');

    async function loadCelebrities() {
        try {
            const resp = await fetch('/api/Celebrities/');
            if (!resp.ok) throw new Error('Ошибка загрузки списка знаменитостей');

            const celebrities = await resp.json();

            celebrities.forEach(c => {
                const id = c.id ?? c.Id;
                const fullName = c.fullName || c.FullName || '';
                const photoPath = c.reqPhotoPath || c.ReqPhotoPath;
                if (!photoPath) return;

                const fileName = photoPath.split('\\').pop().split('/').pop();

                const wrapper = document.createElement('div');
                wrapper.style.display = 'inline-block';
                wrapper.style.margin = '10px';
                wrapper.style.textAlign = 'center';
                wrapper.style.cursor = 'pointer';

                const img = document.createElement('img');
                img.src = `/api/Celebrities/Photo/${fileName}`;
                img.alt = fullName;
                img.style.maxWidth = '200px';
                img.style.display = 'block';
                img.style.marginBottom = '4px';

                const caption = document.createElement('div');
                caption.textContent = fullName;

                wrapper.appendChild(img);
                wrapper.appendChild(caption);
                photosContainer.appendChild(wrapper);

                wrapper.addEventListener('click', () => {
                    loadEventsForCelebrity(id, fullName);
                });
            });

            if (!photosContainer.hasChildNodes()) {
                photosContainer.textContent = 'Нет фотографий';
            }
        } catch (err) {
            console.error(err);
            photosContainer.textContent = 'Ошибка при загрузке фотографий';
        }
    }

    async function loadEventsForCelebrity(celebrityId, fullName) {
        eventsContainer.innerHTML = '';

        const title = document.createElement('h2');
        title.textContent = `События: ${fullName}`;
        eventsContainer.appendChild(title);

        try {
            const resp = await fetch(`/api/Lifeevents/Celebrities/${celebrityId}`);
            if (!resp.ok) throw new Error('Ошибка загрузки событий');

            const events = await resp.json();

            if (!events.length) {
                const empty = document.createElement('div');
                empty.textContent = 'Нет событий для этой знаменитости';
                eventsContainer.appendChild(empty);
                return;
            }

            const list = document.createElement('ul');

            events.forEach(e => {
                const date = e.date || e.Date;
                const desc = e.description || e.Description;

                const li = document.createElement('li');
                li.textContent = (date ? `${date}: ` : '') + (desc ?? '');
                list.appendChild(li);
            });

            eventsContainer.appendChild(list);
        } catch (err) {
            console.error(err);
            const errDiv = document.createElement('div');
            errDiv.textContent = 'Ошибка при загрузке событий';
            eventsContainer.appendChild(errDiv);
        }
    }
    loadCelebrities();
});
