package uk.co.tkce.toolkit.test;

import rpgtoolkit.common.types.StatusEffect;

import javax.swing.*;
import java.io.File;

public class StatusEffectEditor extends JInternalFrame
{
    private StatusEffect effect;

    public StatusEffectEditor()
    {
        super("New Status Effect", true, true, true, true);
        this.setSize(600, 400);
        this.setVisible(true);
    }

    public void open(File fileName)
    {
        effect = new StatusEffect(fileName);
        this.setTitle("Status Effect: " + fileName.getName());
    }
}
