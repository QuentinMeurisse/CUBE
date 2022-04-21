package tiling.gui;

import util.Tiling;


import javax.swing.JPanel;
import java.awt.*;

public abstract class TilingPanel extends JPanel{

  protected Tiling grid;

  public abstract void drawCell(int i, int j, Graphics g, Color c);
  public abstract void setTiling(Tiling t);

  public TilingPanel(Tiling grid){
    super();
    this.grid = grid;
  }

  @Override
  protected void paintComponent(Graphics g){
    super.paintComponent(g);
    for(int i = 0; i < grid.getHeight(); i++){
      for(int j = 0; j < grid.getWidth(); j++){
        int cellColor = grid.getColorOfCorrespondingLetter(i, j);
        Color colorToDraw = Color.WHITE;
        switch(cellColor){
          case -1:
            colorToDraw = Color.GREEN;
            break;
          case 0:
            break;
          case 1:
            colorToDraw = Color.RED;
            break;
          case 2:
            colorToDraw = new Color(1, 114, 130);
            break;
          case 3:
            colorToDraw = new Color(244, 124, 4);
            break;
          case 4:
            colorToDraw = new Color(234, 234, 14);
            break;
          case 5:
            colorToDraw = Color.CYAN;
            break;
          case 6:
            colorToDraw = new Color(167, 6, 6);
            break;
          case 7:
            colorToDraw = new Color(144, 146, 242);
            break;
          case 8:
            colorToDraw = new Color(124, 105, 172);
            break;
          case 9:
            colorToDraw = new Color(0, 29, 85);
            break;
          case 10:
            colorToDraw = new Color(255, 86, 58);
            break;
          case 11:
            colorToDraw = new Color(161, 139, 129);
            break;
          case 12:
            colorToDraw = new Color(255, 254, 98);
            break;
          case 13:
            colorToDraw = new Color(107, 3, 40);
            break;
          case 14:
            colorToDraw = new Color(0, 183, 171);
            break;
          case 15:
            colorToDraw = new Color(255, 182, 46);
            break;
          case 16:
            colorToDraw = new Color(204, 0, 255);
            break;
          case 17:
            colorToDraw = new Color(99, 154, 255);
            break;
          case 20:
            colorToDraw = new Color(81, 62, 51);
            break;
          default:
            colorToDraw = Color.DARK_GRAY;
            break;
        }
        drawCell(i, j, g, colorToDraw);
      }
    }
  }
}
