#include "Classes\Component.prg";

class Titlebar : Component
{
public:
 
    method Titlebar()
    {
        this->title = "";
        this->actionButton = Button();
        this->setBackgroundColour(100, 100, 100);
    }
    
    method Titlebar(parent)
    {
        Titlebar();
        this->setParent(parent);
        
        this->setSize(parent->getWidth(), 10);
    }
    
    method ~Titlebar()
    {
        
    }
    
    method setTitle(value)
    {
        title = value;
    }
    
    method getTitle()
    {
        return title;
    }
    
    method setActionButton(value)
    {
        actionButton = value;
    }
    
    method getActionButton()
    {
        return actionButton;
    }
    
    method draw()
    {
        local(colour) = this->getBackgroundColour();
        local(rgbValues[]);
        split(colour, ",", rgbValues);
        
        local(canvas) = createCanvas(size[0], size[1]);
        colorRGB(rgbValues[0], rgbValues[1], rgbValues[2]);
        fillRect(0, 0, this->getWidth(), this->getHeight(), canvas);
        
        return canvas;
    }
    
private:
    
    var title;
    var actionButton;
}