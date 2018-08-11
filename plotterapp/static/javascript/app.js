// Set the canvas id to the variable canvas
var canvasDiv = document.getElementById('canvasDiv');
var clearCanvas = document.getElementById('clearcanvas');
canvas = document.createElement('canvas');

// Get the context 2d for the canvas
context = canvas.getContext('2d');

// Define arrays for storing the pixel values corresponding to the drawing the user has made.
var mouse_X_pos = new Array();
var mouse_Y_pos = new Array();
var clickDrag = new Array();

//Canvas dimension variables
//TODO: For now dummy numbers. Change these later to get data from the user form input.
var canvasWidth = 1000;
var canvasHeight = 500;

// Set the canvas window attributes.
canvas.setAttribute('width', canvasWidth);
canvas.setAttribute('height', canvasHeight);
canvas.setAttribute('id', 'canvas')

//Append the canvas to canvasDiv
canvasDiv.appendChild(canvas);

//Intially set the paint to false
var paint = false;

if(typeof G_vmlCanvasManager != 'undefined') {
	canvas = G_vmlCanvasManager.initElement(canvas);
}

// When the user starts drawing we record the position in an array via the add click function.
// This is for when the user is clicking down on the screen.
$('#canvas').mousedown(function(e){
  var mouseX = e.pageX - this.offsetLeft;
  var mouseY = e.pageY - this.offsetTop;

  paint = true;
  addClick(e.pageX - this.offsetLeft, e.pageY - this.offsetTop);
  redraw();
});

// If paint is true, then this will start drawing.
// We first record the position in the array & drawing starts. This is for moving the
// virtual pen on the screen.
$('#canvas').mousemove(function(e){
  if(paint){
    addClick(e.pageX - this.offsetLeft, e.pageY - this.offsetTop, true);
    redraw();
  }
});

// For when the mouse button is released.
$('#canvas').mouseup(function(e){
  paint = false;
});
$('#canvas').mouseleave(function(e){
  paint = false;
});

// Stores the x & y coordinates of mouse
function addClick(x, y, dragging)
{
    mouse_X_pos.push(x);
    mouse_Y_pos.push(y);
    clickDrag.push(dragging);
}

// This is where we draw in the canvas.
function redraw(){
  $("#clearCanvas").click(function () {
      context.clearRect(0, 0, context.canvas.width, context.canvas.height);
      clickX = []; clickY = []; clickDrag = []; // This will empty the array after the clear button has been pressed.
    }); // Clears the canvas

  context.strokeStyle = "#df4b26";
  context.lineJoin = "round";
  //TODO: Later set this to user preference or pencil width of the plotter.
  context.lineWidth = 5;

  for(var i=0; i < mouse_X_pos.length; i++) {
    context.beginPath();
    if(clickDrag[i] && i){
      context.moveTo(mouse_X_pos[i-1], mouse_Y_pos[i-1]);
     }else{
       context.moveTo(mouse_X_pos[i]-1, mouse_Y_pos[i]);
     }
     context.lineTo(mouse_X_pos[i], mouse_Y_pos[i]);
     context.closePath();
     context.stroke();
  }
}
