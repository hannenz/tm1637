# tm1637

This is a small library / "driver" for using a TM1637 7-segment display (I2C) for the Raspberry Pi using the pigpio library.
(Todo would be to make a library-agnostic version!)

Just because I could not find one I adapted the python code from 


## Usage

```
// Init (initialises pigpio as well!)
tm1637_init(CLOCK, DATA, true, 1.0);

// Clear the display
tm1637_clear()

// Print a number (integer)
tm1637_show_int(8172)

// Clean up (terminates pigpio)
tm1637_cleanup();
```

`tm1637_init()` takes the following parameters: 

- Clock Pin

- Data Pin

- Show colon (bool)

- Brightness (double: 0.0 - 7.0)

