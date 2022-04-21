package tiling.constraint;

import org.jamesframework.core.problems.constraints.PenalizingConstraint;
import org.jamesframework.core.problems.constraints.validations.PenalizingValidation;
import org.jamesframework.core.problems.constraints.validations.SimplePenalizingValidation;
import tiling.solution.GreenSpacesInvariant;
import tiling.solution.UrbanBlockData;
import tiling.solution.UrbanBlockSolution;
import util.Tiling;

public class NumGreenSpacesConstraint implements PenalizingConstraint<UrbanBlockSolution, UrbanBlockData>{

  private final int minGreenSpaces, maxGreenSpaces;
  private final boolean normalized;

  public NumGreenSpacesConstraint(int minGreenSpaces, int maxGreenSpaces, boolean normalized){
    this.minGreenSpaces = minGreenSpaces;
    this.maxGreenSpaces = maxGreenSpaces;
    this.normalized = normalized;
  }

  @Override
  public PenalizingValidation validate(UrbanBlockSolution urbanBlockSolution, UrbanBlockData urbanBlockData){
    if(normalized)
      return normalizedValidation(urbanBlockSolution);
    else
      return basicValidation(urbanBlockSolution);
  }

  private PenalizingValidation normalizedValidation(UrbanBlockSolution urbanBlockSolution){
    GreenSpacesInvariant inv = (GreenSpacesInvariant) urbanBlockSolution.getInvariants().get(1);
    int currentNumBuilding = inv.numGreenSpaces();
    Tiling t = urbanBlockSolution.getGrid();
    int greenSpacesPossibles = t.numCells();

    if(currentNumBuilding < minGreenSpaces){
      double penalty = (minGreenSpaces - currentNumBuilding) / (minGreenSpaces - 0.);
      return SimplePenalizingValidation.FAILED(penalty);
    }
    else if(currentNumBuilding > maxGreenSpaces){
      double penalty = (currentNumBuilding - maxGreenSpaces) / (greenSpacesPossibles - 0.);
      return SimplePenalizingValidation.FAILED(penalty);
    }
    else
      return SimplePenalizingValidation.PASSED;
  }

  private PenalizingValidation basicValidation(UrbanBlockSolution urbanBlockSolution){
    GreenSpacesInvariant inv = (GreenSpacesInvariant) urbanBlockSolution.getInvariants().get(1);
    int currentNumBuilding = inv.numGreenSpaces();
    if(currentNumBuilding < minGreenSpaces){
      return SimplePenalizingValidation.FAILED(minGreenSpaces - currentNumBuilding);
    }
    else if(currentNumBuilding > maxGreenSpaces){
      return SimplePenalizingValidation.FAILED(currentNumBuilding - maxGreenSpaces);
    }
    else
      return SimplePenalizingValidation.PASSED;
  }
}
