package rpgtoolkit.editor.board;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.ListIterator;
import javax.swing.JPanel;
import rpgtoolkit.common.editor.types.MultiLayerContainer;
import rpgtoolkit.common.io.types.Board;
import rpgtoolkit.common.utilities.TileSetCache;
import rpgtoolkit.editor.main.menus.actions.ZoomInAction;
import rpgtoolkit.editor.main.menus.actions.ZoomOutAction;

/**
 *
 * @author Joshua Michael Daly
 */
public abstract class AbstractBoardView extends JPanel implements MultiLayerContainer
{
    protected AffineTransform affineTransform = new AffineTransform();
    protected TileSetCache tileSetCache;
    
    // Put this somewhere else like an enum.
    public static int ZOOM_NORMALSIZE = 5;
    
    protected Board board;
    protected BufferedImage bufferedImage;
    protected BoardEditor boardEditor;
    
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
    
    protected ArrayList<BoardLayer> layers;
    protected Rectangle bounds;          // in tiles
    
    /**
     * 
     * @param board 
     */
    protected AbstractBoardView(Board board)
    {
        this.board = board;
        layers = new ArrayList();
        bounds = new Rectangle();
        this.setPreferredSize(new Dimension((board.getWidth() * 32)
                , (board.getHeight() * 32)));
        
        this.configureBoardController();
        
        //this.zoomInAction = new ZoomInAction(this);
        //this.zoomOutAction = new ZoomOutAction(this);
        //this.zoomNormalAction = new ZoomNormalAction(this);
    }
    
    /**
     * 
     * @param boardEditor 
     */    
    public void setBoardEditor(BoardEditor boardEditor)
    {
        this.boardEditor = boardEditor;
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
            this.reScale(zoom);
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
            this.reScale(zoom);
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
            this.reScale(zoom);
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
    
    public void reScale(double scale) 
    {  
      affineTransform = AffineTransform.getScaleInstance(zoom, zoom); 
      int width = (int) ((board.getWidth() * 32) * zoom);  
      int height = (int) ((board.getHeight() * 32) * zoom);  
      this.setPreferredSize(new Dimension(width, height));
      this.repaint(); 
   }  
    
    /* Interface Methods */
    
     /**
     * Returns the total number of layers.
     *
     * @return the size of the layer vector
     */
    @Override
    public int getTotalLayers() 
    {
        return layers.size();
    }

    /**
     * Changes the bounds of this plane to include all layers completely.
     */
    @Override
    public void fitBoundsToLayers() 
    {
        int width = 0;
        int height = 0;

        Rectangle layerBounds = new Rectangle();

        for (int i = 0; i < layers.size(); i++) 
	{
            	getLayer(i).getBounds(layerBounds);

 		if (width < layerBounds.width)
		{ 
			width = layerBounds.width;
		}

            	if (height < layerBounds.height)
		{ 
			height = layerBounds.height;
		}
        }

        bounds.width = width;
        bounds.height = height;
    }

    /**
     * Returns a <code>Rectangle</code> representing the maximum bounds in
     * tiles.
     * @return a new rectangle containing the maximum bounds of this plane
     */
    @Override
    public Rectangle getBounds() 
    {
        return new Rectangle(bounds);
    }

    /**
     * Adds a layer to the map.
     *
     * @param layer The {@link MapLayer} to add
     * @return the layer passed to the function
     */
    @Override
    public BoardLayer addLayer(BoardLayer layer) 
    {
        layers.add(layer);
        return layer;
    }

    /**
     * Adds the MapLayer <code>l</code> after the MapLayer <code>after</code>.
     *
     * @param l the layer to add
     * @param after specifies the layer to add <code>l</code> after
     */
    @Override
    public void addLayerAfter(BoardLayer l, BoardLayer after) 
    {
        layers.add(layers.indexOf(after) + 1, l);
    }

    /**
     * Add a layer at the specified index, which should be within
     * the valid range.
     *
     * @param index the position at which to add the layer
     * @param layer the layer to add
     */
    @Override
    public void addLayer(int index, BoardLayer layer) 
    {
        layers.add(index, layer);
    }

    @Override
    public void setLayer(int index, BoardLayer layer) 
    {
        layers.set(index, layer);
    }

    /**
     * Adds all the layers in a given java.util.Collection.
     *
     * @param layers a collection of layers to add
     */
    public void addAllLayers(Collection<BoardLayer> layers) 
    {
        this.layers.addAll(layers);
    }

    /**
     * Removes the layer at the specified index. Layers above this layer will
     * move down to fill the gap.
     *
     * @param index the index of the layer to be removed
     * @return the layer that was removed from the list
     */
    @Override
    public BoardLayer removeLayer(int index) 
    {
        return layers.remove(index);
    }

    /**
     * Returns the layer vector.
     *
     * @return Vector the layer vector
     */
    @Override
    public ArrayList<BoardLayer> getLayerArrayList() 
    {
        return layers;
    }

    /**
     * Sets the layer vector to the given java.util.Vector.
     *
     * @param layers the new set of layers
     */
    @Override
    public void setLayerArrayList(ArrayList<BoardLayer> layers) 
    {
        this.layers = layers;
    }

    /**
     * Moves the layer at <code>index</code> up one in the vector.
     *
     * @param index the index of the layer to swap up
     */
    @Override
    public void swapLayerUp(int index) 
    {
        if (index + 1 == layers.size()) 
        {            
            throw new RuntimeException(
                    "Can't swap up when already at the top.");
        }

        BoardLayer hold = layers.get(index + 1);
        layers.set(index + 1, getLayer(index));
        layers.set(index, hold);
    }

    /**
     * Moves the layer at <code>index</code> down one in the vector.
     *
     * @param index the index of the layer to swap down
     */
    @Override
    public void swapLayerDown(int index) 
    {
        if (index - 1 < 0) 
        {
            throw new RuntimeException(
                    "Can't swap down when already at the bottom.");
        }

        BoardLayer hold = layers.get(index - 1);
        layers.set(index - 1, getLayer(index));
        layers.set(index, hold);
    }

    /**
     * Returns the layer at the specified vector index.
     *
     * @param i the index of the layer to return
     * @return the layer at the specified index, or null if the index is out of
     *         bounds
     */
    @Override
    public BoardLayer getLayer(int i) 
    {
        try 
        {
            return layers.get(i);
        } 
        catch (ArrayIndexOutOfBoundsException e) 
        {
            
        }
        
        return null;
    }

    /**
     * Gets a listIterator of all layers.
     *
     * @return a listIterator
     */
    @Override
    public ListIterator<BoardLayer> getLayers() 
    {
        return layers.listIterator();
    }

    /**
     * Determines whether the point (x,y) falls within the plane.
     *
     * @param x
     * @param y
     * @return <code>true</code> if the point is within the plane,
     *         <code>false</code> otherwise
     */
    @Override
    public boolean inBounds(int x, int y) 
    {
        return x >= 0 && y >= 0 && x < bounds.width && y < bounds.height;
    }

    @Override
    public Iterator<BoardLayer> iterator() 
    {
        return layers.iterator();
    }
    
    private void configureBoardController()
    {
        /*
         * Move this code to the view
         */
        
        //My Hacky code
        tileSetCache = new TileSetCache();
        board.initializeTileSetCache(tileSetCache);
        
        int layerCount = board.getLayerCount();
        
        for (int i = 0; i < layerCount; i++)
        {
            BoardLayer layer = new BoardLayer(board, i);
            this.addLayer(layer);
        }
    }
}
