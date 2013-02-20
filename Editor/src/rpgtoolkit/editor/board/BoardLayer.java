package rpgtoolkit.editor.board;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.util.ArrayList;
import rpgtoolkit.common.editor.types.MultiLayerContainer;
import rpgtoolkit.common.editor.types.Tile;
import rpgtoolkit.common.io.types.Board;
import rpgtoolkit.editor.board.types.BoardVector;
import rpgtoolkit.editor.exceptions.TilePixelOutOfRangeException;

/**
 *
 * @author Joshua Michael Daly
 */
public final class BoardLayer implements Cloneable
{
    protected String name;
    protected int layerNumber;
    protected boolean isVisible;
    protected boolean isLocked;
    protected Board parentBoard;
    protected MultiLayerContainer parentContainer;
    protected float opacity;
    protected Rectangle bounds;
    
    protected ArrayList<Tile> tiles;
    
    /**
     * 
     */
    public BoardLayer()
    {
        
    }
    
    /**
     * 
     * @param width
     * @param height 
     */
    public BoardLayer(int width, int height)
    {
        
    }
    
    /**
     * 
     * @param rectangle 
     */
    public BoardLayer(Rectangle rectangle)
    {
        
    }
    
    /**
     * 
     * @param board 
     */
    public BoardLayer(Board board)
    {
        
    }
    
    /**
     * 
     * @param boardController
     * @param layerNumber 
     */
    public BoardLayer(Board board, int layerNumber)
    {
        this.parentBoard = board;
        this.layerNumber = layerNumber;
        this.tiles = new ArrayList();
        this.isVisible = true;
        this.configureLayer();
    }
    
    /**
     * 
     * @param board
     * @param width
     * @param height 
     */
    public BoardLayer(Board board, int width, int height)
    {
        
    }
   
    /**
     * Returns layer width in tiles.
     * @return layer width in tiles.
     */
    public int getWidth() 
    {
        return bounds.width;
    }

    /**
     * Returns layer height in tiles.
     * @return layer height in tiles.
     */
    public int getHeight() 
    {
        return bounds.height;
    }

    /**
     * Returns the layer bounds in tiles.
     * @return the layer bounds in tiles
     */
    public Rectangle getBounds() 
    {
        return new Rectangle(bounds);
    }
    
    /**
     * Assigns the layer bounds in tiles to the given rectangle.
     * @param rectangle the rectangle to which the layer bounds are assigned
     */
    public void getBounds(Rectangle rectangle) 
    {
        rectangle.setBounds(bounds);
    }
    
    /**
     * Sets the bounds (in tiles) to the specified Rectangle.
     *
     * @param bounds
     */
    protected void setBounds(Rectangle bounds) 
    {
        this.bounds = new Rectangle(bounds);
    }
    
    /**
     * Returns the name of this layer.
     * @return the name of the layer
     */
    public String getName() 
    {
        return name;
    }

    /**
     * Sets the name of this layer.
     *
     * @param name the new name
     */
    public void setName(String name) 
    {
        this.name = name;
    }

    /**
     * Sets the map this layer is part of.
     *
     * @param map the Map object
     */
    public void setBoard(Board board)
    {
        parentBoard = board;
    }

    /**
     * Sets layer opacity. If it is different from the previous value and the
     * layer is visible, a MapChangedEvent is fired.
     *
     * @param opacity the new opacity for this layer
     */
    public void setOpacity(float opacity) 
    {
        if (this.opacity != opacity) 
        {
            this.opacity = opacity;

            if (this.getVisibility() && parentBoard != null) 
            {
                //parentBoard.fireMapChanged();
            }
        }
    }

    /**
     * 
     * @return 
     */
    public boolean getVisibility()
    {
        return isVisible;
    }
    
    /**
     * Sets the visibility of this map layer. If it changes from its current
     * value, a MapChangedEvent is fired visibility.
     *
     * @param visible <code>true</code> to make the layer visible;
     *                <code>false</code> to make it invisible
     */
    public void setVisibility(boolean visible) 
    {
        if (isVisible != visible) 
        {
            isVisible = visible;
            
            if (parentBoard != null) 
            {
                //parentBoard.fireMapChanged();
            }
        }
    }

    /**
     * Sets the offset of this map layer. The offset is a distance by which to
     * shift this layer from the origin of the map.
     *
     * @param xOffset x offset in tiles
     * @param yOffset y offset in tiles
     */
    public void setOffset(int xOffset, int yOffset) 
    {
        bounds.x = xOffset;
        bounds.y = yOffset;
    }
    
    /**
     * 
     */
    public void configureLayer()
    {
        int width = parentBoard.getWidth();
        int height = parentBoard.getHeight();
        
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                int tileIndex = parentBoard.getIndexAtLocation(x, y, layerNumber) - 1;

                // There is tile data on this layer 
                if (tileIndex != -1)
                {
                    tiles.add(parentBoard.getTileFromIndex(tileIndex));
                }
                else // -1 no tile data on this layer
                {
                    tiles.add(new Tile());
                }
            }
        }
    }
    
    public void drawTiles(Graphics2D g, int zoom) throws TilePixelOutOfRangeException
    {
        for (int x = 0; x < parentBoard.getWidth(); x++)
        {
            for (int y = 0; y < parentBoard.getHeight(); y++)
            {
                int indexToPaint = parentBoard.getIndexAtLocation(x, y, 
                        layerNumber) - 1;

                if (indexToPaint >= 0)
                {
                    Tile tile = parentBoard.getTileFromIndex(indexToPaint);

                    g.drawImage(tile.getTileAsImage(zoom), (x * 32) * zoom, 
                            (y * 32) * zoom, null);
                }
                else
                {
                    g.setColor(Color.white);
                }
            }
        }
    }
    
    public void drawVectors(Graphics2D g, int zoom)
    {
        // Draw Vectors
        ArrayList<BoardVector> vectors = parentBoard.getVectors();
        
        for (BoardVector vector : vectors)
        {
            // Draw lines from points 0 > 1 , 1 > 2, 2 > 3 etc..
            int count = vector.getPointCount();
            switch (vector.getTileType())
            {
                case 1:
                    g.setColor(Color.WHITE);
                    break;
                case 2:
                    g.setColor(Color.GREEN);
                    break;
                case 16:
                    g.setColor(Color.RED);
                    break;
                default:
                    g.setColor(Color.WHITE);
            }

            for (int i = 0; i < count - 1; i++)
            {
                g.drawLine(vector.getPointX(i) * zoom, vector.getPointY(i) * zoom
                        , vector.getPointX(i + 1) * zoom, vector.getPointY(i + 1) * zoom);
            }

            // Draw the final lines
            g.drawLine(vector.getPointX(count - 1) * zoom, 
                    vector.getPointY(count - 1) * zoom, 
                    vector.getPointX(0) * zoom, 
                    vector.getPointY(0) * zoom);
        }
    }
}
