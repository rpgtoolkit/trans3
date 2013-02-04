package rpgtoolkit.editor.main.menus;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;
import rpgtoolkit.editor.main.MainWindow;

/**
 * Consider splitting this class up!
 * 
 * @author Geoff Wilson
 * @author Joshua Michael Daly
 */
public class MainMenuBar extends JMenuBar
{
    private final MainWindow parent;

    private FileMenu fileMenu;
    private EditMenu editMenu;
    private ViewMenu viewMenu;
    private ProjectMenu projectMenu;
    private RunMenu runMenu;
    private ToolsMenu toolsMenu;
    private HelpMenu helpMenu;
    
    // All redundant
    private JMenuItem openProjectMenuItem;
    private JMenuItem openBoardMenuItem;
    private JMenuItem openAnimationMenuItem;
    private JMenuItem openBackgroundMenuItem;
    private JMenuItem openCharMenuItem;
    private JMenuItem openItemMenuItem;
    private JMenuItem openProgramMenuItem;
    private JMenuItem openStatusEffectMenuItem;
    private JMenuItem openSpecialMoveMenuItem;
    private JMenuItem openTilesetMenuItem;

    private JMenu objectMenu;
    private JMenu newObjectMenu;
    private JMenuItem newAnimationMenuItem;
    private JMenuItem newItemMenuItem;
    private JMenuItem newTileMenuItem;
    private JMenuItem newProgramMenuItem;
    private JMenu openObjectMenu;
    
    private JMenu windowsMenu;

    public MainMenuBar(MainWindow menuBarParent)
    {
        super();

        this.parent = menuBarParent;

        fileMenu = new FileMenu(this.parent);
        editMenu = new EditMenu(this.parent);
        viewMenu = new ViewMenu(this.parent);
        projectMenu = new ProjectMenu(this.parent);
        runMenu = new RunMenu(this.parent);
        toolsMenu = new ToolsMenu(this.parent);
        helpMenu = new HelpMenu(this.parent);

        this.add(fileMenu);
        this.add(editMenu);
        this.add(viewMenu);
        this.add(projectMenu);
        this.add(runMenu);
        this.add(toolsMenu);
        this.add(helpMenu);
    }
    
    /**
     * This code will be moved to the Open -> File menu action listener.
     */
    private void configureOpenSubMenuMenu()
    {
        openAnimationMenuItem = new JMenuItem("Animation");
        openAnimationMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.openAnimation();
            }
        });

        openBoardMenuItem = new JMenuItem("Board");
        openBoardMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.openBoard();
            }
        });

        openItemMenuItem = new JMenuItem("Item");
        openItemMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.openItem();
            }
        });

        openCharMenuItem = new JMenuItem("Player");
        openCharMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.openCharacter();
            }
        });

        openProgramMenuItem = new JMenuItem("Program");
        openProgramMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.openProgram();
            }
        });

        openSpecialMoveMenuItem = new JMenuItem("Special Move");
        openSpecialMoveMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.openSpecialMove();
            }
        });

        openStatusEffectMenuItem = new JMenuItem("Status Effect");
        openStatusEffectMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.openStatusEffect();
            }
        });

        openTilesetMenuItem = new JMenuItem("TileSet");
        openTilesetMenuItem.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                parent.openTileset();
            }
        });

        // Add Sub Menus in ALPHABETICAL ORDER
        openObjectMenu.add(openAnimationMenuItem);      // ANIMATION        .ANM
        openObjectMenu.add(openBoardMenuItem);          // BOARD            .BRD
        openObjectMenu.add(openItemMenuItem);           // ITEM             .ITM
        openObjectMenu.add(openCharMenuItem);           // PLAYER           .TEM
        openObjectMenu.add(openProgramMenuItem);        // PROGRAM          .PRG
        openObjectMenu.add(openSpecialMoveMenuItem);    // SPECIAL MOVE     .SPC
        openObjectMenu.add(openStatusEffectMenuItem);   // STATUS EFFECT    .STE
        openObjectMenu.add(openTilesetMenuItem);        // TILESET          .TST
    }
}


