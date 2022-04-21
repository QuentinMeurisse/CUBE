package util;

import static java.lang.Math.PI;
import static java.lang.Math.cos;
import static java.lang.Math.round;
import static java.lang.Math.sin;

public class RootOfUnit{

  public static int[][] nRootOfUnit(int n, double centerX, double centerY, int r){
    int[] xCoord = new int[n];
    int[] yCoord = new int[n];
    for(int k = 0; k < n; k++){
      double x = r * cos((2 * k * PI) / n) + centerX;
      double y = r * sin((2 * k * PI) / n) + centerY;
      xCoord[k] = (int) round(x);
      yCoord[k] = (int) round(y);
    }

    return new int[][]{xCoord, yCoord};
  }

  public static int[][] nRootOfUnitWithRotation(int n, double centerX, double centerY, int r, double angle){
    int[] xCoord = new int[n];
    int[] yCoord = new int[n];
    for(int k = 0; k < n; k++){
      double x = r * cos((2 * k * PI) / n);
      double y = r * sin((2 * k * PI) / n);
      double x2 = x * cos(angle) - y * sin(angle);
      double y2 = x * sin(angle) + y * cos(angle);
      xCoord[k] = (int) round(x2 + centerX);
      yCoord[k] = (int) round(y2 + centerY);
    }

    return new int[][]{xCoord, yCoord};
  }
}