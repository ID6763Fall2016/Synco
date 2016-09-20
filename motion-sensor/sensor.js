// button is attaced to pin 17, LED to 18
var GPIO = require('onoff').Gpio,
    //led = new GPIO(18, 'out'),
    button = new GPIO(18, 'in', 'both');

// pass the callback function to the
// as the first argument to watch() and define
// it all in one step
console.log('started');
button.watch(function(err, state) {

  // check the state of the button
  // 1 == pressed, 0 == not pressed
  if(state == 1) {
    // turn LED on
//    led.writeSync(1);
  } else {
    // turn LED off
//    led.writeSync(0);
  }

        console.log('sensor', state);
});
