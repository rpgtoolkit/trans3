package rpgtoolkit.editor.main;

import javax.swing.*;

public class Driver
{
    public static void main(String[] args)
    {
        try
        {
            System.out.println(System.getProperty("os.name"));
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            MainWindow mainWindow = new MainWindow();
        }
        catch (Exception e)
        {
            
        }
    }
}
