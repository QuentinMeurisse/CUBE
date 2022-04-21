package tiling.gui;

import util.*;

import java.awt.*;

public class SquarePanel extends TilingPanel{


  public SquarePanel(SquareTiling grid){
    super(grid);
  }


  @Override
  public void drawCell(int i, int j, Graphics g, Color c){
    g.setColor(c);
    int cellX = 10 + j * 13;
    int cellY = 10 + i * 13;
    g.fillRect(cellX, cellY, 13, 13);
    g.setColor(Color.BLACK);
    g.drawRect(cellX, cellY, 13, 13);
  }

  @Override
  public void setTiling(Tiling t){
    grid = (SquareTiling) t;
  }

}
