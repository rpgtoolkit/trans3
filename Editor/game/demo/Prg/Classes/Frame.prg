//Basic GUI frame.

class Frame : Component
{
public:
    
    method Frame()
    {
        this->titlebar = Titlebar();
        this->Component();
    }
    
    method Frame(title)
    {
        this->Component();
        this->Frame();
        this->setTitle(title);
    }
    
    method ~Frame()
    {
        
    }
    
    method setTitlebarParent(parent)
    {
       this->titlebar->setParent(parent);
    }
    
    method setLocation(x, y)
    {
        location[0] = x;
        location[1] = y;
    }
    
    method getLocation()
    {
        return location;
    }
    
    method setTitle(value)
    {
        title = value
    }
    
    method getTitle()
    {
        return title
    }
    
    method setTitlebar(value)
    {
        titlebar = value;
    }
    
    method getTitlebar()
    {
        return titlebar;   
    }
    
    method setContentPane(value)
    {
        contentPane = value;
    }
    
    method getContentPane()
    {
        return contentPane;
    }
    
    method showFrame()
    {
        this->draw();
    }
    
    method hide()
    {
        
    }
    
    
    method close()
    {
        
    }
    
private:
    
    //private memebers
    var title;
    var frame;
    var titlebar;
    var location[];
    var contentPane;
    
    //methods
    method draw()
    {
        local(titlebarCanvas) = this->drawTitlebar();

        local(colour) = this->getBackgroundColour();
        local(rgbValues[]);
        split(colour, ",", rgbValues);
        
        local(canvas) = createCanvas(this->getWidth, this->getHeight);
        colorRGB(rgbValues[0], rgbValues[1], rgbValues[2]);
        fillRect(0, 0, this->getWidth, this->getHeight, canvas);
        
        colorRGB(0, 0, 0);
        text(1, 1, this->getTitle(), canvas);
        
        drawCanvas(titlebarCanvas, location[0], location[1]);
        drawCanvas(canvas, location[0], location[1]);
    }
    
    method drawTitlebar()
    {
        mwin("drawing");
        wait();
        local(titlebarCanvas) = this->titlebar->draw();
        return titlebarCanvas;
    }
    
    method drawContentPane()
    {
        
    }
}