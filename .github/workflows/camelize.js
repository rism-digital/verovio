var test = 'It-is-a-great-day-today';

function dashCaseToCamel(str) {
    return str[0].toLowerCase() + str.replace(/-([a-z])/g, function(a, b) {
        return b.toUpperCase();
    }).slice(1);
}

function camelCaseToDash (str) {
    str = str.replace(/([a-zA-Z])(?=[A-Z])/g, '$1-').toLowerCase();
    str = '--' + str;
    return str;
}

var testCamel = dashCaseToCamel(test);
document.getElementById("camel").innerHTML = testCamel;

var testDash = camelCaseToDash(testCamel);
document.getElementById("dash").innerHTML = testDash;
