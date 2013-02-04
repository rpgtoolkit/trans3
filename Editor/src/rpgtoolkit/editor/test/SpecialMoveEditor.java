package uk.co.tkce.toolkit.test;

import rpgtoolkit.common.types.SpecialMove;

import javax.swing.*;
import java.io.File;

public class SpecialMoveEditor extends JInternalFrame
{
    private SpecialMove specialMove;

    public SpecialMoveEditor()
    {
        super("New Special Move", true, true, true, true);
        this.setSize(600, 400);
        this.setVisible(true);
    }

    public void open(File fileName)
    {
        specialMove = new SpecialMove(fileName);
        this.setTitle("Special Move: " + fileName.getName());
    }
}