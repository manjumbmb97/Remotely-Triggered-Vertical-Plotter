//Define a variable to get the input from the current value of the slider.
var slider = document.getElementById('myRange');

function leftIn() {
    console.log('LI-' + slider.value);
}

function leftOut() {
    console.log('LO' + slider.value);
}

function rightIn() {
    console.log('RI-' + slider.value);
}

function rightOut() {
    console.log('RO' + slider.value);
}