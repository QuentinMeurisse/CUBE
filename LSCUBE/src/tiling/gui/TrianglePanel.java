package tiling.gui;

import util.RootOfUnit;
import util.Tiling;
import util.TriangleAlphabet;
import util.TriangularTiling;

import java.awt.*;

import static java.lang.Math.*;

public class TrianglePanel extends TilingPanel{

  private double lastCenterX = 20;
  private double lastCenterY = 20;


  public TrianglePanel(TriangularTiling t){
    super(t);
  }

  @Override
  public void drawCell(int i, int j, Graphics g, Color c){
    double cx, cy = 0, theta = 0;
    int r = 9;
    double l = r * sin(PI/3);
    double a = r * cos(PI/3);
    TriangleAlphabet chr = (TriangleAlphabet) grid.getWord().get(i, j);
    if(j == 0){
      cx = 20;
      int numTriangles = 0;
      int numTrianglesOnTop = 0;

      switch(chr){
        case O:
        case H:
          numTriangles = i/2;
          numTrianglesOnTop = i % 2 == 0 ? i/2 : (i/2)+1;
          theta = -1 * PI/2;
          break;
        case C:
        case B:
          numTriangles = i % 2 == 0 ? i/2 : (i/2)+1;
          numTrianglesOnTop = i/2;
          theta = PI/2;
          break;
      }
      cy = 20 + numTrianglesOnTop * 2 * r + numTriangles * 2 * a;
    }
    else{
      cx = lastCenterX + l;
      switch(chr){
        case O:
        case H:
          cy = lastCenterY + 2 * a * sin(PI/6);
          theta = -1 * PI/2;
          break;
        case C:
        case B:
          cy = lastCenterY + 2 * a * sin(-1*PI/6);
          theta = PI/2;
          break;
      }
    }
    lastCenterX = cx;
    lastCenterY = cy;
    int[][] pointCoord = RootOfUnit.nRootOfUnitWithRotation(3, cx, cy, r, theta);
    switch(chr){
      case O:
        g.setColor(getBackground());
        g.fillPolygon(pointCoord[0], pointCoord[1], 3);
      case C:
        if(j != 0 && grid.getWord().get(i, j -1) == TriangleAlphabet.H){
          g.setColor(Color.BLACK);
          g.drawLine(pointCoord[0][0], pointCoord[1][0], pointCoord[0][1], pointCoord[1][1]);
        }
        break;
      case B:
      case H:
        g.setColor(c);
        g.fillPolygon(pointCoord[0], pointCoord[1], 3);
        g.setColor(Color.BLACK);
        g.drawPolygon(pointCoord[0], pointCoord[1], 3);
        break;
    }

  }


  @Override
  public void setTiling(Tiling t){
    grid = (TriangularTiling) t;
  }
}
