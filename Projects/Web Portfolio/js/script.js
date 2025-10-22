// Блок со скиллами: .skill-list
const skills = {
    data: [],
    isSorted: false,
    isInErrorState: false,
    jsonPath: '',
    skillList: null,
    sectionSkills: null,
    initList: function(jsonPath, skillList, sectionSkills) {
        this.jsonPath = jsonPath;
        this.skillList = skillList;
        this.sectionSkills = sectionSkills;
        fetch(jsonPath)
            .then(data => data.json())
            .then(object => {
                if (this.isInErrorState) {
                    this.isInErrorState = false;
                    this.renderErrorToggle(skillList, sectionSkills);
                }
                this.data = object;
                this.generateList(skillList);
            })
            .catch(() => {
                // Фейковые данные, для отображения при ошибке
                skills.data =  [
                    {"item": "Sample","level": 90, "iconPath": "img/skills/vue.svg"},
                    {"item": "Sample","level": 80, "iconPath": "img/skills/php.svg"},
                    {"item": "Sample","level": 70, "iconPath": "img/skills/nodejs.svg"},
                    {"item": "Sample","level": 60, "iconPath": "img/skills/1c.svg"},
                    {"item": "Sample","level": 50, "iconPath": "img/skills/python.svg"}]
                this.generateList(skillList);
                this.isInErrorState = true;
                this.renderErrorToggle(skillList, sectionSkills);
            });
    },

    renderErrorToggle: function(skillList, sectionSkills) {
        // Сообщение с ошибкой
        errorMsg = document.querySelector('#createdErrorMessage');
        // Если нет ошибки получения данных:
        if (!this.isInErrorState) {
            // Удаляем прошлое сообщение об ошибке
            if (errorMsg !== null) {
                errorMsg.remove();
            }
            // Старое сообщение уже удалено, достаточно просто убрать блюр
            skillList.style.webkitFilter = '';
            // Восстанавливаем кнопки
            this.btns.forEach((btn) => {
                btn.disabled = false;
            })
            // На этом этапе страница в нормальном состоянии
            return;
        }

        // Создаем сообщение об ошибке, если его нет
        if (errorMsg === null) {
            skillsWrapper = document.querySelector('.skills-wrapper');
            this.btns = sortBtnsBlock.querySelectorAll('button')
            this.btns.forEach((btn) => {
                btn.disabled = true;
            })
            // Задний фон для "скиллов"
            skillList.style.webkitFilter = 'blur(10px)';
            skillList.style.zIndex = 0;

            errorMsg = document.createElement('div');
            errorMsg.textContent = 'При загрузке данных произошла ошибка.'
            errorMsg.classList.add('error-message');


            // Кнопка в сообщении
            errorBtn = document.createElement('button');
            errorBtn.textContent = 'Попробовать еще раз';
            errorMsg.appendChild(errorBtn);
            errorMsg.id = 'createdErrorMessage';
            // Для кнопки добавить ивент для повторной попытки
            errorBtn.addEventListener('click', () => {
                // Разумеется, при текущих условиях мы не выйдем из ошибки (другого пути json файла не будет)
                this.initList(this.jsonPath, this.skillList, this.sectionSkills);
                // Можно проверить по валидному файлу :)
                //this.initList('db/skills.json', this.skillList, this.sectionSkills);
            })
            
            // Вставить сообщение об ошибке в центр "скиллов"
            skillsWrapper.append(errorMsg);
        }
    },
    generateList: function(parentElement) {
        parentElement.innerHTML = '';
        this.data.forEach(element => {
            const dt = document.createElement('dt');
            dt.classList.add('skill-item');
            dt.textContent = element.item;
            dt.style.backgroundImage = `url(${element.iconPath})`;

            const dd = document.createElement('dd');
            dd.classList.add('skill-level');
            
            const div = document.createElement('div');
            div.style.width = `${element.level}%`;
            div.textContent = `${element.level}%`;

            dd.append(div);
            parentElement.append(dt, dd);
        });
    },
    sortList: function(sortingType) {
        if (skills.isSorted !== sortingType) {
            switch (sortingType) {
                case 'name':
                    this.data.sort((a, b) => a.item.localeCompare(b.item)); break;
                case 'level':
                    this.data.sort((a, b) => b.level - a.level); break;
                default:
                    return;
            }
            this.isSorted = sortingType;
        } else {
            this.data.reverse();
        }
        this.generateList(skillList);
    },
};


// Меню навигации: .main-header
const menu = {
    open: function() {
        nav.classList.remove('main-nav_closed');
        btn.classList.add('nav-btn_close');
        btn.classList.remove('nav-btn_open');
        btn.innerHTML = '<span class="visually-hidden">Закрыть меню</span>';
    },
    close: function() {
        nav.classList.add('main-nav_closed');
        btn.classList.remove('nav-btn_close');
        btn.classList.add('nav-btn_open');
        btn.innerHTML = '<span class="visually-hidden">Открыть меню</span>';
    },
};

// Первое создание списка навыков
const skillList = document.querySelector('dl.skill-list');
const sectionSkills = document.querySelector('section.skills');
skills.initList('db/skills.json', skillList, sectionSkills);

// Сортировки списка по кнопкам
const sortBtnsBlock = document.querySelector('.skills-buttons div');
sortBtnsBlock.addEventListener('click', (e) => {
    if (e.target.nodeName === "BUTTON") {
        skills.sortList(e.target.dataset.type);
    }
});

// Показ и скрытие меню (гамбургер)
const nav = document.querySelector('.main-nav');
const btn = document.querySelector('.nav-btn');
btn.addEventListener('click', (e) => {
    e.target.classList.contains('nav-btn_open') ? menu.open() : menu.close();
});
menu.close();

// Чекбокс темной темы
const changeToDarkTheme = (theme) => {
    theme
      ? document.body.classList.add('dark-theme')
      : document.body.classList.remove('dark-theme');
    localStorage.setItem('dark-theme-enabled', theme);
}
const checkbox = document.querySelector(".switch-checkbox");
checkbox.addEventListener("change", (e) => {
    changeToDarkTheme(!checkbox.checked);
});
// Загрузка сохраненной темы
const darkThemeOnLoad = localStorage.getItem('dark-theme-enabled');
if (darkThemeOnLoad === null) {
    changeToDarkTheme(true);
    checkbox.checked = false;
} else {
    changeToDarkTheme(darkThemeOnLoad === "true");
    checkbox.checked = (darkThemeOnLoad !== "true");
}