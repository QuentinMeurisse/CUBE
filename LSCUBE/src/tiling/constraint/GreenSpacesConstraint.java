package tiling.constraint;

import org.jamesframework.core.problems.constraints.PenalizingConstraint;
import org.jamesframework.core.problems.constraints.validations.PenalizingValidation;
import org.jamesframework.core.problems.constraints.validations.SimplePenalizingValidation;
import tiling.solution.GreenSpacesInvariant;
import tiling.solution.UrbanBlockData;
import tiling.solution.UrbanBlockSolution;

public class GreenSpacesConstraint implements PenalizingConstraint<UrbanBlockSolution, UrbanBlockData>{

  private final double minAreaRate;

  public GreenSpacesConstraint(double minAreaRate){
    this.minAreaRate = minAreaRate;
  }

  @Override
  public PenalizingValidation validate(UrbanBlockSolution urbanBlockSolution, UrbanBlockData urbanBlockData){
    GreenSpacesInvariant greenInv = (GreenSpacesInvariant) urbanBlockSolution.getInvariants().get(1);
    double greenSpacesArea = greenInv.getGreenSpacesArea();
    double totalArea = urbanBlockSolution.getGrid().getTotalArea();
    if((greenSpacesArea / totalArea) <= minAreaRate - 1e-12)
      return SimplePenalizingValidation.FAILED((minAreaRate - (greenSpacesArea / totalArea)) * 100);
    else
      return SimplePenalizingValidation.PASSED;
  }
}
