# Surnia's Sugar Glider

![mechwild/sugarglider](https://i.imgur.com/mSPmynX.jpg)

I seem to prefer reversed R4 MT3 keycaps for thumbs, so I sadly had to sub in upside down letters instead of R5 1u spacebars.

This is Surnia's Keymap setup. COLEMAK DH is the primary map, and a qwerty map is defined as well. The map is VIA enabled. 

* Keymap: [KLE Link](http://www.keyboard-layout-editor.com/#/gists/8de81be187c12512f1ce66ae676c5716)
* Hardware Availability: [Sugar Glider on MechWild](https://mechwild.com/product/sugar-glider/)

Customizations that are done:

* Left layer and right layer keys are setup under tap dance, allowing layer access through one key. 
    Left order is F keys (and Symbols), Numpad, Nav
    Right order is Numpad, Nav, F Keys (and Symbols)
**Nav is setup for right hand only, the left half is not setup with any keys.**

* Middle mouse click is set with dual tap dance, access to Browser layer. Left click is back, Right click is forward. 
* If Right layer key is used, the mouse clicks will switch and the left becomes right click. This does not affect the BROWSER layer. 
* Left alt is tap danced with GUI. 2 taps for GUI. 

* Edge scroll was tweaked to actuate more frequently. cirque_pinnacle_gestures.c, line 64 was adjusted from a stock value of 18 ticks per revolution, to 30. 