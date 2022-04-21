package util;

import java.util.ArrayList;
import java.util.List;

import static util.TriangleAlphabet.*;

public class TriangularTiling extends Tiling{


  public TriangularTiling(int length, double sideLength){
    height = length;
    width = 2 * length - 1;
    this.sideLength = sideLength;
    generate();
    totalArea = numCells() * getCell(0).getArea();
    init2DWord();
  }

  public TriangularTiling(int width, int height, double sideLength){
    this.height = height;
    this.width = width;
    this.sideLength = sideLength;
    generate();
    totalArea = numCells() * getCell(0).getArea();
    init2DWord();
  }

  @Override
  public int getColorOfCorrespondingLetter(int i, int j){
    if(Math.floor(width/2.) - i <= j && j <= Math.floor(width/2.) + i){
      int index = (int) (i*i + i - (Math.floor(width/2.) - j));
      Cell c = cells.get(index);
      return c.getColor();
    }
    else
      return 0;
  }

  @Override
  public boolean equals(Tiling t){
    if(!(t instanceof  TriangularTiling))
      return false;
    TriangularTiling other = (TriangularTiling) t;
    if(this.width != other.width || this.height != other.height)
      return false;
    for(int x = 0; x < cells.size(); x++){
      if(this.cells.get(x) != other.cells.get(x))
        return false;
    }
    return this.word.equals(other.word);
  }

  @Override
  public double getOuterPerimeter(){
    double cellLength = cells.get(0).getSideLength();
    return 3 * height * cellLength;
  }

  @Override
  public double maxPerimeter(){
    int x = (height * (height + 1)) / 2;
    return 3 * sideLength * x;
  }

  @Override
  protected void generate(){
    for(int i = 0; i < height; i++){
      for(int j = 0; j < 2*i + 1; j++){
        if(j % 2 == 0){
          Cell c = new Cell(3, 12, new int[]{2, 6, 10}, 0, sideLength);
          //Classic triangle
          cells.add(c);
          int n = cells.size() - 1;

          if(j != 2*i){
            c.addNeighbour(1, cells.get(n - 2*i + 1));
            cells.get(n - 2*i + 1).addNeighbour(7, c);

          }
          if(j != 0){
            c.addNeighbour(11, cells.get(n - 2*i - 1));
            cells.get(n - 2*i - 1).addNeighbour(5, c);
            c.addNeighbour(10, cells.get(n-1));
            cells.get(n-1).addNeighbour(4, c);
            c.addNeighbour(9, cells.get(n - 2));
            cells.get(n - 2).addNeighbour(3, c);
          }
          if(j!= 0 && j!= 2*i){
            c.addNeighbour(0, cells.get(n - 2*i));
            cells.get(n - 2*i).addNeighbour(6, c);
            c.addNeighbour(9, cells.get(n - 2));
            cells.get(n - 2).addNeighbour(3, c);
          }
        }
        else{
          Cell c = new Cell(3, 12, new int[]{0, 4, 8}, 0, sideLength);
          //Triangle on top
          cells.add(c);
          int n = cells.size() - 1;

          c.addNeighbour(0, cells.get(n - 2*i));
          cells.get(n - 2*i).addNeighbour(6, c);
          c.addNeighbour(8, cells.get(n-1));
          cells.get(n-1).addNeighbour(2, c);
          if(j != 2*i - 1){
            c.addNeighbour(1, cells.get(n - 2*i + 1));
            cells.get(n - 2*i + 1).addNeighbour(7, c);
            c.addNeighbour(2, cells.get(n - 2*i + 2));
            cells.get(n - 2*i + 2).addNeighbour(8, c);
          }
          if(j != 1){
            c.addNeighbour(9, cells.get(n - 2));
            cells.get(n - 2).addNeighbour(3, c);
            c.addNeighbour(10, cells.get(n  -2*i - 2));
            cells.get(n - 2*i - 2).addNeighbour(4, c);
            c.addNeighbour(11, cells.get(n - 2*i - 1));
            cells.get(n - 2*i - 1).addNeighbour(5, c);
          }
        }
      }
    }
  }

  @Override
  protected Tiling newInstance(ArrayList<Cell> cells){
    Tiling toReturn = new TriangularTiling(height, sideLength);
    toReturn.cells = cells;
    toReturn.totalArea = this.totalArea;
    toReturn.word = this.word;
    return toReturn;
  }

  private void init2DWord(){
    word = new TwoDWord<>(O);
    TwoDWord<TriangleAlphabet> word = (TwoDWord<TriangleAlphabet>) this.word;
    List<ArrayList<TriangleAlphabet>> changes = new ArrayList<>();
    for(int i = 0; i < height; i++){
      ArrayList<TriangleAlphabet> t = new ArrayList<>();
      for(int j = 0; j < width; j++){
        if(height % 2 == 0){
          if(i % 2 == 0){
            if(Math.floor(width / 2.) - i <= j && j <= Math.floor(width /2.) + i){
              TriangleAlphabet chr =  j % 2 == 1 ? H : B;
              t.add(chr);
            }
            else{
              TriangleAlphabet chr = j % 2 == 1 ? O : C;
              t.add(chr);
            }
          }
          else{
            if(Math.floor(width / 2.) - i <= j && j <= Math.floor(width /2.) + i){
              TriangleAlphabet chr =  j % 2 == 0 ? H : B;
              t.add(chr);
            }
            else{
              TriangleAlphabet chr = j % 2 == 0 ? O : C;
              t.add(chr);
            }
          }
        }
        else{
          if(i % 2 == 0){
            if(Math.floor(width / 2.) - i <= j && j <= Math.floor(width /2.) + i){
              TriangleAlphabet chr =  j % 2 == 0 ? H : B;
              t.add(chr);
            }
            else{
              TriangleAlphabet chr = j % 2 == 0 ? O : C;
              t.add(chr);
            }
          }
          else{
            if(Math.floor(width / 2.) - i <= j && j <= Math.floor(width /2.) + i){
              TriangleAlphabet chr =  j % 2 == 1 ? H : B;
              t.add(chr);
            }
            else{
              TriangleAlphabet chr = j % 2 == 1 ? O : C;
              t.add(chr);
            }
          }
        }
      }
      changes.add(t);
    }
    word.applyVerticalChanges(0, changes);

  }
}
