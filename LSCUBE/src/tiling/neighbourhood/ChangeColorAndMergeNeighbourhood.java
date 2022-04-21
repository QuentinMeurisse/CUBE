package tiling.neighbourhood;

import org.jamesframework.core.search.neigh.Move;
import org.jamesframework.core.search.neigh.Neighbourhood;
import tiling.solution.UrbanBlockSolution;
import util.Cell;
import util.Tiling;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class ChangeColorAndMergeNeighbourhood implements Neighbourhood<UrbanBlockSolution>{

  private final int radius;
  private final int maxNegativeColor;

  public ChangeColorAndMergeNeighbourhood(int radius, int maxNegativeColor){
    this.radius = radius;
    this.maxNegativeColor = maxNegativeColor;
  }


  @Override
  public Move<? super UrbanBlockSolution> getRandomMove(UrbanBlockSolution urbanBlockSolution, Random random){
    Tiling t = urbanBlockSolution.getGrid();
    int rndIndex;
    int rndColor;
    boolean cond;
    do{
      rndIndex = random.nextInt(t.numCells());
      rndColor = random.nextInt(19 - maxNegativeColor ) + maxNegativeColor;
      if(rndColor < 0){
        cond = rndColor == t.getCell(rndIndex).getColor();
        cond = cond || t.getCell(rndIndex).isLocked();
      }
      else{
        cond = t.getCell(rndIndex).isALockCellClose();
        cond = cond || rndColor == t.getCell(rndIndex).getColor();
      }
    }while(cond);

    return new ChangeColorAndMergeMove(rndIndex, rndColor, radius);
  }

  @Override
  public List<ChangeColorAndMergeMove> getAllMoves(UrbanBlockSolution urbanBlockSolution){
    ArrayList<ChangeColorAndMergeMove> moves = new ArrayList<>();
    Tiling t = urbanBlockSolution.getGrid();
    for(int k = 0; k < t.numCells(); k++){
      Cell c = t.getCell(k);
      for(int color = maxNegativeColor; color < 19; color++){
        if(c.getColor() != color && !(c.isALockCellClose() && color > 0))
          moves.add(new ChangeColorAndMergeMove(k, color, radius));
      }
    }
    return moves;
  }
}
