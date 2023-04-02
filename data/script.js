// Variables to hold elements
let counterEl = document.getElementById("counter");
let incBtn = document.getElementById("increment-btn");
let decBtn = document.getElementById("decrement-btn");
let redLed = document.getElementById("red-led");
let greenLed = document.getElementById("green-led");

// Variables to hold state
let counter = 0;
let intervalId = null;
let isPressed = false;

// Update the counter display
function updateCounterDisplay() {
  counterEl.innerHTML = counter;
  if (counter === 0) {
    redLed.style.backgroundColor = "red";
  } else {
    redLed.style.backgroundColor = "gray";
  }
}

// Increment the counter
function incrementCounter() {
  counter++;
  updateCounterDisplay();
}

// Decrement the counter
function decrementCounter() {
  if (counter > 0) {
    counter--;
  }
  updateCounterDisplay();
}

// Handler for increment button press
function handleIncrementPress() {
  incrementCounter();
  isPressed = true;
  intervalId = setInterval(incrementCounter, 1000);
}

// Handler for increment button release
function handleIncrementRelease() {
  isPressed = false;
  clearInterval(intervalId);
}

// Handler for decrement button press
function handleDecrementPress() {
  decrementCounter();
}

// Handler for increment button release
function handleDecrementRelease() {}

// Add event listeners for button presses and releases
incBtn.addEventListener("mousedown", handleIncrementPress);
incBtn.addEventListener("mouseup", handleIncrementRelease);
decBtn.addEventListener("mousedown", handleDecrementPress);
decBtn.addEventListener("mouseup", handleDecrementRelease);

// Add event listeners for touch events on mobile devices
incBtn.addEventListener("touchstart", handleIncrementPress);
incBtn.addEventListener("touchend", handleIncrementRelease);
decBtn.addEventListener("touchstart", handleDecrementPress);
decBtn.addEventListener("touchend", handleDecrementRelease);

// Update the green LED when the increment button is pressed
setInterval(function() {
  if (isPressed) {
    greenLed.style.backgroundColor = (greenLed.style.backgroundColor === "gray") ? "green" : "gray";
  } else {
    greenLed.style.backgroundColor = "gray";
  }
}, 500);