package rpgtoolkit.editor.main.menus;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import rpgtoolkit.editor.main.MainWindow;

/**
 *
 * @author Joshua Michael Daly
 */
public final class ToolsMenu extends JMenu 
{
    private final MainWindow parent;
    
    private JMenuItem boardViewerMenuItem;
    private JMenuItem tileSetViewerMenuItem;
    
    public ToolsMenu(MainWindow parent)
    {
        super("Tools");
        
        this.parent = parent;
        this.setMnemonic(KeyEvent.VK_T); 
        
        this.configureBoardViewerMenuItem();
        this.configureTileSetViewerMenuItem();
        
        this.add(boardViewerMenuItem);
        this.add(tileSetViewerMenuItem);
    }
    
    public void configureBoardViewerMenuItem()
    {
        boardViewerMenuItem = new JMenuItem("Board Viewer");
        boardViewerMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.openBoardForView();
            }
        });
        
        boardViewerMenuItem.setEnabled(false);
    }
    
    public void configureTileSetViewerMenuItem()
    {
        tileSetViewerMenuItem = new JMenuItem("Tileset Viewer");
        tileSetViewerMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.openTilesetForView();
            }
        });
        
        tileSetViewerMenuItem.setEnabled(false);
    }
}
