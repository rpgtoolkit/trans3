/**
* The base class for all RPGCode GUI components, it is effectively abstract.
*
* @author Joshua Michael Daly
* @version 0.1
*/
class Component
{
public:
    
    /**
    *  
    */
    method Component()
    {
        this->setName("")
        this->setCanvas(createCanvas(0, 0))
        this->setParent("")
        this->setX(0)
        this->setY(0)
        this->setWidth(0)
        this->setHeight(0)
        this->setVisibility(true)
        this->setBackgroundImage("")
        this->setForegroundColour(255, 255, 255)
        this->setBackgroundColour(0, 0, 0)
        this->setFontType("Arial")
    }
    
    /**
    *
    * @param
    */
    method Component(parent)
    {
        this->Component()
        this->setParent(parent)
    }
    
    /**
    *
    */
    method ~Component()
    {
        kill(canvas)
    }
    
    /**
    *
    * @return
    */
    method getName()
    {
        return name
    }
    
    /**
    *
    * @param
    */
    method setName(value)
    {
        name = value
    }
    
    /**
    *
    * @return
    */
    method getX()
    {
        return this->x   
    }
    
    /**
    *
    * @param
    */
    method setX(value)
    {
        this->x = value
    }
    
    /**
    *
    * @return
    */
    method getY()
    {
        return this->y   
    }
    
    /**
    *
    * @param
    */
    method setY(value)
    {
        this->y = value
    }
    
    /**
    *
    * @return
    */
    method getLocation()
    {
        return this->getx() + "," + this->getY()
    }
    
    /**
    *
    * @param 
    */
    method setLocation(valueX, valueY)
    {
        this->setX(valueX) 
        this->setY(valueY)
    }
    
    /**
    *
    * @return
    */    
    method getWidth()
    {
        return width
    }
    
    /**
    *
    * @param
    */
    method setWidth(value)
    {
        width = value
    }
    
    /**
    *
    * @return
    */ 
    method getHeight()
    {
        return height
    }
    
    /**
    *
    * @param
    */
    method setHeight(value)
    {
        height = value
    }
    
    /**
    *
    * @return 
    */
    method getSize()
    {
        return this->getWidth() + "," + this->getHeight()
    }
    
    /**
    *
    * @param
    */
    method setSize(valueX, valueY)
    {
        this->setWidth(valueX)
        this->setHeight(valueY)
    }
    
    /**
    *
    * @return
    */
    method getCanvas()
    {
        return canvas
    }
    
    /**
    *
    * @param
    */
    method setCanvas(value)
    {
        canvas = value
    }
    
    /**
    *
    * @return
    */
    method getParent()
    {
        return parent
    }
    
    /**
    *
    * @param
    */
    method setParent(value)
    {
        parent = value
    }
    
    /**
    *
    * @return
    */
    method getVisibility()
    {
        return isVisible
    }  
    
    /**
    *
    * @param
    */
    method setVisibility(value)
    {
        isVisible = value
    } 
    
    /**
    *
    * @return
    */
    method getBackgroundImage()
    {
        return backgroundImage
    }
  
    /**
    *
    * @param
    */
    method setBackgroundImage(value)
    {
        backgroundImage = value
    }
    
    /**
    *
    * @return
    */
    method getForegroundColour()
    {
        local(rgb) = foregroundColour[0] + "," + foregroundColour[1] + "," + foregroundColour[2]
        
        return rgb
    }
    
    /**
    *
    * @param
    */
    method setForegroundColour(r, g, b)
    {
        foregroundColour[0] = r
        foregroundColour[1] = g
        foregroundColour[2] = b
    }
    
    /**
    *
    * @return
    */
    method getBackgroundColour()
    {
        local(rgb) = backgroundColour[0] + "," + backgroundColour[1] + "," + backgroundColour[2]
        
        return rgb
    }
    
    /**
    *
    * @param
    */
    method setBackgroundColour(r, g, b)
    {
        backgroundColour[0] = r
        backgroundColour[1] = g
        backgroundColour[2] = b
    }
    
    /**
    *
    * @return
    */
    method getFontType()
    {
        return this->fontType
    }
    
    /**
    *
    * @param
    */
    method setFontType(value)
    {
        this->fontType = value
    }
    
private:
   
    var name
    var canvas
    var parent
    var x
    var y
    var width
    var height
    var isVisible
    var backgroundImage
    var foregroundColour[]
    var backgroundColour[]
    var fontType
}