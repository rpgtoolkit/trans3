#include "rpgcode\slide\Frame.prg"

local(frame) = MyFrame()
frame->setName("Hello")
frame->setWidth(200)
frame->setHeight(200)
frame->setTitlebarParent(frame)
frame->setLocation(50, 50)
frame->setBackgroundColour(122, 122, 122)
frame->showFrame()
wait()

class MyFrame : Frame
{
public:
    
    method MyFrame()
    {
        this->Frame()
    }
}