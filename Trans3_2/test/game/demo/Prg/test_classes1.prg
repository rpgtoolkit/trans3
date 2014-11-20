//GUI Framework Package
debugger("1");
local(frame) = Frame();
debugger("2");
frame->setName("Hello");
debugger("3");
frame->setWidth(200);
debugger("4");
frame->setHeight(200);
debugger("5");
frame->setTitlebarParent(frame);
debugger("6");
frame->setLocation(50, 50);
debugger("7");
frame->setBackgroundColour(122, 122, 122);
debugger("8");
frame->showFrame();
debugger("9");
wait();

class Frame : Component
{
public:

    method Frame()
    {
        this->titlebar = Titlebar();
        this->Component();
	  setName("AHA");
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

        local(canvas) = createCanvas(this->getWidth(), this->getHeight());
        colorRGB(rgbValues[0], rgbValues[1], rgbValues[2]);
        fillRect(0, 0, this->getWidth(), this->getHeight(), canvas);

        drawCanvas(canvas, location[0], location[1]);
        drawCanvas(titlebarCanvas, location[0], location[1]);
    }

    method drawFrame()
    {

    }

    method drawTitlebar()
    {
        local(titlebarCanvas) = this->titlebar->draw();
        return titlebarCanvas;
    }

    method drawContentPane()
    {

    }
}

class Titlebar : Component
{
public:

    method Titlebar()
    {
        this->title = "";
        this->setBackgroundColour(255, 255, 255);
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

        local(canvas) = createCanvas(parent->getWidth(), 10);
        colorRGB(rgbValues[0], rgbValues[1], rgbValues[2]);
        fillRect(0, 0, parent->getWidth(), 10, canvas);

        return canvas;
    }

private:

    var title;
    var actionButton;
}

class Component
{
public:

    method Component()
    {
        this->setName("");
        this->setWidth(0);
        this->setHeight(0);
        this->setVisibility(false);
        this->setBackgroundImage("");
        this->setBackgroundColour(0, 0, 0);
    }

    method Component(parent)
    {
        this->Component();
        this->setParent(parent);   
    }

    method ~Component()
    {
        kill(canvas);
    }

    method setName(value)
    {
        name = value;
    }

    method getName()
    {
        return name;
    }

    method setWidth(value)
    {
        width = value;
    }

    method getWidth()
    {
        return width;
    }

    method setHeight(value)
    {
        height = value;
    }

    method getHeight()
    {
        return height;
    }

    method setCanvas(value)
    {
        canvas = value;
    }

    method getCanvas()
    {
        return canvas;
    }

    method setParent(value)
    {
        parent = value;   
    }

    method getParent()
    {
        return parent;   
    }

    method setVisibility(value)
    {
        isVisible = value;
    } 

    method getVisibility()
    {
        return isVisible;
    }  

    method setBackgroundImage(value)
    {
        backgroundImage = value;
    }

    method getBackgroundImage()
    {
        return backgroundImage;
    }

    method setBackgroundColour(r, g, b)
    {
        backgroundColour[0] = r;
        backgroundColour[1] = g;
        backgroundColour[2] = b;
    }

    method getBackgroundColour()
    {
        local(rgb) = backgroundColour[0] + "," + backgroundColour[1] + "," + backgroundColour[2];
        return rgb;
    }

private:

    method HelloWorld()
    {

    }

    var name;
    var canvas;
    var parent;
    var width;
    var height;
    var isVisible;
    var backgroundImage;
    var backgroundColour[];
}
