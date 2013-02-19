package rpgtoolkit.editor.main.menus;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import javax.swing.ImageIcon;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import rpgtoolkit.editor.main.MainWindow;

/**
 *
 * @author Joshua Michael Daly
 */
public final class HelpMenu extends JMenu
{
    private final MainWindow parent;
    
    private JMenuItem indexMenuItem;
    private JMenuItem aboutMenuItem;
    
    public HelpMenu(MainWindow parent)
    {
        super("Help");
        
        this.parent = parent;
        this.setMnemonic(KeyEvent.VK_H);
        
        this.configureIndexMenuItem();
        this.configureAboutMenuItem();
        
        this.add(indexMenuItem);
        this.add(aboutMenuItem);
    }
    
    public void configureIndexMenuItem()
    {
        indexMenuItem = new JMenuItem("Index");     // Help Index Menu (browser based?)
        indexMenuItem.setIcon(new ImageIcon(getClass()
                .getResource("/rpgtoolkit/editor/resources/help.png")));
        indexMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.showHelpMenu();
            }
        });
        
        indexMenuItem.setEnabled(false);
    }
    
    public void configureAboutMenuItem()
    {
        aboutMenuItem = new JMenuItem("About");     // About Menu
        aboutMenuItem.setIcon(new ImageIcon(getClass()
                .getResource("/rpgtoolkit/editor/resources/information.png")));
        aboutMenuItem.setMnemonic(KeyEvent.VK_A);
        aboutMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.showAbout();

            }
        });
        
        aboutMenuItem.setEnabled(false);
    }
}
