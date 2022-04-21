package tiling.gui;

import util.Tiling;

import javax.swing.*;
import java.awt.*;

public class BWFrame extends JFrame{

  private BlackAndWhiteTilingPanel grid;

  public BWFrame(BlackAndWhiteTilingPanel grid){
    super("Output");
    this.grid = grid;
    setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    setSize(1000, 1000);
    add(grid);
    setVisible(true);
  }

  public void setGrid(Tiling t){
    grid.setTiling(t);
    repaint();
  }
}
