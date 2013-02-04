package uk.co.tkce.toolkit.test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class CodeTextArea extends JEditorPane implements KeyListener
{
    JPopupMenu rclickMenu = new JPopupMenu();
    JComboBox testField;
    String testString = "";

    public CodeTextArea()
    {
        super();

        try
        {
            Font menschFont = new Font("Mensch", Font.PLAIN, 12);
            this.setFont(menschFont);
        }
        catch (Exception e)
        {
            System.out.println("Failed to load default font!");
            this.setFont(new Font("Courier New", 0, 14));
        }

        rclickMenu = new JPopupMenu("CHEESE");
        this.setComponentPopupMenu(rclickMenu);
        this.addKeyListener(this);
    }

    public void keyTyped(KeyEvent e)
    {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    public void keyPressed(KeyEvent e)
    {
        if (!e.isActionKey())
        {
            testString += e.getKeyChar();
            if (testString.equals("mw"))
            {
                String[] testStrArray = {"mwin( var$ );", "mwin(\" \");", "mwincls();"};
                testField = new JComboBox(testStrArray);
                rclickMenu.add(testField);
                Point location = this.getCaret().getMagicCaretPosition();
                SwingUtilities.convertPointToScreen(location, this);
                location.translate(15, -15);
                rclickMenu.setLocation(location);
                rclickMenu.setVisible(true);
                rclickMenu.grabFocus();
                testString = "";
            }
            System.out.println((int) e.getKeyChar());
        }
        char code = e.getKeyChar();
        if ((int) code == 10)
        {
            testString = "";
        }
    }

    public void keyReleased(KeyEvent e)
    {
        //To change body of implemented methods use File | Settings | File Templates.
    }
}
