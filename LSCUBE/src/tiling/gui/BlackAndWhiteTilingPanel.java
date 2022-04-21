package tiling.gui;

import util.Tiling;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

public abstract class BlackAndWhiteTilingPanel extends JPanel{

  protected Tiling grid;

  public abstract void drawCell(int i, int j, Graphics2D g2, Paint paint);
  public abstract void setTiling(Tiling t);

  public BlackAndWhiteTilingPanel(Tiling grid){
    super();
    this.grid = grid;
  }

  @Override
  protected void paintComponent(Graphics g){
    super.paintComponent(g);
    for(int i = 0; i < grid.getHeight(); i++){
      for(int j = 0; j < grid.getWidth(); j++){
        int cellColor = grid.getColorOfCorrespondingLetter(i, j);
        Paint pattern = Color.WHITE;
        BufferedImage image = new BufferedImage(16,16,
                BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = image.createGraphics();
        switch(cellColor){
          case -1://+ cross
            g2d.setColor(Color.BLACK);
            g2d.setStroke(new BasicStroke(2));
            g2d.drawLine(8, 4, 8, 12);
            g2d.drawLine(4, 8, 12, 8);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 0:
            break;
          case 1:
            pattern = Color.GRAY;
            break;
          case 2:// light gray circle
            g2d.setColor(Color.LIGHT_GRAY);
            g2d.fillOval(4, 4, 8, 8);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 3:
            pattern = Color.LIGHT_GRAY;
            break;
          case 4: //vertical stripes
            g2d.setColor(Color.GRAY);
            g2d.drawLine(4, 0, 4, 16);
            g2d.drawLine(8, 0, 8, 16);
            g2d.drawLine(12, 0, 12, 16);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 5: //horizontal stripes
            g2d.setColor(Color.GRAY);
            g2d.drawLine(0, 4, 16, 4);
            g2d.drawLine(0, 8, 16, 8);
            g2d.drawLine(0, 12, 16, 12);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 6: //diagonal stripes (left top to right bottom)
            g2d.setColor(Color.GRAY);
            g2d.drawLine(0, 0, 16, 16);
            g2d.drawLine(4, 0, 16, 12);
            g2d.drawLine(8, 0, 16, 8);
            g2d.drawLine(12, 0, 16, 4);
            g2d.drawLine(0, 4, 12, 16);
            g2d.drawLine(0, 8, 8, 16);
            g2d.drawLine(0, 12, 4, 16);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 7: //diagonal stripes (right top to left bottom)
            g2d.setColor(Color.GRAY);
            g2d.drawLine(16, 0, 0, 16);
            g2d.drawLine(12, 0, 0, 12);
            g2d.drawLine(8, 0, 0, 8);
            g2d.drawLine(4, 0, 0, 4);
            g2d.drawLine(16, 4, 4, 16);
            g2d.drawLine(16, 8, 8, 16);
            g2d.drawLine(16, 12, 12, 16);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 8: //vertical light gray diamond
            int[] x = {8, 12, 8, 4};
            int[] y = {2, 8, 14, 8};
            g2d.setColor(Color.LIGHT_GRAY);
            g2d.fillPolygon(x, y, 4);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 9: //horizontal light gray diamond
            int[] x2 = {8, 14, 8, 2};
            int[] y2 = {4, 8, 12, 8};
            g2d.setColor(Color.LIGHT_GRAY);
            g2d.fillPolygon(x2, y2, 4);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 10: //light gray top square
            int[] x3 = {8, 16, 8, 0};
            int[] y3 = {0, 8, 16, 8};
            g2d.setColor(Color.LIGHT_GRAY);
            g2d.fillPolygon(x3, y3, 4);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 11: //gray circle
            g2d.setColor(Color.GRAY);
            g2d.fillOval(4, 4, 8, 8);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 12: //gray vertical diamond
            int[] x4 = {8, 12, 8, 4};
            int[] y4 = {2, 8, 14, 8};
            g2d.setColor(Color.GRAY);
            g2d.fillPolygon(x4, y4, 4);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 13: //gray horizontal diamond
            int[] x5 = {8, 14, 8, 2};
            int[] y5 = {4, 8, 12, 8};
            g2d.setColor(Color.GRAY);
            g2d.fillPolygon(x5, y5, 4);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 14: //gray top square
            int[] x6 = {8, 16, 8, 0};
            int[] y6 = {0, 8, 16, 8};
            g2d.setColor(Color.GRAY);
            g2d.fillPolygon(x6, y6, 4);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 15: //dark gray circle
            g2d.setColor(Color.DARK_GRAY);
            g2d.fillOval(4, 4, 8, 8);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 16: //dark gray horizontal diamond
            int[] x7 = {8, 14, 8, 2};
            int[] y7 = {4, 8, 12, 8};
            g2d.setColor(Color.DARK_GRAY);
            g2d.fillPolygon(x7, y7, 4);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 17: //dark gray top square
            int[] x8 = {8, 16, 8, 0};
            int[] y8 = {0, 8, 16, 8};
            g2d.setColor(Color.DARK_GRAY);
            g2d.fillPolygon(x8, y8, 4);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          case 20: // X cross
            g2d.setColor(Color.BLACK);
            g2d.setStroke(new BasicStroke(2));
            g2d.drawLine(4, 4, 12, 12);
            g2d.drawLine(12, 4, 4, 12);
            pattern = new TexturePaint(image, new Rectangle(10, 10, 16, 16));
            break;
          default:
            pattern = Color.DARK_GRAY;
            break;
        }
        drawCell(i, j, (Graphics2D) g, pattern);
      }
    }
  }

}
