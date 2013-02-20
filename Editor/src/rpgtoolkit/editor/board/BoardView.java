package rpgtoolkit.editor.board;

import java.awt.*;
import java.awt.font.FontRenderContext;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.SwingConstants;
import rpgtoolkit.common.io.types.Board;
import rpgtoolkit.editor.exceptions.TilePixelOutOfRangeException;

/**
 * A view for drawing the board onto.
 * 
 * @author Geoff Wilson
 * @author Joshua Michael Daly
 */
public final class BoardView extends AbstractBoardView
{   
    /**
     * 
     * @param boardController
     * @param board 
     */
    public BoardView(BoardController boardController, Board board)
    {
        super(board);
        
        this.boardController = boardController;
        this.setGridColor(Color.black);
       
        this.repaint();
    }

    /**
     * 
     * @return 
     */
    @Override
    public Dimension getPreferredSize()
    {
        return createPreferredSize();
    }

    /**
     * 
     * @return 
     */
    private Dimension createPreferredSize()
    {
        return new Dimension((board.getWidth() * 32), (board.getHeight() * 32));
    }

    /**
     * 
     * @param g 
     */
    @Override
    public void paint(Graphics g)
    {
        bufferedImage = new BufferedImage((board.getWidth() * 32)
                , (board.getHeight() * 32), BufferedImage.TYPE_INT_ARGB);
        
        try
        {
            this.paintBoard();
        }
        catch (TilePixelOutOfRangeException e)
        {
            
        }
        
        g.drawImage(bufferedImage, 0, 0, this);
    }

    /**
     * 
     * @throws TilePixelOutOfRangeException 
     */
    public void paintBoard() throws TilePixelOutOfRangeException
    {
        Graphics2D g = bufferedImage.createGraphics();

        // Draw background image first
        g.setColor(Color.white);
        g.fillRect(0, 0, (board.getWidth() * 32), (board.getHeight() * 32));
        
        this.paintLayers(g);
        
        if (this.showGrid)
        {
            this.paintGrid(g);
        }
        
        if (this.showVectors)
        {
            this.paintVectors(g);
        }
        
        if (this.showCoordinates)
        {
            this.paintCoordinates(g);
        }
            
        g.dispose();
    }
    
    /**
     * 
     * @param g 
     */
    @Override
    protected void paintLayers(Graphics2D g)
    {
        ArrayList<BoardLayer> layers = boardController.getLayerArrayList();
        
        for (BoardLayer layer : layers)
        {
            if (layer.getVisibility())
            {
                try 
                {
                    layer.drawTiles(g);
                } 
                catch (TilePixelOutOfRangeException ex) 
                {
                    Logger.getLogger(BoardView.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }
    
    /**
     * 
     * @param g 
     */
    @Override
    protected void paintVectors(Graphics2D g) 
    {
        ArrayList<BoardLayer> layers = boardController.getLayerArrayList();
        
        for (BoardLayer layer : layers)
        {
            if (layer.getVisibility())
            {
                layer.drawVectors(g);
            }
        }
    }
    
    /**
     * 
     * @param g 
     */
    @Override
    protected void paintGrid(Graphics2D g) 
    {
        // Determine tile size
        Dimension tileSize = new Dimension(32, 32);
        
        if (tileSize.width <= 0 || tileSize.height <= 0) 
        {
            return;
        }

        // Determine lines to draw from clipping rectangle
        Rectangle clipRectangle = new Rectangle(bufferedImage.getWidth(), 
                bufferedImage.getHeight());
        int startX = (clipRectangle.x / tileSize.width * tileSize.width) ;
        int startY = (clipRectangle.y / tileSize.height * tileSize.height);
        int endX = (clipRectangle.x + clipRectangle.width);
        int endY = (clipRectangle.y + clipRectangle.height);

        g.setColor(this.getGridColor());
        
        for (int x = startX; x < endX; x += tileSize.width) 
        {
            g.drawLine(x, clipRectangle.y, x, clipRectangle.y + clipRectangle.height - 1);
        }
        
        for (int y = startY; y < endY; y += tileSize.height) 
        {
            g.drawLine(clipRectangle.x, y, clipRectangle.x + clipRectangle.width - 1, y);
        }
    }

    /**
     * 
     * @param g 
     */
    @Override
    protected void paintCoordinates(Graphics2D g) 
    {
        Dimension tileSize = new Dimension(32, 32);
        
        if (tileSize.width <= 0 || tileSize.height <= 0) 
        {
            return;
        }
        
        g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
                             RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

        // Determine tile size and offset
        Font font = new Font("SansSerif", Font.PLAIN, tileSize.height / 4);
        g.setFont(font);
        FontRenderContext fontRenderContext = g.getFontRenderContext();
        
        g.setColor(Color.white);

        // Determine area to draw from clipping rectangle
        Rectangle clipRectangle = new Rectangle(bufferedImage.getWidth(), 
                bufferedImage.getHeight());
        int startX = clipRectangle.x / tileSize.width;
        int startY = clipRectangle.y / tileSize.height;
        int endX = (clipRectangle.x + clipRectangle.width) / tileSize.width + 1;
        int endY = (clipRectangle.y + clipRectangle.height) / tileSize.height + 1;

        // Draw the coordinates
        int gy = startY * tileSize.height;
        
        for (int y = startY; y < endY; y++) 
        {
            int gx = startX * tileSize.width;
            
            for (int x = startX; x < endX; x++) 
            {
                String coordinates = "(" + x + "," + y + ")";
                Rectangle2D textSize =
                        font.getStringBounds(coordinates, fontRenderContext);

                int fx = gx + (int) ((tileSize.width - textSize.getWidth()) / 2);
                int fy = gy + (int) ((tileSize.height + textSize.getHeight()) / 2);

                g.drawString(coordinates, fx, fy);
                gx += tileSize.width;
            }
            
            gy += tileSize.height;
        }
    }

    /**
     * 
     * @param visibleRectangle
     * @param orientation
     * @param direction
     * @return 
     */
    @Override
    public int getScrollableBlockIncrement(Rectangle visibleRectangle, 
    int orientation, int direction) 
    {
        Dimension tileSize = new Dimension(32, 32);

        if (orientation == SwingConstants.VERTICAL) 
        {
            return (visibleRectangle.height / tileSize.height) * tileSize.height;
        }
        else 
        {
            return (visibleRectangle.width / tileSize.width) * tileSize.width;
        }
    }

    /**
     * 
     * @param visibleRect
     * @param orientation
     * @param direction
     * @return 
     */
    @Override
    public int getScrollableUnitIncrement(Rectangle visibleRect, 
    int orientation, int direction) 
    {
         Dimension tileSize = new Dimension(32, 32);
         
        if (orientation == SwingConstants.VERTICAL) 
        {
            return tileSize.height;
        }
        else 
        {
            return tileSize.width;
        }
    }
}
