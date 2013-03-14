

/**
* The base class for all RPGCode GUI components, it is effectively abstract.
*
*@author Joshua Michael Daly
*@version 0.1
*/
class Component
{
public:
   
    method Component()
    {
        this->setName("")
        this->setWidth(0)
        this->setHeight(0)
        this->setVisibility(false)
        this->setBackgroundImage("")
        this->setBackgroundColour(0, 0, 0)
    }
    
    method Component(parent)
    {
        this->Component()
        this->setParent(parent)
    }
    
    method ~Component()
    {
        kill(canvas)
    }
    
    method setName(value)
    {
        name = value
    }
    
    method getName()
    {
        return name
    }
    
    method setWidth(value)
    {
        width = value
    }
    
    method getWidth()
    {
        return width
    }
    
    method setHeight(value)
    {
        height = value
    }
    
    method getHeight()
    {
        return height
    }
    
    method setCanvas(value)
    {
        canvas = value
    }
    
    method getCanvas()
    {
        return canvas
    }
    
    method setParent(value)
    {
        parent = value
    }
    
    method getParent()
    {
        return parent
    }
    
    method setVisibility(value)
    {
        isVisible = value
    } 
  
    method getVisibility()
    {
        return isVisible
    }  
    
    method setBackgroundImage(value)
    {
        backgroundImage = value
    }
    
    method getBackgroundImage()
    {
        return backgroundImage
    }
    
    method setBackgroundColour(r, g, b)
    {
        backgroundColour[0] = r
        backgroundColour[1] = g
        backgroundColour[2] = b
    }
    
    method getBackgroundColour()
    {
        local(rgb) = backgroundColour[0] + "," + backgroundColour[1] + "," + backgroundColour[2]
        return rgb
    }
    
 
private:
   
    var name
    var canvas
    var parent
    var width
    var height
    var isVisible
    var backgroundImage
    var backgroundColour[]
}