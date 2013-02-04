package rpgtoolkit.editor.main;

import rpgtoolkit.editor.main.MainWindow;
import javax.swing.*;

public class AboutDialog extends JDialog
{
    public AboutDialog(MainWindow parent)
    {
        super(parent, "About", true);

        GroupLayout layout = new GroupLayout(this.getContentPane());
        this.getContentPane().setLayout(layout);
        layout.setAutoCreateGaps(true);
        layout.setAutoCreateContainerGaps(true);

        JLabel logo = new JLabel();
        logo.setIcon(new ImageIcon(getClass().getResource("/uk/co/tkce/toolkit/assets/application.png")));
        JLabel version = new JLabel("version 1.0.0_svn");

        layout.setHorizontalGroup(layout.createParallelGroup(GroupLayout.Alignment.CENTER)
                .addComponent(logo)
                .addComponent(version)
        );

        layout.setVerticalGroup(layout.createSequentialGroup()
                .addComponent(logo)
                .addComponent(version)
        );

        this.setSize(300, 200);
        this.setLocationRelativeTo(parent);
        this.setResizable(false);

        ImageIcon icon = new ImageIcon(getClass().getResource("/uk/co/tkce/toolkit/assets/application.png"));
        this.setIconImage(icon.getImage());

        this.setVisible(true);
    }
}
