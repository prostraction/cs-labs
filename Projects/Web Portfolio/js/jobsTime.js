let parseYear = (month) => {
    let year = (month/12|0)
    switch (year) {
        case 0:
            return ""
        case 1:
            return year + " year"
        case 2:
        case 3: 
        case 4: 
            return year + " years"
        default:
            return year + " years"
    }
}
let parseMonth = (month) => {
    let monthDived = month - 12*(month/12|0)
    switch (monthDived) {
        case 0:
            return ""
        case 1: 
            return " " + monthDived + " month"
        case 2:
        case 3:
        case 4:
            return " " + monthDived + " months"
        default:
            return " " + monthDived + " months"
    }
}
let jobs = []
let jobsCount = 2
jobs.push({begin: new Date(2000, 1, 1, 0, 0, 0, 0), end: new Date(2025, 1, 1, 0, 0, 0, 0)})
jobs.push({begin: new Date(2025, 1, 1, 0, 0, 0, 0), end: new Date()})
let intervalMonth = 0
for (let i = 0; i < jobsCount; i++) {
    let intervalYear = jobs[i].end.getFullYear() - jobs[i].begin.getFullYear()
    intervalMonth += (12*intervalYear) + jobs[i].end.getMonth() - jobs[i].begin.getMonth() + 1
}
document.write(parseYear(intervalMonth) + parseMonth(intervalMonth))