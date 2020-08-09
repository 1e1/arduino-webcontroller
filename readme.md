# WrightSlave

A single controller connected by USB or Web.


## Commands

Standard REST routes:
- `GET` `/`
  full status list of all pins

- `GET` `/{pin_id}`
  view a pin

- `PUT` `/{pin_id}/{value}`
  set a value (0-255) to this pin
  (set a pin to OUTPUT mode)
  On digital: false=0-127; true-128-255

- `DELETE` `/{pin_id}`
  set a pin to INPUT mode

Extended routes:
- `PUT` `/{pin_id}.`
  hide a pin in the full status list

- `PUT` `/{pin_id}!`
  show a pin in the full status list 


## Web

The Arduino boots on the DHCP. 
If `#define MODE_PRO 1` is not set (by default), 
open a bowser on `http://{ip}/$`. 


## USB

If `#define MODE_PRO 1` is not set (by default), 
write anything and the read the help


## dependancies

### software

- standard Ethernet.h


### hardware

- Serial
