package rpgtoolkit.editor.main.menus;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import javax.swing.ImageIcon;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;
import rpgtoolkit.editor.main.MainWindow;
import rpgtoolkit.editor.main.menus.actions.ZoomInAction;
import rpgtoolkit.editor.main.menus.actions.ZoomOutAction;

/**
 *
 * @author Joshua Michael Daly
 */
public final class ViewMenu extends JMenu
{
    private final MainWindow parent;
    
    private JMenuItem zoomInMenuItem;
    private JMenuItem zoomOutMenuItem;
    private JCheckBoxMenuItem showGridMenuItem;
    private JMenuItem projectExplorerMenuItem;
    
    public ViewMenu(MainWindow parent)
    {
        super("View");
        
        this.parent = parent;
        this.setMnemonic(KeyEvent.VK_V);
        
        this.configureZoomInMenuItem();
        this.configureZoomOutMenuItem();
        this.configureProjectExplorerMenuItem();
        
        this.add(zoomInMenuItem);
        this.add(zoomOutMenuItem);
        this.add(projectExplorerMenuItem);
    }
    
    public void configureZoomInMenuItem()
    {
        zoomInMenuItem = new JMenuItem("Zoom In");
        zoomInMenuItem.setIcon(new ImageIcon(getClass()
                .getResource("/rpgtoolkit/editor/resources/zoom-in.png")));
        zoomInMenuItem.setAccelerator(
                KeyStroke.getKeyStroke(KeyEvent.VK_PLUS, ActionEvent.CTRL_MASK));
        zoomInMenuItem.setMnemonic(KeyEvent.VK_PLUS);
        zoomInMenuItem.addActionListener(new ZoomInAction(parent));
    }
    
    public void configureZoomOutMenuItem()
    {
        zoomOutMenuItem = new JMenuItem("Zoom Out");
        zoomOutMenuItem.setIcon(new ImageIcon(getClass()
                .getResource("/rpgtoolkit/editor/resources/zoom-out.png")));
        zoomOutMenuItem.setAccelerator(
                KeyStroke.getKeyStroke(KeyEvent.VK_M, ActionEvent.CTRL_MASK));
        zoomOutMenuItem.setMnemonic(KeyEvent.VK_MINUS);
        zoomOutMenuItem.addActionListener(new ZoomOutAction(parent));
    }
    
    public void configureShowGridMenuItem()
    {
        zoomOutMenuItem = new JMenuItem("Zoom Out");
        zoomOutMenuItem.setIcon(new ImageIcon(getClass()
                .getResource("/rpgtoolkit/editor/resources/grid.png")));
        zoomOutMenuItem.setAccelerator(
                KeyStroke.getKeyStroke(KeyEvent.VK_M, ActionEvent.CTRL_MASK));
        zoomOutMenuItem.setMnemonic(KeyEvent.VK_MINUS);
        zoomOutMenuItem.addActionListener(new ZoomOutAction(parent));
    }
    
    public void configureProjectExplorerMenuItem()
    {
        projectExplorerMenuItem = new JCheckBoxMenuItem("Show Grid");
        projectExplorerMenuItem.setIcon(new ImageIcon(getClass()
                .getResource("/rpgtoolkit/editor/resources/project-explorer.png")));
        projectExplorerMenuItem.setAccelerator(
                KeyStroke.getKeyStroke(KeyEvent.VK_E, ActionEvent.CTRL_MASK));
        projectExplorerMenuItem.setMnemonic(KeyEvent.VK_P);
    }
}
