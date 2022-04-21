package tiling.objective;

import org.jamesframework.core.problems.objectives.Objective;
import org.jamesframework.core.problems.objectives.evaluations.Evaluation;
import org.jamesframework.core.problems.objectives.evaluations.SimpleEvaluation;
import tiling.solution.BuildingInvariant;
import tiling.solution.UrbanBlockData;
import tiling.solution.UrbanBlockSolution;

public class PorosityObjective implements Objective<UrbanBlockSolution, UrbanBlockData>{

  @Override
  public Evaluation evaluate(UrbanBlockSolution urbanBlockSolution, UrbanBlockData urbanBlockData){
    BuildingInvariant inv = (BuildingInvariant) urbanBlockSolution.getInvariants().get(0);
    double totalArea = urbanBlockSolution.getGrid().getTotalArea();
    double buildArea = inv.getBuildArea();
    double porosity = 1 - (buildArea / totalArea);
    return SimpleEvaluation.WITH_VALUE(porosity);
  }

  @Override
  public boolean isMinimizing(){
    return true;
  }
}
