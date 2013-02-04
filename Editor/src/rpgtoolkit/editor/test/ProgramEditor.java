package uk.co.tkce.toolkit.test;

//import jsyntaxpane.DefaultSyntaxKit;

import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class ProgramEditor extends JInternalFrame
{
    CodeTextArea area = new CodeTextArea();
    JScrollPane scrollPane = new JScrollPane();

    public ProgramEditor()
    {
        super("New Program", true, true, true, true);
        this.setSize(600, 400);
        this.setLayout(new BorderLayout());
        //DefaultSyntaxKit.initKit();
        scrollPane = new JScrollPane(area);
        area.setContentType("text/java");
        area.setFont(new Font("Mensch", Font.PLAIN, 12));
        this.add(scrollPane, BorderLayout.CENTER);

        this.setVisible(true);
    }

    public void open(File fileName)
    {
        try
        {
            area.read(new FileInputStream(fileName), this);
            this.setTitle("RPG Code Program: " + fileName.getName());

        }
        catch (IOException e)
        {
            e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
        }

    }
}
