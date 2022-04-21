package util;

import javax.swing.*;
import java.awt.*;

public class OutputFrame extends JFrame{

  private JLabel label;
  private Grid grid;

  public OutputFrame(){
    super("Output");
    setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    setSize(1900, 1000);
    label = new JLabel("Search will start");
    label.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 16) );
    grid = new Grid();
    setLayout(new BorderLayout());
    JPanel labelPanel = new JPanel();
    labelPanel.setPreferredSize(new Dimension(500, 1000));
    labelPanel.add(label);
    add(grid, BorderLayout.CENTER);
    add(labelPanel, BorderLayout.EAST);
    setVisible(true);
  }

  public void setText(String newText){
    label.setText(newText);
  }

  public void setGrid(int[][] newGird){
    grid.fillGrid(newGird);
    repaint();
  }

  public void finished(){
    label.setForeground(Color.GREEN);
  }

}
