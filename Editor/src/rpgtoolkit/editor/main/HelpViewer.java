package rpgtoolkit.editor.main;

import javax.swing.*;

public class HelpViewer extends JInternalFrame
{
    private JTextPane textArea;

    public HelpViewer()
    {
        super("Help Index", true, true, true, true);

        try
        {
            textArea = new JTextPane();
            textArea.setPage(getClass().getResource("/uk/co/tkce/toolkit/html/index.html"));
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

        this.add(textArea);
        this.setSize(640, 480);
        this.setVisible(true);

    }
}
