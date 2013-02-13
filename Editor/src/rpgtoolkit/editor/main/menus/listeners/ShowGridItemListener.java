package rpgtoolkit.editor.main.menus.listeners;

import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import javax.swing.JCheckBoxMenuItem;
import rpgtoolkit.editor.main.MainWindow;

/**
 *
 * @author Joshua Michael Daly
 */
public class ShowGridItemListener implements ItemListener
{
    private MainWindow parent;

    /**
     * 
     */
    public ShowGridItemListener()
    {
        
    }
    
    /**
     * 
     * @param parent 
     */
    public ShowGridItemListener(MainWindow parent)
    {
        this.parent = parent;
    }

    /**
     * 
     * @param ie 
     */
    @Override
    public void itemStateChanged(ItemEvent ie) 
    {
        JCheckBoxMenuItem showGridMenuItem = (JCheckBoxMenuItem)ie.getItem();
        
        if (showGridMenuItem.isSelected())
        {
            parent.toogleGridOnBoardEditor(true);
        }
        else
        {
            parent.toogleGridOnBoardEditor(false);
        }
    }
}
