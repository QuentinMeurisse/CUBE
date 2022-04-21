package tiling.constraint;

import tiling.neighbourhood.ChangeColorAndMergeMove;
import tiling.solution.BuildingInvariant;
import tiling.solution.GroupOfCells;
import tiling.solution.UrbanBlockData;
import org.jamesframework.core.exceptions.IncompatibleDeltaEvaluationException;
import org.jamesframework.core.problems.constraints.Constraint;
import org.jamesframework.core.problems.constraints.validations.SimpleValidation;
import org.jamesframework.core.problems.constraints.validations.Validation;
import org.jamesframework.core.search.neigh.Move;
import tiling.neighbourhood.ChangeColorMove;
import tiling.solution.UrbanBlockSolution;
import util.Cell;
import util.Tiling;

import java.util.ArrayList;
import java.util.HashSet;

public class RadiusConstraint implements Constraint<UrbanBlockSolution, UrbanBlockData>{

  private final int radius;

  public RadiusConstraint(int radius){
    this.radius = radius;
  }


  @Override
  public Validation validate(UrbanBlockSolution urbanBlockSolution, UrbanBlockData urbanBlockData){
    Tiling t = urbanBlockSolution.getGrid();
    for(Cell c : t){
      if(!sameColor(c))
        return SimpleValidation.FAILED;
    }
    return SimpleValidation.PASSED;
  }

  @Override
  public Validation validate(Move move, UrbanBlockSolution curSolution, Validation curValidation, UrbanBlockData data){
    if(move instanceof ChangeColorMove)
      return changeColorValidation((ChangeColorMove) move, curSolution);
    if(move instanceof ChangeColorAndMergeMove)
      return changeColorAndMergeValidation((ChangeColorAndMergeMove) move, curSolution);
    else
      throw new IncompatibleDeltaEvaluationException("Urban block constraint should be use with neighbourhoods" +
              "that works on UrbanBlockSolution ");
  }

  public Validation changeColorValidation(ChangeColorMove move, UrbanBlockSolution sol){
    int k = move.getK();
    move.apply(sol);
    Tiling t = sol.getGrid();
    Cell c = t.getCell(k);
    BuildingInvariant inv = (BuildingInvariant) sol.getInvariants().get(0);
    boolean constraintOk;

    if(move.isRecolored() && move.getNewColor() != 0){
      GroupOfCells b = sol.getComponents(c.getData(), c.getColor());
      for(Cell d : b.getBorder()){
        constraintOk = sameColor(d);
        if(!constraintOk){
          move.undo(sol);
          return SimpleValidation.FAILED;
        }
      }
      move.undo(sol);
      return SimpleValidation.PASSED;
    }
    else{
      constraintOk = sameColor(c);
      move.undo(sol);
      return new SimpleValidation(constraintOk);
    }
  }


  public Validation changeColorAndMergeValidation(ChangeColorAndMergeMove move, UrbanBlockSolution sol){
    move.apply(sol);
    ArrayList<ChangeColorMove> formingMoves = new ArrayList<>(move.getStack());

    if(formingMoves.size() == 1){
      move.undo(sol);
      return changeColorValidation(formingMoves.get(0), sol);
    }
    else{
      boolean constraintOk = true;
      Tiling t = sol.getGrid();
      for(ChangeColorMove ccm : formingMoves){
        Cell c = t.getCell(ccm.getK());
        constraintOk = constraintOk && sameColor(c);
      }
      move.undo(sol);
      return new SimpleValidation(constraintOk);
    }
  }


  private boolean sameColor(Cell c){
    int refColor = c.getColor();
    if(refColor <= 0)
      return true;
    else{
      HashSet<Cell> ball = c.ball(radius);
      for(Cell neigh : ball){
        if(neigh.getColor() > 0 && neigh.getData() != c.getData())
          return false;
      }
      return true;
    }
  }

}

