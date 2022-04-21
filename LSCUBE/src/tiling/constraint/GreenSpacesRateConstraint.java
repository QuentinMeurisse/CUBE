package tiling.constraint;

import org.jamesframework.core.exceptions.IncompatibleDeltaEvaluationException;
import org.jamesframework.core.problems.constraints.PenalizingConstraint;
import org.jamesframework.core.problems.constraints.validations.PenalizingValidation;
import org.jamesframework.core.problems.constraints.validations.SimplePenalizingValidation;
import org.jamesframework.core.problems.constraints.validations.Validation;
import org.jamesframework.core.search.neigh.Move;
import tiling.neighbourhood.ChangeColorAndMergeMove;
import tiling.neighbourhood.ChangeColorMove;
import tiling.solution.GreenSpacesInvariant;
import tiling.solution.UrbanBlockData;
import tiling.solution.UrbanBlockSolution;
import util.Cell;
import util.Tiling;
import util.Utils;

import java.util.ArrayList;
import java.util.Set;

public class GreenSpacesRateConstraint implements PenalizingConstraint<UrbanBlockSolution, UrbanBlockData>{

  private final double rate;
  private final int maxNumGreenSpaces;

  public GreenSpacesRateConstraint(double rate, int maxNumGreenSpaces){
    this.rate = rate;
    this.maxNumGreenSpaces = maxNumGreenSpaces;
  }

  @Override
  public PenalizingValidation validate(UrbanBlockSolution urbanBlockSolution, UrbanBlockData urbanBlockData){
    GreenSpacesInvariant greenInv = (GreenSpacesInvariant) urbanBlockSolution.getInvariants().get(1);
    double greenSpacesArea = greenInv.getGreenSpacesArea();
    if(greenInv.numGreenSpaces() <= maxNumGreenSpaces)
      return SimplePenalizingValidation.PASSED;
    else{
      Set<Integer> ids = greenInv.getGreenSpaces().keySet();
      ArrayList<ArrayList<Integer>> combinations = Utils.generateNTuples(ids, maxNumGreenSpaces);
      for(ArrayList<Integer> tuple : combinations){
        double sum = 0.;
        for(int id : tuple){
          sum += greenInv.getGreenSpaceOfId(id).getArea();
        }
        if((sum / greenSpacesArea) >= rate - 1e-12)
          return SimplePenalizingValidation.PASSED;

      }
    }
    return SimplePenalizingValidation.FAILED(1.);
  }

  @Override
  public PenalizingValidation validate(Move move, UrbanBlockSolution curSolution, Validation curValidation, UrbanBlockData data){
    if(move instanceof ChangeColorMove)
      return changeColorValidation((ChangeColorMove) move, curSolution, (PenalizingValidation) curValidation);
    else if(move instanceof ChangeColorAndMergeMove)
      return changeColorAndMergeValidation((ChangeColorAndMergeMove) move, curSolution,
              (PenalizingValidation) curValidation);
    else
      throw new IncompatibleDeltaEvaluationException("Urban block constraint should be use with neighbourhoods" +
              "that works on UrbanBlockSolution ");
  }

  private PenalizingValidation changeColorValidation(ChangeColorMove move, UrbanBlockSolution sol,
                                                     PenalizingValidation curValidation){

    Tiling t = sol.getGrid();
    Cell c = t.getCell(move.getK());
    if(c.getColor() != -1 &&  move.getNewColor() != -1) //If the move
      return curValidation;
    else{
      move.apply(sol);
      PenalizingValidation pv =  validate(sol, new UrbanBlockData());
      move.undo(sol);
      return pv;
    }
  }

  private PenalizingValidation changeColorAndMergeValidation(ChangeColorAndMergeMove move, UrbanBlockSolution sol,
                                                             PenalizingValidation curValidation){
    Tiling t = sol.getGrid();
    Cell c = t.getCell(move.getK());
    if(c.getColor() != -1 && move.getNewColor() != -1) //If the move
      return curValidation;
    else{
      move.apply(sol);
      PenalizingValidation pv = validate(sol, new UrbanBlockData());
      move.undo(sol);
      return pv;
    }
  }
}
