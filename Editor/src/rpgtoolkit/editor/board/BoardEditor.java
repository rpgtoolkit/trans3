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

    private JScrollPane scrollPane;
    
    private BoardController boardController;

    public BoardEditor()
    {

    }

    public BoardEditor(MainWindow parent, File fileName)
    {
        super("Board Viewer", true, true, true, true);
        
        this.parent = parent;
        this.boardController = new BoardController(fileName);
        this.setTitle("Viewing " + fileName.getAbsolutePath());
        
        this.scrollPane = new JScrollPane(boardController.getBoardView());
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
     
        this.add(scrollPane);
        this.pack();
    }
    
    public BoardController getBoardController()
    {
        return this.boardController;
    }
    
    public void zoomIn()
    {
        boardController.performZoomIn();
        scrollPane.getViewport().revalidate();
    }
    
    public void zoomOut()
    {
        boardController.performZoomOut();
        scrollPane.getViewport().revalidate();
    }
    
    public void toogleGrid(boolean isVisible)
    {
        boardController.toogleGrid(isVisible);
    }
    
    public void toogleCoordinates(boolean isVisible)
    {
        boardController.toogleCoordinates(isVisible);
    }
}
