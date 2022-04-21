package util;

import tiling.solution.UrbanBlockSolution;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.function.Function;

import static util.HexagonAlphabet.*;

public class HexagonalTiling extends Tiling{

  private TwoDGrammar<HexagonAlphabet> grammar;


  public HexagonalTiling(int width, int height, double sideLength){
    this.width = width;
    this.height = height;
    this.sideLength = sideLength;
    word = new TwoDWord<>(B);

    Function<HexagonAlphabet, List<HexagonAlphabet>> f = chr -> {
      List<HexagonAlphabet> toReturn = new ArrayList<>();
      switch(chr){
        case B:
          toReturn = Arrays.asList(B, H);
          break;
        case H:
          toReturn = Arrays.asList(H, B);
          break;
      }
      return toReturn;
    };

    Function<HexagonAlphabet, List<HexagonAlphabet>> g = chr ->{
      List<HexagonAlphabet> toReturn = new ArrayList<>();
      switch(chr){
        case B:
          toReturn = Arrays.asList(B, B);
          break;
        case H:
          toReturn = Arrays.asList(H, H);
          break;
      }
      return toReturn;
    };

    ArrayList<Function<HexagonAlphabet, List<HexagonAlphabet>>> horizontalRules = new ArrayList<>();
    horizontalRules.add(f);
    ArrayList<Function<HexagonAlphabet, List<HexagonAlphabet>>> verticalRules = new ArrayList<>();
    verticalRules.add(g);

    grammar = new TwoDGrammar<>(horizontalRules, verticalRules);

    generate();
    totalArea = cells.size() * getCell(0).getArea();
  }

  @Override
  public int getColorOfCorrespondingLetter(int i, int j){
    int k = i * word.getWidth() + j;
    return cells.get(k).getColor();
  }

  @Override
  public boolean equals(Tiling t){
    if(!(t instanceof  HexagonalTiling))
      return false;
    HexagonalTiling other = (HexagonalTiling) t;
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
    //TODO: changer la formule quand on aura rendu rectangulaire le pavage
    int numSidesOnWidth = 2 * width - 1;
    return 2 * 2 * height * sideLength + 2 * numSidesOnWidth * sideLength;
  }

  @Override
  public double maxPerimeter(){
    double toReturn = getOuterPerimeter();
    int heightBy3 = height / 3;
    int widthBy2 = width / 2;
    if(width % 2 == 1 && height % 3 == 0)
      toReturn += heightBy3 * widthBy2 * sideLength * 6 +
              (heightBy3 - 1) * ((widthBy2 - 1) * sideLength * 6 + 2 * 2 * sideLength);
    else if(width % 2 == 1 && height % 3 == 1)
      toReturn +=  heightBy3 * widthBy2 * sideLength * 6 +
              heightBy3 * ((widthBy2 - 1) * sideLength * 6 + 2 * 2 * sideLength);
    else if(width % 2 == 1 && height % 3 == 2)
      toReturn +=  heightBy3 * widthBy2 * sideLength * 6 +
              heightBy3 * ((widthBy2 - 1) * sideLength * 6 + 2 * 2 * sideLength) +
              widthBy2 * 4;
    else if(width % 2 == 0 && height % 3 == 0)
      toReturn += heightBy3 * ((widthBy2 - 1) * sideLength * 6 + 2 * sideLength) +
              (heightBy3 - 1) * ((widthBy2 - 1) * sideLength * 6 + 2 * sideLength);
    else if(width % 2 == 0 && height % 3 == 1)
      toReturn += 2 * heightBy3 * ((widthBy2 - 1) * sideLength * 6 + 2 * sideLength);
    else
      toReturn += 2 * heightBy3 * ((widthBy2 - 1) * sideLength * 6 + 2 * sideLength)
              + (widthBy2 - 1) * 4 * sideLength;


    return toReturn;
  }

  @Override
  protected void generate(){
    TwoDWord<HexagonAlphabet> word = (TwoDWord<HexagonAlphabet>) this.word;
    for(int i = 0; i < height; i++){
      List<ArrayList<HexagonAlphabet>> b = new ArrayList<>();
      for(int j = 0; j < width; j++){
        Cell c = new Cell(6, 6, sideLength);
        cells.add(c);
        int n = cells.size() - 1;
        if(i == 0 && j != 0){
          List<HexagonAlphabet> a = grammar.applyHorizontalRules(0, word.get(i, j - 1));
          word.applyHorizontalChanges(i, j - 1, a);
        }
        //Very similar to the square tiling
        if(j != 0){
          if(j % 2 == 1){
            c.addNeighbour(4, cells.get(n - 1));
            cells.get(n - 1).addNeighbour(1, c);
          }
          else{
            c.addNeighbour(5, cells.get(n - 1));
            cells.get(n - 1).addNeighbour(2, c);
          }
        }
        if(i != 0){
          c.addNeighbour(0, cells.get(n - width));
          cells.get(n - width).addNeighbour(3, c);
          if(j % 2 == 1){
            c.addNeighbour(5, cells.get(n - width - 1));
            cells.get(n - width - 1).addNeighbour(2, c);
            if(j < width -1){
              c.addNeighbour(1, cells.get(n - width + 1));
              cells.get(n - width + 1).addNeighbour(4, c);
            }
          }

          List<HexagonAlphabet> a = grammar.applyVerticalRules(0, word.get(i - 1, j));
          for(int k = 0; k < a.size(); k++){
            if(j == 0)
              b.add(new ArrayList<>());
            b.get(k).add(a.get(k));
          }
        }
      }

      if(i != 0)
        word.applyVerticalChanges(i-1, b);
    }
  }

  @Override
  protected Tiling newInstance(ArrayList<Cell> cells){
    Tiling toReturn = new HexagonalTiling(width, height, sideLength);
    toReturn.cells = cells;
    toReturn.totalArea = this.totalArea;
    toReturn.word = this.word;
    return toReturn;
  }
}
