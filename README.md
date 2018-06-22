# VFS5 Vox footswitch emulation and improved functions with Arduino Nano
Based on the reverse engineering from [Whitelamp](http://www.whitelamp.com/public/vox_vfs5.html) and the PWM implementation from [@santoxyz](https://github.com/santoxyz/vfs5)

This is an emulator for the VFS5 Vox footswitch for their own amps running on a Arduino nano.

### Features:

  - 2 DPDT buttons support (up/down on channel selection)
  - 4 Led for mode indication (solid light for the first four modes and blink for the second bank)
  - Auto shutdown detection for saving the last channel activated on EEPROM
  - Auto setting the last saved channel when connected
  - Shelf powered from the amp connector (no need of batteries)

### Schematic:

                                   DPDT         DPDT

                                   X X X+-----+X X X

                                   X X X<+   +>X X X
                                         |   |
                                   X X X |   | X X X
                                         |   |
                                     +---+   +-+
                                     |         |
                  +------------------------------------------------------------------+
                  |                  |         |                                     |
                  |                  |         |                                     |
      FROM AMP    |             +----+---------+------+                              |
     +--------+   |             | bankPin  channelPin |                              |
     |        |   |             |                     |                              |
     |      T +-----------------+ VCC             OUT +---->1K2Ω+---+----->1k2Ω+-----+
     |        |   |             |                     |             |                |
     |      R | <-+    +--------+ GND   ARDUINO       |             |                |
     |        |        |        |        NANO         |             v                v
     |      S +--------+        |                     |           47nF             47nF
     |        |                 |                 GND +-----+       +                +
     +--------+                 |                     |     +-------+----------------+
                                | LED1 LED2 LED3 LED4 |     |
                                +--+----+----+----+---+     |
                                   |    |    |    |         |
                                   |    |    |    |         |
                                   v    v    v    v         |
                                  XXX  XXX  XXX  XXX        |
                                  XXX  XXX  XXX  XXX        |
                                  XXX  XXX  XXX  XXX        |
                                   +    +    +    +         |
                                   |    |    |    |         |
                                   +----+----+----+---------+





