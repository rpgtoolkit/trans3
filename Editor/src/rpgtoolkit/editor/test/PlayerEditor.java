package uk.co.tkce.toolkit.test;

import rpgtoolkit.common.types.Player;

import javax.swing.*;
import java.io.File;

public class PlayerEditor extends JInternalFrame
{
    private Player player;

    public PlayerEditor()
    {
        super("New Player", true, true, true, true);
        this.setSize(600, 400);
        this.setVisible(true);
    }

    public void open(File fileName)
    {
        player = new Player(fileName);
        this.setTitle("Player: " + fileName.getName());
    }
}