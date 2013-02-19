package rpgtoolkit.editor.board;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import javax.swing.JPanel;
import javax.swing.Scrollable;
import rpgtoolkit.common.types.Board;
import rpgtoolkit.editor.main.menus.actions.ZoomInAction;
import rpgtoolkit.editor.main.menus.actions.ZoomOutAction;

/**
 *
 * @author Joshua Michael Daly
 */
public abstract class AbstractBoardView extends JPanel implements Scrollable
{
    // Put this somewhere else like an enum.
    public static int ZOOM_NORMALSIZE = 1;
    
    protected Board board;
    protected BufferedImage bufferedImage;
    protected BoardController boardController;
    
    protected double zoom = 1.0;
    protected int zoomLevel = ZOOM_NORMALSIZE;
    
    // Put this in an enum.
    protected static double[] zoomLevels = {
        0.0625, 0.125, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 4.0
    };
    
    // Grid properties
    protected boolean showGrid;
    protected boolean antialiasGrid;
    protected Color gridColor;
    protected int gridOpacity;
    
    protected boolean showCoordinates;
    protected boolean showVectors;
    
    protected static final Color DEFAULT_GRID_COLOR = Color.black;
    private static final Color DEFAULT_BACKGROUND_COLOR = new Color(64, 64, 64);
    
    private ZoomInAction zoomInAction;
    private ZoomOutAction zoomOutAction;
    
    /**
     * 
     * @param board 
     */
    protected AbstractBoardView(Board board)
    {
        this.board = board;
        //this.zoomInAction = new ZoomInAction(this);
        //this.zoomOutAction = new ZoomOutAction(this);
        //this.zoomNormalAction = new ZoomNormalAction(this);
    }
    
    /**
     * 
     * @param boardController 
     */    
    public void setBoardController(BoardController boardController)
    {
        this.boardController = boardController;
    }

    /**
     * 
     * @return 
     */
    public Board getBoard()
    {
        return this.board;
    }
    
    /**
     * 
     * @param board 
     */
    public void setBoard(Board board)
    {
        this.board = board;
    }
    
    /**
     * 
     * @return 
     */
    public Color getGridColor()
    {
        return gridColor;
    }
    
    /**
     * 
     * @param gridColor 
     */
    public void setGridColor(Color gridColor)
    {
        this.gridColor = gridColor;
        this.repaint();
    }
    
    /**
     * 
     * @param gridOpacity 
     */
    public void setGridOpacity(int gridOpacity)
    {
        this.gridOpacity = gridOpacity;
        this.repaint();
    }
    
    /**
     * 
     * @param antialiasGrid 
     */
    public void setAntialiasGrid(boolean antialiasGrid)
    {
        this.antialiasGrid = antialiasGrid;
        this.repaint();
    }
    
    /**
     * 
     * @return 
     */
    public boolean getShowGrid()
    {
        return showGrid;
    }
    
    /**
     * 
     * @param showGrid 
     */
    public void setShowGrid(boolean showGrid)
    {
        this.showGrid = showGrid;
        this.revalidate();
        this.repaint();
    }
    
    public boolean isShowCoordinates() 
    {
        return showCoordinates;
    }

    public void setShowCoordinates(boolean showCoordinates) 
    {
        this.showCoordinates = showCoordinates;
        this.revalidate();
        this.repaint();
    }
    
    public boolean isShowVectors() 
    {
        return showVectors;
    }

    public void setShowVectors(boolean showVectors) 
    {
        this.showVectors = showVectors;
        this.revalidate();
        this.repaint();
    }
    
    public ZoomInAction getZoomInAction()
    {
        return this.zoomInAction;
    }
    
    public void setZoomInAction(ZoomInAction zoomInAction)
    {
        this.zoomInAction = zoomInAction;
    }
    
    public ZoomOutAction getZoomOutAction()
    {
        return zoomOutAction;
    }
    
    public void setZoomOutAction(ZoomOutAction zoomOutAction)
    {
        this.zoomOutAction = zoomOutAction;
    }
    
    /**
     * 
     * @return 
     */
    public boolean zoomIn() 
    {
        if (zoomLevel < zoomLevels.length - 1) 
        {
            this.setZoomLevel(zoomLevel + 1);
            this.repaint();
        }

        return zoomLevel < zoomLevels.length - 1;
    }

    /**
     * 
     * @return 
     */
    public boolean zoomOut() 
    {
        if (zoomLevel > 0) 
        {
            this.setZoomLevel(zoomLevel - 1);
            this.repaint();
        }

        return zoomLevel > 0;
    }

    /**
     * 
     * @param zoom 
     */
    public void setZoom(double zoom) 
    {
        if (zoom > 0) 
        {
            this.zoom = zoom;
            //revalidate();
            this.setSize(getPreferredSize());
        }
    }

    /**
     * 
     * @param zoomLevel 
     */
    public void setZoomLevel(int zoomLevel) 
    {
        if (zoomLevel >= 0 && zoomLevel < zoomLevels.length) 
        {
            this.zoomLevel = zoomLevel;
            this.setZoom(zoomLevels[zoomLevel]);
        }
    }

    /**
     * 
     * @return 
     */
    public double getZoom() 
    {
        return zoom;
    }

    /**
     * 
     * @return 
     */
    public int getZoomLevel() 
    {
        return zoomLevel;
    }
    
    /**
     * 
     * @return 
     */
    @Override
    public Dimension getPreferredScrollableViewportSize() 
    {
        return this.getPreferredSize();
    }

    /**
     * 
     * @return 
     */
    @Override
    public boolean getScrollableTracksViewportWidth()
    {
        return false;
    }


    /**
     * 
     * @return 
     */
    @Override
    public boolean getScrollableTracksViewportHeight() 
    {
        return false;
    }
    
    /**
     * 
     * @param visibleRect
     * @param orientation
     * @param direction
     * @return 
     */
    @Override
    public abstract int getScrollableUnitIncrement(Rectangle visibleRect, 
    int orientation, int direction);
    
    /**
     * 
     * @param visibleRect
     * @param orientation
     * @param direction
     * @return 
     */
    @Override
    public abstract int getScrollableBlockIncrement(Rectangle visibleRect, 
    int orientation, int direction);
     
    /**
     * 
     * @param g 
     */
    protected abstract void paintLayers(Graphics2D g);
    
    /**
     * 
     * @param g 
     */
    protected abstract void paintVectors(Graphics2D g);
    
    /**
     * 
     * @param g 
     */
    protected abstract void paintGrid(Graphics2D g);
    
    /**
     * 
     * @param g 
     */
    protected abstract void paintCoordinates(Graphics2D g);
}
