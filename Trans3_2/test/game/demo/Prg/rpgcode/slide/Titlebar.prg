#include "rpgcode\slide\Component.prg"

/*
* 
*
*@author Joshua Michael Daly
*@version 0.1
*/
class Titlebar : Component
{
public:
 
    method Titlebar()
    {
        this->title = "";
        this->setBackgroundColour(255, 255, 255)
    }
    
    method ~Titlebar()
    {
        
    }
    
    method setTitle(value)
    {
        title = value
    }
    
    method getTitle()
    {
        return title
    }
    
    method setActionButton(value)
    {
        actionButton = value
    }
    
    method getActionButton()
    {
        return actionButton
    }
    
    method draw()
    {
        local(colour) = this->getBackgroundColour()
        local(rgbValues[])
        split(colour, ",", rgbValues)
        
        local(canvas) = createCanvas(parent->getWidth(), 10)
        colorRGB(rgbValues[0], rgbValues[1], rgbValues[2])
        fillRect(0, 0, parent->getWidth(), 10, canvas)
        
        return canvas
    }
    
private:
    
    var title
    var actionButton
}