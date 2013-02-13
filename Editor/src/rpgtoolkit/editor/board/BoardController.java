package rpgtoolkit.editor.board;

import java.io.File;
import rpgtoolkit.common.types.Board;
import rpgtoolkit.common.utilities.TileSetCache;
import rpgtoolkit.editor.main.menus.actions.ZoomInAction;
import rpgtoolkit.editor.main.menus.actions.ZoomOutAction;

/**
 *
 * @author Joshua Michael Daly
 */
public final class BoardController extends MultiLayerContainer
{
    private Board board;
    private BoardView boardView;
    private TileSetCache tileSetCache;
    private ZoomInAction zoomInAction;
    private ZoomOutAction zoomOutAction;
    
    public BoardController()
    {
        
    }
    
    public BoardController(File fileName)
    {
        this.board = new Board(fileName);
        this.configureBoardController();
        this.boardView = new BoardView(this, board);
        this.zoomInAction = new ZoomInAction();
        this.zoomOutAction = new ZoomOutAction();
    }
    
    public Board getBoard()
    {
        return board;
    }
    
    public BoardView getBoardView()
    {
        return boardView;
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
   
    public void configureBoardController()
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
    
    /**
     * Move this to the BoardView Class instead.
     * @param layer
     * @return 
     */
    @Override
    public BoardLayer addLayer(BoardLayer layer) 
    {
        layer.setBoard(board);
        super.addLayer(layer);
  
        return layer;
    }
    
    @Override
    public void setLayer(int index, BoardLayer layer) 
    {
        layer.setBoard(board);
        super.setLayer(index, layer);
        //fireMapChanged();
    }
    
     /**
     * Calls super method, and additionally fires a {@link MapChangedEvent}.
     *
     * @see MultilayerPlane#removeLayer(int)
     */
    @Override
    public BoardLayer removeLayer(int index) 
    {
        BoardLayer layer = super.removeLayer(index);
        //fireMapChanged();
        
        return layer;
    }
    
    /**
     * Calls super method, and additionally fires a {@link MapChangedEvent}.
     *
     * @see MultilayerPlane#removeAllLayers
     */
//    @Override
//    public void removeAllLayers() 
//    {
//        super.removeAllLayers();
//        //fireMapChanged();
//    }
    
    /**
     * 
     * @param filename 
     */
    public void setFilename(String filename) 
    {
         
    }
    
    /**
     * Returns width of map in tiles.
     *
     * @return int
     */
    public int getWidth() 
    {
        return bounds.width;
    }

    /**
     * Returns height of map in tiles.
     *
     * @return int
     */
    public int getHeight() 
    {
        return bounds.height;
    }
    
    public void performZoomIn()
    {
        boardView.zoomIn();
    }
    
    public void performZoomOut()
    {
        boardView.zoomOut();
    }
    
    public void toogleGrid(boolean isVisible)
    {
        boardView.setShowGrid(isVisible);
    }
}
