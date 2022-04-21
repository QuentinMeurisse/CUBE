package tiling.gui;

import util.HexagonAlphabet;
import util.HexagonalTiling;
import util.RootOfUnit;
import util.Tiling;

import java.awt.*;
import static java.lang.Math.cos;
import static java.lang.Math.sin;
import static java.lang.Math.PI;

public class HexagonPanel extends TilingPanel{

  private double lastCenterX = 20;
  private double lastCenterY = 20;

  public HexagonPanel(HexagonalTiling grid){
    super(grid);
  }

  @Override
  public void drawCell(int i, int j, Graphics g, Color c){
    double centerX;
    double centerY = 0;
    int r = 8;
    double h = r * cos(PI/6);
    if(j == 0){
      centerX = 20;
      centerY = 20 + i * 2 * h;
    }
    else{
      centerX = lastCenterX + 2 * h * cos(PI/6) ;
      HexagonAlphabet chr =(HexagonAlphabet) grid.getWord().get(i, j);
      switch(chr){
        case H:
          centerY = lastCenterY + 2 * h * sin(-PI/6);
          break;
        case B:
          centerY = lastCenterY + 2 * h * sin(PI/6);
          break;
      }
    }
    lastCenterX = centerX;
    lastCenterY = centerY;

    int[][] pointCoord = RootOfUnit.nRootOfUnit(6, centerX, centerY, r);
    g.setColor(c);
    g.fillPolygon(pointCoord[0], pointCoord[1], 6);
    g.setColor(Color.BLACK);
    g.drawPolygon(pointCoord[0], pointCoord[1], 6);
  }

  @Override
  public void setTiling(Tiling t){
    grid = (HexagonalTiling) t;

  }
}
