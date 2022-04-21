package util;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.function.Function;

import static util.SquareAlphabet.A;

public class SquareTiling extends Tiling{


  private TwoDGrammar<SquareAlphabet> grammar;


  public SquareTiling(int width, int height, double sideLength){
    this.width = width;
    this.height = height;
    this.sideLength = sideLength;
    word = new TwoDWord<>(A);
    Function<SquareAlphabet, List<SquareAlphabet>> f =
            chr -> Arrays.asList(A, A);
    ArrayList<Function<SquareAlphabet, List<SquareAlphabet>>> horizontalRules = new ArrayList<>();
    horizontalRules.add(f);
    ArrayList<Function<SquareAlphabet, List<SquareAlphabet>>> verticalRules = new ArrayList<>();
    verticalRules.add(f);
    grammar = new TwoDGrammar<>(horizontalRules, verticalRules);

    generate(); //When we instantiate a tiling, we must generate it
    totalArea = cells.size() * getCell(0).getArea();

  }


  @Override
  public int getColorOfCorrespondingLetter(int i, int j){
    int k = i * word.getWidth() + j;
    return cells.get(k).getColor();
  }

  @Override
  public boolean equals(Tiling t){
    if(!(t instanceof  SquareTiling))
      return false;
    SquareTiling other = (SquareTiling) t;
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
    return 2 * height * cellLength + 2 * width * cellLength;
  }

  @Override
  public double maxPerimeter(){
    int x = (int) Math.ceil(height / 2.) * width;
    x -=  width % 2 == 1 ? width / 2 : 0;
    return 4 * sideLength * x;
  }

  @Override
  protected void generate(){
    TwoDWord<SquareAlphabet> word = (TwoDWord<SquareAlphabet>) this.word;
    for(int i = 0; i < height; i++){
      List<ArrayList<SquareAlphabet>> b = new ArrayList<>();
      for(int j = 0; j < width; j++){
        Cell c = new Cell(4, 8, new int[]{0, 2, 4, 6}, 0, sideLength);
        cells.add(c);
        int n = cells.size() - 1;
        if(i == 0 && j != 0){ //If we are at the 1-st line but not the first column
          List<SquareAlphabet> a = grammar.applyHorizontalRules(0, word.get(i, j-1));
          word.applyHorizontalChanges(i, j-1, a);
        }
        //Link the cells according to the position in the tiling (if we are in the border for example)
        if(j != 0){
          c.addNeighbour(6, cells.get(n-1));
          cells.get(n-1).addNeighbour(2, c);
        }
        if(i != 0){

          c.addNeighbour(0, cells.get(n - width));
          cells.get(n- width).addNeighbour(4, c);
          if(j != width - 1){
            c.addNeighbour(1, cells.get(n - width + 1));
            cells.get(n - width + 1).addNeighbour(5, c);
          }
          if(j!= 0){
            c.addNeighbour(7, cells.get(n - width  - 1));
            cells.get(n - width - 1).addNeighbour(3, c);
          }
          List<SquareAlphabet> a = grammar.applyVerticalRules(0,word.get(i-1, j));
          for( int k = 0; k < a.size(); k++){
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
    Tiling toReturn = new SquareTiling(width, height, sideLength);
    toReturn.cells = cells;
    toReturn.totalArea = this.totalArea;
    toReturn.word = this.word;
    return toReturn;
  }

}
