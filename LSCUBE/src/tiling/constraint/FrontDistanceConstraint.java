package tiling.constraint;

import org.jamesframework.core.exceptions.IncompatibleDeltaEvaluationException;
import org.jamesframework.core.problems.constraints.Constraint;
import org.jamesframework.core.problems.constraints.validations.SimpleValidation;
import org.jamesframework.core.problems.constraints.validations.Validation;
import org.jamesframework.core.search.neigh.Move;
import tiling.solution.UrbanBlockData;
import tiling.neighbourhood.ChangeColorMove;
import tiling.solution.UrbanBlockSolution;
import util.Cell;
import util.Tiling;

import java.util.HashMap;

@Deprecated
public class FrontDistanceConstraint implements Constraint<UrbanBlockSolution, UrbanBlockData>{

  //Specify for each neighbour the min distance between the current cell and another building
  public HashMap<Integer, Integer> distances;

  public FrontDistanceConstraint(HashMap<Integer, Integer> distances){
    this.distances = distances;
  }

  @Override
  public Validation validate(UrbanBlockSolution urbanBlockSolution, UrbanBlockData urbanBlockData){
    Tiling grid = urbanBlockSolution.getGrid();
    for(Cell c : grid){
      if(!(noObstacle(c)))
        return SimpleValidation.FAILED;
    }
    return SimpleValidation.PASSED;
  }

  @Override
  public Validation validate(Move move, UrbanBlockSolution curSolution, Validation curValidation, UrbanBlockData data){
    if(move instanceof ChangeColorMove)
      return changeColorValidation((ChangeColorMove) move, curSolution);
    else{
       throw new IncompatibleDeltaEvaluationException("Island constraint should be use with neighbourhoods" +
              "that generate IslandMove");
    }
  }
  private Validation changeColorValidation(ChangeColorMove move, UrbanBlockSolution sol){
    int k = move.getK();
    boolean constraintOk = true;
    if(!(move.isRecolored())){
      move.apply(sol);
      constraintOk = sameColor(sol.getGrid().getCell(k));
      move.undo(sol);
    }
    return new SimpleValidation(constraintOk);
  }

  private boolean noObstacle(Cell c){
    int data = c.getData();
    for(int key : c.getNeighbours().keySet()){
      int d = distances.get(key);
      for(int x = 1; x <= d; x++){
        int neighData = c.exploreNeighbours(key, x);
        if(neighData != 0 && neighData != data)
          return false;
      }
    }
    return true;
  }

  private boolean sameColor(Cell c){
    int color = c.getColor();
    for(int key : c.getNeighbours().keySet()){
      int d = distances.get(key);
      for(int x = 1; x <= d; x++){
        int neighColor = c.exploreColor(key, x);
        if(neighColor != 0 && neighColor != color)
          return false;
      }
    }
    return true;
  }
}
