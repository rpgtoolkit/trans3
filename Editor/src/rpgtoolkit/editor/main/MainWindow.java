package rpgtoolkit.editor.main;

import java.awt.*;
import java.io.File;
import java.util.ArrayList;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import rpgtoolkit.common.types.Animation;
import rpgtoolkit.common.types.Project;
import rpgtoolkit.editor.animation.AnimationEditor;
import rpgtoolkit.editor.board.BoardEditor;
import rpgtoolkit.editor.main.menus.MainMenuBar;
import rpgtoolkit.editor.main.menus.MainToolBar;
import rpgtoolkit.editor.project.ProjectEditor;
import rpgtoolkit.editor.tile.TileEditor;
import uk.co.tkce.toolkit.test.*;

/**
 * Currently opening Tilesets, tiles, programs, boards, animations, characters
 * etc.
 * 
 * @author Geoff Wilson
 * @author Joshua Michael Daly
 */
public class MainWindow extends JFrame
{
    private TileEditor tileEditor;
    private JDesktopPane desktopPane;
    private JPanel debugPane;
    private JTextField debugLog;
    private Project activeProject;
    private MainToolBar toolBar;
    private JFileChooser fileChooser;
    private final String workingDir = System.getProperty("user.dir");
    private ArrayList<ToolkitEditorWindow> activeWindows;

    public MainWindow()
    {
        super("RPG Toolkit");

        activeWindows = new ArrayList();

        class DesktopPaneWithBackground extends JDesktopPane
        {
            public DesktopPaneWithBackground()
            {
                super();
            }

            @Override
            protected void paintComponent(Graphics g)
            {
                super.paintComponent(g);
                //ImageIcon icon = new ImageIcon(getClass().getResource("/uk.co.tkce.toolkit/assets/palm.png"));
                //g.drawImage(icon.getImage(),0,0,this);
            }
        }

        desktopPane = new DesktopPaneWithBackground();
        desktopPane.setDesktopManager(new ToolkitDesktopManager(this));
        desktopPane.setBackground(Color.LIGHT_GRAY);

        ImageIcon icon = new ImageIcon(getClass()
                .getResource("/rpgtoolkit/editor/resources/application.png"));
        this.setIconImage(icon.getImage());

        debugPane = new JPanel();
        debugLog = new JTextField("Debug Messages:");
        debugLog.setEditable(false);
        debugLog.setFocusable(false);

        debugLog.setText(System.getProperty("user.dir"));

        debugPane.setLayout(new BorderLayout());
        debugPane.add(debugLog, BorderLayout.CENTER);

        this.setLayout(new BorderLayout());

        fileChooser = new JFileChooser();
        fileChooser.setCurrentDirectory(new File(System.getProperty("user.dir")));

        toolBar = new MainToolBar(this);

//        JPanel fileBrowser = new JPanel();
//        fileBrowser.setPreferredSize(new Dimension(150, 10));
//        fileBrowser.setMaximumSize(new Dimension(175, 10));
//        fileBrowser.setLayout(new BorderLayout());
//        DefaultMutableTreeNode projectNode = new DefaultMutableTreeNode("NULL");
//        JTree files = new JTree(projectNode);
//
//        fileBrowser.add(files, BorderLayout.CENTER);
//
//        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, fileBrowser, desktopPane);
//        splitPane.setDividerSize(10);

        this.add(toolBar, BorderLayout.PAGE_START);
        this.add(desktopPane, BorderLayout.CENTER);
        this.add(debugPane, BorderLayout.PAGE_END);
        
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setExtendedState(JFrame.MAXIMIZED_BOTH);
        this.setJMenuBar(new MainMenuBar(this));
        this.setSize(new Dimension(1024, 768));
        this.setLocationByPlatform(true);
        this.setVisible(true);
    }

    public void runGame()
    {
        
    }

    public void newProject()
    {
        ProjectEditor pe = new ProjectEditor();
        desktopPane.add(pe, BorderLayout.CENTER);
    }

    public void openProject()
    {
        FileNameExtensionFilter filter = 
                new FileNameExtensionFilter("Toolkit Project", "gam");
        fileChooser.setFileFilter(filter);

        if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION)
        {
            activeProject = new Project(fileChooser.getSelectedFile());
            ProjectEditor projectEditor = new ProjectEditor(activeProject);
            this.setTitle(this.getTitle() + " - " + activeProject.getGameTitle() + 
                    " project loaded");
            desktopPane.add(projectEditor, BorderLayout.CENTER);
            projectEditor.setWindowParent(this);
            activeWindows.add(projectEditor);
            toolBar.enableRun();
        }
    }
    
    public void openFile()
    {
       // FileNameExtensionFilter filter = 
       //         new FileNameExtensionFilter("Supported Files", "*.*");
       //fileChooser.setFileFilter(filter);
        
        if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION)
        {
            this.checkFileExtension(fileChooser.getSelectedFile());
        }
    }
    
    public void checkFileExtension(File file)
    {
        String fileName = file.getName().toLowerCase();
        
        if (fileName.endsWith(".anm"))
        {
            this.openAnimation();
        }
        else if (fileName.endsWith(".brd"))
        {
            this.openBoardForView();
        }
        else if (fileName.endsWith(".prg"))
        {
            this.openProgram();
        }
        else if (fileName.endsWith(".tst"))
        {
            this.openTilesetForView();
        }
    }

    public void newProgram()
    {
        ProgramEditor testCodeEditor = new ProgramEditor();
        desktopPane.add(testCodeEditor);
    }

    public void openProgram()
    {
        ProgramEditor testCodeEditor = new ProgramEditor();
        testCodeEditor.open(fileChooser.getSelectedFile());
        desktopPane.add(testCodeEditor);
    }

    /**
     * Creates a new animation editor window
     */
    public void newAnimation()
    {

    }

    /**
     * Creates an animation editor window for modifying the specified animation file.
     */
    public void openAnimation()
    {
        Animation animation = new Animation(fileChooser.getSelectedFile());
        AnimationEditor animationEditor = new AnimationEditor(animation);
        desktopPane.add(animationEditor);
    }

    public void openBoard()
    {
        BoardEditor boardEditor = new BoardEditor(this, fileChooser.getSelectedFile());
    }

    /**
     * Creates a new item editor window
     */
    public void newItem()
    {

    }

    /**
     * Creates an item editor window for modifying the specified item file.
     */
    public void openItem()
    {
        
    }

    /**
     * Creates a new Tile (Tileset) editor window
     */
    public void newTileset()
    {

    }

    /**
     * Creates a tileset editor window for modifying the specified tilset
     */
    public void openTileset()
    {
        TileEditor testTileEditor = new TileEditor(fileChooser.getSelectedFile());
        desktopPane.add(testTileEditor);
    }

    public void openTilesetForView()
    {
        TilesetViewer testTileEditor = new TilesetViewer(fileChooser.getSelectedFile());
        desktopPane.add(testTileEditor);
    }

    public void openBoardForView()
    {
        BoardEditor testBoardEditor = new BoardEditor(this, fileChooser.getSelectedFile());
        testBoardEditor.setVisible(true);
        desktopPane.add(testBoardEditor);
    }

    /**
     * Creates a new character (player) editor window
     */
    public void newCharacter()
    {

    }

    /**
     * Creates an character editor window for modifying the specified character file.
     */
    public void openCharacter()
    {
        PlayerEditor testCharEditor = new PlayerEditor();
        testCharEditor.open(fileChooser.getSelectedFile());
        desktopPane.add(testCharEditor);
    }

    public void newSpecialMove()
    {

    }

    public void openSpecialMove()
    {
        SpecialMoveEditor testSpecialEditor = new SpecialMoveEditor();
        testSpecialEditor.open(fileChooser.getSelectedFile());
        desktopPane.add(testSpecialEditor);
    }

    public void newStatusEffect()
    {

    }

    public void openStatusEffect()
    {
        StatusEffectEditor testEffectEditor = new StatusEffectEditor();
        testEffectEditor.open(fileChooser.getSelectedFile());
        desktopPane.add(testEffectEditor);
    }

    public void showHelpMenu()
    {
        HelpViewer helpViewer = new HelpViewer();
        desktopPane.add(helpViewer);
    }

    public void showAbout()
    {
        AboutDialog about = new AboutDialog(this);
    }

    public boolean saveAll()
    {
        for (ToolkitEditorWindow activeWindow : activeWindows)
        {
            System.out.println("Saving: " + activeWindow.toString());
            if (!activeWindow.save())
            {
                System.out.println("Failed to Save All");
                return false;
            }
        }
        return true;
    }

    public void removeActiveWindow(ToolkitEditorWindow window)
    {
        activeWindows.remove(window);
    }
    
    public void zoomInOnBoardEditor()
    {
        if (desktopPane.getSelectedFrame() instanceof BoardEditor)
        {
            BoardEditor editor = (BoardEditor)desktopPane.getSelectedFrame();
            editor.zoomIn();
        }
    }
    
    public void zoomOutOnBoardEditor()
    {
        if (desktopPane.getSelectedFrame() instanceof BoardEditor)
        {
            BoardEditor editor = (BoardEditor)desktopPane.getSelectedFrame();
            editor.zoomOut();
        }
    }
    
    public void toogleGridOnBoardEditor(boolean isVisible)
    {
        if (desktopPane.getSelectedFrame() instanceof BoardEditor)
        {
            BoardEditor editor = (BoardEditor)desktopPane.getSelectedFrame();
            editor.toogleGrid(isVisible);
        }
    }
    
    public void toogleCoordinatesOnBoardEditor(boolean isVisible)
    {
        if (desktopPane.getSelectedFrame() instanceof BoardEditor)
        {
            BoardEditor editor = (BoardEditor)desktopPane.getSelectedFrame();
            editor.toogleCoordinates(isVisible);
        }
    }
}
