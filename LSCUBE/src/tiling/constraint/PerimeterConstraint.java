package tiling.constraint;

import tiling.solution.BuildingInvariant;
import tiling.solution.UrbanBlockData;
import org.jamesframework.core.problems.constraints.PenalizingConstraint;
import org.jamesframework.core.problems.constraints.validations.PenalizingValidation;
import org.jamesframework.core.problems.constraints.validations.SimplePenalizingValidation;
import tiling.solution.UrbanBlockSolution;
import util.Tiling;

public class PerimeterConstraint implements PenalizingConstraint<UrbanBlockSolution, UrbanBlockData>{


  private final double minPerimeter, maxPerimeter;
  private final boolean normalized;

  public PerimeterConstraint(double minPerimeter, double maxPerimeter, boolean normalized){
    this.minPerimeter = minPerimeter;
    this.maxPerimeter = maxPerimeter;
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
    double currentPerimeter = inv.getBuildPerimeter();
    Tiling t = urbanBlockSolution.getGrid();
    double worstConfig = t.maxPerimeter();
    if(minPerimeter - currentPerimeter > 1e-10){
      double penalty = (minPerimeter - currentPerimeter) / minPerimeter;
      return SimplePenalizingValidation.FAILED(penalty);
    }
    else if(currentPerimeter - maxPerimeter > 1e-10){
      double penalty = (currentPerimeter - maxPerimeter) / (worstConfig - maxPerimeter);
      return SimplePenalizingValidation.FAILED(penalty);
    }
    else
      return SimplePenalizingValidation.PASSED;
  }

  private PenalizingValidation basicValidation(UrbanBlockSolution urbanBlockSolution){
    BuildingInvariant inv = (BuildingInvariant) urbanBlockSolution.getInvariants().get(0);
    double currentPerimeter = inv.getBuildPerimeter();
    if(minPerimeter - currentPerimeter > 1e-10){
      return SimplePenalizingValidation.FAILED(minPerimeter - currentPerimeter);
    }
    else if(currentPerimeter - maxPerimeter > 1e-10){
      return SimplePenalizingValidation.FAILED(currentPerimeter - maxPerimeter);
    }
    else
      return SimplePenalizingValidation.PASSED;
  }
}
