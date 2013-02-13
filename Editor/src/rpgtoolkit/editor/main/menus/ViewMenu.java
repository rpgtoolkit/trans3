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
import rpgtoolkit.editor.main.menus.listeners.ShowGridItemListener;

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
    
    /**
     * 
     * @param parent 
     */
    public ViewMenu(MainWindow parent)
    {
        super("View");
        
        this.parent = parent;
        this.setMnemonic(KeyEvent.VK_V);
        
        this.configureZoomInMenuItem();
        this.configureZoomOutMenuItem();
        this.configureShowGridMenuItem();
        
        this.add(zoomInMenuItem);
        this.add(zoomOutMenuItem);
        this.add(showGridMenuItem);
    }
    
    /**
     * 
     */
    public void configureZoomInMenuItem()
    {
        zoomInMenuItem = new JMenuItem("Zoom In");
        zoomInMenuItem.setIcon(new ImageIcon(getClass()
                .getResource("/rpgtoolkit/editor/resources/zoom-in.png")));
        zoomInMenuItem.setAccelerator(
                KeyStroke.getKeyStroke(KeyEvent.VK_ADD, ActionEvent.CTRL_MASK));
        zoomInMenuItem.setMnemonic(KeyEvent.VK_PLUS);
        zoomInMenuItem.addActionListener(new ZoomInAction(parent));
    }
    
    /**
     * 
     */
    public void configureZoomOutMenuItem()
    {
        zoomOutMenuItem = new JMenuItem("Zoom Out");
        zoomOutMenuItem.setIcon(new ImageIcon(getClass()
                .getResource("/rpgtoolkit/editor/resources/zoom-out.png")));
        zoomOutMenuItem.setAccelerator(
                KeyStroke.getKeyStroke(KeyEvent.VK_SUBTRACT, ActionEvent.CTRL_MASK));
        zoomOutMenuItem.setMnemonic(KeyEvent.VK_MINUS);
        zoomOutMenuItem.addActionListener(new ZoomOutAction(parent));
    }
    
    /**
     * 
     */
    public void configureShowGridMenuItem()
    {
        showGridMenuItem = new JCheckBoxMenuItem("Show Grid");
        showGridMenuItem.setIcon(new ImageIcon(getClass()
                .getResource("/rpgtoolkit/editor/resources/grid.png")));
        showGridMenuItem.setAccelerator(
                KeyStroke.getKeyStroke(KeyEvent.VK_G, ActionEvent.CTRL_MASK));
        showGridMenuItem.setMnemonic(KeyEvent.VK_G);
        showGridMenuItem.addItemListener(new ShowGridItemListener(parent));
    }
}
