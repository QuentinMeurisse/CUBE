package tiling.constraint;

import org.jamesframework.core.problems.constraints.PenalizingConstraint;
import org.jamesframework.core.problems.constraints.validations.PenalizingValidation;
import org.jamesframework.core.problems.constraints.validations.SimplePenalizingValidation;
import tiling.solution.BuildingInvariant;
import tiling.solution.UrbanBlockData;
import tiling.solution.UrbanBlockSolution;
import util.Tiling;

public class NumBuildingConstraint implements PenalizingConstraint<UrbanBlockSolution, UrbanBlockData>{

  private final int minBuilding, maxBuilding;
  private final boolean normalized;

  public NumBuildingConstraint(int minBuilding, int maxBuilding, boolean normalized){
    this.minBuilding = minBuilding;
    this.maxBuilding = maxBuilding;
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
    BuildingInvariant inv = (BuildingInvariant) urbanBlockSolution.getInvariants().get(0);
    int currentNumBuilding = inv.getNumBuilding();
    Tiling t = urbanBlockSolution.getGrid();
    int maxBuildingPossible = t.numCells();

    if(currentNumBuilding < minBuilding){
      double penalty = (minBuilding - currentNumBuilding) / (minBuilding - 0.);
      return SimplePenalizingValidation.FAILED(penalty);
    }
    else if(currentNumBuilding > maxBuilding){
      double penalty = (currentNumBuilding - maxBuilding) / (maxBuildingPossible - 0.);
      return SimplePenalizingValidation.FAILED(penalty);
    }
    else
      return SimplePenalizingValidation.PASSED;
  }

  private PenalizingValidation basicValidation(UrbanBlockSolution urbanBlockSolution){
    BuildingInvariant inv = (BuildingInvariant) urbanBlockSolution.getInvariants().get(0);
    int currentNumBuilding = inv.getNumBuilding();
    if(currentNumBuilding < minBuilding){
      return SimplePenalizingValidation.FAILED(minBuilding - currentNumBuilding);
    }
    else if(currentNumBuilding > maxBuilding){
      return SimplePenalizingValidation.FAILED(currentNumBuilding - maxBuilding);
    }
    else
      return SimplePenalizingValidation.PASSED;
  }
}