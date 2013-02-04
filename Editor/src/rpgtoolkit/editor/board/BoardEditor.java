package rpgtoolkit.editor.board;

import java.awt.Dimension;
import java.io.File;
import javax.swing.*;
import rpgtoolkit.editor.main.MainWindow;

/**
 * 
 * @author Geoff Wilson
 * @author Joshua Michael Daly
 */
public class BoardEditor extends JInternalFrame
{
    private MainWindow parent;

    private JScrollPane scroll;
    
    private BoardController boardController;

    public BoardEditor()
    {
        this.setupWindow();
    }

    public BoardEditor(MainWindow parent, File fileName)
    {
        super("Board Viewer", true, true, true, true);
        
        this.parent = parent;
        this.boardController = new BoardController(fileName);
        
        this.setTitle("Viewing " + fileName.getAbsolutePath());
        
        this.scroll = new JScrollPane(boardController.getBoardView());
        scroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
     
        this.add(scroll);
        this.setupWindow();
    }
    
    public BoardController getBoardController()
    {
        return this.boardController;
    }

    private void setupWindow()
    {
        Dimension canvasDimensions = this.boardController.
                getBoardView().getPreferredSize();        
        this.setSize((int)canvasDimensions.getWidth(), (int)canvasDimensions.getHeight());
    }
    
    public void zoomIn()
    {
        boardController.performZoomIn();
    }
    
    public void zoomOut()
    {
        boardController.performZoomOut();
    }
}
