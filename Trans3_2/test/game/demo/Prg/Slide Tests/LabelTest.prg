#include "rpgcode\slide\Label.prg"

local(label) = Label("Hello World")
label->setName("myLabel")
label->setX(10)
label->setY(10)
label->setBackgroundColour(100, 100, 100)
label->setForegroundColour(255, 255, 255)
label->setFontType("Arial")
label->paint()

local(label2) = Label("Hello World")
label2->setName("myLabel")
label2->setLocation(10, 50)
label2->setBackgroundColour(100, 100, 100)
label2->setForegroundColour(255, 255, 255)
label2->setFontType("Times")
label2->paint()
wait()