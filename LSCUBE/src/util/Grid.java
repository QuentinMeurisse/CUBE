package util;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class Grid extends JPanel{

  private int[][] grid;

  public Grid() {
    super();
    grid = new int[10][10];
  }

  @Override
  protected void paintComponent(Graphics g){
    super.paintComponent(g);
    int height = grid.length;
    int width = grid[0].length;
    for(int i = 0; i < height; i++){
      for(int j = 0; j < width; j++){
        int x = grid[i][j];
        switch(x){
          case 0:
            g.setColor(Color.WHITE);
            break;
          case 1:
            g.setColor(Color.RED);
            break;
          case 2:
            g.setColor(Color.GREEN);
            break;
          case 3:
            g.setColor(new Color(244, 124, 4));
            break;
          case 4:
            g.setColor(new Color(234, 234, 14));
            break;
          case 5:
            g.setColor(Color.CYAN);
            break;
          case 6:
            g.setColor(Color.LIGHT_GRAY);
            break;
          case 7:
            g.setColor(Color.BLUE);
            break;
          case 8:
            g.setColor(new Color(144, 146, 242));
            break;
          case 9:
            g.setColor(Color.PINK);
            break;
          case 10:
            g.setColor(new Color(167, 6, 6));
            break;
          default:
            g.setColor(Color.DARK_GRAY);
            break;

        }
        int cellX = 10 + j * 13;
        int cellY = 10 + i * 13;
        g.fillRect(cellX, cellY, 13, 13);
      }
    }
    g.setColor(Color.BLACK);
    g.drawRect(10, 10, 13 * width, 13 * height);

    for(int i = 10; i <= 13 * width; i += 13){
      g.drawLine(i, 10, i, 13 * height + 10);
    }

    for(int i = 10; i <= 13 * height; i += 13){
      g.drawLine(10, i, 13 * width + 10, i);
    }
  }

  public void fillGrid(int[][] grid){
    this.grid = grid;

  }

}

