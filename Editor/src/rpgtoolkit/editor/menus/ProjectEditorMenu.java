package rpgtoolkit.editor.menus;

import javax.swing.*;
import rpgtoolkit.editor.main.MainWindow;

public class ProjectEditorMenu extends JMenuBar
{
    private final MainWindow parent;

    public ProjectEditorMenu(MainWindow parent)
    {
        this.parent = parent;

        JMenu projectMenu = new JMenu("PROJECT");
        JMenuItem saveMenu = new JMenuItem("SAVE PROJECT");

        projectMenu.add(saveMenu);
        this.add(projectMenu);
    }
}
