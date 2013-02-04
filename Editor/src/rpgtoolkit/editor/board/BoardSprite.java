package rpgtoolkit.editor.board;

import java.awt.image.BufferedImage;
import java.io.File;
import rpgtoolkit.common.types.BasicType;
import rpgtoolkit.common.types.Item;

public class BoardSprite extends BasicType
{
    private long x;
    private long y;
    private long layer;
    private long activate;
    private String initialVariable;
    private Item spriteFile; // Item filename
    private String finalVariable;
    private String initialValue;
    private String finalValue;
    private String loadingVariable;
    private String loadingValue;
    private long activationType; // Defines how the sprite is activated (step-on or key-press)

    private String activationProgram; // Override activation program
    private String multitaskingProgram; // Override multitask program

    private TKVector boardPath; // TK3.10 relic - not used.

    public BoardSprite()
    {
        super();
    }

    public void setFileName(String fileName)
    {
        System.out.println("Loading Item: " + fileName);
        spriteFile = new Item(new File(System.getProperty("project.path") + "/item/" + fileName));
        spriteFile.loadAnimations();
        spriteFile.setActiveAnimation(0); // SOUTH FACING
    }

    public BufferedImage getAnimationFrame()
    {
        return spriteFile.getAnimationFrame();
    }

    public void setX(long x)
    {
        this.x = x;
    }

    public void setY(long y)
    {
        this.y = y;
    }

    public void setLayer(long layer)
    {
        this.layer = layer;
    }

    public void setActivate(long activate)
    {
        this.activate = activate;
    }

    public void setInitialVariable(String initialVariable)
    {
        this.initialVariable = initialVariable;
    }

    public void setFinalVariable(String finalVariable)
    {
        this.finalVariable = finalVariable;
    }

    public void setInitialValue(String initialValue)
    {
        this.initialValue = initialValue;
    }

    public void setFinalValue(String finalValue)
    {
        this.finalValue = finalValue;
    }

    public void setLoadingVariable(String loadingVariable)
    {
        this.loadingVariable = loadingVariable;
    }

    public void setLoadingValue(String loadingValue)
    {
        this.loadingValue = loadingValue;
    }

    public void setActivationType(long activationType)
    {
        this.activationType = activationType;
    }

    public void setActivationProgram(String activationProgram)
    {
        this.activationProgram = activationProgram;
    }

    public void setMultitaskingProgram(String multitaskingProgram)
    {
        this.multitaskingProgram = multitaskingProgram;
    }

    public void setBoardPath(TKVector boardPath)
    {
        this.boardPath = boardPath;
    }

    public long getX()
    {
        return x;
    }

    public long getY()
    {
        return y;
    }

    public int getWidth()
    {
        return spriteFile.getAnimationFrame().getWidth();
    }

    public int getHeight()
    {
        return spriteFile.getAnimationFrame().getHeight();
    }

    public long getLayer()
    {
        return layer;
    }

    public long getActivate()
    {
        return activate;
    }

    public String getInitialVariable()
    {
        return initialVariable;
    }

    public String getFinalVariable()
    {
        return finalVariable;
    }

    public String getInitialValue()
    {
        return initialValue;
    }

    public String getFinalValue()
    {
        return finalValue;
    }

    public String getLoadingVariable()
    {
        return loadingVariable;
    }

    public String getLoadingValue()
    {
        return loadingValue;
    }

    public long getActivationType()
    {
        return activationType;
    }

    public String getActivationProgram()
    {
        return activationProgram;
    }

    public String getMultitaskingProgram()
    {
        return multitaskingProgram;
    }

    public TKVector getBoardPath()
    {
        return boardPath;
    }
}

