#include "rpgcode\slide\Component.prg"

/**
*  A Label object is a component for placing text in a 
*  container. A label displays a single line of read-only text.
*  The text can be changed by the application, but a user cannot 
*  edit it directly.
*
* @author Joshua Michael Daly
* @version 0.1
*/
class Label : Component
{
public:
    
    /**
    * Constructs an empty label. The text of the label is the empty
    * string "".
    */
    method Label()
    {
        this->Label("")   
    }
    
    /**
    * Constructs a new label with the specified string of text.
    *
    * @param textValue The string that the label represents.
    */
    method Label(value)
    {
        this->Component()
        this->textValue = value
    }
    
    /**
    *
    */
    method ~Label()
    {
        
    }
    
    /**
    * Gets the text of this label.
    *
    * @return The text of this label.
    */
    method getText()
    {
        return this->textValue   
    }
    
    /**
    * Sets the text of this label to the specified text.
    *
    * @param value The text that this label displays.
    */
    method setText(value)
    {
        this->textValue = value  
    }
    
    /**
    * Paints the label to its canvas member and if the labels 
    * visibility is true it blits the canvas to the screen.
    */
    method paint()
    {
        // Set the global font to this labels font.
        // We need do this here so we can determine 
        // the size for the drawing canvas.
        font(this->getFontType())
        
        // Override the components width and height
        // values, it is unlikely that the user will 
        // only want to see part of a label.
        local(width)
        local(height)
        width = getTextWidth(this->getText())
        height = getTextHeight(this->getText())
        
	  debugger(width)
	  debugger(height)

        // For performance reasons labels with
        // 0x0 dimensions will not be drawn.
        if (width == 0 && height == 0)
        {
            return 0 // Must explictly return a value in RPGCode.
        }

        // Create the canvas for drawing to.
        this->setCanvas(createCanvas(width, height))
        
        // Get the background colour to use for the canvas.
        local(backgroundColour) = this->getBackgroundColour()
        local(backgroundRGB[])
        split(backgroundColour, ",", backgroundRGB)
        
        // Draw the background colour.
        colorRGB(backgroundRGB[0], backgroundRGB[1], backgroundRGB[2])
        fillRect(0, 0, width, height, this->getCanvas())
        
        // Get the foreground colour to use for the text.
        local(foregroundColour) = this->getForegroundColour()
        local(foregroundRGB[])
        split(foregroundColour, ",", foregroundRGB)
        
        // Draw the text.
        colorRGB(foregroundRGB[0], foregroundRGB[1], foregroundRGB[2])
        pixelText(0, 0, this->getText(), this->getCanvas())
        
        // Only draw the label if it should be visible.
        if (this->getVisibility())
        {
            drawCanvas(canvas, this->getX(), this->getY())
        }     
    }
    
private:
    
    var textValue /* The labels text */
}
