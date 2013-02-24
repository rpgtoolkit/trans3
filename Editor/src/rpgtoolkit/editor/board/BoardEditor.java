package rpgtoolkit.editor.board;

import java.io.File;
import javax.swing.JInternalFrame;
import javax.swing.JScrollPane;
import rpgtoolkit.common.io.types.Board;
import rpgtoolkit.common.utilities.TileSetCache;
import rpgtoolkit.editor.main.MainWindow;
import rpgtoolkit.editor.main.menus.actions.ZoomInAction;
import rpgtoolkit.editor.main.menus.actions.ZoomOutAction;

/**
 * 
 * @author Geoff Wilson
 * @author Joshua Michael Daly
 */
public final class BoardEditor extends JInternalFrame
{
    private TileSetCache tileSetCache;
    
    private MainWindow parent;

    private JScrollPane scrollPane;
    
    private Board board;
    private BoardView boardView;
    private ZoomInAction zoomInAction;
    private ZoomOutAction zoomOutAction;

    public BoardEditor()
    {

    }

    public BoardEditor(MainWindow parent, File fileName)
    {
        super("Board Viewer", true, true, true, true);
        
        this.parent = parent;
        
        this.board = new Board(fileName);
        this.configureBoardController();
        this.boardView = new BoardView(this, board);
        this.zoomInAction = new ZoomInAction();
        this.zoomOutAction = new ZoomOutAction();
         
        this.setTitle("Viewing " + fileName.getAbsolutePath());
        
        this.scrollPane = new JScrollPane(boardView);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
     
        this.add(scrollPane);
        this.pack();
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
    
    public void zoomIn()
    {
        boardView.zoomIn();
        scrollPane.getViewport().revalidate();
    }
    
    public void zoomOut()
    {
        boardView.zoomOut();
        scrollPane.getViewport().revalidate();
    }
    
    public void toogleGrid(boolean isVisible)
    {
        boardView.setShowGrid(isVisible);
    }
    
    public void toogleCoordinates(boolean isVisible)
    {
        boardView.setShowCoordinates(isVisible);
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
            boardView.addLayer(layer);
        }
    }
}
