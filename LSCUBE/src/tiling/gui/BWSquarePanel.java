package tiling.gui;

import util.SquareTiling;
import util.Tiling;

import java.awt.*;

public class BWSquarePanel extends BlackAndWhiteTilingPanel{

  public BWSquarePanel(Tiling grid){
    super(grid);
  }

  @Override
  public void drawCell(int i, int j, Graphics2D g2, Paint paint){
    g2.setPaint(paint);
    int cellX = 10 + j * 16;
    int cellY = 10 + i * 16;
    g2.fillRect(cellX, cellY, 16, 16);
    g2.setColor(Color.BLACK);
    g2.drawRect(cellX, cellY, 16, 16);
  }

  @Override
  public void setTiling(Tiling t){
    grid = (SquareTiling) t;

  }
}
